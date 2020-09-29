// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/OptionValueTest.cpp                            ----====//
//
//	Unit tests for the template class Sudoku::OptionValue
//====--------------------------------------------------------------------====//
#include <Sudoku/OptionValue.h> // Class under test

#include <type_traits>

#include <gtest/gtest.h>


namespace SudokuTests::OptionValueTest
{
using ::Sudoku::OptionValue;
using ::Sudoku::Value;

namespace compiletime
{
	// Type properties
	using typeT = OptionValue<9>;

	static_assert(noexcept(Value{1}));
	// http://howardhinnant.github.io/TypeHiearchy.pdf
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
#if defined(_MSC_VER)
	static_assert(not std::is_trivial_v<typeT>);
#else
	static_assert(std::is_trivial_v<typeT>);
#endif
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
	static_assert(std::has_unique_object_representations_v<typeT>);
	static_assert(not std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_aggregate_v<typeT>);

	// default constructor: typeT()
	static_assert(not std::is_default_constructible_v<typeT>);
	static_assert(not std::is_nothrow_default_constructible_v<typeT>);
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

	static_assert(std::is_swappable_v<typeT>);
	static_assert(std::is_nothrow_swappable_v<typeT>);

	// Explicit construction
	static_assert(std::is_constructible_v<typeT, Value>);
	static_assert(std::is_constructible_v<typeT, size_t>);
	static_assert(
		std::is_constructible_v<typeT, OptionValue<2>> &&
		not std::is_same_v<typeT, OptionValue<2>>);
	static_assert(std::is_constructible_v<typeT, unsigned short>);
	static_assert(std::is_constructible_v<typeT, unsigned int>);
	static_assert(std::is_constructible_v<typeT, unsigned long>);
	static_assert(std::is_constructible_v<typeT, unsigned long long>);
	static_assert(not std::is_constructible_v<typeT, void>);
	static_assert(not std::is_constructible_v<typeT, bool>);
	static_assert(not std::is_constructible_v<typeT, short>);
	static_assert(not std::is_constructible_v<typeT, int>);
	static_assert(not std::is_constructible_v<typeT, long>);
	static_assert(not std::is_constructible_v<typeT, long long>);
	static_assert(not std::is_constructible_v<typeT, char>); // implementation
	static_assert(not std::is_constructible_v<typeT, signed char>);
	static_assert(not std::is_constructible_v<typeT, unsigned char>);
	static_assert(not std::is_constructible_v<typeT, wchar_t>);
	static_assert(not std::is_constructible_v<typeT, char8_t>);
	static_assert(not std::is_constructible_v<typeT, char16_t>);
	static_assert(not std::is_constructible_v<typeT, char32_t>);
	static_assert(not std::is_constructible_v<typeT, float>);
	static_assert(not std::is_constructible_v<typeT, double>);

	// Not-throwing Explicit construction
	static_assert(not std::is_nothrow_constructible_v<typeT, Value>);
	static_assert(not std::is_nothrow_constructible_v<typeT, size_t>);
	static_assert(not std::is_nothrow_constructible_v<typeT, OptionValue<2>>);
	static_assert(not std::is_nothrow_constructible_v<typeT, unsigned short>);
	static_assert(not std::is_nothrow_constructible_v<typeT, unsigned int>);
	static_assert(not std::is_nothrow_constructible_v<typeT, unsigned long>);
	static_assert(
		not std::is_nothrow_constructible_v<typeT, unsigned long long>);

	// Implicit conversion (to typeT)
	static_assert(std::is_convertible_v<Value, typeT>);
	static_assert(not std::is_convertible_v<size_t, typeT>);
	static_assert(
		not std::is_convertible_v<OptionValue<2>, typeT> &&
		not std::is_same_v<typeT, OptionValue<2>>);
	static_assert(not std::is_convertible_v<unsigned short, typeT>);
	static_assert(not std::is_convertible_v<unsigned int, typeT>);
	static_assert(not std::is_convertible_v<unsigned long, typeT>);
	static_assert(not std::is_convertible_v<unsigned long long, typeT>);

	// Not-throwing implicit conversion (to typeT)
	static_assert(not std::is_nothrow_convertible_v<Value, typeT>);

	// Implicit conversion (from typeT)
	static_assert(std::is_convertible_v<typeT, Value>);
	static_assert(std::is_convertible_v<typeT, size_t>);
	static_assert(std::is_convertible_v<typeT, unsigned short>);
	static_assert(std::is_convertible_v<typeT, unsigned int>);
	static_assert(std::is_convertible_v<typeT, unsigned long>);
	static_assert(std::is_convertible_v<typeT, unsigned long long>);
	static_assert(
		not std::is_convertible_v<typeT, OptionValue<2>> &&
		not std::is_same_v<typeT, OptionValue<2>>);
	static_assert(not std::is_convertible_v<typeT, short>);
	static_assert(not std::is_convertible_v<typeT, int>);
	static_assert(not std::is_convertible_v<typeT, long>);
	static_assert(not std::is_convertible_v<typeT, long long>);
	static_assert(not std::is_convertible_v<typeT, bool>);
	static_assert(not std::is_convertible_v<typeT, char>);
	static_assert(not std::is_convertible_v<typeT, signed char>);
	static_assert(not std::is_convertible_v<typeT, unsigned char>);

	// Not-throwing implicit conversion (from typeT)
	static_assert(std::is_nothrow_convertible_v<typeT, Value>);
	static_assert(std::is_nothrow_convertible_v<typeT, size_t>);
	static_assert(std::is_nothrow_convertible_v<typeT, unsigned short>);
	static_assert(std::is_nothrow_convertible_v<typeT, unsigned int>);
	static_assert(std::is_nothrow_convertible_v<typeT, unsigned long>);
	static_assert(std::is_nothrow_convertible_v<typeT, unsigned long long>);

	// Explicit construction (from typeT)
	// none

	// Assignable (to typeT)
	static_assert(std::is_assignable_v<typeT, Value>);
	static_assert(not std::is_assignable_v<typeT, size_t>);
	static_assert(not std::is_assignable_v<typeT, unsigned short>);

	static_assert(not std::is_nothrow_assignable_v<typeT, Value>);

	static_assert(not std::is_assignable_v<int, typeT>);
	static_assert(not std::is_assignable_v<unsigned int, typeT>);

	static_assert(not std::is_swappable_with_v<typeT, Value>);
	static_assert(not std::is_swappable_with_v<typeT, size_t>);
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);


	static_assert(typeT{1u} == typeT{Value{1}});
	static_assert(typeT{1u} != typeT{2u});
	static_assert(typeT{1u} < typeT{2u});
	static_assert(not(typeT{2u} < typeT{1u}));
	static_assert(typeT{3u} > typeT{2u});
	static_assert(not(typeT{2u} > typeT{3u}));


} // namespace compiletime

TEST(OptionValueTest, OutsideDomain)
{
	EXPECT_THROW(OptionValue<9>{Value{0}}, Sudoku::error::invalid_option);
	EXPECT_THROW(OptionValue<9>{Value{10}}, Sudoku::error::invalid_option);

	EXPECT_THROW(OptionValue<9>{0u}, Sudoku::error::invalid_option);
	EXPECT_THROW(OptionValue<9>{10u}, Sudoku::error::invalid_option);
}

} // namespace SudokuTests::OptionValueTest
