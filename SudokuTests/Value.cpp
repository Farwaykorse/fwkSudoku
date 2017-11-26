//===--	SudokuTests/Value.cpp											--===//
//
//	Unit tests for the template class Sudoku::Value
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
#include "../Sudoku/Value.h"
// Helpers

// aditional
#include <vector>
#include <type_traits>

using namespace Sudoku;

namespace SudokuTests::LocationTest
{
namespace compileTime
{
	using typeT = value_t;
	// TODO
} // namespace compileTime

TEST(Value, is_valid)
{
	// EXPECT_FALSE(is_valid<2>(-1));
	EXPECT_FALSE(is_valid<2>(0));
	EXPECT_TRUE(is_valid<2>(1));
	EXPECT_TRUE(is_valid<2>(4));
	EXPECT_FALSE(is_valid<2>(5));
	EXPECT_TRUE(is_valid<3>(5));
	EXPECT_FALSE(is_valid<3>(16));

	// vector input
	using list = std::vector<value_t>;
	EXPECT_FALSE(is_valid<2>(list{})) << "can't be empty";
	EXPECT_FALSE(is_valid<2>(std::vector<value_t>{})) << "can't be empty";
	EXPECT_TRUE(is_valid<2>(list{1, 2, 3, 4, 3, 1}));
	EXPECT_TRUE(is_valid<2>(std::vector<value_t>{1, 2, 3, 4, 3, 1}));
	EXPECT_TRUE(is_valid<2>(list{1}));
	EXPECT_TRUE(is_valid<2>(std::vector<value_t>{1}));
	EXPECT_FALSE(is_valid<2>(list{0}));
	EXPECT_FALSE(is_valid<2>(std::vector<value_t>{0}));
	EXPECT_FALSE(is_valid<2>(list{5}));
	EXPECT_FALSE(is_valid<2>(std::vector<value_t>{5}));
	EXPECT_FALSE(is_valid<2>(list{1, 2, 0, 4, 3, 1}));
	EXPECT_FALSE(is_valid<2>(std::vector<value_t>{1, 2, 0, 4, 3, 1}));
	// EXPECT_FALSE(is_valid<2>(std::vector<value_t>{1, -2, 3, 4, 3, 1}));
	EXPECT_FALSE(is_valid<2>(list{1, 5, 3, 4, 3, 1}));
	EXPECT_FALSE(is_valid<2>(std::vector<value_t>{1, 5, 3, 4, 3, 1}));

	// is constexpr
	EXPECT_TRUE(noexcept(is_valid<2>(1)));
	EXPECT_TRUE(noexcept(is_valid<3>(7)));
	EXPECT_FALSE(noexcept(is_valid<2>(std::vector<value_t>{1, 2, 3, 4, 3, 1})));
}

} // namespace SudokuTests::LocationTest
