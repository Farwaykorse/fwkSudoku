/**	Unit tests for the template class Sudoku::Board
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Board.h"
// helpers
#include "Location.h"

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
		Board<int, 3> f3;
		Board<int, 10> f10; // 1 ms
		// different types
		Board<size_t, 3> b_size_t;
		Board<std::set<int>, 3> b_set; // 1 ms

		// Board(const value_type&)
		Board<int, 3> vat1(2);
		const std::set<int> s4{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		Board<std::set<int>, 3> f4(s4);	// 4 ms

		// Board(self_type&&)					move constructor
		Board<int, 3> A(Board<int, 3>(2));
		// Board& operator=(self_type&&)		move assignment
		Board<int, 3> B = Board<int, 3>(9);
		//Board(const self_type&)				copy constructor
		Sudoku::Board<int, 3> copy(f3);
		// Board& operator=(const self_type&)	copy assignment
		Sudoku::Board<int, 3> copy2 = f3;

		// Checks
		analyze_values(f3);
		analyze_values(f2);

		Assert::IsTrue(f3.at(12) == 0 && f3.at(73) == 0,
					   L"Empty board isn't filled with 0's", LINE_INFO());
		Assert::IsTrue(f3.size() == 81, L"Function Error first.size()", LINE_INFO());
		Assert::IsTrue(f4.at(43) == s4, L"Options board value set doesn't match input", LINE_INFO());
		Assert::IsTrue(f3 == copy, L"copy constructor failure", LINE_INFO());
		Assert::IsTrue(f3 == copy2, L"copy operation failure 2", LINE_INFO());
	}
	TEST_METHOD(clear)
	{
		using namespace Sudoku;
		// test objects
		Board<int, 3> f3(5);
		Board<std::set<int>, 2> b_set({ 1, 2 });
		// check begin state
		Assert::IsTrue(f3.size() == 81 && f3.at(45) == 5 && f3.at(32) == 5,
					   L"Test Board incorrect");
		Assert::IsTrue(b_set.size() == 16 && b_set.at(12).size() == 2,
					   L"Test Board set incorrect");
		// process
		f3.clear();
		b_set.clear();
		// check result
		Assert::IsTrue(f3.size() == 81, L"clear() not retaining size", LINE_INFO());
		Assert::IsTrue(f3.at(17) == 0, L"clear() not removing values", LINE_INFO());
		Assert::IsTrue(b_set.size() == 16, L"clear() not retaining board size", LINE_INFO());
		Assert::IsTrue(b_set.at(8).empty(), L"clear() not removing values", LINE_INFO());
	}
	TEST_METHOD(properties)
	{
		// test objects
		Sudoku::Board<int, 3> board(1);
		Sudoku::Board<int, 3> board2(board);
		Sudoku::Board<int, 3> empty;
		Sudoku::Board<std::set<int>, 2> b_set0;
		Sudoku::Board<std::set<int>, 2> b_set1({ 1,2 });
		Sudoku::Board<std::set<int>, 2> b_set2(b_set1);

		// empty()
		//Assert::IsTrue(empty.empty(), L"failure empty() on int", LINE_INFO());
		//Assert::IsTrue(b_set0.empty(), L"failure empty() on set", LINE_INFO());

		// operator==()
		Assert::IsTrue(board == board, L"failure operator== on self", LINE_INFO());
		Assert::IsTrue(board == board2, L"failure operator==", LINE_INFO());
		Assert::IsFalse(board == empty, L"failure operator== (inverse)", LINE_INFO());
		Assert::IsTrue(b_set1 == b_set2, L"failure operator== for set", LINE_INFO());
		Assert::IsFalse(b_set0 == b_set1, L"failure operator== for set (inverse)", LINE_INFO());

		// operator!=()
		Assert::IsTrue(board != empty, L"failure operator!=", LINE_INFO());
		Assert::IsFalse(board != board, L"failure operator!= on self", LINE_INFO());
		Assert::IsFalse(board != board2, L"failure operator!=", LINE_INFO());
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

		Sudoku::Board<int, 3> board4;
		try
		{
			set_Value(board4);
		}
		catch (const std::exception&)
		{
			Assert::Fail(L"writing with operator[Location] filed", LINE_INFO());
		}
		Assert::IsTrue(board4[Sudoku::Location<3>(80)] == 9, L"reading with operator[Location] failed", LINE_INFO());


		// operator==() / operator!=()
		// repeat from TEST_METHOD(properties)
		Assert::IsTrue(board == board2, L"failure operator==", LINE_INFO());
		Assert::IsFalse(board == empty, L"failure operator== (inverse)", LINE_INFO());
		Assert::IsTrue(board != empty, L"failure operator!=", LINE_INFO());
	}
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

	template<size_t N>
	void set_Value_2(Sudoku::Board<int, N>& target)
	{
		constexpr size_t elem{ N*N };
		for (size_t i = 0; i < target.size(); ++i)
		{
			target[Location(i)] = i % elem + 1;
		}
	}
};

} // namespace Sudoku_Test