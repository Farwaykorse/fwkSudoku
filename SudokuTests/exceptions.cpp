//===--- SudokuTests/exceptions.cpp                                     ---===//
//
//	Unit tests for the custom exceptions defined in Sudoku::error
//===----------------------------------------------------------------------===//
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
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/exceptions.h>
// library
#include <string>
#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace SudokuTests::errorTests
{
TEST(Error, invalid_Board)
{
	using ::Sudoku::error::invalid_Board;
	{ // Type properties
		using typeT = invalid_Board;

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
		static_assert(not std::is_trivial_v<typeT>);            // ++
		static_assert(not std::is_trivially_copyable_v<typeT>); // ++
		static_assert(not std::is_standard_layout_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6)
		static_assert(not std::has_unique_object_representations_v<typeT>);
#endif
		static_assert(not std::is_empty_v<typeT>);
		static_assert(std::is_polymorphic_v<typeT>);  // --
		static_assert(not std::is_abstract_v<typeT>); // ++
		static_assert(not std::is_final_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900)
		static_assert(not std::is_aggregate_v<typeT>);
#endif // __ICL

		static_assert(std::is_base_of_v<std::exception, typeT>);
		static_assert(std::is_base_of_v<std::logic_error, typeT>);
		static_assert(std::is_convertible_v<typeT, std::logic_error>);

		// default constructor: typeT()
		static_assert(std::is_default_constructible_v<typeT>);         // ++
		static_assert(std::is_nothrow_default_constructible_v<typeT>); // ++
		static_assert(not std::is_trivially_default_constructible_v<typeT>);
		// copy constructor: typeT(const typeT&)
		static_assert(std::is_copy_constructible_v<typeT>);               // ++
		static_assert(std::is_nothrow_copy_constructible_v<typeT>);       // ++
		static_assert(not std::is_trivially_copy_constructible_v<typeT>); // ++
		// move constructor: typeT(typeT&&)
		static_assert(std::is_move_constructible_v<typeT>);
		static_assert(std::is_nothrow_move_constructible_v<typeT>);
		static_assert(not std::is_trivially_move_constructible_v<typeT>);
		// copy assignment
		static_assert(std::is_copy_assignable_v<typeT>);               // ++
		static_assert(std::is_nothrow_copy_assignable_v<typeT>);       // ++
		static_assert(not std::is_trivially_copy_assignable_v<typeT>); // ++
		static_assert(std::is_move_assignable_v<typeT>);               // ++
		static_assert(std::is_nothrow_move_assignable_v<typeT>);       // ++
		static_assert(not std::is_trivially_move_assignable_v<typeT>); // ++
		static_assert(std::is_destructible_v<typeT>);                  // ++
		static_assert(std::is_nothrow_destructible_v<typeT>);          // ++
		static_assert(not std::is_trivially_destructible_v<typeT>);    // ++
		static_assert(std::has_virtual_destructor_v<typeT>);           // --
#if not(defined(__ICL) && __ICL <= 1900)
		static_assert(std::is_swappable_v<typeT>);         // ++ C++17
		static_assert(std::is_nothrow_swappable_v<typeT>); // ++ C++17
#endif
	}

	const std::string name{"invalid_Board"};

	try
	{
		throw invalid_Board();
	}
	catch (const invalid_Board& e)
	{
		ASSERT_NE(e.what(), std::nullptr_t{});
		const size_t length{std::strlen(e.what())};
		EXPECT_EQ(length, size_t{13});
		EXPECT_EQ(std::strcmp(e.what(), name.c_str()), 0);
	}
}

TEST(Error, invalid_Location)
{
	using ::Sudoku::error::invalid_Location;
	{ // Type properties
		using typeT = invalid_Location;

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
		static_assert(not std::is_trivial_v<typeT>);            // ++
		static_assert(not std::is_trivially_copyable_v<typeT>); // ++
		static_assert(not std::is_standard_layout_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6)
		static_assert(not std::has_unique_object_representations_v<typeT>);
#endif
		static_assert(not std::is_empty_v<typeT>);
		static_assert(std::is_polymorphic_v<typeT>);  // --
		static_assert(not std::is_abstract_v<typeT>); // ++
		static_assert(not std::is_final_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900)
		static_assert(not std::is_aggregate_v<typeT>);
#endif

		static_assert(std::is_base_of_v<std::exception, typeT>);
		static_assert(std::is_base_of_v<std::logic_error, typeT>);
		static_assert(std::is_base_of_v<std::out_of_range, typeT>);
		static_assert(std::is_convertible_v<typeT, std::logic_error>);

		// default constructor: typeT()
		static_assert(std::is_default_constructible_v<typeT>);         // ++
		static_assert(std::is_nothrow_default_constructible_v<typeT>); // ++
		static_assert(not std::is_trivially_default_constructible_v<typeT>);
		// copy constructor: typeT(const typeT&)
		static_assert(std::is_copy_constructible_v<typeT>);               // ++
		static_assert(std::is_nothrow_copy_constructible_v<typeT>);       // ++
		static_assert(not std::is_trivially_copy_constructible_v<typeT>); // ++
		// move constructor: typeT(typeT&&)
		static_assert(std::is_move_constructible_v<typeT>);
		static_assert(std::is_nothrow_move_constructible_v<typeT>);
		static_assert(not std::is_trivially_move_constructible_v<typeT>);
		// copy assignment
		static_assert(std::is_copy_assignable_v<typeT>);               // ++
		static_assert(std::is_nothrow_copy_assignable_v<typeT>);       // ++
		static_assert(not std::is_trivially_copy_assignable_v<typeT>); // ++
		static_assert(std::is_move_assignable_v<typeT>);               // ++
		static_assert(std::is_nothrow_move_assignable_v<typeT>);       // ++
		static_assert(not std::is_trivially_move_assignable_v<typeT>); // ++
		static_assert(std::is_destructible_v<typeT>);                  // ++
		static_assert(std::is_nothrow_destructible_v<typeT>);          // ++
		static_assert(not std::is_trivially_destructible_v<typeT>);    // ++
		static_assert(std::has_virtual_destructor_v<typeT>);           // --
#if not(defined(__ICL) && __ICL <= 1900)
		static_assert(std::is_swappable_v<typeT>);         // ++ C++17
		static_assert(std::is_nothrow_swappable_v<typeT>); // ++ C++17
#endif
	}

	const std::string name{"invalid_Location"};
	// const std::string name_wrong{"invalid_chairs"};
	try
	{
		throw invalid_Location();
	}
	catch (const invalid_Location& e)
	{
		ASSERT_NE(e.what(), std::nullptr_t{});
		const size_t length{std::strlen(e.what())};
		EXPECT_EQ(length, size_t{16});
		EXPECT_EQ(std::strcmp(e.what(), name.c_str()), 0);
		// EXPECT_NE(*(e.what()), *(name_wrong.c_str()));

		ASSERT_EQ(e.where(), std::nullptr_t{});
		// length = std::strlen(e.where()); // first is the \0 character.
	}
}
} // namespace SudokuTests::errorTests
