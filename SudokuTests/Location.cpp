/**	Unit tests for the template class Sudoku::Location
*
*/
#include "gtest/gtest.h"

// Class under test
#include "../Sudoku/Location.h"
// aditional
#include <type_traits>
#include <vector>

using namespace Sudoku;

namespace SudokuTests::LocationTest
{
namespace compiletime_Location
{
	// Type properties
	using typeT = Location<3>;
	static_assert(std::is_class<typeT>(), "-- a class, hiding datarepresentation");
	static_assert(!std::is_trivial<typeT>(), "trivial default constructors & trivially copyable");
	static_assert(!std::is_trivially_copyable<typeT>(), "-- compatible with std::memcpy & binary copy from/to files");
	static_assert(std::is_standard_layout<typeT>(), "-- StandardLayoutType");	// can be converted with reinterpret_cast
	static_assert(!std::is_pod<typeT>(), "++ Plain Old Data, both trivial and standard-layout, C compatible");
	//static_assert(std::has_unique_object_representations<typeT>(), "");	//C++17	trivially_copyable same object representation
	static_assert(!std::is_empty<typeT>(), "-- class with no datamembers");	// nothing virtual
	static_assert(!std::is_polymorphic<typeT>(), "-- inherits atleast one virtual function");
	static_assert(!std::is_final<typeT>(), "-- cannot be used as base class");
	static_assert(!std::is_abstract<typeT>(), "-- inherits or declares at least one pure virtual function");

	// default constructor: typeT()
	static_assert(std::is_default_constructible<typeT>(), "-- default constructor");
	static_assert(std::is_nothrow_default_constructible<typeT>(), "-- notrow default constructor");
	static_assert(!std::is_trivially_default_constructible<typeT>(), "++ default, nothing virtual");

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible<typeT>(), "-- copy constructor");
	static_assert(std::is_nothrow_copy_constructible<typeT>(), "-- notrow copy constructor");
	static_assert(std::is_trivially_copy_constructible<typeT>(), "-- trivially copy constructor"); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible<typeT>(), "-- move constructor");
	static_assert(std::is_nothrow_move_constructible<typeT>(), "-- nothrow move constructor");
	static_assert(std::is_trivially_move_constructible<typeT>(), "-- trivially move constructor");

	// copy assingment
	static_assert(!std::is_copy_assignable<typeT>(), "-- copy assignable");
	static_assert(!std::is_nothrow_copy_assignable<typeT>(), "-- notrow copy assignable");
	static_assert(!std::is_trivially_copy_assignable<typeT>(), "-- trivially copy assignable");

	static_assert(!std::is_move_assignable<typeT>(), "-- move assignable");
	static_assert(!std::is_nothrow_move_assignable<typeT>(), "-- move assignable");
	static_assert(!std::is_trivially_move_assignable<typeT>(), "-- trivially move assignable");

	static_assert(std::is_destructible<typeT>(), "-- destructable");
	static_assert(std::is_nothrow_destructible<typeT>(), "-- nothrow destructable");
	static_assert(std::is_trivially_destructible<typeT>(), "-- trivially destructable");
	static_assert(!std::has_virtual_destructor<typeT>(), "-- virtual destructor");

	static_assert(!std::is_swappable<typeT>(), "-- swappable");			//C++17
	static_assert(!std::is_nothrow_swappable<typeT>(), "-- nothrow swappable");	//C++17
	// other types
	static_assert(std::is_constructible<typeT, int>(), "-- should construct from int");
	static_assert(std::is_constructible<typeT, unsigned int>(), "-- construct from unsigned int");
	static_assert(std::is_constructible<typeT, size_t>(), "-- construct from size_t");
	static_assert(std::is_constructible<typeT, Block_Loc<3>>(), "-- should construct from Block_Loc");
	static_assert(!std::is_constructible<typeT, Block_Loc<2>>(), "-- shouldn't accept non matching dimensions_1");
	static_assert(!std::is_assignable<typeT, Block_Loc<3>>(), "--");
	static_assert(!std::is_assignable<typeT, int>(), "-- shouldn't be assignable from int, prevent with explicit!!");

	static_assert(!std::is_swappable_with<typeT, Block_Loc<3>>(), "++");	//C++17
	static_assert(!std::is_nothrow_swappable_with<typeT, Block_Loc<3>>(), "++");	//C++17
}

namespace compiletime_Block_Loc
{
	// Type properties
	using typeT = Block_Loc<3>;
	static_assert(std::is_class<typeT>(), "-- a class, hiding datarepresentation");
	static_assert(!std::is_trivial<typeT>(), "trivial default constructors & trivially copyable");
	static_assert(!std::is_trivially_copyable<typeT>(), "++ compatible with std::memcpy & binary copy from/to files");
	static_assert(std::is_standard_layout<typeT>(), "-- StandardLayoutType");	// can be converted with reinterpret_cast
	static_assert(!std::is_pod<typeT>(), "++ Plain Old Data, both trivial and standard-layout, C compatible");
	//static_assert(std::has_unique_object_representations<typeT>(), "");	//C++17	trivially_copyable same object representation
	static_assert(!std::is_empty<typeT>(), "-- class with no datamembers");	// nothing virtual
	static_assert(!std::is_polymorphic<typeT>(), "-- inherits atleast one virtual function");
	static_assert(!std::is_final<typeT>(), "-- cannot be used as base class");
	static_assert(!std::is_abstract<typeT>(), "-- inherits or declares at least one pure virtual function");

	// default constructor: typeT()
	static_assert(!std::is_default_constructible<typeT>(), "-- default constructor");
	static_assert(!std::is_nothrow_default_constructible<typeT>(), "-- notrow default constructor");
	static_assert(!std::is_trivially_default_constructible<typeT>(), "++ default, nothing virtual");

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible<typeT>(), "-- copy constructor");
	static_assert(std::is_nothrow_copy_constructible<typeT>(), "-- notrow copy constructor");
	static_assert(std::is_trivially_copy_constructible<typeT>(), "-- trivially copy constructor"); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible<typeT>(), "-- move constructor");
	static_assert(std::is_nothrow_move_constructible<typeT>(), "-- nothrow move constructor");
	static_assert(std::is_trivially_move_constructible<typeT>(), "-- trivially move constructor");

	// copy assingment
	static_assert(!std::is_copy_assignable<typeT>(), "copy assignable");
	static_assert(!std::is_nothrow_copy_assignable<typeT>(), "notrow copy assignable");
	static_assert(!std::is_trivially_copy_assignable<typeT>(), "trivially copy assignable");

	static_assert(!std::is_move_assignable<typeT>(), "move assignable");
	static_assert(!std::is_nothrow_move_assignable<typeT>(), "move assignable");
	static_assert(!std::is_trivially_move_assignable<typeT>(), "trivially move assignable");

	static_assert(std::is_destructible<typeT>(), "-- destructable");
	static_assert(std::is_nothrow_destructible<typeT>(), "-- nothrow destructable");
	static_assert(std::is_trivially_destructible<typeT>(), "-- trivially destructable");
	static_assert(!std::has_virtual_destructor<typeT>(), "-- virtual destructor");

	static_assert(!std::is_swappable<typeT>(), "swappable");			//C++17
	static_assert(!std::is_nothrow_swappable<typeT>(), "nothrow swappable");	//C++17
	// other types
	static_assert(!std::is_constructible<typeT, int>(), "-- should not construct from int");
	static_assert(std::is_constructible<typeT, Location<3>>(), "-- should construct from Location");
	static_assert(!std::is_constructible<typeT, Block_Loc<2>>(), "-- shouldn't accept non matching dimensions_1");
	static_assert(!std::is_constructible<typeT, Location<2>>(), "-- should not construct from non matching Location");

	static_assert(!std::is_assignable<typeT, Location<3>>(), "assignable from Location");
	static_assert(!std::is_assignable<typeT, Block_Loc<2>>(), "-- assignable wrong size");
	static_assert(!std::is_assignable<typeT, Location<2>>(), "-- assignable Location wrong size");
	static_assert(!std::is_assignable<typeT, int>(), "-- shouldn't be assignable from int, prevent with explicit!!");

	static_assert(!std::is_swappable_with<typeT, Location<3>>(), "++");	//C++17
	static_assert(!std::is_nothrow_swappable_with<typeT, Location<3>>(), "++");	//C++17
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
	//EXPECT_NO_THROW(Location<3> LL1());	// generates a warning == LL3
	EXPECT_NO_THROW(Location<3> LL2{});
	EXPECT_NO_THROW(Location<2> LL3);
	
	EXPECT_NO_THROW(Location<3>(12));
	EXPECT_EQ(Location<3>(12).element(), 12);
	EXPECT_EQ(Location<3>{12}.element(), 12);

	EXPECT_NO_THROW(Location<3>(1, 8));
	EXPECT_EQ(Location<3>(1,8).element(), 17);
	//EXPECT_EQ(Location<3>{1,8}.element(), 18);

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
	ASSERT_NO_THROW(Location<3> L3(Location<3>(6)));
	Location<3> L3(Location<3>(6));
	EXPECT_EQ(L3.element(), 6);
	EXPECT_EQ(Location<3>(L3).element(), 6);
	EXPECT_EQ(Location<3>{L3}.element(), 6);
}
TEST(Block_Loc, Construction)
{
	Block_Loc<3> B1(8, 8);
	EXPECT_EQ(B1.element(), 8);
	EXPECT_EQ(Block_Loc<3>(8, 8).element(), 8);
	//ERROR if using {}
	//EXPECT_EQ(Block_Loc<3>{8, 8}.element(), 8);
	Block_Loc<3> B6{ 0, 0, 2 };
	EXPECT_EQ(B6.element(), 2);
	//ERROR if using {}
	//EXPECT_EQ(Block_Loc<3>{0, 0, 2}.element(), 2);
	EXPECT_EQ(Block_Loc<3>(0, 0, 2).element(), 2);
	Block_Loc<3> B2(B1);
	EXPECT_EQ(B2.element(), 8) << "Copy constructor";
	EXPECT_EQ(Block_Loc<3>(B1).element(), 8) << "Copy constructor";
	EXPECT_EQ(Block_Loc<3>{B1}.element(), 8) << "Copy constructor";
	Block_Loc<3> B3 = B1;
	EXPECT_EQ(B3.element(), 8) << "Copy";
	Block_Loc<3> B4(Block_Loc<3>(0, 2));
	EXPECT_EQ(B4.element(), 2) << "Move constructor";
	EXPECT_EQ(Block_Loc<3>(Block_Loc<3>(0,2)).element(), 2) << "Move constructor";
	EXPECT_EQ(Block_Loc<3>(Block_Loc<3>{0,2}).element(), 2) << "Move constructor";
	//ERROR if using {}
	//EXPECT_EQ(Block_Loc<3>{Block_Loc<3>{0,2}}.element(), 2) << "Move constructor";
	EXPECT_EQ(Block_Loc<3>{Block_Loc<3>(0,2)}.element(), 2) << "Move constructor";
	Block_Loc<3> B5 = Block_Loc<3>(8, 8);
	EXPECT_EQ(B5.element(), 8) << "Move";

	// back and forth
	ASSERT_NO_THROW(Location<3>(Block_Loc<3>(1, 3)));
	EXPECT_EQ(Block_Loc<3>{ Location<3>{12} }.id(), 1);
	//ERROR if using {} 
	//EXPECT_EQ(Location<3>{ Block_Loc<3>{8, 8} }.element(), 80);
	EXPECT_EQ(Location<3>( Block_Loc<3>{8, 8} ).element(), 80);
	EXPECT_EQ(Location<3>{ Block_Loc<3>(8, 8) }.element(), 80);
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
TEST(Block_Loc, Properties)
{
	EXPECT_EQ(Block_Loc<3>(2, 6).id(), 2);
	EXPECT_EQ(Block_Loc<3>(2, 6).element(), 6);
	EXPECT_EQ(Block_Loc<3>(2, 6).row(), 2);
	EXPECT_EQ(Block_Loc<3>(2, 6).col(), 0);

	Block_Loc<3> B1(2, 6);
	EXPECT_EQ(B1.id(), 2);
	EXPECT_EQ(B1.element(), 6);
	EXPECT_EQ(B1.row(), 2);
	EXPECT_EQ(B1.col(), 0);

	Location<3> L1(B1);
	EXPECT_EQ(L1.block(), B1.id());
	EXPECT_EQ(L1.block_elem(), B1.element());
	EXPECT_EQ(L1.block_row(), B1.row());
	EXPECT_EQ(L1.block_col(), B1.col());

	Block_Loc<3> B2(2, 1, 0);
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
TEST(Block_Loc, Comparisson)
{
	EXPECT_EQ(Block_Loc<3>(0, 0), Block_Loc<3>(0, 0, 0))	<< "verifies a == b";
	EXPECT_FALSE(Block_Loc<3>(4, 3) == Block_Loc<3>(4, 4));
	//TODO EXPECT_NE(Block_Loc<3>(0, 0), Block_Loc<3>(0, 13))		<< "verifies a != b";
	//EXPECT_FALSE(Block_Loc<3>(4, 4) != Block_Loc<3>(4, 4));
	EXPECT_LT(Block_Loc<3>(0, 5), Block_Loc<3>(3, 2))		<< "verifies a < b";
	EXPECT_FALSE(Block_Loc<3>(0, 5) < Block_Loc<3>(0, 2));
	EXPECT_FALSE(Block_Loc<3>(8, 2, 2) < Block_Loc<3>(8, 8));

	EXPECT_EQ(Block_Loc<3>(0, 0), Location<3>(0));
	EXPECT_EQ(Location<3>(1), Block_Loc<3>(0, 1));
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
TEST(Location, external)
{
	EXPECT_TRUE(noexcept(shared_row(Location<3>(0), Location<3>(8))));
	EXPECT_TRUE(noexcept(shared_col(Location<3>(0), Location<3>(8))));
	EXPECT_TRUE(noexcept(shared_block(Location<3>(0), Location<3>(8))));
	EXPECT_TRUE(shared_row(Location<3>(0), Location<3>(8)));
	EXPECT_FALSE(shared_row(Location<3>(9), Location<3>(8)));
	EXPECT_TRUE(shared_col(Location<3>(0), Location<3>(18)));
	EXPECT_FALSE(shared_col(Location<3>(9), Location<3>(8)));
	EXPECT_TRUE(shared_block(Location<3>(0), Location<3>(11)));
	EXPECT_FALSE(shared_block(Location<3>(9), Location<3>(8)));
	//NEEDTEST shared_row / col / block
	//return vector<Location<N>>
	//
	std::vector<Location<3>> list1{};
	//std::vector<Location<3>> list2{};
	std::vector<Location<3>> list3{};
	for (int i{}; i < 9; ++i)
	{
		list1.push_back(Location<3>{i});
		//list2.push_back(Location<3>{i*3});
		list3.push_back(Location<3>{i*9});
	}
	EXPECT_FALSE(noexcept(shared_row(Location<3>(0), list1)));
	EXPECT_FALSE(noexcept(shared_col(Location<3>(0), list1)));
	EXPECT_FALSE(noexcept(shared_block(Location<3>(0), list1)));
	EXPECT_EQ(shared_row(Location<3>(0), list1), list1);
	EXPECT_EQ(shared_row(Location<3>(0), list1).size(), 9) << "vector length";
	EXPECT_EQ(shared_col(Location<3>(0), list3), list3);
	EXPECT_EQ(shared_col(Location<3>(0), list3).size(), 9) << "vector length";
	EXPECT_EQ(shared_block(Location<3>(0), list1).size(), 3) << "vector length";
}

TEST(Block_Loc, is_constexpr)
{
	EXPECT_TRUE(noexcept(Block_Loc<3>{Location<3>(12)}));
	EXPECT_TRUE(noexcept(Block_Loc<3>{5, 3}));
	EXPECT_TRUE(noexcept(Block_Loc<3>{5, 3, 2}));
	EXPECT_TRUE(noexcept(Block_Loc<3>{5, 3}.id()));
	EXPECT_TRUE(noexcept(Block_Loc<3>{5, 3}.element()));
	EXPECT_TRUE(noexcept(Block_Loc<3>{5, 3}.row()));
	EXPECT_TRUE(noexcept(Block_Loc<3>{5, 3}.col()));
	EXPECT_TRUE(noexcept(Block_Loc<3>(0, 1) == Block_Loc<3>(0, 2)));
	EXPECT_TRUE(noexcept(Block_Loc<3>(0, 2) < Block_Loc<3>(0, 2)));

	EXPECT_TRUE(noexcept(Location<3>(Block_Loc<3>{5, 3}).element()));
	EXPECT_TRUE(noexcept(Block_Loc<3>(0, 2) == Location<3>(0)));
	EXPECT_TRUE(noexcept(Location<3>() == Block_Loc<3>(0, 2)));

	// not precalculated
	Location<3> L0{};
	EXPECT_FALSE(noexcept(Block_Loc<3>(L0)));
	Block_Loc<3> B0{ L0 };
	EXPECT_FALSE(noexcept(B0.id()));
	EXPECT_FALSE(noexcept(B0.element()));
	EXPECT_FALSE(noexcept(B0.row()));
	EXPECT_FALSE(noexcept(B0.col()));
	Block_Loc<3> B1{ 2, 4 };
	EXPECT_FALSE(noexcept(B1.id()));
	EXPECT_FALSE(noexcept(B1.element()));
	EXPECT_FALSE(noexcept(B1.row()));
	EXPECT_FALSE(noexcept(B1.col()));
}


}	// namespace SudokuTests::LocationTest
