/**	Unit tests for the template class Sudoku::Location
*
*/
#include "stdafx.h"
#include "CppUnitTest.h"

// Class under test
#include "Location.h"
// aditional


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Sudoku_Test
{
	TEST_CLASS(Class_Location)
	{
	public:
		TEST_METHOD(default_size)
		{
			Sudoku::Location<3> loc1(52);
			Assert::IsTrue(loc1.element() == 52, L".element() wrong", LINE_INFO());
			Assert::IsTrue(loc1.row() == 5, L".row() wrong", LINE_INFO());
			Assert::IsTrue(loc1.col() == 7, L".col() wrong", LINE_INFO());
			Assert::IsTrue(loc1.block() == 5, L".block() wrong", LINE_INFO());
			Sudoku::Location<3> loc2(3, 2);
			Assert::IsTrue(loc2.element() == 29, L".element() wrong", LINE_INFO());
			Assert::IsTrue(loc2.row() == 3, L".row() wrong", LINE_INFO());
			Assert::IsTrue(loc2.col() == 2, L".col() wrong", LINE_INFO());
			Assert::IsTrue(loc2.block() == 3, L".block() wrong", LINE_INFO());
			Sudoku::Location<3> loc3(loc1);
			Assert::IsTrue(loc3.element() == 52, L"Copy constructor", LINE_INFO());
			Sudoku::Location<3> loc4 = loc1;
			Assert::IsTrue(loc4.element() == 52, L"Copy Assignment", LINE_INFO());
			Sudoku::Location<3> loc5(Sudoku::Location<3>(2));
			Assert::IsTrue(loc5.element() == 2, L"Move constructor", LINE_INFO());
			Sudoku::Location<3> loc6 = Sudoku::Location<3>(18);
			Assert::IsTrue(loc6.element() == 18, L"Move assignment", LINE_INFO());
			std::swap(loc5, loc6);
			Assert::IsTrue(loc6.element() == 2, L"std::swap() failed", LINE_INFO());
		}
		//TEST_METHOD(different_sizes)
		//{
		//	
		//}
		TEST_METHOD(comparisson)
		{
			Assert::IsTrue(Sudoku::Location<3>(4) == Sudoku::Location<3>(4), L"", LINE_INFO());
			Assert::IsFalse(Sudoku::Location<3>(0) == Sudoku::Location<3>(13), L"", LINE_INFO());
			//
		}
		//TEST_METHOD(moving)
		//{
		//
		//}
	private:
	};
}