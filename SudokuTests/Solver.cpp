// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//===--- SudokuTests/Solver.cpp                                         ---===//
//
//	Unit tests for the template class Sudoku::Solver
//===----------------------------------------------------------------------===//
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
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Solver.h>
// helpers
#include <Sudoku/Board.h>
#include <Sudoku/Location.h>
#include <Sudoku/Location_Utilities.h>
#include <Sudoku/Options.h>
#include <Sudoku/exceptions.h>
// Debug Output
#include "print_Options.h"
// library
#include <array>
#include <bitset>
#include <vector>
#include <algorithm> // std::copy
#include <stdexcept>
#include <type_traits>


namespace SudokuTests::SolversTest
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Options;
using ::Sudoku::Value;

TEST(Solver, unique_in_section)
{
	// clang-format off
	const std::array<char, 16> v1
	{
		// start	// after set_Value
		0,2, 0,0,	// 1	2	3	4
		4,0, 0,0,	// 4	3	1,2	1,2
		0,1, 4,0,	// 2,3	1	4	2,3
		0,0, 0,0	// 2,3	4	1,2	1,2,3
	}; // clang-format on
	Board<Options<4>, 2> B1;
	ASSERT_EQ(B1[0][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(set_Value(B1, v1.cbegin(), v1.cend()));
	const Board<Options<4>, 2> cB1{B1}; // copy to compare with
	EXPECT_EQ(B1[0][1], Value{2}) << "setup error";
	EXPECT_EQ(B1[1][0], Value{4}) << "setup error";
	EXPECT_EQ(B1[2][2], Value{4}) << "setup error";
	EXPECT_EQ(B1, cB1) << "copy error";
	// single row
	EXPECT_NO_THROW(unique_in_section(B1, B1.row(0)));
	EXPECT_EQ(B1, cB1) << "row 0 was completely fixed by set_Value";

	// clang-format off
	const std::array<char, 16> v2
	{
		//start		// after set_Value
		3,2, 0,0,	// 3	2	14	14		3	2	14	14
		0,0, 0,0,	// 14	14	3	2		14	14	3	2
		0,0, 2,0,	// 14	134	2	134		14	134	2	134
		0,0, 0,0	// 12.4	134	134	134		2	134	14	134
	}; // clang-format on

	Board<Options<4>, 2> B2{}; // working copy
	EXPECT_NO_THROW(set_Value(B2, v2.cbegin(), v2.cend()));
	const Board<Options<4>, 2> cB2{B2}; // copy to compare with
	// single row 0
	EXPECT_NO_THROW(unique_in_section(B2, B2.row(0)));
	EXPECT_EQ(B2, cB2) << "row 0 was completely fixed by set_Value";
	// single row 1
	EXPECT_NO_THROW(unique_in_section(B2, B2.row(3)));
	EXPECT_NE(B2, cB2) << "row 3 should have changed";
	// full board
	for (int i = 0; i < ::Sudoku::elem_size<2>; ++i)
	{
		EXPECT_NO_THROW(unique_in_section(B2, B2.row(i)));
	}
	EXPECT_EQ(B2[0][0], Value{3}) << "input values are lost";
	EXPECT_EQ(B2[0][1], Value{2});
	EXPECT_EQ(B2[2][2], Value{2});
	EXPECT_EQ(B2[1][3], Value{2}) << "not all uniques found in rows";
	EXPECT_EQ(B2[3][0], Value{2});

	// clang-format off
	const std::array<char, 16> v3
	{
		// start	// after set_Value
		0,0, 1,0,	// 
		1,0, 0,0,	// 
		0,1, 0,0,	// 
		0,0, 0,0	//					//	0	0	0	1
	}; // clang-format on
	Board<Options<4>, 2> B3;
	// reset
	const auto reset_B3 = [&]() {
		B3.clear();
		ASSERT_TRUE(B3[0][0].all());
		EXPECT_NO_THROW(set_Value(B3, v3.cbegin(), v3.cend()));
	};
	reset_B3();
	EXPECT_EQ(B3[3][3].count(), 4U);
	EXPECT_TRUE(is_option(B3[3][3], Value{1}));
	// Row()
	EXPECT_NO_THROW(unique_in_section(B3, B3.row(3)));
	EXPECT_TRUE(is_answer(B3[3][3], Value{1}));
	// Col()
	reset_B3();
	EXPECT_NO_THROW(unique_in_section(B3, B3.col(3)));
	EXPECT_TRUE(is_answer(B3[3][3], Value{1}));
	// BLock()
	reset_B3();
	EXPECT_NO_THROW(unique_in_section(B3, B3.block(3)));
	EXPECT_TRUE(is_answer(B3[3][3], Value{1}));
}

// NOLINTNEXTLINE(readability-function-size)
TEST(Solver, section_exclusive)
{
	// section_exclusive(SectionT)
	// section_exclusive(Block)

	// same result as unique_in_section
	{
		// clang-format off
		const std::array<char, 16> v3
		{
			// start	// after set_Value
			0,0, 1,0,	// 
			1,0, 0,0,	// 
			0,1, 0,0,	// 
			0,0, 0,0	//					//	0	0	0	1
		}; // clang-format on
		Board<Options<4>, 2> B3;
		// reset
		const auto reset_B3 = [&]() {
			B3.clear();
			ASSERT_TRUE(B3[0][0].all());
			EXPECT_NO_THROW(set_Value(B3, v3.cbegin(), v3.cend()));
		};
		reset_B3();
		EXPECT_EQ(B3[3][3].count(), 4U);
		EXPECT_TRUE(is_option(B3[3][3], Value{1}));
		// Row()
		EXPECT_NO_THROW(section_exclusive(B3, B3.row(3)));
		EXPECT_TRUE(is_answer(B3[3][3], Value{1}));
		// Col()
		reset_B3();
		EXPECT_NO_THROW(section_exclusive(B3, B3.col(3)));
		EXPECT_TRUE(is_answer(B3[3][3], Value{1}));
		// BLock()
		reset_B3();
		EXPECT_NO_THROW(section_exclusive(B3, B3.block(3)));
		EXPECT_TRUE(is_answer(B3[3][3], Value{1}));
	}
	// same result as set_section_locals
	{
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
		ASSERT_TRUE(B[0][1].all());
		ASSERT_TRUE(B[0][3].all());
		EXPECT_EQ(section_exclusive(B, B.row(0)), 0);
		EXPECT_EQ(B[0][0].count(), 2U);
		EXPECT_TRUE(B[0][1].all());
		EXPECT_EQ(B[0][2].count(), 2U);
		EXPECT_TRUE(B[0][3].all());
		EXPECT_TRUE(B[1][0].all());
		EXPECT_TRUE(B[1][1].all());
		//	2 values in row, and in same block
		B[2][0] = set{"11001"};
		B[2][1] = set{"11001"};
		ASSERT_TRUE(B[2][2].all());
		ASSERT_TRUE(B[2][3].all());
		ASSERT_TRUE(B[3][2].all());
		EXPECT_EQ(section_exclusive(B, B.row(2)), 4);
		EXPECT_TRUE(B[3][0].all());
		EXPECT_EQ(B[2][2].count(), 4U); // self
		EXPECT_EQ(B[2][3].count(), 4U); // self
		EXPECT_EQ(B[3][2].count(), 2U); // rest block
		EXPECT_EQ(B[1][2].count(), 4U);

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
		EXPECT_EQ(section_exclusive(B, B.block(0)), 0);
		EXPECT_TRUE(B[0][1].all());
		EXPECT_TRUE(B[0][3].all());
		EXPECT_TRUE(B[1][0].all());
		EXPECT_TRUE(B[1][3].all());
		EXPECT_TRUE(B[3][0].all());
		// same block, same row
		B[2][0] = set{"11001"};
		B[2][1] = set{"11001"};
		EXPECT_EQ(section_exclusive(B, B.block(2)), 4);
		EXPECT_TRUE(B[3][0].all());     // self block
		EXPECT_TRUE(B[3][1].all());     // self block
		EXPECT_EQ(B[3][2].count(), 2U); // rest row
		EXPECT_EQ(B[3][3].count(), 2U); // rest row
		EXPECT_TRUE(B[0][1].all());     // rest col
		EXPECT_TRUE(B[0][2].all());
		EXPECT_TRUE(B[0][3].all());
		// same block, same col
		B       = cB; // reset board
		B[0][1] = set{"00111"};
		B[1][1] = set{"00111"};
		EXPECT_EQ(section_exclusive(B, B.block(0)), 4);
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
		const Board<Options<9>, 3> cB3{};
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
		EXPECT_EQ(section_exclusive(B3, B3.block(0)), 3 * 6);
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
		B3       = cB3;
		B3[0][1] = Value{2};
		B3[0][2] = Value{3};
		B3[1][1] = Value{5};
		B3[1][2] = Value{6};
		B3[2][1] = Value{8};
		B3[2][2] = Value{9};
		EXPECT_EQ(section_exclusive(B3, B3.block(0)), 3 * 6);
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
		B3       = cB3;
		B3[0][1] = Value{2};
		B3[0][2] = Value{3};
		B3[1][1] = Value{5};
		B3[1][2] = Value{6};
		B3[2][0] = Value{7};
		B3[2][2] = Value{9};
		EXPECT_EQ(section_exclusive(B3, B3.block(0)), 0);
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

	{
		// clang-format off
		constexpr std::array<char, 81> b1
		{
			0, 0, 0,	0, 0, 0,	0, 1, 2,
			0, 0, 0,	0, 3, 5,	0, 0, 0,
			0, 0, 0,	6, 0, 0,	0, 7, 0,
			7, 0, 0,	0, 0, 0,	3, 0, 0,
			0, 0, 0,	4, 0, 0,	8, 0, 0,
			1, 0, 0,	0, 0, 0,	0, 0, 0,
			0, 0, 0,	1, 2, 0,	0, 0, 0,
			0, 8, 0,	0, 0, 0,	0, 4, 0,
			0, 5, 0,	0, 0, 0,	6, 0, 0
		};
		constexpr std::array<char, 81> b1a	// requires unique
		{
			6, 7, 3,	8, 9, 4,	5, 1, 2,
			9, 1, 2,	7, 3, 5,	4, 8, 6,
			8, 4, 5,	6, 1, 2,	9, 7, 3,
			7, 9, 8,	2, 6, 1,	3, 5, 4,
			5, 2, 6,	4, 7, 3,	8, 9, 1,
			1, 3, 4,	5, 8, 9,	2, 6, 7,
			4, 6, 9,	1, 2, 8,	7, 3, 5,
			2, 8, 7,	3, 5, 6,	1, 4, 9,
			3, 5, 1,	9, 4, 7,	6, 2, 8
		};
		// clang-format on
		Board<Options<9>, 3> B1{};
		set_Value(B1, b1.cbegin(), b1.cend());
		Board<Options<9>, 3> A1{};
		set_Value(A1, b1a.cbegin(), b1a.cend());
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.row(0)), 6);
		EXPECT_EQ(section_exclusive(B1, B1.row(1)), 2);
		EXPECT_EQ(section_exclusive(B1, B1.row(2)), 8);
		EXPECT_EQ(section_exclusive(B1, B1.row(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(6)), 2);
		EXPECT_EQ(section_exclusive(B1, B1.row(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.col(0)), 3);
		EXPECT_EQ(section_exclusive(B1, B1.col(1)), 6);
		EXPECT_EQ(section_exclusive(B1, B1.col(2)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(6)), 74);
		EXPECT_EQ(section_exclusive(B1, B1.col(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.block(0)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(1)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(2)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(6)), 35);
		EXPECT_EQ(section_exclusive(B1, B1.block(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.row(0)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(1)), 27);
		EXPECT_EQ(section_exclusive(B1, B1.row(2)), 15);
		EXPECT_EQ(section_exclusive(B1, B1.row(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(4)), 5);
		EXPECT_EQ(section_exclusive(B1, B1.row(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(6)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.col(0)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(1)), 30);
		EXPECT_EQ(section_exclusive(B1, B1.col(2)), 5);
		EXPECT_EQ(section_exclusive(B1, B1.col(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(5)), 86);
		EXPECT_EQ(section_exclusive(B1, B1.col(6)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(8)), 0);
		EXPECT_EQ(A1, B1);
	}
	{
		// clang-format off
		constexpr std::array<char, 81> b5
		{
			4, 0, 0,	0, 0, 0,	0, 3, 8,
			0, 0, 2,	0, 0, 4,	1, 0, 0,
			0, 0, 5,	3, 0, 0,	2, 4, 0,

			0, 7, 0,	6, 0, 9,	0, 0, 4,
			0, 2, 0,	0, 0, 0,	0, 7, 0,
			6, 0, 0,	7, 0, 3,	0, 9, 0,

			0, 5, 7,	0, 0, 8,	3, 0, 0,
			0, 0, 3,	9, 0, 0,	4, 0, 0,
			2, 4, 0,	0, 0, 0,	0, 0, 9
		};
		constexpr std::array<char, 81> b5a // requires double_option, not unique
		{
			4, 6, 1,	5, 7, 2,	9, 3, 8,
			7, 3, 2,	8, 9, 4,	1, 5, 6,
			8, 9, 5,	3, 1, 6,	2, 4, 7,

			3, 7, 8,	6, 2, 9,	5, 1, 4,
			5, 2, 9,	4, 8, 1,	6, 7, 3,
			6, 1, 4,	7, 5, 3,	8, 9, 2,

			9, 5, 7,	2, 4, 8,	3, 6, 1,
			1, 8, 3,	9, 6, 7,	4, 2, 5,
			2, 4, 6,	1, 3, 5,	7, 8, 9
		};
		// clang-format on
		Board<Options<9>, 3> B5{};
		set_Value(B5, b5.cbegin(), b5.cend());
		Board<Options<9>, 3> A5{};
		set_Value(A5, b5a.cbegin(), b5a.cend());
		EXPECT_NE(A5, B5);

		EXPECT_EQ(section_exclusive(B5, B5.row(0)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(1)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(2)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(3)), 6);
		EXPECT_EQ(section_exclusive(B5, B5.row(4)), 9);
		EXPECT_EQ(section_exclusive(B5, B5.row(5)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(6)), 5);
		EXPECT_EQ(section_exclusive(B5, B5.row(7)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(8)), 5);
		EXPECT_NE(A5, B5);

		EXPECT_EQ(section_exclusive(B5, B5.col(0)), 16);
		EXPECT_EQ(section_exclusive(B5, B5.col(1)), 5);
		EXPECT_EQ(section_exclusive(B5, B5.col(2)), 7);
		EXPECT_EQ(section_exclusive(B5, B5.col(3)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(4)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(5)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(6)), 13);
		EXPECT_EQ(section_exclusive(B5, B5.col(7)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(8)), 0);
		EXPECT_NE(A5, B5);

		EXPECT_EQ(section_exclusive(B5, B5.block(0)), 17);
		EXPECT_EQ(section_exclusive(B5, B5.block(1)), 98);
		EXPECT_EQ(section_exclusive(B5, B5.block(2)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.block(3)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.block(4)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.block(5)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.block(6)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.block(7)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.block(8)), 0);
		EXPECT_EQ(A5, B5);

		// Triggered a bug while testing, on B5
		Board<Options<9>, 3> B6{};
		const auto& A6 = A5;
		set_Value(B6, b5.cbegin(), b5.cend());
		ASSERT_NE(A6, B6);

		EXPECT_EQ(section_exclusive(B6, B6.row(0)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.col(0)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.block(0)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.row(1)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.col(1)), 8);
		EXPECT_EQ(section_exclusive(B6, B6.block(1)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.row(2)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.col(2)), 9);
		EXPECT_NO_FATAL_FAILURE(section_exclusive(B6, B6.block(2)));
	}

	// reproduce functionality of unique
	//===------------------------------------------------------------------===//
	// clang-format off
	constexpr std::array<char, 16> V1
	{
		// start	// after set_Value	// unique_block
		0,0, 1,0,	// 
		1,0, 0,0,	// 
		0,1, 0,0,	// 
		0,0, 0,0	//					//	0	0	0	1
	}; // clang-format on
	Board<Options<4>, 2> B1{};
	set_Value(B1, V1.cbegin(), V1.cend());
	// check input:
	{
		EXPECT_FALSE(is_answer(B1[0][0]));
		EXPECT_FALSE(is_answer(B1[0][1]));
		EXPECT_EQ(B1[0][2], Value{1});
		EXPECT_FALSE(is_answer(B1[0][3]));
		EXPECT_EQ(B1[1][0], Value{1});
		EXPECT_FALSE(is_answer(B1[1][1]));
		EXPECT_FALSE(is_answer(B1[1][2]));
		EXPECT_FALSE(is_answer(B1[1][3]));
		EXPECT_FALSE(is_answer(B1[2][0]));
		EXPECT_EQ(B1[2][1], Value{1});
		EXPECT_FALSE(is_answer(B1[2][2]));
		EXPECT_FALSE(is_answer(B1[2][3]));
		EXPECT_FALSE(is_answer(B1[3][0]));
		EXPECT_FALSE(is_answer(B1[3][1]));
		EXPECT_FALSE(is_answer(B1[3][2]));
		EXPECT_FALSE(is_answer(B1[3][3]));
	}
	// block_exclusive:
	EXPECT_EQ(section_exclusive(B1, B1.block(3)), 4)
		<< "section_exclusive(Block) should remove 4 options";
	EXPECT_EQ(B1[3][3], Value{1})
		<< "section_exclusive(Block) unique value failed";
	int found1{0};
	for (int i{0}; i < ::Sudoku::elem_size<2>; ++i)
	{
		found1 += section_exclusive(B1, B1.block(i));
	}
	EXPECT_EQ(found1, 0) << "shouldn't find any others";

	// working for more than 1 unique (use set_section_locals)
	//===------------------------------------------------------------------===//
	// clang-format off
	constexpr std::array<char, 81> V2
	{
	/*	start board					answer board
	*	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
	*	|		| 		|   1 2 |	| 6 7 3	| 8 9 4	| 5 1 2 |
	*	|		|	3 5 |		|	| 9 1 2	| 7 3 5 | 4 8 6	|
	*	| _ _ _ | 6 _ _ | _ 7 _ |	|_8_4_5_|_6_1_2_|_9_7_3_|
	*	| 7		|		| 3		|	| 7 9 8	| 2 6 1	| 3 5 4 |
	*	|		| 4		| 8		|	| 5 2 6	| 4 7 3	| 8 9 1	|
	*	| 1 _ _ | _ _ _	| _ _ _ |	|_1_3_4_|_5_8_9_|_2_6_7_|
	*	| 		| 1 2	|		|	| 4 6 9	| 1 2 8 | 7 3 5	|
	*	|	8	|		|	4	|	| 2 8 7	| 3 5 6	| 1 4 9	|
	*	|_ _5_ _|_ _ _ _|_6_ _ _|	|_3_5_1_|_9_4_7_|_6_2_8_|
	*/
		0, 0, 0,	0, 0, 0,	0, 1, 2,	//	
		0, 0, 0,	0, 3, 5,	0, 0, 0,	//	. . .	2 . .
		0, 0, 0,	6, 0, 0,	0, 7, 0,	//	. . .	. 1 12	0 . [3]

		7, 0, 0,	0, 0, 0,	3, 0, 0,	//
		0, 0, 0,	4, 0, 0,	8, 0, 0,	//
		1, 0, 0,	0, 0, 0,	0, 0, 0,	//

		0, 0, 0,	1, 2, 0,	0, 0, 0,
		0, 8, 0,	0, 0, 0,	0, 4, 0,	//	
		0, 5, 0,	0, 0, 0,	6, 0, 0		//	. 5 [1] |	
	};
	// clang-format on
	Sudoku::Board<Options<9>, 3> B2{};
	set_Value(B2, V2.cbegin(), V2.cend());
	// block 0
	//		nothing
	// block 1
	//		double: 1 in row 2;	2 not paired
	// block 2
	//		unique: 8=3
	const int count_s = section_exclusive(B2, B2.block(2));

	EXPECT_GE(count_s, 1)
		<< "section_exclusive(block) should find at least 1 value";
	EXPECT_EQ(B2[2][8], Value{3})
		<< "section_exclusive(block) unique value failed N=3";
	for (int i = 0; i < ::Sudoku::elem_size<3>; ++i)
	{
		EXPECT_NO_THROW(section_exclusive(B2, B2.block(i)));
	}
}

TEST(Solver, single_option)
{
	using Sudoku::error::invalid_Location;
	using Sudoku::error::invalid_Board;
	using L = Location<2>;

	Board<Options<4>, 2> B1;
	ASSERT_EQ(B1[1][0], Options<4>{}) << "incorrect instantiation";

	//====----------------------------------------------------------------====//
	static_assert(not noexcept(single_option(B1, L(), Value{1})));
	// set_Value: throws for invalid Location
#ifndef NDEBUG
	EXPECT_DEATH(single_option(B1, L{-1}, Value{1}), "is_valid\\(loc\\)");
	EXPECT_DEATH(single_option(B1, L{16}, Value{1}), "is_valid\\(loc\\)");
#else
	EXPECT_THROW(single_option(B1, L{-1}, Value{1}), invalid_Location);
	EXPECT_THROW(single_option(B1, L{16}, Value{1}), invalid_Location);
#endif // NDEBUG
	// invalid Value:
	EXPECT_DEBUG_DEATH(
		single_option(B1, L{0}, Value{0}), "is_valid<N>\\(value\\)");
#ifdef NDEBUG
	EXPECT_THROW(single_option(B1, L(1), Value{1}), invalid_Board);
	EXPECT_THROW(single_option(B1, L(2), Value{4}), invalid_Board);
	// thrown by std::bitset
	EXPECT_THROW(single_option(B1, L{3}, Value{5}), std::out_of_range);
#else
	EXPECT_NO_THROW(single_option(B1, L(1), Value{1}));
	EXPECT_NO_THROW(single_option(B1, L(2), Value{4}));
	EXPECT_DEBUG_DEATH(
		single_option(B1, L{0}, Value{5}), "is_valid<N>\\(value\\)");
#endif // NDEBUG
	// not an option:
	B1[0][0] = Options<4>{Value{2}};
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(single_option(B1, L{0}, Value{1}), "test\\(value\\)");
	EXPECT_NO_THROW(single_option(B1, L(0), Value{2}));
#else
	EXPECT_THROW(single_option(B1, L{0}, Value{1}), invalid_Board);
	EXPECT_THROW(single_option(B1, L(0), Value{2}), invalid_Board);
#endif // NDEBUG

	//====----------------------------------------------------------------====//
	// is single option (loc, value)
	B1.clear();
	ASSERT_EQ(B1[1][0], Options<4>{}) << "incorrect instantiation";
	B1[0][1] = std::bitset<5>{"00101"}; // 2
	ASSERT_FALSE(is_answer(B1[0][1], Value{2}));
	ASSERT_TRUE(is_option(B1[0][1], Value{2}));
	EXPECT_EQ(B1[0][0].count(), 4U);
	EXPECT_TRUE(B1[0][2].count() == 4U && B1[0][3].count() == 4U);
	EXPECT_EQ(B1[1][1].count(), 4U);
	EXPECT_TRUE(B1[2][1].count() == 4U && B1[3][1].count() == 4U);
	EXPECT_NO_THROW(single_option(B1, L(1), Value{2}));
	EXPECT_EQ(B1[0][0].count(), 3U);
	EXPECT_FALSE(is_option(B1[0][0], Value{2}));
	EXPECT_FALSE(
		is_option(B1[0][2], Value{2}) || is_option(B1[0][3], Value{2}));
	// is answer (loc, value)
	B1[1][0] = Value{4};
	ASSERT_TRUE(is_answer(B1[1][0], Value{4}));
	EXPECT_EQ(B1[1][2].count(), 4U);
	EXPECT_EQ(B1[2][0].count(), 4U);
	EXPECT_NO_THROW(single_option(B1, L(1, 0), Value{4}));
	EXPECT_EQ(B1[0][0].count(), 2U);
	EXPECT_EQ(B1[1][2].count(), 3U);
	// cascade
	B1[2][1] = Value{1};
	ASSERT_TRUE(is_answer(B1[2][1], Value{1}));
	EXPECT_EQ(B1[0][0].count(), 2U);
	EXPECT_EQ(B1[1][1].count(), 2U);
	EXPECT_NO_THROW(single_option(B1, L(2, 1), Value{1}));
	EXPECT_TRUE(is_answer(B1[1][1], Value{3}));
	EXPECT_TRUE(is_answer(B1[0][0], Value{1}));
	EXPECT_TRUE(is_answer(B1[3][1], Value{4}));

	// single_option(Location)
	Board<Options<4>, 2> B2;
	ASSERT_EQ(B2[1][0], Options<4>{}) << "incorrect instantiation";
	// more than 1 option available
	ASSERT_NO_THROW(single_option(B2, L(1)));
	EXPECT_EQ(single_option(B2, L(1)), 0);
	B2[0][1] = std::bitset<5>{"00101"}; // 2
	ASSERT_FALSE(is_answer(B2[0][1], Value{2}));
	ASSERT_TRUE(is_option(B2[0][1], Value{2}));
	EXPECT_EQ(B2[0][0].count(), 4U);
	EXPECT_TRUE(B2[0][2].count() == 4U && B2[0][3].count() == 4U);
	EXPECT_EQ(B2[1][1].count(), 4U);
	EXPECT_TRUE(B2[2][1].count() == 4U && B2[3][1].count() == 4U);
	EXPECT_EQ(single_option(B2, L(1)), 8);
	EXPECT_EQ(B2[0][0].count(), 3U);
	EXPECT_FALSE(is_option(B2[0][0], Value{2}));
	EXPECT_FALSE(
		is_option(B2[0][2], Value{2}) || is_option(B2[0][3], Value{2}));
	// is answer (loc, value)
	B2[1][0] = Value{4};
	ASSERT_TRUE(is_answer(B2[1][0], Value{4}));
	EXPECT_EQ(B2[1][2].count(), 4U);
	EXPECT_EQ(B2[2][0].count(), 4U);
	EXPECT_EQ(single_option(B2, L(1, 0)), 6);
	EXPECT_EQ(B2[0][0].count(), 2U);
	EXPECT_EQ(B2[1][2].count(), 3U);
	// cascade
	B2[2][1] = Value{1};
	ASSERT_TRUE(is_answer(B2[2][1], Value{1}));
	EXPECT_EQ(B2[0][0].count(), 2U);
	EXPECT_EQ(B2[1][1].count(), 2U);
	EXPECT_EQ(single_option(B2, L(2, 1)), 17);
	EXPECT_TRUE(is_answer(B2[1][1], Value{3}));
	EXPECT_TRUE(is_answer(B2[0][0], Value{1}));
	EXPECT_TRUE(is_answer(B2[3][1], Value{4}));

	// set_value, with single_option should give the same result
	Board<Options<4>, 2> B3;
	// clang-format off
	constexpr std::array<char, 16> v1
	{
		// start	// after set_Value
		0,2, 0,0,	// 1	2	34	34
		4,0, 0,0,	// 4	3	12	12
		0,1, 0,0,	// 23	1	234	234
		0,0, 0,0	// 23	4	123	123
	}; // clang-format on
	ASSERT_EQ(B3[1][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(set_Value(B3, v1.cbegin(), v1.cend()));
	EXPECT_EQ(B1, B3);
	{ // when more than 1 option available
		Board<Options<4>, 2> B{};
		B[1][2] = std::bitset<5>{"10011"}; // 1, 4
		EXPECT_NO_THROW(single_option(B, L(1, 2), Value{1}));
	}
}

TEST(Solver, dual_option)
{
	using ::Sudoku::error::invalid_Board;
	using ::Sudoku::error::invalid_Location;

	Board<Options<4>, 2> board{};
	static_assert(not noexcept(dual_option(board, Location<2>())));

	// invalid Loc
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		dual_option(board, Location<2>{-1}), "is_valid\\(loc\\)");
	EXPECT_DEBUG_DEATH(
		dual_option(board, Location<2>{16}), "is_valid\\(loc\\)");
#else
	EXPECT_THROW(dual_option(board, Location<2>{-1}), invalid_Location);
	EXPECT_THROW(dual_option(board, Location<2>{16}), invalid_Location);
#endif // NDEBUG
	board[0][0] = Options<4>{std::bitset<5>{"11001"}};
	board[3][3] = Options<4>{std::bitset<5>{"10101"}};
	EXPECT_NO_THROW(dual_option(board, Location<2>{0}));
	EXPECT_NO_THROW(dual_option(board, Location<2>{15}));
	// count != 2
	board[0][1] = Options<4>{};
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(dual_option(board, Location<2>{1}), "count\\(\\) == 2");
#else
	EXPECT_THROW(dual_option(board, Location<2>{1}), invalid_Board);
#endif
	// invalid board
	board.clear();
	board[0][0] = Options<4>{std::bitset<5>{"10101"}};
	board[0][2] = Options<4>{std::bitset<5>{"10101"}};
	board[0][3] = Options<4>{std::bitset<5>{"10101"}};
	EXPECT_THROW(dual_option(board, Location<2>{0}), invalid_Board);

	// return type
	static_assert(
		std::is_same_v<int, decltype(dual_option(board, Location<2>()))>);

	board.clear();
	board[0][1] = Options<4>{std::bitset<5>{"11001"}};
	board[0][2] = Options<4>{std::bitset<5>{"11001"}};
	EXPECT_EQ(dual_option(board, Location<2>(0, 1)), 4); // same row
	board.clear();
	board[0][1] = Options<4>{std::bitset<5>{"11001"}};
	board[3][1] = Options<4>{std::bitset<5>{"11001"}};
	EXPECT_EQ(dual_option(board, Location<2>(0, 1)), 4); // same col
	board.clear();
	board[0][0] = Options<4>{std::bitset<5>{"11001"}};
	board[1][1] = Options<4>{std::bitset<5>{"11001"}};
	EXPECT_EQ(dual_option(board, Location<2>(1, 1)), 4); // same block
	board.clear();
	board[1][0] = Options<4>{std::bitset<5>{"11001"}};
	board[1][1] = Options<4>{std::bitset<5>{"11001"}};
	EXPECT_EQ(dual_option(board, Location<2>(1, 1)), 8); // same row & block
	board.clear();
	board[0][0] = Options<4>{std::bitset<5>{"11001"}};
	board[1][0] = Options<4>{std::bitset<5>{"11001"}};
	EXPECT_EQ(dual_option(board, Location<2>(0, 0)), 8); // same col & block

	board.clear();
	board[0][0] = Options<4>{std::bitset<5>{"11001"}};
	EXPECT_EQ(dual_option(board, Location<2>(0, 0)), 0);
	board[1][0] = Options<4>{std::bitset<5>{"10101"}};
	EXPECT_EQ(dual_option(board, Location<2>(0, 0)), 0);
	board[3][3] = Options<4>{std::bitset<5>{"10101"}};
	EXPECT_EQ(dual_option(board, Location<2>(0, 0)), 0);
	EXPECT_EQ(dual_option(board, Location<2>(1, 0)), 0);
	EXPECT_EQ(dual_option(board, Location<2>(3, 3)), 0);
	board[3][0] = Options<4>{std::bitset<5>{"10101"}};
	EXPECT_EQ(dual_option(board, Location<2>(3, 3)), 4); // same row
	board[1][1] = Options<4>{std::bitset<5>{"11001"}};
	EXPECT_EQ(dual_option(board, Location<2>(1, 1)), 50); // same block cascade
	for (const auto& elem : board)
	{
		EXPECT_TRUE(elem.is_answer());
	}

	// clang-format off
	/*	start board				
	*	 _ _ _ _ _ _ _ _ _ _ _ _
	*	|     3 |      	| - 1,2 |	test for row
	*	| 4 5 6 |       |       |
	*	|_7_8_9_|_ _ _ _|_ _ _ _|
	*	|   	|       |   	|
	*	|	    |   all |       |
	*	|_ _ _ _|_ _ _ _|_ _ _ _|
	*	| 		|    	| 1 2 3 |
	*	|	    |	    | 4 5   |test for col
	*	|_ _ _ _|_ _ _ _|_7_8_ _|
	*	before: 0,8: contains 1,2,4,5,6,7,8,9 (all except 3)
	*	after:  0,8: contains only 4,5,7,8
	*	after:	8,0: contains all except 4,7,8
	*	after:  block 4 contains all options in all cells
	*/
	constexpr std::array<char, 81> b1
	{
		0, 0, 3,	0, 0, 0,	0, 0, 0,
		4, 5, 6,	0, 0, 0,	0, 0, 0,
		7, 8, 9,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	1, 2, 3,
		0, 0, 0,	0, 0, 0,	4, 5, 0,
		0, 0, 0,	0, 0, 0,	7, 8, 0
	}; // clang-format on
	Board<Options<9>, 3> B1;
	EXPECT_NO_THROW(set_Value(B1, b1.cbegin(), b1.cend()))
		<< "set_Value failed in copying from the array";
	EXPECT_EQ(B1[0][0].count(), 2U) << "dual_option before 1";
	EXPECT_EQ(B1[0][8].count(), 8U) << "dual_option before 2";
	EXPECT_EQ(B1[4][4].count(), 9U) << "dual_option before 3";
	EXPECT_EQ(B1[8][0].count(), 6U) << "dual_option before 4";
	EXPECT_NO_THROW(dual_option(B1, Location<3>(0))) << "dual_option failed 1";
	EXPECT_EQ(B1[0][0].count(), 2U) << "dual_option 1"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6U) << "dual_option 2";
	EXPECT_EQ(B1[4][4].count(), 9U) << "dual_option 3"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6U) << "dual_option 4"; // unchanged
	EXPECT_NO_THROW(dual_option(B1, Location<3>(1))) << "dual_option failed 3";
	EXPECT_EQ(B1[0][0].count(), 2U) << "dual_option 5"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6U) << "dual_option 6"; // unchanged
	EXPECT_EQ(B1[4][4].count(), 9U) << "dual_option 7"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6U) << "dual_option 8"; // unchanged
	EXPECT_NO_THROW(dual_option(B1, Location<3>(80))) << "dual_option failed3";
	EXPECT_EQ(B1[0][0].count(), 2U) << "dual_option 9"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 4U) << "dual_option 10";
	EXPECT_EQ(B1[4][4].count(), 9U) << "dual_option 11"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6U) << "dual_option 12"; // unchanged
}

TEST(Solver, multi_option)
{
	/*	start board
	 *	 _ _ _ _ _ _ _ _ _ _ _ _
	 *	|       |      	|       | test for row
	 *	| 4 5 6 |       |       |
	 *	|_7_8_9_|_ _ _ _|_ _ _ _|
	 *	|   	|       |   	|
	 *	|	    |   all |       |
	 *	|_ _ _ _|_ _ _ _|_ _ _ _|
	 *	| 		|    	| 1 2   |
	 *	|	    |	    | 4 5   | test for col
	 *	|_ _ _ _|_ _ _ _|_7_8_ _|
	 *	before: 0,8: contains all
	 *	after:  0,8: contains only 4,5,7,8 (not 1,2,3,6,9)
	 *	no change:  block 4 contains all options in all cells
	 */
	// clang-format off
	constexpr std::array<char, 81> b1
	{
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		4, 5, 6,	0, 0, 0,	0, 0, 0,
		7, 8, 9,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	1, 2, 0,
		0, 0, 0,	0, 0, 0,	4, 5, 0,
		0, 0, 0,	0, 0, 0,	7, 8, 0
	}; // clang-format on
	Board<Options<9>, 3> B1;
	EXPECT_NO_THROW(set_Value(B1, b1.cbegin(), b1.cend()))
		<< "set_Value failed in copying from the array";
	EXPECT_EQ(B1[0][0].count(), 3U) << "before 1";
	EXPECT_EQ(B1[0][8].count(), 9U) << "before 2";
	EXPECT_EQ(B1[4][4].count(), 9U) << "before 3";
	EXPECT_EQ(B1[8][8].count(), 3U) << "before 4";
	// run for row
	EXPECT_NO_THROW(multi_option(B1, Location<3>(2)))
		<< "multi_option failed 1";
	EXPECT_EQ(B1[0][0].count(), 3U) << "after 1"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6U) << "after 2";
	EXPECT_EQ(B1[4][4].count(), 9U) << "after 3"; // unchanged
	EXPECT_EQ(B1[0][8].DebugString(), "1111110001");
	// run for col
	EXPECT_NO_THROW(multi_option(B1, Location<3>{6, 8}))
		<< "multi_option failed 2";
	EXPECT_EQ(B1[0][0].count(), 3U) << "after 21"; // unchanged
	EXPECT_EQ(B1[8][8].count(), 3U) << "after 22"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 4U) << "after 23";
	EXPECT_EQ(B1[4][4].count(), 9U) << "after 24"; // unchanged
	EXPECT_EQ(B1[0][8].DebugString(), "0110110001");

	// run for block
	// clang-format off
	constexpr std::array<char, 81> b2
	{ //							 //  _ _ _ _ _ _ _ _ _ _ _ _
		0, 0, 3,  0, 0, 0,  7, 0, 0, // |     3 |       | 7     | diagonal:1,5,9
		4, 0, 6,  2, 7, 0,  0, 0, 0, // | 4   6 | 2 7   |       |
		0, 8, 0,  0, 0, 0,  0, 0, 0, // |_ _8_ _|_ _ _ _|_ _ _ _|
		2, 0, 0,  0, 0, 0,  0, 0, 0, // | 2     |       |       |
		0, 0, 7,  0, 0, 0,  0, 0, 0, // |     7 |       |       |
		0, 0, 0,  0, 0, 0,  0, 0, 0, // |_ _ _ _|_ _ _ _|_ _ _ _|
		0, 0, 2,  0, 0, 0,  0, 0, 0, // |     2 |       |       |
		0, 0, 0,  0, 0, 0,  0, 0, 0, // |       |       |       | test for col
		0, 0, 0,  0, 0, 0,  0, 0, 0	 // |_ _ _ _|_ _ _ _|_ _ _ _|
	}; // clang-format on
	Board<Options<9>, 3> B2;
	EXPECT_NO_THROW(set_Value(B2, b2.cbegin(), b2.cend()))
		<< "set_Value failed in copying from array 4";
	EXPECT_EQ(B2[0][0].count(), 3U) << "before 31";
	EXPECT_EQ(B2[1][1].count(), 3U) << "before 32";
	EXPECT_EQ(B2[2][2].count(), 3U) << "before 33";
	EXPECT_EQ(B2[0][1].count(), 4U) << "before 34";
	EXPECT_EQ(B2[2][0].count(), 4U) << "before 35";
	EXPECT_EQ(B2[0][0].DebugString(), "1000100011") << "before 36";
	EXPECT_EQ(B2[2][0].DebugString(), "1010100011") << "before 37";
	EXPECT_EQ(B2[8][8].count(), 9U) << "before 38";
	EXPECT_EQ(B2[2][5].count(), 6U) << "before 39";
	EXPECT_EQ(B2[2][5].DebugString(), "1001111011") << "before 310";
	EXPECT_NO_THROW(multi_option(B2, Location<3>(0)))
		<< "multi_option failed 3";
	EXPECT_EQ(B2[0][0].count(), 3U) << "after 31";
	EXPECT_EQ(B2[1][1].count(), 3U) << "after 32";
	EXPECT_EQ(B2[2][2].count(), 3U) << "after 33";
	EXPECT_TRUE(is_answer(B2[0][1], Value{2})) << "after 34";
	EXPECT_TRUE(is_answer(B2[2][0], Value{7})) << "after 35";
	EXPECT_EQ(B2[0][0].DebugString(), "1000100011") << "after 36";
	EXPECT_EQ(B2[8][8].count(), 9U) << "after 38";
	EXPECT_EQ(B2[2][5].count(), 6U) << "after 39";
	EXPECT_EQ(B2[2][5].DebugString(), "1001111011") << "after 310";

	// clang-format off
	// 9*9 partials forming a set: 3 cells containing (123,12,13)
	constexpr std::array<char, 81> b3
	{ //							 //  _ _ _ _ _ _ _ _ _ _ _ _
		0, 0, 0,  0, 0, 0,  0, 0, 0, // |       |       |       | row: 1,2; 1,2,3; 1,2,3
		4, 5, 6,  0, 0, 0,  0, 0, 0, // | 4 5 6 |       |       |
		7, 8, 9,  0, 0, 0,  0, 0, 0, // |_7_8_9_|_ _ _ _|_ _ _ _|
		0, 0, 0,  0, 2, 3,  0, 0, 0, // | 3     |   2 3 |       |
		3, 0, 0,  4, 0, 6,  0, 0, 0, // |       | 4   6 |       |
		0, 0, 0,  7, 8, 0,  0, 0, 0, // |_ _ _ _|_7_8_ _|_ _ _ _|
		0, 0, 0,  0, 9, 0,  1, 2, 0, // |       |   9   | 1 2   |		 3,6
		0, 0, 0,  0, 3, 0,  4, 5, 0, // |       |   3 1 | 4 5   |	col: 6,9
		0, 0, 0,  0, 0, 0,  7, 8, 0  // |_ _ _ _|_ _ _5_|_7_8_ _|		 3,6,9
	}; // clang-format on
	Board<Options<9>, 3> B3;
	EXPECT_NO_THROW(set_Value(B3, b3.cbegin(), b3.cend()))
		<< "set_Value failed in copying from array 3";
	EXPECT_EQ(B3[0][0].count(), 2U) << "before 41";
	EXPECT_EQ(B3[0][1].count(), 3U) << "before 42";
	EXPECT_EQ(B3[0][2].count(), 3U) << "before 43";
	EXPECT_EQ(B3[0][3].count(), 7U) << "before 44";
	// EXPECT_EQ(B3[5][1].count(), 6) << "before 45";
	EXPECT_EQ(B3[0][1].DebugString(), "0000001111") << "before 46";
	EXPECT_EQ(B3[6][8].count(), 2U) << "before 47";
	EXPECT_EQ(B3[7][8].count(), 2U) << "before 48";
	EXPECT_EQ(B3[8][8].count(), 3U) << "before 49";
	EXPECT_EQ(B3[8][8].DebugString(), "1001001001") << "before 410";
	EXPECT_NO_THROW(multi_option(B3, Location<3>(1)))
		<< "multi_option failed 4";
	// row:
	EXPECT_EQ(B3[0][0].count(), 2U) << "after 41"; // unchanged
	EXPECT_EQ(B3[0][1].count(), 3U) << "after 42"; // unchanged
	EXPECT_EQ(B3[0][2].count(), 3U) << "after 43"; // unchanged
	EXPECT_EQ(B3[0][3].count(), 4U) << "after 44";
	// EXPECT_EQ(B3[5][1].count(), 6) << "after 45"; // unchanged
	EXPECT_EQ(B3[0][0].DebugString(), "0000000111") << "after 46";
	EXPECT_EQ(B3[0][1].DebugString(), "0000001111") << "after 47";
	EXPECT_EQ(B3[0][3].DebugString(), "1101100001") << "after 48";
}

// NOLINTNEXTLINE(readability-function-size)
TEST(Solver, multi_option_2)
{
	using L = Location<2>;
	using V = Value;
	using B = std::bitset<5>;
	Board<Options<4>, 2> board{}; // reused object for full test
	Board<Options<9>, 3> board_3{};

	// return type
	static_assert(std::is_same_v<int, decltype(multi_option(board, L{}))>);
	static_assert(
		std::is_same_v<int, decltype(multi_option(board, L(), size_t{}))>);

	// error handling
	static_assert(not noexcept(multi_option(board, L())));
	static_assert(not noexcept(multi_option(board, L(), size_t{})));
	// defaults when count is 0 or all:
	// TODO not working in GCC 7.3
	// static_assert(multi_option(board, L(), 0) == 0);
	// static_assert(multi_option(board, L(), elem_size<2>) == 0);
	{ // sanity check:
		board_3[0][0] = Options<9>{std::bitset<10>{"1111000001"}};
		board_3[0][1] = Options<9>{std::bitset<10>{"1111000001"}};
		board_3[0][2] = Options<9>{std::bitset<10>{"1111000001"}};
		board_3[0][3] = Options<9>{std::bitset<10>{"1111000001"}};
		EXPECT_EQ(multi_option(board_3, Location<3>{0}, 4), 20);
	}

	// invalid loc
	board[0][0] = Options<4>{B{"11101"}};
	EXPECT_NO_THROW(multi_option(board, L{0}));
	EXPECT_NO_THROW(multi_option(board, L{0}, 3));
	board[3][3] = Options<4>{B{"11101"}};
	EXPECT_NO_THROW(multi_option(board, L{15}));
	EXPECT_NO_THROW(multi_option(board, L{15}, 3));
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(multi_option(board, L{-1}), "is_valid\\(loc\\)");
	EXPECT_DEBUG_DEATH(multi_option(board, L{16}), "is_valid\\(loc\\)");
	EXPECT_DEBUG_DEATH(multi_option(board, L{-1}, 3), "is_valid\\(loc\\)");
	EXPECT_DEBUG_DEATH(multi_option(board, L{16}, 3), "is_valid\\(loc\\)");
#else
	using ::Sudoku::error::invalid_Location;
	EXPECT_THROW(multi_option(board, L{-1}), invalid_Location);
	EXPECT_THROW(multi_option(board, L{16}), invalid_Location);
	EXPECT_THROW(multi_option(board, L{-1}, 3), invalid_Location);
	EXPECT_THROW(multi_option(board, L{16}, 3), invalid_Location);
#endif // NDEBUG
	// count > elem_size
	EXPECT_DEBUG_DEATH(multi_option(board, L{}, 5), "count < elem_size<N>");
	{ // single_option specialization (count = 1)
		board[0][0] = Options<4>{B{"01001"}};
		EXPECT_NO_THROW(multi_option(board, L{0}));
		EXPECT_NO_THROW(multi_option(board, L{0}, 1));
		board[3][3] = Options<4>{B{"01001"}};
		EXPECT_NO_THROW(multi_option(board, L{15}, 1));
		EXPECT_NO_THROW(multi_option(board, L{15}));
#ifndef NDEBUG
		EXPECT_DEBUG_DEATH(multi_option(board, L{-1}, 1), "is_valid\\(loc\\)");
		EXPECT_DEBUG_DEATH(multi_option(board, L{16}, 1), "is_valid\\(loc\\)");
#else
		// thrown by Board::at(Location)
		EXPECT_THROW(multi_option(board, L{-1}, 1), invalid_Location);
		EXPECT_THROW(multi_option(board, L{16}, 1), invalid_Location);
#endif // NDEBUG
		EXPECT_NO_THROW(multi_option(board_3, Location<3>{16}));
	}
	{ // dual_option specialization
		board[0][0] = Options<4>{B{"01011"}};
		EXPECT_NO_THROW(multi_option(board, L{0}));
		EXPECT_NO_THROW(multi_option(board, L{0}, 2));
		board[3][3] = Options<4>{B{"01011"}};
		EXPECT_NO_THROW(multi_option(board, L{15}, 2));
		EXPECT_NO_THROW(multi_option(board, L{15}));
#ifndef NDEBUG
		EXPECT_DEBUG_DEATH(multi_option(board, L{-1}, 2), "is_valid\\(loc\\)");
		EXPECT_DEBUG_DEATH(multi_option(board, L{16}, 2), "is_valid\\(loc\\)");
#else
		EXPECT_THROW(multi_option(board, L{-1}, 2), invalid_Location);
		EXPECT_THROW(multi_option(board, L{16}, 2), invalid_Location);
#endif // NDEBUG
	}
	// count too small
	board[0][0] = Options<4>{B{"11111"}};
	EXPECT_DEBUG_DEATH(
		multi_option(board, L{0}, 3), "item.count\\(\\) == count");
	// count too large
	board[0][0] = Options<4>{B{"11001"}};
	EXPECT_DEBUG_DEATH(
		multi_option(board, L{0}, 3), "item.count\\(\\) == count");

	//====----------------------------------------------------------------====//
	// Operational testing

	// empty board to limit influence and cascades
	const auto empty_base = [&board]() {
		for (auto& v : board)
		{ // start with all set to "00000"
			v.clear();
		}
	};
	empty_base();
	ASSERT_EQ(board[L{10}].count_all(), 0u);

	//====----------------------------------------------------------------====//
	{ // influence on an isolated row
		const auto reset_row = [&board] {
			board.at(L{0}) = B{"01111"};
			board.at(L{1}) = B{"11111"};
			board.at(L{2}) = B{"00111"};
			board.at(L{3}) = B{"00101"};
		};
		empty_base();
		reset_row();
		EXPECT_EQ(multi_option(board, L{1}), 0);    // all set
		EXPECT_EQ(multi_option(board, L{0}, 4), 0); // as if all set
		EXPECT_EQ(multi_option(board, L{0}), 4);    // 0,1 -> ans 4
		EXPECT_EQ(board[L{0}].count(), 3u);
		EXPECT_TRUE(is_answer(board.at(L{1})));
		EXPECT_TRUE(is_answer(board.at(L{1}), V{4}));
		EXPECT_EQ(board[L{2}].count(), 2u);
		EXPECT_EQ(board[L{3}].count(), 1u);
		EXPECT_EQ(multi_option(board, L{0}), 0); // repeated
		EXPECT_EQ(multi_option(board, L{1}), 0);
		// specialization: dual option
		reset_row();
		EXPECT_EQ(multi_option(board, L{2}), 0); // 7 without specialization
		board.at(L{3}) = B{"00111"};
		EXPECT_EQ(multi_option(board, L{2}), 7);
		EXPECT_TRUE(is_answer(board.at(L{0}), V{3}));
		EXPECT_TRUE(is_answer(board.at(L{1}), V{4}));
		EXPECT_EQ(board[L{2}].count(), 2u);
		EXPECT_EQ(board[L{3}].count(), 2u);
		// specialization: single option
		reset_row();
		EXPECT_EQ(multi_option(board, L{3}), 10);
		EXPECT_TRUE(is_answer(board.at(L{2}), V{1}));
		EXPECT_EQ(board[L{3}].count(), 0u);

		// combined
		reset_row();
		EXPECT_FALSE(is_answer(board.at(L{1})));
		EXPECT_EQ(multi_option(board, L{0}), 4);
		EXPECT_TRUE(is_answer(board.at(L{1}), V{4}));
		EXPECT_EQ(multi_option(board, L{1}), 0);
		EXPECT_EQ(multi_option(board, L{2}), 0);
		EXPECT_FALSE(is_answer(board.at(L{0})));
		EXPECT_FALSE(is_answer(board.at(L{2})));
		EXPECT_FALSE(is_answer(board.at(L{3})));
		EXPECT_EQ(multi_option(board, L{3}), 6);
		EXPECT_TRUE(is_answer(board.at(L{0}), V{3}));
		EXPECT_TRUE(is_answer(board.at(L{2}), V{1}));
		EXPECT_TRUE(is_answer(board.at(L{3}), V{2}));

		// different order
		reset_row();
		// Setting (0,2) as answer triggers a cascade, removing Value 1 from all
		EXPECT_EQ(multi_option(board, L{3}), 10);
		EXPECT_TRUE(is_answer(board.at(L{0}), V{3}));
		EXPECT_TRUE(is_answer(board.at(L{1}), V{4}));
		EXPECT_TRUE(is_answer(board.at(L{2}), V{1}));
		EXPECT_TRUE(is_answer(board.at(L{3}), V{2}));
		EXPECT_EQ(multi_option(board, L{2}), 0);
		EXPECT_EQ(multi_option(board, L{1}), 0);
		EXPECT_EQ(multi_option(board, L{0}), 0);
	}
	//====----------------------------------------------------------------====//
	{ // influence on an isolated col
		const auto reset_col = [&board] {
			board.at(L{2, 0}) = B{"01111"};
			board.at(L{2, 1}) = B{"11111"};
			board.at(L{2, 2}) = B{"00111"};
			board.at(L{2, 3}) = B{"00101"};
		};
		empty_base();
		reset_col();
		EXPECT_EQ(multi_option(board, L{2, 1}), 0);    // all set
		EXPECT_EQ(multi_option(board, L{2, 0}, 4), 0); // as if all set
		EXPECT_EQ(multi_option(board, L{2, 0}), 4);    // 0,1 -> ans 4
		EXPECT_EQ(board[L(2, 0)].count(), 3u);
		EXPECT_TRUE(is_answer(board.at(L{2, 1})));
		EXPECT_TRUE(is_answer(board.at(L{2, 1}), V{4}));
		EXPECT_EQ(board[L(2, 2)].count(), 2u);
		EXPECT_EQ(board[L(2, 3)].count(), 1u);
		EXPECT_EQ(multi_option(board, L{2, 0}), 0); // repeated
		EXPECT_EQ(multi_option(board, L{2, 1}), 0);
		// specialization: dual option
		reset_col();
		EXPECT_EQ(multi_option(board, L{2, 2}), 0); // 7 without specialization
		board.at(L{2, 3}) = B{"00111"};
		EXPECT_EQ(multi_option(board, L{2, 2}), 7);
		EXPECT_TRUE(is_answer(board.at(L{2, 0}), V{3}));
		EXPECT_TRUE(is_answer(board.at(L{2, 1}), V{4}));
		EXPECT_EQ(board[L(2, 2)].count(), 2u);
		EXPECT_EQ(board[L(2, 3)].count(), 2u);
		// specialization: single option
		reset_col();
		EXPECT_EQ(multi_option(board, L{2, 3}), 10);
		EXPECT_TRUE(is_answer(board.at(L{2, 2}), V{1}));
		EXPECT_EQ(board[L(2, 3)].count(), 0u);
	}
	//====----------------------------------------------------------------====//
	{ // influence on an isolated block
		using LB = ::Sudoku::Location_Block<2>;

		const auto reset_block = [&board] {
			board.at(LB{2, 0}) = B{"01111"};
			board.at(LB{2, 1}) = B{"11111"};
			board.at(LB{2, 2}) = B{"00111"};
			board.at(LB{2, 3}) = B{"00101"};
		};
		empty_base();
		reset_block();
		EXPECT_EQ(multi_option(board, L{LB{2, 1}}), 0);    // all set
		EXPECT_EQ(multi_option(board, L{LB{2, 0}}, 4), 0); // as if all set
		EXPECT_EQ(multi_option(board, L{LB{2, 0}}), 4);    // 0,1 -> ans 4
		EXPECT_EQ(board[LB(2, 0)].count(), 3u);
		EXPECT_TRUE(is_answer(board.at(LB{2, 1})));
		EXPECT_TRUE(is_answer(board.at(LB{2, 1}), V{4}));
		EXPECT_EQ(board[LB(2, 2)].count(), 2u);
		EXPECT_EQ(board[LB(2, 3)].count(), 1u);
		EXPECT_EQ(multi_option(board, L{LB{2, 0}}), 0); // repeated
		EXPECT_EQ(multi_option(board, L{LB{2, 1}}), 0);
		// specialization: dual option
		reset_block();
		EXPECT_EQ(multi_option(board, L{LB{2, 2}}), 0);
		board.at(LB{2, 3}) = B{"00111"};
		EXPECT_EQ(multi_option(board, L{LB{2, 2}}), 7);
		EXPECT_TRUE(is_answer(board.at(LB{2, 0}), V{3}));
		EXPECT_TRUE(is_answer(board.at(LB{2, 1}), V{4}));
		EXPECT_EQ(board[LB(2, 2)].count(), 2u);
		EXPECT_EQ(board[LB(2, 3)].count(), 2u);
		// specialization: single option
		reset_block();
		EXPECT_EQ(multi_option(board, L{LB{2, 3}}), 10);
		EXPECT_TRUE(is_answer(board.at(LB{2, 2}), V{1}));
		EXPECT_EQ(board[LB(2, 3)].count(), 0u);
	}
	//====----------------------------------------------------------------====//
	{ // combined removes
		[&board] {
			board             = Board<Options<4>, 2>();
			board.at(L{0, 0}) = B{"01111"};
			board.at(L{0, 1}) = B{"11111"};
			board.at(L{0, 2}) = B{"00111"};
			board.at(L{0, 3}) = B{"00101"};
			board.at(L{3, 0}) = B{"10000"};
		}();
		EXPECT_EQ(multi_option(board, L{0, 1}), 0); // all set
		EXPECT_EQ(multi_option(board, L{0, 0}), 8);
		EXPECT_TRUE(is_answer(board[0][1], Value{4}));
		EXPECT_EQ(multi_option(board, L{0, 3}), 17);
		EXPECT_EQ(multi_option(board, L{3, 0}), 0);
		board.at(L{3, 0}) = B{"10001"};
		EXPECT_FALSE(is_answer(board[3][0]));
		EXPECT_EQ(multi_option(board, L{3, 0}), 4);
		EXPECT_TRUE(is_answer(board[3][0]));
	}
}

TEST(Solver, solve_board)
{
	/*	start board					answer board
	 *	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
	 *	|		| 		|   1 2 |	| 6 7 3	| 8 9 4	| 5 1 2 |
	 *	|		|	3 5 |		|	| 9 1 2	| 7 3 5 | 4 8 6	|
	 *	| _ _ _ | 6 _ _ | _ 7 _ |	|_8_4_5_|_6_1_2_|_9_7_3_|
	 *	| 7		|		| 3		|	| 7 9 8	| 2 6 1	| 3 5 4 |
	 *	|		| 4		| 8		|	| 5 2 6	| 4 7 3	| 8 9 1	|
	 *	| 1 _ _ | _ _ _	| _ _ _ |	|_1_3_4_|_5_8_9_|_2_6_7_|
	 *	| 		| 1 2	|		|	| 4 6 9	| 1 2 8 | 7 3 5	|
	 *	|	8	|		|	4	|	| 2 8 7	| 3 5 6	| 1 4 9	|
	 *	|_ _5_ _|_ _ _ _|_6_ _ _|	|_3_5_1_|_9_4_7_|_6_2_8_|
	 */
	// clang-format off
	constexpr std::array<char, 81> b1
	{
		0, 0, 0,	0, 0, 0,	0, 1, 2,
		0, 0, 0,	0, 3, 5,	0, 0, 0,
		0, 0, 0,	6, 0, 0,	0, 7, 0,
		7, 0, 0,	0, 0, 0,	3, 0, 0,
		0, 0, 0,	4, 0, 0,	8, 0, 0,
		1, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	1, 2, 0,	0, 0, 0,
		0, 8, 0,	0, 0, 0,	0, 4, 0,
		0, 5, 0,	0, 0, 0,	6, 0, 0
	};
	constexpr std::array<char, 81> b1a
	{
		6, 7, 3,	8, 9, 4,	5, 1, 2,
		9, 1, 2,	7, 3, 5,	4, 8, 6,
		8, 4, 5,	6, 1, 2,	9, 7, 3,
		7, 9, 8,	2, 6, 1,	3, 5, 4,
		5, 2, 6,	4, 7, 3,	8, 9, 1,
		1, 3, 4,	5, 8, 9,	2, 6, 7,
		4, 6, 9,	1, 2, 8,	7, 3, 5,
		2, 8, 7,	3, 5, 6,	1, 4, 9,
		3, 5, 1,	9, 4, 7,	6, 2, 8
	};
	// clang-format on
	Board<int, 3> start;
	std::copy(b1.cbegin(), b1.cend(), start.begin());
	Board<int, 3> answer;
	std::copy(b1a.cbegin(), b1a.cend(), answer.begin());
	Board<Options<9>, 3> options{};
	set_Value(options, b1.cbegin(), b1.cend());
	for (int i = 0; i < ::Sudoku::elem_size<3>; ++i)
	{
		unique_in_section(options, options.row(i));
	}
}

TEST(Solver, deathtest)
{
	Board<Options<4>, 2> B{};

	// single_option()
#ifndef NDEBUG // note: release triggers normal exception
	// when wrong value
	B[1][2] = std::bitset<5>{"00011"}; // 1, not answer
	EXPECT_DEBUG_DEATH(
		single_option(B, Location<2>(1, 2), Value{4}), "Assertion .*test.*");
#endif // NDEBUG
}

} // namespace SudokuTests::SolversTest
