/**	Unit tests for the template class Sudoku::Board
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Board.h"
// helpers
#include "uTestObjects.h"
// library
#include <set>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(Class_Board_Section_Iterator)
{
public:
	TEST_METHOD(category_all)
	{
		// Set-up
		Sudoku::Board<int, 3> B;
		Test_Boards::bN_fill_1::set_ElemNr(B);
		auto itr1 = B.row(0).begin();

		Sudoku::Board<std::set<int>, 3> S;
		auto itr_S1 = S.row(3).begin();

		// All categories
		//	b(a); b = a; ~a
		//	++a; a++
		Sudoku::Board<int,3>::Row::iterator itr2(itr1);
		Sudoku::Board<int,3>::Row::iterator itr3 = itr1;

		Sudoku::Board<std::set<int>>::Row::iterator itr_S2(itr_S1);
		
		// destructor?
		++itr1;
		itr2++;

		++itr_S2;
		itr_S2++;

		// Input
		//	a == b; a != b
		//	*a; a->m		rvalue dereference
		Assert::IsFalse(itr1 == B.row(0).begin(), L"operator== didn't fail");
		Assert::IsTrue(itr1 == itr2, L"operator== failed");
		Assert::IsTrue(itr1 != B.row(0).begin(), L"operator!= didn't fail");
		Assert::IsFalse(itr1 != itr2, L"operator!= failed");
		Assert::IsTrue(*itr3 == B[0][0], L"operator* failed");
		Assert::IsTrue(*itr1 == B[0][1], L"operator++() failed");
		Assert::IsTrue(*itr2 == B[0][1], L"operator++(int) failed");

		S[3][0].insert({ 9, 8, 6 });
		Assert::IsTrue(itr_S1->size() == 3, L"operator-> failed");

		// Output
		//	*a = t; *a++ = t
		*itr3 = 5;
		Assert::IsTrue(B[0][0] == 5, L"Row: Output: *a = t failed", LINE_INFO());
		*itr3++ = 8;
		Assert::IsFalse(B[0][1] == 8, L"Row: Output: *a++ =t moved", LINE_INFO());
		Assert::IsTrue(B[0][0] == 8, L"Row: Output: *a++ = t failed", LINE_INFO());

		itr_S2->insert({ 1, 2 });
		Assert::IsTrue(S[3][2].size() == 2, L"operator-> failed");

		// Forward
		//	X a; X()		default-constructable
		// Multi-pass: neither dereferencing nor incrementing affects dereferenceability
		
		//TODO end()

		// Bidirectional
		//	--a; a--; *a--
		

		// Random Access
		//	a + n


		// move out of first row
		auto itr4 = B.row(5).begin();
		Assert::IsTrue(*itr4 == B.row(5)[0], L"row movement error", LINE_INFO());
		auto itc1 = B.col(0).begin();
		Assert::IsTrue(*itc1 == B[0][0], L"Col: initialisation error", LINE_INFO());
		++itc1;
		Assert::IsTrue(*itc1 == B[1][0], L"Col: operator++() failed");



		Sudoku::Board<int,3>::Row XX = B.row(0);
		//auto s = XX.begin().addressof(0);

		auto deref = B.row(0).begin();
		auto dere = B.col(6).begin();


		//size_t b = S.row(0).begin()->size();
		//Assert::IsTrue(S.row(0).begin()->size() == 0, L"operator-> failed", LINE_INFO());
		//int* c = std::addressof(B[0][0]);
		//int* d = B.row(0).addressof(0);

	}
	TEST_METHOD(range_for_loop)
	{
		Sudoku::Board<int, 3> B;
		Test_Boards::bN_fill_1::set_ColNr(B);	// all row 0 1 2 3 4 5 6 7 8 sum = 36
		size_t total{};
		for (size_t i = 0; i < 9; ++i)
		{
			total = 0;
			try
			{
				for (auto& s : B.row(i)) { total += s; }
			}
			catch (const std::exception&)
			{
				Assert::Fail(L"Section::Row range-for", LINE_INFO());
			}
			Assert::IsTrue(total == 36, L"Section::Row range-for incorrect result", LINE_INFO());
		}
		
		Test_Boards::bN_fill_1::set_RowNr(B);
		for (size_t i = 0; i < 9; ++i)
		{
			total = 0;
			try
			{
				for (auto& s : B.col(i)) { total += s; }
			}
			catch (const std::exception&)
			{
				Assert::Fail(L"Section::Col range-for", LINE_INFO());
			}
			Assert::IsTrue(total == 36, L"Section::Col range-for incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_BlockNr_X(B);
		for (size_t i = 0; i < 9; ++i)
		{
			total = 0;
			try
			{
				for (auto& s: B.block(i)) { total += s; }
			}
			catch (const std::exception&)
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

		for (size_t i = 0; i < 9; ++i)
		{
			size_t total = std::accumulate(B.row(i).begin(), B.row(i).end(), 0);
			Assert::IsTrue(total == 36, L"Section::Row std::accumulate incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_RowNr(B);
		for (size_t i = 0; i < 9; ++i)
		{
			size_t total = std::accumulate(B.col(i).begin(), B.col(i).end(), 0);
			Assert::IsTrue(total == 36, L"Section::Col std::accumulate incorrect result", LINE_INFO());
		}

		Test_Boards::bN_fill_1::set_BlockNr_X(B);
		for (size_t i = 0; i < 9; ++i)
		{
			size_t total = std::accumulate(B.block(i).begin(), B.block(i).end(), 0);
			Assert::IsTrue(total == 36, L"Section::Block std::accumulate incorrect result", LINE_INFO());
		}
	}
};

}	// namespace Sudoku_Test