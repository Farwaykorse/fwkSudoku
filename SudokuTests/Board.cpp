// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//===--- SudokuTests/Board.cpp                                          ---===//
//
// Unit tests for the template class Sudoku::Board
//===----------------------------------------------------------------------===//
// Implemented with GoogleTest
//
// Notes:
// - The gTest macros have issues with commas,
//   add extra parentheses around expressions containing a comma.
// - not implemented tests are flagged as NEEDTEST [gTest]
// - gTest tries to print iterators if they use inheritance,
//   if used in *_EQ/NE etc. use an explicit test like EXPECT_TRUE(.. == ..).
//
//===----------------------------------------------------------------------===//
#include "print_Options.h" // Debug Output

#include <Sudoku/Board.h> // Class under test
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>
#include <Sudoku/Value.h>
#include <Sudoku/exceptions.h>

#include <array>
#include <bitset>
#include <initializer_list>
#include <set>

#include <numeric> // accumulate
#include <random>  // random-access tests
#include <type_traits>

#include <gtest/gtest.h>


namespace SudokuTests::BoardTest
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Location_Block;
using ::Sudoku::Options;
using ::Sudoku::Value;

namespace compiletime
{
	// try initializing a board in different ways
	// will trigger compile-time errors

	// Type properties
	using typeT  = Board<int, 3>;
	using arrayT = std::array<int, 81>;

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
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
#ifdef _DEBUG
	//? WHY different behaviour between debug/release mode? (MSVC & Clang, !GCC)
	// static_assert(not std::is_standard_layout_v<typeT>, "standard layout");
#else
	static_assert(std::is_standard_layout_v<typeT>, "standard layout");
	// can be converted with reinterpret_cast
#endif // _DEBUG
	static_assert(not std::is_empty_v<typeT>);
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
	static_assert(std::is_default_constructible_v<typeT>);               // ++
	static_assert(std::is_nothrow_default_constructible_v<typeT>);       // --
	static_assert(not std::is_trivially_default_constructible_v<typeT>); // ++

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>);
	static_assert(std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(std::is_trivially_copy_constructible_v<typeT>);

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(std::is_trivially_move_constructible_v<typeT>);

	// copy assignment
	static_assert(std::is_copy_assignable_v<typeT>);           // ++
	static_assert(std::is_nothrow_copy_assignable_v<typeT>);   // ++
	static_assert(std::is_trivially_copy_assignable_v<typeT>); // ++

	static_assert(std::is_move_assignable_v<typeT>);           // ++
	static_assert(std::is_nothrow_move_assignable_v<typeT>);   // ++
	static_assert(std::is_trivially_move_assignable_v<typeT>); // ++

	static_assert(std::is_destructible_v<typeT>);            // ++
	static_assert(std::is_nothrow_destructible_v<typeT>);    // ++
	static_assert(std::is_trivially_destructible_v<typeT>);  // ++
	static_assert(not std::has_virtual_destructor_v<typeT>); // --

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(std::is_swappable_v<typeT>);                          // C++17
	static_assert(std::is_nothrow_swappable_v<typeT>);                  // C++17
	static_assert(not std::is_swappable_with_v<typeT, arrayT>);         // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, arrayT>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, Options<9>>);     // C++17
	static_assert(
		not std::is_nothrow_swappable_with_v<typeT, Options<9>>); // C++17
#endif

	// is_constructible from different types
	// set to non-default value at initialization
	static_assert(std::is_constructible_v<typeT, int>);
	static_assert(not std::is_nothrow_constructible_v<typeT, int>);
	static_assert(std::is_constructible_v<typeT, unsigned int>);
	static_assert(not std::is_nothrow_constructible_v<typeT, unsigned int>);
	static_assert(std::is_constructible_v<typeT, size_t>);
	static_assert(not std::is_nothrow_constructible_v<typeT, size_t>);
	static_assert(std::is_constructible_v<Board<Options<9>, 3>, Options<9>>);
	static_assert(
		not std::is_nothrow_constructible_v<Board<Options<9>, 3>, Options<9>>);
	static_assert(std::is_constructible_v<typeT, arrayT>);
	static_assert(not std::is_nothrow_constructible_v<typeT, arrayT>);
	static_assert(std::is_constructible_v<
				  Board<Options<9>, 3>,
				  std::array<Options<9>, 81>>);
	static_assert(not std::is_nothrow_constructible_v<
				  Board<Options<9>, 3>,
				  std::array<Options<9>, 81>>);
	static_assert(not std::is_constructible_v<
				  Board<Options<9>, 3>,
				  std::array<Options<4>, 81>>);
	static_assert(not std::is_constructible_v<
				  Board<Options<9>, 3>,
				  std::array<Options<4>, 16>>);
	static_assert(std::is_constructible_v<typeT, std::initializer_list<int>>);
	static_assert(
		not std::is_nothrow_constructible_v<typeT, std::initializer_list<int>>);
	static_assert(std::is_constructible_v<
				  Board<Options<3>, 3>,
				  std::initializer_list<Options<3>>>);
	static_assert(not std::is_nothrow_constructible_v<
				  Board<Options<3>, 3>,
				  std::initializer_list<Options<3>>>);

	// Assign to Board:
	static_assert(std::is_assignable_v<Board<int, 3>, Board<int, 3>>);
	static_assert(std::is_assignable_v<typeT, arrayT>);
	static_assert(std::is_assignable_v<typeT, std::array<int, 81>>);
	static_assert(std::is_assignable_v<Board<int, 2>, std::array<int, 16>>);
	static_assert(not std::is_assignable_v<typeT, std::array<int, 12>>);
	// Assign from Board:
	static_assert(not std::is_assignable_v<arrayT, typeT>);
	static_assert(not std::is_assignable_v<Board<int, 3>, int>);
	static_assert(not std::is_assignable_v<typeT, Location_Block<3>>);
	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<int, typeT>);
	// shouldn't be assignable from int, prevent with explicit!!
	static_assert(std::is_assignable_v<typeT, arrayT>);
	static_assert(std::is_assignable_v<typeT, std::initializer_list<int>>);
	static_assert(std::is_assignable_v<
				  Board<Options<3>, 3>,
				  std::initializer_list<Options<3>>>);

	// Implicit conversion to Board: (prefer all negative)
	static_assert(std::is_convertible_v<arrayT, typeT>);
	static_assert(std::is_convertible_v<std::array<int, 81>, typeT>);
	static_assert(std::is_convertible_v<std::initializer_list<int>, typeT>);
	static_assert(not std::is_convertible_v<std::array<int, 12>, typeT>);
	static_assert(not std::is_convertible_v<std::array<Options<9>, 81>, typeT>);
	static_assert(not std::is_convertible_v<typeT, std::initializer_list<int>>);
	// Implicit conversion from Board:
	static_assert(not std::is_convertible_v<typeT, arrayT>);
	static_assert(not std::is_convertible_v<typeT, std::array<Options<9>, 81>>);

	//====----------------------------------------------------------------====//
	// Member types
	static_assert(std::is_same_v<typeT::value_type, int>);
	static_assert(std::is_same_v<Board<Options<4>, 2>::value_type, Options<4>>);
	static_assert(std::is_same_v<typeT::value_type const, const int>);
	static_assert(std::is_same_v<typeT::size_type, decltype(typeT::size())>);
	static_assert(std::is_same_v<typeT::difference_type, std::ptrdiff_t>);
	static_assert(std::is_same_v<typeT::reference, int&>);
	static_assert(std::is_same_v<typeT::const_reference, int const&>);
	static_assert(std::is_same_v<typeT::pointer, int*>);
	static_assert(std::is_same_v<typeT::const_pointer, int const*>);
	static_assert(std::is_same_v<typeT::pointer const, int* const>);
	static_assert(std::is_same_v<typeT::const_pointer const, int const* const>);

} // namespace compiletime
//===----------------------------------------------------------------------===//

TEST(Board, Construction)
{
	// default constructor
	EXPECT_NO_THROW((Board<int, 2>()));
	EXPECT_NO_THROW((Board<Options<4>, 2>()));
	EXPECT_NO_THROW((Board<int, 3>()));
	EXPECT_NO_THROW((Board<Options<9>, 3>()));
	EXPECT_NO_THROW((Board<int, 4>()));
	EXPECT_NO_THROW((Board<Options<16>, 4>()));
	EXPECT_NO_THROW(Board<int>()) << "fall back on default value for base_size";
	EXPECT_NO_THROW(Board<Options<9>>());
	EXPECT_NO_THROW((Board<int, 3>{})) << "initializer list";

	// initialize to non-default value
	EXPECT_NO_THROW((Board<int, 2>(3)));

	EXPECT_NO_THROW((Board<Options<12>, 2>())) << "unexpected size for Options";
	EXPECT_NO_THROW((Board<Options<1>, 2>())) << "unexpected size for Options";

	{
		SCOPED_TRACE("Copy Constructor : Board(const Board&)");
		const Board<int, 2> D_2{};
		// NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
		[[maybe_unused]] const Board<int, 2> Opt{D_2}; // copy construct
		EXPECT_NO_THROW((Board<int, 2>(D_2)));         // copy construct
		EXPECT_EQ((Board<int, 2>(D_2)), D_2);
		const Board<int, 3> D_3;
		// NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
		EXPECT_NO_THROW([[maybe_unused]] Board<int> Opt2 = D_3);
	}
	{
		SCOPED_TRACE("Copy Assignment : Board& operator=(const Board&)");
		const Board<int, 2> D_2{};
		Board<int, 2> Opt{};
		EXPECT_NO_THROW(Opt = D_2);
		EXPECT_EQ(Opt, D_2) << "test requires operator==";
	}
	{
		SCOPED_TRACE("Move Constructor : Board(Board&&)");
		EXPECT_NO_THROW([[maybe_unused]] Board<int> Opt = (Board<int, 3>()));
		EXPECT_NO_THROW(
			[[maybe_unused]] auto Opt = (Board<int, 2>())); // move construct
	}
	{
		SCOPED_TRACE("Move Assign : Board& operator=(Board&&)");
		[[maybe_unused]] Board<int, 2> Opt{};
		EXPECT_NO_THROW(Opt = (Board<int, 2>()));
	}
	{
		SCOPED_TRACE("Construct from array");
		constexpr std::array<int, 16> list{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		[[maybe_unused]] constexpr Board<int, 2> board1{list};
		[[maybe_unused]] constexpr auto board2 = list;
		ASSERT_NO_THROW((Board<int, 2>{list}));
	}
	{
		SCOPED_TRACE("Construct from iterators");
		constexpr std::array<int, 16> long_list{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		[[maybe_unused]] constexpr std::array<uint16_t, 16> long_u_list{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		std::vector<int> vector{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		const Board<Options<4>, 2> board{};

		// Direct copy
		static_assert(not noexcept(
			Board<int, 2>(std::begin(long_list), std::end(long_list))));
		ASSERT_NO_THROW(
			(Board<int, 2>(std::begin(long_list), std::end(long_list))));
		static_assert(
			not noexcept(Board<int, 2>(std::begin(vector), std::end(vector))));
		ASSERT_NO_THROW((Board<int, 2>(std::begin(vector), std::end(vector))));
		static_assert(not noexcept(
			Board<Options<4>, 2>(std::begin(board), std::end(board))));
		ASSERT_NO_THROW(
			(Board<Options<4>, 2>(std::begin(board), std::end(board))));
		Board<int, 2> new_board(std::begin(long_list), std::end(long_list));
		EXPECT_EQ(new_board[0][3], 3);
		EXPECT_EQ(new_board[3][3], 15);
		// Conversion (implicit)
		static_assert(std::is_convertible_v<int, int16_t>);
		ASSERT_NO_THROW(
			(Board<int32_t, 2>(std::begin(long_list), std::end(long_list))));
		ASSERT_NO_THROW(
			(Board<int64_t, 2>(std::begin(long_list), std::end(long_list))));
		// Conversion (explicit)
		static_assert(not std::is_convertible_v<uint16_t, Value>);
		static_assert(std::is_constructible_v<Value, uint16_t>);
		ASSERT_NO_THROW(
			(Board<Value, 2>(std::begin(long_u_list), std::end(long_u_list))));

		EXPECT_THROW(
			(Board<int, 2>(std::begin(long_list), std::begin(long_list))),
			std::length_error);
		EXPECT_THROW(
			(Board<int, 2>(std::begin(long_list), std::end(long_list) - 1)),
			std::length_error);
		static_assert(not std::is_convertible_v<std::array<Value, 2>, int>);
		static_assert(not std::is_constructible_v<int, std::array<Value, 2>>);
		EXPECT_THROW(
			(Board<std::array<Value, 2>, 2>{
				std::begin(long_list), std::end(long_list)}),
			Sudoku::error::unsupported_type);
	}
	{
		SCOPED_TRACE("Construct from Initializer_list");
		const std::initializer_list<int> list{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		ASSERT_NO_THROW((Board<int, 2>{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}));
		Board<int, 2> Opt{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		EXPECT_EQ(
			Opt,
			(Board<int, 2>{
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}));
		EXPECT_EQ(Opt[0][3], 3);
		EXPECT_EQ(Opt[3][3], 15);
		EXPECT_NO_THROW((Board<int, 2>{list}));
		Board<int, 2> Opt1{};
		EXPECT_NO_THROW(
			(Opt1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}));
		EXPECT_EQ(Opt1, Opt);
		Board<int, 2> Opt2{};
		EXPECT_NO_THROW(Opt2 = list);
		EXPECT_EQ(Opt, Opt2);
		// Initializer_list invalid length
		const std::initializer_list<int> short_list{0, 1, 2, 3};
		const std::initializer_list<int> long_list{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
		EXPECT_THROW((Board<int, 2>{short_list}), std::length_error);
		EXPECT_THROW((Board<int, 2>{long_list}), std::length_error);

		// NOTE no type conversion through range constructor
	}
}

TEST(Board, defaultValues)
{
	// for int, all 0
	Board<int, 2> B_int{};
	EXPECT_EQ(B_int[Location<2>(0)], 0);
	EXPECT_EQ(B_int[Location<2>(13)], 0);
	EXPECT_EQ(B_int[Location<2>(15)], 0);
	//	options -> all
	Board<Options<4>, 2> B_opt{};
	EXPECT_TRUE(B_opt[Location<2>(0)].all());
	EXPECT_TRUE(B_opt[Location<2>(7)].all());
}

TEST(Board, size)
{
	static_assert(noexcept(Board<int>::size()));
	static_assert(noexcept(Board<int, 2>::size()));
	static_assert(noexcept(Board<int, 3>::size()));
	static_assert(Board<int>::size() == 81);
	static_assert(Board<int, 2>::size() == 16);
	static_assert(Board<int, 3>::size() == 81);
	static_assert(Board<int, 4>::size() == 256);

	EXPECT_EQ(Board<int>::size(), size_t{81});
	EXPECT_EQ((Board<int, 2>::size()), size_t{16});

	// Max_size
	static_assert(noexcept(Board<int>::max_size()));
	static_assert(noexcept(Board<int, 2>::max_size()));
	static_assert(noexcept(Board<int, 3>::max_size()));
	static_assert(Board<int>::max_size() == 81);
	static_assert(Board<int, 2>::max_size() == 16);
	static_assert(Board<int, 3>::max_size() == 81);
	static_assert(Board<int, 4>::max_size() == 256);

	EXPECT_EQ(Board<int>::max_size(), size_t{81});
	EXPECT_EQ((Board<int, 2>::max_size()), size_t{16});
}

TEST(Board, empty)
{
	static_assert(noexcept(Board<int, 2>::empty()));
	static_assert(std::is_same_v<bool, decltype(Board<int, 2>::empty())>);
	static_assert(!(Board<int, 2>::empty()));
}

TEST(Board, operatorEqual)
{
	static_assert(not noexcept(Board<int>() == Board<int>(3)));
	// assuming the used algorithms only throw `std::bad_alloc`
	// check if operator== for type T (here int) can throw
	EXPECT_EQ(Board<int>(), Board<int>());
	EXPECT_EQ(Board<int>(), (Board<int, 3>()));
	EXPECT_EQ(Board<int>(), Board<int>(0));
	EXPECT_EQ(Board<int>(), (Board<int, 3>(0)));
	EXPECT_EQ(Board<int>(0), Board<int>());
	EXPECT_EQ(Board<int>(4), Board<int>(4));
	EXPECT_EQ(Board<int>(5), (Board<int, 3>(5)));
	EXPECT_EQ((Board<int, 3>(5)), (Board<int, 3>(5)));
	EXPECT_EQ((Board<int, 2>(1)), (Board<int, 2>(1)));
	EXPECT_FALSE((Board<int, 2>(1)) == (Board<int, 2>(2)));
	// single element difference
	Board<int, 2> B1{};
	EXPECT_TRUE((Board<int, 2>() == B1));
	EXPECT_NO_THROW(B1[Location<2>(13)] = 3);
	EXPECT_FALSE((Board<int, 2>() == B1));
	// value-type: Options
	EXPECT_EQ((Board<Options<4>, 2>()), (Board<Options<4>, 2>()));
	EXPECT_FALSE(
		(Board<Options<4>, 2>(Options<4>{Value{1}})) ==
		(Board<Options<4>, 2>()));
}

TEST(BoardUtilities, operatorNotEqual)
{
	static_assert(not noexcept(Board<int>() != Board<int>(3)));
	// Board == Board => ...
	EXPECT_NE(Board<int>(), Board<int>(4));
	EXPECT_NE(Board<int>(1), Board<int>(4));
	EXPECT_NE(Board<int>(5), Board<int>(4));
	EXPECT_NE(Board<int>(), (Board<int, 3>(2)));
	EXPECT_FALSE(Board<int>(4) != Board<int>(4));
	// single element difference
	Board<int, 2> B1{};
	EXPECT_FALSE((Board<int, 2>() != B1));
	EXPECT_NO_THROW(B1[Location<2>(13)] = 3);
	EXPECT_NE((Board<int, 2>()), B1);
	// value-type: Options
	EXPECT_NE(
		(Board<Options<4>, 2>(Options<4>{Value{1}})), (Board<Options<4>, 2>()));
	EXPECT_FALSE((Board<Options<4>, 2>()) != (Board<Options<4>, 2>()));
}

TEST(Board, accessFrontBack)
{
	constexpr std::array<int, 16> list{
		5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 7};
	Board<int, 2> B = std::array<int, 16>{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	constexpr Board<int, 2> cB = list;
	constexpr Board<int, 2> empty{};
	// Read
	static_assert(noexcept(B.front() == 9));
	static_assert(noexcept(B.back() == 9));
	static_assert(noexcept(cB.front() == 9));
	static_assert(noexcept(cB.back() == 9));

	static_assert(std::is_same_v<int&, decltype(B.front())>);
	static_assert(std::is_same_v<int const&, decltype(cB.front())>);
	EXPECT_EQ(B.front(), 0);
	EXPECT_EQ(B.back(), 15);
	EXPECT_EQ(cB.front(), 5);
	EXPECT_EQ(cB.back(), 7);
	// constexpr use
	static_assert(cB.front() == 5);
	static_assert(cB.back() == 7);
	static_assert(empty.front() == 0);
	static_assert(empty.back() == 0);

	// Write
	static_assert(noexcept(B.front() = 9));
	static_assert(std::is_assignable_v<decltype(B.front()), int>);
	static_assert(std::is_trivially_assignable_v<decltype(B.front()), int>);
	static_assert(std::is_nothrow_assignable_v<decltype(B.front()), int>);
	B.front() = 12;
	EXPECT_EQ(B.front(), 12);

	static_assert(noexcept(B.back() = 9));
	static_assert(std::is_assignable_v<decltype(B.back()), int>);
	static_assert(std::is_trivially_assignable_v<decltype(B.back()), int>);
	static_assert(std::is_nothrow_assignable_v<decltype(B.back()), int>);
	B.back() = 89;
	EXPECT_EQ(B.back(), 89);
}

TEST(Board, accessChecked)
{
	using ::Sudoku::error::invalid_Location;

	// at(Location)
	Board<int, 2> B1{};
	static_assert(std::is_assignable_v<decltype(B1.at(Location<2>(0))), int>);
	static_assert(
		std::is_trivially_assignable_v<decltype(B1.at(Location<2>(0))), int>);
	static_assert(
		std::is_nothrow_assignable_v<decltype(B1.at(Location<2>(0))), int>);
#if not defined(__clang__) && (defined(_MSC_VER) && _MSC_VER < 1922 ||         \
							   defined(__GNUC__) && __GNUC__ < 9)
	static_assert(noexcept(B1.at(Location<2>(0)) = 2));
	static_assert(noexcept(B1.at(Location<2>(0)) == 2));
#else // newer compilers
	static_assert(not noexcept(B1.at(Location<2>(0)) = 2));
	static_assert(not noexcept(B1.at(Location<2>(0)) == 2));
#endif
	static_assert(not noexcept(B1.at(Location<2>(20)) == 2));
	static_assert(std::is_same_v<int&, decltype(B1.at(Location<2>(2)))>);
	EXPECT_THROW({ B1.at(Location<2>{17}) = 3; }, invalid_Location);
	EXPECT_THROW({ B1.at(Location<2>{4, 0}) = 2; }, invalid_Location);
	// EXPECT_THROW({ B1.at(Location<2>{0, 5}) = 2; }, error::invalid_Location);
	EXPECT_THROW(B1.at(Location<2>(16)), std::out_of_range);
	EXPECT_THROW(B1.at(Location<2>(-1)), std::out_of_range);

	EXPECT_NO_THROW(B1.at(Location<2>(2)) = 2);
	EXPECT_EQ(B1.at(Location<2>(2)), 2);
	EXPECT_NO_THROW(B1.at(Location<2>(0, 3)) = 3);
	EXPECT_EQ(B1.at(Location<2>(3)), 3);
	EXPECT_NO_THROW(B1.at(Location_Block<2>(0, 2)) = 4);
	EXPECT_EQ(B1.at(Location<2>(4)), 4);
	EXPECT_NO_THROW(B1.at(Location_Block<2>(0, 1, 1)) = 5);
	EXPECT_EQ(B1.at(Location<2>(5)), 5);
	EXPECT_THROW(B1.at(Location<2>(16)), std::out_of_range);
	EXPECT_THROW(B1.at(Location<2>(16)), invalid_Location);
	EXPECT_THROW(B1.at(Location<2>(-1)), std::out_of_range);
	EXPECT_THROW(B1.at(Location<2>(-1)), invalid_Location);

	// at(Location) const(expr)
	const Board<int, 2> cB = std::array<int, 16>{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	constexpr Board<int, 2> cexprB = std::array<int, 16>{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
#if not defined(__clang__) && (defined(_MSC_VER) && _MSC_VER < 1922 ||         \
							   defined(__GNUC__) && __GNUC__ < 9)
	static_assert(noexcept(cB.at(Location<2>(0)) == 1));
#else
	static_assert(not noexcept(cB.at(Location<2>(0)) == 1));
#endif
#if not defined(__clang__) && (defined(_MSC_VER) && _MSC_VER < 1922 ||         \
							   defined(__GNUC__) && __GNUC__ < 9) ||           \
	defined(__ICL)
	static_assert(noexcept(cexprB.at(Location<2>(0)) == 1));
#else
	static_assert(not noexcept(cexprB.at(Location<2>(0)) == 1));
#endif
	static_assert(not noexcept(cB.at(Location<2>(20)) == 1));
	static_assert(not noexcept(cexprB.at(Location<2>(20)) == 1));
	static_assert(std::is_same_v<int const&, decltype(cB.at(Location<2>(2)))>);
	[[maybe_unused]] int tmp{};
	EXPECT_EQ(cB.at(Location<2>(2)), 2) << "at(Location) const";
	EXPECT_THROW(tmp = cB.at(Location<2>(16)), invalid_Location);
	EXPECT_THROW(tmp = cB.at(Location<2>(-1)), invalid_Location);
	EXPECT_THROW(tmp = cB.at(Location<2>(16)), std::out_of_range);
	EXPECT_THROW(tmp = cB.at(Location<2>(-1)), std::out_of_range);
	EXPECT_THROW(tmp = cB.at(Location<2>{4, 0}), invalid_Location);

	// at(row, col)
	// at(row, col)
#if not defined(__clang__) && (defined(_MSC_VER) && _MSC_VER < 1922 ||         \
							   defined(__GNUC__) && __GNUC__ < 9)
	static_assert(noexcept(B1.at(0, 1) == 1));
#else
	static_assert(not noexcept(B1.at(0, 1) == 1));
#endif
	static_assert(not noexcept(B1.at(5, 1) == 1));
	EXPECT_THROW(B1.at(1, 4), invalid_Location);
	EXPECT_THROW(B1.at(1, 4), std::out_of_range);
	EXPECT_THROW(B1.at(4, 0), invalid_Location);
	EXPECT_THROW(B1.at(4, 0), std::out_of_range);
	EXPECT_THROW(B1.at(-1, 0), invalid_Location);
	EXPECT_THROW(B1.at(1, -2), invalid_Location);
	static_assert(std::is_same_v<int&, decltype(B1.at(2, 2))>);
	EXPECT_NO_THROW(B1.at(1, 1) = 5);
	EXPECT_NO_THROW(B1.at(0, 0) = 1);
	EXPECT_NO_THROW(B1.at(1, 1) = 1);
	EXPECT_NO_THROW(B1.at(3, 3) = 5);

	EXPECT_THROW(B1.at(1, 4), invalid_Location);
	EXPECT_THROW(B1.at(4, 0), invalid_Location);
	EXPECT_THROW(B1.at(-1, 0), invalid_Location);
	EXPECT_THROW(B1.at(1, -2), invalid_Location);
	// at(Location) const
	EXPECT_NO_THROW(tmp = cB.at(2, 1));
#if not defined(__clang__) && (defined(_MSC_VER) && _MSC_VER < 1922 ||         \
							   defined(__GNUC__) && __GNUC__ < 9)
	static_assert(noexcept(cB.at(0, 1) == 1));
#else
	static_assert(not noexcept(cB.at(0, 1) == 1));
#endif
	static_assert(not noexcept(cB.at(0, 9) == 1));
	EXPECT_THROW(tmp = cB.at(1, 4), invalid_Location);
	EXPECT_THROW(tmp = cB.at(4, 0), invalid_Location);
	EXPECT_THROW(tmp = cB.at(-1, 0), invalid_Location);
	EXPECT_THROW(tmp = cB.at(1, -2), invalid_Location);
	static_assert(std::is_same_v<int const&, decltype(cB.at(2, 2))>);
	EXPECT_EQ(cB.at(0, 0), 0);
	EXPECT_EQ(cB.at(3, 1), 13);
}

TEST(Board, accessUnchecked)
{
	Board<int, 2> B{};
	constexpr Board<int, 2> cB = std::array<int, 16>{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	// operator[](Location)
	static_assert(noexcept(B[Location<2>(0)]));
	static_assert(noexcept(B[Location<2>(16)]));
	static_assert(noexcept(B[Location<2>(-1)]));
	static_assert(noexcept(B[Location<2>(0)] = 0));
	static_assert(std::is_assignable_v<decltype(B[Location<2>(0)]), int>);
	static_assert(
		std::is_trivially_assignable_v<decltype(B[Location<2>(0)]), int>);
	static_assert(
		std::is_nothrow_assignable_v<decltype(B[Location<2>(0)]), int>);
	static_assert(noexcept(cB[Location<2>(0)]));
	static_assert(noexcept(cB[Location<2>(0)]));
	static_assert(noexcept(cB[Location<2>(0)] == 1));
	static_assert(noexcept(B[Location<2>(0)] == 1));

	static_assert(std::is_same_v<int&, decltype(B[Location<2>(12)])>);
	static_assert(std::is_same_v<int const&, decltype(cB[Location<2>(12)])>);
	B[Location<2>(0)] = 0;
	B[Location<2>(1)] = 1;
	EXPECT_EQ(B[Location<2>(0)], 0);
	EXPECT_EQ(B[Location<2>(1)], 1);
	static_assert(noexcept(cB[Location<2>(0)] == 1));
	EXPECT_EQ(cB[Location<2>(5)], 5);

	static_assert(cB[Location<2>(0)] == 0);
	static_assert(cB[Location<2>(5)] == 5);
	static_assert(cB[Location<2>(2, 1)] == 9);
}

TEST(Board, clear)
{
	Board<int, 2> Obj(3);
	ASSERT_EQ(Obj[Location<2>(2)], 3);

	static_assert(not noexcept(Obj.clear()));
	EXPECT_NO_THROW(Obj.clear());
	EXPECT_EQ(Obj[Location<2>(2)], 0);

	Board<Options<4>, 2> Opt(Options<4>{Value{2}});
	ASSERT_EQ(Opt[Location<2>(2)], Value{2});
	ASSERT_EQ(Opt[Location<2>(2)], Options<4>{Value{2}});

	static_assert(not noexcept(Opt.clear()));
	EXPECT_NO_THROW(Opt.clear());
	EXPECT_EQ(Opt[Location<2>(2)], Options<4>());
}

TEST(Board, InBetween)
{ // Implemented in terms of Board_Section::Row()
	constexpr Board<int, 2> cboard = std::array<int, 16>{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	Board<int, 2> board{cboard};
	static_assert(noexcept(board[0][0]));
	static_assert(noexcept(board[4][5]));
	static_assert(noexcept(board[-4][-5]));
	static_assert(noexcept(board[0][0] = 1));
	static_assert(std::is_assignable_v<decltype(board[0][0]), int>);
	static_assert(std::is_trivially_assignable_v<decltype(board[0][0]), int>);
	static_assert(std::is_nothrow_assignable_v<decltype(board[0][0]), int>);
	board[1][0] = 9;
	EXPECT_EQ(board[1][0], 9);
	EXPECT_EQ(board[0][3], 3);
	EXPECT_EQ(board[3][3], 15);
	EXPECT_NE(board[3][3], 10);

	Board<Options<4>, 2> board2{};
	static_assert(noexcept(board2[0][0]));
	static_assert(noexcept(board2[0][0] = Value{1}));
	board2[2][0] = Value{2};
	EXPECT_EQ(board2[2][0], Value{2}); // short for is_answer(value)
	EXPECT_NE(board2[2][0], Value{1});

	// const_InBetween
	static_assert(noexcept(cboard[0][0]));
	static_assert(noexcept(cboard[4][5]));
	EXPECT_EQ(cboard[0][0], 0);
	EXPECT_EQ(cboard[2][0], 8);
	EXPECT_EQ(cboard[3][3], 15);
	EXPECT_NE(cboard[3][3], 10);
	static_assert(cboard[0][0] == 0);
	static_assert(cboard[2][0] == 8);
	static_assert(cboard[3][3] == 15);
}
} // namespace SudokuTests::BoardTest
