//===--	SudokuTests/Solver_remove_option.cpp							--===//
//
//	Unit tests for the option removing functions in Sudoku::Solver
//===---------------------------------------------------------------------===//
//	Implemented with GoogleTest
//
//	Notes:
//	gTest is limited for use with multiple template parameters.
//	These expressions need to be implemented between extra parentheses
//	- test elements implementing this are flagged with [gTest]
//	- not implemented tests are flagged as NEEDTEST [gTest]
//	gTest tries to print iterators if they use inheritance,
//		if used in *_EQ/NE etc.
//		use an explicit test like EXPECT_TRUE(.. == ..).
//
//===---------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include "../Sudoku/Solver.h"
#include "../Sudoku/Solvers_Appearance.h"
// helpers
#include "../Sudoku/Board.h"
#include "../Sudoku/Location.h"
#include "../Sudoku/Options.h"
// library
#include <bitset>
#include <vector>
#include <type_traits>


using namespace Sudoku;

namespace SudokuTests::SolversTest
{
TEST(Solver, remove_option)
{
	// remove_option(location, int value)
	Board<Options<4>, 2> board{};
	ASSERT_EQ(board[1][0], Options<4>{}) << "incorrect instantiation";
	// value is not an option -> return 0 (only value is missing)
	board[0][0] = std::bitset<5>{"11011"};
	board[1][0] = std::bitset<5>{"11011"};
	ASSERT_FALSE(board[0][0].is_option(2));
	EXPECT_NO_THROW(remove_option(board, Location<2>(0), 2));
	EXPECT_EQ(remove_option(board, Location<2>(1, 0), 2), 0);
	// location = answer (not value)
	board[0][1] = std::bitset<5>{"10000"}; // answer 4
	board[1][1] = std::bitset<5>{"10000"}; // answer 4
	EXPECT_NO_THROW(remove_option(board, Location<2>(1), 2));
	EXPECT_EQ(remove_option(board, Location<2>(1, 1), 2), 0);
	// location = answer (value)
	EXPECT_NO_THROW(remove_option(board, Location<2>(1), 4));
	EXPECT_EQ(remove_option(board, Location<2>(1), 2), 0);

	// normal remove from 4
	EXPECT_TRUE(board[2][1].is_option(4));
	EXPECT_EQ(board[2][1].count(), 4);
	EXPECT_EQ(remove_option(board, Location<2>(2, 1), 4), 1);
	// normal remove from 3 -> dual_option
	EXPECT_EQ(board[2][1].count(), 3);
	EXPECT_EQ(remove_option(board, Location<2>(2, 1), 3), 1);
	// normal remove from 2 -> single option
	EXPECT_EQ(board[2][1].count(), 2);
	EXPECT_GT(remove_option(board, Location<2>(2, 1), 2), 1);
	EXPECT_EQ(board[2][1].count(), 0);
	EXPECT_EQ(board[2][1].count_all(), 1);

	// SEE deathtest: last option (not answer)
	// check input
	// SEE deathtest: loc out of bounds
	// SEE deathtest: value out of bounds
}
TEST(Solver, remove_option_section)
{
	using L = Location<2>;
	// remove_option_section(SectionT, Location ignore, int value)
	// SEE deathtest: loc-> !is_answer(value)
	const Board<Options<4>, 2> cB1;
	Board<Options<4>, 2> B1;
	B1[0][0] = 1;
	ASSERT_TRUE(B1[0][0].is_answer(1));
	// row
	ASSERT_EQ(B1[0][1].count(), 4);
	EXPECT_NO_THROW(remove_option_section(B1, B1.row(0), L(0), 1));
	EXPECT_EQ(B1[0][1].count(), 3);
	B1[0][1] = 2;
	EXPECT_EQ(remove_option_section(B1, B1.row(0), L(0, 1), 2), 2);
	// trigger single_option
	B1[0][2] = 3;
	EXPECT_EQ(remove_option_section(B1, B1.row(0), L(0, 2), 3), 6);
	EXPECT_TRUE(B1[0][3].is_answer(4));
	EXPECT_EQ(B1[3][3].count(), 3);
	EXPECT_EQ(B1[1][3].count(), 3);
	EXPECT_EQ(B1[1][2].count(), 3);
	EXPECT_TRUE(B1[0][0].is_answer(1));
	EXPECT_TRUE(B1[0][1].is_answer(2));
	EXPECT_TRUE(B1[0][2].is_answer(3));
	EXPECT_TRUE(B1[3][0].all());

	// col
	B1       = cB1;
	B1[0][0] = 1;
	ASSERT_EQ(B1[1][0].count(), 4);
	EXPECT_NO_THROW(remove_option_section(B1, B1.col(0), L(0), 1));
	EXPECT_EQ(B1[1][0].count(), 3);
	B1[1][0] = 2;
	EXPECT_EQ(remove_option_section(B1, B1.col(0), L(1, 0), 2), 2);
	// trigger single_option
	B1[2][0] = 3;
	EXPECT_EQ(remove_option_section(B1, B1.col(0), L(2, 0), 3), 6);
	EXPECT_TRUE(B1[3][0].is_answer(4));
	EXPECT_EQ(B1[3][3].count(), 3);
	EXPECT_EQ(B1[3][1].count(), 3);
	EXPECT_EQ(B1[2][1].count(), 3);
	EXPECT_TRUE(B1[0][0].is_answer(1));
	EXPECT_TRUE(B1[1][0].is_answer(2));
	EXPECT_TRUE(B1[2][0].is_answer(3));
	EXPECT_TRUE(B1[2][3].all());

	// block
	B1       = cB1;
	B1[0][0] = 1;
	ASSERT_EQ(B1[1][1].count(), 4);
	EXPECT_NO_THROW(remove_option_section(B1, B1.block(0), L(0), 1));
	EXPECT_EQ(B1[1][1].count(), 3);
	B1[0][1] = 2;
	EXPECT_EQ(remove_option_section(B1, B1.block(0), L(0, 1), 2), 2);
	// trigger single_option
	B1[1][0] = 3;
	EXPECT_EQ(remove_option_section(B1, B1.block(0), L(1, 0), 3), 6);
	EXPECT_TRUE(B1[1][1].is_answer(4));
	EXPECT_EQ(B1[1][2].count(), 3);
	EXPECT_EQ(B1[1][3].count(), 3);
	EXPECT_EQ(B1[3][1].count(), 3);
	EXPECT_TRUE(B1[0][0].is_answer(1));
	EXPECT_TRUE(B1[0][1].is_answer(2));
	EXPECT_TRUE(B1[1][0].is_answer(3));
	EXPECT_TRUE(B1[3][3].all());

	// conflict handling? (none, use setValue)
	B1       = cB1;
	B1[0][0] = 2;
	B1[0][1] = 2;
	EXPECT_NO_THROW(remove_option_section(B1, B1.row(0), L(0), 2));
	EXPECT_TRUE(B1[0][0].is_answer(2));
	EXPECT_TRUE(B1[0][1].is_answer(2));
}
TEST(Solver, remove_option_section_1)
{
	using L  = Location<2>;
	using vL = std::vector<L>;

	// remove_option_section(SectionT, const std::vector<Location>& ignore, int
	Board<Options<4>, 2> B1;
	// SEE deathtest: when ignore is empty
	// row
	ASSERT_NO_THROW(remove_option_section(B1, B1.row(0), vL{L(0), L(1)}, 1));
	EXPECT_EQ(B1[0][3].count(), 3);
	EXPECT_EQ(remove_option_section(B1, B1.row(0), vL{L(0), L(1)}, 2), 2);
	EXPECT_EQ(B1[0][0].count(), 4);
	EXPECT_EQ(B1[0][1].count(), 4);
	EXPECT_EQ(B1[0][2].count(), 2);
	EXPECT_EQ(B1[0][3].count(), 2);
	// col
	ASSERT_NO_THROW(remove_option_section(B1, B1.col(0), vL{L(0), L(1, 0)}, 1));
	EXPECT_EQ(B1[3][0].count(), 3);
	EXPECT_EQ(remove_option_section(B1, B1.col(0), vL{L(0), L(1, 0)}, 2), 2);
	EXPECT_EQ(B1[0][0].count(), 4);
	EXPECT_EQ(B1[1][0].count(), 4);
	EXPECT_EQ(B1[2][0].count(), 2);
	EXPECT_EQ(B1[3][0].count(), 2);
	// block
	ASSERT_NO_THROW(remove_option_section(B1, B1.block(0), vL{L(0), L(1)}, 1));
	EXPECT_EQ(B1[1][0].count(), 3);
	EXPECT_EQ(remove_option_section(B1, B1.block(0), vL{L(0), L(1)}, 2), 2);
	EXPECT_EQ(B1[0][0].count(), 4);
	EXPECT_EQ(B1[0][1].count(), 4);
	EXPECT_EQ(B1[1][0].count(), 2);
	EXPECT_EQ(B1[1][1].count(), 2);
	// single ignore value
	ASSERT_EQ(B1[3][1].count(), 4);
	ASSERT_NO_THROW(remove_option_section(B1, B1.row(3), vL{L(3, 0)}, 3));
	EXPECT_EQ(B1[3][1].count(), 3);
}
TEST(Solver, remove_option_section_2)
{
	using L  = Location<2>;
	using vL = std::vector<L>;
	using v  = std::vector<unsigned int>;

	// remove_option_section(SectionT, vector<Location>, vecor<int>
	const Board<Options<4>, 2> cB;
	Board<Options<4>, 2> B;
	// row
	ASSERT_NO_THROW(
		remove_option_section(B, B.row(0), vL{L(0), L(1)}, v{1, 2}));
	EXPECT_EQ(B[0][3].count(), 2);
	EXPECT_EQ(
		remove_option_section(B, B.row(1), vL{L(1, 0), L(1, 1)}, v{3, 4}), 4);
	EXPECT_EQ(B[0][0].count(), 4);
	EXPECT_EQ(B[0][1].count(), 4);
	EXPECT_EQ(B[1][1].count(), 4);
	EXPECT_EQ(B[0][2].count(), 2);
	EXPECT_EQ(B[0][3].count(), 2);
	// col
	B = cB; // reset
	ASSERT_NO_THROW(
		remove_option_section(B, B.col(0), vL{L(0), L(1, 0)}, v{1, 3}));
	EXPECT_EQ(B[3][0].count(), 2);
	EXPECT_EQ(
		remove_option_section(B, 
			B.col(3), vL{L(0, 3), L(1, 3)}, v{2, 3, 4}),
		44);
	EXPECT_EQ(B[0][0].count(), 2);
	EXPECT_EQ(B[1][0].count(), 2);
	EXPECT_EQ(B[2][0].count(), 0);
	EXPECT_EQ(B[3][0].count(), 0);
	// block
	B = cB; // reset
	ASSERT_NO_THROW(
		remove_option_section(B, B.block(0), vL{L(0), L(1)}, v{1, 3}));
	EXPECT_EQ(B[1][0].count(), 2);
	EXPECT_EQ(
		remove_option_section(B, B.block(3), vL{L(2, 2), L(3, 1)}, v{2, 4}), 6);
	EXPECT_EQ(B[0][0].count(), 4);
	EXPECT_EQ(B[0][1].count(), 4);
	EXPECT_EQ(B[1][0].count(), 2);
	EXPECT_EQ(B[1][1].count(), 2);
	// single ignore value
	ASSERT_EQ(B[3][1].count(), 4);
	ASSERT_NO_THROW(remove_option_section(B, B.row(3), vL{L(3, 0)}, v{3}));
	EXPECT_EQ(B[3][1].count(), 2);
}
TEST(Solver, remove_option_outside_block)
{
	using L = Location<2>;

	const Board<Options<4>, 2> cB1;
	Board<Options<4>, 2> B1;

	// row
	ASSERT_EQ(B1[0][1].count(), 4);
	ASSERT_NO_THROW(
		Solver<2>(B1).remove_option_outside_block(B1.row(0), L(0), 1));
	EXPECT_EQ(
		Solver<2>(B1).remove_option_outside_block(B1.row(1), L(1, 0), 3), 2);
	// col
	ASSERT_NO_THROW(
		Solver<2>(B1).remove_option_outside_block(B1.col(0), L(0), 1));
	EXPECT_EQ(
		Solver<2>(B1).remove_option_outside_block(B1.col(1), L(0, 1), 2), 2);
	EXPECT_EQ(B1[0][2].count(), 3);
	EXPECT_EQ(B1[0][3].count(), 3);
	EXPECT_EQ(B1[2][0].count(), 3);
	EXPECT_EQ(B1[2][1].count(), 3);
	EXPECT_EQ(B1[0][1].count(), 4);
	// block (no effect) disabled: static_assert
	// ASSERT_NO_THROW(Solver<2>(B1).remove_option_outside_block(B1.block(0),
	// L(0), 1));
	// EXPECT_EQ(Solver<2>(B1).remove_option_outside_block(B1.block(0), L(1, 1),
	// 4), 0);
	// element is answer
	B1[2][2] = 4;
	B1[2][3] = 3;
	ASSERT_TRUE(B1[2][2].is_answer(4));
	ASSERT_TRUE(B1[2][3].is_answer(3));
	EXPECT_EQ(
		Solver<2>(B1).remove_option_outside_block(B1.row(2), L(2, 0), 2), 0);
}

//===---------------------------------------------------------------------===//
TEST(Solver, deathtests_remove_option)
{
	using L  = Location<2>;
	using vL = std::vector<L>;
	using v  = std::vector<unsigned int>;

	Board<Options<4>, 2> B{};
	//===-----------------------------------------------------------------===//
	// remove_option(location, int value)
	// last option (not answer)
	B[0][2] = std::bitset<5>{"01001"}; // 3
	EXPECT_DEBUG_DEATH(
		remove_option(B, Location<2>(2), 3), "Assertion failed: count > 0");
	// check input
	// loc out of bounds (2-sided)
#ifdef _DEBUG
	EXPECT_DEBUG_DEATH(
		remove_option(B, Location<2>(-1), 3), "Assertion failed: is_valid");
	EXPECT_DEBUG_DEATH(
		remove_option(B, Location<2>(20), 3), "Assertion failed: is_valid");
	// value out of bounds (2-sided)
	EXPECT_DEBUG_DEATH(
		remove_option(B, Location<2>(2), 9),
		"Assertion failed: is_valid_value");
	EXPECT_DEBUG_DEATH(
		remove_option(B, Location<2>(2), 0),
		"Assertion failed: is_valid_value");
#endif // _DEBUG

	//===-----------------------------------------------------------------===//
	// remove_option_section(SectionT, Location ignore, int value)
	// deathtest: loc-> !is_answer(value)
	ASSERT_FALSE(B[0][0].is_answer());
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), L(0), 2),
		"Assertion failed: .*is_answer.*");
	// ignore is part of section
	B[0][0] = 2;
	ASSERT_TRUE(B[0][0].is_answer(2));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(1), L(0), 2),
		"Assertion failed: is_same_section.*");
	ASSERT_TRUE(B[0][0].is_answer(2));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.col(1), L(0), 2),
		"Assertion failed: is_same_section.*");
	ASSERT_TRUE(B[0][0].is_answer(2));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.block(1), L(0), 2),
		"Assertion failed: is_same_section.*");

	//===-----------------------------------------------------------------===//
	// remove_option_section(SectionT, const std::vector<Location>& ignore, int)
	// what if ignore is empty?
	B[0][0] = 3;
	ASSERT_TRUE(B[0][0].is_answer(3));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{}, 3),
		"Assertion failed: is_valid.ignore.");
	// ignore-location out of bounds (1-sided, assume check tested earlier)
#ifdef _DEBUG
	B[0][0] = 3;
	ASSERT_TRUE(B[0][0].is_answer(3));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(1), L(16)}, 3),
		"Assertion failed: is_valid.ignore.");
#endif // _DEBUG
	// ignore-locations not sorted
	ASSERT_TRUE(B[0][0].is_answer(3));
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(7), L(6)}, 3),
		"Assertion failed: is_valid.ignore.");
	// invalid value
#ifdef _DEBUG
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(1), L(3)}, 0),
		"Assertion failed: is_valid_value<N>.value.");
#endif // _DEBUG
	// assert atleast one ignore-location inside section
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(1, 0), L(1, 1), L(2, 1)}, 1),
		"Assertion failed: is_same_section.*");

	//===-----------------------------------------------------------------===//
	// remove_option_section(SectionT, std::vector<Location>, std::vector<int>)
	// ignore is empty
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{}, v{3, 1, 4}),
		"Assertion failed: is_valid.ignore.");
	// values is empty
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, 
			B.row(0), vL{L(0), L(1), L(11)}, v{}),
		"Assertion failed: is_valid_value");
	// an value out-of-bounds
#ifdef _DEBUG
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(1), L(11)}, v{1, 2, 0}),
		"Assertion failed: is_valid_value");
	EXPECT_DEBUG_DEATH(
		remove_option_section(B, B.row(0), vL{L(0), L(1), L(11)}, v{1, 2, 5}),
		"Assertion failed: is_valid_value");
#endif // _DEBUG
	// assert atleast one ignore-location inside section
	EXPECT_DEBUG_DEATH(
		remove_option_section(
			B, B.row(0), vL{L(1, 0), L(1, 1), L(2, 1)}, v{1, 2}),
		"Assertion failed: is_same_section.*");

	//===-----------------------------------------------------------------===//
	// remove_option_outside_block(SectionT, Location, int)
	// invalid location
	B[0][0] = 3;
	ASSERT_TRUE(B[0][0].is_answer(3));
	EXPECT_DEBUG_DEATH(
		Solver<2>(B).remove_option_outside_block(B.row(0), L(21), 3),
		"Assertion failed: is_valid.block_loc.");
	// invalid value
#ifdef _DEBUG
	B[0][0] = 3;
	ASSERT_TRUE(B[0][0].is_answer(3));
	EXPECT_DEBUG_DEATH(
		Solver<2>(B).remove_option_outside_block(B.row(0), L(0), 23),
		"Assertion failed: is_valid_value<N>.value.");
#endif // _DEBUG
	// Locations block and section don't intersect
	ASSERT_TRUE(B[0][0].is_answer(3));
	EXPECT_DEBUG_DEATH(
		Solver<2>(B).remove_option_outside_block(B.row(2), L(0), 3),
		"Assertion failed: intersect_block.*");
	B[3][2] = 4;
	ASSERT_TRUE(B[3][2].is_answer(4));
	EXPECT_DEBUG_DEATH(
		Solver<2>(B).remove_option_outside_block(B.col(1), L(3, 2), 4),
		"Assertion failed: intersect_block.*");
}

} // namespace SudokuTests::SolversTest
