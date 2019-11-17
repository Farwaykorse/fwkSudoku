//====---- SudokuTests/Normalize.cpp                                  ----====//
//
// Unit-tests for the Options switching functionality.
//====--------------------------------------------------------------------====//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Normalize.h>
// Helpers
#include <Sudoku/Solver.h>

// additional
#include <array>
#include <algorithm>
#include <stdexcept>
#include <type_traits>


namespace SudokuTests::Translate
{
using ::Sudoku::Board;
using ::Sudoku::Options;
using ::Sudoku::Value;
using ::Sudoku::elem_size;

TEST(translate, switchOptionsBoard)
{
	using ::Sudoku::switch_options;

	// clang-format off
	constexpr std::array<char, 81> b1a	// requires unique
	{
		6, 7, 3,	8, 9, 4,	5, 1, 2,
		9, 1, 2,	7, 3, 5,	4, 8, 6,
		8, 4, 5,	6, 1, 2,	9, 7, 3,
		7, 9, 8,	2, 6, 1,	3, 5, 4,
		5, 2, 6,	4, 7, 3,	8, 9, 1,
		1, 3, 4,	5, 8, 9,	2, 6, 7,
		4, 6, 9,	1, 2, 8,	7, 3, 5,
		2, 8, 7,	3, 5, 6,	1, 4, 9,
		3, 5, 1,	9, 4, 7,	6, 2, 8
	};
	// clang-format on
	Board<Options<elem_size<3>>, 3> B1{};
	std::transform(
		begin(b1a), end(b1a), std::begin(B1), ::Sudoku::to_Value<3, int>);
	ASSERT_TRUE(is_answer(B1[0][0], Value{6}));

	// error-handling
	static_assert(not noexcept(switch_options(B1, Value{1}, Value{1})));
#ifdef NDEBUG
	EXPECT_NO_THROW(switch_options(B1, Value{0}, Value{1}));
	EXPECT_NO_THROW(switch_options(B1, Value{1}, Value{0}));
	EXPECT_THROW(switch_options(B1, Value{10}, Value{1}), std::out_of_range);
	EXPECT_THROW(switch_options(B1, Value{0}, Value{10}), std::out_of_range);
#else
	EXPECT_DEBUG_DEATH(
		switch_options(B1, Value{10}, Value{1}), "Assertion failed: is_valid");
	EXPECT_DEBUG_DEATH(switch_options(B1, Value{1}, Value{10}), ".* is_valid");
	EXPECT_DEBUG_DEATH(switch_options(B1, Value{0}, Value{1}), ".* is_valid");
	EXPECT_DEBUG_DEATH(switch_options(B1, Value{1}, Value{0}), ".* is_valid");
#endif
	// return type:
	static_assert(std::is_same_v<
				  decltype(B1)&,
				  decltype(switch_options(B1, Value{1}, Value{1}))>);

	// before:
	EXPECT_EQ(get_answer(B1[0][0]), Value{6});
	EXPECT_EQ(get_answer(B1[0][7]), Value{1});
	EXPECT_EQ(get_answer(B1[2][3]), Value{6});
	EXPECT_EQ(get_answer(B1[8][6]), Value{6});
	EXPECT_EQ(get_answer(B1[7][6]), Value{1});
	// run:
	switch_options(B1, Value{6}, Value{1});
	// after:
	EXPECT_EQ(get_answer(B1[0][0]), Value{1});
	EXPECT_EQ(get_answer(B1[0][7]), Value{6});
	EXPECT_EQ(get_answer(B1[2][3]), Value{1});
	EXPECT_EQ(get_answer(B1[8][6]), Value{1});
	EXPECT_EQ(get_answer(B1[7][6]), Value{6});
	// unchanged:
	EXPECT_EQ(get_answer(B1[0][1]), Value{7});
	EXPECT_EQ(get_answer(B1[0][8]), Value{2});
}

TEST(translate, normalizeBoard)
{
	using ::Sudoku::normalize;

	// clang-format off
	constexpr std::array<char, 81> b1a	// requires unique
	{
		6, 7, 3,	8, 9, 4,	5, 1, 2,
		9, 1, 2,	7, 3, 5,	4, 8, 6,
		8, 4, 5,	6, 1, 2,	9, 7, 3,
		7, 9, 8,	2, 6, 1,	3, 5, 4,
		5, 2, 6,	4, 7, 3,	8, 9, 1,
		1, 3, 4,	5, 8, 9,	2, 6, 7,
		4, 6, 9,	1, 2, 8,	7, 3, 5,
		2, 8, 7,	3, 5, 6,	1, 4, 9,
		3, 5, 1,	9, 4, 7,	6, 2, 8
	};
	// clang-format on
	Board<Options<elem_size<3>>, 3> B1{};
	std::transform(
		begin(b1a), end(b1a), std::begin(B1), ::Sudoku::to_Value<3, int>);
	ASSERT_TRUE(is_answer(B1[0][0], Value{6}));
	ASSERT_TRUE(is_answer(B1[1][2], Value{2}));

	// error-handling
	static_assert(noexcept(normalize(B1)));

	// return type:
	static_assert(std::is_same_v<decltype(B1)&, decltype(normalize(B1))>);

	// before:
	EXPECT_EQ(get_answer(B1[0][0]), Value{6});
	EXPECT_EQ(get_answer(B1[0][7]), Value{1});
	EXPECT_EQ(get_answer(B1[2][3]), Value{6});
	EXPECT_EQ(get_answer(B1[8][6]), Value{6});
	EXPECT_EQ(get_answer(B1[7][6]), Value{1});
	// run:
	normalize(B1);
	// after:
	EXPECT_EQ(get_answer(B1[0][0]), Value{1});
	EXPECT_EQ(get_answer(B1[0][1]), Value{2});
	EXPECT_EQ(get_answer(B1[0][2]), Value{3});
	EXPECT_EQ(get_answer(B1[0][7]), Value{8});
	EXPECT_EQ(get_answer(B1[0][8]), Value{9});
	EXPECT_EQ(get_answer(B1[2][3]), Value{1});
	EXPECT_EQ(get_answer(B1[8][6]), Value{1});
	EXPECT_EQ(get_answer(B1[7][6]), Value{8});
}

TEST(translate, normalizeBoard2)
{
	using ::Sudoku::normalize;

	// clang-format off
	constexpr std::array<char, 81> b1a
	{
		5, 3, 0,	0, 7, 0,	0, 0, 0,
		6, 0, 0,	1, 9, 5,	0, 0, 0,
		0, 9, 8,	0, 0, 0,	0, 6, 0,

		8, 0, 0,	0, 6, 0,	0, 0, 3,
		4, 0, 0,	8, 0, 3,	0, 0, 1,
		7, 0, 0,	0, 2, 0,	0, 0, 6,

		0, 6, 0,	0, 0, 0,	2, 8, 0,
		0, 0, 0,	4, 1, 9,	0, 0, 5,
		0, 0, 0,	0, 8, 0,	0, 7, 9
	};
	// clang-format on
	Board<Options<elem_size<3>>, 3> B1{};
	::Sudoku::set_Value(B1, begin(b1a), end(b1a));

	// before:
	ASSERT_EQ(get_answer(B1[0][0]), Value{5});
	EXPECT_EQ(get_answer(B1[0][1]), Value{3});
	EXPECT_EQ(get_answer(B1[0][7]), Value{1});
	EXPECT_EQ(get_answer(B1[1][0]), Value{6});
	EXPECT_EQ(get_answer(B1[2][0]), Value{1});
	EXPECT_EQ(get_answer(B1[8][4]), Value{8});
	EXPECT_EQ(get_answer(B1[8][8]), Value{9});
	// run:
	normalize(B1);
	// after:
	EXPECT_EQ(get_answer(B1[0][0]), Value{1});
	EXPECT_EQ(get_answer(B1[0][1]), Value{2});
	EXPECT_EQ(get_answer(B1[0][2]), Value{3});
	EXPECT_EQ(get_answer(B1[0][7]), Value{8});
	EXPECT_EQ(get_answer(B1[0][8]), Value{9});
	EXPECT_EQ(get_answer(B1[1][0]), Value{4});
	EXPECT_EQ(get_answer(B1[2][0]), Value{8});
	EXPECT_EQ(get_answer(B1[2][3]), Value{2});
	EXPECT_EQ(get_answer(B1[8][6]), Value{8});
	EXPECT_EQ(get_answer(B1[7][6]), Value{4});
}
} // namespace SudokuTests::Translate
