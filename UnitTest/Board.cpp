/**	Unit tests for the template class Sudoku::Board
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Board.h"
// helpers
#include "Location.h"
#include "Options.h"

// library
#include <set>
#include <bitset>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(Class_Board)
{
public:
	// try initializing a board in different ways
	// will trigger compile-time errors
	TEST_METHOD(T1_initialize)
	{
		using namespace Sudoku;
		// Board
		static_assert(std::is_default_constructible<Board<int, 3>>(), "default constructor");
		//static_assert(std::is_trivially_constructible<Board<int, 3>>(), "default constructor");
		//static_assert(std::is_nothrow_default_constructible<Board<int, 3>>(), "notrow default constructor");
		static_assert(std::is_copy_constructible<Board<int, 3>>(), "copy constructor");
		//static_assert(std::is_trivially_copy_constructible<Board<int, 3>>(), "trivially copy constructor");
		//static_assert(std::is_nothrow_copy_constructible<Board<int, 3>>(), "notrow copy constructor");
		static_assert(std::is_move_constructible<Board<int, 3>>(), "move constructor");
		//static_assert(std::is_trivially_move_constructible<Board<int, 3>>(), "trivially move constructor");
		static_assert(std::is_nothrow_move_constructible<Board<int, 3>>(), "nothrow move constructor");
		static_assert(std::is_copy_assignable<Board<int, 3>>(), "copy assignable");
		//static_assert(std::is_trivially_copy_assignable<Board<int, 3>>(), "trivially copy assignable");
		//static_assert(std::is_nothrow_copy_assignable<Board<int, 3>>(), "notrow copy assignable");
		static_assert(std::is_move_assignable<Board<int, 3>>(), "move assignable");
		//static_assert(std::is_trivially_move_assignable<Board<int, 3>>(), "trivially move assignable");
		static_assert(std::is_nothrow_move_assignable<Board<int, 3>>(), "move assignable");
		static_assert(std::is_destructible<Board<int, 3>>(), "destructable");
		//static_assert(std::is_trivially_destructible<Board<int, 3>>(), "trivially destructable");
		static_assert(std::is_nothrow_destructible<Board<int, 3>>(), "nothrow destructable");

		// different sizes
		static const Board<int, 2> f2;
		static const Board<int, 3> f3;
		compiler_checks(f2);
		compiler_checks(f3);
		compiler_checks(Board<int,10>());
		static_assert(f2.size() == 16, "size() value error[f2]");
		static_assert(f3.size() == 81, "size() value error[f3]");
		// different types
		compiler_checks(Board<size_t, 3>());
		compiler_checks(Board<std::set<int>, 2>());
		compiler_checks(Board<std::bitset<9>,3>());
		compiler_checks(Board<Options<9>,3>());

		// default element value : Board(const value_type&)
		compiler_checks(Board<int, 3>(2));

		// Board(self_type&&)					move constructor
		Board<int, 3> A(Board<int, 3>(2));
		// Board& operator=(self_type&&)		move assignment
		Board<int, 3> B = Board<int, 3>(9);
		//Board(const self_type&)				copy constructor
		Sudoku::Board<int, 3> copy(f3);
		// Board& operator=(const self_type&)	copy assignment
		//TEST not triggered: copy assignment
		Sudoku::Board<int, 3> copy2 = f3;

		// using at(elem_id) !
		Assert::IsTrue(f3.at(12) == 0 && f3.at(73) == 0,
						L"Empty board isn't filled with 0's", LINE_INFO());
		Assert::IsTrue(A.at(21) == 2, L"move constructor value doesn't match input", LINE_INFO());
		Assert::IsTrue(B.at(39) == 9, L"move assignment, value doesn't match input", LINE_INFO());
		Assert::IsTrue(f3 == copy, L"copy constructor failure", LINE_INFO());
		Assert::IsTrue(f3 == copy2, L"copy operation failure 2", LINE_INFO());
	}
	TEST_METHOD(T2_properties)
	{
		using namespace Sudoku;
		// test objects
		Board<int, 3> board(1);
		Board<int, 3> board2(board);
		Board<int, 3> empty;
		Board<std::set<int>, 2> b_set0;
		Board<std::set<int>, 2> b_set1({ 1,2 });
		Board<std::set<int>, 2> b_set2(b_set1);
		Board<Options<9>, 3> O_1;
		Board<Options<9>, 3> O_2(O_1);

		// empty()
		//Assert::IsTrue(empty.empty(), L"failure empty() on int", LINE_INFO());

		// operator==()
		Assert::IsTrue(board == board, L"failure operator== on self", LINE_INFO());
		Assert::IsTrue(board == board2, L"failure operator==", LINE_INFO());
		Assert::IsFalse(board == empty, L"failure operator== (inverse)", LINE_INFO());
		Assert::IsTrue(b_set1 == b_set2, L"failure operator== for set", LINE_INFO());
		Assert::IsFalse(b_set0 == b_set1, L"failure operator== for set (inverse)", LINE_INFO());
		Assert::IsTrue(O_1 == O_2, L"failure operator== for Options", LINE_INFO());

		// operator!=()
		Assert::IsTrue(board != empty, L"failure operator!=", LINE_INFO());
		Assert::IsFalse(board != board, L"failure operator!= on self", LINE_INFO());
		Assert::IsFalse(board != board2, L"failure operator!=", LINE_INFO());

		Sudoku::Board<int,2> B;
		Assert::IsTrue(B.base_size == 2, L"base_size error", LINE_INFO());
		Assert::IsTrue(B.elem_size == 4, L"elem_size error", LINE_INFO());
		Assert::IsTrue(B.full_size == 16, L"full_size error", LINE_INFO());

		//TEST	valid_size(size_t elem);
		//TEST	valid_size(size_t row, size_t col);
	}
	TEST_METHOD(T3_element_access)
	{
		Sudoku::Board<int, 3> board;
		Sudoku::Board<int, 3> board2;
		Sudoku::Board<int, 3> empty;

		// Writing
		try { set_atValue(board); }
		catch (const std::exception&) { Assert::Fail(L"writing with atValue(elem) failed", LINE_INFO()); }
		Assert::IsTrue(board.at(80) == 9, L"reading with atValue(elem) failed", LINE_INFO());
		try { if (board.at(100)) { Assert::Fail(L"at(elem) should give out-of-bounds", LINE_INFO()); } }
		catch (const std::exception&) { } // caught

		try { set_atValue_2(board2); }
		catch (const std::exception&) { Assert::Fail(L"writing with atValue(row, col) failed", LINE_INFO()); }
		Assert::IsTrue(board2.at(8, 8) == 9, L"reading with atValue(row, col) failed", LINE_INFO());
		try { if (board2.at(11, 4)) { Assert::Fail(L"at(row, col) should fail with out-of-bound", LINE_INFO()); } }
		catch (const std::exception&) { }

		Sudoku::Board<int, 3> board3;
		try { set_Value(board3); }
		catch (const std::exception&) { Assert::Fail(L"writing with operator[row][col] filed", LINE_INFO()); }
		Assert::IsTrue(board3[8][8] == 9, L"reading with operator[row][col] failed", LINE_INFO());

		Sudoku::Board<int, 3> board4;
		try { set_Value(board4); }
		catch (const std::exception&) { Assert::Fail(L"writing with operator[Location] failed", LINE_INFO()); }
		Assert::IsTrue(board4[Sudoku::Location<3>(80)] == 9, L"reading with operator[Location] failed", LINE_INFO());

		// operator==() / operator!=()
		// repeat from TEST_METHOD(properties)
		Assert::IsTrue(board == board2, L"failure operator==", LINE_INFO());
		Assert::IsFalse(board == empty, L"failure operator== (inverse)", LINE_INFO());
		Assert::IsTrue(board != empty, L"failure operator!=", LINE_INFO());
	}
	TEST_METHOD(T4_iterator_access)
	{
		using namespace Sudoku;

		Board<int, 2> B1{};
		Assert::IsTrue(*B1.begin() == 0, L"begin() failed", LINE_INFO());
		Assert::IsTrue(*B1.cbegin() == 0, L"cbegin() failed", LINE_INFO());
		Assert::IsTrue(*(B1.end() -1) == 0, L"cbegin() failed", LINE_INFO());
		Assert::IsTrue(*(B1.cend() -1) == 0, L"cbegin() failed", LINE_INFO());

		//TEST Board iterator / const_iterator access ...
		// begin / cbegin / const begin
	}
	TEST_METHOD(T5_clear)
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
private:
	template<typename T, size_t N>
	constexpr void compiler_checks(const Sudoku::Board<T, N>& B) const
	{
		// independend size calculations
		static_assert(N > 0 && N < 11, "Size out of bounds, for testing a 10'000 elements board should be enough.");
		static_assert(B.base_size == N, "base_size wrong");
		static_assert(B.elem_size == N*N, "elem_size wrong");
		static_assert(B.full_size == B.elem_size*B.elem_size, "full_size wrong");
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