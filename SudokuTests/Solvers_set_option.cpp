// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//===--- SudokuTests/Solver_set_option.cpp                              ---===//
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
#include "print_Options.h" // Debug Output

#include <Strategic/Solver.h> // Class under test
#include <Sudoku/Board.h>
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>

#include <array>
#include <bitset>

#include <type_traits>

#include <gtest/gtest.h>


namespace SudokuTests::Solvers_set_option
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Options;
using ::Sudoku::Value;

TEST(Solver, setValue)
{
	using ::Sudoku::set_Value;
	using L = Location<2>;
	// clang-format off
	//	4		1234	3		1234
	//	134		1234	1234	1234
	//	1234	1234	1234	1234
	//	1234	1234	1234	4
	// clang-format on

	// Set single value
	//====----------------------------------------------------------------====//
	// precondition checks
	Board<Options<4>, 2> board;
	static_assert(not noexcept(set_Value(board, L(), Value{2})));
	EXPECT_NO_THROW(set_Value(board, L{0}, Value{1}));
	EXPECT_NO_THROW(set_Value(board, L{15}, Value{1}));
	EXPECT_NO_THROW(set_Value(board, L{1}, Value{4}));

	// test: value is not an option
	board[1][1] = std::bitset<4>{"1101"}; // options: 1,3,4
	EXPECT_THROW(
		set_Value(board, L(1, 1), Value{2}), Sudoku::error::invalid_Board);
	EXPECT_THROW(set_Value(board, L(1, 1), Value{2}), std::logic_error);
	// test: already set to another answer
	board[1][2] = Options<4>{Value{2}};
	EXPECT_THROW(
		set_Value(board, L{1, 2}, Value{1}), Sudoku::error::invalid_Board);
	EXPECT_THROW(set_Value(board, L{1, 2}, Value{1}), std::logic_error);

	board.clear(); // reset
	ASSERT_EQ(board[1][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_EQ(set_Value(board, L(2), Value{3}), 4);
	EXPECT_EQ(board[0][2], Value{3});
	EXPECT_EQ(set_Value(board, L(0), Value{4}), 4);
	EXPECT_EQ(board[0][0], Value{4});
	// test: already set to THIS answer (allow to overwrite with same)
	EXPECT_EQ(set_Value(board, L(15), Value{4}), 4);
	ASSERT_EQ(board[3][3], Value{4});
	EXPECT_EQ(set_Value(board, L(3, 3), Value{4}), 0); // <==
}

TEST(SolverDeathTest, setValue)
{
	using ::Sudoku::set_Value;
	using L = Location<2>;

	Board<Options<4>, 2> board;
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(set_Value(board, L{-1}, Value{1}), "is_valid\\(loc\\)");
	EXPECT_DEBUG_DEATH(set_Value(board, L{16}, Value{1}), "is_valid\\(loc\\)");
	EXPECT_DEBUG_DEATH(
		set_Value(board, L{1}, Value{0}), "is_valid<N>\\(value\\)");
	EXPECT_DEBUG_DEATH(
		set_Value(board, L{1}, Value{5}), "is_valid<N>\\(value\\)");
#else
	// thrown by Board::at(Location)
	EXPECT_THROW(
		set_Value(board, L{-1}, Value{1}), Sudoku::error::invalid_Location);
	EXPECT_THROW(
		set_Value(board, L{16}, Value{1}), Sudoku::error::invalid_Location);
	EXPECT_THROW(
		set_Value(board, L{1}, Value{0}), Sudoku::error::invalid_option);
	EXPECT_THROW(
		set_Value(board, L{1}, Value{5}), Sudoku::error::invalid_option);
#endif // NDEBUG}
}

TEST(Solver, setValueVector)
{
	using ::Sudoku::set_Value;
	{
		Board<Options<4>, 2> B1;
		// precondition checks
		constexpr std::array<Value, 17> too_long{};

		// exceptions
		static_assert(
			!noexcept(set_Value(B1, too_long.cbegin(), too_long.cend())));
		// NOLINTNEXTLINE(clang-analyzer-deadcode.DeadStores)
		const std::array<Options<4>, 16> optie{};
		static_assert(!noexcept(set_Value(B1, optie.cbegin(), optie.cend())));
		constexpr std::array<char, 16> ints{};
		static_assert(!noexcept(set_Value(B1, ints.cbegin(), ints.cend())));

		constexpr std::array<char, 16> zero{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		Board<Options<4>, 2> B2;
		EXPECT_EQ(set_Value(B2, zero.cbegin(), zero.cend()), 0);
		constexpr std::array<char, 16> negative{
			0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		EXPECT_THROW(
			set_Value(B2, negative.cbegin(), negative.cend()),
			std::domain_error);
		constexpr std::array<char, 16> wrong{
			0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		EXPECT_THROW(
			set_Value(B2, wrong.cbegin(), wrong.cend()), std::domain_error);

		// return type
		static_assert(
			std::is_same_v<
				int,
				decltype(set_Value(B1, too_long.cbegin(), too_long.cend()))>);
	}
	{ // using Value as input
		using V = Value;
		// clang-format off
		constexpr std::array<Value, 16> v1
		{
			V{0}, V{2},  V{0}, V{0},
			V{4}, V{0},  V{0}, V{0},
			V{0}, V{1},  V{4}, V{0},
			V{0}, V{0},  V{0}, V{0}
		}; // clang-format on
		Board<Options<4>, 2> B2;
		EXPECT_EQ(set_Value(B2, v1.cbegin(), v1.cend()), 44);
		EXPECT_EQ(B2[0][1], Value{2});
		EXPECT_EQ(B2[1][0], Value{4});
		EXPECT_EQ(B2[2][1], Value{1});
		EXPECT_EQ(B2[2][2], Value{4});
		// check if processed single option cells:
		EXPECT_EQ(B2[0][3], Value{4});
		EXPECT_EQ(B2[3][1], Value{4});
		// from array didn't cascade over single option cells
		EXPECT_EQ(B2[0][0], Value{1});
		EXPECT_EQ(B2[0][2], Value{3});
		EXPECT_EQ(B2[1][1], Value{3});
		EXPECT_EQ(B2[3][1], Value{4});
	}
	{      // using int as input
		// clang-format off
		constexpr std::array<char, 16> v1
		{	// start     // after set_Value
			0, 2,  0, 0, // 1  2 3  4
			4, 0,  0, 0, // 4  3 12 12
			0, 1,  4, 0, // 23 1 4  23
			0, 0,  0, 0  // 23 4 12 123
		}; // clang-format on
		// Copy data from the array
		Board<Options<4>, 2> B2;
		EXPECT_EQ(set_Value(B2, v1.cbegin(), v1.cend()), 44);
		EXPECT_EQ(B2[0][1], Value{2});
		EXPECT_EQ(B2[1][0], Value{4});
		EXPECT_EQ(B2[2][1], Value{1});
		EXPECT_EQ(B2[2][2], Value{4});
		// check if processed single option cells:
		EXPECT_EQ(B2[0][3], Value{4});
		EXPECT_EQ(B2[3][1], Value{4});
		// from array didn't cascade over single option cells
		EXPECT_EQ(B2[0][0], Value{1});
		EXPECT_EQ(B2[0][2], Value{3});
		EXPECT_EQ(B2[1][1], Value{3});
		EXPECT_EQ(B2[3][1], Value{4});

		// using Options as input:
#if not(defined(__ICL) && __ICL <= 1900 && defined(_DEBUG))
		Board<Options<4>, 2> B3;
		EXPECT_EQ(set_Value(B3, B2.cbegin(), B2.cend()), 46);
#endif // __ICL
	}
}

TEST(SolverDeathTest, setValueVector)
{
	using ::Sudoku::set_Value;
	{
		Board<Options<4>, 2> B1;
		// precondition checks
		constexpr std::array<Value, 15> too_short{};
		constexpr std::array<Value, 17> too_long{};
		EXPECT_DEBUG_DEATH(
			set_Value(B1, too_short.cbegin(), too_short.cend()), "full_size");
		EXPECT_DEBUG_DEATH(
			set_Value(B1, too_long.cbegin(), too_long.cend()), "full_size");
	}
}

TEST(SolverDeathTest, setSectionLocals)
{
	using ::Sudoku::set_section_locals;
	// called by: section_exclusive
	//	find and process values appearing 2 or 3x in row/col

	// int set_section_locals(section, rep_count, worker)
	using set = std::bitset<4>;
	Board<Options<4>, 2> B{};

	B[0][0] = Value{3};
	B.clear();
	ASSERT_TRUE(B[0][0].all()); // works

	// Incorrect rep_count
	B[2][0] = set{"1100"};
	B[2][1] = set{"1110"};
	Options<4> worker{set{"0011"}};
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(2), 0, worker), "rep_count > 1");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(2), 1, worker), "rep_count > 1");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.col(2), 1, worker), "rep_count > 1");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.block(3), 1, worker), "rep_count > 1");
	EXPECT_DEBUG_DEATH(set_section_locals(B, B.row(2), 2, worker), ".*");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(2), 4, worker), "rep_count <= N");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(2), 3, worker), "rep_count <= N");
#ifdef NDEBUG
	EXPECT_EQ(set_section_locals(B, B.row(2), 0, worker), 0);
	EXPECT_EQ(set_section_locals(B, B.row(2), 1, worker), 1);
	EXPECT_EQ(set_section_locals(B, B.row(2), 2, worker), 0);
	EXPECT_EQ(set_section_locals(B, B.row(2), 3, worker), 0);
	EXPECT_EQ(set_section_locals(B, B.row(2), 4, worker), 0);
#endif // NDEBUG
	// option doesn't exist in section
	B.clear();
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1111"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	worker  = Value{2};
	ASSERT_TRUE(worker[Value{2}]);
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(0), 2, worker), "Assertion .*");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.col(0), 2, worker), "Assertion .*");
	B[1][0] = set{"1101"};
	B[1][1] = set{"1101"};
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.block(0), 2, worker), "Assertion .*");
	// worker empty
	worker = set{"0000"};
	ASSERT_TRUE(worker.is_empty());
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(0), 2, worker), "count\\(\\) > 0");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.col(0), 2, worker), "count\\(\\) > 0");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.block(0), 2, worker), "count\\(\\) > 0");
#else
	EXPECT_EQ(set_section_locals(B, B.row(0), 2, worker), 0);
	EXPECT_EQ(set_section_locals(B, B.col(0), 2, worker), 0);
	EXPECT_EQ(set_section_locals(B, B.block(0), 2, worker), 0);
#endif // NDEBUG
	// worker empty (with answer-bit)
	worker = set{"0000"};
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(0), 2, worker), "count\\(\\) > 0");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.col(0), 2, worker), "count\\(\\) > 0");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.block(0), 2, worker), "count\\(\\) > 0");
	// worker all set
	worker = set{"1111"};
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.col(2), 4, worker), "rep_count <= N");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.block(2), 4, worker), "rep_count <= N");


	// set_section_locals(section, rep_count, worker)
	{
#if false
		Options<4> worker{Value{0}};
		EXPECT_DEBUG_DEATH(set_section_locals(B, B.row(0), 1, worker), "count");
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.block(0), 1, worker), "count");
		worker = std::bitset<5>{"00001"};
		EXPECT_DEBUG_DEATH(set_section_locals(B, B.col(0), 1, worker), "count");
		EXPECT_DEBUG_DEATH(
			set_section_locals(B, B.block(0), 1, worker), "count");
#endif
	}
	// option doesn't exist in section
	B.clear();
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1111"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"1101"};
	B[1][1] = set{"1101"};
	worker  = Value{2};
	ASSERT_TRUE(worker[Value{2}]);
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.block(0), 2, worker), "Assertion .*");
	EXPECT_DEBUG_DEATH(
		set_section_locals(B, B.row(0), 2, worker), "Assertion .*");
}

TEST(Solver, setSectionLocals)
{
	using ::Sudoku::set_section_locals;
	// called by: section_exclusive
	//	find and process values appearing 2 or 3x in row/col

	// int set_section_locals(section, rep_count, worker)
	using set = std::bitset<4>;
	Board<Options<4>, 2> B{};

	B[0][0] = Value{3};
	B.clear();
	ASSERT_TRUE(B[0][0].all()); // works

	//====----------------------------------------------------------------====//
	// Row
	//
	//	12		1234	12		1234
	//	1234	1234	1234	1234
	//	34		34		1234	1234
	//	1234	1234	1234	1234
	//
	//	2 values in row, not in same block
	B[0][0] = set{"0011"};
	B[0][2] = set{"0011"};
	Options<4> worker{set{"1100"}};
	ASSERT_TRUE(B[0][1].all());
	ASSERT_TRUE(B[0][3].all());
	EXPECT_EQ(set_section_locals(B, B.row(0), 2, worker), 0);
	EXPECT_EQ(B[0][0].count(), 2U);
	EXPECT_TRUE(B[0][1].all());
	EXPECT_EQ(B[0][2].count(), 2U);
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][0].all());
	EXPECT_TRUE(B[1][1].all());
	//	2 values in row, and in same block
	B.clear(); // reset board
	B[2][0] = set{"1100"};
	B[2][1] = set{"1100"};
	worker  = set{"0011"};
	ASSERT_TRUE(B[2][2].all());
	ASSERT_TRUE(B[2][3].all());
	ASSERT_TRUE(B[3][2].all());
	EXPECT_EQ(set_section_locals(B, B.row(2), 2, worker), 4);
	EXPECT_TRUE(B[3][0].all());
	EXPECT_EQ(B[2][2].count(), 4U); // self
	EXPECT_EQ(B[2][3].count(), 4U); // self
	EXPECT_EQ(B[3][2].count(), 2U); // rest block
	EXPECT_EQ(B[1][2].count(), 4U);

	//====----------------------------------------------------------------====//
	// Col
	//
	//	12		1234	1234	1234
	//	1234	1234	1234	1234
	//	12		1234	1234	34
	//	1234	1234	1234	34
	//
	B.clear(); // reset board
	B[0][0] = set{"0011"};
	B[2][0] = set{"0011"};
	worker  = set{"1100"};
	EXPECT_EQ(set_section_locals(B, B.col(0), 2, worker), 0);

	//====----------------------------------------------------------------====//
	// Block
	//
	//	12		1234	1234	1234
	//	1234	12		1234	1234
	//	34		34		1234	1234
	//	1234	1234	1234	1234
	//
	B.clear(); // reset board
	B[0][0] = set{"0011"};
	B[1][1] = set{"0011"};
	worker  = set{"1100"};
	EXPECT_EQ(set_section_locals(B, B.block(0), 2, worker), 0);
	EXPECT_TRUE(B[0][1].all());
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][0].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_TRUE(B[3][0].all());
	// same block, same row
	B[2][0] = set{"1100"};
	B[2][1] = set{"1100"};
	worker  = set{"0011"};
	EXPECT_EQ(set_section_locals(B, B.block(2), 2, worker), 4);
	EXPECT_TRUE(B[3][0].all());     // self block
	EXPECT_TRUE(B[3][1].all());     // self block
	EXPECT_EQ(B[3][2].count(), 2U); // rest row
	EXPECT_EQ(B[3][3].count(), 2U); // rest row
	EXPECT_TRUE(B[0][1].all());     // rest col
	EXPECT_TRUE(B[0][2].all());
	EXPECT_TRUE(B[0][3].all());
	// same block, same col
	B.clear(); // reset board
	B[0][1] = set{"0011"};
	B[1][1] = set{"0011"};
	worker  = set{"1100"};
	EXPECT_EQ(set_section_locals(B, B.block(0), 2, worker), 4);
	EXPECT_TRUE(B[0][0].all());     // self block
	EXPECT_EQ(B[0][1].count(), 2U); // self block
	EXPECT_TRUE(B[0][2].all());     // rest row
	EXPECT_TRUE(B[0][3].all());     // rest row
	EXPECT_TRUE(B[1][0].all());     // self block
	EXPECT_EQ(B[1][1].count(), 2U); // self block
	EXPECT_TRUE(B[1][2].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_EQ(B[2][0].count(), 2U); // rest col
	EXPECT_TRUE(B[2][1].all());
	EXPECT_TRUE(B[2][2].all());
	EXPECT_TRUE(B[2][3].all());
	EXPECT_EQ(B[3][0].count(), 2U); // rest col
	EXPECT_TRUE(B[3][1].all());
	EXPECT_TRUE(B[3][2].all());
	EXPECT_TRUE(B[3][3].all());
	Board<Options<9>, 3> B3{};
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
	ASSERT_TRUE(is_answer(B3[1][0], Value{4}));
	ASSERT_TRUE(B3[1][8].all());
	worker3 = std::bitset<9>{"0000001111"};
	EXPECT_EQ(set_section_locals(B3, B3.block(0), 3, worker3), 3 * 6);
	EXPECT_EQ(B3[0][0].count(), 9U);
	EXPECT_EQ(B3[0][3].count(), 6U);
	EXPECT_EQ(B3[0][8].count(), 6U);
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
	B3.clear(); // reset board
	B3[0][1] = Value{2};
	B3[0][2] = Value{3};
	B3[1][1] = Value{5};
	B3[1][2] = Value{6};
	B3[2][1] = Value{8};
	B3[2][2] = Value{9};
	worker3  = std::bitset<9>{"0010010011"};
	EXPECT_EQ(set_section_locals(B3, B3.block(0), 3, worker3), 3 * 6);
	EXPECT_EQ(B3[0][0].count(), 9U);
	EXPECT_TRUE(B3[0][4].all()); // rest row
	EXPECT_TRUE(B3[0][8].all()); // rest row
	EXPECT_TRUE(B3[1][0].all()); // self
	EXPECT_TRUE(is_answer(B3[1][1], Value{5}));
	EXPECT_EQ(B3[3][0].count(), 6U); // rest col
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_EQ(B3[4][0].count(), 6U); // rest col
	EXPECT_EQ(B3[8][0].count(), 6U); // rest col
	EXPECT_TRUE(B3[8][1].all());
	// 2 in same block & col, one in different col
	//
	//	0	2	3
	//	0	5	6
	//	7	0	9
	//
	B3.clear(); // reset board
	B3[0][1] = Value{2};
	B3[0][2] = Value{3};
	B3[1][1] = Value{5};
	B3[1][2] = Value{6};
	B3[2][0] = Value{7};
	B3[2][2] = Value{9};
	worker3  = std::bitset<9>{"0100010011"};
	EXPECT_EQ(set_section_locals(B3, B3.block(0), 3, worker3), 0);
	EXPECT_TRUE(B3[0][0].all()); // self
	EXPECT_TRUE(B3[0][4].all()); // rest row
	EXPECT_TRUE(B3[0][8].all()); // rest row
	EXPECT_TRUE(B3[1][0].all()); // self
	EXPECT_TRUE(is_answer(B3[1][1], Value{5}));
	EXPECT_TRUE(B3[2][1].all()); // self
	EXPECT_TRUE(B3[3][0].all()); // rest col
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_TRUE(B3[4][0].all()); // rest col
	EXPECT_TRUE(B3[8][0].all()); // rest col
	EXPECT_TRUE(B3[8][1].all());
}

TEST(Solver, setUnique)
{
	using ::Sudoku::set_unique;
	using L = Location<2>;
	Board<Options<4>, 2> board{};
	constexpr std::array<char, 16> v1{
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
	set_Value(board, v1.cbegin(), v1.cend());
	const Board<Options<4>, 2> cB1{board}; // to reset board

	static_assert(not noexcept(set_unique(board, board.row(0), Value{3})));
	EXPECT_EQ(set_unique(board, board.row(2), Value{1}), 4);
	EXPECT_TRUE(is_answer(board[2][1], Value{1}));
	EXPECT_EQ(board[2][1].count(), 1U);
	// is already an answer
	EXPECT_EQ(set_unique(board, board.row(2), Value{1}), 0);

	board = cB1; // reset
	// Value is not an option
	remove_option(board, L{5}, Value{3});
	remove_option(board, L{6}, Value{3});
	remove_option(board, L{7}, Value{3});
	EXPECT_FALSE(is_option(board[1][2], Value{3}));
	EXPECT_THROW(
		set_unique(board, board.row(1), Value{3}),
		::Sudoku::error::invalid_Board);
}

TEST(SolverDeathTest, setUnique)
{
	using ::Sudoku::set_unique;
	Board<Options<4>, 2> board{};
	constexpr std::array<char, 16> v1{
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
	set_Value(board, v1.cbegin(), v1.cend());
	const Board<Options<4>, 2> cB1{board}; // to reset board

#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		set_unique(board, board.row(2), Value{12}), "is_valid<N>\\(value\\)");
#else
	EXPECT_THROW(
		set_unique(board, board.row(2), Value{12}),
		Sudoku::error::invalid_option);
#endif // NDEBUG

	board = cB1; // reset
	// Section is not a part of board
	EXPECT_DEBUG_DEATH(set_unique(board, cB1.row(0), Value{1}), "board");
	// Value is not unique
	EXPECT_DEBUG_DEATH(
		set_unique(board, board.row(2), Value{4}), "std::find_if");
}

TEST(Solver, setUniques)
{
	using ::Sudoku::appearance_once;
	using ::Sudoku::set_uniques;
	// Solver<N>::set_uniques(const SectionT section, const Options& worker)
	//===------------------------------------------------------------------===//
	// processing only one value per section.
	// also in the test: appearance_sets and appearance_once
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	//
	Board<Options<4>, 2> B1{};
	constexpr std::array<char, 16> v1{
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
	set_Value(B1, v1.cbegin(), v1.cend());
	const Board<Options<4>, 2> cB1{B1}; // to reset B1
	// verify setup
	ASSERT_EQ(B1[0][0].count(), 3U);
	ASSERT_EQ(B1[0][1].count(), 3U);
	ASSERT_EQ(B1[0][2].count(), 3U);
	ASSERT_EQ(B1[0][3].count(), 4U);
	ASSERT_EQ(B1[2][1].count(), 4U);

	// worker empty -> return 0, no execution
	EXPECT_EQ(
		set_uniques(B1, B1.row(0), Options<4>{std::bitset<4>{"0000"}}), 0);
	EXPECT_EQ(B1[0][0].count(), 3U);
	EXPECT_EQ(B1[0][3].count(), 4U);

	// Row (one value)
	auto worker = appearance_once<2>(B1.row(0));
	EXPECT_EQ(worker, Options<4>{std::bitset<4>{"0001"}});
	EXPECT_EQ(set_uniques(B1, B1.row(0), worker), 4);
	EXPECT_EQ(B1[0][0].count(), 3U); // no change
	EXPECT_EQ(B1[0][1].count(), 3U); // no change
	EXPECT_EQ(B1[0][2].count(), 3U); // no change
	EXPECT_TRUE(is_answer(B1[0][3], Value{1}));
	worker = appearance_once<2>(B1.row(2));
	EXPECT_EQ(worker, Options<4>{std::bitset<4>{"0001"}});
	EXPECT_EQ(set_uniques(B1, B1.row(2), worker), 4);
	EXPECT_TRUE(is_answer(B1[2][1], Value{1}));
	// Col (one value)
	B1 = cB1; // reset
	ASSERT_EQ(B1[0][3].count(), 4U);
	ASSERT_EQ(B1[2][1].count(), 4U);
	worker = appearance_once<2>(B1.col(1));
	EXPECT_EQ(worker, Options<4>{std::bitset<4>{"0001"}});
	EXPECT_EQ(set_uniques(B1, B1.col(1), worker), 4);
	EXPECT_EQ(B1[0][1].count(), 3U); // no change
	EXPECT_EQ(B1[1][1].count(), 3U); // no change
	EXPECT_TRUE(is_answer(B1[2][1], Value{1}));
	EXPECT_EQ(B1[3][1].count(), 3U); // no change
	// Block (one value)
	B1 = cB1; // reset
	ASSERT_EQ(B1[2][1].count(), 4U);
	worker = appearance_once<2>(B1.block(2));
	EXPECT_EQ(worker, Options<4>{std::bitset<4>{"0001"}});
	EXPECT_EQ(set_uniques(B1, B1.block(2), worker), 4);
	EXPECT_EQ(B1[2][0].count(), 3U); // no change
	EXPECT_TRUE(is_answer(B1[2][1], Value{1}));
	EXPECT_EQ(B1[3][0].count(), 3U); // no change
	EXPECT_EQ(B1[3][1].count(), 3U); // no change

	//===------------------------------------------------------------------===//
	// multiple values in a single go
	// 12	24	324	24
	Board<Options<4>, 2> B2{};
	B2[0][0] = std::bitset<4>{"0011"};
	B2[0][1] = std::bitset<4>{"1010"};
	B2[0][2] = std::bitset<4>{"1110"};
	B2[0][3] = std::bitset<4>{"1010"};
	worker   = appearance_once<2>(B2.row(0));
	EXPECT_EQ(worker, Options<4>{std::bitset<4>{"0101"}});
	EXPECT_EQ(set_uniques(B2, B2.row(0), worker), 13);
	// 10 = ans(0){1} + col(0){3} + block(0){1}
	//		ans(2){1} + col(2){3} + block(1){1}
	EXPECT_TRUE(is_answer(B2[0][0], Value{1}));
	EXPECT_TRUE(is_answer(B2[0][2], Value{3}));
	EXPECT_EQ(B2[0][1].count(), 2U); // no change
	EXPECT_EQ(B2[0][3].count(), 2U); // no change
	EXPECT_FALSE(is_option(B2[3][0], Value{1}));
}

TEST(Solver, deathtestSetOption)
{
	Board<Options<4>, 2> B{};

	{ // SetValue(Itr, Itr)
		constexpr std::array<char, 10> v1{};
		constexpr std::array<char, 18> v2{};
		// input too short / too long
		EXPECT_DEBUG_DEATH(
			set_Value(B, v1.cbegin(), v1.cend()), "Assertion .*");
		EXPECT_DEBUG_DEATH(
			set_Value(B, v2.cbegin(), v2.cend()), "Assertion .*");
	}
	// set_uniques
	//{
	//	Board<Options<4>, 2> B1{};
	//	// death test: a unique Value in worker doesn't exist in the section
	//	// 1	24	324	24
	//	B1[0][0] = std::bitset<4>{"0001"}; // ans 1
	//	EXPECT_FALSE(is_option(B1[0][0], Value{1}));
	//	B1[0][1]    = std::bitset<4>{"1010"};
	//	B1[0][2]    = std::bitset<4>{"1110"};
	//	B1[0][3]    = std::bitset<4>{"1010"};
	//	auto worker = Options<4>{std::bitset<4>{"0101"}};
	//	EXPECT_DEBUG_DEATH(
	//		set_uniques(B1, B1.row(0), worker), "Assertion failed: false");
	//}
}

} // namespace SudokuTests::Solvers_set_option
