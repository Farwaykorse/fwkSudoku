/**	Unit tests for the template class Sudoku::SolverBase<N>
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "SolverBase.h"
// helpers

// library
#include <set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using val_t = unsigned int;

namespace Sudoku_Test
{
TEST_CLASS(Class_SolverBase)
{
public:
	// initializeing different objects
	// problems will create compile-time errors
	TEST_METHOD(initialize)
	{
		//std::vector<int> two_by_two = Test_Boards::b2_valid::full_1;
		std::vector<val_t> two_by_two
		{
			1,2,3,4,
			3,4,1,2,
			2,1,4,3,
			4,3,2,1
		};
		std::vector<val_t> two_by_two_1{ 0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

		/* empty start */
		//Sudoku::SolverBase<1> T0;	// won't compile (useless dimension)
		Sudoku::SolverBase<2> T1;
		Sudoku::SolverBase<3> T2;

		/* initlize using a Board */
		Sudoku::Board<val_t, 2> board1;
		std::copy(two_by_two.cbegin(), two_by_two.cend(), board1.begin());
		Sudoku::SolverBase<2> B1(board1);

		/* initialize using a vector */
		Sudoku::SolverBase<2> V1(two_by_two_1);

		/* initalise using self_type */
		Sudoku::SolverBase<2> T3(T1);

		// move
		// copy
	}
	TEST_METHOD(input_output)
	{
		/* input */
		// setBoard

		/* output */
		// getStart
		// getOptions
		// getResults
	}
	TEST_METHOD(board1)
	{
		/*	start board					answer board
		*	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
		*	|		| 		|   1 2 |	| 6 7 3	| 8 9 4	| 5 1 2 |
		*	|		|   3 5 |		|	| 9 1 2	| 7 3 5 | 4 8 6	|
		*	| _ _ _ | 6 _ _ | _ 7 _ |	|_8_4_5_|_6_1_2_|_9_7_3_|
		*	| 7		|       | 3     |	| 7 9 8	| 2 6 1	| 3 5 4 |
		*	|		| 4		| 8		|	| 5 2 6	| 4 7 3	| 8 9 1	|
		*	| 1 _ _ | _ _ _	| _ _ _ |	|_1_3_4_|_5_8_9_|_2_6_7_|
		*	| 		| 1 2   |		|	| 4 6 9	| 1 2 8 | 7 3 5	|
		*	|   8	|		|   4	|	| 2 8 7	| 3 5 6	| 1 4 9	|
		*	|_ _5_ _|_ _ _ _|_6_ _ _|	|_3_5_1_|_9_4_7_|_6_2_8_|
		*/
		static const std::vector<val_t> b1
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
		static const std::vector<val_t> b1a
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

		//TODO temporary SolverBase should accept input
		Sudoku::Board<val_t, 3> start;
		std::copy(b1.cbegin(), b1.cend(), start.begin());
		// answer to compare to
		Sudoku::Board<val_t, 3> answer;
		std::copy(b1a.cbegin(), b1a.cend(), answer.begin());

		// working object:
		Sudoku::SolverBase<3> other;
		// setup: generate options & find single options
		Sudoku::SolverBase<3> first(start);
		// find uniques (should solve this board)
		first.solver_unique();

		Assert::IsTrue(first.getResult()  == answer, L"solver_unique() failed", LINE_INFO());
	}
};
}