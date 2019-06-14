// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//===--- SudokuTests/Location.cpp                                       ---===//
//
// Unit tests for the template class Sudoku::Location
//===----------------------------------------------------------------------===//
// Implemented with GoogleTest
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Location.h>
#include <Sudoku/Location_Utilities.h>
// Helpers
#include <Sudoku/Board.h>
// additional
#include <vector>
#include <type_traits>


namespace SudokuTests::LocationTest
{
using ::Sudoku::Location;
using ::Sudoku::Location_Block;

namespace compiletime
{
	// Type properties
	using typeT = Location<3>;
	// Composite Type Categories
	static_assert(not std::is_fundamental_v<typeT>);
	static_assert(std::is_object_v<typeT>);
	static_assert(std::is_compound_v<typeT>);

	static_assert(not std::is_scalar_v<typeT>);
	static_assert(not std::is_array_v<typeT>);
	static_assert(not std::is_union_v<typeT>);
	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_reference_v<typeT>);
	static_assert(not std::is_function_v<typeT>);
	// Type Properties
	static_assert(not std::is_const_v<typeT>);
	static_assert(not std::is_volatile_v<typeT>);
	static_assert(not std::is_trivial_v<typeT>);
#if defined(__clang__) || defined(__GNUC__)
	static_assert(std::is_trivially_copyable_v<typeT>);
#if not(defined(__clang__) && __clang_major__ < 6) &&                          \
	not(defined(__APPLE__) && defined(__clang__) && __clang_major__ < 10)
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif
#else
	static_assert(not std::is_trivially_copyable_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::has_unique_object_representations_v<typeT>);
#endif // __ICL
#endif // __clang__
	static_assert(std::is_standard_layout_v<typeT>);
	static_assert(not std::is_empty_v<typeT>); // nothing virtual
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__APPLE__) && defined(__clang__) &&                            \
		(__clang_major__ < 10 ||                                               \
		 (__clang_major__ == 9 && __clang_minor__ < 1)))
	static_assert(not std::is_aggregate_v<typeT>);
#endif

	// default constructor: typeT()
	static_assert(std::is_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>);
	static_assert(std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(std::is_trivially_copy_constructible_v<typeT>); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(std::is_trivially_move_constructible_v<typeT>);

	// other types
	static_assert(std::is_constructible_v<typeT, bool>); // TODO unwanted
	static_assert(std::is_constructible_v<typeT, char>);
	static_assert(std::is_constructible_v<typeT, int>);
	static_assert(std::is_constructible_v<typeT, unsigned int>);
	static_assert(std::is_constructible_v<typeT, size_t>);
	static_assert(std::is_nothrow_constructible_v<typeT, int>);
	static_assert(std::is_nothrow_constructible_v<typeT, unsigned int>);
	static_assert(std::is_nothrow_constructible_v<typeT, size_t>);
	static_assert(not std::is_constructible_v<int, typeT>);
	static_assert(not std::is_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<size_t, typeT>);

	static_assert(std::is_constructible_v<typeT, Location_Block<3>>);
	static_assert(not std::is_constructible_v<typeT, Location_Block<2>>);

	// Implicit conversion to Location:
	static_assert(std::is_convertible_v<Location_Block<3>, typeT>);
	static_assert(not std::is_convertible_v<bool, typeT>);
	static_assert(not std::is_convertible_v<char, typeT>);
	static_assert(not std::is_convertible_v<int, typeT>);
	static_assert(not std::is_convertible_v<size_t, typeT>);
	// Implicit conversion from Location:
	static_assert(not std::is_convertible_v<typeT, Location_Block<3>>);
	static_assert(not std::is_convertible_v<typeT, bool>);
	static_assert(not std::is_convertible_v<typeT, char>);
	static_assert(not std::is_convertible_v<typeT, int>);
	static_assert(not std::is_convertible_v<typeT, size_t>);

	// copy assignment
	static_assert(not std::is_copy_assignable_v<typeT>);
	static_assert(not std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(not std::is_trivially_copy_assignable_v<typeT>);

	static_assert(not std::is_move_assignable_v<typeT>);
	static_assert(not std::is_nothrow_move_assignable_v<typeT>);
	static_assert(not std::is_trivially_move_assignable_v<typeT>);

	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<typeT, Location_Block<3>>);

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_v<typeT>);         // C++17
	static_assert(not std::is_nothrow_swappable_v<typeT>); // C++17

	static_assert(not std::is_swappable_with_v<typeT, Location_Block<3>>);
	static_assert(
		not std::is_nothrow_swappable_with_v<typeT, Location_Block<3>>);
#endif // __ICL

	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(std::is_trivially_destructible_v<typeT>);
	static_assert(not std::has_virtual_destructor_v<typeT>);
} // namespace compiletime

namespace Location_Block_compiletime
{
	using typeT = Location_Block<3>;
	// Composite Type Categories
	static_assert(not std::is_fundamental_v<typeT>);
	static_assert(std::is_object_v<typeT>);
	static_assert(std::is_compound_v<typeT>);

	static_assert(not std::is_scalar_v<typeT>);
	static_assert(not std::is_array_v<typeT>);
	static_assert(not std::is_union_v<typeT>);
	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_reference_v<typeT>);
	static_assert(not std::is_function_v<typeT>);
	// Type Properties
	static_assert(not std::is_const_v<typeT>);
	static_assert(not std::is_volatile_v<typeT>);
	static_assert(not std::is_trivial_v<typeT>);
#if defined(__GNUC__) && !defined(__clang__)
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::has_unique_object_representations_v<typeT>);
#else
	static_assert(not std::is_trivially_copyable_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6) &&                          \
	not(defined(__APPLE__) && defined(__clang__) && __clang_major__ < 10)
	static_assert(not std::has_unique_object_representations_v<typeT>);
#endif // __ICL && ! clang before 6.0
#endif // __GNUC__
	static_assert(std::is_standard_layout_v<typeT>);
	static_assert(not std::is_empty_v<typeT>); // nothing virtual
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__APPLE__) && defined(__clang__) &&                            \
		(__clang_major__ < 10 ||                                               \
		 (__clang_major__ == 9 && __clang_minor__ < 1)))
	static_assert(not std::is_aggregate_v<typeT>);
#endif

	// default constructor: typeT()
	static_assert(std::is_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>);
	static_assert(std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(std::is_trivially_copy_constructible_v<typeT>); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(std::is_trivially_move_constructible_v<typeT>);

	// copy assignment
	static_assert(not std::is_copy_assignable_v<typeT>);
	static_assert(not std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(not std::is_trivially_copy_assignable_v<typeT>);

	static_assert(not std::is_move_assignable_v<typeT>);
	static_assert(not std::is_nothrow_move_assignable_v<typeT>);
	static_assert(not std::is_trivially_move_assignable_v<typeT>);

	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(std::is_trivially_destructible_v<typeT>);
	static_assert(not std::has_virtual_destructor_v<typeT>);

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_v<typeT>);         // C++17
	static_assert(not std::is_nothrow_swappable_v<typeT>); // C++17
#endif                                                     // __ICL
	// other types
	static_assert(not std::is_constructible_v<typeT, int>);
	static_assert(std::is_constructible_v<typeT, Location<3>>);
	static_assert(not std::is_constructible_v<typeT, Location_Block<2>>);
	static_assert(not std::is_constructible_v<typeT, Location<2>>);

	static_assert(not std::is_assignable_v<typeT, Location<3>>);
	static_assert(not std::is_assignable_v<typeT, Location_Block<2>>);
	static_assert(not std::is_assignable_v<typeT, Location<2>>);
	static_assert(not std::is_assignable_v<typeT, int>);
} // namespace Location_Block_compiletime

// Shared data used for all tests
struct LocationTest
{
	const Location<3> locE{};
	const Location<3> loc0{0};
	const Location<3> loc1{1};
	const Location<3> loc52{52};
};

TEST(Location, Construction)
{
	static_assert(noexcept(Location<3>()));
	static_assert(noexcept(Location<3>{}));
	static_assert(noexcept(Location<2>()));
	EXPECT_NO_THROW(Location<3>());
	EXPECT_NO_THROW(Location<3>{});
	EXPECT_NO_THROW([[maybe_unused]] Location<3> LL2);
	EXPECT_NO_THROW([[maybe_unused]] Location<2> LL3);

	static_assert(noexcept(Location<3>(-171)));
	static_assert(noexcept(Location<3>(-1))); // expected when past the begin
	static_assert(noexcept(Location<3>(0)));  // begin
	static_assert(noexcept(Location<3>(1)));
	static_assert(noexcept(Location<3>(12)));
	static_assert(noexcept(Location<3>(80))); // end
	static_assert(noexcept(Location<3>(81))); // expected when past the end
	static_assert(noexcept(Location<3>(92))); // two rows past the end
	static_assert(noexcept(Location<3>(1091)));
	EXPECT_NO_THROW(Location<3>(12));
	EXPECT_NO_THROW([[maybe_unused]] Location<3> L1(12));

	static_assert(noexcept(Location<3>(-51, 7)));
	static_assert(noexcept(Location<3>(1, -83)));
	static_assert(noexcept(Location<3>(-1, 7)));
	static_assert(noexcept(Location<3>(1, -1)));
	static_assert(noexcept(Location<3>(0, 0)));
	static_assert(noexcept(Location<3>(0, 1)));
	static_assert(noexcept(Location<3>(1, 0)));
	static_assert(noexcept(Location<3>(8, 1)));
	static_assert(noexcept(Location<3>(1, 8)));
	static_assert(noexcept(Location<3>(9, 1)));
	static_assert(noexcept(Location<3>(1, 9)));
	static_assert(noexcept(Location<3>(90, 1)));
	static_assert(noexcept(Location<3>(1, 71)));
	EXPECT_NO_THROW(Location<3>(1, 8));
	EXPECT_NO_THROW([[maybe_unused]] Location<3> L2(1, 8));

	// move construct
	EXPECT_NO_THROW([[maybe_unused]] auto L3(Location<3>(6)));
	static_assert(noexcept(Location<3>{Location<3>(6)}));
	// copy construct
	constexpr Location<3> c1(12);
	static_assert(noexcept(Location<3>(c1)));
	EXPECT_NO_THROW([[maybe_unused]] Location<3> c3(c1));
}

TEST(Location, Construction_result)
{ // depends on Location::element()
	static_assert(noexcept(Location<3>(12).element()));
	static_assert(Location<3>(12).element() == 12);
	EXPECT_EQ(Location<3>(12).element(), 12);
	static_assert(noexcept(Location<3>{51}.element()));
	static_assert(Location<3>{12}.element() == 12);
	EXPECT_EQ(Location<3>{12}.element(), 12);

	static_assert(Location<3>(1, 8).element() == 17);
	static_assert(Location<3>{1, 8}.element() == 17);
	EXPECT_EQ(Location<3>(1, 8).element(), 17);

	static_assert(Location<3>{Location<3>(6)}.element() == 6);
	EXPECT_EQ(Location<3>{Location<3>(6)}.element(), 6);
	static_assert(Location<3>{Location<3>{6}}.element() == 6);
	EXPECT_EQ(Location<3>{Location<3>{6}}.element(), 6);

	[[maybe_unused]] const Location<3> L1(12);
	[[maybe_unused]] constexpr Location<3> c1(12);
	[[maybe_unused]] const Location<3> L2(1, 8);
	[[maybe_unused]] constexpr Location<3> c2(1, 8);
	EXPECT_EQ(L1.element(), 12);
	static_assert(c1.element() == 12);
	const Location<3> L3(Location<3>(6));
	constexpr Location<3> c3(Location<3>(6));
	EXPECT_EQ(L3.element(), 6);
	EXPECT_EQ(c3.element(), 6);
	static_assert(c3.element() == 6);

	// copy construct
	EXPECT_EQ(Location<3>(L3).element(), 6);
	static_assert(Location<3>(c3).element() == 6);
	EXPECT_EQ(Location<3>{L3}.element(), 6);
	static_assert(Location<3>{c3}.element() == 6);
}

TEST(Location, Construction_Block)
{ // depends on Location
	static_assert(noexcept(Location_Block<3>()));
	static_assert(noexcept(Location_Block<3>{}));
	EXPECT_NO_THROW([[maybe_unused]] Location_Block<3> L1);
	EXPECT_NO_THROW([[maybe_unused]] Location_Block<3> LL1);
	static_assert(noexcept(Location_Block<3>(Location<3>())));
	static_assert(noexcept(Location_Block<2>(Location<2>())));
	static_assert(noexcept(Location_Block<3>(Location<3>(-31))));
	static_assert(noexcept(Location_Block<3>(Location<3>(-1))));
	static_assert(noexcept(Location_Block<3>(Location<3>(0))));
	static_assert(noexcept(Location_Block<3>(Location<3>(12))));
	static_assert(noexcept(Location_Block<3>(Location<3>(80))));
	static_assert(noexcept(Location_Block<3>(Location<3>(81))));
	static_assert(noexcept(Location_Block<3>(Location<3>(981))));
	EXPECT_NO_THROW([[maybe_unused]] Location_Block<3> L2(Location<3>{}));
	static_assert(noexcept(Location_Block<3>(-1, 0)));
	static_assert(noexcept(Location_Block<3>(0, -1)));
	static_assert(noexcept(Location_Block<3>(0, 0)));
	static_assert(noexcept(Location_Block<3>(8, 0)));
	static_assert(noexcept(Location_Block<3>(8, 8)));
	static_assert(noexcept(Location_Block<3>(0, 8)));
	static_assert(noexcept(Location_Block<3>(9, 0)));
	static_assert(noexcept(Location_Block<3>(9, 9)));
	static_assert(noexcept(Location_Block<3>(0, 9)));
	static_assert(noexcept(Location_Block<3>(992, 9)));
	EXPECT_NO_THROW([[maybe_unused]] Location_Block<3> L3(0, 0));
	static_assert(noexcept(Location_Block<3>(-1, 0, 0)));
	static_assert(noexcept(Location_Block<3>(0, 0, 0)));
	static_assert(noexcept(Location_Block<3>(0, 2, 0)));
	static_assert(noexcept(Location_Block<3>(0, 2, 2)));
	static_assert(noexcept(Location_Block<3>(0, 0, 2)));
	static_assert(noexcept(Location_Block<3>(8, 0, 0)));
	static_assert(noexcept(Location_Block<3>(8, 2, 0)));
	static_assert(noexcept(Location_Block<3>(8, 2, 2)));
	static_assert(noexcept(Location_Block<3>(8, 0, 2)));
	static_assert(noexcept(Location_Block<3>(9, 0, 0)));
	static_assert(noexcept(Location_Block<3>(5, 0, 3)));
	static_assert(noexcept(Location_Block<3>(5, 3, 3)));
	static_assert(noexcept(Location_Block<3>(5, 3, 0)));
	EXPECT_NO_THROW([[maybe_unused]] Location_Block<3> L4(0, 0, 0));

	// move construct
	static_assert(noexcept(Location_Block<3>{Location_Block<3>()}));

	// copy construct
	constexpr Location_Block<3> c1{1, 3};
	static_assert(noexcept(Location_Block<3>(c1)));
	EXPECT_NO_THROW([[maybe_unused]] Location_Block<3> c2(c1));

	static_assert(noexcept(Location_Block<3>{Location<3>{12}}));
	static_assert(noexcept(Location<3>{Location_Block<3>{4, 2}}));
	ASSERT_NO_THROW([[maybe_unused]] Location<3> X(Location_Block<3>(1, 3)));
}

TEST(Location, Construction_result_Block)
{ // check if initialized to correct value
	static_assert(noexcept(Location_Block<3>(1, 4)));
	static_assert(noexcept(Location_Block<3>(1, 4).element()));
	static_assert(Location_Block<3>(1, 4).element() == 4);
	EXPECT_EQ(Location_Block<3>(8, 8).element(), 8);
	static_assert(noexcept(Location_Block<3>{2, 3}));
	static_assert(Location_Block<3>{2, 3}.element() == 3);
	EXPECT_EQ((Location_Block<3>{8, 8}.element()), 8);
	constexpr Location_Block<3> c1{3, 7};
	static_assert(noexcept(c1.element()));
	static_assert(c1.element() == 7);
	const Location_Block<3> B1(7, 8);
	EXPECT_EQ(B1.element(), 8);

	static_assert(Location_Block<3>{1, 2, 0}.element() == 6);
	EXPECT_EQ((Location_Block<3>{0, 0, 2}.element()), 2);
	static_assert(Location_Block<3>{6, 1, 1}.element() == 4);
	EXPECT_EQ(Location_Block<3>(0, 0, 2).element(), 2);
	constexpr Location_Block<3> c2{2, 0, 1};
	static_assert(c2.element() == 1);
	EXPECT_EQ(c2.element(), 1);
	const Location_Block<3> B6{0, 0, 2};
	EXPECT_EQ(B6.element(), 2);

	static_assert(Location_Block<3>{Location_Block<3>(0, 2)}.element() == 2);
	const Location_Block<3> B4(Location_Block<3>(0, 2));
	EXPECT_EQ(B4.element(), 2) << "Move constructor";
	EXPECT_EQ((Location_Block<3>{Location_Block<3>{0, 2}}.element()), 2);
	EXPECT_EQ(Location_Block<3>{Location_Block<3>(0, 2)}.element(), 2);

	static_assert(Location_Block<3>(c1).element() == 7);
	static_assert(Location_Block<3>{c2}.element() == 1);
	constexpr Location_Block<3> c3{c1};
	static_assert(c3.element() == 7);
	Location_Block<3> B2(B1);
	EXPECT_EQ(B2.element(), 8) << "Copy constructor";
	EXPECT_EQ(Location_Block<3>(B1).element(), 8) << "Copy constructor";
	EXPECT_EQ(Location_Block<3>{B1}.element(), 8) << "Copy constructor";
	const Location_Block<3> B3 = B1;
	EXPECT_EQ(B3.element(), 8) << "Copy";
	const Location_Block<3> B5 = Location_Block<3>(8, 8);
	EXPECT_EQ(B5.element(), 8) << "Move";

	// back and forth
	static_assert(Location_Block<3>(Location<3>{12}).element() == 3);
	static_assert(Location<3>(Location_Block<3>{1, 3}).element() == 12);
	EXPECT_EQ(Location_Block<3>{Location<3>{12}}.id(), 1);
	EXPECT_EQ((Location<3>{Location_Block<3>{8, 8}}.element()), 80);
	EXPECT_EQ(Location<3>(Location_Block<3>{8, 8}).element(), 80);
	EXPECT_EQ(Location<3>{Location_Block<3>(8, 8)}.element(), 80);
}

// clang-format off
template<int N>
void SubProperties(Location<N> loc,
	int elem,	int row,	int col,
	int block,	int b_elem,	int b_row,	int b_col)
{
	EXPECT_EQ(loc.element(),	elem);
	EXPECT_EQ(loc.row(),		row);
	EXPECT_EQ(loc.col(),		col);
	EXPECT_EQ(loc.block(),		block);
	EXPECT_EQ(loc.block_elem(),	b_elem);
	EXPECT_EQ(loc.block_row(),	b_row);
	EXPECT_EQ(loc.block_col(),	b_col);
}
// clang-format on
TEST(Location, Properties)
{
	using Loc = Location<3>;

	ASSERT_EQ(Loc{36}.element(), 36);
	static_assert(noexcept(Loc{-1}.element()));
	static_assert(noexcept(Loc{0}.element()));
	static_assert(noexcept(Loc{1}.element()));
	static_assert(noexcept(Loc{80}.element()));
	static_assert(noexcept(Loc{81}.element()));
	static_assert(Loc{-1}.element() == -1);
	static_assert(Loc{0}.element() == 0);
	static_assert(Loc{8}.element() == 8);
	static_assert(Loc{36}.element() == 36);
	static_assert(Loc{80}.element() == 80);
	static_assert(Loc{81}.element() == 81);
	// sanity check: operator==(int, int)
	static_assert(not(Loc{36}.element() == 35));
	static_assert(!(Loc{36}.element() == 35));

	EXPECT_EQ(Loc{36}.row(), 4);
	static_assert(noexcept(Loc{-1}.row()));
	static_assert(noexcept(Loc{0}.row()));
	static_assert(noexcept(Loc{18}.row()));
	static_assert(noexcept(Loc{80}.row()));
	static_assert(noexcept(Loc{81}.row()));
	static_assert(Loc{-9}.row() == -1);
	static_assert(Loc{-8}.row() == -0);
	static_assert(Loc{-1}.row() == -0);
	static_assert(Loc{-1}.row() == 0); // tricky
	static_assert(Loc{0}.row() == 0);
	static_assert(Loc{8}.row() == 0);
	static_assert(Loc{9}.row() == 1);
	static_assert(Loc{36}.row() == 4);
	static_assert(Loc{80}.row() == 8);
	static_assert(Loc{81}.row() == 9);

	EXPECT_EQ(Loc{36}.col(), 0);
	static_assert(noexcept(Loc{-1}.col()));
	static_assert(noexcept(Loc{0}.col()));
	static_assert(noexcept(Loc{18}.col()));
	static_assert(noexcept(Loc{80}.col()));
	static_assert(noexcept(Loc{81}.col()));
	static_assert(Loc{-9}.col() == -0);
	static_assert(Loc{-8}.col() == -8);
	static_assert(Loc{-1}.col() == -1);
	static_assert(Loc{0}.col() == 0);
	static_assert(Loc{1}.col() == 1);
	static_assert(Loc{8}.col() == 8);
	static_assert(Loc{9}.col() == 0);
	static_assert(Loc{33}.col() == 6);
	static_assert(Loc{80}.col() == 8);
	static_assert(Loc{81}.col() == 0);

	EXPECT_EQ(Loc{36}.block(), 3);
	static_assert(noexcept(Loc{-1}.block()));
	static_assert(noexcept(Loc{0}.block()));
	static_assert(noexcept(Loc{18}.block()));
	static_assert(noexcept(Loc{80}.block()));
	static_assert(noexcept(Loc{81}.block()));
	static_assert(Loc{-18}.block() == -0);
	static_assert(Loc{-8}.block() == -2);
	static_assert(Loc{-1}.block() == -0);
	static_assert(Loc{-1}.block() == 0); // tricky
	static_assert(Loc{0}.block() == 0);
	static_assert(Loc{2}.block() == 0);
	static_assert(Loc{3}.block() == 1);
	static_assert(Loc{8}.block() == 2);
	static_assert(Loc{18}.block() == 0);
	static_assert(Loc{36}.block() == 3);
	static_assert(Loc{80}.block() == 8);
	static_assert(Loc{81}.block() == 9);

	EXPECT_EQ(Loc{36}.block_row(), 1);
	EXPECT_EQ(Loc{36}.block_col(), 0);
	EXPECT_EQ(Loc{36}.block_elem(), 3);
	static_assert(noexcept(Loc{-1}.block_row()));
	static_assert(noexcept(Loc{0}.block_row()));
	static_assert(noexcept(Loc{18}.block_row()));
	static_assert(noexcept(Loc{80}.block_row()));
	static_assert(noexcept(Loc{81}.block_row()));

	static_assert(noexcept(Loc{-1}.block_col()));
	static_assert(noexcept(Loc{0}.block_col()));
	static_assert(noexcept(Loc{18}.block_col()));
	static_assert(noexcept(Loc{80}.block_col()));
	static_assert(noexcept(Loc{81}.block_col()));

	static_assert(noexcept(Loc{-1}.block_elem()));
	static_assert(noexcept(Loc{0}.block_elem()));
	static_assert(noexcept(Loc{18}.block_elem()));
	static_assert(noexcept(Loc{80}.block_elem()));
	static_assert(noexcept(Loc{81}.block_elem()));

	static_assert(Loc{-1}.block_row() == 0);
	static_assert(Loc{-1}.block_col() == -1);
	static_assert(Loc{-1}.block_elem() == -1);
	static_assert(Loc{-18}.block() == -0); //
	static_assert(Loc{-8}.block() == -2);  //
	static_assert(Loc{-1}.block() == -0);  //
	static_assert(Loc{-1}.block() == 0);   // tricky
	static_assert(Loc{0}.block_row() == 0);
	static_assert(Loc{0}.block_col() == 0);
	static_assert(Loc{0}.block_elem() == 0);
	static_assert(Loc{2}.block_row() == 0);
	static_assert(Loc{2}.block_col() == 2);
	static_assert(Loc{2}.block_elem() == 2);
	static_assert(Loc{38}.block_row() == 1);
	static_assert(Loc{38}.block_col() == 2);
	static_assert(Loc{38}.block_elem() == 5);
	static_assert(Loc{80}.block_row() == 2);
	static_assert(Loc{80}.block_col() == 2);
	static_assert(Loc{80}.block_elem() == 8);

	// sanity check: inverse construction
	static_assert(Loc(1, 2).row() == 1);
	static_assert(Loc(1, 2).col() == 2);
	static_assert(Loc(8, 7).row() == 8);
	static_assert(Loc(7, 8).col() == 8);
	static_assert(Loc(-1, 2).row() == 0);  // tricky
	static_assert(Loc(-2, 2).row() == -1); //
	static_assert(Loc(-3, 2).row() == -2); //
	static_assert(Loc(1, -1).col() == 8);
	// TODO consider throwing when a row() is requested for a negative id_
	// This would effect: block(); block_row(); block_elem()
	// And form Location_BLock: id(); element(); row()
	{
		SCOPED_TRACE("Location<3> loc{ 52 }");
		SubProperties(Location<3>{52}, 52, 5, 7, 5, 7, 2, 1);
	}
	{
		SCOPED_TRACE("Location<3> loc(3, 2)");
		SubProperties(Location<3>(3, 2), 29, 3, 2, 3, 2, 0, 2);
	}
	{
		SCOPED_TRACE("Location<3> loc(0)");
		SubProperties(Location<3>(0), 0, 0, 0, 0, 0, 0, 0);
	}
	{
		SCOPED_TRACE("Location<3> loc(80)");
		SubProperties(Location<3>(80), 80, 8, 8, 8, 8, 2, 2);
	}
	{
		SCOPED_TRACE("Location<2> loc{}");
		SubProperties(Location<2>{}, 0, 0, 0, 0, 0, 0, 0);
	}
	{
		SCOPED_TRACE("Location<2> loc{ 15 }");
		SubProperties(Location<2>{15}, 15, 3, 3, 3, 3, 1, 1);
	}
	{ // out of bounds
		SCOPED_TRACE("Location<2> loc{ 16 }");
		SubProperties(Location<2>{16}, 16, 4, 0, 4, 0, 0, 0);
	}
	{
		SCOPED_TRACE("Location<3> loc{ 100 }");
		SubProperties(Location<3>{100}, 100, 11, 1, 9, 7, 2, 1);
	}
}

TEST(Location, Properties_Block)
{
	using LB = Location_Block<3>;

	// Location_Block::id()
	EXPECT_EQ(LB(2, 6).id(), 2);
	static_assert(noexcept(LB(-1, 4).id()));
	static_assert(noexcept(LB(0, -1).id()));
	static_assert(noexcept(LB(0, 0).id()));
	static_assert(noexcept(LB(1, 4).id()));
	static_assert(noexcept(LB(8, 8).id()));
	static_assert(noexcept(LB(9, 8).id()));
	static_assert(noexcept(LB(8, 9).id()));
	static_assert(LB(-1, 4).id() == 2); // tricky
	static_assert(LB(0, -1).id() == 0); //
	static_assert(LB(0, 0).id() == 0);
	static_assert(LB(1, 4).id() == 1);
	static_assert(LB(8, 8).id() == 8);
	static_assert(LB(9, 8).id() == 9);
	static_assert(LB(8, 9).id() == 11); // tricky

	// Location_Block::element()
	EXPECT_EQ(LB(2, 6).element(), 6);
	static_assert(noexcept(LB(-1, 4).element()));
	static_assert(noexcept(LB(0, -1).element()));
	static_assert(noexcept(LB(0, 0).element()));
	static_assert(noexcept(LB(1, 4).element()));
	static_assert(noexcept(LB(8, 8).element()));
	static_assert(noexcept(LB(9, 8).element()));
	static_assert(noexcept(LB(8, 9).element()));
	static_assert(LB(-1, 0).element() == 0);
	static_assert(LB(-1, 1).element() == -2); // tricky
	static_assert(LB(-1, 2).element() == -1); //
	static_assert(LB(-1, 3).element() == 0);  //
	static_assert(LB(-1, 4).element() == 1);  //
	static_assert(LB(0, -1).element() == -1);
	static_assert(LB(0, -5).element() == -5);
	static_assert(LB(0, 0).element() == 0);
	static_assert(LB(1, 4).element() == 4);
	static_assert(LB(8, 8).element() == 8);
	static_assert(LB(9, 8).element() == 8);
	static_assert(LB(8, 9).element() == 0); // overflow

	// Location_Block::row()
	EXPECT_EQ(LB(2, 6).row(), 2);
	EXPECT_EQ(LB(2, 2, 1).row(), 2);
	static_assert(noexcept(LB(1, 4).row()));
	static_assert(noexcept(LB(-1, 4).row()));
	static_assert(noexcept(LB(0, -1).row()));
	static_assert(noexcept(LB(0, 0).row()));
	static_assert(noexcept(LB(1, 4).row()));
	static_assert(noexcept(LB(8, 8).row()));
	static_assert(noexcept(LB(9, 8).row()));
	static_assert(noexcept(LB(8, 9).row()));
	static_assert(LB(-3, 0, 0).row() == 0);
	static_assert(LB(-1, 0, 0).row() == 0);
	static_assert(LB(-1, 1, 0).row() == 0); // tricky
	static_assert(LB(-1, 2, 0).row() == 1); //
	static_assert(LB(-1, 2, 2).row() == 1); //
	static_assert(LB(0, -1, 0).row() == -1);
	static_assert(LB(0, -2, 0).row() == -2);
	static_assert(LB(0, -1, -1).row() == -1);
	static_assert(LB(0, 0, -1).row() == 0); // tricky
	static_assert(LB(0, 0, 0).row() == 0);
	static_assert(LB(0, 0, 2).row() == 0);
	static_assert(LB(0, 0, 3).row() == 1);  // overflow
	static_assert(LB(0, 2, -1).row() == 1); //
	static_assert(LB(0, 2, 0).row() == 2);
	static_assert(LB(0, 2, 2).row() == 2);
	static_assert(LB(0, 2, 3).row() == 0); // overflow
	static_assert(LB(0, 3, 0).row() == 0); //

	// Location_Block::col()
	EXPECT_EQ(LB(2, 6).col(), 0);
	static_assert(noexcept(LB(1, 4).col()));
	static_assert(noexcept(LB(1, 4).col()));
	static_assert(noexcept(LB(-1, 4).col()));
	static_assert(noexcept(LB(0, -1).col()));
	static_assert(noexcept(LB(0, 0).col()));
	static_assert(noexcept(LB(1, 4).col()));
	static_assert(noexcept(LB(8, 8).col()));
	static_assert(noexcept(LB(9, 8).col()));
	static_assert(noexcept(LB(8, 9).col()));
	static_assert(LB(-3, 0, 0).col() == 0);
	static_assert(LB(-3, 0, 1).col() == -2); // tricky
	static_assert(LB(-3, 0, 2).col() == -1); // tricky
	static_assert(LB(-1, 0, 0).col() == 0);
	static_assert(LB(-1, 1, 0).col() == 0);
	static_assert(LB(-1, 2, -1).col() == 2); // overflow
	static_assert(LB(-1, 2, 0).col() == 0);
	static_assert(LB(-1, 2, 2).col() == 2);
	static_assert(LB(-1, 2, 3).col() == 0); // overflow
	static_assert(LB(0, -1, 0).col() == 0);
	static_assert(LB(0, -2, 0).col() == 0);
	static_assert(LB(0, -1, -1).col() == -1); // tricky
	static_assert(LB(0, -1, 2).col() == -1);  // tricky
	static_assert(LB(0, -1, 3).col() == 0);   // tricky
	static_assert(LB(0, 0, -1).col() == -1);  // tricky
	static_assert(LB(0, 0, 0).col() == 0);
	static_assert(LB(0, 0, 2).col() == 2);
	static_assert(LB(0, 0, 3).col() == 0);  // overflow
	static_assert(LB(0, 2, -1).col() == 2); // overflow
	static_assert(LB(0, 2, 0).col() == 0);
	static_assert(LB(0, 2, 2).col() == 2);
	static_assert(LB(0, 2, 3).col() == 0); // overflow
	static_assert(LB(0, 3, 0).col() == 0);

	const LB B1(2, 6);
	EXPECT_EQ(B1.id(), 2);
	EXPECT_EQ(B1.element(), 6);
	EXPECT_EQ(B1.row(), 2);
	EXPECT_EQ(B1.col(), 0);

	const Location<3> L1(B1);
	EXPECT_EQ(L1.block(), B1.id());
	EXPECT_EQ(L1.block_elem(), B1.element());
	EXPECT_EQ(L1.block_row(), B1.row());
	EXPECT_EQ(L1.block_col(), B1.col());

	const LB B2(2, 1, 0);
	EXPECT_EQ(B2.id(), 2);
	EXPECT_EQ(B2.element(), 3);
	EXPECT_EQ(B2.row(), 1);
	EXPECT_EQ(B2.col(), 0);

	const Location<3> L2(B2);
	EXPECT_EQ(L2.block(), B2.id());
	EXPECT_EQ(L2.block_elem(), B2.element());
	EXPECT_EQ(L2.block_row(), B2.row());
	EXPECT_EQ(L2.block_col(), B2.col());
}

TEST(Location, Comparisson)
{
	using Loc = Location<3>;

	// operator==(Location, Location)
	static_assert(noexcept(Loc(1) == Loc(3)));
	static_assert(noexcept(Loc(1) == Loc(-3)));
	static_assert(noexcept(Loc(0) == Loc(94)));
	static_assert(noexcept(Loc(0) == Loc(-94)));
	static_assert(noexcept(Loc(1, 4) == Loc(3)));
	EXPECT_EQ(Loc(4), Loc(4)) << "verifies a == b";
	static_assert(Loc(4) == Loc{4});
	static_assert(Loc(0, 0) == Loc{0});
	static_assert(Loc(29) == Loc{3, 2});
	static_assert(Loc(3, 2) == Loc{3, 2});
	static_assert(not(Loc(14) == Loc{0}));
	static_assert(not(Loc(1) == Loc(12)));
	static_assert(not(Loc(1, 0) == Loc(12)));
	static_assert(not(Loc(1, 0) == Loc(1, 2)));
	EXPECT_FALSE(Loc(0) == Loc(13));

	// operator!=(Location, Location)
	static_assert(noexcept(Loc(1) != Loc(2)));
	static_assert(noexcept(Loc(1) != Loc(1)));
	static_assert(Loc(1) != Loc(2));
	static_assert(not(Loc(1) != Loc(1)));
	static_assert(Loc(0) != Loc(81));
	EXPECT_NE(Loc(0), Loc(13)) << "verifies a != b";

	// operator<(Location, Location)
	static_assert(noexcept(Loc(1) < Loc(3)));
	static_assert(noexcept(Loc(1) < Loc(1)));
	static_assert(noexcept(Loc(1, 0) < Loc(1, 8)));
	static_assert(noexcept(Loc(8, 0) < Loc(1, 8)));
	static_assert(Loc(1) < Loc(3));
	static_assert(Loc(8) < Loc(72));
	static_assert(Loc(-1) < Loc(0));
	static_assert(not(Loc(0) < Loc(0)));
	static_assert(not(Loc(80) < Loc(80)));
	static_assert(not(Loc(80) < Loc(0)));
	EXPECT_LT(Loc(0, 5), Loc(3, 2)) << "verifies a < b";
	EXPECT_FALSE(Loc(80) < Loc(8, 3));

	// operator<=(Location, Location)
	static_assert(noexcept(Loc(1) <= Loc(-3)));
	static_assert(noexcept(Loc(1) <= Loc(3)));
	static_assert(noexcept(Loc(3) <= Loc(3)));
	static_assert(noexcept(Loc(18) <= Loc(2)));
	static_assert(noexcept(Loc(112) <= Loc(92)));
	static_assert(Loc(1) <= Loc(3));
	static_assert(Loc(3) <= Loc(3));
	static_assert(not(Loc(4) <= Loc(3)));
	EXPECT_LE(Loc(3), Loc(4)) << "verifies a <= b";
	EXPECT_LE(Loc(4), Loc(4, 0)) << "verifies a <= b";
	EXPECT_FALSE(Loc(80) <= Loc(0));

	// operator>=(Location, Location)
	static_assert(noexcept(Loc(1) >= Loc(-3)));
	static_assert(noexcept(Loc(1) >= Loc(3)));
	static_assert(noexcept(Loc(3) >= Loc(3)));
	static_assert(noexcept(Loc(18) >= Loc(2)));
	static_assert(noexcept(Loc(112) >= Loc(92)));
	static_assert(Loc(4) >= Loc(3));
	static_assert(Loc(5) >= Loc(5));
	static_assert(not(Loc(0) >= Loc(8)));
	EXPECT_GE(Loc(5), Loc(4)) << "verifies a >= b";
	EXPECT_GE(Loc(3), Loc(3)) << "verifies a >= b";
	EXPECT_FALSE(Loc(8) >= Loc(19));

	// operator>(Location, Location)
	static_assert(noexcept(Loc(1) > Loc(-3)));
	static_assert(noexcept(Loc(1) > Loc(3)));
	static_assert(noexcept(Loc(3) > Loc(3)));
	static_assert(noexcept(Loc(18) > Loc(2)));
	static_assert(noexcept(Loc(112) > Loc(92)));
	static_assert(Loc(4) > Loc(3));
	static_assert(not(Loc(5) > Loc(5)));
	static_assert(not(Loc(0) > Loc(8)));
	EXPECT_GT(Loc(12), Loc(9)) << "verifies a > b";
	EXPECT_FALSE(Loc(8) > Loc(19));
}

TEST(Location, Comparisson_Block)
{
	using LB = Location_Block<3>;

	// operator==(Location_Block, Location_Block)
	static_assert(noexcept(LB(3, 0) == LB(3, 0)));
	static_assert(noexcept(LB(-1, 0) == LB(-1, 0)));
	static_assert(noexcept(LB(-1, 0) == LB(0, 0)));
	static_assert(noexcept(LB(1, 0) == LB(-3, 0)));
	static_assert(noexcept(LB(0, 0) == LB(94, 0)));
	static_assert(noexcept(LB(0, 0) == LB(-94, 1)));
	static_assert(noexcept(LB(1, 4) == LB(1, 1, 1)));
	EXPECT_EQ(LB(4, 0), LB(4, 0));
	static_assert(LB(4, 2) == LB{4, 2});
	static_assert(LB(3, 0) == LB(0, 9));
	static_assert(not(LB(12, 0) == LB{1, 0}));
	EXPECT_FALSE(LB(0, 1) == LB(3, 1));
	static_assert(LB(2, 7) == LB(2, 2, 1));
	EXPECT_EQ(LB(0, 0), LB(0, 0, 0));

	// operator!=(Location_Block, Location_Block)
	static_assert(noexcept(LB(1, 1) != LB(2, 1)));
	static_assert(noexcept(LB(1, 1) != LB(1, 1)));
	static_assert(LB(1, 2) != LB(2, 1));
	static_assert(not(LB(0, 1) != LB(0, 0, 1)));
	static_assert(LB(0, 0) != LB(8, 8));
	EXPECT_NE(LB(0, 0), LB(0, 13));
	EXPECT_FALSE(LB(4, 4) != LB(4, 4));

	// operator<(Location_Block, Location_Block)
	static_assert(noexcept(LB(0, 5) < LB(1, 8)));
	static_assert(noexcept(LB(0, 5) < LB(0, 5)));
	static_assert(noexcept(LB(-1, 0) < LB(0, 0)));
	static_assert(noexcept(LB(1, -1) < LB(0, 0)));
	static_assert(noexcept(LB(1, 1) < LB(-1, 0)));
	static_assert(noexcept(LB(1, 1) < LB(1, -1)));
	static_assert(LB(-1, 0) < LB(0, 0));
	static_assert(LB(0, -1) < LB(0, 0));
	static_assert(LB(0, 0) < LB(0, 1));
	static_assert(not(LB(0, 1) < LB(0, 1)));
	static_assert(not(LB(0, 1) < LB(0, 0)));
	static_assert(LB(0, 0) < LB(1, 0));
	static_assert(not(LB(1, 0) < LB(1, 0)));
	static_assert(not(LB(1, 0) < LB(0, 0)));
	static_assert(LB(0, 0) < LB(8, 8));
	static_assert(LB(0, 0) < LB(9, 0));
	static_assert(LB(1, 0) < LB(0, 9));
	static_assert(LB(1, 1, 2) < LB(2, 2, 2));
	static_assert(not(LB(2, 2, 2) < LB(1, 1, 2)));
	static_assert(LB(1, 2, 1) < LB(2, 1, 2));
	static_assert(not(LB(2, 1, 2) < LB(1, 2, 1)));
	static_assert(not(LB(3, 0) < LB(0, 9)));
	static_assert(not(LB(0, 9) < LB(3, 0)));
	static_assert(not(LB(1, 0) < LB(0, 8)));
	static_assert(LB(1, 0) < LB(0, 9));
	EXPECT_LT(LB(0, 5), LB(3, 2)) << "verifies a < b";
	EXPECT_FALSE(LB(0, 5) < LB(0, 2));
	EXPECT_FALSE(LB(8, 2, 2) < LB(8, 8));

	// operator<=
	static_assert(noexcept(LB(8, 3) <= LB(7, 3)));
	static_assert(noexcept(LB(7, 3) <= LB(7, 3)));
	static_assert(not(LB(8, 3) <= LB(7, 3)));
	static_assert(not(LB(7, 3) <= LB(7, 2)));
	static_assert(LB(7, 3) <= LB(7, 3));
	static_assert(LB(7, 3) <= LB(7, 4));
	static_assert(LB(6, 4) <= LB(7, 4));

	// operator>=
	static_assert(noexcept(LB(8, 3) >= LB(7, 3)));
	static_assert(noexcept(LB(7, 3) >= LB(7, 3)));
	static_assert(not(LB(8, 3) >= LB(8, 8)));
	static_assert(not(LB(7, 3) >= LB(7, 4)));
	static_assert(LB(2, 5) >= LB(2, 5));
	static_assert(LB(2, 7) >= LB(2, 5));
	static_assert(LB(5, 4) >= LB(2, 4));

	// operator>
	static_assert(noexcept(LB(8, 3) > LB(7, 3)));
	static_assert(noexcept(LB(7, 3) > LB(7, 3)));
	static_assert(not(LB(8, 3) > LB(8, 8)));
	static_assert(not(LB(7, 3) > LB(7, 4)));
	static_assert(LB(2, 5) > LB(2, 3));
	static_assert(LB(2, 7) > LB(2, 5));
	static_assert(LB(5, 4) > LB(2, 4));
}

TEST(Location, Comparisson_Location_and_Block)
{ // Compare Location with Location_Block
	using LB = Location_Block<3>;

	// operator==
	static_assert(noexcept(LB(1, 4) == Location<3>(2)));
	static_assert(noexcept(Location<3>(2) == LB(1, 4)));
	static_assert(noexcept(LB(1, 4) == Location<3>(1, 4)));
	static_assert(noexcept(Location<3>(1, 4) == LB(1, 4)));
	static_assert(LB(0, 0) == Location<3>(0));
	static_assert(Location<3>(0) == LB(0, 0));
	static_assert(LB(0, 0, 0) == Location<3>(0));
	static_assert(Location<3>(0) == LB(0, 0, 0));
	static_assert(LB(3, 3) == Location<3>(4, 0));
	static_assert(Location<3>(4, 0) == LB(3, 3));
	static_assert(not(LB{1, 1} == Location<3>{1, 1}));
	static_assert(not(Location<3>{1, 1} == LB{1, 1}));
	EXPECT_EQ(LB(0, 0), Location<3>(0));
	EXPECT_EQ(Location<3>(1), LB(0, 1));

	// operator!=
	static_assert(noexcept(LB{1, 4} != Location<3>(12)));
	static_assert(noexcept(Location<3>(12) != LB{1, 4}));
	static_assert(noexcept(LB{8, 8} != Location<3>(80)));
	static_assert(noexcept(Location<3>(80) != LB{8, 8}));
	static_assert(LB{1, 1} != Location<3>{1, 1});
	static_assert(Location<3>{1, 1} != LB{1, 1});
}

//===----------------------------------------------------------------------===//
TEST(Location_Utilities, Size_definitions)
{
	using ::Sudoku::base_size;
	using ::Sudoku::elem_size;
	using ::Sudoku::full_size;

	static_assert(base_size<2> == 2);
	static_assert(elem_size<2> == 4);
	static_assert(full_size<2> == 16);
#if defined(__ICL) && __ICL <= 1900
	EXPECT_TRUE(base_size<2> == 2);
	EXPECT_TRUE(elem_size<2> == 4);
	EXPECT_TRUE(full_size<2> == 16);
#else
	EXPECT_EQ(base_size<2>, 2);
	EXPECT_EQ(elem_size<2>, 4);
	EXPECT_EQ(full_size<2>, 16);
#endif // __ICL
	static_assert(base_size<3> == 3);
	static_assert(elem_size<3> == 9);
	static_assert(full_size<3> == 81);
#if defined(__ICL) && __ICL <= 1900
	EXPECT_TRUE(base_size<3> == 3);
	EXPECT_TRUE(elem_size<3> == 9);
	EXPECT_TRUE(full_size<3> == 81);
#else
	EXPECT_EQ(base_size<3>, 3);
	EXPECT_EQ(elem_size<3>, 9);
	EXPECT_EQ(full_size<3>, 81);
#endif // __ICL
	static_assert(base_size<4> == 4);
	static_assert(elem_size<4> == 16);
	static_assert(full_size<4> == 256);
	EXPECT_EQ(base_size<4>, 4);
	EXPECT_EQ(elem_size<4>, 16);
	EXPECT_EQ(full_size<4>, 256);

	::Sudoku::valid_dimensions<2>();
	::Sudoku::valid_dimensions<3>();
	::Sudoku::valid_dimensions<4>();
}

TEST(Location_Utilities, is_valid)
{
	using ::Sudoku::is_valid;

	static_assert(noexcept(is_valid(Location<2>(-1))));
	static_assert(noexcept(is_valid(Location<2>(0))));
	static_assert(noexcept(is_valid(Location<2>(15))));
	static_assert(noexcept(is_valid(Location<2>(16))));
	static_assert(noexcept(is_valid(Location<2>(18))));
	static_assert(noexcept(is_valid(Location<3>(81))));
	static_assert(not(is_valid(Location<2>(-1))));
	static_assert(is_valid(Location<2>(0)));
	static_assert(is_valid(Location<2>(15)));
	static_assert(not(is_valid(Location<2>(16))));
	static_assert(is_valid(Location<3>(16)));
	static_assert(is_valid(Location<3>(80)));
	static_assert(not(is_valid(Location<3>(81))));
	EXPECT_FALSE(is_valid(Location<2>(-1)));
	EXPECT_TRUE(is_valid(Location<2>(0)));
	EXPECT_TRUE(is_valid(Location<2>(1)));
	EXPECT_TRUE(is_valid(Location<2>(15)));
	EXPECT_FALSE(is_valid(Location<2>(16)));
	EXPECT_TRUE(is_valid(Location<3>(16)));
	EXPECT_TRUE(is_valid(Location<3>(80)));
	EXPECT_FALSE(is_valid(Location<3>(81)));

	using L     = Location<2>;
	using list2 = std::vector<L>;
	static_assert(noexcept(is_valid(list2{})));
	EXPECT_FALSE(noexcept(is_valid(list2{Location<2>(0)})));
	EXPECT_FALSE(noexcept(is_valid(std::vector<Location<3>>{Location<3>(0)})));
	EXPECT_TRUE(noexcept(list2{}));
	EXPECT_TRUE(noexcept(is_valid(std::vector<Location<3>>{})));
	// std::is_sorted can throw std::bad_alloc
	// TODO From C++20 std::is_sorted will be constexpr
	// static_assert(!is_valid<2>(list2{})); // empty()
	EXPECT_FALSE(is_valid(list2{})) << "can't be empty";
	EXPECT_TRUE(is_valid(list2{L(0), L(12), L(13)}));
	EXPECT_TRUE(is_valid(list2{L(8)})) << "must except single";
	EXPECT_FALSE(is_valid(list2{L(5), L(2), L(16)})) << "must be sorted";
	EXPECT_FALSE(is_valid(list2{L(15), L(2), L(1)}))
		<< "must be sorted ascending";
	EXPECT_FALSE(is_valid(list2{L(0), L(15), L(16)}));
	EXPECT_FALSE(is_valid(list2{L(-1), L(0), L(1)}));
	EXPECT_TRUE(is_valid(list2{L(0)}));
	EXPECT_FALSE(is_valid(list2{L(16)}));
	EXPECT_FALSE(is_valid(list2{L(-6)}));
}

TEST(Location_Utilities, is_valid_size)
{
	using ::Sudoku::is_valid_size;

	static_assert(noexcept(is_valid_size<2>(-1)));
	static_assert(noexcept(is_valid_size<2>(0)));
	static_assert(noexcept(is_valid_size<2>(3)));
	static_assert(noexcept(is_valid_size<2>(4)));
	static_assert(not(is_valid_size<2>(-1)));
	static_assert(is_valid_size<2>(0));
	static_assert(is_valid_size<2>(3));
	static_assert(not(is_valid_size<2>(4)));
	static_assert(is_valid_size<3>(4));
	static_assert(is_valid_size<3>(8));
	static_assert(not(is_valid_size<3>(9)));
	EXPECT_FALSE(is_valid_size<2>(-1));
	EXPECT_TRUE(is_valid_size<2>(0));
	EXPECT_TRUE(is_valid_size<2>(1));
	EXPECT_FALSE(is_valid_size<2>(4));
	EXPECT_TRUE(is_valid_size<3>(4));
	EXPECT_TRUE(is_valid_size<3>(8));
	EXPECT_FALSE(is_valid_size<3>(9));

	static_assert(noexcept(is_valid_size<3>(-1, 8)));
	static_assert(noexcept(is_valid_size<3>(0, 8)));
	static_assert(noexcept(is_valid_size<3>(0, 9)));
	static_assert(not(is_valid_size<2>(-1, 3)));
	static_assert(not(is_valid_size<2>(2, -3)));
	static_assert(is_valid_size<2>(0, 0));
	static_assert(is_valid_size<2>(1, 2));
	static_assert(is_valid_size<2>(3, 3));
	static_assert(not(is_valid_size<2>(2, 4)));
	static_assert(not(is_valid_size<2>(4, 1)));
	EXPECT_FALSE(is_valid_size<2>(-1, 3));
	EXPECT_FALSE(is_valid_size<2>(2, -3));
	EXPECT_TRUE(is_valid_size<2>(1, 0));
	EXPECT_FALSE(is_valid_size<2>(2, 4));
}

TEST(Location_Utilities, is_same_section)
{
	using ::Sudoku::is_same_row;
	using ::Sudoku::is_same_col;
	using ::Sudoku::is_same_block;
	using ::Sudoku::is_same_section;
	using ::Sudoku::intersect_block;

	static_assert(std::is_same_v<
				  bool,
				  decltype(is_same_row(Location<3>(), Location<3>()))>);
	static_assert(noexcept(is_same_row(Location<3>(-1), Location<3>(1))));
	static_assert(noexcept(is_same_row(Location<3>(0), Location<3>(0))));
	static_assert(noexcept(is_same_row(Location<3>(80), Location<3>(80))));
	static_assert(noexcept(is_same_row(Location<3>(81), Location<3>(80))));
	static_assert(noexcept(is_same_row(Location<3>(80), Location<3>(81))));
	static_assert(not(is_same_row(Location<3>(-1), Location<3>(0))));
	static_assert(not(is_same_row(Location<3>(0), Location<3>(-1))));
	static_assert(is_same_row(Location<3>(0), Location<3>(0)));
	static_assert(is_same_row(Location<3>(0), Location<3>(1)));
	static_assert(is_same_row(Location<3>(0), Location<3>(8)));
	static_assert(not(is_same_row(Location<3>(0), Location<3>(9))));
	static_assert(is_same_row(Location<3>(17), Location<3>(9)));
	static_assert(is_same_row(Location<3>(72), Location<3>(80)));
	static_assert(not(is_same_row(Location<3>(72), Location<3>(81))));
	static_assert(not(is_same_row(Location<3>(82), Location<3>(81))));
	EXPECT_TRUE(is_same_row(Location<3>(0), Location<3>(8)));
	EXPECT_FALSE(is_same_row(Location<3>(9), Location<3>(8)));

	static_assert(std::is_same_v<
				  bool,
				  decltype(is_same_col(Location<3>(), Location<3>()))>);
	static_assert(noexcept(is_same_col(Location<3>(-1), Location<3>(1))));
	static_assert(noexcept(is_same_col(Location<3>(0), Location<3>(0))));
	static_assert(noexcept(is_same_col(Location<3>(80), Location<3>(80))));
	static_assert(noexcept(is_same_col(Location<3>(81), Location<3>(72))));
	static_assert(noexcept(is_same_col(Location<3>(72), Location<3>(81))));
	static_assert(not(is_same_col(Location<3>(-1), Location<3>(0))));
	static_assert(not(is_same_col(Location<3>(0), Location<3>(-1))));
	static_assert(is_same_col(Location<3>(0), Location<3>(0)));
	static_assert(is_same_col(Location<3>(0), Location<3>(9)));
	static_assert(is_same_col(Location<3>(0), Location<3>(72)));
	static_assert(is_same_col(Location<3>(72), Location<3>(0)));
	static_assert(not(is_same_col(Location<3>(0), Location<3>(81))));
	static_assert(is_same_col(Location<3>(17), Location<3>(8)));
	static_assert(is_same_col(Location<3>(75), Location<3>(21)));
	static_assert(not(is_same_col(Location<3>(72), Location<3>(81))));
	static_assert(not(is_same_col(Location<3>(90), Location<3>(81))));
	EXPECT_TRUE(is_same_col(Location<3>(0), Location<3>(18)));
	EXPECT_FALSE(is_same_col(Location<3>(9), Location<3>(8)));

	static_assert(std::is_same_v<
				  bool,
				  decltype(is_same_block(Location<3>(), Location<3>()))>);
	static_assert(noexcept(is_same_block(Location<3>(-1), Location<3>(1))));
	static_assert(noexcept(is_same_block(Location<3>(0), Location<3>(0))));
	static_assert(noexcept(is_same_block(Location<3>(80), Location<3>(80))));
	static_assert(noexcept(is_same_block(Location<3>(81), Location<3>(72))));
	static_assert(noexcept(is_same_block(Location<3>(72), Location<3>(81))));
	static_assert(not(is_same_block(Location<3>(-1), Location<3>(0))));
	static_assert(not(is_same_block(Location<3>(0), Location<3>(-1))));
	static_assert(is_same_block(Location<3>(0), Location<3>(2)));
	static_assert(is_same_block(Location<3>(0), Location<3>(20)));
	static_assert(is_same_block(Location<3>(3, 3), Location<3>(5, 5)));
	static_assert(is_same_block(Location<3>(80), Location<3>(6, 6)));
	static_assert(not(is_same_block(Location<3>(80), Location<3>(81))));
	static_assert(not(is_same_block(Location<3>(72), Location<3>(81))));
	static_assert(not(is_same_block(Location<3>(90), Location<3>(81))));
	EXPECT_TRUE(is_same_block(Location<3>(0), Location<3>(11)));
	EXPECT_FALSE(is_same_block(Location<3>(9), Location<3>(8)));

	// is_same_section (taking iterators)
	using L = Location<3>;
	// empty can cause issue with begin+1 (SEH exception)
	const std::vector<L> empty{};
	const std::vector<L> single{L(4)};
	static_assert(noexcept(is_same_row<3>(empty.begin(), empty.end())));
	EXPECT_FALSE(is_same_row<3>(empty.begin(), empty.end()));
	EXPECT_TRUE(is_same_row<3>(single.begin(), single.end()));
	EXPECT_FALSE(is_same_col<3>(empty.begin(), empty.end()));
	EXPECT_TRUE(is_same_col<3>(single.begin(), single.end()));
	EXPECT_FALSE(is_same_block<3>(empty.begin(), empty.end()));
	EXPECT_TRUE(is_same_block<3>(single.begin(), single.end()));
	// normal operation
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
	static_assert(
		std::is_same_v<bool, decltype(is_same_row<3>(row.begin(), row.end()))>);
	static_assert(noexcept(is_same_row<3>(row.begin(), row.end())));
	static_assert(noexcept(is_same_row<3>(row.cbegin(), row.cend())));
	static_assert(noexcept(is_same_row<3>(col.cbegin(), col.cend())));
	static_assert(
		noexcept(is_same_row<3>(notsortedrow.cbegin(), notsortedrow.cend())));
	EXPECT_TRUE(is_same_row<3>(row.begin(), row.end()));
	EXPECT_TRUE(is_same_row<3>(row.cbegin(), row.cend()));
	EXPECT_TRUE(is_same_row<3>(notsortedrow.cbegin(), notsortedrow.cend()));
	EXPECT_TRUE(is_same_row<3>(duplicate.cbegin(), duplicate.cend()));
	static_assert(
		std::is_same_v<bool, decltype(is_same_col<3>(col.begin(), col.end()))>);
	static_assert(noexcept(is_same_col<3>(col.begin(), col.end())));
	static_assert(noexcept(is_same_col<3>(col.cbegin(), col.cend())));
	static_assert(noexcept(is_same_col<3>(row.cbegin(), row.cend())));
	EXPECT_TRUE(is_same_col<3>(col.cbegin(), col.cend()));
	EXPECT_FALSE(is_same_col<3>(row.cbegin(), row.cend()));
	static_assert(std::is_same_v<
				  bool,
				  decltype(is_same_block<3>(blo.begin(), blo.end()))>);
	static_assert(noexcept(is_same_block<3>(blo.begin(), blo.end())));
	static_assert(noexcept(is_same_block<3>(blo.cbegin(), blo.cend())));
	static_assert(noexcept(is_same_block<3>(row.cbegin(), row.cend())));
	EXPECT_TRUE(is_same_block<3>(blo.cbegin(), blo.cend()));
	EXPECT_FALSE(is_same_block<3>(row.cbegin(), row.cend()));

	// is_same_section (taking a section)
	[[maybe_unused]] bool tmp{};
	const ::Sudoku::Board<int, 3> B1;
	static_assert(
		std::is_same_v<bool, decltype(is_same_section(B1.row(0), L()))>);
	static_assert(noexcept(is_same_section(B1.row(0), L(12))));
	EXPECT_NO_THROW(tmp = is_same_section(B1.row(0), L(12)));
	EXPECT_TRUE(is_same_section(B1.row(0), L(8)));
	EXPECT_TRUE(is_same_section(B1.row(1), L(12)));
	EXPECT_TRUE(is_same_section(B1.row(7), L(70)));
	EXPECT_FALSE(is_same_section(B1.row(2), L(15)));
	static_assert(
		std::is_same_v<bool, decltype(is_same_section(B1.col(0), L()))>);
	static_assert(noexcept(is_same_section(B1.col(0), L(12))));
	EXPECT_NO_THROW(tmp = is_same_section(B1.col(0), L(12)));
	EXPECT_TRUE(is_same_section(B1.col(0), L(0)));
	EXPECT_TRUE(is_same_section(B1.col(0), L(72)));
	EXPECT_FALSE(is_same_section(B1.col(1), L(9)));
	static_assert(
		std::is_same_v<bool, decltype(is_same_section(B1.block(0), L()))>);
	static_assert(noexcept(is_same_section(B1.block(0), L(12))));
	EXPECT_NO_THROW(tmp = is_same_section(B1.block(0), L(12)));
	EXPECT_TRUE(is_same_section(B1.block(0), L(10)));
	EXPECT_FALSE(is_same_section(B1.block(1), L(16)));

	// is_same_section (taking a section and a vector)
	static_assert(
		std::is_same_v<bool, decltype(is_same_section(B1.row(0), row))>);
	// static_assert(noexcept(is_same_section(B1.row(0), row)));
	EXPECT_NO_THROW(tmp = is_same_section(B1.row(0), row));
	EXPECT_TRUE(is_same_section(B1.row(0), row));
	EXPECT_TRUE(is_same_section(B1.row(0), col));
	EXPECT_FALSE(is_same_section(B1.row(0), blo));
	EXPECT_FALSE(is_same_section(B1.row(1), row));
	static_assert(
		std::is_same_v<bool, decltype(is_same_section(B1.col(0), col))>);
	EXPECT_NO_THROW(tmp = is_same_section(B1.col(0), row));
	EXPECT_TRUE(is_same_section(B1.col(0), row));
	EXPECT_TRUE(is_same_section(B1.col(6), col));
	EXPECT_FALSE(is_same_section(B1.col(0), col));
	EXPECT_FALSE(is_same_section(B1.col(0), blo));
	EXPECT_TRUE(is_same_section(B1.col(5), blo));
	static_assert(
		std::is_same_v<bool, decltype(is_same_section(B1.col(0), col))>);
	EXPECT_NO_THROW(tmp = is_same_section(B1.block(0), row));
	EXPECT_TRUE(is_same_section(B1.block(4), blo));
	EXPECT_FALSE(is_same_section(B1.block(5), blo));

	// intersect_block
	static_assert(noexcept(intersect_block(B1.row(0), L(12))));
	EXPECT_NO_THROW(tmp = intersect_block(B1.row(0), L(55)));
	EXPECT_NO_THROW(tmp = intersect_block(B1.col(0), L(55)));
	EXPECT_TRUE(intersect_block(B1.row(0), L(8)));
	EXPECT_TRUE(intersect_block(B1.row(6), L(54)));
	EXPECT_FALSE(intersect_block(B1.row(5), L(59)));
	EXPECT_TRUE(intersect_block(B1.col(0), L(11)));
	EXPECT_FALSE(intersect_block(B1.col(8), L(2)));
}

TEST(Location_Utilities, get_same_section)
{
	std::vector<Location<3>> list1{};
	// std::vector<Location<3>> list2{};
	std::vector<Location<3>> list3{};
	for (gsl::index i{}; i < 9; ++i)
	{
		list1.emplace_back(Location<3>{i});
		// list2.emplace_back(Location<3>{i*3});
		list3.emplace_back(Location<3>{i * 9});
	}
	const std::vector<Location<3>> clist1{list1};

	static_assert(std::is_same_v<
				  std::vector<Location<3>>,
				  decltype(get_same_row(Location<3>(0), list1))>);
	static_assert(noexcept(get_same_row(Location<3>(0), list1)));
	static_assert(noexcept(get_same_col(Location<3>(0), list1)));
	static_assert(noexcept(get_same_block(Location<3>(0), list1)));
	EXPECT_EQ(get_same_row(Location<3>(0), list1), list1);
	EXPECT_EQ(get_same_row(Location<3>(0), list1).size(), size_t{9})
		<< "vector length";
	EXPECT_EQ(get_same_col(Location<3>(0), list3), list3);
	EXPECT_EQ(get_same_col(Location<3>(0), list3).size(), size_t{9})
		<< "vector length";
	EXPECT_EQ(get_same_block(Location<3>(0), list1).size(), size_t{3})
		<< "length";
	EXPECT_EQ(get_same_row(Location<3>(0), clist1), list1);
}

} // namespace SudokuTests::LocationTest
