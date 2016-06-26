/**	Unit tests for the template class Sudoku::Board
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Board.h"
// helpers

// library
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(Class_Board)
{
public:
	// try initializing a board in different ways
	// will trigger compile-time errors
	TEST_METHOD(initialize)
	{
		using namespace Sudoku;
		// Board()
		// different sizes
		Board<int, 2> f2;
		Board<int, 3> first;
		Board<int, 10> f10; // 1 ms
		// different types
		Board<size_t, 3> b_size_t;
		Board<std::set<int>, 3> b_set; // 1 ms

		// Board(const value_type&)
		Board<int, 3> vat1(2);
		const std::set<int> s4{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		Board<std::set<int>, 3> f4(s4);	// 4 ms

		// Board(self_type&&)

		// Board& operator=(self_type&)

		//Board(const self_type&)
		Sudoku::Board<int, 3> copy(first);

		// Board& operator=(const self_type&)
		Sudoku::Board<int, 3> copy2 = first;



		analyze_values(first);
		analyze_values(f2);

		Assert::IsTrue(first.at(12) == 0 && first.at(73) == 0,
					   L"Empty board isn't filled with 0's", LINE_INFO());
		Assert::IsTrue(first.size() == 81, L"Function Error first.size()", LINE_INFO());

		//Assert::IsTrue(f4.at(43) == s4, L"Options board value set doesn't match input", LINE_INFO());

		Assert::IsTrue(first == copy, L"copy constructor failure", LINE_INFO());

		Assert::IsTrue(first == copy2, L"copy operation failure 2", LINE_INFO());
	}
	TEST_METHOD(element_access)
	{
		Sudoku::Board<int, 3> board;
		Sudoku::Board<int, 3> board2;
		Sudoku::Board<int, 3> empty;
		try
		{
			set_atValue(board);
		}
		catch (const std::exception&)
		{
			Assert::Fail(L"writing with atValue(elem) failed", LINE_INFO());
		}
		Assert::IsTrue(board.at(80) == 9, L"reading with atValue(elem) failed", LINE_INFO());

		try
		{
			set_atValue_2(board2);
		}
		catch (const std::exception&)
		{
			Assert::Fail(L"writing with atValue(row, col) failed", LINE_INFO());
		}
		Assert::IsTrue(board2.at(8, 8) == 9, L"reading with atValue(row, col) failed", LINE_INFO());

		Sudoku::Board<int, 3> board3;
		try
		{
			set_Value(board3);
		}
		catch (const std::exception&)
		{
			Assert::Fail(L"writing with operator[row][col] filed", LINE_INFO());
		}
		Assert::IsTrue(board3[8][8] == 9, L"reading with operator[row][col] failed", LINE_INFO());

		Assert::IsTrue(board == board2, L"failure operator==", LINE_INFO());
		Assert::IsFalse(board == empty, L"failure operator== (inverse)", LINE_INFO());
		Assert::IsTrue(board != empty, L"failure operator!=", LINE_INFO());
	}
	//TEST_METHOD(location)
	//{
	//	Sudoku::Board<int, 3> board;
	//	Assert::IsTrue(board.location(0, 0) == 0, L"Location failed 0", LINE_INFO());
	//	Assert::IsTrue(board.location(8, 8) == 80, L"Location failed 1", LINE_INFO());
	//	Assert::IsTrue(board.location(1, 1) == 10, L"Location failed 2", LINE_INFO());
	//	Assert::IsTrue(board.block_loc(0, 0) == 0, L"Location block failed 0", LINE_INFO());
	//	Assert::IsTrue(board.block_loc(8, 8) == 80, L"Location block failed 1", LINE_INFO());
	//	Assert::IsTrue(board.block_loc(3, 1) == 28, L"Location block failed 2", LINE_INFO());
	//	Assert::IsTrue(board.block_loc(8, 3) == 69, L"Location block failed 3", LINE_INFO());
	//	Assert::IsFalse(board.location(1, 1) == 0, L"Location board, test", LINE_INFO());
	//}
private:
	template<size_t N>
	void analyze_values(Sudoku::Board<int, N> B) const
	{
		// independend size calculations
		static_assert(N > 0 && N < 11, "Size out of bounds, for testing a 10'000 elements board should be enough.");
		constexpr size_t elem{ N*N };
		constexpr size_t full{ elem*elem };
		static_assert(B.base_size == N, "base_size wrong");
		static_assert(B.elem_size == elem, "elem_size wrong");
		static_assert(B.full_size == full, "full_size wrong");

		//static_assert(B.elem_size == B.numCols(), "Function Error numCols()");
		//static_assert(B.elem_size == B.numRows(), "Function Error numRows()");
		//static_assert(B.elem_size == B.block_size(), "Function Error block_size()");
		static_assert(B.full_size == B.size(), "Function Error size()");
	}

	template<size_t N>
	void set_atValue(Sudoku::Board<int, N>& target)
	{
		constexpr size_t elem{ N*N };
		for (size_t i = 0; i < target.size(); ++i)
		{
			target.at(i) = i % elem + 1;
		}
	}

	template<size_t N>
	void set_Value(Sudoku::Board<int, N>& target)
	{
		for (size_t row = 0; row < target.col(0).size(); ++row)
		{
			for (size_t col = 0; col < target.row(0).size(); ++col)
			{
				target[row][col] = col + 1;
			}
		}
	}

	template<size_t N>
	void set_atValue_2(Sudoku::Board<int, N>& target)
	{
		for (size_t row = 0; row < target.col(0).size(); ++row)
		{
			for (size_t col = 0; col < target.row(0).size(); ++col)
			{
				target.at(row, col) = col + 1;
			}
		}
	}
};

} // namespace Sudoku_Test