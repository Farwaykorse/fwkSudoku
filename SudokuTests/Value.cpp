// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/Value.cpp                                      ----====//
//
//	Unit tests for the template class Sudoku::Value
//====--------------------------------------------------------------------====//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Value.h>
// Helpers

// additional
#include <vector>
#include <type_traits>
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
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6) &&                          \
	not(defined(__APPLE__) && defined(__clang__) && __clang_major__ < 10)
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif // __ICL
	static_assert(not std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
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

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(std::is_swappable_v<typeT>);         // C++17
	static_assert(std::is_nothrow_swappable_v<typeT>); // C++17
#endif

	// Explicit construction:
	static_assert(std::is_constructible_v<typeT, size_t>);
	static_assert(not std::is_convertible_v<size_t, typeT>);

	// By implicit conversion to size_t first:
	static_assert(std::is_constructible_v<typeT, char>);
	static_assert(std::is_constructible_v<typeT, int>);
	static_assert(std::is_constructible_v<typeT, unsigned int>);
	static_assert(std::is_constructible_v<typeT, std::uint32_t>);
	static_assert(std::is_constructible_v<typeT, std::uint64_t>);
	static_assert(std::is_constructible_v<typeT, bool>); // ... via size_t
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

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
#endif
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

TEST(Value, is_valid)
{
	using ::Sudoku::is_valid;
	using ::Sudoku::is_valid_option;

	EXPECT_FALSE(is_valid<2>(Value{0}));
	EXPECT_TRUE(is_valid<2>(Value{1}));
	EXPECT_TRUE(is_valid<2>(Value(4)));
	EXPECT_FALSE(is_valid<2>(Value{5}));
	EXPECT_TRUE(is_valid<3>(Value{5}));
	EXPECT_FALSE(is_valid<3>(Value{16}));

	static_assert(noexcept(is_valid<3>(Value{0})));
	static_assert(noexcept(is_valid<3>(Value{1})));
	static_assert(noexcept(is_valid<3>(Value{9})));
	static_assert(noexcept(is_valid<3>(Value{10})));

	// is constexpr
	static_assert(!is_valid<2>(Value{0}));
	static_assert(is_valid<2>(Value{1}));
	static_assert(is_valid<2>(Value{4}));
	static_assert(!is_valid<2>(Value{5}));
	static_assert(!is_valid<3>(Value{0}));
	static_assert(is_valid<3>(Value{1}));
	static_assert(is_valid<3>(Value{9}));
	static_assert(!is_valid<3>(Value{10}));

	static_assert(noexcept(is_valid_option<3>(Value{0})));
	static_assert(noexcept(is_valid_option<3>(Value{1})));
	static_assert(noexcept(is_valid_option<3>(Value{9})));
	static_assert(noexcept(is_valid_option<3>(Value{10})));

	static_assert(!is_valid_option<4>(Value{0}));
	static_assert(is_valid_option<4>(Value{1}));
	static_assert(is_valid_option<4>(Value{4}));
	static_assert(!is_valid_option<4>(Value{5}));
	static_assert(!is_valid_option<9>(Value{0}));
	static_assert(is_valid_option<9>(Value{1}));
	static_assert(is_valid_option<9>(Value{9}));
	static_assert(!is_valid_option<9>(Value{10}));
}

TEST(Value, operator_bool)
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

TEST(Value, operator_increment)
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

TEST(Value, is_valid_vector)
{
	using ::Sudoku::is_valid;
	// vector input
	using list = std::vector<Value>;
	const std::vector<Value> cList{
		Value{1}, Value{2}, Value{3}, Value{4}, Value{3}, Value{1}};
	std::vector<Value> List{cList};
	EXPECT_FALSE(is_valid<2>(list{})) << "can't be empty";
	EXPECT_TRUE(is_valid<2>(List));
	EXPECT_TRUE(is_valid<2>(list{Value{1}}));
	EXPECT_TRUE(is_valid<2>(list{Value{4}}));
	EXPECT_FALSE(is_valid<2>(list{Value{0}}));
	EXPECT_FALSE(is_valid<2>(list{Value{5}}));
	List.at(2) = Value{0}; // 1, 2, 0, 4, 3, 1
	EXPECT_FALSE(is_valid<2>(List));
	// EXPECT_FALSE(is_valid<2>(std::vector<Value>{1, -2, 3, 4, 3, 1}));
	List.at(2) = Value{3}; // 1, 2, 3, 4, 3, 1
	List.at(1) = Value{5}; // 1, 5, 3, 4, 3, 1
	EXPECT_FALSE(is_valid<2>(List));

	static_assert(noexcept(is_valid<2>(cList)));
	static_assert(noexcept(is_valid<2>(List)));
}

TEST(Value, to_Value)
{
	using ::Sudoku::to_Value;
	// Input: Value
	static_assert(to_Value<3>(Value{0}) == Value{0});
	static_assert(to_Value<3>(Value{1}) == Value{1});
	static_assert(to_Value<3>(Value{9}) == Value{9});
	static_assert(noexcept(Value{0}));
	// TODO: fails with Clang, works for MSVC and GCC 7.1+
	// static_assert(noexcept(to_Value<3>(Value{0}))); // fails with Clang
	// static_assert(noexcept(to_Value<3>(Value{9}))); // fails with Clang
	static_assert(noexcept(Value{10}));
	static_assert(not(noexcept(to_Value<3>(Value{10}))));
	static_assert(not(noexcept(to_Value<3>(Value{21}))));

	EXPECT_NO_THROW(to_Value<3>(Value{0}));
	EXPECT_NO_THROW(to_Value<3>(Value{9}));
	EXPECT_THROW(to_Value<3>(Value{10}), std::domain_error);

	// Input: int
	static_assert(to_Value<3>(0) == Value{0});
	static_assert(to_Value<3>(1) == Value{1});
	static_assert(to_Value<3>(9) == Value{9});
	// static_assert(noexcept(to_Value<3>(2))); // fails with Clang
	static_assert(not(noexcept(to_Value<3>(-2))));
	static_assert(not(noexcept(to_Value<3>(10))));
	static_assert(not(noexcept(to_Value<3>(21))));

	EXPECT_NO_THROW(to_Value<3>(0));
	EXPECT_NO_THROW(to_Value<3>(9));
	EXPECT_THROW(to_Value<3>(-1), std::domain_error);
	EXPECT_THROW(to_Value<3>(10), std::domain_error);

	// Input: size_t
	static_assert(to_Value<3>(size_t{0}) == Value{0});
	static_assert(to_Value<3>(size_t{1}) == Value{1});
	static_assert(to_Value<3>(size_t{9}) == Value{9});
	static_assert(noexcept(size_t{0}));
	// static_assert(noexcept(to_Value<3>(size_t{0}))); // fails with Clang
	// static_assert(noexcept(to_Value<3>(size_t{9}))); // fails with Clang
	static_assert(noexcept(size_t{10}));
	static_assert(not(noexcept(to_Value<3>(size_t{10}))));
	static_assert(not(noexcept(to_Value<3>(size_t{21}))));

	EXPECT_NO_THROW(to_Value<3>(size_t{0}));
	EXPECT_NO_THROW(to_Value<3>(size_t{9}));
	EXPECT_THROW(to_Value<3>(size_t{10}), std::domain_error);

	// Input: unsigned int
	static_assert(to_Value<3>(0u) == Value{0});
	static_assert(to_Value<3>(1u) == Value{1});
	static_assert(to_Value<3>(9u) == Value{9});
	// static_assert(noexcept(to_Value<3>(2u))); // fails with Clang
	static_assert(not(noexcept(to_Value<3>(10u))));
	static_assert(not(noexcept(to_Value<3>(21u))));

	// Input: long int
	static_assert(to_Value<3>(0L) == Value{0});
	static_assert(to_Value<3>(1L) == Value{1});
	static_assert(to_Value<3>(9L) == Value{9});
	// static_assert(noexcept(to_Value<3>(2L))); // fails with Clang
	static_assert(not(noexcept(to_Value<3>(-1L))));
	static_assert(not(noexcept(to_Value<3>(10L))));
	static_assert(not(noexcept(to_Value<3>(21L))));

	// Input: char
	static_assert(to_Value<3>(char{0}) == Value{0});
	static_assert(to_Value<3>(char{1}) == Value{1});
	static_assert(to_Value<3>(char{9}) == Value{9});
	// static_assert(noexcept(to_Value<3>(2))); // fails with Clang
	static_assert(not(noexcept(to_Value<3>(char{-2}))));
	static_assert(not(noexcept(to_Value<3>(char{10}))));
	static_assert(not(noexcept(to_Value<3>(char{21}))));

	EXPECT_THROW(to_Value<3>(char{-1}), std::domain_error);
	EXPECT_THROW(to_Value<3>(char{10}), std::domain_error);

	// Will not compile when elements cannot be represented.
	using u_char = unsigned char;

	[[maybe_unused]] const Value U = to_Value<15>(u_char{0}); // N < 16
	[[maybe_unused]] const Value V = to_Value<11>(char{0});   // N < 12
}

} // namespace SudokuTests::ValueTest
