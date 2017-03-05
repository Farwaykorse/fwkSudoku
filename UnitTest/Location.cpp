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
	TEST_METHOD(T0_initialize_Location)
	{
		// Type properties
		using typeT = Location<3>;
		static_assert(std::is_class<typeT>(), "-- a class, hiding datarepresentation");
		static_assert(! std::is_trivial<typeT>(), "trivial default constructors & trivially copyable");
		static_assert(std::is_trivially_copyable<typeT>(), "-- compatible with std::memcpy & binary copy from/to files");
		static_assert(std::is_standard_layout<typeT>(), "-- StandardLayoutType");	// can be converted with reinterpret_cast
		static_assert(! std::is_pod<typeT>(), "++ Plain Old Data, both trivial and standard-layout, C compatible");
		//static_assert(std::has_unique_object_representations<typeT>(), "");	//C++17	trivially_copyable same object representation
		static_assert(!std::is_empty<typeT>(), "-- class with no datamembers");	// nothing virtual
		static_assert(!std::is_polymorphic<typeT>(), "-- inherits atleast one virtual function");
		static_assert(!std::is_final<typeT>(), "-- cannot be used as base class");
		static_assert(!std::is_abstract<typeT>(), "-- inherits or declares at least one pure virtual function");

		// default constructor: typeT()
		static_assert(std::is_default_constructible<typeT>(), "-- default constructor");
		static_assert(std::is_nothrow_default_constructible<typeT>(), "-- notrow default constructor");
		static_assert(! std::is_trivially_default_constructible<typeT>(), "++ default, nothing virtual");

		// copy constructor: typeT(const typeT&)
		static_assert(std::is_copy_constructible<typeT>(), "-- copy constructor");
		static_assert(std::is_nothrow_copy_constructible<typeT>(), "-- notrow copy constructor");
		static_assert(std::is_trivially_copy_constructible<typeT>(), "-- trivially copy constructor"); // = default

		// move constructor: typeT(typeT&&)
		static_assert(std::is_move_constructible<typeT>(), "-- move constructor");
		static_assert(std::is_nothrow_move_constructible<typeT>(), "-- nothrow move constructor");
		static_assert(std::is_trivially_move_constructible<typeT>(), "-- trivially move constructor");

		// copy assingment
		static_assert(std::is_copy_assignable<typeT>(), "-- copy assignable");
		static_assert(std::is_nothrow_copy_assignable<typeT>(), "-- notrow copy assignable");
		static_assert(std::is_trivially_copy_assignable<typeT>(), "-- trivially copy assignable");

		static_assert(std::is_move_assignable<typeT>(), "-- move assignable");
		static_assert(std::is_nothrow_move_assignable<typeT>(), "-- move assignable");
		static_assert(std::is_trivially_move_assignable<typeT>(), "-- trivially move assignable");

		static_assert(std::is_destructible<typeT>(), "-- destructable");
		static_assert(std::is_nothrow_destructible<typeT>(), "-- nothrow destructable");
		static_assert(std::is_trivially_destructible<typeT>(), "-- trivially destructable");
		static_assert(!std::has_virtual_destructor<typeT>(), "-- virtual destructor");

		static_assert(std::is_swappable<typeT>(), "-- swappable");			//C++17
		static_assert(std::is_nothrow_swappable<typeT>(), "-- nothrow swappable");	//C++17
		// other types
		static_assert(std::is_constructible<typeT, int>(), "-- should construct from int");
		static_assert(std::is_constructible<typeT, unsigned int>(), "-- construct from unsigned int");
		static_assert(std::is_constructible<typeT, size_t>(), "-- construct from size_t");
		static_assert(std::is_constructible<typeT, Block_Loc<3>>(), "-- should construct from Block_Loc");
		static_assert(!std::is_constructible<typeT, Block_Loc<2>>(), "-- shouldn't accept non matching dimensions_1");
		static_assert(std::is_assignable<typeT, Block_Loc<3>>(), "--");
		static_assert(! std::is_assignable<typeT, int>(), "-- shouldn't be assignable from int, prevent with explicit!!");

		static_assert(! std::is_swappable_with<typeT, Block_Loc<3>>(), "++");	//C++17
		static_assert(! std::is_nothrow_swappable_with<typeT, Block_Loc<3>>(), "++");	//C++17


		//try { Location<3>(); }		catch (...) { Assert::Fail(L"Location<3>() Default Construction failed"); }
		try { Location<3>(12); }	catch (...) { Assert::Fail(L"Location<3>(12) Construction from int failed"); }
		try { Location<3>(1, 8); }	catch (...) { Assert::Fail(L"Location<3>(1, 8) Construction failed"); }
		try { Location<3>(Location<3>(6)); }	catch (...) { Assert::Fail(L"Location<3>(1, 8) Construction failed"); }
		try { Location<3>(Block_Loc<3>(1, 3)); }	catch (...) { Assert::Fail(L"Block_Lock<3>(1,3) Construction failed"); }
	}
	TEST_METHOD(T01_initialize_Block_Loc)
	{
		// Type properties
		using typeT = Block_Loc<3>;
		static_assert(std::is_class<typeT>(), "-- a class, hiding datarepresentation");
		static_assert(! std::is_trivial<typeT>(), "trivial default constructors & trivially copyable");
		static_assert(std::is_trivially_copyable<typeT>(), "-- compatible with std::memcpy & binary copy from/to files");
		static_assert(std::is_standard_layout<typeT>(), "-- StandardLayoutType");	// can be converted with reinterpret_cast
		static_assert(! std::is_pod<typeT>(), "++ Plain Old Data, both trivial and standard-layout, C compatible");
		//static_assert(std::has_unique_object_representations<typeT>(), "");	//C++17	trivially_copyable same object representation
		static_assert(!std::is_empty<typeT>(), "-- class with no datamembers");	// nothing virtual
		static_assert(!std::is_polymorphic<typeT>(), "-- inherits atleast one virtual function");
		static_assert(!std::is_final<typeT>(), "-- cannot be used as base class");
		static_assert(!std::is_abstract<typeT>(), "-- inherits or declares at least one pure virtual function");

		// default constructor: typeT()
		static_assert(! std::is_default_constructible<typeT>(), "-- default constructor");
		static_assert(! std::is_nothrow_default_constructible<typeT>(), "-- notrow default constructor");
		static_assert(! std::is_trivially_default_constructible<typeT>(), "++ default, nothing virtual");

		// copy constructor: typeT(const typeT&)
		static_assert(std::is_copy_constructible<typeT>(), "-- copy constructor");
		static_assert(std::is_nothrow_copy_constructible<typeT>(), "-- notrow copy constructor");
		static_assert(std::is_trivially_copy_constructible<typeT>(), "-- trivially copy constructor"); // = default

		// move constructor: typeT(typeT&&)
		static_assert(std::is_move_constructible<typeT>(), "-- move constructor");
		static_assert(std::is_nothrow_move_constructible<typeT>(), "-- nothrow move constructor");
		static_assert(std::is_trivially_move_constructible<typeT>(), "-- trivially move constructor");

		// copy assingment
		static_assert(std::is_copy_assignable<typeT>(), "-- copy assignable");
		static_assert(std::is_nothrow_copy_assignable<typeT>(), "-- notrow copy assignable");
		static_assert(std::is_trivially_copy_assignable<typeT>(), "-- trivially copy assignable");

		static_assert(std::is_move_assignable<typeT>(), "-- move assignable");
		static_assert(std::is_nothrow_move_assignable<typeT>(), "-- move assignable");
		static_assert(std::is_trivially_move_assignable<typeT>(), "-- trivially move assignable");

		static_assert(std::is_destructible<typeT>(), "-- destructable");
		static_assert(std::is_nothrow_destructible<typeT>(), "-- nothrow destructable");
		static_assert(std::is_trivially_destructible<typeT>(), "-- trivially destructable");
		static_assert(!std::has_virtual_destructor<typeT>(), "-- virtual destructor");

		static_assert(std::is_swappable<typeT>(), "-- swappable");			//C++17
		static_assert(std::is_nothrow_swappable<typeT>(), "-- nothrow swappable");	//C++17
		// other types
		static_assert(! std::is_constructible<typeT, int>(), "-- should not construct from int");
		static_assert(std::is_constructible<typeT, Location<3>>(), "-- should construct from Location");
		static_assert(! std::is_constructible<typeT, Block_Loc<2>>(), "-- shouldn't accept non matching dimensions_1");
		static_assert(! std::is_constructible<typeT, Location<2>>(), "-- should not construct from non matching Location");

		static_assert(std::is_assignable<typeT, Location<3>>(), "-- assignable from Location");
		static_assert(! std::is_assignable<typeT, Block_Loc<2>>(), "-- assignable wrong size");
		static_assert(! std::is_assignable<typeT, Location<2>>(), "-- assignable Location wrong size");
		static_assert(! std::is_assignable<typeT, int>(), "-- shouldn't be assignable from int, prevent with explicit!!");

		static_assert(! std::is_swappable_with<typeT, Location<3>>(), "++");	//C++17
		static_assert(! std::is_nothrow_swappable_with<typeT, Location<3>>(), "++");	//C++17

		Block_Loc<3> B1(8, 8);
		Assert::IsTrue(B1.element() == 8, L"block element", LINE_INFO());
		Block_Loc<3> B6{0, 0, 2};
		Assert::IsTrue(B6.element() == 2, L"block row-col", LINE_INFO());
		Block_Loc<3> B2(B1);
		Assert::IsTrue(B2.element() == 8, L"Copy constructor", LINE_INFO());
		Block_Loc<3> B3 = B1;
		Assert::IsTrue(B3.element() == 8, L"Copy", LINE_INFO());
		Block_Loc<3> B4(Block_Loc<3>(0, 2));
		Assert::IsTrue(B4.element() == 2, L"Move constructor", LINE_INFO());
		Block_Loc<3> B5 = Block_Loc<3>(8, 8);
		Assert::IsTrue(B5.element() == 8, L"Move", LINE_INFO());
		// back and forth
		Assert::IsTrue(Block_Loc<3>{ Location<3>{12} }.id() == 1, L"", LINE_INFO());
		Assert::IsTrue(Location<3>{Block_Loc<3>{8, 8}}.element() == 80, L"", LINE_INFO());
	}
	TEST_METHOD(T1_information)
	{
		// size definitions
		Assert::IsTrue(Location<2>().base_size == 2, L"base_size error");
		Assert::IsTrue(Location<2>().elem_size == 4);
		Assert::IsTrue(Location<2>().full_size == 16);
		Assert::IsTrue(Location<3>().base_size == 3, L"base_size<3> error");
		Assert::IsTrue(Location<3>().elem_size == 9);
		Assert::IsTrue(Location<3>().full_size == 81);

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
	TEST_METHOD(T2_comparisson)
	{
		Assert::IsTrue(Location<3>(4) == Location<3>(4), L"", LINE_INFO());
		Assert::IsFalse(Location<3>(0) == Location<3>(13), L"", LINE_INFO());
		Assert::IsTrue(Location<3>(0, 5) < Location<3>(3, 2), L"", LINE_INFO());
		Assert::IsFalse(Location<3>(80) < Location<3>(8, 3), L"", LINE_INFO());

		Assert::IsTrue(Block_Loc<3>(0,0) == Block_Loc<3>(0,0,0), L"", LINE_INFO());
		Assert::IsFalse(Block_Loc<3>(4,3) == Block_Loc<3>(4,4), L"", LINE_INFO());
		Assert::IsTrue(Block_Loc<3>(0, 5) < Block_Loc<3>(3, 2), L"", LINE_INFO());
		Assert::IsFalse(Block_Loc<3>(0, 5) < Block_Loc<3>(0, 2), L"", LINE_INFO());
		Assert::IsFalse(Block_Loc<3>(8,2,2) < Block_Loc<3>(8, 8), L"", LINE_INFO());

		Assert::IsTrue(Block_Loc<3>(0,0) == Location<3>(0), L"", LINE_INFO());
		Assert::IsTrue(Location<3>(1) == Block_Loc<3>(0, 1), L"", LINE_INFO());
	}
	TEST_METHOD(T3_helper_Block_Loc)
	{
		Block_Loc<3> B1(2, 6);
		Assert::IsTrue(B1.element() == 6, L"Block_Loc.elem", LINE_INFO());
		Assert::IsTrue(B1.id() == 2, L"Block_loc.id", LINE_INFO());
		Location<3> L1(B1);
		Assert::IsTrue(L1.block() == B1.id());
		Assert::IsTrue(L1.block_elem() == B1.element());

		Block_Loc<3> B2(2, 2, 0);
		Assert::IsTrue(B2.id() == 2);
		Assert::IsTrue(B2.element() == 6);
		Location<3> L2(B2);
		Assert::IsTrue(L2.block() == B2.id());
		Assert::IsTrue(L2.block_elem() == B2.element());
		Assert::IsTrue(L2.block_row() == 2);
		Assert::IsTrue(L2.block_col() == 0);
	}
	TEST_METHOD(T4_is_constexpr)
	{
		// noexcept is always true for a constant expression.
		// therefor it can be used to check if a particular invocation takes the constexpr branch
		Assert::IsTrue(noexcept(Location<3>()));
		Assert::IsTrue(noexcept(Location<3>{}));
		Assert::IsTrue(noexcept(Location<3>{5}));
		Assert::IsTrue(noexcept(Location<3>{5, 3}));
		Assert::IsTrue(noexcept(Location<3>().base_size));
		Assert::IsTrue(noexcept(Location<3>().elem_size));
		Assert::IsTrue(noexcept(Location<3>().full_size));
		Assert::IsTrue(noexcept(Location<3>().element()));
		Assert::IsTrue(noexcept(Location<3>(0).element()));
		Assert::IsTrue(noexcept(Location<3>(1).element()));
		Assert::IsTrue(noexcept(Location<3>(2).element()));
		Assert::IsTrue(noexcept(Location<3>(79).element()));
		Assert::IsTrue(noexcept(Location<3>(2,7).element()));
		Assert::IsTrue(noexcept(Location<3>(79).row()));
		Assert::IsTrue(noexcept(Location<3>(79).col()));
		Assert::IsTrue(noexcept(Location<3>(79).block_row()));
		Assert::IsTrue(noexcept(Location<3>(80).block_col()));
		Assert::IsTrue(noexcept(Location<3>(80).block_elem()));
		Assert::IsTrue(noexcept(Block_Loc<3>{Location<3>(12)}));
		Assert::IsTrue(noexcept(Block_Loc<3>{5, 3}));
		Assert::IsTrue(noexcept(Block_Loc<3>{5, 3, 2}));
		Assert::IsTrue(noexcept(Block_Loc<3>{5, 3}.id()));
		Assert::IsTrue(noexcept(Block_Loc<3>{5, 3}.element()));
		Assert::IsTrue(noexcept(Block_Loc<3>{5, 3}.row()));
		Assert::IsTrue(noexcept(Block_Loc<3>{5, 3}.col()));
		Assert::IsTrue(noexcept(Location<3>(Block_Loc<3>{5, 3}).element()));
		Assert::IsTrue(noexcept(Location<3>()==Location<3>(0)));
		Assert::IsTrue(noexcept(Location<3>()==Block_Loc<3>(0,2)));
		Assert::IsTrue(noexcept(Location<3>()<Location<3>(0)));

		// not precalculated
		Location<3> L0{};
		Assert::IsFalse(noexcept(L0.element()));
		Assert::IsFalse(noexcept(L0.row()));
		Assert::IsFalse(noexcept(L0.col()));
		Assert::IsFalse(noexcept(L0.block()));
		Assert::IsFalse(noexcept(L0.block_row()));
		Assert::IsFalse(noexcept(L0.block_col()));
		Assert::IsFalse(noexcept(L0.block_elem()));
		Assert::IsFalse(noexcept(Block_Loc<3>(L0)));
		Block_Loc<3> B0{ L0 };
		Assert::IsFalse(noexcept(B0.id()));
		Assert::IsFalse(noexcept(B0.element()));
		Assert::IsFalse(noexcept(B0.row()));
		Assert::IsFalse(noexcept(B0.col()));
		Block_Loc<3> B1{ 2, 4 };
		Assert::IsFalse(noexcept(B1.id()));
		Assert::IsFalse(noexcept(B1.element()));
		Assert::IsFalse(noexcept(B1.row()));
		Assert::IsFalse(noexcept(B1.col()));

		Assert::IsTrue(noexcept(Location<2>().element()));
		Assert::IsFalse(noexcept(Location<4>().element()));
	}
	TEST_METHOD(T5_external)
	{
		Assert::IsTrue(noexcept(shared_row(Location<3>(0), Location<3>(8))));
		Assert::IsTrue(noexcept(shared_col(Location<3>(0), Location<3>(8))));
		Assert::IsTrue(noexcept(shared_block(Location<3>(0), Location<3>(8))));
		Assert::IsTrue(shared_row(Location<3>(0), Location<3>(8)));
		Assert::IsFalse(shared_row(Location<3>(9), Location<3>(8)));
		Assert::IsTrue(shared_col(Location<3>(0), Location<3>(18)));
		Assert::IsFalse(shared_col(Location<3>(9), Location<3>(8)));
		Assert::IsTrue(shared_block(Location<3>(0), Location<3>(11)));
		Assert::IsFalse(shared_block(Location<3>(9), Location<3>(8)));
	}
};
}