//===--	SudokuTests/Board.cpp											--===//
//
// Unit tests for the template class Sudoku::Board
//===---------------------------------------------------------------------===//
// Implemented with GoogleTest
//
// Notes:
// gTest is limited for use with multiple template parameters.
// These expressions need to be implemented between extra parentheses
// - test elements implementing this are flagged with [gTest]
// - not implemented tests are flagged as NEEDTEST [gTest]
// gTest tries to print iterators if they use inheritance,
//   if used in *_EQ/NE etc.
//   use an explicit test like EXPECT_TRUE(.. == ..).
//
//===---------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include "../Sudoku/Board.h"
// helpers
#include "../Sudoku/Location.h"
#include "../Sudoku/Options.h"
// Debug Output
#include "print_Options.h"
// library
#include <bitset>
#include <initializer_list>
#include <set>
#include <numeric> // accumulate
#include <random>  // randomaccess tests
#include <type_traits>

using namespace Sudoku;

namespace SudokuTests::BoardTest
{
namespace compiletime
{
	// try initializing a board in different ways
	// will trigger compile-time errors

	// Type properties
	using typeT = Board<int, 3>;
	static_assert(std::is_class_v<typeT>);
	static_assert(!std::is_trivial_v<typeT>); // ++
	// trivial default constructors & trivially copyable
	static_assert(!std::is_trivially_copyable_v<typeT>); // ++
	// compatible with std::memcpy & binary copy from / to files"
//??? WHY different behaviour between debug/release mode?
#ifdef _DEBUG
	static_assert(!std::is_standard_layout_v<typeT>, "standard layout");
#else
	static_assert(std::is_standard_layout_v<typeT>, "standard layout");
	// can be converted with reinterpret_cast
#endif
	static_assert(!std::is_pod_v<typeT>);
	// ++ Plain Old Data, both trivial and standard-layout, C compatible
	// static_assert(std::has_unique_object_representations_v<typeT>,
	// "");//C++17
	// trivially_copyable same object representation
	static_assert(!std::is_empty_v<typeT>);
	// class with no datamembers; nothing virtual
	static_assert(!std::is_polymorphic_v<typeT>); // --
	// inherits atleast one virtual function
	static_assert(!std::is_final_v<typeT>, "-- cannot be used as base class");
	static_assert(!std::is_abstract_v<typeT>); // ++
	// inherits or declares at least one pure virtual function");

	// default constructor: typeT()
	static_assert(std::is_default_constructible_v<typeT>);            // ++
	static_assert(!std::is_nothrow_default_constructible_v<typeT>);   // --
	static_assert(!std::is_trivially_default_constructible_v<typeT>); // ++

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>);
	static_assert(!std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(!std::is_trivially_copy_constructible_v<typeT>); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(!std::is_trivially_move_constructible_v<typeT>);

	// copy assingment
	static_assert(std::is_copy_assignable_v<typeT>);            // ++
	static_assert(!std::is_nothrow_copy_assignable_v<typeT>);   // ++
	static_assert(!std::is_trivially_copy_assignable_v<typeT>); // ++

	static_assert(std::is_move_assignable_v<typeT>);            // ++
	static_assert(std::is_nothrow_move_assignable_v<typeT>);    // ++
	static_assert(!std::is_trivially_move_assignable_v<typeT>); // ++

	static_assert(std::is_destructible_v<typeT>);            // ++
	static_assert(std::is_nothrow_destructible_v<typeT>);    // ++
	static_assert(!std::is_trivially_destructible_v<typeT>); // ++
	static_assert(!std::has_virtual_destructor_v<typeT>);    // --

	static_assert(std::is_swappable_v<typeT>);         // C++17
	static_assert(std::is_nothrow_swappable_v<typeT>); // C++17

	// is_constructible from different types
	// set to non-default value at initialization
	static_assert(std::is_constructible_v<typeT, int>);
	static_assert(std::is_constructible_v<typeT, unsigned int>);
	static_assert(std::is_constructible_v<typeT, size_t>);
	static_assert(std::is_constructible_v<Board<Options<9>, 3>, Options<9>>);
	static_assert(std::is_constructible_v<typeT, std::initializer_list<int>>);
	static_assert(std::is_constructible_v<
				  Board<Options<3>, 3>,
				  std::initializer_list<Options<3>>>);

	static_assert(std::is_assignable_v<Board<int, 3>, Board<int, 3>>);
	static_assert(!std::is_assignable_v<Board<int, 3>, int>);
	static_assert(!std::is_assignable_v<typeT, Location_Block<3>>);
	static_assert(!std::is_assignable_v<typeT, int>);
	// shouldn't be assignable from int, prevent with explicit!!
	static_assert(std::is_assignable_v<typeT, std::initializer_list<int>>);
	static_assert(std::is_assignable_v<
				  Board<Options<3>, 3>,
				  std::initializer_list<Options<3>>>);

	static_assert(!std::is_swappable_with_v<typeT, Options<9>>); // C++17
	static_assert(
		!std::is_nothrow_swappable_with_v<typeT, Options<9>>); // C++17
} // namespace compiletime
//===---------------------------------------------------------------------===//

TEST(Board, Construction)
{
	// default constructor
	// EXPECT_NO_THROW((Sudoku::Board<int, 0>())); // shouldn't compile
	// EXPECT_NO_THROW((Sudoku::Board<int, 1>())); // shouldn't compile
	EXPECT_NO_THROW((Board<int, 2>()));         // [gTest]
	EXPECT_NO_THROW((Board<Options<4>, 2>()));  // [gTest]
	EXPECT_NO_THROW((Board<int, 3>()));         // [gTest]
	EXPECT_NO_THROW((Board<Options<9>, 3>()));  // [gTest]
	EXPECT_NO_THROW((Board<int, 4>()));         // [gTest]
	EXPECT_NO_THROW((Board<Options<16>, 4>())); // [gTest]
	EXPECT_NO_THROW(Board<int>()) << "fallback on default value for base_size";
	EXPECT_NO_THROW(Board<Options<9>>())
		<< "fallback on default value for base_size";
	EXPECT_NO_THROW((Board<int, 3>{})) << "initializer list"; // [gTest]

	// initialize to non-default value
	EXPECT_NO_THROW((Board<int, 2>(3))); // [gTest]

	EXPECT_NO_THROW((Board<Options<12>, 2>()))
		<< "unexpected size for Options"; // [gTest]
	EXPECT_NO_THROW((Board<Options<1>, 2>()))
		<< "unexpected size for Options"; // [gTest]

	{
		SCOPED_TRACE("Copy Constructor : Board(const Board&)");
		const Board<int, 2> D_2{};
		const Board<int, 2> Opt{D_2};          // copy construct
		EXPECT_NO_THROW((Board<int, 2>(D_2))); // [gTest]	// copy construct
		EXPECT_EQ((Board<int, 2>(D_2)), D_2);  // [gTest]

		const Board<int, 3> D_3;
		EXPECT_NO_THROW(Board<int> Opt2 = D_3);
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
		EXPECT_NO_THROW((Board<int, 2>(Board<int, 2>()))); // [gTest]

		EXPECT_NO_THROW(Board<int> Opt = (Board<int, 3>())); // [gTest]
		EXPECT_NO_THROW(auto Opt = (Board<int, 2>()));       // move construct
	}
	{
		SCOPED_TRACE("Move Assign : Board& operator=(Board&&)");
		Board<int, 2> Opt{};
		EXPECT_NO_THROW(Opt = (Board<int, 2>()));
	}
	{
		SCOPED_TRACE("Construct from Initializer_list");
		const std::initializer_list<int> list{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		ASSERT_NO_THROW((Board<int, 2>{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15})); // [gTest]
		Board<int, 2> Opt{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; // [gTest]
		EXPECT_EQ(
			Opt,
			(Board<int, 2>{
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}));
		EXPECT_EQ(Opt[0][3], 3);
		EXPECT_EQ(Opt[3][3], 15);
		EXPECT_NO_THROW((Board<int, 2>{list})); // [gTest]
		Board<int, 2> Opt1{};
		EXPECT_NO_THROW(
			(Opt1 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}));
		EXPECT_EQ(Opt1, Opt);
		Board<int, 2> Opt2{};
		EXPECT_NO_THROW(Opt2 = list); // [gTest]
		EXPECT_EQ(Opt, Opt2);
		// see deathtests
	}
}
TEST(Board, size)
{
	EXPECT_EQ((Board<int, 2>().size()), 16); // [gTest]
	EXPECT_EQ(Board<int>().size(), 81);
	EXPECT_EQ((Board<int, 3>().size()), 81); // [gTest]
	const Board<int> D_0;
	EXPECT_EQ(D_0.size(), 81);
	EXPECT_EQ((Board<int, 4>().size()), 256); // [gTest]
}
TEST(Board, operator_equal)
{
	EXPECT_EQ(Board<int>(), Board<int>());
	EXPECT_EQ(Board<int>(), (Board<int, 3>())); // [gTest]
	EXPECT_EQ(Board<int>(), Board<int>(0));
	EXPECT_EQ(Board<int>(), (Board<int, 3>(0))); // [gTest]
	EXPECT_EQ(Board<int>(0), Board<int>());
	EXPECT_EQ(Board<int>(4), Board<int>(4));
	EXPECT_EQ(Board<int>(5), (Board<int, 3>(5)));           // [gTest]
	EXPECT_EQ((Board<int, 3>(5)), (Board<int, 3>(5)));      // [gTest]
	EXPECT_EQ((Board<int, 2>(1)), (Board<int, 2>(1)));      // [gTest]
	EXPECT_FALSE((Board<int, 2>(1)) == (Board<int, 2>(2))); // [gTest]
	// single element difference
	Board<int, 2> B1{};
	EXPECT_TRUE((Board<int, 2>() == B1)); // [gTest]
	EXPECT_NO_THROW(B1[Location<2>(13)] = 3);
	EXPECT_FALSE((Board<int, 2>() == B1)); // [gTest]
	// value-type: Options
	EXPECT_EQ((Board<Options<4>, 2>()), (Board<Options<4>, 2>()));
	EXPECT_FALSE((Board<Options<4>, 2>(1)) == (Board<Options<4>, 2>()));
}
TEST(Board_Utilities, operator_not_equal)
{
	EXPECT_NE(Board<int>(), Board<int>(4));
	EXPECT_NE(Board<int>(1), Board<int>(4));
	EXPECT_NE(Board<int>(5), Board<int>(4));
	EXPECT_NE(Board<int>(), (Board<int, 3>(2))); // [gTest]
	EXPECT_FALSE(Board<int>(4) != Board<int>(4));
	// single element difference
	Board<int, 2> B1{};
	EXPECT_FALSE((Board<int, 2>() != B1)); // [gTest]
	EXPECT_NO_THROW(B1[Location<2>(13)] = 3);
	EXPECT_NE((Board<int, 2>()), B1); // [gTest]
	// value-type: Options
	EXPECT_NE((Board<Options<4>, 2>(1)), (Board<Options<4>, 2>()));
	EXPECT_FALSE((Board<Options<4>, 2>()) != (Board<Options<4>, 2>()));
}
TEST(Board, elementaccess)
{
	Board<int, 2> B1{};
	EXPECT_FALSE(noexcept(B1.at(Location<2>(0)) = 2));
	EXPECT_FALSE(noexcept(B1.at(Location<2>(0)) == 1));
	// at(Location)
	EXPECT_NO_THROW(B1.at(Location<2>(2)) = 2);
	EXPECT_EQ(B1.at(Location<2>(2)), 2);
	EXPECT_NO_THROW(B1.at(Location<2>(0, 3)) = 3);
	EXPECT_EQ(B1.at(Location<2>(3)), 3);
	EXPECT_NO_THROW(B1.at(Location_Block<2>(0, 2)) = 4);
	EXPECT_EQ(B1.at(Location<2>(4)), 4);
	EXPECT_NO_THROW(B1.at(Location_Block<2>(0, 1, 1)) = 5);
	EXPECT_EQ(B1.at(Location<2>(5)), 5);
	EXPECT_THROW(B1.at(Location<2>(16)), std::out_of_range);
	EXPECT_THROW(B1.at(Location<2>(-1)), std::out_of_range);
	// at(Location) const
	const Board<int, 2> cB{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	EXPECT_FALSE(noexcept(cB.at(Location<2>(0)) == 1));
	EXPECT_EQ(cB.at(Location<2>(2)), 2) << "at(Location) const";
	EXPECT_THROW(cB.at(Location<2>(16)), std::out_of_range);
	EXPECT_THROW(cB.at(Location<2>(-1)), std::out_of_range);

	// at(row, col)
	EXPECT_NO_THROW(B1.at(1, 1) = 5);
	EXPECT_THROW(B1.at(1, 4), std::out_of_range);
	EXPECT_THROW(B1.at(4, 0), std::out_of_range);
	EXPECT_THROW(B1.at(-1, 0), std::out_of_range);
	EXPECT_THROW(B1.at(1, -2), std::out_of_range);
	// at(Location) const
	EXPECT_NO_THROW(cB.at(2, 1));
	EXPECT_THROW(cB.at(1, 4), std::out_of_range);
	EXPECT_THROW(cB.at(4, 0), std::out_of_range);
	EXPECT_THROW(cB.at(-1, 0), std::out_of_range);
	EXPECT_THROW(cB.at(1, -2), std::out_of_range);
	EXPECT_EQ(cB.at(0, 0), 0);
	EXPECT_EQ(cB.at(3, 1), 13);

	// operator[](Location)
	EXPECT_TRUE(noexcept(B1[Location<2>(0)] = 0));
	EXPECT_TRUE(noexcept(B1[Location<2>(0)] == 1));
	B1[Location<2>(0)] = 0;
	B1[Location<2>(1)] = 1;
	EXPECT_EQ(B1[Location<2>(0)], 0);
	EXPECT_EQ(B1[Location<2>(1)], 1);
	EXPECT_TRUE(noexcept(cB[Location<2>(0)] == 1));
	EXPECT_EQ(cB[Location<2>(5)], 5);
}
TEST(Board, default_values)
{
	// for int, all 0
	Board<int, 2> B_int{};
	EXPECT_EQ(B_int[Location<2>(0)], 0);
	EXPECT_EQ(B_int[Location<2>(13)], 0);
	//	options -> all
	Board<Options<4>, 2> B_opt{};
	EXPECT_TRUE(B_opt[Location<2>(0)].all());
	EXPECT_TRUE(B_opt[Location<2>(7)].all());
}
TEST(Board, clear)
{
	Board<int, 2> Obj(3);
	ASSERT_EQ(Obj[Location<2>(2)], 3);

	EXPECT_FALSE(noexcept(Obj.clear()));
	EXPECT_NO_THROW(Obj.clear());
	EXPECT_EQ(Obj[Location<2>(2)], 0);

	Board<Options<4>, 2> Opt(2);
	ASSERT_EQ(Opt[Location<2>(2)], 2);
	ASSERT_EQ(Opt[Location<2>(2)], Options<4>{2});

	EXPECT_FALSE(noexcept(Opt.clear()));
	EXPECT_NO_THROW(Opt.clear());
	EXPECT_EQ(Opt[Location<2>(2)], Options<4>());
}

TEST(Board, InBetween)
{
	Board<int, 2> board{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	EXPECT_TRUE(noexcept(board[0][0]));
	EXPECT_TRUE(noexcept(board[4][5]));
	EXPECT_TRUE(noexcept(board[-4][-5]));
	EXPECT_NO_THROW(board[1][0] = 9);
	EXPECT_EQ(board[1][0], 9);
	EXPECT_EQ(board[0][3], 3);
	EXPECT_EQ(board[3][3], 15);
	EXPECT_NE(board[3][3], 10);
	Board<Options<4>, 2> board2{};
	EXPECT_TRUE(noexcept(board2[0][0]));
	EXPECT_NO_THROW(board2[2][0] = 2);
	EXPECT_EQ(board2[2][0], 2);
	EXPECT_NE(board2[2][0], 1);
	// see deathtests

	// const_InBetween
	const Board<int, 2> cboard{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	EXPECT_NO_THROW(cboard[1][0]);
	EXPECT_EQ(cboard[3][3], 15);
	EXPECT_NE(cboard[3][3], 10);
	// see deathtests
}
TEST(Board, deathtests)
{
	// Board.h
	// Initializer_list construction
	EXPECT_DEBUG_DEATH({ (Board<int, 2>{0, 1, 2, 3}); },
					   ""); // [gTest]
	EXPECT_DEBUG_DEATH(
		{
			(Board<int, 2>{
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
		},
		""); // [gTest]

	// ERROR assert in vector escapes test context
	// Board<int, 2> board{};
	// EXPECT_DEBUG_DEATH({ board[4][4]; }, "") << "Out of bounds";	// [gTest]
}

} // namespace SudokuTests::BoardTest
