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
		static_assert(std::is_class<Board<int, 3>>(), "should be a class, hiding datarepresentation");
		static_assert(!std::is_empty<Board<int, 3>>(), "memberdata missing");
		static_assert(! std::is_standard_layout<Board<int, 3>>(), "should have standard layout");
		//static_assert(std::has_unique_object_representations<Board<int, 3>>(), "");	//C++17
		static_assert(! std::is_trivially_copyable<Board<int, 3>>(), "should be trivially copyable");
		static_assert(! std::is_trivially_constructible<Board<int, 3>>(), "default constructor");
		static_assert(std::is_default_constructible<Board<int, 3>>(), "default constructor");
		static_assert(! std::is_trivially_default_constructible<Board<int, 3>>(), "not using the default constructor");
		static_assert(! std::is_nothrow_default_constructible<Board<int, 3>>(), "notrow default constructor");
		static_assert(std::is_copy_constructible<Board<int, 3>>(), "copy constructor");
		static_assert(! std::is_trivially_copy_constructible<Board<int, 3>>(), "trivially copy constructor");
		static_assert(! std::is_nothrow_copy_constructible<Board<int, 3>>(), "notrow copy constructor");
		static_assert(std::is_move_constructible<Board<int, 3>>(), "move constructor");
		static_assert(! std::is_trivially_move_constructible<Board<int, 3>>(), "trivially move constructor");
		static_assert(std::is_nothrow_move_constructible<Board<int, 3>>(), "nothrow move constructor");
		static_assert(std::is_copy_assignable<Board<int, 3>>(), "copy assignable");
		static_assert(! std::is_trivially_copy_assignable<Board<int, 3>>(), "trivially copy assignable");
		static_assert(! std::is_nothrow_copy_assignable<Board<int, 3>>(), "notrow copy assignable");
		static_assert(std::is_move_assignable<Board<int, 3>>(), "move assignable");
		static_assert(! std::is_trivially_move_assignable<Board<int, 3>>(), "trivially move assignable");
		static_assert(std::is_nothrow_move_assignable<Board<int, 3>>(), "move assignable");
		static_assert(std::is_destructible<Board<int, 3>>(), "destructable");
		static_assert(! std::is_trivially_destructible<Board<int, 3>>(), "trivially destructable");
		static_assert(std::is_nothrow_destructible<Board<int, 3>>(), "nothrow destructable");
		static_assert(std::is_swappable<Board<int, 3>>(), "swappable");			//C++17
		static_assert(std::is_nothrow_swappable<Board<int, 3>>(), "swappable");	//C++17 
		static_assert(! std::is_swappable_with<Board<int, 3>, std::vector<int>>(), "");	//C++17
		static_assert(! std::is_nothrow_swappable_with<Board<int, 3>, std::vector<int>>(), "");	//C++17
		//is_constructible from different types
		static_assert(std::is_constructible<Board<int, 3>, int>(), "should construct from int");
		static_assert(std::is_constructible<Board<int, 3>, unsigned int>(), "");
		static_assert(std::is_constructible<Board<int, 3>, size_t>(), "");
		static_assert(std::is_constructible<Board<Options<10>, 3>, Options<10>>(), "should construct from Options");
		static_assert(! std::is_assignable<Board<int, 3>, int>(), "");
		static_assert(std::is_assignable<Board<int, 3>, Board<int, 3>>(), "");

		// different sizes
		static const Board<int, 2> f2;
		static const Board<int, 3> f3;
		compiler_checks(f2);
		compiler_checks(f3);
		compiler_checks(Board<int,10>());
		static_assert(f2.size() == 16, "size() value error[f2]");
		static_assert(f3.size() == 81, "size() value error[f3]");
		// different types
		compiler_checks(Board<int, 3>());
		compiler_checks(Board<std::set<int>, 2>());
		compiler_checks(Board<std::bitset<9>,3>());
		compiler_checks(Board<Options<9>,3>());

		// default element value : Board(const value_type&)
		compiler_checks(Board<int, 3>(2));
		//static const std::set<int> s4{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		//static const Board<std::set<int>, 3> f4(s4);	// 4 ms (2 ms using static const)
		//compiler_checks(f4);
		//static_assert(f4.size() == 81, "size() value error[f4]");

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
		//Assert::IsTrue(f4.at(43) == s4, L"Options board value set doesn't match input", LINE_INFO());
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
		//Assert::IsTrue(b_set0.empty(), L"failure empty() on set", LINE_INFO());

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

		//TEST	valid_size(int elem);
		//TEST	valid_size(int row, int col);
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
		//try { if (board3[11][4]) { Assert::Fail(L"at[row][col] should fail with out-of-bound", LINE_INFO()); } }
		//catch (const std::exception&) { }
		//catch (...) { Assert::Fail(L"unknown error", LINE_INFO()); }

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
	template<typename T, int N>
	constexpr void compiler_checks(const Sudoku::Board<T, N>& B) const
	{
		// independend size calculations
		static_assert(N > 0 && N < 11, "Size out of bounds, for testing a 10'000 elements board should be enough.");
		static_assert(B.base_size == N, "base_size wrong");
		static_assert(B.elem_size == N*N, "elem_size wrong");
		static_assert(B.full_size == B.elem_size*B.elem_size, "full_size wrong");
		static_assert(B.full_size == B.size(), "Function Error size()");
	}

	template<int N>
	void set_atValue(Sudoku::Board<int, N>& target)
	{
		constexpr int elem{ N*N };
		for (int i = 0; i < target.size(); ++i)
		{
			target.at(i) = i % elem + 1;
		}
	}

	template<int N>
	void set_Value(Sudoku::Board<int, N>& target)
	{
		for (int row = 0; row < target.col(0).size(); ++row)
		{
			for (int col = 0; col < target.row(0).size(); ++col)
			{
				target[row][col] = col + 1;
			}
		}
	}

	template<int N>
	void set_atValue_2(Sudoku::Board<int, N>& target)
	{
		for (int row = 0; row < target.col(0).size(); ++row)
		{
			for (int col = 0; col < target.row(0).size(); ++col)
			{
				target.at(row, col) = col + 1;
			}
		}
	}

	template<int N>
	void set_Value_2(Sudoku::Board<int, N>& target)
	{
		constexpr int elem{ N*N };
		for (int i = 0; i < target.size(); ++i)
		{
			target[Location(i)] = i % elem + 1;
		}
	}
};

} // namespace Sudoku_Test