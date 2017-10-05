//===--	SudokuTests/Board_Sections.cpp									--===//
//
//	Unit tests for the template class Sudoku::Board
//===---------------------------------------------------------------------===//
//	Implemented with GoogleTest
//
//	Notes:
//	gTest is limited for use with multiple template parameters.
//	These expressions need to be implemented between extra parentheses
//	- test elements implementing this are flagged with [gTest]
//	- not implemented tests are flagged as NEEDTEST [gTest]
//	gTest tries to print iterators if they use inheritance,
//		if used in *_EQ/NE etc.
//		use an explicit test like EXPECT_TRUE(.. == ..).
//
//===---------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include "../Sudoku/Board.h"
// helpers
#include "../Sudoku/Location.h"
#include "../Sudoku/Options.h"

// library
#include <bitset>
#include <initializer_list>
#include <numeric>		// accumulate
#include <random>		// randomaccess tests
#include <set>
#include <type_traits>

using namespace Sudoku;

namespace SudokuTests::Board_SectionsTest
{
namespace compiletime
{
	// Type properties
	using Section = Board<int, 3>::Section;
	using const_Row = Board<int, 3>::const_Row;
	using const_Col = Board<int, 3>::const_Col;
	using const_Block = Board<int, 3>::const_Block;
	using Row = Board<int, 3>::Row;
	using Col = Board<int, 3>::Col;
	using Block = Board<int, 3>::Block;

	using typeT = const_Row;

	static_assert(std::is_base_of_v<Section, typeT>);
	static_assert(std::is_base_of_v<Section, const_Row>);
	static_assert(std::is_base_of_v<Section, const_Col>);
	static_assert(std::is_base_of_v<Section, const_Block>);
	static_assert(std::is_base_of_v<Section, Row>);
	static_assert(std::is_base_of_v<Section, Col>);
	static_assert(std::is_base_of_v<Section, Block>);

	static_assert(std::is_class_v<typeT>, "-- a class");
	static_assert(!std::is_trivial_v<typeT>, "trivial default constructors");
	static_assert(!std::is_trivially_copyable_v<typeT>,
		"-- compatible with std::memcpy & binary copy from/to files");
	static_assert(std::is_standard_layout_v<typeT>, "standard layout");
	static_assert(!std::is_pod_v<typeT>,
		"++ Plain Old Data, both trivial and standard-layout, C compatible");
	//static_assert(std::has_unique_object_representations_v<typeT>, "");	//C++17	trivially_copyable same object representation
	static_assert(!std::is_empty_v<typeT>, "-- class with no datamembers");
	static_assert(!std::is_polymorphic_v<typeT>,
		"-- inherits atleast one virtual function");
	static_assert(!std::is_abstract_v<typeT>,
		"-- inherits or declares at least one pure virtual function");
	static_assert(!std::is_final_v<typeT>, "-- cannot be used as base class");

	static_assert(std::is_trivial_v<Section>, "trivial Section");
	static_assert(std::is_trivially_copyable_v<Section>, "trivial copy Section");
	static_assert(std::is_empty_v<Section>, "empty Section");
	static_assert(std::is_pod_v<Section>, "POD Section");
	static_assert(!std::is_standard_layout_v<Row>, "standard layout");

	// default constructor: typeT()
	static_assert(!std::is_default_constructible_v<typeT>, "class()");
	static_assert(!std::is_nothrow_default_constructible_v<typeT>, "class() notrow");
	static_assert(!std::is_trivially_default_constructible_v<typeT>, "++ default, nothing virtual");

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>, "-- copy constructor");
	static_assert(std::is_nothrow_copy_constructible_v<typeT>, "-- notrow copy constructor");
	static_assert(std::is_trivially_copy_constructible_v<typeT>, "-- trivially copy constructor"); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>, "-- move constructor");
	static_assert(std::is_nothrow_move_constructible_v<typeT>, "-- nothrow move constructor");
	static_assert(std::is_trivially_move_constructible_v<typeT>, "-- trivially move constructor");

	// copy assingment
	static_assert(!std::is_copy_assignable_v<typeT>, "-- copy assignable");
	static_assert(!std::is_nothrow_copy_assignable_v<typeT>, "-- notrow copy assignable");
	static_assert(!std::is_trivially_copy_assignable_v<typeT>, "-- trivially copy assignable");

	static_assert(!std::is_move_assignable_v<typeT>, "-- move assignable");
	static_assert(!std::is_nothrow_move_assignable_v<typeT>, "-- move assignable");
	static_assert(!std::is_trivially_move_assignable_v<typeT>, "-- trivially move assignable");

	static_assert(std::is_destructible_v<typeT>, "-- destructable");
	static_assert(std::is_nothrow_destructible_v<typeT>, "-- nothrow destructable");
	static_assert(std::is_trivially_destructible_v<typeT>, "-- trivially destructable");
	static_assert(!std::has_virtual_destructor_v<typeT>, "-- virtual destructor");

	static_assert(!std::is_swappable_v<typeT>, "-- swappable");			//C++17
	static_assert(!std::is_nothrow_swappable_v<typeT>, "-- nothrow swappable");	//C++17

	// variations
	static_assert(std::is_default_constructible_v<Section>, "not Section()");
	static_assert(std::is_copy_assignable_v<Section>, "-- copy assignable");
	static_assert(std::is_move_assignable_v<Section>, "-- move assignable");
	static_assert(std::is_swappable_v<Section>, "-- swappable");			//C++17
}
TEST(Board_Sections, Row)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	ASSERT_NO_THROW(Board<int>().row(0));	//??? intellisense: incomplete type
	// see deathtests
	ASSERT_NO_THROW(Board<int>().row(Location<3>(12)));

	EXPECT_NO_THROW(Board<int>().row(0)[0]);
	EXPECT_NO_THROW((Board<int, 2>().row(0)[0]));	// [gTest]
	EXPECT_NO_THROW(A.row(0)[0]);
	EXPECT_NO_THROW(cA.row(0)[0]);
	EXPECT_EQ(A.row(0)[3], 3);
	EXPECT_EQ(cA.row(0)[3], 3);
	EXPECT_EQ(A.row(3)[3], 15);
	EXPECT_NE(A.row(3)[3], 10);
	// see deathtests

	EXPECT_NO_THROW(Board<int>().row(Location<3>(13))[0]);
	EXPECT_NO_THROW(A.row(Location<2>(13))[0]);
	EXPECT_EQ(A.row(Location<2>(13))[0], 12);
	EXPECT_EQ(cA.row(Location<2>(13))[0], 12);
	EXPECT_NE(A.row(Location<2>(13))[0], 10);
	EXPECT_NE(cA.row(Location<2>(13))[0], 10);
}


TEST(Board_Sections, Col)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	ASSERT_NO_THROW(Board<int>().col(0));	//??? intellisense: incomplete type
	ASSERT_NO_THROW(Board<int>().col(Location<3>(12)));

	EXPECT_NO_THROW(Board<int>().col(0)[0]);
	EXPECT_NO_THROW((Board<int, 2>().col(0)[0]));	// [gTest]
	EXPECT_NO_THROW(A.col(0)[0]);
	EXPECT_NO_THROW(cA.col(0)[0]);
	EXPECT_EQ(A.col(0)[0], 9);
	EXPECT_EQ(A.col(0)[1], 4);
	EXPECT_EQ(A.col(1)[0], 1);
	EXPECT_EQ(A.col(1)[3], 13);
	EXPECT_EQ(cA.col(0)[3], 12);
	EXPECT_EQ(cA.col(3)[3], 15);
	EXPECT_EQ(A.col(3)[3], 15);
	EXPECT_NE(A.col(3)[3], 10);

	EXPECT_NO_THROW(Board<int>().col(Location<3>(13))[0]);
	EXPECT_NO_THROW(A.col(Location<2>(13))[0]);
	EXPECT_EQ(A.col(Location<2>(13))[0], 1);
	EXPECT_EQ(cA.col(Location<2>(13))[0], 1);
	EXPECT_NE(A.col(Location<2>(13))[0], 10);
	EXPECT_NE(cA.col(Location<2>(13))[0], 10);
}

TEST(Board_Sections, Block)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	ASSERT_NO_THROW(Board<int>().block(0));	//??? intellisense: incomplete type
	ASSERT_NO_THROW(Board<int>().block(Location<3>(12)));

	EXPECT_NO_THROW(Board<int>().block(0)[0]);
	EXPECT_NO_THROW((Board<int, 2>().block(0)[0]));	// [gTest]
	EXPECT_NO_THROW(A.block(0)[0]);
	EXPECT_NO_THROW(cA.block(0)[0]);
	EXPECT_EQ(A.block(0)[0], 9);
	EXPECT_EQ(A.block(0)[1], 1);
	EXPECT_EQ(A.block(1)[0], 2);
	EXPECT_EQ(A.block(1)[3], 7);
	EXPECT_EQ(cA.block(0)[3], 5);
	EXPECT_EQ(cA.block(3)[3], 15);
	EXPECT_EQ(A.block(3)[3], 15);
	EXPECT_NE(A.block(3)[3], 10);

	EXPECT_NO_THROW(Board<int>().block(Location<3>(13))[0]);
	EXPECT_NO_THROW(A.block(Location<2>(13))[0]);
	EXPECT_EQ(A.block(Location<2>(13))[0], 8);
	EXPECT_EQ(cA.block(Location<2>(13))[0], 8);
	EXPECT_NE(A.block(Location<2>(13))[0], 10);
	EXPECT_NE(cA.block(Location<2>(13))[0], 10);
}

TEST(Board_Sections, deathtests)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	EXPECT_DEBUG_DEATH({ cA.row(-1); }, "")		<< "const_Row(-1) element out of bounds";
	EXPECT_DEBUG_DEATH({ A.row(-1); }, "")		<< "Row(-1) element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.row(4); }, "")		<< "const_Row(4) element out of bounds";
	EXPECT_DEBUG_DEATH({ A.row(4); }, "")		<< "Row(4) element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.col(-1); }, "")		<< "const_Col(-1) element out of bounds";
	EXPECT_DEBUG_DEATH({ A.col(-1); }, "")		<< "Col(-1) element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.col(4); }, "")		<< "const_Col(4) element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.block(-1); }, "")	<< "const_Block(-1) element out of bounds";
	EXPECT_DEBUG_DEATH({ A.block(-1); }, "")	<< "Block(-1) element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.block(4); }, "")	<< "const_Block(4) element out of bounds";
	EXPECT_DEBUG_DEATH({ A.block(4); }, "")		<< "Block(4) element out of bounds";

	//TODO Row(Location) out of bounds
	//const Location<2> min{ -1 };
	//const Location<2> plus{ 21 };
	//EXPECT_DEBUG_DEATH({ cA.row(min); }, "")	<< "const_Row(Loc(-1) element out of bounds";
	//EXPECT_DEBUG_DEATH({ cA.row(plus); }, "")	<< "const_Row(Loc(21) element out of bounds";

	EXPECT_DEBUG_DEATH({ cA.row(0)[-1]; }, "")	<< "const_Row[-1] element out of bounds";
	EXPECT_DEBUG_DEATH({ A.row(0)[-1]; }, "")	<< "row[-1] element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.row(0)[4]; }, "")	<< "row[4] element out of bounds";
	EXPECT_DEBUG_DEATH({ A.row(0)[4]; }, "")	<< "row[4] element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.col(0)[-1]; }, "")	<< "const_Col[-1] element out of bounds";
	EXPECT_DEBUG_DEATH({ A.col(0)[-1]; }, "")	<< "Col[-1] element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.col(0)[4]; }, "")	<< "const_Col[4] element out of bounds";
	EXPECT_DEBUG_DEATH({ A.col(0)[4]; }, "")	<< "Col[4] element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.block(0)[-1]; }, "") << "const_Block[-1] element out of bounds";
	EXPECT_DEBUG_DEATH({ A.block(0)[-1]; }, "")	<< "Block[-1] element out of bounds";
	EXPECT_DEBUG_DEATH({ cA.block(0)[4]; }, "")	<< "const_Block[4] element out of bounds";
	EXPECT_DEBUG_DEATH({ A.block(0)[4]; }, "")	<< "Block[4] element out of bounds";


	// compatible_(other)
	//TODO check asserts that check if same board
		//assert(owner_->owner_ == other.owner_->owner_);
		//assert(owner_->id() == other.owner_->id());
}


} // namespace SudokuTests::Board_SectionsTest
