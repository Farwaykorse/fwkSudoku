/**	Unit tests for the template class Sudoku::Board
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "../Sudoku/Board.h"
// aditional
#include "../Sudoku/Location.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
TEST_CLASS(Class_Board_Section)
{
public:
	TEST_METHOD(properties)
	{
		Sudoku::Board<int, 3> A;
		using Location = Sudoku::Location<3>;
		Assert::IsTrue(A.row(0).id() == 0 &&
					   A.row(3).id() == 3 &&
					   A.row(8).id() == 8, L"Row::id()");
		Assert::IsTrue(A.col(0).id() == 0 &&
					   A.col(3).id() == 3 &&
					   A.col(8).id() == 8, L"Col::id()");
		Assert::IsTrue(A.block(0).id() == 0 &&
					   A.block(3).id() == 3 &&
					   A.block(8).id() == 8, L"Block::id()");
		Assert::IsTrue(A.row(0).size() == 9 &&
					   A.col(7).size() == 9 &&
					   A.block(8).size() == 9, L"Section::size()");
		Sudoku::Board<int, 2> B;
		Assert::IsTrue(B.row(1).id() == 1 &&
					   B.col(3).id() == 3 &&
					   B.block(3).id() == 3, L"Section::id()");
		Assert::IsTrue(B.col(2).size() == 4, L"Section.size() 2");

		Assert::IsTrue(A.row(Location(0)).id() == 0, L"Location instantiation");
		Assert::IsTrue(A.row(Location(9)).id() == 1, L"Location instantiation 2");
		Assert::IsTrue(A.row(Location(76)).id() == 8, L"Location instantiation row 3");
		Assert::IsTrue(A.col(Location(5)).id() == 5, L"Location instantiation col");
		Assert::IsTrue(A.block(Location(29)).id() == 3, L"Location instantiation block");
	}
	TEST_METHOD(section_element_access)
	{
		Sudoku::Board<int, 3> B;

		Assert::IsTrue(B.row(0).size() == 9, L"...");
		Assert::IsTrue(B.row(0).at(0) == 0, L"");
		try { B.row(5).at(3) = 8; }
		catch (...)
		{
			Assert::Fail(L"Write to row with at() failed");
		}
		try { B.row(8)[8] = 2; }
		catch (...)
		{
			Assert::Fail(L"Write to row section with [] failed");
		}
		Assert::IsTrue(B[5][3] == 8, L"Write using section failed 1");
		Assert::IsTrue(B[8][8] == 2, L"Write using section failed 2");
		Assert::IsTrue(B.row(8).at(8) == 2, L"Reading from row section with at() failed");
		Assert::IsTrue(B.row(5)[3] == 8, L"Reading from row section with [] failed");

		Assert::IsTrue(B.col(3)[5] == 8, L"Col section access failed");
		Assert::IsTrue(B.block(8)[8] == 2, L"Block section access failed");

		auto block_unit = B.block(4);
		try
		{
			for (int i = 0; i < 9; ++i)
			{
				block_unit[i] = 9 - i;
			}
			for (int i = 1; i < 4; ++i)
			{
				Assert::IsTrue(B.col(i + 2)[5] == 4 - i, L"loop value different");
			}
		}
		catch (...)
		{
			Assert::Fail(L"Loop over block failed");
		}
	}
private:

};
} // namespace Sudoku_Test