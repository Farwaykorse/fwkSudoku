//===--	SudokuTests/Location.cpp										--===//
//
//	Unit tests for the template class Sudoku::Location
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
#include "../Sudoku/Location.h"
#include "../Sudoku/Location_Utilities.h"
// Helpers
#include "../Sudoku/Board.h"
// aditional
#include <type_traits>
#include <vector>

using namespace Sudoku;

namespace SudokuTests::LocationTest
{
namespace compiletime
{
	// Type properties
	using typeT = Location<3>;
	static_assert(std::is_class<typeT>::value, "-- a class, hiding datarepresentation");
	static_assert(!std::is_trivial<typeT>::value, "trivial default constructors & trivially copyable");
	//! different between VC++ / Clang
	//static_assert(!std::is_trivially_copyable<typeT>::value, "-- compatible with std::memcpy & binary copy from/to files");
	static_assert(std::is_standard_layout<typeT>::value, "-- StandardLayoutType");	// can be converted with reinterpret_cast
	static_assert(!std::is_pod<typeT>::value, "++ Plain Old Data, both trivial and standard-layout, C compatible");
	//static_assert(std::has_unique_object_representations<typeT>::value, "");	//C++17	trivially_copyable same object representation
	static_assert(!std::is_empty<typeT>::value, "-- class with no datamembers");	// nothing virtual
	static_assert(!std::is_polymorphic<typeT>::value, "-- inherits atleast one virtual function");
	static_assert(!std::is_final<typeT>::value, "-- cannot be used as base class");
	static_assert(!std::is_abstract<typeT>::value, "-- inherits or declares at least one pure virtual function");

	// default constructor: typeT()
	static_assert(std::is_default_constructible<typeT>::value, "-- default constructor");
	static_assert(std::is_nothrow_default_constructible<typeT>::value, "-- notrow default constructor");
	static_assert(!std::is_trivially_default_constructible<typeT>::value, "++ default, nothing virtual");

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible<typeT>::value, "-- copy constructor");
	static_assert(std::is_nothrow_copy_constructible<typeT>::value, "-- notrow copy constructor");
	static_assert(std::is_trivially_copy_constructible<typeT>::value, "-- trivially copy constructor"); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible<typeT>::value, "-- move constructor");
	static_assert(std::is_nothrow_move_constructible<typeT>::value, "-- nothrow move constructor");
	static_assert(std::is_trivially_move_constructible<typeT>::value, "-- trivially move constructor");

	// copy assingment
	static_assert(!std::is_copy_assignable<typeT>::value, "-- copy assignable");
	static_assert(!std::is_nothrow_copy_assignable<typeT>::value, "-- notrow copy assignable");
	static_assert(!std::is_trivially_copy_assignable<typeT>::value, "-- trivially copy assignable");

	static_assert(!std::is_move_assignable<typeT>::value, "-- move assignable");
	static_assert(!std::is_nothrow_move_assignable<typeT>::value, "-- move assignable");
	static_assert(!std::is_trivially_move_assignable<typeT>::value, "-- trivially move assignable");

	static_assert(std::is_destructible<typeT>::value, "-- destructable");
	static_assert(std::is_nothrow_destructible<typeT>::value, "-- nothrow destructable");
	static_assert(std::is_trivially_destructible<typeT>::value, "-- trivially destructable");
	static_assert(!std::has_virtual_destructor<typeT>::value, "-- virtual destructor");

	//static_assert(!std::is_swappable<typeT>::value, "-- swappable");			//C++17
	//static_assert(!std::is_nothrow_swappable<typeT>::value, "-- nothrow swappable");	//C++17
	// other types
	static_assert(std::is_constructible<typeT, int>::value, "-- should construct from int");
	static_assert(std::is_constructible<typeT, unsigned int>::value, "-- construct from unsigned int");
	static_assert(std::is_constructible<typeT, size_t>::value, "-- construct from size_t");
	static_assert(std::is_constructible<typeT, Location_Block<3>>::value, "-- should construct from Location_Block");
	static_assert(!std::is_constructible<typeT, Location_Block<2>>::value, "-- shouldn't accept non matching dimensions_1");
	static_assert(!std::is_assignable<typeT, Location_Block<3>>::value, "--");
	static_assert(!std::is_assignable<typeT, int>::value, "-- shouldn't be assignable from int, prevent with explicit!!");

	//static_assert(!std::is_swappable_with<typeT, Location_Block<3>>::value, "++");	//C++17
	//static_assert(!std::is_nothrow_swappable_with<typeT, Location_Block<3>>::value, "++");	//C++17
}

namespace Location_Block_compiletime
{
	// Type properties
	using typeT = Location_Block<3>;
	static_assert(std::is_class<typeT>::value, "-- a class, hiding datarepresentation");
	static_assert(!std::is_trivial<typeT>::value, "trivial default constructors & trivially copyable");
	//! different between VC++ / Clang
	//static_assert(!std::is_trivially_copyable<typeT>::value, "++ compatible with std::memcpy & binary copy from/to files");
	static_assert(std::is_standard_layout<typeT>::value, "-- StandardLayoutType");	// can be converted with reinterpret_cast
	static_assert(!std::is_pod<typeT>::value, "++ Plain Old Data, both trivial and standard-layout, C compatible");
	//static_assert(std::has_unique_object_representations<typeT>::value, "");	//C++17	trivially_copyable same object representation
	static_assert(!std::is_empty<typeT>::value, "-- class with no datamembers");	// nothing virtual
	static_assert(!std::is_polymorphic<typeT>::value, "-- inherits atleast one virtual function");
	static_assert(!std::is_final<typeT>::value, "-- cannot be used as base class");
	static_assert(!std::is_abstract<typeT>::value, "-- inherits or declares at least one pure virtual function");

	// default constructor: typeT()
	static_assert(!std::is_default_constructible<typeT>::value, "-- default constructor");
	static_assert(!std::is_nothrow_default_constructible<typeT>::value, "-- notrow default constructor");
	static_assert(!std::is_trivially_default_constructible<typeT>::value, "++ default, nothing virtual");

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible<typeT>::value, "-- copy constructor");
	static_assert(std::is_nothrow_copy_constructible<typeT>::value, "-- notrow copy constructor");
	static_assert(std::is_trivially_copy_constructible<typeT>::value, "-- trivially copy constructor"); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible<typeT>::value, "-- move constructor");
	static_assert(std::is_nothrow_move_constructible<typeT>::value, "-- nothrow move constructor");
	static_assert(std::is_trivially_move_constructible<typeT>::value, "-- trivially move constructor");

	// copy assingment
	static_assert(!std::is_copy_assignable<typeT>::value, "copy assignable");
	static_assert(!std::is_nothrow_copy_assignable<typeT>::value, "notrow copy assignable");
	static_assert(!std::is_trivially_copy_assignable<typeT>::value, "trivially copy assignable");

	static_assert(!std::is_move_assignable<typeT>::value, "move assignable");
	static_assert(!std::is_nothrow_move_assignable<typeT>::value, "move assignable");
	static_assert(!std::is_trivially_move_assignable<typeT>::value, "trivially move assignable");

	static_assert(std::is_destructible<typeT>::value, "-- destructable");
	static_assert(std::is_nothrow_destructible<typeT>::value, "-- nothrow destructable");
	static_assert(std::is_trivially_destructible<typeT>::value, "-- trivially destructable");
	static_assert(!std::has_virtual_destructor<typeT>::value, "-- virtual destructor");

	//static_assert(!std::is_swappable<typeT>::value, "swappable");			//C++17
	//static_assert(!std::is_nothrow_swappable<typeT>::value, "nothrow swappable");	//C++17
	// other types
	static_assert(!std::is_constructible<typeT, int>::value, "-- should not construct from int");
	static_assert(std::is_constructible<typeT, Location<3>>::value, "-- should construct from Location");
	static_assert(!std::is_constructible<typeT, Location_Block<2>>::value, "-- shouldn't accept non matching dimensions_1");
	static_assert(!std::is_constructible<typeT, Location<2>>::value, "-- should not construct from non matching Location");

	static_assert(!std::is_assignable<typeT, Location<3>>::value, "assignable from Location");
	static_assert(!std::is_assignable<typeT, Location_Block<2>>::value, "-- assignable wrong size");
	static_assert(!std::is_assignable<typeT, Location<2>>::value, "-- assignable Location wrong size");
	static_assert(!std::is_assignable<typeT, int>::value, "-- shouldn't be assignable from int, prevent with explicit!!");

	//static_assert(!std::is_swappable_with<typeT, Location<3>>::value, "++");	//C++17
	//static_assert(!std::is_nothrow_swappable_with<typeT, Location<3>>::value, "++");	//C++17
}

// Shared data used for all tests
// Initiate tests with: TEST_F(LocationTest, test_name)
class LocationTest : public ::testing::Test
{
protected:
	// Per-test-case, used for all tests
	// Use for objects that are expensive to generate
	// but are not changed by the tests
	//static void SetUpTestCase() {}
	//static void TearDownTestCase() {}

	// Per-test
	//virtual void SetUp() {}
	//virtual void TearDown() {}
	const Location<3> locE{};
	const Location<3> loc0{ 0 };
	const Location<3> loc1{ 1 };
	const Location<3> loc52{ 52 };
};

TEST(Location, Construction)
{
	EXPECT_NO_THROW(Location<3>());
	EXPECT_NO_THROW(Location<3>{});
	EXPECT_NO_THROW(Location<3> LL2{});
	EXPECT_NO_THROW(Location<2> LL3);
	
	EXPECT_NO_THROW(Location<3>(12));
	EXPECT_EQ(Location<3>(12).element(), 12);
	EXPECT_EQ(Location<3>{12}.element(), 12);

	EXPECT_NO_THROW(Location<3>(1, 8));
	EXPECT_EQ(Location<3>(1,8).element(), 17);

	EXPECT_NO_THROW(Location<3>(Location<3>(6)));
	EXPECT_EQ(Location<3>(Location<3>(6)).element(), 6);
	EXPECT_EQ(Location<3>(Location<3>{6}).element(), 6);
	EXPECT_EQ(Location<3>{Location<3>(6)}.element(), 6);
	EXPECT_EQ(Location<3>{Location<3>{6}}.element(), 6);

	ASSERT_NO_THROW(Location<3> L1(12));
	Location<3> L1(12);
	EXPECT_EQ(L1.element(), 12);
	ASSERT_NO_THROW(Location<3> L2(1, 8));
	Location<3> L2(1, 8);
	EXPECT_EQ(L2.element(), 17);
	ASSERT_NO_THROW([[maybe_unused]] Location<3> L3(Location<3>(6)));
	Location<3> L3(Location<3>(6));
	EXPECT_EQ(L3.element(), 6);
	EXPECT_EQ(Location<3>(L3).element(), 6);
	EXPECT_EQ(Location<3>{L3}.element(), 6);
}
TEST(Location_Block, Construction)
{
	Location_Block<3> B1(8, 8);
	EXPECT_EQ(B1.element(), 8);
	EXPECT_EQ(Location_Block<3>(8, 8).element(), 8);
	//??? ERROR if using {}
	//EXPECT_EQ(Location_Block<3>{8, 8}.element(), 8);
	Location_Block<3> B6{ 0, 0, 2 };
	EXPECT_EQ(B6.element(), 2);
	//??? ERROR if using {}
	//EXPECT_EQ(Location_Block<3>{0, 0, 2}.element(), 2);
	EXPECT_EQ(Location_Block<3>(0, 0, 2).element(), 2);
	Location_Block<3> B2(B1);
	EXPECT_EQ(B2.element(), 8) << "Copy constructor";
	EXPECT_EQ(Location_Block<3>(B1).element(), 8) << "Copy constructor";
	EXPECT_EQ(Location_Block<3>{B1}.element(), 8) << "Copy constructor";
	Location_Block<3> B3 = B1;
	EXPECT_EQ(B3.element(), 8) << "Copy";
	Location_Block<3> B4(Location_Block<3>(0, 2));
	EXPECT_EQ(B4.element(), 2) << "Move constructor";
	EXPECT_EQ(Location_Block<3>(Location_Block<3>(0,2)).element(), 2) << "Move constructor";
	EXPECT_EQ(Location_Block<3>(Location_Block<3>{0,2}).element(), 2) << "Move constructor";
	//??? ERROR if using {}
	//EXPECT_EQ(Location_Block<3>{Location_Block<3>{0,2}}.element(), 2) << "Move constructor";
	EXPECT_EQ(Location_Block<3>{Location_Block<3>(0,2)}.element(), 2) << "Move constructor";
	Location_Block<3> B5 = Location_Block<3>(8, 8);
	EXPECT_EQ(B5.element(), 8) << "Move";

	// back and forth
	ASSERT_NO_THROW(Location<3>(Location_Block<3>(1, 3)));
	EXPECT_EQ(Location_Block<3>{ Location<3>{12} }.id(), 1);
	//??? ERROR if using {} 
	//EXPECT_EQ(Location<3>{ Location_Block<3>{8, 8} }.element(), 80);
	EXPECT_EQ(Location<3>( Location_Block<3>{8, 8} ).element(), 80);
	EXPECT_EQ(Location<3>{ Location_Block<3>(8, 8) }.element(), 80);
}
TEST(Location, Size_definitions)
{
	EXPECT_EQ(Location<2>().base_size, 2);
	EXPECT_EQ(Location<2>().elem_size, 4);
	EXPECT_EQ(Location<2>().full_size, 16);
	EXPECT_EQ(Location<3>().base_size, 3);
	EXPECT_EQ(Location<3>().elem_size, 9);
	EXPECT_EQ(Location<3>().full_size, 81);
	EXPECT_EQ(Location<4>().base_size, 4);
	EXPECT_EQ(Location<4>().elem_size, 16);
	EXPECT_EQ(Location<4>().full_size, 256);
}

template<int N>
void SubProperties(Location<N> loc,
	int base,	int elem,	int row,	int col,
	int block,	int b_elem,	int b_row,	int b_col)
{
	ASSERT_EQ(loc.base_size,	base);

	EXPECT_EQ(loc.element(),	elem);
	EXPECT_EQ(loc.row(),		row);
	EXPECT_EQ(loc.col(),		col);
	EXPECT_EQ(loc.block(),		block);
	EXPECT_EQ(loc.block_elem(),	b_elem);
	EXPECT_EQ(loc.block_row(),	b_row);
	EXPECT_EQ(loc.block_col(),	b_col);
}
TEST(Location, Properties)
{
	ASSERT_EQ(Location<3>{36}.base_size, 3);
	ASSERT_EQ(Location<3>{36}.element(), 36);

	EXPECT_EQ(Location<3>{36}.row(), 4);
	EXPECT_EQ(Location<3>{36}.col(), 0);
	EXPECT_EQ(Location<3>{36}.block(), 3);
	EXPECT_EQ(Location<3>{36}.block_row(), 1);
	EXPECT_EQ(Location<3>{36}.block_col(), 0);
	EXPECT_EQ(Location<3>{36}.block_elem(), 3);
	{
		SCOPED_TRACE("Location<3> loc{ 52 }");
		SubProperties(
			Location<3>{ 52 },
			3,	52,	5,	7,
			5,	7,	2,	1
		);
	}
	{
		SCOPED_TRACE("Location<3> loc(3, 2)");
		SubProperties(
			Location<3>(3, 2),
			3,	29,	3,	2,
			3,	2,	0,	2
		);
	}
	{
		SCOPED_TRACE("Location<3> loc(0)");
		SubProperties(
			Location<3>(0),
			3,	0,	0,	0,
			0,	0,	0,	0
		);
	}
	{
		SCOPED_TRACE("Location<3> loc(80)");
		SubProperties(
			Location<3>(80),
			3,	80,	8,	8,
			8,	8,	2,	2
		);
	}
	{
		SCOPED_TRACE("Location<2> loc{}");
		SubProperties(
			Location<2>{},
			2,	0,	0,	0,
			0,	0,	0,	0
		);
	}
	{
		SCOPED_TRACE("Location<2> loc{ 15 }");
		SubProperties(
			Location<2>{ 15 },
			2,	15,	3,	3,
			3,	3,	1,	1
		);
	}
}
TEST(Location, OutOfBounds)
{
	{
		SCOPED_TRACE("Location<2> loc{ 16 }");
		SubProperties(
			Location<2>{ 16 },
			2, 16, 4, 0,
			4, 0, 0, 0
		);
	}
	{
		SCOPED_TRACE("Location<3> loc{ 100 }");
		SubProperties(
			Location<3>{ 100 },
			3, 100, 11, 1,
			9, 7, 2, 1
		);
	}
}
TEST(Location_Block, Properties)
{
	EXPECT_EQ(Location_Block<3>(2, 6).id(), 2);
	EXPECT_EQ(Location_Block<3>(2, 6).element(), 6);
	EXPECT_EQ(Location_Block<3>(2, 6).row(), 2);
	EXPECT_EQ(Location_Block<3>(2, 6).col(), 0);

	Location_Block<3> B1(2, 6);
	EXPECT_EQ(B1.id(), 2);
	EXPECT_EQ(B1.element(), 6);
	EXPECT_EQ(B1.row(), 2);
	EXPECT_EQ(B1.col(), 0);

	Location<3> L1(B1);
	EXPECT_EQ(L1.block(), B1.id());
	EXPECT_EQ(L1.block_elem(), B1.element());
	EXPECT_EQ(L1.block_row(), B1.row());
	EXPECT_EQ(L1.block_col(), B1.col());

	Location_Block<3> B2(2, 1, 0);
	EXPECT_EQ(B2.id(), 2);
	EXPECT_EQ(B2.element(), 3);
	EXPECT_EQ(B2.row(), 1);
	EXPECT_EQ(B2.col(), 0);

	Location<3> L2(B2);
	EXPECT_EQ(L2.block(), B2.id());
	EXPECT_EQ(L2.block_elem(), B2.element());
	EXPECT_EQ(L2.block_row(), B2.row());
	EXPECT_EQ(L2.block_col(), B2.col());
}

TEST(Location, Comparisson)
{
	EXPECT_EQ(Location<3>(4), Location<3>(4)) << "verifies a == b";
	EXPECT_FALSE(Location<3>(0) == Location<3>(13));
	EXPECT_NE(Location<3>(0), Location<3>(13)) << "verifies a != b";
	EXPECT_LT(Location<3>(0, 5), Location<3>(3, 2)) << "verifies a < b";
	EXPECT_FALSE(Location<3>(80) < Location<3>(8, 3));
	//TODO EXPECT_LE(Location<3>(3), Location<3>(4))		<< "verifies a <= b";
	//EXPECT_LE(Location<3>(4), Location<3>(4, 0))	<< "verifies a <= b";
	//EXPECT_FALSE(Location<3>(80) <= Location<3>(0));
	//TODO EXPECT_GT(Location<3>(12), Location<3>(9))		<< "verifies a > b";
	//EXPECT_FALSE(Location<3>(8) > Location<3>(19));
	//TODO EXPECT_GE(Location<3>(3), Location<3>(4))		<< "verifies a >= b";
	//EXPECT_GE(Location<3>(3), Location<3>(3))		<< "verifies a >= b";
	//EXPECT_FALSE(Location<3>(8) >= Location<3>(19));
}
TEST(Location_Block, Comparisson)
{
	EXPECT_EQ(Location_Block<3>(0, 0), Location_Block<3>(0, 0, 0))	<< "verifies a == b";
	EXPECT_FALSE(Location_Block<3>(4, 3) == Location_Block<3>(4, 4));
	//TODO EXPECT_NE(Location_Block<3>(0, 0), Location_Block<3>(0, 13))		<< "verifies a != b";
	//EXPECT_FALSE(Location_Block<3>(4, 4) != Location_Block<3>(4, 4));
	EXPECT_LT(Location_Block<3>(0, 5), Location_Block<3>(3, 2))		<< "verifies a < b";
	EXPECT_FALSE(Location_Block<3>(0, 5) < Location_Block<3>(0, 2));
	EXPECT_FALSE(Location_Block<3>(8, 2, 2) < Location_Block<3>(8, 8));

	EXPECT_EQ(Location_Block<3>(0, 0), Location<3>(0));
	EXPECT_EQ(Location<3>(1), Location_Block<3>(0, 1));
}

TEST(Location, is_constexpr)
{
	// noexcept is always true for a constant expression.
	// therefor it can be used to check if a particular invocation takes the constexpr branch
	EXPECT_TRUE(noexcept(Location<3>()));
	EXPECT_TRUE(noexcept(Location<3>{}));
	EXPECT_TRUE(noexcept(Location<3>{5}));
	EXPECT_TRUE(noexcept(Location<3>{5, 3}));
	EXPECT_TRUE(noexcept(Location<3>().base_size));
	EXPECT_TRUE(noexcept(Location<3>().elem_size));
	EXPECT_TRUE(noexcept(Location<3>().full_size));
	EXPECT_TRUE(noexcept(Location<3>().element()));
	EXPECT_TRUE(noexcept(Location<3>(0).element()));
	EXPECT_TRUE(noexcept(Location<3>(1).element()));
	EXPECT_TRUE(noexcept(Location<3>(2).element()));
	EXPECT_TRUE(noexcept(Location<3>(79).element()));
	EXPECT_TRUE(noexcept(Location<3>(2, 7).element()));
	EXPECT_TRUE(noexcept(Location<3>(79).row()));
	EXPECT_TRUE(noexcept(Location<3>(79).col()));
	EXPECT_TRUE(noexcept(Location<3>(79).block_row()));
	EXPECT_TRUE(noexcept(Location<3>(80).block_col()));
	EXPECT_TRUE(noexcept(Location<3>(80).block_elem()));
	EXPECT_TRUE(noexcept(Location<3>() == Location<3>(0)));
	EXPECT_TRUE(noexcept(Location<3>() != Location<3>(0)));
	EXPECT_TRUE(noexcept(Location<3>() < Location<3>(0)));

	EXPECT_TRUE(noexcept(Location<2>().element()));
	EXPECT_TRUE(noexcept(Location<4>().element()));

	// not precalculated
	Location<3> L0{};
	EXPECT_FALSE(noexcept(L0.element()));
	EXPECT_FALSE(noexcept(L0.row()));
	EXPECT_FALSE(noexcept(L0.col()));
	EXPECT_FALSE(noexcept(L0.block()));
	EXPECT_FALSE(noexcept(L0.block_row()));
	EXPECT_FALSE(noexcept(L0.block_col()));
	EXPECT_FALSE(noexcept(L0.block_elem()));

	EXPECT_FALSE(noexcept(Location<5>().element()));
}
TEST(Location_Block, is_constexpr)
{
	EXPECT_TRUE(noexcept(Location_Block<3>{Location<3>(12)}));
	EXPECT_TRUE(noexcept(Location_Block<3>{5, 3}));
	EXPECT_TRUE(noexcept(Location_Block<3>{5, 3, 2}));
	EXPECT_TRUE(noexcept(Location_Block<3>{5, 3}.id()));
	EXPECT_TRUE(noexcept(Location_Block<3>{5, 3}.element()));
	EXPECT_TRUE(noexcept(Location_Block<3>{5, 3}.row()));
	EXPECT_TRUE(noexcept(Location_Block<3>{5, 3}.col()));
	EXPECT_TRUE(noexcept(Location_Block<3>(0, 1) == Location_Block<3>(0, 2)));
	EXPECT_TRUE(noexcept(Location_Block<3>(0, 2) < Location_Block<3>(0, 2)));

	EXPECT_TRUE(noexcept(Location<3>(Location_Block<3>{5, 3}).element()));
	EXPECT_TRUE(noexcept(Location_Block<3>(0, 2) == Location<3>(0)));
	EXPECT_TRUE(noexcept(Location<3>() == Location_Block<3>(0, 2)));

	// not precalculated
	Location<3> L0{};
	EXPECT_FALSE(noexcept(Location_Block<3>(L0)));
	Location_Block<3> B0{ L0 };
	EXPECT_FALSE(noexcept(B0.id()));
	EXPECT_FALSE(noexcept(B0.element()));
	EXPECT_FALSE(noexcept(B0.row()));
	EXPECT_FALSE(noexcept(B0.col()));
	Location_Block<3> B1{ 2, 4 };
	EXPECT_FALSE(noexcept(B1.id()));
	EXPECT_FALSE(noexcept(B1.element()));
	EXPECT_FALSE(noexcept(B1.row()));
	EXPECT_FALSE(noexcept(B1.col()));
}

//===---------------------------------------------------------------------===//
TEST(Location_Utilities, Size_definitions)
{
	EXPECT_EQ(base_size<2>, 2);
	EXPECT_EQ(elem_size<2>, 4);
	EXPECT_EQ(full_size<2>, 16);
	EXPECT_EQ(base_size<3>, 3);
	EXPECT_EQ(elem_size<3>, 9);
	EXPECT_EQ(full_size<3>, 81);
	EXPECT_EQ(base_size<4>, 4);
	EXPECT_EQ(elem_size<4>, 16);
	EXPECT_EQ(full_size<4>, 256);
}
TEST(Location_Utilities, is_valid)
{
	EXPECT_FALSE(is_valid(Location<2>(-1)));
	EXPECT_TRUE(is_valid(Location<2>(0)));
	EXPECT_TRUE(is_valid(Location<2>(1)));
	EXPECT_TRUE(is_valid(Location<2>(15)));
	EXPECT_FALSE(is_valid(Location<2>(16)));
	EXPECT_TRUE(is_valid(Location<3>(16)));
	EXPECT_TRUE(is_valid(Location<3>(80)));
	EXPECT_FALSE(is_valid(Location<3>(81)));

	EXPECT_FALSE(is_valid_size<2>(-1));
	EXPECT_TRUE(is_valid_size<2>(0));
	EXPECT_TRUE(is_valid_size<2>(1));
	EXPECT_FALSE(is_valid_size<2>(4));
	EXPECT_TRUE(is_valid_size<3>(4));
	EXPECT_TRUE(is_valid_size<3>(8));
	EXPECT_FALSE(is_valid_size<3>(9));

	EXPECT_FALSE(is_valid_size<2>(-1, 3));
	EXPECT_FALSE(is_valid_size<2>(2, -3));
	EXPECT_TRUE(is_valid_size<2>(1, 0));
	EXPECT_FALSE(is_valid_size<2>(2, 4));

	EXPECT_FALSE(is_valid_value<2>(-1));
	EXPECT_FALSE(is_valid_value<2>(0));
	EXPECT_TRUE(is_valid_value<2>(1));
	EXPECT_TRUE(is_valid_value<2>(4));
	EXPECT_FALSE(is_valid_value<2>(5));
	EXPECT_TRUE(is_valid_value<3>(5));
	EXPECT_FALSE(is_valid_value<3>(16));

	// vector input
	using list = std::vector<unsigned int>;
	EXPECT_FALSE(is_valid_value<2>(list{})) << "can't be empty";
	EXPECT_FALSE(is_valid_value<2>(std::vector<int>{})) << "can't be empty";
	EXPECT_TRUE(is_valid_value<2>(list{ 1, 2, 3, 4, 3, 1 }));
	EXPECT_TRUE(is_valid_value<2>(std::vector<int>{ 1, 2, 3, 4, 3, 1 }));
	EXPECT_TRUE(is_valid_value<2>(list{ 1 }));
	EXPECT_TRUE(is_valid_value<2>(std::vector<int>{ 1 }));
	EXPECT_FALSE(is_valid_value<2>(list{ 0 }));
	EXPECT_FALSE(is_valid_value<2>(std::vector<int>{ 0 }));
	EXPECT_FALSE(is_valid_value<2>(list{ 5 }));
	EXPECT_FALSE(is_valid_value<2>(std::vector<int>{ 5 }));
	EXPECT_FALSE(is_valid_value<2>(list{ 1, 2, 0, 4, 3, 1 }));
	EXPECT_FALSE(is_valid_value<2>(std::vector<int>{ 1, 2, 0, 4, 3, 1 }));
	EXPECT_FALSE(is_valid_value<2>(std::vector<int>{ 1, -2, 3, 4, 3, 1 }));
	EXPECT_FALSE(is_valid_value<2>(list{ 1, 5, 3, 4, 3, 1 }));
	EXPECT_FALSE(is_valid_value<2>(std::vector<int>{ 1, 5, 3, 4, 3, 1 }));

	using L = Location<2>;
	using list2 = std::vector<L>;
	EXPECT_FALSE(is_valid(list2{})) << "can't be empty";
	EXPECT_TRUE(is_valid(list2{L(0), L(12), L(13)}));
	EXPECT_TRUE(is_valid(list2{L(8)})) << "must except single";
	EXPECT_FALSE(is_valid(list2{L(5), L(2), L(16)})) << "must be sorted";
	EXPECT_FALSE(is_valid(list2{L(15), L(2), L(1)})) << "must be sorted ascending";
	EXPECT_FALSE(is_valid(list2{L(0), L(15), L(16)}));
	EXPECT_FALSE(is_valid(list2{L(-1), L(0), L(1)}));
	EXPECT_TRUE(is_valid(list2{L(0)}));
	EXPECT_FALSE(is_valid(list2{L(16)}));
	EXPECT_FALSE(is_valid(list2{L(-6)}));
}
TEST(Location_Utilities, is_same_section)
{
	EXPECT_TRUE(is_same_row(Location<3>(0), Location<3>(8)));
	EXPECT_FALSE(is_same_row(Location<3>(9), Location<3>(8)));
	EXPECT_TRUE(is_same_col(Location<3>(0), Location<3>(18)));
	EXPECT_FALSE(is_same_col(Location<3>(9), Location<3>(8)));
	EXPECT_TRUE(is_same_block(Location<3>(0), Location<3>(11)));
	EXPECT_FALSE(is_same_block(Location<3>(9), Location<3>(8)));

	// is_same_section (taking iterators)
	using L = Location<3>;
	const std::vector<L> row{
		L(0), L(1), L(2), L(3), L(4), L(5), L(6), L(7), L(8)};
	const std::vector<L> col{
		L(0, 6), L(1, 6), L(3, 6), L(4, 6), L(5, 6), L(6, 6), L(8, 6)};
	const std::vector<L> blo{
		L(3, 3), L(3, 4), L(3, 5), L(4, 3), L(4, 4), L(5, 3), L(5, 5)};
	const std::vector<L> notsortedrow{
		L(5), L(3), L(2), L(0), L(4), L(8), L(6), L(7), L(1)};
	const std::vector<L> duplicate{
		L(0), L(1), L(2), L(0), L(4), L(0), L(6), L(1), L(0)};

	EXPECT_NO_THROW(is_same_row<3>(row.begin(), row.end()));
	EXPECT_NO_THROW(is_same_row<3>(row.cbegin(), row.cend()));
	EXPECT_NO_THROW(is_same_col<3>(col.cbegin(), col.cend()));
	EXPECT_TRUE(is_same_row<3>(row.cbegin(), row.cend()));
	EXPECT_TRUE(is_same_col<3>(col.cbegin(), col.cend()));
	EXPECT_TRUE(is_same_block<3>(blo.cbegin(), blo.cend()));
	EXPECT_TRUE(is_same_row<3>(notsortedrow.cbegin(), notsortedrow.cend()));
	EXPECT_TRUE(is_same_row<3>(duplicate.cbegin(), duplicate.cend()));
	EXPECT_FALSE(is_same_col<3>(row.cbegin(), row.cend()));
	EXPECT_FALSE(is_same_block<3>(row.cbegin(), row.cend()));

	// is_same_section (taking a section)
	const Board<int, 3> B1;
	EXPECT_NO_THROW(is_same_section(B1.row(0), L(12)));
	EXPECT_NO_THROW(is_same_section(B1.col(0), L(12)));
	EXPECT_NO_THROW(is_same_section(B1.block(0), L(12)));
	EXPECT_TRUE(is_same_section(B1.row(0), L(8)));
	EXPECT_TRUE(is_same_section(B1.row(1), L(12)));
	EXPECT_TRUE(is_same_section(B1.row(7), L(70)));
	EXPECT_FALSE(is_same_section(B1.row(2), L(15)));
	EXPECT_TRUE(is_same_section(B1.col(0), L(0)));
	EXPECT_TRUE(is_same_section(B1.col(0), L(72)));
	EXPECT_FALSE(is_same_section(B1.col(1), L(9)));
	EXPECT_TRUE(is_same_section(B1.block(0), L(10)));
	EXPECT_FALSE(is_same_section(B1.block(1), L(16)));

	// is_same_section (taking a section and a vector)
	EXPECT_NO_THROW(is_same_section(B1.row(0), row));
	EXPECT_NO_THROW(is_same_section(B1.col(0), row));
	EXPECT_NO_THROW(is_same_section(B1.block(0), row));
	EXPECT_TRUE(is_same_section(B1.row(0), row));
	EXPECT_TRUE(is_same_section(B1.row(0), col));
	EXPECT_FALSE(is_same_section(B1.row(0), blo));
	EXPECT_FALSE(is_same_section(B1.row(1), row));
	EXPECT_TRUE(is_same_section(B1.col(0), row));
	EXPECT_TRUE(is_same_section(B1.col(6), col));
	EXPECT_FALSE(is_same_section(B1.col(0), col));
	EXPECT_FALSE(is_same_section(B1.col(0), blo));
	EXPECT_TRUE(is_same_section(B1.col(5), blo));
	EXPECT_TRUE(is_same_section(B1.block(4), blo));
	EXPECT_FALSE(is_same_section(B1.block(5), blo));

	// intersect_block
	EXPECT_NO_THROW(intersect_block(B1.row(0), L(55)));
	EXPECT_NO_THROW(intersect_block(B1.col(0), L(55)));
	EXPECT_TRUE(intersect_block(B1.row(0), L(8)));
	EXPECT_TRUE(intersect_block(B1.row(6), L(54)));
	EXPECT_FALSE(intersect_block(B1.row(5), L(59)));
	EXPECT_TRUE(intersect_block(B1.col(0), L(11)));
	EXPECT_FALSE(intersect_block(B1.col(8), L(2)));
}
TEST(Location_Utilities, get_same_section)
{
	std::vector<Location<3>> list1{};
	//std::vector<Location<3>> list2{};
	std::vector<Location<3>> list3{};
	for (int i{}; i < 9; ++i)
	{
		list1.push_back(Location<3>{i});
		//list2.push_back(Location<3>{i*3});
		list3.push_back(Location<3>{i*9});
	}
	const std::vector<Location<3>> clist1{list1};

	EXPECT_EQ(get_same_row(Location<3>(0), list1), list1);
	EXPECT_EQ(get_same_row(Location<3>(0), list1).size(), 9) << "vector length";
	EXPECT_EQ(get_same_col(Location<3>(0), list3), list3);
	EXPECT_EQ(get_same_col(Location<3>(0), list3).size(), 9) << "vector length";
	EXPECT_EQ(get_same_block(Location<3>(0), list1).size(), 3) << "length";
	EXPECT_EQ(get_same_row(Location<3>(0), clist1), list1);
}
TEST(Location_Utilities, is_constexpr)
{
	EXPECT_TRUE(noexcept(is_valid(Location<2>(10))));
	EXPECT_TRUE(noexcept(is_valid(Location<3>(67))));
	EXPECT_TRUE(noexcept(is_valid(Location<3>(97)))); //

	EXPECT_TRUE(noexcept(is_valid_size<2>(3)));
	EXPECT_TRUE(noexcept(is_valid_size<3>(8)));

	EXPECT_TRUE(noexcept(is_valid_size<2>(1, 2)));
	EXPECT_TRUE(noexcept(is_valid_size<3>(7, 3)));

	EXPECT_TRUE(noexcept(is_valid_value<2>(1)));
	EXPECT_TRUE(noexcept(is_valid_value<3>(7)));

	EXPECT_FALSE(
		noexcept(is_valid_value<2>(std::vector<int>{1, 2, 3, 4, 3, 1})));
	EXPECT_FALSE(noexcept(is_valid(std::vector<Location<2>>{Location<2>(0)})));
	EXPECT_FALSE(noexcept(is_valid(std::vector<Location<3>>{Location<3>(0)})));
	EXPECT_FALSE(noexcept(is_valid(std::vector<Location<2>>{})));
	EXPECT_FALSE(noexcept(is_valid(std::vector<Location<3>>{})));

	EXPECT_TRUE(noexcept(is_same_row(Location<3>(0), Location<3>(8))));
	EXPECT_TRUE(noexcept(is_same_col(Location<3>(0), Location<3>(8))));
	EXPECT_TRUE(noexcept(is_same_block(Location<3>(0), Location<3>(8))));

	std::vector<Location<3>> list1{};
	EXPECT_FALSE(noexcept(get_same_row(Location<3>(0), list1)));
	EXPECT_FALSE(noexcept(get_same_col(Location<3>(0), list1)));
	EXPECT_FALSE(noexcept(get_same_block(Location<3>(0), list1)));
}

}	// namespace SudokuTests::LocationTest
