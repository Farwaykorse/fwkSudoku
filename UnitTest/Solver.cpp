/**	Unit tests for the templates in the namespace Sudoku::Solver
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// File under test
#include <Sudoku/Solver.h>
// helpers
#include <Sudoku/Board.h>
#include <Sudoku/Options.h>

// library
#include <vector>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(NS_Solver)
{
public:
	TEST_METHOD(T0_initialization)
	{
		using typeT = Sudoku::Solver<3>;
		static_assert(std::is_class<typeT>(), "a class, hiding datarepresentation");
		static_assert(! std::is_empty<typeT>(), "no datamembers");
		static_assert(! std::is_standard_layout<typeT>(), "standard layout");
		static_assert(! std::is_trivial<typeT>(), "not user-provided, no virtuals");
		static_assert(! std::is_pod<typeT>(), "Plain Old Data, both trivial and standard-layout");
		//static_assert(std::has_unique_object_representations<typeT>(), "");	//C++17

		static_assert(! std::is_default_constructible<typeT>(), "default constructor");
		static_assert(! std::is_nothrow_default_constructible<typeT>(), "notrow default constructor");
		static_assert(! std::is_trivially_default_constructible<typeT>(), "tribial default constructor");

		static_assert(std::is_destructible<typeT>(), "destructable");
		static_assert(std::is_nothrow_destructible<typeT>(), "nothrow destructable");
		static_assert(std::is_trivially_destructible<typeT>(), "trivially destructable");

		static_assert(std::is_copy_constructible<typeT>(), "copy constructor");
		static_assert(std::is_nothrow_copy_constructible<typeT>(), "notrow copy constructor");
		static_assert(std::is_trivially_copy_constructible<typeT>(), "trivially copy constructor");

		static_assert(std::is_move_constructible<typeT>(), "move constructor");
		static_assert(std::is_nothrow_move_constructible<typeT>(), "nothrow move constructor");
		static_assert(std::is_trivially_move_constructible<typeT>(), "trivially move constructor");

		static_assert(! std::is_copy_assignable<typeT>(), "copy assignable");
		static_assert(! std::is_nothrow_copy_assignable<typeT>(), "notrow copy assignable");
		static_assert(! std::is_trivially_copy_assignable<typeT>(), "trivially copy assignable");

		static_assert(! std::is_move_assignable<typeT>(), "move assignable");
		static_assert(! std::is_nothrow_move_assignable<typeT>(), "move assignable");
		static_assert(! std::is_trivially_move_assignable<typeT>(), "trivially move assignable");

		static_assert(! std::is_trivially_copyable<typeT>(), "trivially copyable");
		static_assert(! std::is_swappable<typeT>(), "swappable");			//C++17
		static_assert(! std::is_nothrow_swappable<typeT>(), "swappable");	//C++17
		static_assert(! std::is_swappable_with<typeT, typeT>(), "");	//C++17
		static_assert(! std::is_nothrow_swappable_with<typeT, typeT>(), "");	//C++17
	}
	TEST_METHOD(T1_setValue)
	{
		using namespace Sudoku;

		const std::vector<int> v1
		{
			// start	// after setValue
			0,2, 0,0,	// 1	2	3	4
			4,0, 0,0,	// 4	3	1,2	1,2
			0,1, 4,0,	// 2,3	1	4	2,3
			0,0, 0,0	// 2,3	4	1,2	1,2,
		};
		// Set single value
		Board<Options<4>, 2> board;
		Assert::IsTrue(board[1][1] == Options<4>{}, L"incorrect instantiation");
		try { Solver<2>(board).setValue(Location<2>(2), Value{3}); }
		catch (...) { Assert::Fail(L"setValue failed"); }
		Assert::IsTrue(board[0][2] == Value{3}, L"setValue failed to set the value");
		Solver<2>(board).setValue(Location<2>(0), Value{4});
		Solver<2>(board).setValue(Location<2>(15), Value{4});
		Assert::IsTrue(board[3][3] == Value{4} && board[0][0] == Value{4}, L"setValue failed to set extremes");
		//Solver<2>(board).setValue(Location<2>(2), 4);
		//Assert::IsTrue(board[0][2] == 4, L"setValue failed to overwrite");

		// Copy data from vector
		Board<Options<4>, 2> board1;
		try { Solver<2>(board1).setValue(v1.cbegin(), v1.cend()); }
		catch (...) { Assert::Fail(L"setValue failed in copying from vector"); }
		Assert::IsTrue(
			board1[0][1] == Value{2} &&
			board1[1][0] == Value{4} &&
			board1[2][1] == Value{1} && board1[2][2] == Value{4},
			L"setValue() from vector didn't copy data"
		);
		Assert::IsTrue(
			board1[0][3] == Value{4} &&
			board1[3][1] == Value{4},
			L"setValue() from vector didn't process single option cells"
		);
		Assert::IsTrue(
			board1[0][0] == Value{1} && board1[0][2] == Value{3} &&
			board1[1][1] == Value{3} &&
			board1[3][1] == Value{4},
			L"setValue() from vector didn't cascade over single option cells"
		);
	}
	TEST_METHOD(T2_unique)
	{
		using namespace Sudoku;

		const std::vector<int> v1
		{
			// start	// after setValue
			0,2, 0,0,	// 1	2	3	4
			4,0, 0,0,	// 4	3	1,2	1,2
			0,1, 4,0,	// 2,3	1	4	2,3
			0,0, 0,0	// 2,3	4	1,2	1,2,
		};
		Board<Options<4>, 2> B1;
		Assert::IsTrue(B1[0][0] == Options<4>{} && B1[3][3] == Options<4>{}, L"incorrect instantiation");
		Solver<2>(B1).setValue(v1.cbegin(), v1.cend());
		const Board<Options<4>, 2> cB1{ B1 };	// copy to compare with
		Assert::IsTrue(B1[0][1] == Value{2} && B1[1][0] == Value{4} && B1[2][2] == Value{4}, L"setup error");
		Assert::IsTrue(B1 == cB1, L"copy error");
		// single row
		Solver<2>(B1).unique_in_section(B1.row(0).cbegin(), B1.row(0).cend());
		Assert::IsTrue(B1 == cB1, L"row 0 was completely fixed by setValue");

		const std::vector<int> v2
		{
			//start		// after setValue
			3,2, 0,0,	// 3	2	14	14		3	2	14	14
			0,0, 0,0,	// 14	14	3	2		14	14	3	2
			0,0, 2,0,	// 14	134	2	134		14	134	2	134
			0,0, 0,0	// 12.4	134	134	134		2	134	14	134
		};
		Board<Options<4>, 2> B2{};
		Solver<2>(B2).setValue(v2.cbegin(), v2.cend());
		const Board<Options<4>, 2> cB2{ B2 };
		// single row 0
		Solver<2>(B2).unique_in_section(B2.row(0).cbegin(), B2.row(0).cend());
		Assert::IsTrue(B2 == cB2, L"row 0 was completely fixed by setValue");
		// single row 1
		//Solver<2>(B2).unique_in_section(B2.row(3).cbegin(), B2.row(3).cend());
		//Assert::IsTrue(B2 != cB2, L"row 3 should have changed");
		// full board
		try
		{
			for (int i = 0; i < B2.elem_size; ++i)
			{
				Solver<2>(B2).unique_in_section(B2.row(i).cbegin(), B2.row(i).cend());
			}
		}
		catch (...) { Assert::Fail(L"unique_in_section() threw an exception"); }
		Assert::IsTrue(
			B2[0][0] == Value{3} && B2[0][1] == Value{2} &&
			B2[2][2] == Value{2},
			L"input values are lost"
		);
		Assert::IsTrue(
			B2[1][3] == Value{2} && B2[3][0] == Value{2},
			L"not all uniques found in rows"
		);
	}
	TEST_METHOD(T3_solve_board)
	{
		using namespace Sudoku;
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
		Board<int, 3> start;
		std::copy(b1.cbegin(), b1.cend(), start.begin());
		Board<int, 3> answer;
		std::copy(b1a.cbegin(), b1a.cend(), answer.begin());
		Board<Options<9>, 3> options{};
		Solver<3>(options).setValue(b1.cbegin(), b1.cend());
		for (int i = 0; i < options.elem_size; ++i)
		{
			Solver<3>(options).unique_in_section(options.row(i).cbegin(), options.row(i).cend());
		}
	}
	TEST_METHOD(T4_block_exclusive)
	{
		using namespace Sudoku;

		// reproduce functionality of unique
		static const std::vector<int> V1
		{
			// start	// after setValue	// unique_block
			0,0, 1,0,	// 
			1,0, 0,0,	// 
			0,1, 0,0,	// 
			0,0, 0,0	//					//	0	0	0	1
		};
		Board<Options<4>, 2> B1{};
		//Solver<2>::setValue(B1, V1.cbegin(), V1.cend());
		Solver<2>(B1).setValue(V1.cbegin(), V1.cend());
		Assert::IsTrue(
			1 == Solver<2>(B1).block_exclusive(B1.block(3).cbegin(), B1.block(3).cend()),
			L"block_exclusive() should find 1 value");
		Assert::IsTrue(B1[3][3] == Value{1}, L"block_exclusive() unique value failed");
		int found1{ 0 };
		for (int i{ 0 }; i < B1.elem_size; ++i)
		{
			found1 += Solver<2>(B1).block_exclusive(B1.block(i).cbegin(), B1.block(i).cend());
		}
		Assert::IsTrue(found1 == 0, L"shouldn't find any others");

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
		Sudoku::Board<Options<9>, 3> B2{};
		Solver<3>(B2).setValue(V2.cbegin(), V2.cend());
		// block 0
		//		nothing
		// block 1
		//		double: 1 in row 2;	2 not paired
		// block 2
		//		unique: 9=3
		int count_s = Solver<3>(B2).block_exclusive(B2.block(2).cbegin(), B2.block(2).cend());

		Assert::IsTrue(
			count_s >= 1,
			L"block_exclusive() should find at least 1 value");
		Assert::IsTrue(B2[2][8] == Value{3}, L"block_exclusive() unique value failed N=3");

		for (int i = 0; i < B2.elem_size; ++i)
		{
			Solver<3>(B2).block_exclusive(B2.block(i).cbegin(), B2.block(i).cend());
		}
	}
	//TEST_METHOD(T5_single_option)
	//{
	//TESTNEEDED single_option
	//	const std::vector<int> b1a
	//	{
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0,
	//		0, 0, 0,	0, 0, 0,	0, 0, 0
	//	};
	//}
	TEST_METHOD(T6_dual_option)
	{
		using namespace Sudoku;
		/*	start board				
		*	 _ _ _ _ _ _ _ _ _ _ _ _
		*	|     3 |      	| - 1,2 |	test for row
		*	| 4 5 6 |       |       |
		*	|_7_8_9_|_ _ _ _|_ _ _ _|	
		*	|   	|       |   	|	
		*	|	    |   all |       |	
		*	|_ _ _ _|_ _ _ _|_ _ _ _|	
		*	| 		|    	| 1 2 3 |	
		*	|	    |	    | 4 5   |	test for col
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
		};
		Board<Options<9>, 3> B1;
		Solver<3> Run1(B1);
		try { Run1.setValue(b1.cbegin(), b1.cend()); }
		catch (...) { Assert::Fail(L"setValue failed in copying from vector"); }
		Assert::IsTrue(B1[0][0].count() == 2, L"dual_option before 1");
		Assert::IsTrue(B1[0][8].count() == 8, L"dual_option before 2");
		Assert::IsTrue(B1[4][4].count() == 9, L"dual_option before 3");
		Assert::IsTrue(B1[8][0].count() == 6, L"dual_option before 4");
		try { Run1.dual_option(Location<3>(0)); }
		catch (...) { Assert::Fail(L"dual_option failed 1"); }
		Assert::IsTrue(B1[0][0].count() == 2, L"dual_option 1"); // unchanged
		Assert::IsTrue(B1[0][8].count() == 6, L"dual_option 2");
		Assert::IsTrue(B1[4][4].count() == 9, L"dual_option 3"); // unchanged
		Assert::IsTrue(B1[8][0].count() == 6, L"dual_option 4"); // unchanged
		try { Run1.dual_option(Location<3>(1)); }
		catch (...) { Assert::Fail(L"dual_option failed 2"); }
		Assert::IsTrue(B1[0][0].count() == 2, L"dual_option 5"); // unchanged
		Assert::IsTrue(B1[0][8].count() == 6, L"dual_option 6"); // unchanged
		Assert::IsTrue(B1[4][4].count() == 9, L"dual_option 7"); // unchanged
		Assert::IsTrue(B1[8][0].count() == 6, L"dual_option 8"); // unchangedtry { Run1.dual_option(Location<3>(0)); }
		try { Run1.dual_option(Location<3>(80)); }
		catch (...) { Assert::Fail(L"dual_option failed 3"); }
		Assert::IsTrue(B1[0][0].count() == 2, L"dual_option 9"); // unchanged
		Assert::IsTrue(B1[0][8].count() == 4, L"dual_option 10");
		Assert::IsTrue(B1[4][4].count() == 9, L"dual_option 11"); // unchanged
		Assert::IsTrue(B1[8][0].count() == 6, L"dual_option 12"); // unchanged
	}
	TEST_METHOD(T7_multi_option)
	{
		using namespace Sudoku;
		/*	start board				
		*	 _ _ _ _ _ _ _ _ _ _ _ _
		*	|       |      	|       |	test for row
		*	| 4 5 6 |       |       |
		*	|_7_8_9_|_ _ _ _|_ _ _ _|	
		*	|   	|       |   	|	
		*	|	    |   all |       |	
		*	|_ _ _ _|_ _ _ _|_ _ _ _|	
		*	| 		|    	| 1 2   |	
		*	|	    |	    | 4 5   |	test for col
		*	|_ _ _ _|_ _ _ _|_7_8_ _|	
		*	before: 0,8: contains all
		*	after:  0,8: contains only 4,5,7,8 (not 1,2,3,6,9)
		*	no change:  block 4 contains all options in all cells
		*/
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
		};
		Board<Options<9>, 3> B1;
		Solver<3> Run1(B1);
		try { Run1.setValue(b1.cbegin(), b1.cend()); }
		catch (...) { Assert::Fail(L"setValue failed in copying from vector"); }
		Assert::IsTrue(B1[0][0].count() == 3, L"before 1");
		Assert::IsTrue(B1[0][8].count() == 9, L"before 2");
		Assert::IsTrue(B1[4][4].count() == 9, L"before 3");
		Assert::IsTrue(B1[8][8].count() == 3, L"before 4");
		// run for row
		try { Run1.multi_option(Location<3>(2)); }
		catch (...) { Assert::Fail(L"multi_option failed 1"); }
		Assert::IsTrue(B1[0][0].count() == 3, L"after 1"); // unchanged
		Assert::IsTrue(B1[0][8].count() == 6, L"after 2");
		Assert::IsTrue(B1[4][4].count() == 9, L"after 3"); // unchanged
		//Assert::IsTrue(B1[0][8].available() == std::vector<int>{4,5,6,7,8,9}, L"after 4");
		// run for col
		try { Run1.multi_option(Location<3>{6, 8}); }
		catch (...) { Assert::Fail(L"multi_option failed 2"); }
		Assert::IsTrue(B1[0][0].count() == 3, L"after 21"); // unchanged
		Assert::IsTrue(B1[8][8].count() == 3, L"after 22"); // unchanged
		Assert::IsTrue(B1[0][8].count() == 4, L"after 23");
		Assert::IsTrue(B1[4][4].count() == 9, L"after 24"); // unchanged
		//Assert::IsTrue(B1[0][8].available() == std::vector<int>{4,5,7,8}, L"after 25");
		// run for block
		const std::vector<int> b2
		{										//	 _ _ _ _ _ _ _ _ _ _ _ _
			0, 0, 3,	0, 0, 0,	7, 0, 0,	//	|     3 |       | 7     |	diagonal: 1,5,9
			4, 0, 6,	2, 7, 0,	0, 0, 0,	//	| 4   6 | 2 7   |       |
			0, 8, 0,	0, 0, 0,	0, 0, 0,	//	|_ _8_ _|_ _ _ _|_ _ _ _|	
			2, 0, 0,	0, 0, 0,	0, 0, 0,	//	| 2     |       |       |	
			0, 0, 7,	0, 0, 0,	0, 0, 0,	//	|     7 |       |       |	
			0, 0, 0,	0, 0, 0,	0, 0, 0,	//	|_ _ _ _|_ _ _ _|_ _ _ _|	
			0, 0, 2,	0, 0, 0,	0, 0, 0,	//	|     2 |       |       |	
			0, 0, 0,	0, 0, 0,	0, 0, 0,	//	|       |       |       |	test for col
			0, 0, 0,	0, 0, 0,	0, 0, 0		//	|_ _ _ _|_ _ _ _|_ _ _ _|	
		};
		Board<Options<9>, 3> B2;
		Solver<3> Run2(B2);
		try { Run2.setValue(b2.cbegin(), b2.cend()); }
		catch (...) { Assert::Fail(L"setValue failed in copying from vector 3"); }
		Assert::IsTrue(B2[0][0].count() == 3, L"before 31");
		Assert::IsTrue(B2[1][1].count() == 3, L"before 32");
		Assert::IsTrue(B2[2][2].count() == 3, L"before 33");
		Assert::IsTrue(B2[0][1].count() == 4, L"before 34");
		Assert::IsTrue(B2[2][0].count() == 4, L"before 35");
		//Assert::IsTrue(B2[0][0].available() == std::vector<int>{1,5,9}, L"before 36");
		//Assert::IsTrue(B2[2][0].available() == std::vector<int>{1,5,7,9}, L"before 37");
		Assert::IsTrue(B2[8][8].count() == 9, L"before 38");
		Assert::IsTrue(B2[2][5].count() == 6, L"before 39");
		//Assert::IsTrue(B2[2][5].available() == std::vector<int>{1,3,4,5,6,9}, L"before 310");
		try { Run2.multi_option(Location<3>(0)); }
		catch (...) { Assert::Fail(L"multi_option failed 3"); }
		Assert::IsTrue(B2[0][0].count() == 3, L"after 31");
		Assert::IsTrue(B2[1][1].count() == 3, L"after 32");
		Assert::IsTrue(B2[2][2].count() == 3, L"after 33");
		Assert::IsTrue(B2[0][1].is_answer(2), L"after 34");
		Assert::IsTrue(B2[2][0].is_answer(7), L"after 35");
		//Assert::IsTrue(B2[0][0].available() == std::vector<int>{1,5,9}, L"after 36");
		Assert::IsTrue(B2[8][8].count() == 9, L"after 38");
		Assert::IsTrue(B2[2][5].count() == 6, L"after 39");
		//Assert::IsTrue(B2[2][5].available() == std::vector<int>{1,3,4,5,6,9}, L"after 310");

		//NEEDTEST 9*9 partials forming a set: 3 cels containing (123,12,13)
		const std::vector<int> b3
		{										//	 _ _ _ _ _ _ _ _ _ _ _ _
			0, 0, 0,	0, 0, 0,	0, 0, 0,	//	|       |       |       |	row: 1,2; 1,2,3; 1,2,3
			4, 5, 6,	0, 0, 0,	0, 0, 0,	//	| 4 5 6 |       |       |
			7, 8, 9,	0, 0, 0,	0, 0, 0,	//	|_7_8_9_|_ _ _ _|_ _ _ _|	
			0, 0, 0,	0, 2, 3,	0, 0, 0,	//	| 3     |   2 3 |       |	
			3, 0, 0,	4, 0, 6,	0, 0, 0,	//	|       | 4   6 |       |	
			0, 0, 0,	7, 8, 0,	0, 0, 0,	//	|_ _ _ _|_7_8_ _|_ _ _ _|	
			0, 0, 0,	0, 9, 0,	1, 2, 0,	//	|       |   9   | 1 2   |		 3,6
			0, 0, 0,	0, 3, 0,	4, 5, 0,	//	|       |   3 1 | 4 5   |	col: 6,9
			0, 0, 0,	0, 0, 0,	7, 8, 0		//	|_ _ _ _|_ _ _5_|_7_8_ _|		 3,6,9
		};
		Board<Options<9>, 3> B3;
		Solver<3> Run3(B3);
		try { Run3.setValue(b3.cbegin(), b3.cend()); }
		catch (...) { Assert::Fail(L"setValue failed in copying from vector 3"); }
		Assert::IsTrue(B3[0][0].count() == 2, L"before 41");
		Assert::IsTrue(B3[0][1].count() == 3, L"before 42");
		Assert::IsTrue(B3[0][2].count() == 3, L"before 43");
		Assert::IsTrue(B3[0][3].count() == 7, L"before 44");
		//Assert::IsTrue(B3[5][1].count() == 6, L"before 45");
		//Assert::IsTrue(B3[0][1].available() == std::vector<int>{1,2,3}, L"before 46");
		Assert::IsTrue(B3[6][8].count() == 2, L"before 47");
		Assert::IsTrue(B3[7][8].count() == 2, L"before 48");
		Assert::IsTrue(B3[8][8].count() == 3, L"before 49");
		//Assert::IsTrue(B3[8][8].available() == std::vector<int>{3,6,9}, L"before 410");
		try { Run3.multi_option(Location<3>(1)); }
		catch (...) { Assert::Fail(L"multi_option failed 4"); }
		// row:
		Assert::IsTrue(B3[0][0].count() == 2, L"after 41");	// unchanged
		Assert::IsTrue(B3[0][1].count() == 3, L"after 42");	// unchanged
		Assert::IsTrue(B3[0][2].count() == 3, L"after 43"); // unchanged
		Assert::IsTrue(B3[0][3].count() == 4, L"after 44");
		//Assert::IsTrue(B3[5][1].count() == 6, L"after 45"); // unchanged
		//Assert::IsTrue(B3[0][0].available() == std::vector<int>{1,2}, L"after 46");
		//Assert::IsTrue(B3[0][1].available() == std::vector<int>{1,2,3}, L"after 47");
		//Assert::IsTrue(B3[0][3].available() == std::vector<int>{5,6,8,9}, L"after 48");
	
	}
};
}	// namespace Sudoku_Test
