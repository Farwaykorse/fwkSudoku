/**	Unit tests for the template class Sudoku::SolverBase<N>
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "SolverBase.h"
// helpers
//#include "uTestObjects.h"

// library
#include <set>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
		std::vector<int> two_by_two
		{
			1,2,3,4,
			3,4,1,2,
			2,1,4,3,
			4,3,2,1
		};
		std::vector<int> two_by_two_1{ 0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

		/* empty start */
		//Sudoku::SolverBase<1> T0;	// won't compile (useless dimension)
		Sudoku::SolverBase<2> T1;
		Sudoku::SolverBase<3> T2;

		/* initlize using a Board */
		Sudoku::Board<int, 2> board1;
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
	TEST_METHOD(single_option)
	{
		using namespace Sudoku;
		std::vector<int> v1{ 0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
		SolverBase<2> V1(v1);
		Board<std::set<int>, 2> options = V1.getOptions();
		Assert::IsTrue(options.at(1).size() == 1, L"setValue() clear()&insert() failed", LINE_INFO());
		Assert::IsTrue(
			options.at(0).size() == 3 &&
			options.at(2).size() == 3 &&
			options.at(3).size() == 3,
			L"single_option() failed for Row", LINE_INFO()
		);
		Assert::IsTrue(options.at(4).size() == 3, L"singleValue() failed for Block", LINE_INFO());
		Assert::IsTrue(options.at(9).size() == 3, L"singleValue() failed for Row", LINE_INFO());
		Assert::IsTrue(options.at(15).size() == 4, L"singleValue() error", LINE_INFO());

		// single_option cascading
		std::vector<int> v2
		{
			0,2, 0,0,
			4,0, 0,0,
			0,1, 0,0,
			0,0, 0,0
		};
		std::vector<int> v2a
		{
			1,2, 0,0,
			4,3, 0,0,
			0,1, 0,0,
			0,4, 0,0
		};
		SolverBase<2> V2(v2);
		Board<int, 2> answer{};
		std::copy(v2a.cbegin(), v2a.cend(), answer.begin());
		Assert::IsTrue(answer == V2.getResult(), L"singleValue() cascade failed", LINE_INFO());
		Board<int, 2> start{};
	}
	TEST_METHOD(solver_unique)
	{
		// test if working on Row(0)
		static const std::vector<int> v1
		{
			0,2, 0,0,	// 1,3	2	1,3	1,3,4
			4,0, 0,0,
			0,1, 4,0,
			0,0, 0,0
		};
		Sudoku::SolverBase<2> test1(v1);
		test1.solver_unique();
		Sudoku::Board<int, 2> result = test1.getResult();
		Assert::IsTrue(result.at(3) == 4, L"failed", LINE_INFO());
		Assert::IsTrue(result.at(1) == 2, L"removed valid answer",LINE_INFO());
		Assert::IsTrue(result.at(0) == 1 && result.at(2) == 3, L"failed to use single_option() properly", LINE_INFO());
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
		static const std::vector<int> b1
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
		static const std::vector<int> b1a
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
		Sudoku::Board<int, 3> start;
		std::copy(b1.cbegin(), b1.cend(), start.begin());
		Sudoku::Board<int, 3> answer;
		std::copy(b1a.cbegin(), b1a.cend(), answer.begin());
		Sudoku::SolverBase<3> first(start);
		first.solver_unique();
		Assert::IsTrue(first.getResult()  == answer, L"solver_unique() failed", LINE_INFO());
		int a = 1;
	}
};
}