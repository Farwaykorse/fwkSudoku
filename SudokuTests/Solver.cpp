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
// Debug Output
#include "print_Options.h"
// library
#include <bitset>
#include <vector>
#include <type_traits>


using namespace Sudoku;

namespace SudokuTests::SolversTest
{
TEST(Solver, unique_in_section)
{
	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,2, 0,0,	// 1	2	3	4
		4,0, 0,0,	// 4	3	1,2	1,2
		0,1, 4,0,	// 2,3	1	4	2,3
		0,0, 0,0	// 2,3	4	1,2	1,2,3
	}; // clang-format on
	Board<Options<4>, 2> B1;
	ASSERT_EQ(B1[0][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(setValue(B1, v1.cbegin(), v1.cend()));
	const Board<Options<4>, 2> cB1{B1}; // copy to compare with
	EXPECT_EQ(B1[0][1], Value{2}) << "setup error";
	EXPECT_EQ(B1[1][0], Value{4}) << "setup error";
	EXPECT_EQ(B1[2][2], Value{4}) << "setup error";
	EXPECT_EQ(B1, cB1) << "copy error";
	// single row
	EXPECT_NO_THROW(unique_in_section(B1, B1.row(0)));
	EXPECT_EQ(B1, cB1) << "row 0 was completely fixed by setValue";

	// clang-format off
	const std::vector<int> v2
	{
		//start		// after setValue
		3,2, 0,0,	// 3	2	14	14		3	2	14	14
		0,0, 0,0,	// 14	14	3	2		14	14	3	2
		0,0, 2,0,	// 14	134	2	134		14	134	2	134
		0,0, 0,0	// 12.4	134	134	134		2	134	14	134
	}; // clang-format on

	Board<Options<4>, 2> B2{}; // working copy
	EXPECT_NO_THROW(setValue(B2, v2.cbegin(), v2.cend()));
	const Board<Options<4>, 2> cB2{B2}; // copy to compare with
	// single row 0
	EXPECT_NO_THROW(unique_in_section(B2, B2.row(0)));
	EXPECT_EQ(B2, cB2) << "row 0 was completely fixed by setValue";
	// single row 1
	EXPECT_NO_THROW(unique_in_section(B2, B2.row(3)));
	EXPECT_NE(B2, cB2) << "row 3 should have changed";
	// full board
	for (int i = 0; i < elem_size<2>; ++i)
	{
		EXPECT_NO_THROW(unique_in_section(B2, B2.row(i)));
	}
	EXPECT_EQ(B2[0][0], Value{3}) << "input values are lost";
	EXPECT_EQ(B2[0][1], Value{2});
	EXPECT_EQ(B2[2][2], Value{2});
	EXPECT_EQ(B2[1][3], Value{2}) << "not all uniques found in rows";
	EXPECT_EQ(B2[3][0], Value{2});

	// clang-format off
	const std::vector<int> v3
	{
		// start	// after setValue
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
		EXPECT_NO_THROW(setValue(B3, v3.cbegin(), v3.cend()));
	};
	reset_B3();
	EXPECT_EQ(B3[3][3].count(), 4);
	EXPECT_TRUE(B3[3][3].is_option(Value{1}));
	// Row()
	EXPECT_NO_THROW(unique_in_section(B3, B3.row(3)));
	EXPECT_TRUE(B3[3][3].is_answer(Value{1}));
	// Col()
	reset_B3();
	EXPECT_NO_THROW(unique_in_section(B3, B3.col(3)));
	EXPECT_TRUE(B3[3][3].is_answer(Value{1}));
	// BLock()
	reset_B3();
	EXPECT_NO_THROW(unique_in_section(B3, B3.block(3)));
	EXPECT_TRUE(B3[3][3].is_answer(Value{1}));
}

TEST(Solver, section_exclusive)
{
	// section_exclusive(SectionT)
	// section_exclusive(Block)

	// same result as unique_in_section
	{
		// clang-format off
		const std::vector<int> v3
		{
			// start	// after setValue
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
			EXPECT_NO_THROW(setValue(B3, v3.cbegin(), v3.cend()));
		};
		reset_B3();
		EXPECT_EQ(B3[3][3].count(), 4);
		EXPECT_TRUE(B3[3][3].is_option(Value{1}));
		// Row()
		EXPECT_NO_THROW(section_exclusive(B3, B3.row(3)));
		EXPECT_TRUE(B3[3][3].is_answer(Value{1}));
		// Col()
		reset_B3();
		EXPECT_NO_THROW(section_exclusive(B3, B3.col(3)));
		EXPECT_TRUE(B3[3][3].is_answer(Value{1}));
		// BLock()
		reset_B3();
		EXPECT_NO_THROW(section_exclusive(B3, B3.block(3)));
		EXPECT_TRUE(B3[3][3].is_answer(Value{1}));
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
		EXPECT_EQ(B[0][0].count(), 2);
		EXPECT_TRUE(B[0][1].all());
		EXPECT_EQ(B[0][2].count(), 2);
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
		EXPECT_EQ(B[2][2].count(), 4); // self
		EXPECT_EQ(B[2][3].count(), 4); // self
		EXPECT_EQ(B[3][2].count(), 2); // rest block
		EXPECT_EQ(B[1][2].count(), 4);

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
		EXPECT_EQ(section_exclusive(B, B.block(0)), 4);
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
		EXPECT_EQ(section_exclusive(B3, B3.block(0)), 3 * 6);
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
		EXPECT_EQ(section_exclusive(B3, B3.block(0)), 3 * 6);
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
		EXPECT_EQ(section_exclusive(B3, B3.block(0)), 0);
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

	{
		// clang-format off
		const std::vector<int> b1
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
		const std::vector<int> b1a	// requires unique
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
		setValue(B1, b1.cbegin(), b1.cend());
		Board<Options<9>, 3> A1{};
		setValue(A1, b1a.cbegin(), b1a.cend());
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.row(0)), 6);
		EXPECT_EQ(section_exclusive(B1, B1.row(1)), 2);
		EXPECT_EQ(section_exclusive(B1, B1.row(2)), 4);
		EXPECT_EQ(section_exclusive(B1, B1.row(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(6)), 2);
		EXPECT_EQ(section_exclusive(B1, B1.row(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.col(0)), 3);
		EXPECT_EQ(section_exclusive(B1, B1.col(1)), 2);
		EXPECT_EQ(section_exclusive(B1, B1.col(2)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(6)), 65);
		EXPECT_EQ(section_exclusive(B1, B1.col(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.block(0)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(1)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(2)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(6)), 24);
		EXPECT_EQ(section_exclusive(B1, B1.block(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.block(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.row(0)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(1)), 23);
		EXPECT_EQ(section_exclusive(B1, B1.row(2)), 7);
		EXPECT_EQ(section_exclusive(B1, B1.row(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(4)), 2);
		EXPECT_EQ(section_exclusive(B1, B1.row(5)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(6)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.row(8)), 0);
		EXPECT_NE(A1, B1);

		EXPECT_EQ(section_exclusive(B1, B1.col(0)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(1)), 23);
		EXPECT_EQ(section_exclusive(B1, B1.col(2)), 1);
		EXPECT_EQ(section_exclusive(B1, B1.col(3)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(4)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(5)), 82);
		EXPECT_EQ(section_exclusive(B1, B1.col(6)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(7)), 0);
		EXPECT_EQ(section_exclusive(B1, B1.col(8)), 0);
		EXPECT_EQ(A1, B1);
	}
	{
		// clang-format off
		const std::vector<int> b5
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
		const std::vector<int> b5a	// requires double_option, not unique
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
		setValue(B5, b5.cbegin(), b5.cend());
		Board<Options<9>, 3> A5{};
		setValue(A5, b5a.cbegin(), b5a.cend());
		EXPECT_NE(A5, B5);

		EXPECT_EQ(section_exclusive(B5, B5.row(0)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(1)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(2)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(3)), 3);
		EXPECT_EQ(section_exclusive(B5, B5.row(4)), 4);
		EXPECT_EQ(section_exclusive(B5, B5.row(5)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(6)), 4);
		EXPECT_EQ(section_exclusive(B5, B5.row(7)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.row(8)), 1);
		EXPECT_NE(A5, B5);

		EXPECT_EQ(section_exclusive(B5, B5.col(0)), 14);
		EXPECT_EQ(section_exclusive(B5, B5.col(1)), 2);
		EXPECT_EQ(section_exclusive(B5, B5.col(2)), 3);
		EXPECT_EQ(section_exclusive(B5, B5.col(3)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(4)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(5)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(6)), 9);
		EXPECT_EQ(section_exclusive(B5, B5.col(7)), 0);
		EXPECT_EQ(section_exclusive(B5, B5.col(8)), 0);
		EXPECT_NE(A5, B5);

		EXPECT_EQ(section_exclusive(B5, B5.block(0)), 14);
		EXPECT_EQ(section_exclusive(B5, B5.block(1)), 89);
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
		setValue(B6, b5.cbegin(), b5.cend());
		ASSERT_NE(A6, B6);

		EXPECT_EQ(section_exclusive(B6, B6.row(0)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.col(0)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.block(0)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.row(1)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.col(1)), 5);
		EXPECT_EQ(section_exclusive(B6, B6.block(1)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.row(2)), 0);
		EXPECT_EQ(section_exclusive(B6, B6.col(2)), 4);
		EXPECT_NO_FATAL_FAILURE(section_exclusive(B6, B6.block(2)));
	}

	// reproduce functionality of unique
	//===------------------------------------------------------------------===//
	// clang-format off
	static const std::vector<int> V1
	{
		// start	// after setValue	// unique_block
		0,0, 1,0,	// 
		1,0, 0,0,	// 
		0,1, 0,0,	// 
		0,0, 0,0	//					//	0	0	0	1
	}; // clang-format on
	Board<Options<4>, 2> B1{};
	setValue(B1, V1.cbegin(), V1.cend());
	// check input:
	{
		EXPECT_FALSE(B1[0][0].is_answer());
		EXPECT_FALSE(B1[0][1].is_answer());
		EXPECT_EQ(B1[0][2], Value{1});
		EXPECT_FALSE(B1[0][3].is_answer());
		EXPECT_EQ(B1[1][0], Value{1});
		EXPECT_FALSE(B1[1][1].is_answer());
		EXPECT_FALSE(B1[1][2].is_answer());
		EXPECT_FALSE(B1[1][3].is_answer());
		EXPECT_FALSE(B1[2][0].is_answer());
		EXPECT_EQ(B1[2][1], Value{1});
		EXPECT_FALSE(B1[2][2].is_answer());
		EXPECT_FALSE(B1[2][3].is_answer());
		EXPECT_FALSE(B1[3][0].is_answer());
		EXPECT_FALSE(B1[3][1].is_answer());
		EXPECT_FALSE(B1[3][2].is_answer());
		EXPECT_FALSE(B1[3][3].is_answer());
	}
	// block_exclusive:
	EXPECT_EQ(1, section_exclusive(B1, B1.block(3)))
		<< "section_exclusive(Block) should find 1 value";
	EXPECT_EQ(B1[3][3], Value{1})
		<< "section_exclusive(Block) unique value failed";
	int found1{0};
	for (int i{0}; i < elem_size<2>; ++i)
	{
		found1 += section_exclusive(B1, B1.block(i));
	}
	EXPECT_EQ(found1, 0) << "shouldn't find any others";

	// working for more than 1 unique (use set_section_locals)
	//===------------------------------------------------------------------===//
	// clang-format off
	const std::vector<int> V2
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
	setValue(B2, V2.cbegin(), V2.cend());
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
	for (int i = 0; i < elem_size<3>; ++i)
	{
		EXPECT_NO_THROW(section_exclusive(B2, B2.block(i)));
	}
}

TEST(Solver, single_option)
{
	using L = Location<2>;
	Board<Options<4>, 2> B1;
	ASSERT_EQ(B1[1][0], Options<4>{}) << "incorrect instantiation";

	// is single option (loc, value)
	B1[0][1] = std::bitset<5>{"00101"}; // 2
	ASSERT_FALSE(B1[0][1].is_answer(Value{2}));
	ASSERT_TRUE(B1[0][1].is_option(Value{2}));
	EXPECT_EQ(B1[0][0].count(), 4);
	EXPECT_TRUE(B1[0][2].count() == 4 && B1[0][3].count() == 4);
	EXPECT_EQ(B1[1][1].count(), 4);
	EXPECT_TRUE(B1[2][1].count() == 4 && B1[3][1].count() == 4);
	EXPECT_NO_THROW(single_option(B1, L(1), Value{2}));
	EXPECT_EQ(B1[0][0].count(), 3);
	EXPECT_FALSE(B1[0][0].is_option(Value{2}));
	EXPECT_FALSE(B1[0][2].is_option(Value{2}) || B1[0][3].is_option(Value{2}));
	// is answer (loc, value)
	B1[1][0] = Value{4};
	ASSERT_TRUE(B1[1][0].is_answer(Value{4}));
	EXPECT_EQ(B1[1][2].count(), 4);
	EXPECT_EQ(B1[2][0].count(), 4);
	EXPECT_NO_THROW(single_option(B1, L(1, 0), Value{4}));
	EXPECT_EQ(B1[0][0].count(), 2);
	EXPECT_EQ(B1[1][2].count(), 3);
	// cascase
	B1[2][1] = Value{1};
	ASSERT_TRUE(B1[2][1].is_answer(Value{1}));
	EXPECT_EQ(B1[0][0].count(), 2);
	EXPECT_EQ(B1[1][1].count(), 2);
	EXPECT_NO_THROW(single_option(B1, L(2, 1), Value{1}));
	EXPECT_TRUE(B1[1][1].is_answer(Value{3}));
	EXPECT_TRUE(B1[0][0].is_answer(Value{1}));
	EXPECT_TRUE(B1[3][1].is_answer(Value{4}));

	// single_option(Location)
	Board<Options<4>, 2> B2;
	ASSERT_EQ(B2[1][0], Options<4>{}) << "incorrect instantiation";
	// more than 1 option available
	ASSERT_NO_THROW(single_option(B2, L(1)));
	EXPECT_EQ(single_option(B2, L(1)), 0);
	B2[0][1] = std::bitset<5>{"00101"}; // 2
	ASSERT_FALSE(B2[0][1].is_answer(Value{2}));
	ASSERT_TRUE(B2[0][1].is_option(Value{2}));
	EXPECT_EQ(B2[0][0].count(), 4);
	EXPECT_TRUE(B2[0][2].count() == 4 && B2[0][3].count() == 4);
	EXPECT_EQ(B2[1][1].count(), 4);
	EXPECT_TRUE(B2[2][1].count() == 4 && B2[3][1].count() == 4);
	EXPECT_EQ(single_option(B2, L(1)), 8);
	EXPECT_EQ(B2[0][0].count(), 3);
	EXPECT_FALSE(B2[0][0].is_option(Value{2}));
	EXPECT_FALSE(B2[0][2].is_option(Value{2}) || B2[0][3].is_option(Value{2}));
	// is answer (loc, value)
	B2[1][0] = Value{4};
	ASSERT_TRUE(B2[1][0].is_answer(Value{4}));
	EXPECT_EQ(B2[1][2].count(), 4);
	EXPECT_EQ(B2[2][0].count(), 4);
	EXPECT_EQ(single_option(B2, L(1, 0)), 6);
	EXPECT_EQ(B2[0][0].count(), 2);
	EXPECT_EQ(B2[1][2].count(), 3);
	// cascase
	B2[2][1] = Value{1};
	ASSERT_TRUE(B2[2][1].is_answer(Value{1}));
	EXPECT_EQ(B2[0][0].count(), 2);
	EXPECT_EQ(B2[1][1].count(), 2);
	EXPECT_EQ(single_option(B2, L(2, 1)), 17);
	EXPECT_TRUE(B2[1][1].is_answer(Value{3}));
	EXPECT_TRUE(B2[0][0].is_answer(Value{1}));
	EXPECT_TRUE(B2[3][1].is_answer(Value{4}));

	// set_value, with single_option should give the same result
	Board<Options<4>, 2> B3;
	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,2, 0,0,	// 1	2	34	34
		4,0, 0,0,	// 4	3	12	12
		0,1, 0,0,	// 23	1	234	234
		0,0, 0,0	// 23	4	123	123
	}; // clang-format on
	ASSERT_EQ(B3[1][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(setValue(B3, v1.cbegin(), v1.cend()));
	EXPECT_EQ(B1, B3);
}
TEST(Solver, dual_option)
{
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
	const std::vector<int> b1
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
	EXPECT_NO_THROW(setValue(B1, b1.cbegin(), b1.cend()))
		<< "setValue failed in copying from vector";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option before 1";
	EXPECT_EQ(B1[0][8].count(), 8) << "dual_option before 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option before 3";
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option before 4";
	EXPECT_NO_THROW(dual_option(B1, Location<3>(0))) << "dual_option failed 1";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option 1"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6) << "dual_option 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option 3"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option 4"; // unchanged
	EXPECT_NO_THROW(dual_option(B1, Location<3>(1))) << "dual_option failed 3";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option 5"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6) << "dual_option 6"; // unchanged
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option 7"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option 8"; // unchanged
	EXPECT_NO_THROW(dual_option(B1, Location<3>(80))) << "dual_option failed3";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option 9"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 4) << "dual_option 10";
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option 11"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option 12"; // unchanged
}
TEST(Solver, multi_option)
{
	// clang-format off
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
	using list = std::vector<Value>;
	using V = Value;
	const std::vector<int> b1
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
	EXPECT_NO_THROW(setValue(B1, b1.cbegin(), b1.cend()))
		<< "setValue failed in copying from vector";
	EXPECT_EQ(B1[0][0].count(), 3) << "before 1";
	EXPECT_EQ(B1[0][8].count(), 9) << "before 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "before 3";
	EXPECT_EQ(B1[8][8].count(), 3) << "before 4";
	// run for row
	EXPECT_NO_THROW(multi_option(B1, Location<3>(2)))
		<< "multi_option failed 1";
	EXPECT_EQ(B1[0][0].count(), 3) << "after 1"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6) << "after 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "after 3"; // unchanged
	EXPECT_EQ(B1[0][8].available(), (list{V{4}, V{5}, V{6}, V{7}, V{8}, V{9}}))
		<< "after 4";
	// run for col
	EXPECT_NO_THROW(multi_option(B1, Location<3>{6, 8}))
		<< "multi_option failed 2";
	EXPECT_EQ(B1[0][0].count(), 3) << "after 21"; // unchanged
	EXPECT_EQ(B1[8][8].count(), 3) << "after 22"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 4) << "after 23";
	EXPECT_EQ(B1[4][4].count(), 9) << "after 24"; // unchanged
	EXPECT_EQ(B1[0][8].available(), (list{V{4}, V{5}, V{7}, V{8}}))
		<< "after 25";

	// run for block
	// clang-format off
	const std::vector<int> b2
	{								 //  _ _ _ _ _ _ _ _ _ _ _ _
		0, 0, 3,  0, 0, 0,  7, 0, 0, // |     3 |       | 7     | diagonal: 1,5,9
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
	EXPECT_NO_THROW(setValue(B2, b2.cbegin(), b2.cend()))
		<< "setValue failed in copying from vector 4";
	EXPECT_EQ(B2[0][0].count(), 3) << "before 31";
	EXPECT_EQ(B2[1][1].count(), 3) << "before 32";
	EXPECT_EQ(B2[2][2].count(), 3) << "before 33";
	EXPECT_EQ(B2[0][1].count(), 4) << "before 34";
	EXPECT_EQ(B2[2][0].count(), 4) << "before 35";
	EXPECT_EQ(B2[0][0].available(), (list{V{1}, V{5}, V{9}})) << "before 36";
	EXPECT_EQ(B2[2][0].available(), (list{V{1}, V{5}, V{7}, V{9}}))
		<< "before 37";
	EXPECT_EQ(B2[8][8].count(), 9) << "before 38";
	EXPECT_EQ(B2[2][5].count(), 6) << "before 39";
	EXPECT_EQ(B2[2][5].available(), (list{V{1}, V{3}, V{4}, V{5}, V{6}, V{9}}))
		<< "before 310";
	EXPECT_NO_THROW(multi_option(B2, Location<3>(0)))
		<< "multi_option failed 3";
	EXPECT_EQ(B2[0][0].count(), 3) << "after 31";
	EXPECT_EQ(B2[1][1].count(), 3) << "after 32";
	EXPECT_EQ(B2[2][2].count(), 3) << "after 33";
	EXPECT_TRUE(B2[0][1].is_answer(Value{2})) << "after 34";
	EXPECT_TRUE(B2[2][0].is_answer(Value{7})) << "after 35";
	EXPECT_EQ(B2[0][0].available(), (list{V{1}, V{5}, V{9}})) << "after 36";
	EXPECT_EQ(B2[8][8].count(), 9) << "after 38";
	EXPECT_EQ(B2[2][5].count(), 6) << "after 39";
	EXPECT_EQ(B2[2][5].available(), (list{V{1}, V{3}, V{4}, V{5}, V{6}, V{9}}))
		<< "after 310";

	// clang-format off
	// 9*9 partials forming a set: 3 cels containing (123,12,13)
	const std::vector<int> b3
	{								 //  _ _ _ _ _ _ _ _ _ _ _ _
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
	EXPECT_NO_THROW(setValue(B3, b3.cbegin(), b3.cend()))
		<< "setValue failed in copying from vector 3";
	EXPECT_EQ(B3[0][0].count(), 2) << "before 41";
	EXPECT_EQ(B3[0][1].count(), 3) << "before 42";
	EXPECT_EQ(B3[0][2].count(), 3) << "before 43";
	EXPECT_EQ(B3[0][3].count(), 7) << "before 44";
	// EXPECT_EQ(B3[5][1].count(), 6) << "before 45";
	EXPECT_EQ(B3[0][1].available(), (list{V{1}, V{2}, V{3}})) << "before 46";
	EXPECT_EQ(B3[6][8].count(), 2) << "before 47";
	EXPECT_EQ(B3[7][8].count(), 2) << "before 48";
	EXPECT_EQ(B3[8][8].count(), 3) << "before 49";
	EXPECT_EQ(B3[8][8].available(), (list{V{3}, V{6}, V{9}})) << "before 410";
	EXPECT_NO_THROW(multi_option(B3, Location<3>(1)))
		<< "multi_option failed 4";
	// row:
	EXPECT_EQ(B3[0][0].count(), 2) << "after 41"; // unchanged
	EXPECT_EQ(B3[0][1].count(), 3) << "after 42"; // unchanged
	EXPECT_EQ(B3[0][2].count(), 3) << "after 43"; // unchanged
	EXPECT_EQ(B3[0][3].count(), 4) << "after 44";
	// EXPECT_EQ(B3[5][1].count(), 6) << "after 45"; // unchanged
	EXPECT_EQ(B3[0][0].available(), (list{V{1}, V{2}})) << "after 46";
	EXPECT_EQ(B3[0][1].available(), (list{V{1}, V{2}, V{3}})) << "after 47";
	EXPECT_EQ(B3[0][3].available(), (list{V{5}, V{6}, V{8}, V{9}}))
		<< "after 48";

	// TODO invalid value for count, should return 0
	// count = 1
	// count > elem_size / 2
}

TEST(Solver, solve_board)
{
	// clang-format off
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
	const std::vector<int> b1
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
	const std::vector<int> b1a
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
	setValue(options, b1.cbegin(), b1.cend());
	for (int i = 0; i < elem_size<3>; ++i)
	{
		unique_in_section(options, options.row(i));
	}
}
TEST(Solver, deathtest)
{
	using L = Location<2>;
	Board<Options<4>, 2> B{};

	// single_option()
#ifdef _DEBUG // note: release triggers normal exception
	// when wrong value
	B[1][2] = std::bitset<5>{"00011"}; // 1, not answer
	EXPECT_DEBUG_DEATH(
		single_option(B, L(1, 2), Value{4}), "Assertion failed: .*test.*");
#endif // _DEBUG
	// when more than 1 option available
	B[1][2] = std::bitset<5>{"10011"}; // 1, 4
	EXPECT_DEBUG_DEATH(
		single_option(B, L(1, 2), Value{1}), "Assertion failed: .*count_all.*");
}

} // namespace SudokuTests::SolversTest
