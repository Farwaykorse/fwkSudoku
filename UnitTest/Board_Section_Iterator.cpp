/**	Unit tests for the template class Sudoku::Board
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Board.h"
// helpers
#include "TestObjects.h"
#include "Location.h"
// library
#include <set>
#include <algorithm>
#include <iterator>	// distance()
#include <numeric>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(Class_Board_Section_Iterator)
{
public:
	TEST_METHOD(category_all)
	{
		using Board = Sudoku::Board<int,3>;
		using Row = Board::Row;
		using Col = Board::Col;
		using Block = Board::Block;

		/* Set-up */
		Board B;
		// Board B : 9 x 9 sudoku board
		Test_Boards::bN_fill_1::set_ColNr(B);
		// Board B : all element contain their collumn id ranging: 0 to 8

		Sudoku::Board<std::set<int>, 3> S;

		/* constructors */
		// compile errors only
		Row::iterator itr1 = B.row(0).begin();	// 0
		Row::iterator itr_end = B.row(0).end();	// nullptr
		Sudoku::Board<std::set<int>>::Row::iterator itr_S1 = S.row(3).begin();

		/* All iterator categories */
		//	b(a); b = a; ~a
		//	++a; a++
		Row::iterator itr2(itr1);	// 0
		Row::iterator itr3 = itr1;	// 0
		Sudoku::Board<std::set<int>>::Row::iterator itr_S2(itr_S1);
		// destructor?
		++itr2;		// 1
		itr3++;		// 1
		++itr_S2;
		itr_S2++;

		// Input
		//	a == b; a != b
		//	*a; a->m		rvalue dereference
		Assert::IsTrue(itr1 == B.row(0).begin(), L"operator== failed", LINE_INFO());
		Assert::IsFalse(itr1 == B.row(0).end(), L"operator== didn't fail", LINE_INFO());
		Assert::IsFalse(itr1 != B.row(0).begin(), L"operator!= failed", LINE_INFO());
		Assert::IsTrue(itr1 != B.row(0).end(), L"operator!= didn't fail", LINE_INFO());

		//Assert::IsFalse(B.row(0).begin() == B.col(0).begin(), L"should fail");

		Assert::IsTrue(*itr1 == B[0][0], L"operator* failed", LINE_INFO());
		S[3][0].insert({ 9, 8, 6 });
		Assert::IsTrue(itr_S1->size() == 3, L"operator-> failed", LINE_INFO());
		// check operations from under All categories:
		Assert::IsTrue(*itr2 == B[0][1], L"operator++() failed", LINE_INFO());
		Assert::IsTrue(*itr3 == B[0][1], L"operator++(int) failed", LINE_INFO());

		auto itr5 = B.row(0).begin();
		for (size_t i = 0; i<B.elem_size-1; ++i) { ++itr5; }
		Assert::IsTrue(*itr5 == 8, L"moving forward to last element failed", LINE_INFO());
		Assert::IsTrue(++itr5 == B.row(0).end(), L"moving forward to end() failed", LINE_INFO());

		// Output
		//	*a = t; *a++ = t
		*itr1 = 5;	// change from 0 to 5
		Assert::IsTrue(B[0][0] == 5, L"Row: Output: *a = t failed", LINE_INFO());
		*itr1++ = 8;	// change from 5 to 8 move to element 1
		Assert::IsFalse(B[0][1] == 8, L"Row: Output: *a++ =t moved before setting the value", LINE_INFO());
		Assert::IsTrue(B[0][0] == 8, L"Row: Output: *a++ = t failed", LINE_INFO());
		Assert::IsTrue(*itr1 == B[0][1], L"Row: Output: *a++ = t didn't move after setting the value", LINE_INFO());
		*++itr1 = 7;	// move to element 2, change value from 2 to 7
		Assert::IsTrue(*itr1 == B[0][2], L"*++a failed to move", LINE_INFO());
		Assert::IsFalse(B[0][1] == 7, L"*++a set value before moving", LINE_INFO());
		Assert::IsTrue(*itr1 == 7, L"*++a failed to set value", LINE_INFO());

		itr_S2->insert({ 1, 2 });
		Assert::IsTrue(S[3][2].size() == 2, L"operator-> failed", LINE_INFO());

		// Forward
		//	X a; X()		default-constructable
		//NEEDTEST is forward iterator
		static_assert(std::is_default_constructible<Row::iterator>(), "");
		static_assert(std::is_default_constructible<Col::iterator>(), "");
		static_assert(std::is_default_constructible<Block::iterator>(), "");
		Assert::IsTrue(
			std::is_default_constructible<Sudoku::Board<int,2>::Row::iterator>(),
			L"An forward iterator requires to be default constructable", LINE_INFO());
		// Multi-pass: neither dereferencing nor incrementing affects dereferenceability

		// Bidirectional
		//	--a; a--; *a--
		itr1 = B.row(1).end();
		itr2 = itr1;
		itr3 = itr1;
		Assert::IsTrue(itr1 == B.row(1).end(), L"Setup-error", LINE_INFO());
		Assert::IsFalse(--itr2 == itr1, L"--itr failed", LINE_INFO());
		Assert::IsTrue(*itr2 == B[1][8], L"--itr failed", LINE_INFO());
		Assert::IsTrue(itr3-- == itr1, L"itr-- failed to dereference before moving", LINE_INFO());
		Assert::IsTrue(itr3 == itr2, L"itr-- failed to move at all", LINE_INFO());
		Assert::IsTrue(*--itr2 == 7, L"*--itr failed", LINE_INFO());
		Assert::IsTrue(*itr3-- == 8, L"*itr-- failed in start position", LINE_INFO());
		Assert::IsTrue(*itr3 == 7, L"*itr-- failed for new position", LINE_INFO());

		// Random Access
		//	a + n; a+=n, a[n], a - n; a-=n; a[-n]
		//	a - b; a < b; a > b; a <= b; a >= b
		//	n + a
		itr1 = B.row(2).begin();
		itr2 = itr1;
		itr3 = itr1;
		Assert::IsTrue(itr2+1 == ++itr3, L"a+n failed", LINE_INFO());
		// itr3 = 1
		Assert::IsTrue(itr2 == itr1, L"a+1 shouldn't change a!", LINE_INFO());
		Assert::IsTrue((itr2+=3) == itr1+3, L"a+=n failed", LINE_INFO());
		// itr2 = 3
		Assert::IsTrue(itr2 == itr1+3, L"a+=n changes a", LINE_INFO());
		Assert::IsTrue(itr3[2] == *(itr1+3), L"a[n] failed", LINE_INFO());
		Assert::IsTrue(itr3 == itr1+1, L"a[n] shouldn't change a", LINE_INFO());
		Assert::IsTrue(itr2-3 == itr1, L"a-n failed", LINE_INFO());
		Assert::IsTrue(itr2 == itr1+3, L"a-n shouldn't change a", LINE_INFO());
		Assert::IsTrue((itr2-=3) == itr1, L"a-=n failed", LINE_INFO());
		// itr2 = 0
		Assert::IsTrue(itr2 == itr1, L"a-=n changes a", LINE_INFO());
		Assert::IsTrue(itr3[-1] == *(itr1), L"a[-n] failed", LINE_INFO());
		Assert::IsTrue(itr1+B.elem_size == B.row(2).end(), L"movement to end failed", LINE_INFO());

		itr1 = B.row(2).begin();	// 0
		itr2 = B.row(2).begin()+2;	// 2
		itr3 = B.row(2).end();		// 9
		Assert::IsTrue(itr1 - itr2 == -2, L"distance failed", LINE_INFO());
		//TODO int + iterator, returns same result as itr + int
		//Assert::IsTrue(5 + itr2 == itr2+5, L"sum with number failed", LINE_INFO());
		Assert::IsTrue(itr1 < itr2, L"a < b failed", LINE_INFO());
		Assert::IsTrue(itr2 > itr1, L"a > b failed", LINE_INFO());
		Assert::IsTrue(itr1 <= itr2 && itr1 <= B.row(2).begin(), L"a <= b failed", LINE_INFO());
		Assert::IsTrue(itr2 >= itr1 && itr1 >= B.row(2).begin(), L"a >= b failed", LINE_INFO());

		// Custom functions
		Assert::IsTrue(itr1.location() == Sudoku::Location<3>(2,0), L"location() failed", LINE_INFO());
		Assert::IsTrue(itr1.location().row() == 2, L"location().row() failed", LINE_INFO());
		Assert::IsTrue(itr3.location().element() == 27, L"location().elemen() of end() failed", LINE_INFO());

		// Other
		Assert::IsTrue(
			std::distance(B.row(0).begin(), B.row(0).end()) == B.elem_size,
			L"std::distance failed", LINE_INFO());
	}
	TEST_METHOD(const_iterator)
	{
		using Board = Sudoku::Board<int,3>;
		using Row = Board::Row;
		using Col = Board::Col;
		using Block = Board::Block;

		/* Set-up */
		Board B;
		// Board B : 9 x 9 sudoku board
		Test_Boards::bN_fill_1::set_ColNr(B);
		// Board B : all element contain their collumn id ranging: 0 to 8

		Sudoku::Board<std::set<int>, 3> S;

		/* constructors */
		// compile errors only
		Row::const_iterator itr1 = B.row(0).cbegin();	// 0
		Row::const_iterator itr_end = B.row(0).cend();	// nullptr
		Sudoku::Board<std::set<int>>::Row::const_iterator itr_S1 = S.row(3).cbegin();

		/* All iterator categories */
		//	b(a); b = a; ~a
		//	++a; a++
		Row::const_iterator itr2(itr1);	// 0
		Row::const_iterator itr3 = itr1;	// 0
		Sudoku::Board<std::set<int>>::Row::const_iterator itr_S2(itr_S1);
		// destructor?
		++itr2;		// 1
		itr3++;		// 1
		++itr_S2;
		itr_S2++;

		// Input
		//	a == b; a != b
		//	*a; a->m		rvalue dereference
		Assert::IsTrue(itr1 == B.row(0).cbegin(), L"operator== failed", LINE_INFO());
		Assert::IsFalse(itr1 == B.row(0).cend(), L"operator== didn't fail", LINE_INFO());
		Assert::IsFalse(itr1 != B.row(0).cbegin(), L"operator!= failed", LINE_INFO());
		Assert::IsTrue(itr1 != B.row(0).cend(), L"operator!= didn't fail", LINE_INFO());
		Assert::IsTrue(*itr1 == B[0][0], L"operator* failed", LINE_INFO());
		S[3][0].insert({ 9, 8, 6 });
		Assert::IsTrue(itr_S1->size() == 3, L"operator-> failed", LINE_INFO());
		// check operations from under All categories:
		Assert::IsTrue(*itr2 == B[0][1], L"operator++() failed", LINE_INFO());
		Assert::IsTrue(*itr3 == B[0][1], L"operator++(int) failed", LINE_INFO());

		auto itr5 = B.row(0).cbegin();
		for (size_t i = 0; i<B.elem_size - 1; ++i) { ++itr5; }
		Assert::IsTrue(*itr5 == 8, L"moving forward to last element failed", LINE_INFO());
		Assert::IsTrue(++itr5 == B.row(0).cend(), L"moving forward to end() failed", LINE_INFO());

		// Output
		//	*a = t; *a++ = t
		// not for const_iterator

		// Forward
		//	X a; X()		default-constructable
		static_assert(std::is_default_constructible<Row::const_iterator>(), "");
		static_assert(std::is_default_constructible<Col::const_iterator>(), "");
		static_assert(std::is_default_constructible<Block::const_iterator>(), "");
		Assert::IsTrue(
			std::is_default_constructible<Sudoku::Board<int, 2>::Row::const_iterator>(),
			L"An forward iterator requires to be default constructable", LINE_INFO());
		// Multi-pass: neither dereferencing nor incrementing affects dereferenceability

		// Bidirectional
		//	--a; a--; *a--
		itr1 = B.row(1).cend();
		itr2 = itr1;
		itr3 = itr1;
		Assert::IsTrue(itr1 == B.row(1).cend(), L"Setup-error", LINE_INFO());
		Assert::IsFalse(--itr2 == itr1, L"--itr failed", LINE_INFO());
		Assert::IsTrue(*itr2 == B[1][8], L"--itr failed", LINE_INFO());
		Assert::IsTrue(itr3-- == itr1, L"itr-- failed to dereference before moving", LINE_INFO());
		Assert::IsTrue(itr3 == itr2, L"itr-- failed to move at all", LINE_INFO());
		Assert::IsTrue(*--itr2 == 7, L"*--itr failed", LINE_INFO());
		Assert::IsTrue(*itr3-- == 8, L"*itr-- failed in start position", LINE_INFO());
		Assert::IsTrue(*itr3 == 7, L"*itr-- failed for new position", LINE_INFO());

		// Random Access
		//	a + n; a+=n, a[n], a - n; a-=n; a[-n]
		//	a - b; a < b; a > b; a <= b; a >= b
		//	n + a
		itr1 = B.row(2).cbegin();
		itr2 = itr1;
		itr3 = itr1;
		Assert::IsTrue(itr2 + 1 == ++itr3, L"a+n failed", LINE_INFO());
		// itr3 = 1
		Assert::IsTrue(itr2 == itr1, L"a+1 shouldn't change a!", LINE_INFO());
		Assert::IsTrue((itr2 += 3) == itr1 + 3, L"a+=n failed", LINE_INFO());
		// itr2 = 3
		Assert::IsTrue(itr2 == itr1 + 3, L"a+=n changes a", LINE_INFO());
		Assert::IsTrue(itr3[2] == *(itr1 + 3), L"a[n] failed", LINE_INFO());
		Assert::IsTrue(itr3 == itr1 + 1, L"a[n] shouldn't change a", LINE_INFO());
		Assert::IsTrue(itr2 - 3 == itr1, L"a-n failed", LINE_INFO());
		Assert::IsTrue(itr2 == itr1 + 3, L"a-n shouldn't change a", LINE_INFO());
		Assert::IsTrue((itr2 -= 3) == itr1, L"a-=n failed", LINE_INFO());
		// itr2 = 0
		Assert::IsTrue(itr2 == itr1, L"a-=n changes a", LINE_INFO());
		Assert::IsTrue(itr3[-1] == *(itr1), L"a[-n] failed", LINE_INFO());
		Assert::IsTrue(itr1 + B.elem_size == B.row(2).cend(), L"movement to end failed", LINE_INFO());

		itr1 = B.row(2).cbegin();	// 0
		itr2 = B.row(2).cbegin() + 2;	// 2
		itr3 = B.row(2).cend();		// 9
		Assert::IsTrue(itr1 - itr2 == -2, L"distance failed", LINE_INFO());
		//TODO int + iterator, returns same result as itr + int
		// https://msdn.microsoft.com/en-us/library/d56eb4y4.aspx
		Assert::IsTrue(itr1 < itr2, L"a < b failed", LINE_INFO());
		Assert::IsTrue(itr2 > itr1, L"a > b failed", LINE_INFO());
		Assert::IsTrue(itr1 <= itr2 && itr1 <= B.row(2).cbegin(), L"a <= b failed", LINE_INFO());
		Assert::IsTrue(itr2 >= itr1 && itr1 >= B.row(2).cbegin(), L"a >= b failed", LINE_INFO());


		/* for-loop */
		int sum{};
		for (auto itr = B.row(0).cbegin(); itr != B.row(0).cend(); ++itr)
		{
			sum += *itr;
		}
		Assert::IsTrue(sum == 36, L"const_iterator failed in for-loop", LINE_INFO());

		// Custom functions
		Assert::IsTrue(itr1.location() == Sudoku::Location<3>(2, 0), L"location() failed", LINE_INFO());
		Assert::IsTrue(itr1.location().row() == 2, L"location().row() failed", LINE_INFO());
		Assert::IsTrue(itr3.location().element() == 27, L"location().elemen() of end() failed", LINE_INFO());

		// Other
		Assert::IsTrue(
			std::distance(B.row(0).cbegin(), B.row(0).cend()) == B.elem_size,
			L"std::distance failed", LINE_INFO());
	}
	TEST_METHOD(range_for_loop)
	{
		Sudoku::Board<int, 3> B;
		Test_Boards::bN_fill_1::set_ColNr(B);	// all row 0 1 2 3 4 5 6 7 8 sum = 36
		size_t total{};
		for (int i = 0; i < 9; ++i)
		{
			total = 0;
			try
			{
				for (auto& s : B.row(i)) { total += s; }
			}
			catch (...)
			{
				Assert::Fail(L"Section::Row range-for", LINE_INFO());
			}
			Assert::IsTrue(total == 36, L"Section::Row range-for incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_RowNr(B);
		for (int i = 0; i < 9; ++i)
		{
			total = 0;
			try
			{
				for (auto& s : B.col(i)) { total += s; }
			}
			catch (...)
			{
				Assert::Fail(L"Section::Col range-for", LINE_INFO());
			}
			Assert::IsTrue(total == 36, L"Section::Col range-for incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_BlockNr_X(B);
		for (int i = 0; i < 9; ++i)
		{
			total = 0;
			try
			{
				for (auto& s: B.block(i)) { total += s; }
			}
			catch (...)
			{
				Assert::Fail(L"Section::Block range-for", LINE_INFO());
			}
			Assert::IsTrue(total == 36, L"Section::Block range-for incorrect result", LINE_INFO());
		}
	}
	TEST_METHOD(manual_loop)
	{
		Sudoku::Board<int, 3> B;
		Test_Boards::bN_fill_1::set_ColNr(B);

		size_t total{};
		auto section = B.row(5);
		for (auto itr = section.begin(); itr != section.end(); ++itr)
		{
			total += *itr;
		}
	}
	TEST_METHOD(stl_algorithms)
	{
		Sudoku::Board<int, 3> B;
		Test_Boards::bN_fill_1::set_ColNr(B);

		//auto itr = B.row(0).begin();
		//auto end = B.row(0).end();
		//size_t total = std::accumulate(itr, end, 0);

		// accumulate requires an input iterator
		for (int i = 0; i < 9; ++i)
		{
			size_t total = std::accumulate(B.row(i).cbegin(), B.row(i).cend(), 0);
			Assert::IsTrue(total == 36, L"Section::Row std::accumulate incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_RowNr(B);
		for (int i = 0; i < 9; ++i)
		{
			size_t total = std::accumulate(B.col(i).begin(), B.col(i).end(), 0);
			Assert::IsTrue(total == 36, L"Section::Col std::accumulate incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_BlockNr_X(B);
		for (int i = 0; i < 9; ++i)
		{
			size_t total = std::accumulate(B.block(i).begin(), B.block(i).end(), 0);
			Assert::IsTrue(total == 36, L"Section::Block std::accumulate incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_ColNr(B);
		// upper_bound requires a forward iterator
		auto first_higher = std::upper_bound(B.row(5).cbegin(), B.row(5).cend(), 6);
		Assert::IsTrue(*first_higher == 7, L"std::upper_bound() failed", LINE_INFO());
		// fill requires a forward iterator
		std::fill(B.block(2).begin(), B.block(2).end(), 3);
		Assert::IsTrue(
			B.at(7) == 3 && B.at(8) ==3 && B.at(17) == 3 && B.at(25) == 3,
			L"std::fill() failed", LINE_INFO()
		);
		// next_permutation(start, stop) requires a Bidirectional iterator
		bool success = std::next_permutation(B.row(2).begin(), B.row(2).end());

		// shuffle(start, stop) requires a random iterator
		std::random_device rd;		// non-deterministic generator
		std::mt19937 urng(rd());	// to seed mersenne twister
		std::shuffle(B.row(8).begin(), B.row(8).end(), urng);
	}
};
}	// namespace Sudoku_Test