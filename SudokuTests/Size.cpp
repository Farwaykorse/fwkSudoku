// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/Size.cpp                                       ----====//
//
//	Unit tests for the template class Sudoku::Size
//=====--------------------------------------------------------------------====//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Size.h>


namespace SudokuTests::SizeTest
{
using ::Sudoku::Size;

namespace compiletime
{ // Type properties
	using typeT = Size<3>;
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
	static_assert(std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6) &&                          \
	not(defined(__APPLE__) && defined(__clang__) && __clang_major__ < 10)
	static_assert(not std::has_unique_object_representations_v<typeT>);
#endif
	static_assert(std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__APPLE__) && defined(__clang__) &&                            \
		(__clang_major__ < 10 ||                                               \
		 (__clang_major__ == 9 && __clang_minor__ < 1)))
	static_assert(std::is_aggregate_v<typeT>);
#endif

	// default constructor: typeT()
	static_assert(std::is_default_constructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_default_constructible<typeT>::value);   // ++
	static_assert(std::is_trivially_default_constructible<typeT>::value); // ++
	// ++ nothing virtual

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_copy_constructible<typeT>::value);   // ++
	static_assert(std::is_trivially_copy_constructible<typeT>::value); // +

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_move_constructible<typeT>::value);   // ++
	static_assert(std::is_trivially_move_constructible<typeT>::value); // ++

	// copy assignment
	static_assert(std::is_copy_assignable<typeT>::value);           // ++
	static_assert(std::is_nothrow_copy_assignable<typeT>::value);   // ++
	static_assert(std::is_trivially_copy_assignable<typeT>::value); // ++

	static_assert(std::is_move_assignable<typeT>::value);           // ++
	static_assert(std::is_nothrow_move_assignable<typeT>::value);   // ++
	static_assert(std::is_trivially_move_assignable<typeT>::value); // ++

	static_assert(std::is_destructible<typeT>::value);           // ++
	static_assert(std::is_nothrow_destructible<typeT>::value);   // ++
	static_assert(std::is_trivially_destructible<typeT>::value); // ++
	static_assert(!std::has_virtual_destructor<typeT>::value);   // --

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(std::is_swappable<typeT>::value);         // C++17
	static_assert(std::is_nothrow_swappable<typeT>::value); // C++17

	static_assert(!std::is_swappable_with<typeT, int>::value);          // C++17
	static_assert(!std::is_swappable_with<typeT, unsigned int>::value); // C++17
#endif

	static_assert(Size<2>::base == 2);
	static_assert(Size<3>::base == 3);
	static_assert(Size<2>::elem == 4);
	static_assert(Size<3>::elem == 9);
	static_assert(Size<2>::full == 16);
	static_assert(Size<3>::full == 81);
	static_assert(Size<3>::base == ::Sudoku::base_size<3>);
	static_assert(Size<3>::elem == ::Sudoku::elem_size<3>);
	static_assert(Size<3>::full == ::Sudoku::full_size<3>);

	static_assert(std::is_scalar_v<decltype(Size<3>::base)>);
	static_assert(std::is_scalar_v<decltype(Size<3>::elem)>);
	static_assert(std::is_scalar_v<decltype(Size<3>::full)>);
	static_assert(std::is_signed_v<decltype(Size<3>::base)>);

} // namespace compiletime

} // namespace SudokuTests::SizeTest
