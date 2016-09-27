/**	Unit tests for the template class Sudoku::Location
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Location.h"
// aditional


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sudoku;

namespace Sudoku_Test
{
TEST_CLASS(Class_Location)
{
public:
	TEST_METHOD(constructors)
	{
		Location<3> L1(52);
		Assert::IsTrue(L1.element() == 52, L"element input", LINE_INFO());
		Location<3> L6(1, 8);
		Assert::IsTrue(L6.element() == 17, L"row-col input", LINE_INFO());
		Location<3> L2(L1);
		Assert::IsTrue(L2.element() == 52, L"Copy constructor", LINE_INFO());
		Location<3> L3 = L1;
		Assert::IsTrue(L3.element() == 52, L"Copy Assignment", LINE_INFO());
		Location<3> L4(Location<3>(2));
		Assert::IsTrue(L4.element() == 2, L"Move constructor", LINE_INFO());
		Location<3> L5 = Location<3>(18);
		Assert::IsTrue(L5.element() == 18, L"Move assignment", LINE_INFO());
	}
	TEST_METHOD(information_size_3)
	{
		Sudoku::Location<3> loc1(52);
		Assert::IsTrue(loc1.element() == 52, L"element()", LINE_INFO());
		Assert::IsTrue(loc1.row() == 5, L"row()", LINE_INFO());
		Assert::IsTrue(loc1.col() == 7, L"col()", LINE_INFO());
		Assert::IsTrue(loc1.block() == 5, L"block()", LINE_INFO());
		Assert::IsTrue(loc1.block_row() == 2, L"block_row()", LINE_INFO());
		Assert::IsTrue(loc1.block_col() == 1, L"block_col()", LINE_INFO());
		Assert::IsTrue(loc1.block_elem() == 7, L"block_elem()", LINE_INFO());

		Sudoku::Location<3> loc2(3, 2);
		Assert::IsTrue(loc2.element() == 29, L"element()", LINE_INFO());
		Assert::IsTrue(loc2.row() == 3, L"row()", LINE_INFO());
		Assert::IsTrue(loc2.col() == 2, L"col()", LINE_INFO());
		Assert::IsTrue(loc2.block() == 3, L"block()", LINE_INFO());
		Assert::IsTrue(loc2.block_row() == 0, L"block_row()", LINE_INFO());
		Assert::IsTrue(loc2.block_col() == 2, L"block_col()", LINE_INFO());
		Assert::IsTrue(loc2.block_elem() == 2, L"block_elem()", LINE_INFO());

		Sudoku::Location<3> loc3(0);
		Assert::IsTrue(loc3.element() == 0, L"element()", LINE_INFO());
		Assert::IsTrue(loc3.row() == 0, L"row()", LINE_INFO());
		Assert::IsTrue(loc3.col() == 0, L"col()", LINE_INFO());
		Assert::IsTrue(loc3.block() == 0, L"block()", LINE_INFO());
		Assert::IsTrue(loc3.block_row() == 0, L"block_row()", LINE_INFO());
		Assert::IsTrue(loc3.block_col() == 0, L"block_col()", LINE_INFO());
		Assert::IsTrue(loc3.block_elem() == 0, L"block_elem()", LINE_INFO());

		Sudoku::Location<3> loc4(80);
		Assert::IsTrue(loc4.element() == 80, L"element()", LINE_INFO());
		Assert::IsTrue(loc4.row() == 8, L"row()", LINE_INFO());
		Assert::IsTrue(loc4.col() == 8, L"col()", LINE_INFO());
		Assert::IsTrue(loc4.block() == 8, L"block()", LINE_INFO());
		Assert::IsTrue(loc4.block_row() == 2, L"block_row()", LINE_INFO());
		Assert::IsTrue(loc4.block_col() == 2, L"block_col()", LINE_INFO());
		Assert::IsTrue(loc4.block_elem() == 8, L"block_elem()", LINE_INFO());
	}
	//TODO other size: Location<2>
	TEST_METHOD(comparisson)
	{
		Assert::IsTrue(Location<3>(4) == Location<3>(4),
					   L"", LINE_INFO());
		Assert::IsFalse(Location<3>(0) == Location<3>(13),
						L"", LINE_INFO());
		Assert::IsTrue(Location<3>(0, 5) < Location<3>(3, 2),
					   L"", LINE_INFO());
		Assert::IsFalse(Location<3>(80) < Location<3>(8, 3),
						L"", LINE_INFO());
	}
	TEST_METHOD(helper_Block_Loc)
	{
		Block_Loc<3> B1(2, 6);
		Assert::IsTrue(B1.elem == 6, L"Block_Loc.elem", LINE_INFO());
		Assert::IsTrue(B1.id == 2, L"Block_loc.id", LINE_INFO());
		Location<3> L1(B1);
		Assert::IsTrue(L1.block() == B1.id);
		Assert::IsTrue(L1.block_elem() == B1.elem);

		Block_Loc<3> B2(2,2,0);
		Assert::IsTrue(B2.id == 2);
		Assert::IsTrue(B2.elem == 6);
		Location<3> L2(B2);
		Assert::IsTrue(L2.block() == B2.id);
		Assert::IsTrue(L2.block_elem() == B2.elem);
		Assert::IsTrue(L2.block_row() == 2);
		Assert::IsTrue(L2.block_col() == 0);
	}
};
}