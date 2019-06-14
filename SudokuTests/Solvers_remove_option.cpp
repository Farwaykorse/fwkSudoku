// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//===--- SudokuTests/Solver_remove_option.cpp                           ---===//
//
// Unit tests for the option removing functions in Sudoku::Solver
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
#include <Sudoku/Solver.h>
// helpers
#include <Sudoku/Board.h>
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>
#include <Sudoku/exceptions.h>
// Debug Output
#include "print_Options.h"
// library
#include <bitset>
#include <vector>
#include <type_traits>


namespace SudokuTests::SolversTest
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Options;
using ::Sudoku::Value;
using ::Sudoku::error::invalid_Location;
using ::Sudoku::error::invalid_Board;

TEST(Solver, remove_option)
{
	using L2 = Location<2>;

	// remove_option(location, int value)
	Board<Options<4>, 2> board{};
	ASSERT_EQ(board[1][0], Options<4>{}) << "incorrect instantiation";

	static_assert(not noexcept(remove_option(board, L2{2}, Value{1})));
	// throw from Board::at(loc)
	EXPECT_NO_THROW(remove_option(board, L2{0}, Value{1}));
	EXPECT_NO_THROW(remove_option(board, L2{15}, Value{1}));
	EXPECT_THROW(remove_option(board, L2{-1}, Value{1}), invalid_Location);
	EXPECT_THROW(remove_option(board, L2{16}, Value{1}), invalid_Location);
	// value out of bounds (2-sided)
	EXPECT_DEBUG_DEATH(
		remove_option(board, L2(2), Value{0}),
		"Assertion .*is_valid<N>\\(value\\)");
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		remove_option(board, L2(2), Value{9}),
		"Assertion .*is_valid<N>\\(value\\)");
#else
	EXPECT_THROW(remove_option(board, L2{2}, Value{9}), std::out_of_range);
#endif // NDEBUG
	// last option (not answer)
	board[0][2] = std::bitset<5>{"01001"}; // 3
	EXPECT_DEBUG_DEATH(
		remove_option(board, L2(2), Value{3}), "Assertion .*count > 0");
	// TODO throw from single_option?

	// return type
	static_assert(
		std::is_same_v<int, decltype(remove_option(board, L2{}, Value{1}))>);
	//===------------------------------------------------------------------===//

	// value is not an option -> return 0 (only value is missing)
	board[0][0] = std::bitset<5>{"11011"};
	board[1][0] = std::bitset<5>{"11011"};
	ASSERT_FALSE(is_option(board[0][0], Value{2}));
	EXPECT_NO_THROW(remove_option(board, Location<2>(0), Value{2}));
	EXPECT_EQ(remove_option(board, Location<2>(1, 0), Value{2}), 0);
	// location = answer (not value)
	board[0][1] = std::bitset<5>{"10000"}; // answer 4
	board[1][1] = std::bitset<5>{"10000"}; // answer 4
	EXPECT_NO_THROW(remove_option(board, Location<2>(1), Value{2}));
	EXPECT_EQ(remove_option(board, Location<2>(1, 1), Value{2}), 0);
	// location = answer (value)
	EXPECT_THROW(
		remove_option(board, Location<2>(1), Value{4}),
		::Sudoku::error::invalid_Board);
	EXPECT_EQ(remove_option(board, Location<2>(1), Value{2}), 0);

	// normal remove from 4
	EXPECT_TRUE(is_option(board[2][1], Value{4}));
	EXPECT_EQ(board[2][1].count(), 4U);
	EXPECT_EQ(remove_option(board, Location<2>(2, 1), Value{4}), 1);
	// normal remove from 3 -> dual_option
	EXPECT_EQ(board[2][1].count(), 3U);
	EXPECT_EQ(remove_option(board, Location<2>(2, 1), Value{3}), 1);
	// normal remove from 2 -> single option
	EXPECT_EQ(board[2][1].count(), 2U);
	EXPECT_GT(remove_option(board, Location<2>(2, 1), Value{2}), 1);
	EXPECT_EQ(board[2][1].count(), 0U);
	EXPECT_EQ(board[2][1].count_all(), 1U);
}

TEST(Solver, remove_option_mask)
{
	using L2 = Location<2>;
	using O2 = Options<4>;
	Board<O2, 2> board{};

	static_assert(not noexcept(remove_option(board, L2{2}, Options<4>{})));
	// answer-bit in mask
	auto mask = Options<4>{Value{0}};
	EXPECT_DEBUG_DEATH(
		remove_option(board, L2{0}, mask), "Assertion .*is_answer_fast.mask.");
	mask = Options<4>{};
#ifdef NDEBUG
	EXPECT_THROW(remove_option(board, L2{0}, mask), invalid_Board);
#else
	EXPECT_DEBUG_DEATH(
		remove_option(board, L2{0}, mask), "Assertion .*is_answer_fast");
#endif
	mask.clear();
	EXPECT_DEBUG_DEATH(
		remove_option(board, L2{0}, mask), "Assertion .*is_empty");
	// throw from Board::at(loc)
	mask = Options<4>{Value{1}};
	EXPECT_NO_THROW(remove_option(board, L2{0}, mask));
	EXPECT_NO_THROW(remove_option(board, L2{15}, mask));
	EXPECT_THROW(remove_option(board, L2{-1}, mask), invalid_Location);
	EXPECT_THROW(remove_option(board, L2{16}, mask), invalid_Location);
	// throw when last option removed
	mask = Options<4>{std::bitset<5>{"11110"}};
	EXPECT_THROW(remove_option(board, L2{4}, mask), invalid_Board);
	// TODO throw from single_option?

	// return type
	static_assert(
		std::is_same_v<int, decltype(remove_option(board, L2{}, mask))>);

	// return 0 when answered
	board[0][0] = O2{Value{4}};
	mask        = O2{std::bitset<5>{"11000"}};
	EXPECT_THROW(remove_option(board, L2{0, 0}, mask), invalid_Board);
	board[1][0] = O2{};
	EXPECT_EQ(remove_option(board, L2{1, 0}, mask), 2);

	// trigger single_option
	board[1][1] = O2{std::bitset<5>{"11101"}};
	EXPECT_EQ(remove_option(board, L2{1, 1}, mask), 15);
}

TEST(Solver, remove_option_section)
{
	using L = Location<2>;
	// remove_option_section(SectionT, Location ignore, int value)
	// SEE death test: loc-> !is_answer(value)
	const Board<Options<4>, 2> cB1;
	Board<Options<4>, 2> B1;
	B1[0][0] = Value{1};
	ASSERT_TRUE(is_answer(B1[0][0], Value{1}));
	// row
	ASSERT_EQ(B1[0][1].count(), 4U);
	EXPECT_NO_THROW(remove_option_section(B1, B1.row(0), L(0), Value{1}));
	EXPECT_EQ(B1[0][1].count(), 3U);
	B1[0][1] = Value{2};
	EXPECT_EQ(remove_option_section(B1, B1.row(0), L(0, 1), Value{2}), 2);
	// trigger single_option
	B1[0][2] = Value{3};
	EXPECT_EQ(remove_option_section(B1, B1.row(0), L(0, 2), Value{3}), 6);
	EXPECT_TRUE(is_answer(B1[0][3], Value{4}));
	EXPECT_EQ(B1[3][3].count(), 3U);
	EXPECT_EQ(B1[1][3].count(), 3U);
	EXPECT_EQ(B1[1][2].count(), 3U);
	EXPECT_TRUE(is_answer(B1[0][0], Value{1}));
	EXPECT_TRUE(is_answer(B1[0][1], Value{2}));
	EXPECT_TRUE(is_answer(B1[0][2], Value{3}));
	EXPECT_TRUE(B1[3][0].all());

	// col
	B1       = cB1;
	B1[0][0] = Value{1};
	ASSERT_EQ(B1[1][0].count(), 4U);
	EXPECT_NO_THROW(remove_option_section(B1, B1.col(0), L(0), Value{1}));
	EXPECT_EQ(B1[1][0].count(), 3U);
	B1[1][0] = Value{2};
	EXPECT_EQ(remove_option_section(B1, B1.col(0), L(1, 0), Value{2}), 2);
	// trigger single_option
	B1[2][0] = Value{3};
	EXPECT_EQ(remove_option_section(B1, B1.col(0), L(2, 0), Value{3}), 6);
	EXPECT_TRUE(is_answer(B1[3][0], Value{4}));
	EXPECT_EQ(B1[3][3].count(), 3U);
	EXPECT_EQ(B1[3][1].count(), 3U);
	EXPECT_EQ(B1[2][1].count(), 3U);
	EXPECT_TRUE(is_answer(B1[0][0], Value{1}));
	EXPECT_TRUE(is_answer(B1[1][0], Value{2}));
	EXPECT_TRUE(is_answer(B1[2][0], Value{3}));
	EXPECT_TRUE(B1[2][3].all());

	// block
	B1       = cB1;
	B1[0][0] = Value{1};
	ASSERT_EQ(B1[1][1].count(), 4U);
	EXPECT_NO_THROW(remove_option_section(B1, B1.block(0), L(0), Value{1}));
	EXPECT_EQ(B1[1][1].count(), 3U);
	B1[0][1] = Value{2};
	EXPECT_EQ(remove_option_section(B1, B1.block(0), L(0, 1), Value{2}), 2);
	// trigger single_option
	B1[1][0] = Value{3};
	EXPECT_EQ(remove_option_section(B1, B1.block(0), L(1, 0), Value{3}), 6);
	EXPECT_TRUE(is_answer(B1[1][1], Value{4}));
	EXPECT_EQ(B1[1][2].count(), 3U);
	EXPECT_EQ(B1[1][3].count(), 3U);
	EXPECT_EQ(B1[3][1].count(), 3U);
	EXPECT_TRUE(is_answer(B1[0][0], Value{1}));
	EXPECT_TRUE(is_answer(B1[0][1], Value{2}));
	EXPECT_TRUE(is_answer(B1[1][0], Value{3}));
	EXPECT_TRUE(B1[3][3].all());

	// conflict Value has already been set as an answer in this section
	B1       = cB1;
	B1[0][0] = Value{2}; // set answer, to be removed from rest row
	B1[0][1] = Value{2}; // set as answer in same row
	EXPECT_THROW(
		remove_option_section(B1, B1.row(0), L(0), Value{2}), invalid_Board);
	EXPECT_TRUE(is_answer(B1[0][0], Value{2}));
	EXPECT_TRUE(is_answer(B1[0][1], Value{2}));
}
TEST(Solver, remove_option_section_1)
{
	using L  = Location<2>;
	using vL = std::vector<L>;

	// remove_option_section(SectionT, const std::vector<Location>& ignore, int
	Board<Options<4>, 2> B1;
	// SEE death test: when ignore is empty
	// row
	ASSERT_NO_THROW(
		remove_option_section(B1, B1.row(0), vL{L(0), L(1)}, Value{1}));
	EXPECT_EQ(B1[0][3].count(), 3U);
	EXPECT_EQ(
		remove_option_section(B1, B1.row(0), vL{L(0), L(1)}, Value{2}), 2);
	EXPECT_EQ(B1[0][0].count(), 4U);
	EXPECT_EQ(B1[0][1].count(), 4U);
	EXPECT_EQ(B1[0][2].count(), 2U);
	EXPECT_EQ(B1[0][3].count(), 2U);
	// col
	ASSERT_NO_THROW(
		remove_option_section(B1, B1.col(0), vL{L(0), L(1, 0)}, Value{1}));
	EXPECT_EQ(B1[3][0].count(), 3U);
	EXPECT_EQ(
		remove_option_section(B1, B1.col(0), vL{L(0), L(1, 0)}, Value{2}), 2);
	EXPECT_EQ(B1[0][0].count(), 4U);
	EXPECT_EQ(B1[1][0].count(), 4U);
	EXPECT_EQ(B1[2][0].count(), 2U);
	EXPECT_EQ(B1[3][0].count(), 2U);
	// block
	ASSERT_NO_THROW(
		remove_option_section(B1, B1.block(0), vL{L(0), L(1)}, Value{1}));
	EXPECT_EQ(B1[1][0].count(), 3U);
	EXPECT_EQ(
		remove_option_section(B1, B1.block(0), vL{L(0), L(1)}, Value{2}), 2);
	EXPECT_EQ(B1[0][0].count(), 4U);
	EXPECT_EQ(B1[0][1].count(), 4U);
	EXPECT_EQ(B1[1][0].count(), 2U);
	EXPECT_EQ(B1[1][1].count(), 2U);
	// single ignore value
	ASSERT_EQ(B1[3][1].count(), 4U);
	ASSERT_NO_THROW(
		remove_option_section(B1, B1.row(3), vL{L(3, 0)}, Value{3}));
	EXPECT_EQ(B1[3][1].count(), 3u);
}
TEST(Solver, remove_option_section_2)
{
	using L  = Location<2>;
	using vL = std::vector<L>;
	using v  = Value;
	using vV = Options<4>;
	using b  = std::bitset<5>;

	// remove_option_section(SectionT, vector<Location>, vector<int>
	const Board<Options<4>, 2> cB;
	Board<Options<4>, 2> B;
	// row
	ASSERT_NO_THROW(
		remove_option_section(B, B.row(0), vL{L(0), L(1)}, vV{b{"00110"}}));
	EXPECT_EQ(B[0][3].count(), 2u);
	EXPECT_EQ(
		remove_option_section(
			B, B.row(1), vL{L(1, 0), L(1, 1)}, vV{b{"11000"}}),
		4);
	EXPECT_EQ(B[0][0].count(), 4u);
	EXPECT_EQ(B[0][1].count(), 4u);
	EXPECT_EQ(B[1][1].count(), 4u);
	EXPECT_EQ(B[0][2].count(), 2u);
	EXPECT_EQ(B[0][3].count(), 2u);
	// col
	B = cB; // reset
	ASSERT_NO_THROW(
		remove_option_section(B, B.col(0), vL{L(0), L(1, 0)}, vV{b{"01010"}}));
	EXPECT_EQ(B[3][0].count(), 2u);
	B = cB; // reset
	EXPECT_EQ(
		remove_option_section(
			B, B.col(3), vL{L(0, 3), L(1, 3)}, vV{b{"01100"}}),
		4);
	EXPECT_EQ(B[0][3].count(), 4u);
	EXPECT_EQ(B[1][3].count(), 4u);
	EXPECT_EQ(B[2][3].count(), 2u);
	EXPECT_EQ(B[3][3].count(), 2u);
	B = cB; // reset
	EXPECT_THROW(
		remove_option_section(
			B, B.col(3), vL{L(0, 3), L(1, 3)}, vV{b{"11100"}}),
		::Sudoku::error::invalid_Board);
	// block
	B = cB; // reset
	ASSERT_NO_THROW(
		remove_option_section(B, B.block(0), vL{L(0), L(1)}, vV{b{"01010"}}));
	EXPECT_EQ(B[1][0].count(), 2u);
	EXPECT_EQ(
		remove_option_section(
			B, B.block(3), vL{L(2, 2), L(3, 1)}, vV{b{"10100"}}),
		6);
	EXPECT_EQ(B[0][0].count(), 4u);
	EXPECT_EQ(B[0][1].count(), 4u);
	EXPECT_EQ(B[1][0].count(), 2u);
	EXPECT_EQ(B[1][1].count(), 2u);
	// single ignore value
	B = cB; // reset
	ASSERT_EQ(B[3][1].count(), 4u);
	ASSERT_NO_THROW(remove_option_section(B, B.row(3), vL{L(3, 0)}, vV{v{3}}));
	EXPECT_EQ(B[3][1].count(), 3u);
}
TEST(Solver, remove_option_outside_block)
{
	using L = Location<2>;

	const Board<Options<4>, 2> cB1;
	Board<Options<4>, 2> B1;

	// row
	ASSERT_EQ(B1[0][1].count(), 4u);
	ASSERT_NO_THROW(remove_option_outside_block(B1, B1.row(0), L(0), Value{1}));
	EXPECT_EQ(remove_option_outside_block(B1, B1.row(1), L(1, 0), Value{3}), 2);
	// col
	ASSERT_NO_THROW(remove_option_outside_block(B1, B1.col(0), L(0), Value{1}));
	EXPECT_EQ(remove_option_outside_block(B1, B1.col(1), L(0, 1), Value{2}), 2);
	EXPECT_EQ(B1[0][2].count(), 3u);
	EXPECT_EQ(B1[0][3].count(), 3u);
	EXPECT_EQ(B1[2][0].count(), 3u);
	EXPECT_EQ(B1[2][1].count(), 3u);
	EXPECT_EQ(B1[0][1].count(), 4u);
	// block (no effect) disabled: static_assert
	// ASSERT_NO_THROW(remove_option_outside_block(B1, B1.block(0),
	// L(0), 1));
	// EXPECT_EQ(remove_option_outside_block(B1, B1.block(0), L(1, 1),
	// 4), 0);
	// element is answer
	B1[2][2] = Value{4};
	B1[2][3] = Value{3};
	ASSERT_TRUE(is_answer(B1[2][2], Value{4}));
	ASSERT_TRUE(is_answer(B1[2][3], Value{3}));
	EXPECT_EQ(remove_option_outside_block(B1, B1.row(2), L(2, 0), Value{2}), 0);
}

//===----------------------------------------------------------------------===//
TEST(Solver, deathtests_remove_option)
{
	using L  = Location<2>;
	using vL = std::vector<L>;
	using vV = Options<4>;
	using b  = std::bitset<5>;

	Board<Options<4>, 2> B{};
	const auto cB{B};
	//===------------------------------------------------------------------===//
	// remove_option_section(SectionT, Location ignore, int value)
	// death test: loc-> !is_answer(value)
	ASSERT_FALSE(B[0][0].is_answer());
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), L(0), Value{2}),
		"Assertion .*is_answer.*");
	// ignore is part of section
	B[0][0] = Value{2};
	ASSERT_TRUE(is_answer(B[0][0], Value{2}));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(1), L(0), Value{2}),
		"Assertion .*is_same_section.*");
	ASSERT_TRUE(is_answer(B[0][0], Value{2}));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.col(1), L(0), Value{2}),
		"Assertion .*is_same_section.*");
	ASSERT_TRUE(is_answer(B[0][0], Value{2}));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.block(1), L(0), Value{2}),
		"Assertion .*is_same_section.*");

	//===------------------------------------------------------------------===//
	// remove_option_section(SectionT, const std::vector<Location>& ignore, int)
	B = cB; // reset
	// what if ignore is empty?
	B[0][0] = Value{3};
	ASSERT_TRUE(is_answer(B[0][0], Value{3}));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{}, Value{3}),
		"Assertion .*is_valid\\(ignore\\)");
	// ignore-location out of bounds (1-sided, assume check tested earlier)
#ifndef NDEBUG
	B[0][0] = Value{3};
	ASSERT_TRUE(is_answer(B[0][0], Value{3}));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(1), L(16)}, Value{3}),
		"Assertion .*is_valid\\(ignore\\)");
#endif // NDEBUG
	B       = cB;
	B[0][0] = Value{3};
	// ignore-locations not sorted
	ASSERT_TRUE(is_answer(B[0][0], Value{3}));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(7), L(6)}, Value{3}),
		"Assertion .*is_valid\\(ignore\\)");
	// invalid value
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(1), L(3)}, Value{0}),
		"Assertion .*is_valid<N>\\(value\\)");
#endif // NDEBUG
	B = cB;
	// assert at least one ignore-location inside section
	EXPECT_DEBUG_DEATH(
		remove_option_section(
			B, B.row(0), vL{L(1, 0), L(1, 1), L(2, 1)}, Value{1}),
		"Assertion .*is_same_section.*");
	B = cB; // reset

	//===------------------------------------------------------------------===//
	// remove_option_section(SectionT, std::vector<Location>, std::vector<int>)
	// ignore is empty
#ifdef NDEBUG
	EXPECT_THROW(
		remove_option_section(B, B.row(0), vL{}, vV{b{"11010"}}),
		Sudoku::error::invalid_Board);
#else
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{}, vV{b{"11010"}}),
		"Assertion .*is_valid\\(ignore\\)");
#endif // NDEBUG
	   // values is all
#ifdef NDEBUG
	EXPECT_THROW(
		remove_option_section(B, B.row(0), vL{L(0), L(1), L(11)}, vV{}),
		Sudoku::error::invalid_Board);
#else
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(1), L(11)}, vV{}),
		"Assertion .*all");
#endif // NDEBUG

	// values is empty
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(1), L(11)}, vV{}.clear()),
		"Assertion .*is_empty");

	// assert at least one ignore-location inside section
	EXPECT_DEBUG_DEATH(
		remove_option_section(
			B, B.row(0), vL{L(1, 0), L(1, 1), L(2, 1)}, vV{b{"00110"}}),
		"Assertion .*is_same_section.*");
	B = cB; // reset

	//===------------------------------------------------------------------===//
	// remove_option_outside_block(Board, SectionT, Location, int)
	// invalid location
	B[0][0] = Value{3};
	ASSERT_TRUE(is_answer(B[0][0], Value{3}));

#ifdef NDEBUG
	EXPECT_THROW(
		remove_option_outside_block(B, B.row(0), L(21), Value{3}),
		Sudoku::error::invalid_Board);
#else
	EXPECT_DEBUG_DEATH(
		remove_option_outside_block(B, B.row(0), L(21), Value{3}),
		"Assertion .*is_valid\\(block_loc\\)");
#endif

	// invalid value
#ifndef NDEBUG
	B[0][0] = Value{3};
	ASSERT_TRUE(is_answer(B[0][0], Value{3}));
	EXPECT_DEBUG_DEATH(
		remove_option_outside_block(B, B.row(0), L(0), Value{23}),
		"Assertion .*is_valid<N>\\(value\\)");
#endif // NDEBUG
	// Locations block and section don't intersect
	ASSERT_TRUE(is_answer(B[0][0], Value{3}));
	EXPECT_DEBUG_DEATH(
		remove_option_outside_block(B, B.row(2), L(0), Value{3}),
		"Assertion .*intersect_block.*");
	B[3][2] = Value{4};
	ASSERT_TRUE(is_answer(B[3][2], Value{4}));
	EXPECT_DEBUG_DEATH(
		remove_option_outside_block(B, B.col(1), L(3, 2), Value{4}),
		"Assertion .*intersect_block.*");
}

} // namespace SudokuTests::SolversTest
