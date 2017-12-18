//===--	SudokuTests/Solver_set_option.cpp								--===//
//
//===---------------------------------------------------------------------===//
// Implemented with GoogleTest
//
// Notes:
// gTest is limited for use with multiple template parameters.
// These expressions need to be implemented between extra parentheses
// - test elements implementing this are flagged with [gTest]
// - not implemented tests are flagged as NEEDTEST [gTest]
// gTest tries to print iterators if they use inheritance,
//   if used in *_EQ/NE etc.
//   use an explicit test like EXPECT_TRUE(.. == ..).
//
//===---------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Solver.h>
// helpers
#include <Sudoku/Board.h>
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>
#include <Sudoku/Solver.h> // setvalue
// Debug Output
#include "print_Options.h"
// library
#include <bitset>
#include <vector>
#include <type_traits>


using namespace Sudoku;

namespace SudokuTests::SolversTest
{
TEST(Solver, setValue)
{
	using L = Location<2>;
	// clang-format off
	//	4		1234	3		1234
	//	134		1234	1234	1234
	//	1234	1234	1234	1234
	//	1234	1234	1234	4
	// clang-format on

	// Set single value
	Board<Options<4>, 2> board;
	ASSERT_EQ(board[1][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(setValue(board, L(2), Value{3}));
	EXPECT_EQ(board[0][2], Value{3});
	EXPECT_NO_THROW(setValue(board, L(0), Value{4}));
	EXPECT_EQ(board[0][0], Value{4});
	// test: already set to THIS answer (allow to overwrite with same)
	EXPECT_NO_THROW(setValue(board, L(15), Value{4}));
	ASSERT_EQ(board[3][3], Value{4});
	EXPECT_NO_THROW(setValue(board, L(3, 3), Value{4}));
	// test: value is not an option
	board[1][1] = std::bitset<5>{"11011"}; // options: 1,3,4
	EXPECT_THROW(setValue(board, L(1, 1), Value{2}), std::logic_error);
	// test: already set to another answer
	EXPECT_THROW(setValue(board, L(0), Value{1}), std::logic_error);

	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,2, 0,0,	// 1	2	3	4
		4,0, 0,0,	// 4	3	1,2	1,2
		0,1, 4,0,	// 2,3	1	4	2,3
		0,0, 0,0	// 2,3	4	1,2	1,2,
	}; // clang-format on
	// Copy data from vector
	Board<Options<4>, 2> B2;
	EXPECT_NO_THROW(setValue(B2, v1.cbegin(), v1.cend()));
	EXPECT_EQ(B2[0][1], Value{2});
	EXPECT_EQ(B2[1][0], Value{4});
	EXPECT_EQ(B2[2][1], Value{1});
	EXPECT_EQ(B2[2][2], Value{4});
	// check if processed single option cells:
	EXPECT_EQ(B2[0][3], Value{4});
	EXPECT_EQ(B2[3][1], Value{4});
	// from vector didn't cascade over single option cells
	EXPECT_EQ(B2[0][0], Value{1});
	EXPECT_EQ(B2[0][2], Value{3});
	EXPECT_EQ(B2[1][1], Value{3});
	EXPECT_EQ(B2[3][1], Value{4});
}
TEST(Solver, set_section_locals)
{
	// called by: section_exclusive
	//	find and process values appearing 2 or 3x in row/col

	// int set_section_locals(section, rep_count, worker)
	using set = std::bitset<5>;
	Board<Options<4>, 2> B{};
	Board<Options<4>, 2> cB{B};
	//
	//	12		1234	12		1234
	//	1234	1234	1234	1234
	//	34		34		1234	1234
	//	1234	1234	1234	1234
	//
	// Row
	//	2 values in row, not in same block
	B[0][0] = set{"00111"};
	B[0][2] = set{"00111"};
	Options<4> worker{set{"11001"}};
	ASSERT_TRUE(B[0][1].all());
	ASSERT_TRUE(B[0][3].all());
	EXPECT_EQ(set_section_locals(B, B.row(0), 2, worker), 0);
	EXPECT_EQ(B[0][0].count(), 2);
	EXPECT_TRUE(B[0][1].all());
	EXPECT_EQ(B[0][2].count(), 2);
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][0].all());
	EXPECT_TRUE(B[1][1].all());
	//	2 values in row, and in same block
	B[2][0] = set{"11001"};
	B[2][1] = set{"11001"};
	worker  = set{"00111"};
	ASSERT_TRUE(B[2][2].all());
	ASSERT_TRUE(B[2][3].all());
	ASSERT_TRUE(B[3][2].all());
	EXPECT_EQ(set_section_locals(B, B.row(2), 2, worker), 4);
	EXPECT_TRUE(B[3][0].all());
	EXPECT_EQ(B[2][2].count(), 4); // self
	EXPECT_EQ(B[2][3].count(), 4); // self
	EXPECT_EQ(B[3][2].count(), 2); // rest block
	EXPECT_EQ(B[1][2].count(), 4);
	// Col
	//
	//	12		1234	1234	1234
	//	1234	1234	1234	1234
	//	12		1234	1234	34
	//	1234	1234	1234	34
	//
	B       = cB; // reset board
	B[0][0] = set{"00111"};
	B[2][0] = set{"00111"};
	worker  = set{"11001"};
	EXPECT_EQ(set_section_locals(B, B.col(0), 2, worker), 0);
	//
	B[2][3] = set{"11001"};
	B[3][3] = set{"11001"};
	worker  = set{"00111"};
	EXPECT_EQ(set_section_locals(B, B.row(2), 2, worker), 4);
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_TRUE(B[0][1].all());
	// Block
	//
	//	12		1234	1234	1234
	//	1234	12		1234	1234
	//	34		34		1234	1234
	//	1234	1234	1234	1234
	//
	B       = cB; // reset board
	B[0][0] = set{"00111"};
	B[1][1] = set{"00111"};
	worker  = set{"11001"};
	EXPECT_EQ(set_section_locals(B, B.block(0), 2, worker), 0);
	EXPECT_TRUE(B[0][1].all());
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][0].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_TRUE(B[3][0].all());
	// same block, same row
	B[2][0] = set{"11001"};
	B[2][1] = set{"11001"};
	worker  = set{"00111"};
	EXPECT_EQ(set_section_locals(B, B.block(2), 2, worker), 4);
	EXPECT_TRUE(B[3][0].all());    // self block
	EXPECT_TRUE(B[3][1].all());    // self block
	EXPECT_EQ(B[3][2].count(), 2); // rest row
	EXPECT_EQ(B[3][3].count(), 2); // rest row
	EXPECT_TRUE(B[0][1].all());    // rest col
	EXPECT_TRUE(B[0][2].all());
	EXPECT_TRUE(B[0][3].all());
	// same block, same col
	B       = cB; // reset board
	B[0][1] = set{"00111"};
	B[1][1] = set{"00111"};
	worker  = set{"11001"};
	EXPECT_EQ(set_section_locals(B, B.block(0), 2, worker), 4);
	EXPECT_TRUE(B[0][0].all());    // self block
	EXPECT_EQ(B[0][1].count(), 2); // self block
	EXPECT_TRUE(B[0][2].all());    // rest row
	EXPECT_TRUE(B[0][3].all());    // rest row
	EXPECT_TRUE(B[1][0].all());    // self block
	EXPECT_EQ(B[1][1].count(), 2); // self block
	EXPECT_TRUE(B[1][2].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_EQ(B[2][0].count(), 2); // rest col
	EXPECT_TRUE(B[2][1].all());
	EXPECT_TRUE(B[2][2].all());
	EXPECT_TRUE(B[2][3].all());
	EXPECT_EQ(B[3][0].count(), 2); // rest col
	EXPECT_TRUE(B[3][1].all());
	EXPECT_TRUE(B[3][2].all());
	EXPECT_TRUE(B[3][3].all());
	Board<Options<9>, 3> B3{};
	const Board<Options<9>, 3> cB3{};
	Options<9> worker3{};
	// 3 option in same block & row
	//
	//	0	0	0
	//	4	5	6
	//	7	8	9
	//
	B3[1][0] = Value{4};
	B3[1][1] = Value{5};
	B3[1][2] = Value{6};
	B3[2][0] = Value{7};
	B3[2][1] = Value{8};
	B3[2][2] = Value{9};
	ASSERT_TRUE(B3[0][0].all());
	ASSERT_TRUE(B3[0][5].all());
	ASSERT_TRUE(B3[1][0].is_answer(Value{4}));
	ASSERT_TRUE(B3[1][8].all());
	worker3 = std::bitset<10>{"0000001111"};
	EXPECT_EQ(set_section_locals(B3, B3.block(0), 3, worker3), 3 * 6);
	EXPECT_EQ(B3[0][0].count(), 9);
	EXPECT_EQ(B3[0][3].count(), 6);
	EXPECT_EQ(B3[0][8].count(), 6);
	EXPECT_TRUE(B3[1][5].all());
	EXPECT_TRUE(B3[2][7].all());
	EXPECT_TRUE(B3[3][0].all());
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_TRUE(B3[3][2].all());
	EXPECT_TRUE(B3[8][2].all());
	EXPECT_TRUE(B3[5][5].all());
	// 3 options in same block & col
	//
	//	0	2	3
	//	0	5	6
	//	0	8	9
	//
	B3       = cB3;
	B3[0][1] = Value{2};
	B3[0][2] = Value{3};
	B3[1][1] = Value{5};
	B3[1][2] = Value{6};
	B3[2][1] = Value{8};
	B3[2][2] = Value{9};
	worker3  = std::bitset<10>{"0010010011"};
	EXPECT_EQ(set_section_locals(B3, B3.block(0), 3, worker3), 3 * 6);
	EXPECT_EQ(B3[0][0].count(), 9);
	EXPECT_TRUE(B3[0][4].all()); // rest row
	EXPECT_TRUE(B3[0][8].all()); // rest row
	EXPECT_TRUE(B3[1][0].all()); // self
	EXPECT_TRUE(B3[1][1].is_answer(Value{5}));
	EXPECT_EQ(B3[3][0].count(), 6); // rest col
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_EQ(B3[4][0].count(), 6); // rest col
	EXPECT_EQ(B3[8][0].count(), 6); // rest col
	EXPECT_TRUE(B3[8][1].all());
	// 2 in same block & col, one in different col
	//
	//	0	2	3
	//	0	5	6
	//	7	0	9
	//
	B3       = cB3;
	B3[0][1] = Value{2};
	B3[0][2] = Value{3};
	B3[1][1] = Value{5};
	B3[1][2] = Value{6};
	B3[2][0] = Value{7};
	B3[2][2] = Value{9};
	worker3  = std::bitset<10>{"0100010011"};
	EXPECT_EQ(set_section_locals(B3, B3.block(0), 3, worker3), 0);
	EXPECT_TRUE(B3[0][0].all()); // self
	EXPECT_TRUE(B3[0][4].all()); // rest row
	EXPECT_TRUE(B3[0][8].all()); // rest row
	EXPECT_TRUE(B3[1][0].all()); // self
	EXPECT_TRUE(B3[1][1].is_answer(Value{5}));
	EXPECT_TRUE(B3[2][1].all()); // self
	EXPECT_TRUE(B3[3][0].all()); // rest col
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_TRUE(B3[4][0].all()); // rest col
	EXPECT_TRUE(B3[8][0].all()); // rest col
	EXPECT_TRUE(B3[8][1].all());
}

TEST(Solver, set_uniques)
{
	// Solver<N>::set_uniques(const SectionT section, const Options& worker)
	//===-----------------------------------------------------------------===//
	// processing only one value per section.
	// also in the test: appearance_sets and appearance_once
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	//
	Board<Options<4>, 2> B1{};
	const std::vector<int> v1{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
	setValue(B1, v1.cbegin(), v1.cend());
	const Board<Options<4>, 2> cB1{B1}; // to reset B1
	// verify setup
	ASSERT_EQ(B1[0][0].count(), 3);
	ASSERT_EQ(B1[0][1].count(), 3);
	ASSERT_EQ(B1[0][2].count(), 3);
	ASSERT_EQ(B1[0][3].count(), 4);
	ASSERT_EQ(B1[2][1].count(), 4);

	// worker empty -> return 0, no execution
	EXPECT_EQ(
		set_uniques(B1, B1.row(0), Options<4>{std::bitset<5>{"00001"}}), 0);
	EXPECT_EQ(B1[0][0].count(), 3);
	EXPECT_EQ(B1[0][3].count(), 4);

	// Row (one value)
	auto worker = appearance_once<2>(B1.row(0));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_NO_FATAL_FAILURE(set_uniques(B1, B1.row(0), worker));
	EXPECT_EQ(B1[0][0].count(), 3); // no change
	EXPECT_EQ(B1[0][1].count(), 3); // no change
	EXPECT_EQ(B1[0][2].count(), 3); // no change
	EXPECT_TRUE(B1[0][3].is_answer(Value{1}));
	worker = appearance_once<2>(B1.row(2));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_EQ(set_uniques(B1, B1.row(2), worker), 1);
	EXPECT_TRUE(B1[2][1].is_answer(Value{1}));
	// Col (one value)
	B1 = cB1; // reset
	ASSERT_EQ(B1[0][3].count(), 4);
	ASSERT_EQ(B1[2][1].count(), 4);
	worker = appearance_once<2>(B1.col(1));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_EQ(set_uniques(B1, B1.col(1), worker), 1);
	EXPECT_EQ(B1[0][1].count(), 3); // no change
	EXPECT_EQ(B1[1][1].count(), 3); // no change
	EXPECT_TRUE(B1[2][1].is_answer(Value{1}));
	EXPECT_EQ(B1[3][1].count(), 3); // no change
	// Block (one value)
	B1 = cB1; // reset
	ASSERT_EQ(B1[2][1].count(), 4);
	worker = appearance_once<2>(B1.block(2));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_EQ(set_uniques(B1, B1.block(2), worker), 1);
	EXPECT_EQ(B1[2][0].count(), 3); // no change
	EXPECT_TRUE(B1[2][1].is_answer(Value{1}));
	EXPECT_EQ(B1[3][0].count(), 3); // no change
	EXPECT_EQ(B1[3][1].count(), 3); // no change

	//===-----------------------------------------------------------------===//
	// multiple values in a single go
	// 12	24	324	24
	Board<Options<4>, 2> B2{};
	B2[0][0] = std::bitset<5>{"00111"};
	B2[0][1] = std::bitset<5>{"10101"};
	B2[0][2] = std::bitset<5>{"11101"};
	B2[0][3] = std::bitset<5>{"10101"};
	worker   = appearance_once<2>(B2.row(0));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"01011"}});
	EXPECT_EQ(set_uniques(B2, B2.row(0), worker), 10);
	// 10 = ans(0){1} + col(0){3} + block(0){1}
	//		ans(2){1} + col(2){3} + block(1){1}
	EXPECT_TRUE(B2[0][0].is_answer(Value{1}));
	EXPECT_TRUE(B2[0][2].is_answer(Value{3}));
	EXPECT_EQ(B2[0][1].count(), 2); // no change
	EXPECT_EQ(B2[0][3].count(), 2); // no change
	EXPECT_FALSE(B2[3][0].is_option(Value{1}));
}

TEST(Solver, deathtest_set_option)
{
	Board<Options<4>, 2> B{};

	// SetValue(Itr, Itr)
	const std::vector<int> v1(10);
	const std::vector<int> v2(18);
	// input too short / too long
	EXPECT_DEBUG_DEATH(
		setValue(B, v1.cbegin(), v1.cend()), "Assertion failed:");
	EXPECT_DEBUG_DEATH(
		setValue(B, v2.cbegin(), v2.cend()), "Assertion failed:");

	// set_uniques
	//{
	//	Board<Options<4>, 2> B1{};
	//	// deathtest: a unique Value in worker doesn't exist in the section
	//	// 1	24	324	24
	//	B1[0][0] = std::bitset<5>{"00010"}; // ans 1
	//	EXPECT_FALSE(B1[0][0].is_option(Value{1}));
	//	B1[0][1]    = std::bitset<5>{"10101"};
	//	B1[0][2]    = std::bitset<5>{"11101"};
	//	B1[0][3]    = std::bitset<5>{"10101"};
	//	auto worker = Options<4>{std::bitset<5>{"01011"}};
	//	EXPECT_DEBUG_DEATH(
	//		set_uniques(B1, B1.row(0), worker), "Assertion failed: false");
	//}

	// set_section_locals()
	{
		using set = std::bitset<5>;
		// worker empty
		Options<4> worker{set{"00000"}};
		ASSERT_TRUE(worker.is_empty());
		EXPECT_DEBUG_DEATH(set_section_locals(B, B.row(0), 2, worker), "");
		EXPECT_DEBUG_DEATH(set_section_locals(B, B.block(0), 2, worker), "");
		// worker all set
		worker = set{"11111"};
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.col(2), 4, worker), "rep_count <= N");
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.block(2), 4, worker), "rep_count <= N");
		// 1 value
		worker = set{"00101"};
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.col(2), 1, worker), "rep_count > 1");
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.block(3), 1, worker), "rep_count > 1");
	}
	// set_section_locals(section, rep_count, worker)
	{
		// worker.count_all() = 0
		Options<4> worker{Value{0}};
		EXPECT_DEBUG_DEATH(set_section_locals(B, B.row(0), 1, worker), "count");
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.block(0), 1, worker), "count");
		worker = std::bitset<5>{"00001"};
		EXPECT_DEBUG_DEATH(set_section_locals(B, B.col(0), 1, worker), "count");
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.block(0), 1, worker), "count");
	}
	{
		// option doesn't excist in section
		using set = std::bitset<5>;
		Board<Options<4>, 2> B5{};
		B5[0][0] = set{"00100"}; // ans 2
		B5[0][1] = set{"11111"};
		B5[0][2] = set{"10011"};
		B5[0][3] = set{"11011"};
		B5[1][0] = set{"11011"};
		B5[1][1] = set{"11011"};
		const Options<4> worker{Value{2}};
		ASSERT_TRUE(worker[Value{2}]);
		EXPECT_DEBUG_DEATH(
			set_section_locals(B5, B5.block(0), 2, worker),
			"Assertion failed: changes > 0");
		EXPECT_DEBUG_DEATH(
			set_section_locals(B5, B5.row(0), 2, worker),
			"Assertion failed: changes > 0");
	}
}

} // namespace SudokuTests::SolversTest
