// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//===--- SudokuTests/Solver_find.cpp                                    ---===//
//
// Unit tests for solvers on Sudoku::Board.
//===----------------------------------------------------------------------===//
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
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Solvers_find.h>
// helpers
#include <Sudoku/Board.h>
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>
#include <Sudoku/Solver.h> // setValue
// Debug Output
#include "print_Options.h"
// library
#include <array>
#include <bitset>
#include <vector>
#include <iterator>
#include <type_traits>


namespace SudokuTests::Solvers_find
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Options;
using ::Sudoku::Value;

TEST(Solver, listWhereOptionSection)
{
	using ::Sudoku::list_where_option;
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// return type
	static_assert(std::is_same_v<
				  std::vector<loc>,
				  decltype(list_where_option<2>(B.row(0), Value{1}, 3))>);
	// 1 section, Value, rep_count
	static_assert(not noexcept(list_where_option<2>(B.row(0), Value{1}, 3)));
	static_assert(not noexcept(list_where_option<2>(B.col(0), Value{1}, 3)));
	static_assert(not noexcept(list_where_option<2>(B.block(0), Value{1}, 3)));

	// no result
	// - no result: explicit
	B[2][0] = set{"11011"};
	B[2][1] = set{"11011"};
	B[2][2] = set{"11011"};
	B[2][3] = set{"11011"};
	list    = list_where_option<2>(B.row(2), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});
	// - no result: because value is answer
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	list    = list_where_option<2>(B.row(0), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});
	// incorrect rep_count: too low
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};

	list = list_where_option<2>(B.block(0), Value{1}, 3);
	// normal operation
	B.clear();
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	list    = list_where_option<2>(B.row(0), Value{1}, 3);
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1));
	EXPECT_EQ(list[1], loc(2));
	EXPECT_EQ(list[2], loc(3));
	list = list_where_option<2>(B.col(0), Value{1}, 3);
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1, 0));
	EXPECT_EQ(list[1], loc(2, 0));
	EXPECT_EQ(list[2], loc(3, 0));
	list = list_where_option<2>(B.block(0), Value{1}, 2);
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], loc(0, 1));
	EXPECT_EQ(list[1], loc(1, 0));
}

TEST(SolverDeathTest, listWhereOptionSection)
{
	using ::Sudoku::list_where_option;
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// invalid value
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{0}, 1), ".*is_valid");
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{5}, 1), ".*is_valid");
#else
	EXPECT_ANY_THROW(list = list_where_option<2>(B.row(0), Value{5}, 1));
#endif // NDEBUG
	// invalid row id // board_sections.h
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(-1), Value{1}, 1), ".*is_valid_size");
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(4), Value{1}, 1), ".*is_valid_size");
	// invalid rep_count
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{1}, 0), ".*rep_count");
	list = list_where_option<2>(B.row(1), Value{3}, 4);
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(1), Value{3}, 5), ".*rep_count");

	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{1}, 2), ".*");
	// incorrect rep_count: too high
	// no assert: allow for use without setting a rep_count
}

TEST(Solver, listWhereOptionItr)
{
	using ::Sudoku::list_where_option;
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// 2 begin, end, Value, rep_count
	static_assert(not noexcept(
		list_where_option<2>(B.row(0).cbegin(), B.row(0).cend(), Value{1}, 3)));
#if not(defined(__ICL) && __ICL <= 1900 && defined(_DEBUG))
	static_assert(not noexcept(
		list_where_option<2>(B.row(0).begin(), B.row(0).end(), Value{1}, 3)));
#endif // __ICL
	// return type
	static_assert(std::is_same_v<
				  std::vector<loc>,
				  decltype(list_where_option<2>(
					  B.row(0).cbegin(), B.row(0).cend(), Value{1}, 3))>);

	// no result
	// - no result: explicit
	B[2][0] = set{"11011"};
	B[2][1] = set{"11011"};
	B[2][2] = set{"11011"};
	B[2][3] = set{"11011"};
	list =
		list_where_option<2>(B.row(2).cbegin(), B.row(2).cend(), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});
	// - no result: because value is answer
	list = list_where_option<2>(B.row(0), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});

	list = list_where_option<2>(B.block(0), Value{1}, 3);
	// normal operation
	B.clear();
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	list =
		list_where_option<2>(B.row(0).cbegin(), B.row(0).cend(), Value{1}, 3);
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1));
	EXPECT_EQ(list[1], loc(2));
	EXPECT_EQ(list[2], loc(3));
	list =
		list_where_option<2>(B.col(0).cbegin(), B.col(0).cend(), Value{1}, 3);
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1, 0));
	EXPECT_EQ(list[1], loc(2, 0));
	EXPECT_EQ(list[2], loc(3, 0));
	list = list_where_option<2>(
		B.block(0).cbegin(), B.block(0).cend(), Value{1}, 2);
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], loc(0, 1));
	EXPECT_EQ(list[1], loc(1, 0));
}

TEST(SolverDeathTest, listWhereOptionItr)
{
	using ::Sudoku::list_where_option;
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// invalid Value
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{0}, 1),
		".*is_valid");
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{5}, 1),
		".*is_valid");
#else
	EXPECT_ANY_THROW(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{5}, 1));
#endif // NDEBUG
	   // invalid rep_count
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(1).cbegin(), B.row(1).cend(), Value{3}, -1),
		".*rep_count");
#else
	EXPECT_ANY_THROW(
		list = list_where_option<2>(
			B.row(1).cbegin(), B.row(1).cend(), Value{3}, -1));
#endif // NDEBUG
	list =
		list_where_option<2>(B.row(1).cbegin(), B.row(1).cend(), Value{3}, 0);
	list =
		list_where_option<2>(B.row(1).cbegin(), B.row(1).cend(), Value{3}, 4);
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(1).cbegin(), B.row(1).cend(), Value{3}, 5),
		".*rep_count");
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(1).cbegin(), B.row(1).cbegin() + 2, Value{3}, 3),
		".*rep_count");

	// incorrect rep_count: too low
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{1}, 2),
		".*");
	// incorrect rep_count: too high
	// no assert: allow for use without setting a rep_count
}

TEST(Solver, listWhereOptionNoRepCount)
{
	using ::Sudoku::list_where_option;
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	std::vector<loc> list{};

	static_assert(not noexcept(list_where_option<2>(B.row(0), Value{1})));
	// static_assert(not noexcept(
	//	list_where_option<2>(B.row(0).cbegin(), B.row(0).cend(), Value{1})));

	// row/col/block
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	list = list_where_option<2>(B.row(0).cbegin(), B.row(0).cend(), Value{1});
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1));
	EXPECT_EQ(list[1], loc(2));
	EXPECT_EQ(list[2], loc(3));
	list = list_where_option<2>(B.col(0).cbegin(), B.col(0).cend(), Value{1});
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1, 0));
	EXPECT_EQ(list[1], loc(2, 0));
	EXPECT_EQ(list[2], loc(3, 0));
	list =
		list_where_option<2>(B.block(0).cbegin(), B.block(0).cend(), Value{1});
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], loc(0, 1));
	EXPECT_EQ(list[1], loc(1, 0));
}

TEST(Solver, listWhereOptionPartial)
{
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// partial section
	list = ::Sudoku::list_where_option<2>(
		B.row(0).cbegin() + 2, B.row(0).cend(), Value{1}, 2);
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], loc(2));
	EXPECT_EQ(list[1], loc(3));
}

TEST(SolverDeathTest, listWhereOptionValue)
{ // list_where_option(SectionT, Value)
	using ::Sudoku::list_where_option;
	using set     = std::bitset<5>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[2][1] = set{"11110"}; // incorrect marked as answer
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible

	// valid_value
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{0}), ".*is_valid");
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{5}), ".*is_valid");
#else
	// - Options::is_option() - std::bitset::test() std::out_of_range
	EXPECT_THROW(
		list = list_where_option<2>(B.row(0), Value{5}), std::out_of_range);
#endif // NDEBUG
}

TEST(Solver, listWhereOptionValue)
{ // list_where_option(SectionT, Value)
	using ::Sudoku::list_where_option;
	using set     = std::bitset<5>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[2][1] = set{"11110"}; // incorrect marked as answer
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible

	// row/col/block
	static_assert(not noexcept(list_where_option<2>(B.row(0), Value{1})));
	static_assert(not noexcept(list_where_option<2>(B.col(0), Value{1})));
	static_assert(not noexcept(list_where_option<2>(B.block(0), Value{1})));
	// return type
	static_assert(std::is_same_v<
				  std::vector<loc>,
				  decltype(list_where_option<2>(B.row(0), Value{1}))>);
	// valid_value
	static_assert(not noexcept(list_where_option<2>(B.row(1), Value{1})));
	static_assert(not noexcept(list_where_option<2>(B.row(0), Value{2})));
	list = list_where_option<2>(B.row(0), Value{1});
	list = list_where_option<2>(B.row(0), Value{4});

	// Do not return answered
	list = list_where_option<2>(B.row(0), Value{2});
	EXPECT_EQ(list.size(), size_t{0});
	list = list_where_option<2>(B.row(2), Value{2});
	EXPECT_EQ(list.size(), size_t{2}); // even when marked wrong

	// row/col/block
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	list    = list_where_option<2>(B.row(0), Value{1});
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1));
	EXPECT_EQ(list[1], loc(2));
	EXPECT_EQ(list[2], loc(3));
	list = list_where_option<2>(B.col(0), Value{1});
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc(1, 0));
	EXPECT_EQ(list[1], loc(2, 0));
	EXPECT_EQ(list[2], loc(3, 0));
	list = list_where_option<2>(B.block(0), Value{1});
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], loc(0, 1));
	EXPECT_EQ(list[1], loc(1, 0));
}

TEST(Solver, listWhereOptions)
{ // list_where_option(Section, Options)
	using ::Sudoku::list_where_option;
	using set     = std::bitset<5>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[2][1] = set{"11110"}; // incorrect marked as answer
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible
	static_assert(noexcept(list_where_option<2>(B.row(0), Options{Value{1}})));
	static_assert(noexcept(list_where_option<2>(B.row(0), B[0][2])));
	list = list_where_option<2>(B.row(0), Options{Value{0}});
	list = list_where_option<2>(B.row(0), Options{Value{4}});
	// return type
	static_assert(std::is_same_v<
				  std::vector<loc>,
				  decltype(list_where_option<2>(B.row(0), Options{Value{1}}))>);
	list = list_where_option<2>(B.row(0), Options{Value{0}});
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc{1});
	EXPECT_EQ(list[1], loc{2});
	EXPECT_EQ(list[2], loc{3});
	list = list_where_option<2>(B.row(0), Options{Value{2}});
	EXPECT_EQ(list.size(), size_t{0});
	// ignore answer-bit in sample
	list = list_where_option<2>(B.row(0), Options{set{"11010"}});
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], loc{1});
	EXPECT_EQ(list[1], loc{3});
	list = list_where_option<2>(B.row(0), Options{set{"11011"}});
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], loc{1});
	EXPECT_EQ(list[1], loc{3});
	// Do not return answered locations
	list = list_where_option<2>(B.row(0), Options{set{"00100"}});
	EXPECT_EQ(list.size(), size_t{0});
	list = list_where_option<2>(B.row(2), Options{set{"00100"}});
	EXPECT_EQ(list.size(), size_t{2}); // even when marked wrong
}

TEST(SolverDeathTest, listWhereOptions)
{ // list_where_option(Section, Options)
	using ::Sudoku::list_where_option;
	using set     = std::bitset<5>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[2][1] = set{"11110"}; // incorrect marked as answer
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	// caught in Options constructor
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Options{Value{5}}), ".*<= Value");
}

TEST(Solver, listWhereEqual)
{
	using ::Sudoku::list_where_equal;
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};

	static_assert(
		noexcept(list_where_equal<2>(B.row(0), Options<4>{Value{1}})));
	static_assert(noexcept(list_where_equal<2>(B.row(0), B[0][1])));

	static_assert(std::is_same_v<
				  std::vector<loc>,
				  decltype(list_where_equal<2>(B.row(0), Options<4>{}))>);
	const Options<4> value{set{"10011"}};
	EXPECT_NO_FATAL_FAILURE(list = list_where_equal<2>(B.row(0), value));
	EXPECT_EQ(list.size(), size_t{1});
	EXPECT_EQ(list[0], loc(2));

	EXPECT_NO_FATAL_FAILURE(
		list = list_where_equal<2>(B.row(0), Options<4>{Value{1}}));
	EXPECT_EQ(list.size(), size_t{0});
	EXPECT_NO_FATAL_FAILURE(
		list = list_where_equal<2>(B.row(0), Options<4>{Value{2}}));
	EXPECT_EQ(list.size(), size_t{1});
	EXPECT_EQ(list[0], Location<2>{0});
	EXPECT_NO_FATAL_FAILURE(list = list_where_equal<2>(B.row(0), B[0][1]));
	EXPECT_EQ(list.size(), size_t{2});
	EXPECT_EQ(list[0], Location<2>(0, 1));
	EXPECT_EQ(list[1], Location<2>(0, 3));
}

TEST(Solver, listWhereSubset)
{
	using ::Sudoku::list_where_subset;
	using L       = Location<2>;
	using B       = std::bitset<5>;
	using Options = Options<4>;
	Board<Options, 2> board{};
	board.at(L{0})    = B{"01111"};
	board.at(L{1})    = B{"11111"}; // should be 10000
	board.at(L{2})    = B{"00111"};
	board.at(L{3})    = B{"00101"};
	board.at(L{1, 0}) = B{"00100"}; // answer 2
	board.at(L{1, 1}) = B{"11110"}; // incorrect marked as answer
	board.at(L{1, 2}) = B{"11111"};
	board.at(L{1, 3}) = B{"11111"};
	board.at(L{2, 0}) = B{"11111"};
	board.at(L{2, 1}) = B{"11111"};
	board.at(L{2, 2}) = B{"11111"};
	board.at(L{2, 3}) = B{"11111"};
	board.at(L{3, 0}) = B{"11111"};
	board.at(L{3, 1}) = B{"11111"};
	board.at(L{3, 2}) = B{"11111"};
	board.at(L{3, 3}) = B{"11111"};

	// return type
	static_assert(std::is_same_v<
				  std::vector<L>,
				  decltype(list_where_subset(board, Options()))>);
	static_assert(std::is_same_v<
				  std::vector<L>,
				  decltype(list_where_subset<2>(board.row(L{0}), Options()))>);

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible
	static_assert(noexcept(list_where_subset(board, Options{})));
	static_assert(noexcept(list_where_subset<2>(board.row(L{0}), Options())));

	// inputs
	// normal sample
	Options item{B{"00111"}};
	auto result = list_where_subset(board, item);
	ASSERT_EQ(result.size(), 2U);
	EXPECT_EQ(result[0], L{2});
	EXPECT_EQ(result[1], L{3});
	result = list_where_subset(board, board[L{0}]);
	ASSERT_EQ(result.size(), 3U);
	EXPECT_EQ(result[0], L{0});
	EXPECT_EQ(result[2], L{3});
	result = list_where_subset(board, Options{B{"00101"}});
	EXPECT_EQ(result.size(), 1U);
	// answer sample
	result = list_where_subset(board, Options{Value{2}});
	ASSERT_EQ(result.size(), 0U);
	// full sample
	result = list_where_subset(board, Options());
	ASSERT_EQ(result.size(), 14U); // 2 marked as answer
	EXPECT_EQ(result[0], L(0));
	EXPECT_EQ(result[13], L(15));
	// empty sample
	result = list_where_subset(board, Options{B{"00000"}});
	ASSERT_EQ(result.size(), 0U);
	result = list_where_subset(board, Options{B{"00001"}});
	ASSERT_EQ(result.size(), 0U);
	// invalid sample (answer-bit set)
	result = list_where_subset(board, Options{B{"11110"}});
	ASSERT_EQ(result.size(), 0U);

	// only on the row:
	item               = B{"00111"};
	const auto section = board.row(L{0});
	result             = list_where_subset<2>(section, item);
	ASSERT_EQ(result.size(), 2U);
	EXPECT_EQ(result[0], L{2});
	EXPECT_EQ(result[1], L{3});
	result = list_where_subset<2>(section, board[L{0}]);
	ASSERT_EQ(result.size(), 3U);
	EXPECT_EQ(result[0], L{0});
	EXPECT_EQ(result[2], L{3});
	result = list_where_subset<2>(section, Options{B{"00101"}});
	EXPECT_EQ(result.size(), 1U);
	// answer sample
	result = list_where_subset<2>(section, Options{Value{2}});
	ASSERT_EQ(result.size(), 0U);
	// full sample
	result = list_where_subset<2>(section, Options());
	ASSERT_EQ(result.size(), 4U); // 2 marked as answer
	EXPECT_EQ(result[0], L(0));
	EXPECT_EQ(result[3], L(3));
	// empty sample
	result = list_where_subset<2>(section, Options{B{"00000"}});
	ASSERT_EQ(result.size(), 0U);
	result = list_where_subset<2>(section, Options{B{"00001"}});
	ASSERT_EQ(result.size(), 0U);
	// invalid sample (answer-bit set)
	result = list_where_subset<2>(section, Options{B{"11110"}});
	ASSERT_EQ(result.size(), 0U);
}

TEST(SolverDeathTest, listWhereAnyOption)
{
	using ::Sudoku::list_where_any_option;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	std::vector<loc> list{};

	// caught in Options constructor
	EXPECT_DEBUG_DEATH(
		list = list_where_any_option<2>(B.row(0), Options{Value{5}}),
		".*<= Value");
}

TEST(Solver, listWhereAnyOption)
{
	using ::Sudoku::list_where_any_option;
	using set     = std::bitset<5>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	std::vector<loc> list{};
	// B[1][0] = set{"01011"};
	// B[1][1] = set{"11001"};
	// B[2][0] = set{"11011"};
	// B[2][1] = set{"11110"}; // incorrect marked as answer
	// B[3][0] = set{"10011"};

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible
	static_assert(
		noexcept(list_where_any_option<2>(B.row(0), Options{Value{1}})));
	static_assert(noexcept(list_where_any_option<2>(B.row(0), B[0][2])));
	list = list_where_any_option<2>(B.row(0), Options{Value{0}});
	list = list_where_any_option<2>(B.row(0), Options{Value{4}});

	// return type
	static_assert(
		std::is_same_v<
			std::vector<loc>,
			decltype(list_where_any_option<2>(B.row(0), Options{Value{1}}))>);
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	list    = list_where_any_option<2>(B.row(0), Options{set{"11000"}});
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc{1});
	list = list_where_any_option<2>(B.row(0), Options{set{"01000"}});
	EXPECT_EQ(list.size(), size_t{2});
	if (list.size() > size_t{0})
	{
		EXPECT_EQ(list[0], loc{1});
	}
	if (list.size() > size_t{1})
	{
		EXPECT_EQ(list[1], loc{3});
	}
	// answered
	list = list_where_any_option<2>(B.row(0), Options{set{"00100"}});
	EXPECT_EQ(list.size(), size_t{0});
	// answer-bit
	EXPECT_EQ(
		list_where_any_option<2>(B.row(0), Options{set{"01000"}}),
		list_where_any_option<2>(B.row(0), Options{set{"01001"}}));
	// all
	list = list_where_any_option<2>(B.row(0), Options{set{"11111"}});
	EXPECT_EQ(list.size(), size_t{3});
	// incorrect marked as answer
	B[0][0] = set{"11100"};
	list    = list_where_any_option<2>(B.row(0), Options{set{"00101"}});
	EXPECT_EQ(list.size(), size_t{0});
	// col
	B[0][2] = set{"11101"};
	B[1][2] = set{"11111"};
	B[2][2] = set{"10011"};
	B[3][2] = set{"11011"};
	list    = list_where_any_option<2>(B.col(2), Options{set{"00101"}});
	EXPECT_EQ(list.size(), size_t{2});
	if (list.size() > size_t{0})
	{
		EXPECT_EQ(list[0], loc(0, 2));
	}
	if (list.size() > size_t{1})
	{
		EXPECT_EQ(list[1], loc(1, 2));
	}
	list = list_where_any_option<2>(B.col(2), Options{set{"01100"}});
	EXPECT_EQ(list.size(), size_t{3});
	if (list.size() > size_t{2})
	{
		EXPECT_EQ(list[2], loc(3, 2));
	}
}

TEST(Solver, appearanceOnce)
{
	using ::Sudoku::appearance_once;
	// clang-format off
	constexpr std::array<char, 16> v1
	{
		// start     // after set_Value
		0, 0,  1, 0, //
		1, 0,  0, 0, //
		0, 1,  0, 0, //
		0, 0,  0, 0  // 0 0 0 1
	}; // clang-format on
	Board<Options<4>, 2> B1;
	Options<4> result{};

	// Using iterators
	EXPECT_NO_THROW(set_Value(B1, v1.cbegin(), v1.cend()));
	EXPECT_EQ(B1[3][3].count(), 4U);
	EXPECT_TRUE(is_option(B1[3][3], Value{1}));
	// on row
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.row(3).cbegin(), B1.row(3).cend()));
	EXPECT_EQ(result.count_all(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	// on col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.col(3).cbegin(), B1.col(3).cend()));
	EXPECT_EQ(result.count_all(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	// on block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.block(3).cbegin(), B1.block(3).cend()));
	EXPECT_EQ(result.count_all(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	// nothing
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.row(0).cbegin(), B1.row(0).cend()));
	EXPECT_EQ(result.count_all(), 0U);
	EXPECT_FALSE(result.test(Value{1}));

	// Using Sections
	// Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(3)));
	EXPECT_EQ(result.count_all(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "00011");
	// Col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.col(3)));
	EXPECT_EQ(result.count_all(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "00011");
	// Block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.block(3)));
	EXPECT_EQ(result.count_all(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "00011");
	// nothing, Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(0)));
	EXPECT_EQ(result.count_all(), 0U);   // <==
	EXPECT_FALSE(result.test(Value{1})); // <==
	EXPECT_FALSE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "00001");
	//===------------------------------------------------------------------===//
	// also in the test: appearance_sets
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	{
		Board<Options<4>, 2> B4{};
		constexpr std::array<char, 16> v4{
			0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
		set_Value(B4, v4.cbegin(), v4.cend());
		EXPECT_EQ(B4[0][0].count(), 3U);
		EXPECT_EQ(B4[0][1].count(), 3U);
		EXPECT_EQ(B4[0][2].count(), 3U);
		EXPECT_EQ(B4[0][3].count(), 4U);
		EXPECT_EQ(B4[2][1].count(), 4U);
		auto result4 = appearance_once<2>(B4.row(0));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_EQ(result4.count_all(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "00011");
		result4 = appearance_once<2>(B4.row(1));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 0U);
		EXPECT_EQ(result4.count_all(), 0U);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "00001");
		result4 = appearance_once<2>(B4.row(2));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_EQ(result4.count_all(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "00011");
		result4 = appearance_once<2>(B4.col(3));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_EQ(result4.count_all(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "00011");
		result4 = appearance_once<2>(B4.block(1));
		EXPECT_TRUE(result4[Value{0}]); // answer bit
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_EQ(result4.count_all(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "00011");
	}
	{ // all set in a section
		Board<Options<4>, 2> B5{};
		constexpr std::array<char, 16> v5{
			1, 2, 3, 4, 3, 4, 1, 2, 2, 1, 4, 3, 4, 3, 2, 1};
		set_Value(B5, v5.cbegin(), v5.cend());
		EXPECT_TRUE(is_answer(B5[0][0], Value{1}));
		EXPECT_TRUE(is_answer(B5[0][1], Value{2}));
		EXPECT_TRUE(is_answer(B5[0][2], Value{3}));
		EXPECT_TRUE(is_answer(B5[0][3], Value{4}));
		const auto result1 = appearance_once<2>(B5.row(0));
		EXPECT_TRUE(result1[Value{0}]); // answer bit
		EXPECT_EQ(result1.count(), 0U);
		EXPECT_EQ(result1.count_all(), 0U);
		EXPECT_FALSE(result1.is_empty());
		EXPECT_EQ(result1.DebugString(), "00001");
		const auto result2 =
			appearance_once<2>(B5.row(0).cbegin(), B5.row(0).cend());
		EXPECT_TRUE(result2[Value{0}]); // answer bit
		EXPECT_FALSE(result2.is_empty());
		EXPECT_EQ(result2.DebugString(), "00001");
		const auto result3 = appearance_once<2>(B5.row(3));
		EXPECT_TRUE(result3[Value{0}]); // answer bit
		EXPECT_EQ(result3.count(), 0U);
		EXPECT_FALSE(result3.is_empty());
		EXPECT_EQ(result3.DebugString(), "00001");
	}
}

TEST(Solver, appearanceSets)
{
	using ::Sudoku::appearance_sets;
	// Example as shown in implementation
	Board<Options<9>, 3> board{};
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][1] = std::bitset<10>{"1101000101"};
	board[0][2] = std::bitset<10>{"0100000111"};
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][6] = std::bitset<10>{"1011001101"};
	board[0][7] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"};
	const Options<9> Ans_0{std::bitset<10>{"0000000001"}}; // complete, 1 answer
	const Options<9> Ans_1{std::bitset<10>{"0000100001"}}; // 5 and 1 answer
	const Options<9> Ans_2{std::bitset<10>{"0010000001"}}; // 7, <2 answers
	const Options<9> Ans_3{std::bitset<10>{"0100001001"}}; // 3 8, <3 answers

	// Using iterators
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// reorder, should give same result
	// set answer last
	board[0][8] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][5] = std::bitset<10>{"0100001011"};
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<10>{"1001100101"};
	board[0][7] = std::bitset<10>{"0000010000"}; // ans 4
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set value appearing 3 times last
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][7] = std::bitset<10>{"1101000101"}; // 1 -> 7
	board[0][6] = std::bitset<10>{"0100000111"}; // 2 -> 6
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][2] = std::bitset<10>{"1011001101"};
	board[0][1] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"}; // 8 -> 8
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// Using Sections
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][1] = std::bitset<10>{"1101000101"};
	board[0][2] = std::bitset<10>{"0100000111"};
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][6] = std::bitset<10>{"1011001101"};
	board[0][7] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"};
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// reorder, should give same result
	// set answer last
	board[0][8] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][5] = std::bitset<10>{"0100001011"};
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<10>{"1001100101"};
	board[0][7] = std::bitset<10>{"0000010000"}; // ans 4
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===------------------------------------------------------------------===//
	// Col
	// Example as shown in implementation
	Board<Options<9>, 3> B2{};
	B2[0][0] = std::bitset<10>{"1000000011"};
	B2[1][0] = std::bitset<10>{"1101000101"};
	B2[2][0] = std::bitset<10>{"0100000111"};
	B2[3][0] = std::bitset<10>{"0000000111"};
	B2[4][0] = std::bitset<10>{"1011001111"};
	B2[5][0] = std::bitset<10>{"0000010000"}; // ans 4
	B2[6][0] = std::bitset<10>{"1011001101"};
	B2[7][0] = std::bitset<10>{"1001100101"};
	B2[8][0] = std::bitset<10>{"0100001011"};
	{ // Using iterators
		const auto result =
			appearance_sets<3>(B2.col(0).cbegin(), B2.col(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	{ // using Col
		const auto result = appearance_sets<3>(B2.col(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===------------------------------------------------------------------===//
	// Block
	using B = ::Sudoku::Location_Block<3>;
	Board<Options<9>, 3> B3{};
	B3[B{2, 0}] = std::bitset<10>{"1000000011"};
	B3[B(2, 1)] = std::bitset<10>{"1101000101"};
	B3[B(2, 2)] = std::bitset<10>{"0100000111"};
	B3[B(2, 3)] = std::bitset<10>{"0000000111"};
	B3[B(2, 4)] = std::bitset<10>{"1011001111"};
	B3[B(2, 5)] = std::bitset<10>{"0000010000"}; // ans 4
	B3[B(2, 6)] = std::bitset<10>{"1011001101"};
	B3[B(2, 7)] = std::bitset<10>{"1001100101"};
	B3[B(2, 8)] = std::bitset<10>{"0100001011"};
	{ // using iterators
		const auto result =
			appearance_sets<3>(B3.block(2).cbegin(), B3.block(2).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	{ // using Block
		const auto result = appearance_sets<3>(B3.block(2));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===------------------------------------------------------------------===//
	// also in the test: appearance_once
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	{
		Board<Options<4>, 2> B4{};
		constexpr std::array<char, 16> v4{
			0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
		set_Value(B4, v4.cbegin(), v4.cend());
		EXPECT_EQ(B4[0][0].count(), 3U);
		EXPECT_EQ(B4[0][1].count(), 3U);
		EXPECT_EQ(B4[0][2].count(), 3U);
		EXPECT_EQ(B4[0][3].count(), 4U);
		EXPECT_EQ(B4[2][1].count(), 4U);
		auto result = appearance_sets<2>(B4.row(0));
		EXPECT_EQ(result.size(), size_t{3}); // max = N
		EXPECT_FALSE(result[0].is_empty());
		EXPECT_EQ(result[0].DebugString(), "00001");
		EXPECT_TRUE(result[1][Value{0}]); // answer bit
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
		EXPECT_EQ(result[2].count(), 0U);
		result = appearance_sets<2>(B4.row(1));
		EXPECT_FALSE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 0U);
		EXPECT_FALSE(result[1].is_empty());
		EXPECT_FALSE(result[2].is_empty());
		result = appearance_sets<2>(B4.row(2));
		EXPECT_FALSE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
		result = appearance_sets<2>(B4.col(3));
		EXPECT_FALSE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
		result = appearance_sets<2>(B4.block(1));
		EXPECT_FALSE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
	}
}

} // namespace SudokuTests::Solvers_find
