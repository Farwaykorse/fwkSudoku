//===--- SudokuTests/Solver_find.cpp                                    ---===//
//
//===----------------------------------------------------------------------===//
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
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Solvers_find.h>
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
TEST(Solver, find_locations)
{
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};
	// row/col/block
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.row(0), Value{1}, 3));
	EXPECT_EQ(list.size(), 3);
	EXPECT_EQ(list[0], loc(1));
	EXPECT_EQ(list[1], loc(2));
	EXPECT_EQ(list[2], loc(3));
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.col(0), Value{1}, 3));
	EXPECT_EQ(list.size(), 3);
	EXPECT_EQ(list[0], loc(1, 0));
	EXPECT_EQ(list[1], loc(2, 0));
	EXPECT_EQ(list[2], loc(3, 0));
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.block(0), Value{1}, 2));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[0], loc(0, 1));
	EXPECT_EQ(list[1], loc(1, 0));
	// incorrect rep_count: too low
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.row(0), Value{1}, 2));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[1], loc(2));
	// incorrect rep_count: too high
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.block(0), Value{1}, 3));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[1], loc(1, 0));
	EXPECT_NO_FATAL_FAILURE(
		find_locations<2>(B.row(0).cbegin(), B.row(0).cend(), Value{3}, 5));
	// no rep_count
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.row(0), Value{1}));
	EXPECT_EQ(list.size(), 3);
	EXPECT_EQ(list[0], loc(1));
	EXPECT_EQ(list[1], loc(2));
	EXPECT_EQ(list[2], loc(3));
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.block(0), Value{1}));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[1], loc(1, 0));
	// partial section
	EXPECT_NO_FATAL_FAILURE(
		list = find_locations<2>(
			B.row(0).cbegin() + 2, B.row(0).cend(), Value{1}, 2));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[0], loc(2));
	EXPECT_EQ(list[1], loc(3));

	// find Options
	const Options<4> value{set{"10011"}};
	EXPECT_NO_FATAL_FAILURE(list = find_locations<2>(B.row(0), value));
	EXPECT_EQ(list.size(), 1);
	EXPECT_EQ(list[0], loc(2));
}
TEST(Solver, appearance_once)
{
	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,0, 1,0,	// 
		1,0, 0,0,	// 
		0,1, 0,0,	// 
		0,0, 0,0	//					//	0	0	0	1
	}; // clang-format on
	Board<Options<4>, 2> B1;
	Options<4> result{};

	// Using iterators
	EXPECT_NO_THROW(setValue(B1, v1.cbegin(), v1.cend()));
	EXPECT_EQ(B1[3][3].count(), 4);
	EXPECT_TRUE(is_option(B1[3][3], Value{1}));
	// on row
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.row(3).cbegin(), B1.row(3).cend()));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// on col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.col(3).cbegin(), B1.col(3).cend()));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// on block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.block(3).cbegin(), B1.block(3).cend()));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// nothing
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.row(0).cbegin(), B1.row(0).cend()));
	EXPECT_EQ(result.count_all(), 0);
	EXPECT_FALSE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed

	// Using Sections
	// Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(3)));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// Col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.col(3)));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// Block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.block(3)));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// nothing, Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(0)));
	EXPECT_EQ(result.count_all(), 0);
	EXPECT_FALSE(result.test(Value{1}));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	//===------------------------------------------------------------------===//
	// also in the test: appearance_sets
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	{
		Board<Options<4>, 2> B4{};
		const std::vector<int> v4{
			0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
		setValue(B4, v4.cbegin(), v4.cend());
		EXPECT_EQ(B4[0][0].count(), 3);
		EXPECT_EQ(B4[0][1].count(), 3);
		EXPECT_EQ(B4[0][2].count(), 3);
		EXPECT_EQ(B4[0][3].count(), 4);
		EXPECT_EQ(B4[2][1].count(), 4);
		auto result4 = appearance_once<2>(B4.row(0));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[Value{1}]);
		result4 = appearance_once<2>(B4.row(1));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 0);
		EXPECT_TRUE(result4.is_empty());
		result4 = appearance_once<2>(B4.row(2));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[Value{1}]);
		result4 = appearance_once<2>(B4.col(3));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[Value{1}]);
		result4 = appearance_once<2>(B4.block(1));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[Value{1}]);
	}
}

TEST(Solvers_, appearance_sets)
{
	// Example as shown in implementation
	Board<Options<9>, 3> board{};
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][1] = std::bitset<10>{"1101000101"};
	board[0][2] = std::bitset<10>{"0100000111"};
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][6] = std::bitset<10>{"1011001101"};
	board[0][7] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"};
	const Options<9> Ans_0{std::bitset<10>{"0000000001"}}; // complete, 1 answer
	const Options<9> Ans_1{std::bitset<10>{"0000100001"}}; // 5 and 1 answer
	const Options<9> Ans_2{std::bitset<10>{"0010000001"}}; // 7, <2 answers
	const Options<9> Ans_3{std::bitset<10>{"0100001001"}}; // 3 8, <3 answers

	// Using iterators
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// reorder, should give same result
	// set answer last
	board[0][8] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][5] = std::bitset<10>{"0100001011"};
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<10>{"1001100101"};
	board[0][7] = std::bitset<10>{"0000010000"}; // ans 4
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set value appearing 3 times last
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][7] = std::bitset<10>{"1101000101"}; // 1 -> 7
	board[0][6] = std::bitset<10>{"0100000111"}; // 2 -> 6
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][2] = std::bitset<10>{"1011001101"};
	board[0][1] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"}; // 8 -> 8
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// Using Sections
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][1] = std::bitset<10>{"1101000101"};
	board[0][2] = std::bitset<10>{"0100000111"};
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][6] = std::bitset<10>{"1011001101"};
	board[0][7] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"};
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// reorder, should give same result
	// set answer last
	board[0][8] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][5] = std::bitset<10>{"0100001011"};
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<10>{"1001100101"};
	board[0][7] = std::bitset<10>{"0000010000"}; // ans 4
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===------------------------------------------------------------------===//
	// Col
	// Example as shown in implementation
	Board<Options<9>, 3> B2{};
	B2[0][0] = std::bitset<10>{"1000000011"};
	B2[1][0] = std::bitset<10>{"1101000101"};
	B2[2][0] = std::bitset<10>{"0100000111"};
	B2[3][0] = std::bitset<10>{"0000000111"};
	B2[4][0] = std::bitset<10>{"1011001111"};
	B2[5][0] = std::bitset<10>{"0000010000"}; // ans 4
	B2[6][0] = std::bitset<10>{"1011001101"};
	B2[7][0] = std::bitset<10>{"1001100101"};
	B2[8][0] = std::bitset<10>{"0100001011"};
	{ // Using iterators
		const auto result =
			appearance_sets<3>(B2.col(0).cbegin(), B2.col(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	{ // using Col
		const auto result = appearance_sets<3>(B2.col(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===------------------------------------------------------------------===//
	// Block
	using B = Location_Block<3>;
	Board<Options<9>, 3> B3{};
	B3[B{2, 0}] = std::bitset<10>{"1000000011"};
	B3[B(2, 1)] = std::bitset<10>{"1101000101"};
	B3[B(2, 2)] = std::bitset<10>{"0100000111"};
	B3[B(2, 3)] = std::bitset<10>{"0000000111"};
	B3[B(2, 4)] = std::bitset<10>{"1011001111"};
	B3[B(2, 5)] = std::bitset<10>{"0000010000"}; // ans 4
	B3[B(2, 6)] = std::bitset<10>{"1011001101"};
	B3[B(2, 7)] = std::bitset<10>{"1001100101"};
	B3[B(2, 8)] = std::bitset<10>{"0100001011"};
	{ // using iterators
		const auto result =
			appearance_sets<3>(B3.block(2).cbegin(), B3.block(2).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	{ // using Block
		const auto result = appearance_sets<3>(B3.block(2));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===------------------------------------------------------------------===//
	// also in the test: appearance_once
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	{
		Board<Options<4>, 2> B4{};
		const std::vector<int> v4{
			0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
		setValue(B4, v4.cbegin(), v4.cend());
		EXPECT_EQ(B4[0][0].count(), 3);
		EXPECT_EQ(B4[0][1].count(), 3);
		EXPECT_EQ(B4[0][2].count(), 3);
		EXPECT_EQ(B4[0][3].count(), 4);
		EXPECT_EQ(B4[2][1].count(), 4);
		auto result = appearance_sets<2>(B4.row(0));
		EXPECT_EQ(result.size(), 3); // max = N
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_TRUE(result[1][Value{0}]); // answer bit
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][Value{1}]);
		EXPECT_EQ(result[2].count(), 0);
		result = appearance_sets<2>(B4.row(1));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 0);
		EXPECT_TRUE(result[1].is_empty());
		EXPECT_TRUE(result[2].is_empty());
		result = appearance_sets<2>(B4.row(2));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][Value{1}]);
		result = appearance_sets<2>(B4.col(3));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][Value{1}]);
		result = appearance_sets<2>(B4.block(1));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][Value{1}]);
	}
}

TEST(Solver, deathtest_find)
{
	// find_locations()
	{
		using set = std::bitset<5>;
		using loc = Location<2>;
		Board<Options<4>, 2> B5{};
		B5[0][0] = set{"00100"}; // ans 2
		B5[0][1] = set{"11011"};
		B5[0][2] = set{"10011"};
		B5[0][3] = set{"11011"};
		B5[1][0] = set{"01011"};
		B5[1][1] = set{"11001"};
		B5[2][0] = set{"11011"};
		B5[3][0] = set{"10011"};
		std::vector<loc> list{};
		// rep_count = 0
		EXPECT_DEBUG_DEATH(
			find_locations<2>(B5.row(0), Value{3}, 0),
			"Assertion failed: rep_count > 0");
		// section -> rep_count > elem_size
		EXPECT_DEBUG_DEATH(
			find_locations<2>(B5.row(0), Value{3}, 5),
			"Assertion failed: .* <= elem_size");
		// itr -> rep_count > full_size
		EXPECT_DEBUG_DEATH(
			find_locations<2>(
				B5.row(0).cbegin(), B5.row(0).cend(), Value{3}, 17),
			"Assertion failed: .* <= full_size");
		EXPECT_DEBUG_DEATH(
			find_locations<2>(B5.row(0).cbegin(), B5.row(0).cend(), Value{2}),
			"Assertion failed: not.*empty..");
	}
}

} // namespace SudokuTests::SolversTest
