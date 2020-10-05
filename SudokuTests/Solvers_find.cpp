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
#include "print_Options.h" // Debug Output

#include <Strategic/Solver.h>       // setValue
#include <Strategic/Solvers_find.h> // Class under test
#include <Sudoku/Board.h>
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>

#include <array>
#include <bitset>
#include <vector>

#include <iterator>
#include <type_traits>

#include <gtest/gtest.h>


namespace SudokuTests::Solvers_find
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Options;
using ::Sudoku::Value;

TEST(Solver, listWhereOptionSection)
{
	using ::Sudoku::list_where_option;
	using set = std::bitset<4>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
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
	B[2][0] = set{"1101"};
	B[2][1] = set{"1101"};
	B[2][2] = set{"1101"};
	B[2][3] = set{"1101"};
	list    = list_where_option<2>(B.row(2), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});
	// - no result: because value is answer
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	list    = list_where_option<2>(B.row(0), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});
	// incorrect rep_count: too low
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};

	list = list_where_option<2>(B.block(0), Value{1}, 3);
	// normal operation
	B.clear();
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
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
	using set = std::bitset<4>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
	std::vector<loc> list{};

	// invalid value
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{0}, 1), ".*is_valid");
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{5}, 1), ".*is_valid");
#else
	EXPECT_THROW(
		list = list_where_option<2>(B.row(0), Value{0}, 1),
		Sudoku::error::invalid_option);
	EXPECT_THROW(
		list = list_where_option<2>(B.row(0), Value{5}, 1),
		Sudoku::error::invalid_option);
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
	using set = std::bitset<4>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
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
	B[2][0] = set{"1101"};
	B[2][1] = set{"1101"};
	B[2][2] = set{"1101"};
	B[2][3] = set{"1101"};
	list =
		list_where_option<2>(B.row(2).cbegin(), B.row(2).cend(), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});
	// - no result: because value is answer
	list = list_where_option<2>(B.row(0), Value{2}, 4);
	EXPECT_EQ(list.size(), size_t{0});

	list = list_where_option<2>(B.block(0), Value{1}, 3);
	// normal operation
	B.clear();
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
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
	using set = std::bitset<4>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
	std::vector<loc> list{};

	// invalid Value
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{0}, 1),
		".*is_valid");
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{5}, 1),
		".*is_valid");
#else
	EXPECT_THROW(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{0}, 1),
		Sudoku::error::invalid_option);
	EXPECT_THROW(
		list = list_where_option<2>(
			B.row(0).cbegin(), B.row(0).cend(), Value{5}, 1),
		Sudoku::error::invalid_option);
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
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
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
	using set = std::bitset<4>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	std::vector<loc> list{};

	static_assert(not noexcept(list_where_option<2>(B.row(0), Value{1})));
	// static_assert(not noexcept(
	//	list_where_option<2>(B.row(0).cbegin(), B.row(0).cend(), Value{1})));

	// row/col/block
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
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
	using set = std::bitset<4>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
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
	using set     = std::bitset<4>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[2][1] = set{"1111"}; // incorrect marked as answer
	B[3][0] = set{"1001"};
	std::vector<loc> list{};

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible

	// valid_value
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{0}), ".*is_valid");
	EXPECT_DEBUG_DEATH(
		list = list_where_option<2>(B.row(0), Value{5}), ".*is_valid");
#else
	EXPECT_THROW(
		list = list_where_option<2>(B.row(0), Value{0}),
		Sudoku::error::invalid_option);
	EXPECT_THROW(
		list = list_where_option<2>(B.row(0), Value{5}),
		Sudoku::error::invalid_option);
#endif // NDEBUG
}

TEST(Solver, listWhereOptionValue)
{ // list_where_option(SectionT, Value)
	using ::Sudoku::list_where_option;
	using set     = std::bitset<4>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[2][1] = set{"1111"};
	B[3][0] = set{"1001"};
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
	list = list_where_option<2>(B.row(2), Value{2});
	EXPECT_EQ(list.size(), size_t{3});

	// Do not return answered
	list = list_where_option<2>(B.row(0), Value{2});
	EXPECT_EQ(list.size(), size_t{0});

	// row/col/block
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
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
	using set     = std::bitset<4>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[2][1] = set{"1111"}; // incorrect marked as answer
	B[3][0] = set{"1001"};
	std::vector<loc> list{};

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible
	static_assert(
		not noexcept(list_where_option<2>(B.row(0), Options{Value{1}})));
	constexpr Sudoku::OptionValue<4> value{1u};
	static_assert(noexcept(list_where_option<2>(B.row(0), Options{value})));
	static_assert(noexcept(list_where_option<2>(B.row(0), B[0][2])));
	list = list_where_option<2>(B.row(0), Options{Value{4}});
	// return type
	static_assert(std::is_same_v<
				  std::vector<loc>,
				  decltype(list_where_option<2>(B.row(0), Options{Value{1}}))>);
	list = list_where_option<2>(B.row(0), Options{Value{2}});
	EXPECT_EQ(list.size(), size_t{0});
	list = list_where_option<2>(B.row(2), Options{set{"00100"}});
	EXPECT_EQ(list.size(), size_t{3});
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
}

TEST(SolverDeathTest, listWhereOptions)
{ // list_where_option(Section, Options)
	using ::Sudoku::list_where_option;
	using set     = std::bitset<4>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[2][1] = set{"1111"}; // incorrect marked as answer
	B[3][0] = set{"1001"};
	std::vector<loc> list{};

	// caught in Options constructor
	EXPECT_THROW(
		list = list_where_option<2>(B.row(0), Options{Value{5}}),
		Sudoku::error::invalid_option);
}

TEST(Solver, listWhereEqual)
{
	using ::Sudoku::list_where_equal;
	using set = std::bitset<4>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	B[1][0] = set{"0101"};
	B[1][1] = set{"1100"};
	B[2][0] = set{"1101"};
	B[3][0] = set{"1001"};
	std::vector<loc> list{};

	const Options<4> value{set{"1001"}};
	static_assert(noexcept(list_where_equal<2>(B.row(0), value)));
	static_assert(noexcept(list_where_equal<2>(B.row(0), B[0][1])));

	static_assert(std::is_same_v<
				  std::vector<loc>,
				  decltype(list_where_equal<2>(B.row(0), Options<4>{}))>);
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
	using B       = std::bitset<4>;
	using Options = Options<4>;
	Board<Options, 2> board{};
	board.at(L{0})    = B{"0111"};
	board.at(L{1})    = B{"1111"}; // should be 10000
	board.at(L{2})    = B{"0011"};
	board.at(L{3})    = B{"0010"};
	board.at(L{1, 0}) = B{"0010"}; // answer 2
	board.at(L{1, 1}) = B{"1111"};
	board.at(L{1, 2}) = B{"1111"};
	board.at(L{1, 3}) = B{"1111"};
	board.at(L{2, 0}) = B{"1111"};
	board.at(L{2, 1}) = B{"1111"};
	board.at(L{2, 2}) = B{"1111"};
	board.at(L{2, 3}) = B{"1111"};
	board.at(L{3, 0}) = B{"1111"};
	board.at(L{3, 1}) = B{"1111"};
	board.at(L{3, 2}) = B{"1111"};
	board.at(L{3, 3}) = B{"1111"};

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
	Options item{B{"0011"}};
	auto result = list_where_subset(board, item);
	ASSERT_EQ(result.size(), 3U);
	EXPECT_EQ(result[0], L{2});
	EXPECT_EQ(result[1], L{3});
	EXPECT_EQ(result[2], L{4});
	result = list_where_subset(board, board[L{0}]);
	ASSERT_EQ(result.size(), 4U);
	EXPECT_EQ(result[0], L{0});
	EXPECT_EQ(result[2], L{3});
	result = list_where_subset(board, Options{B{"0010"}});
	EXPECT_EQ(result.size(), 2U);
	// answer sample
	result = list_where_subset(board, Options{Value{2}});
	ASSERT_EQ(result.size(), 2U);
	// full sample
	result = list_where_subset(board, Options());
	ASSERT_EQ(result.size(), 16U); // 2 marked as answer
	EXPECT_EQ(result[0], L(0));
	EXPECT_EQ(result[15], L(15));
	// empty sample
	result = list_where_subset(board, Options{B{"0000"}});
	ASSERT_EQ(result.size(), 0U);

	// only on the row:
	item               = B{"0011"};
	const auto section = board.row(L{0});
	result             = list_where_subset<2>(section, item);
	ASSERT_EQ(result.size(), 2U);
	EXPECT_EQ(result[0], L{2});
	EXPECT_EQ(result[1], L{3});
	result = list_where_subset<2>(section, board[L{0}]);
	ASSERT_EQ(result.size(), 3U);
	EXPECT_EQ(result[0], L{0});
	EXPECT_EQ(result[2], L{3});
	result = list_where_subset<2>(section, Options{B{"0010"}});
	EXPECT_EQ(result.size(), 1U);
	// answer sample
	result = list_where_subset<2>(section, Options{Value{2}});
	ASSERT_EQ(result.size(), 1U);
	// full sample
	result = list_where_subset<2>(section, Options());
	ASSERT_EQ(result.size(), 4U); // 2 marked as answer
	EXPECT_EQ(result[0], L(0));
	EXPECT_EQ(result[3], L(3));
	// empty sample
	result = list_where_subset<2>(section, Options{B{"0000"}});
	ASSERT_EQ(result.size(), 0U);
}

TEST(Solver, listWhereAnyOption)
{
	using ::Sudoku::list_where_any_option;
	using set     = std::bitset<4>;
	using loc     = Location<2>;
	using Options = Options<4>;
	Board<Options, 2> B{};
	std::vector<loc> list{};
	// B[1][0] = set{"0101"};
	// B[1][1] = set{"1100"};
	// B[2][0] = set{"1101"};
	// B[2][1] = set{"1111"}; // incorrect marked as answer
	// B[3][0] = set{"1001"};

	// exceptions
	// - vector out-of-memory
	// - push_back: strong exception guarantee
	//	 Location is nothrow move constructible
	const Options value{Value{1}};
	static_assert(noexcept(list_where_any_option<2>(B.row(0), value)));
	static_assert(noexcept(list_where_any_option<2>(B.row(0), B[0][2])));
	list = list_where_any_option<2>(B.row(0), Options{Value{4}});

	// caught in OptionValue constructor
	EXPECT_THROW(
		list = list_where_any_option<2>(B.row(0), Options{Value{5}}),
		Sudoku::error::invalid_option);


	// return type
	static_assert(
		std::is_same_v<
			std::vector<loc>,
			decltype(list_where_any_option<2>(B.row(0), Options{Value{1}}))>);
	B[0][0] = set{"0010"}; // ans 2
	B[0][1] = set{"1101"};
	B[0][2] = set{"1001"};
	B[0][3] = set{"1101"};
	list    = list_where_any_option<2>(B.row(0), Options{set{"1100"}});
	EXPECT_EQ(list.size(), size_t{3});
	EXPECT_EQ(list[0], loc{1});
	list = list_where_any_option<2>(B.row(0), Options{set{"0100"}});
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
	list = list_where_any_option<2>(B.row(0), Options{set{"0010"}});
	EXPECT_EQ(list.size(), size_t{0});
	// all
	list = list_where_any_option<2>(B.row(0), Options{set{"1111"}});
	EXPECT_EQ(list.size(), size_t{3});
	// col
	B[0][2] = set{"1110"};
	B[1][2] = set{"1111"};
	B[2][2] = set{"1001"};
	B[3][2] = set{"1101"};
	list    = list_where_any_option<2>(B.col(2), Options{set{"0010"}});
	EXPECT_EQ(list.size(), size_t{2});
	if (list.size() > size_t{0})
	{
		EXPECT_EQ(list[0], loc(0, 2));
	}
	if (list.size() > size_t{1})
	{
		EXPECT_EQ(list[1], loc(1, 2));
	}
	list = list_where_any_option<2>(B.col(2), Options{set{"0110"}});
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
	EXPECT_EQ(result.count(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	// on col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.col(3).cbegin(), B1.col(3).cend()));
	EXPECT_EQ(result.count(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	// on block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.block(3).cbegin(), B1.block(3).cend()));
	EXPECT_EQ(result.count(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	// nothing
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.row(0).cbegin(), B1.row(0).cend()));
	EXPECT_EQ(result.count(), 0U);
	EXPECT_FALSE(result.test(Value{1}));

	// Using Sections
	// Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(3)));
	EXPECT_EQ(result.count(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "0001");
	// Col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.col(3)));
	EXPECT_EQ(result.count(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "0001");
	// Block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.block(3)));
	EXPECT_EQ(result.count(), 1U);
	EXPECT_TRUE(result.test(Value{1}));
	EXPECT_FALSE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "0001");
	// nothing, Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(0)));
	EXPECT_EQ(result.count(), 0U);       // <==
	EXPECT_FALSE(result.test(Value{1})); // <==
	EXPECT_TRUE(result.is_empty());
	EXPECT_EQ(result.DebugString(), "0000");
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
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "0001");
		result4 = appearance_once<2>(B4.row(1));
		EXPECT_EQ(result4.count(), 0U);
		EXPECT_TRUE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "0000");
		result4 = appearance_once<2>(B4.row(2));
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "0001");
		result4 = appearance_once<2>(B4.col(3));
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "0001");
		result4 = appearance_once<2>(B4.block(1));
		EXPECT_EQ(result4.count(), 1U);
		EXPECT_TRUE(result4[Value{1}]);
		EXPECT_FALSE(result4.is_empty());
		EXPECT_EQ(result4.DebugString(), "0001");
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
		EXPECT_EQ(result1.count(), 0U);
		EXPECT_TRUE(result1.is_empty());
		EXPECT_EQ(result1.DebugString(), "0000");
		const auto result2 =
			appearance_once<2>(B5.row(0).cbegin(), B5.row(0).cend());
		EXPECT_TRUE(result2.is_empty());
		EXPECT_EQ(result2.DebugString(), "0000");
		const auto result3 = appearance_once<2>(B5.row(3));
		EXPECT_EQ(result3.count(), 0U);
		EXPECT_TRUE(result3.is_empty());
		EXPECT_EQ(result3.DebugString(), "0000");
	}
}

TEST(Solver, appearanceSets)
{
	using ::Sudoku::appearance_sets;
	// Example as shown in implementation
	Board<Options<9>, 3> board{};
	board[0][0] = std::bitset<9>{"100000001"};
	board[0][1] = std::bitset<9>{"110100010"};
	board[0][2] = std::bitset<9>{"010000011"};
	board[0][3] = std::bitset<9>{"000000011"};
	board[0][4] = std::bitset<9>{"101100111"};
	board[0][5] = std::bitset<9>{"000001000"}; // ans 4
	board[0][6] = std::bitset<9>{"101100110"};
	board[0][7] = std::bitset<9>{"100110010"};
	board[0][8] = std::bitset<9>{"010000101"};
	const Options<9> Ans_0{std::bitset<9>{"000000000"}}; // complete, 1 answer
	const Options<9> Ans_1{std::bitset<9>{"000010000"}}; // 5 and 1 answer
	const Options<9> Ans_2{std::bitset<9>{"001000000"}}; // 7, <2 answers
	const Options<9> Ans_3{std::bitset<9>{"010000100"}}; // 3 8, <3 answers

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
	board[0][8] = std::bitset<9>{"000001000"}; // ans 4
	board[0][5] = std::bitset<9>{"010000101"};
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<9>{"100110010"};
	board[0][7] = std::bitset<9>{"000001000"}; // ans 4
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set value appearing 3 times last
	board[0][0] = std::bitset<9>{"100000001"};
	board[0][7] = std::bitset<9>{"110100010"}; // 1 -> 7
	board[0][6] = std::bitset<9>{"010000011"}; // 2 -> 6
	board[0][3] = std::bitset<9>{"000000011"};
	board[0][4] = std::bitset<9>{"101100111"};
	board[0][5] = std::bitset<9>{"000001000"}; // ans 4
	board[0][2] = std::bitset<9>{"101100110"};
	board[0][1] = std::bitset<9>{"100110010"};
	board[0][8] = std::bitset<9>{"010000101"}; // 8 -> 8
	{
		const auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// Using Sections
	board[0][0] = std::bitset<9>{"100000001"};
	board[0][1] = std::bitset<9>{"110100010"};
	board[0][2] = std::bitset<9>{"010000011"};
	board[0][3] = std::bitset<9>{"000000011"};
	board[0][4] = std::bitset<9>{"101100111"};
	board[0][5] = std::bitset<9>{"000001000"}; // ans 4
	board[0][6] = std::bitset<9>{"101100110"};
	board[0][7] = std::bitset<9>{"100110010"};
	board[0][8] = std::bitset<9>{"010000101"};
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// reorder, should give same result
	// set answer last
	board[0][8] = std::bitset<9>{"000001000"}; // ans 4
	board[0][5] = std::bitset<9>{"010000101"};
	{
		const auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<9>{"100110010"};
	board[0][7] = std::bitset<9>{"000001000"}; // ans 4
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
	B2[0][0] = std::bitset<9>{"100000001"};
	B2[1][0] = std::bitset<9>{"110100010"};
	B2[2][0] = std::bitset<9>{"010000011"};
	B2[3][0] = std::bitset<9>{"000000011"};
	B2[4][0] = std::bitset<9>{"101100111"};
	B2[5][0] = std::bitset<9>{"000001000"}; // ans 4
	B2[6][0] = std::bitset<9>{"101100110"};
	B2[7][0] = std::bitset<9>{"100110010"};
	B2[8][0] = std::bitset<9>{"010000101"};
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
	B3[B{2, 0}] = std::bitset<9>{"100000001"};
	B3[B(2, 1)] = std::bitset<9>{"110100010"};
	B3[B(2, 2)] = std::bitset<9>{"010000011"};
	B3[B(2, 3)] = std::bitset<9>{"000000011"};
	B3[B(2, 4)] = std::bitset<9>{"101100111"};
	B3[B(2, 5)] = std::bitset<9>{"000001000"}; // ans 4
	B3[B(2, 6)] = std::bitset<9>{"101100110"};
	B3[B(2, 7)] = std::bitset<9>{"100110010"};
	B3[B(2, 8)] = std::bitset<9>{"010000101"};
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
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[0].DebugString(), "0000");
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
		EXPECT_EQ(result[2].count(), 0U);
		result = appearance_sets<2>(B4.row(1));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 0U);
		EXPECT_TRUE(result[1].is_empty());
		EXPECT_TRUE(result[2].is_empty());
		result = appearance_sets<2>(B4.row(2));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
		result = appearance_sets<2>(B4.col(3));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
		result = appearance_sets<2>(B4.block(1));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1U);
		EXPECT_TRUE(result[1][Value{1}]);
	}
}

} // namespace SudokuTests::Solvers_find
