// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/Value.cpp                                      ----====//
//
//	Unit tests for the template class Sudoku::Value
//====--------------------------------------------------------------------====//
#include <Sudoku/Value.h> // Class under test

#include <vector>

#include <type_traits>

#include <gtest/gtest.h>

#include <cstdint>


namespace SudokuTests::ValueTest
{
using ::Sudoku::Value;

namespace compiletime
{
	// Type properties
	using typeT = Value;

	static_assert(noexcept(Value{1}));
	// http://howardhinnant.github.io/TypeHiearchy.pdf
	// Composite Type Categories
	static_assert(std::is_fundamental_v<unsigned int>);
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
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
	static_assert(std::has_unique_object_representations_v<typeT>);
	static_assert(not std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_aggregate_v<typeT>);

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
	static_assert(std::is_copy_assignable_v<typeT>);
	static_assert(std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(std::is_trivially_copy_assignable_v<typeT>);

	static_assert(std::is_move_assignable_v<typeT>);
	static_assert(std::is_nothrow_move_assignable_v<typeT>);
	static_assert(std::is_trivially_move_assignable_v<typeT>);

	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(std::is_trivially_destructible_v<typeT>);
	static_assert(not std::has_virtual_destructor_v<typeT>);

	static_assert(std::is_swappable_v<typeT>);         // C++17
	static_assert(std::is_nothrow_swappable_v<typeT>); // C++17

	// Explicit construction:
	static_assert(std::is_constructible_v<typeT, size_t>);
	static_assert(not std::is_convertible_v<size_t, typeT>);

	static_assert(not std::is_constructible_v<typeT, char>);
	static_assert(std::is_constructible_v<typeT, int8_t>);
	static_assert(std::is_constructible_v<typeT, uint8_t>);
	static_assert(std::is_constructible_v<typeT, int>);
	static_assert(std::is_constructible_v<typeT, unsigned int>);
	static_assert(std::is_constructible_v<typeT, std::uint32_t>);
	static_assert(std::is_constructible_v<typeT, std::uint64_t>);
	static_assert(not std::is_constructible_v<typeT, bool>);
	// explicit construction from Value:
	static_assert(std::is_constructible_v<size_t, typeT>);
	static_assert(std::is_nothrow_constructible_v<size_t, typeT>);
	static_assert(std::is_constructible_v<bool, typeT>);
	static_assert(std::is_nothrow_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<int, typeT>);
	// all others fail, unless same as size_t
	// No implicit conversion:
	static_assert(not std::is_convertible_v<typeT, size_t>);
	static_assert(not std::is_convertible_v<typeT, bool>);

	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<int, typeT>);
	static_assert(not std::is_assignable_v<typeT, unsigned int>);
	static_assert(not std::is_assignable_v<unsigned int, typeT>);
	static_assert(not std::is_assignable_v<typeT, size_t>);
	static_assert(not std::is_assignable_v<typeT, bool>);
	static_assert(not std::is_assignable_v<bool, typeT>);

	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
} // namespace compiletime

TEST(Value, comparisons)
{
	static_assert(noexcept(Value{5} == Value{2}));
	static_assert(noexcept(Value{5} != Value{2}));
	static_assert(noexcept(Value{5} < Value{2}));
	static_assert(noexcept(Value{5} > Value{2}));
	static_assert(noexcept(Value{5} <= Value{2}));
	static_assert(noexcept(Value{5} >= Value{2}));

	static_assert(Value{7} == Value{7});
	static_assert(not(Value{2} == Value{7}));
	static_assert(Value{1} != Value{0});
	static_assert(not(Value{0} != Value{0}));
	static_assert(Value{8} > Value{2});
	static_assert(not(Value{2} > Value{2}));
	static_assert(not(Value{1} > Value{2}));
	static_assert(Value{4} >= Value{1});
	static_assert(Value{4} >= Value{4});
	static_assert(not(Value{4} >= Value{6}));
	static_assert(Value{3} <= Value{9});
	static_assert(Value{9} <= Value{9});
	static_assert(not(Value{10} <= Value{9}));
	static_assert(Value{5} < Value{6});
	static_assert(not(Value{5} < Value{5}));
	static_assert(not(Value{6} < Value{5}));

	EXPECT_TRUE(Value{1} == Value{1});
	EXPECT_FALSE(Value{1} == Value{3});
	EXPECT_TRUE(Value{1} != Value{3});
	EXPECT_FALSE(Value{3} != Value{3});
	EXPECT_TRUE(Value{5} < Value{7});
	EXPECT_FALSE(Value{4} < Value{4});
	EXPECT_FALSE(Value{4} < Value{3});
	EXPECT_TRUE(Value{1} <= Value{3});
	EXPECT_TRUE(Value{5} <= Value{5});
	EXPECT_FALSE(Value{7} <= Value{5});
	EXPECT_TRUE(Value{5} >= Value{5});
	EXPECT_TRUE(Value{6} >= Value{5});
	EXPECT_FALSE(Value{6} >= Value{8});
	EXPECT_TRUE(Value{6} > Value{5});
	EXPECT_FALSE(Value{1} > Value{1});
	EXPECT_FALSE(Value{2} > Value{5});
}
TEST(Value, operatorBool)
{
	static_assert(noexcept(Value{0}));
	static_assert(noexcept(Value{1}));
	static_assert(noexcept(Value{999}));
	static_assert(not Value{0}); // seems to result in an explicit conversion
	static_assert(Value{1});
	static_assert(Value{999});

	EXPECT_EQ(bool{Value{0}}, false); // requires explicit conversion
	EXPECT_EQ(bool{Value{1}}, true);
}

TEST(Value, operatorIncrement)
{ // pre- and post-incrementing
	static_assert(noexcept(++Value{0}));
	static_assert(noexcept((Value{0})++));
	// return-type
	static_assert(std::is_same_v<Value&, decltype(++Value{0})>);
	static_assert(std::is_same_v<Value, decltype((Value{0})++)>);

	static_assert(++Value{0} == Value{1});
	static_assert(++Value{99} == Value{100});
	static_assert((Value{0})++ == Value{0});
	static_assert((Value{99})++ == Value{99});
	Value val{2};
	val++;
	EXPECT_EQ(val, Value{3});
}
} // namespace SudokuTests::ValueTest
