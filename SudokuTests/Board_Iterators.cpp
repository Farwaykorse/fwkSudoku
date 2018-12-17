//====---- SudokuTests/Board_Iterators.cpp                            ----====//
//
// Unit tests for the template class Sudoku::Board
//====--------------------------------------------------------------------====//
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
//====--------------------------------------------------------------------====//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Board.h>
#include <Sudoku/Board_Iterators.h>
// helpers
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>
#include <Sudoku/traits.h>
// Debug Output
#include "print_Options.h"
// library
#include <gsl/gsl>
#include <bitset>
#include <initializer_list>
#include <set>
#include <numeric> // accumulate
#include <random>  // random-access tests
#include <type_traits>


namespace SudokuTests::BoardTest
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Options;

using ::Sudoku::traits::is_input;
using ::Sudoku::traits::is_forward;
using ::Sudoku::traits::is_bidir;
using ::Sudoku::traits::is_random;

namespace iterator
{
	// using dataT = int;
	using dataT  = Options<9>;
	using BoardT = Board<dataT, 3>;
	using typeT  = BoardT::iterator;

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
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6)
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif
	static_assert(not std::is_empty_v<typeT>); // nothing virtual
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900)
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

	// explicit conversion from other iterators
	static_assert(std::is_constructible_v<typeT, BoardT::iterator>);
	static_assert(not std::is_constructible_v<typeT, BoardT::const_iterator>);
	static_assert(not std::is_constructible_v<typeT, BoardT::reverse_iterator>);
	static_assert(
		not std::is_constructible_v<typeT, BoardT::const_reverse_iterator>);
	// other types
	static_assert(not std::is_constructible_v<typeT, Location<3>>);
	// explicit construction from typeT:
	static_assert(std::is_constructible_v<Location<3>, typeT>);
	static_assert(std::is_nothrow_constructible_v<Location<3>, typeT>);
	static_assert(not std::is_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<int, typeT>);

	static_assert(std::is_assignable_v<typeT, Location<3>>);
	static_assert(std::is_nothrow_assignable_v<typeT, Location<3>>);
	static_assert(not std::is_assignable_v<Location<3>, typeT>);
	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<int, typeT>);
	static_assert(not std::is_assignable_v<typeT, unsigned int>);
	static_assert(not std::is_assignable_v<unsigned int, typeT>);
	static_assert(not std::is_assignable_v<typeT, size_t>);
	static_assert(not std::is_assignable_v<typeT, bool>);
	static_assert(not std::is_assignable_v<bool, typeT>);

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_with_v<typeT, Location<3>>);  // C++17
	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
#endif
} // namespace iterator
namespace const_iterator
{
	// using dataT = int;
	using dataT  = Options<9>;
	using BoardT = Board<dataT, 3>;
	using typeT  = BoardT::const_iterator;

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
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6)
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif
	static_assert(not std::is_empty_v<typeT>); // nothing virtual
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_aggregate_v<typeT>);
#endif // __ICL

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

	// explicit conversion from other iterators
	static_assert(std::is_constructible_v<typeT, BoardT::iterator>);
	static_assert(std::is_constructible_v<typeT, BoardT::const_iterator>);
	static_assert(not std::is_constructible_v<typeT, BoardT::reverse_iterator>);
	static_assert(
		not std::is_constructible_v<typeT, BoardT::const_reverse_iterator>);
	// other types
	static_assert(not std::is_constructible_v<typeT, Location<3>>);
	// explicit construction from typeT:
	static_assert(std::is_constructible_v<Location<3>, typeT>);
	static_assert(std::is_nothrow_constructible_v<Location<3>, typeT>);
	static_assert(not std::is_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<int, typeT>);

	static_assert(std::is_assignable_v<typeT, Location<3>>);
	static_assert(std::is_nothrow_assignable_v<typeT, Location<3>>);
	static_assert(not std::is_assignable_v<Location<3>, typeT>);
	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<int, typeT>);
	static_assert(not std::is_assignable_v<typeT, unsigned int>);
	static_assert(not std::is_assignable_v<unsigned int, typeT>);
	static_assert(not std::is_assignable_v<typeT, size_t>);
	static_assert(not std::is_assignable_v<typeT, bool>);
	static_assert(not std::is_assignable_v<bool, typeT>);

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_with_v<typeT, Location<3>>);  // C++17
	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
#endif
} // namespace const_iterator
namespace reverse_iterator
{
	// using dataT = int;
	using dataT  = Options<9>;
	using BoardT = Board<dataT, 3>;
	using typeT  = BoardT::reverse_iterator;

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
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6)
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif
	static_assert(not std::is_empty_v<typeT>); // nothing virtual
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900)
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

	// explicit conversion from other iterators
	static_assert(not std::is_constructible_v<typeT, BoardT::iterator>);
	static_assert(not std::is_constructible_v<typeT, BoardT::const_iterator>);
	static_assert(std::is_constructible_v<typeT, BoardT::reverse_iterator>);
	static_assert(
		not std::is_constructible_v<typeT, BoardT::const_reverse_iterator>);
	// other types
	static_assert(not std::is_constructible_v<typeT, Location<3>>);
	// explicit construction from typeT:
	static_assert(std::is_constructible_v<Location<3>, typeT>);
	static_assert(std::is_nothrow_constructible_v<Location<3>, typeT>);
	static_assert(not std::is_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<int, typeT>);

	static_assert(std::is_assignable_v<typeT, Location<3>>);
	static_assert(std::is_nothrow_assignable_v<typeT, Location<3>>);
	static_assert(not std::is_assignable_v<Location<3>, typeT>);
	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<int, typeT>);
	static_assert(not std::is_assignable_v<typeT, unsigned int>);
	static_assert(not std::is_assignable_v<unsigned int, typeT>);
	static_assert(not std::is_assignable_v<typeT, size_t>);
	static_assert(not std::is_assignable_v<typeT, bool>);
	static_assert(not std::is_assignable_v<bool, typeT>);

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_with_v<typeT, Location<3>>);  // C++17
	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
#endif
} // namespace reverse_iterator
namespace const_reverse_iterator
{
	// using dataT = int;
	using dataT  = Options<9>;
	using BoardT = Board<dataT, 3>;
	using typeT  = BoardT::const_reverse_iterator;

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
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__clang__) && __clang_major__ < 6)
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif
	static_assert(not std::is_empty_v<typeT>); // nothing virtual
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_aggregate_v<typeT>);
#endif // __ICL

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

	// explicit conversion from other iterators
	static_assert(not std::is_constructible_v<typeT, BoardT::iterator>);
	static_assert(not std::is_constructible_v<typeT, BoardT::const_iterator>);
	static_assert(std::is_constructible_v<typeT, BoardT::reverse_iterator>);
	static_assert(
		std::is_constructible_v<typeT, BoardT::const_reverse_iterator>);
	// other types
	static_assert(not std::is_constructible_v<typeT, Location<3>>);
	// explicit construction from typeT:
	static_assert(std::is_constructible_v<Location<3>, typeT>);
	static_assert(std::is_nothrow_constructible_v<Location<3>, typeT>);
	static_assert(not std::is_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<int, typeT>);

	static_assert(std::is_assignable_v<typeT, Location<3>>);
	static_assert(std::is_nothrow_assignable_v<typeT, Location<3>>);
	static_assert(not std::is_assignable_v<Location<3>, typeT>);
	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<int, typeT>);
	static_assert(not std::is_assignable_v<typeT, unsigned int>);
	static_assert(not std::is_assignable_v<unsigned int, typeT>);
	static_assert(not std::is_assignable_v<typeT, size_t>);
	static_assert(not std::is_assignable_v<typeT, bool>);
	static_assert(not std::is_assignable_v<bool, typeT>);

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_with_v<typeT, Location<3>>);  // C++17
	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
#endif
} // namespace const_reverse_iterator

namespace iterator_traits
{ // iterators available on Board
	// using dataT = int;
	using dataT = Options<9>;
	using typeT = Board<dataT>;

	// std::iterator_traits<It> has member typedefs:
	// value_type, difference_type, reference, pointer, iterator_category
	using traits = typename std::iterator_traits<typeT::iterator>;
	static_assert(std::is_object_v<traits::iterator_category>);
	static_assert(std::is_object_v<traits::value_type>);
	static_assert(std::is_reference_v<traits::reference>);
	static_assert(std::is_lvalue_reference_v<traits::reference>);
	static_assert(std::is_pointer_v<traits::pointer>);
	static_assert(std::is_object_v<traits::difference_type>);

	using Ctraits = typename std::iterator_traits<typeT::const_iterator>;
	static_assert(std::is_object_v<Ctraits::iterator_category>);
	static_assert(std::is_object_v<Ctraits::value_type>);
	static_assert(std::is_reference_v<Ctraits::reference>);
	static_assert(std::is_lvalue_reference_v<Ctraits::reference>);
	static_assert(std::is_pointer_v<Ctraits::pointer>);
	static_assert(std::is_object_v<Ctraits::difference_type>);

	using Rtraits = typename std::iterator_traits<typeT::reverse_iterator>;
	static_assert(std::is_object_v<Rtraits::iterator_category>);
	static_assert(std::is_object_v<Rtraits::value_type>);
	static_assert(std::is_reference_v<Rtraits::reference>);
	static_assert(std::is_lvalue_reference_v<Rtraits::reference>);
	static_assert(std::is_pointer_v<Rtraits::pointer>);
	static_assert(std::is_object_v<Rtraits::difference_type>);

	using CRtraits =
		typename std::iterator_traits<typeT::const_reverse_iterator>;
	static_assert(std::is_object_v<CRtraits::iterator_category>);
	static_assert(std::is_object_v<CRtraits::value_type>);
	static_assert(std::is_reference_v<CRtraits::reference>);
	static_assert(std::is_lvalue_reference_v<CRtraits::reference>);
	static_assert(std::is_pointer_v<CRtraits::pointer>);
	static_assert(std::is_object_v<CRtraits::difference_type>);

	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<traits::iterator_category, category>);
	static_assert(std::is_same_v<Ctraits::iterator_category, category>);
	static_assert(std::is_same_v<Rtraits::iterator_category, category>);
	static_assert(std::is_same_v<CRtraits::iterator_category, category>);

	static_assert(std::is_same_v<traits::value_type, dataT>);
	static_assert(std::is_same_v<Ctraits::value_type, dataT>);
	static_assert(std::is_same_v<Rtraits::value_type, dataT>);
	static_assert(std::is_same_v<CRtraits::value_type, dataT>);

	using difference_type = std::ptrdiff_t;
	static_assert(std::is_same_v<traits::difference_type, difference_type>);
	static_assert(std::is_same_v<Ctraits::difference_type, difference_type>);
	static_assert(std::is_same_v<Rtraits::difference_type, difference_type>);
	static_assert(std::is_same_v<CRtraits::difference_type, difference_type>);

	static_assert(std::is_same_v<traits::pointer, dataT*>);
	static_assert(std::is_same_v<Ctraits::pointer, dataT const*>);
	static_assert(std::is_same_v<Rtraits::pointer, dataT*>);
	static_assert(std::is_same_v<CRtraits::pointer, dataT const*>);

	static_assert(std::is_same_v<traits::reference, dataT&>);
	static_assert(std::is_same_v<Ctraits::reference, dataT const&>);
	static_assert(std::is_same_v<Rtraits::reference, dataT&>);
	static_assert(std::is_same_v<CRtraits::reference, dataT const&>);
} // namespace iterator_traits

//====--------------------------------------------------------------------====//
struct test_elements
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
};

//====--------------------------------------------------------------------====//
TEST(Board_Iterator, construction)
{ // All iterator categories
	using ::Sudoku::Board_iterator;
	using ::Sudoku::const_Board_iterator;
	using ::Sudoku::reverse_Board_iterator;
	using ::Sudoku::const_reverse_Board_iterator;

	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	using not_null       = ::gsl::not_null<decltype(&A)>;
	using const_not_null = ::gsl::not_null<decltype(&cA)>;

	static_assert(noexcept(A.begin()));
	static_assert(noexcept(A.end()));
	static_assert(noexcept(A.cbegin()));
	static_assert(noexcept(A.cend()));
	static_assert(noexcept(A.rbegin()));
	static_assert(noexcept(A.rend()));
	static_assert(noexcept(A.crbegin()));
	static_assert(noexcept(A.crend()));
	static_assert(noexcept(cA.begin()));
	static_assert(noexcept(cA.end()));
	static_assert(noexcept(cA.rbegin()));
	static_assert(noexcept(cA.rend()));

	// return types:
	static_assert(std::is_same_v<decltype(A.begin()), decltype(A.end())>);
	static_assert(std::is_same_v<decltype(A.cbegin()), decltype(A.cend())>);
	static_assert(std::is_same_v<decltype(A.rbegin()), decltype(A.rend())>);
	static_assert(std::is_same_v<decltype(A.crbegin()), decltype(A.crend())>);
	// const container -> always const
	static_assert(std::is_same_v<decltype(cA.begin()), decltype(cA.cbegin())>);
	static_assert(
		std::is_same_v<decltype(cA.rbegin()), decltype(cA.crbegin())>);
	// no mixing
	static_assert(
		not std::is_same_v<decltype(A.begin()), decltype(A.cbegin())>);
	static_assert(
		not std::is_same_v<decltype(A.begin()), decltype(A.rbegin())>);

	// Default constructor
	const Board_iterator<int, 2> D1;
	EXPECT_TRUE((D1 == Board_iterator<int, 2>()));

	// Construct
	const Board_iterator<int, 2> I1{not_null{&A}};
	EXPECT_TRUE(I1 == A.begin());
	const reverse_Board_iterator<int, 2> rI1{not_null{&A}};
	EXPECT_TRUE(rI1 == A.rbegin());
	{
		const const_Board_iterator<int, 2> cI1{not_null{&A}};
		EXPECT_TRUE(cI1 == A.cbegin());
		const const_reverse_Board_iterator<int, 2> crI1{not_null{&A}};
		EXPECT_TRUE(rI1 == A.rbegin());
	}
	const const_Board_iterator<int, 2> cI1{const_not_null{&cA}};
	EXPECT_TRUE(cI1 == cA.cbegin());
	const const_reverse_Board_iterator<int, 2> crI1{const_not_null{&cA}};
	EXPECT_TRUE(crI1 == cA.crbegin());
#ifdef NDEBUG
	EXPECT_TRUE(cI1 != A.cbegin());
	EXPECT_TRUE(crI1 != A.crbegin());
#endif // NDEBUG
	{  // Copy-construct
		const Board_iterator<int, 2> LI{I1};
		EXPECT_TRUE(LI == A.begin());
		const const_Board_iterator<int, 2> cLI{cI1};
		EXPECT_TRUE(cLI == cA.cbegin());
		const reverse_Board_iterator<int, 2> rLI{rI1};
		EXPECT_TRUE(rLI == A.rbegin());
		const const_reverse_Board_iterator<int, 2> crLI{crI1};
		EXPECT_TRUE(crLI == cA.crbegin());
	}
	{ // conversion to const
		const const_Board_iterator<int, 2> cLI{I1};
		EXPECT_TRUE(cLI == A.cbegin());
		const const_reverse_Board_iterator<int, 2> crLI{rI1};
		EXPECT_TRUE(crLI == A.crbegin());
	}
	{
		// assignment
		[[maybe_unused]] Board_iterator<int, 2> LI{};
		LI = I1;
		[[maybe_unused]] const_Board_iterator<int, 2> cLI{};
		cLI = cI1;
		cLI = I1;
		[[maybe_unused]] reverse_Board_iterator<int, 2> rLI{};
		rLI = rI1;
		[[maybe_unused]] const_reverse_Board_iterator<int, 2> crLI{};
		crLI = rI1;
		crLI = crI1;
	}
	{ // Construct from location
		const Board_iterator<int, 2> I2(not_null{&A}, Location<2>{0});
		EXPECT_TRUE(I2 == A.begin());
		const Board_iterator<int, 2> I3(not_null{&A}, Location<2>{16});
		EXPECT_TRUE(I3 == A.end());
	}
	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.rbegin())> &&
		is_random<decltype(A.cbegin())> && is_random<decltype(A.crbegin())>)
	{
		EXPECT_TRUE(
			(Board_iterator<int, 2>(not_null{&A}, Location<2>{8})) ==
			(A.begin() + 8));
	}
}

TEST(Board_Iterator, assign_Location)
{
	using L = Location<2>;

	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	using not_null = ::gsl::not_null<decltype(&A)>;

	static_assert(noexcept(A.begin() = L{2}));
	static_assert(noexcept(A.cbegin() = L{2}));
	static_assert(noexcept(A.rbegin() = L{2}));
	static_assert(noexcept(A.cbegin() = L{2}));
	static_assert(noexcept(A.crbegin() = L{2}));
	static_assert(noexcept(cA.begin() = L{2}));
	static_assert(noexcept(cA.cbegin() = L{2}));
	{
		Sudoku::Board_iterator<int, 2> x1{not_null{&A}};
		EXPECT_TRUE(x1 == A.begin());
		x1 = L{1};
		EXPECT_TRUE(x1 != A.begin());
		EXPECT_TRUE(x1 == ++A.begin());
		EXPECT_TRUE((x1 = L{15}) != A.end());
		EXPECT_TRUE(x1 == --A.end());
	}
	{
		Sudoku::const_Board_iterator<int, 2> x1{not_null{&A}};
		EXPECT_TRUE(x1 == A.cbegin());
		x1 = L{1};
		EXPECT_TRUE(x1 != A.cbegin());
		EXPECT_TRUE(x1 == ++A.cbegin());
		EXPECT_TRUE((x1 = L{15}) != A.cend());
		EXPECT_TRUE(x1 == --A.cend());
	}
	{
		Sudoku::reverse_Board_iterator<int, 2> r1{not_null{&A}};
		r1 = L{15};
		EXPECT_TRUE(r1 == A.rbegin());
		r1 = L{0};
		EXPECT_TRUE(r1 != A.rend());
		EXPECT_TRUE(r1 == --A.rend());
	}
}

TEST(Board_Iterator, Location)
{
	using ::Sudoku::Board_iterator;
	using ::Sudoku::reverse_Board_iterator;
	using L = Location<2>;

	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	using not_null = ::gsl::not_null<decltype(&A)>;

	static_assert(noexcept(A.begin().location()));
	static_assert(noexcept(A.cbegin().location()));
	static_assert(noexcept(A.rbegin().location()));
	static_assert(noexcept(A.crbegin().location()));
	static_assert(noexcept(Location<2>{A.begin()}));
	static_assert(noexcept(Location<2>{A.cbegin()}));
	static_assert(noexcept(Location<2>{A.rbegin()}));
	static_assert(noexcept(Location<2>{A.crbegin()}));

	// Construct from Location
	const Board_iterator<int, 2> x1{not_null{&A}};
	const Board_iterator<int, 2> x2(not_null{&A}, L{12});

	// Return Location
	static_assert(Board_iterator<int, 2>().location() == Location<2>{0});
	static_assert(reverse_Board_iterator<int, 2>().location() == L{15});
	// Conversion to Location
	static_assert(Location<2>{Board_iterator<int, 2>()} == Location<2>{0});
	// Assign Location
	static_assert((Board_iterator<int, 2>() = L{13}).location() == L{13});

	// construct Location from iterator
	EXPECT_EQ(L{x1}, L{0});
	EXPECT_EQ(L{x2}, L{12});
	EXPECT_EQ(L{A.begin()}, L{0});
	EXPECT_EQ(L{A.cbegin()}, L{0});
	EXPECT_EQ(Location<2>{A.rbegin()}, Location<2>{15});
	EXPECT_EQ(Location<2>{A.crbegin()}, Location<2>{15});
	EXPECT_EQ(Location<2>{A.end()}, Location<2>{16});
	EXPECT_EQ(Location<2>{A.cend()}, Location<2>{16});
	EXPECT_EQ(Location<2>{A.rend()}, Location<2>{-1});
	EXPECT_EQ(Location<2>{A.crend()}, Location<2>{-1});

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.rbegin())> &&
		is_random<decltype(A.cbegin())> && is_random<decltype(A.crbegin())>)
	{
		EXPECT_EQ(L{A.begin() + 8}, L{8});
		EXPECT_EQ(L{A.cbegin() + 8}, L{8});
		EXPECT_EQ(L{A.rbegin() + 8}, L{7});
		EXPECT_EQ(L{A.crbegin() + 2}, L{13});
		EXPECT_EQ(L{cA.begin() + 13}, L{13});
		EXPECT_EQ(L{cA.cbegin() + 13}, L{13});
		EXPECT_EQ(L{cA.crbegin() + 1}, L{14});
	}
}

TEST(Board_Iterator, dereference)
{
	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;
	Board<Options<4>, 2> Opt{};

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		static_assert(noexcept(*A.begin()));
		static_assert(noexcept(*A.cbegin()));
		static_assert(noexcept(*A.rbegin()));
		static_assert(noexcept(*A.crbegin()));
		static_assert(noexcept(*cA.begin()));
		static_assert(noexcept(*Opt.begin()));

		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.end(), "");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.cend(), "");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.rend(), "");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.crend(), "");
		{ // Dereferencing is an error when de container is deleted
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = *(::Sudoku::Board_iterator<int, 3>()),
				"nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U =
					*(::Sudoku::const_Board_iterator<int, 3>()),
				"nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = *(decltype(A.begin())()), "nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = *(decltype(A.cbegin())()), "nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = *(decltype(A.rbegin())()), "nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = *(decltype(A.crbegin())()),
				"nullptr");
		}
		// result type
		static_assert(std::is_same_v<int&, decltype(*A.begin())>);
		static_assert(std::is_same_v<int const&, decltype(*A.cbegin())>);
		static_assert(std::is_same_v<int&, decltype(*A.rbegin())>);
		static_assert(std::is_same_v<int const&, decltype(*A.crbegin())>);
		static_assert(std::is_same_v<Options<4>&, decltype(*Opt.begin())>);

		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);

		EXPECT_EQ(*A.begin(), 9);
		EXPECT_EQ(*A.cbegin(), 9);
		EXPECT_EQ(*A.rbegin(), 15);
		EXPECT_EQ(*A.crbegin(), 15);
		EXPECT_EQ(*cA.begin(), 9);
		EXPECT_EQ(*cA.rbegin(), 15);
		EXPECT_EQ(*Opt.begin(), Options<4>{});
		const auto itr = A.begin();
		EXPECT_EQ(*itr, 9);
		// Input iterator: No guarantee it can be dereferenced more than once
		if constexpr (is_forward<decltype(itr)>)
		{
			EXPECT_EQ(*itr, 9);
		}
		else
			ADD_FAILURE();
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, pre_increment)
{ // All iterator categories
	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	static_assert(noexcept(++A.begin()));
	static_assert(noexcept(++A.cbegin()));
	static_assert(noexcept(++A.rbegin()));
	static_assert(noexcept(++A.crbegin()));
	static_assert(noexcept(++cA.begin()));
	static_assert(noexcept(++cA.rbegin()));

	EXPECT_NO_THROW(++A.begin());
	EXPECT_NO_THROW(++A.cbegin());
	EXPECT_NO_THROW(++A.rbegin());
	EXPECT_NO_THROW(++A.crbegin());
	EXPECT_NO_THROW(++cA.begin());
	EXPECT_NO_THROW(++cA.rbegin());

	EXPECT_DEBUG_DEATH(++A.end(), "< full_size");
	EXPECT_DEBUG_DEATH(++A.cend(), "< full_size");
	EXPECT_DEBUG_DEATH(++A.rend(), ">= 0");
	EXPECT_DEBUG_DEATH(++A.crend(), ">= 0");
	{ // Advancing is an error when de container is deleted
		EXPECT_DEBUG_DEATH(++(::Sudoku::Board_iterator<int, 3>()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.begin())()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.cbegin())()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.rbegin())()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.crbegin())()), "nullptr");
	}
	{ // iterators have value semantics
		const auto x = A.begin();
		auto y       = x; // copy
		EXPECT_TRUE(x == y);
		EXPECT_FALSE(x == ++y);
		const auto cx = A.cbegin();
		auto cy       = cx;
		EXPECT_TRUE(cx == cy);
		EXPECT_FALSE(cx == ++cy);
		y  = x; // suppress warning: assigned only once
		cy = cx;
	}

	//====----------------------------------------------------------------====//
	// input iterator
	if constexpr (is_input<decltype(A.begin())>)
	{ // return type
		static_assert(
			std::is_same_v<decltype(A.begin())&, decltype(++A.begin())>);
		static_assert(std::is_same_v<int&, decltype(*(++A.begin()))>);
		EXPECT_FALSE(A.begin() == ++A.begin());
		EXPECT_TRUE(++A.begin() == ++A.begin());
		EXPECT_FALSE(++A.begin() == ++(++A.begin()));
		EXPECT_EQ(*(++A.begin()), 1);
		EXPECT_EQ(*(++(++A.begin())), 2);
		{
			//! NOTE: Input iterator - no guarantee it can be dereferenced more
			//! than once
			const auto x = A.begin();
			auto y       = x; // copy
			EXPECT_EQ(*y, 9);
			++y;
			EXPECT_EQ(*y, 1);
			EXPECT_EQ(*x, 9);
			EXPECT_EQ(*++y, 2);
		}
	}
	else
		ADD_FAILURE();
	if constexpr (is_input<decltype(A.cbegin())>)
	{
		static_assert(
			std::is_same_v<decltype(A.cbegin())&, decltype(++A.cbegin())>);
		static_assert(std::is_same_v<int const&, decltype(*(++A.cbegin()))>);
		EXPECT_FALSE(A.cbegin() == ++A.cbegin());
		EXPECT_TRUE(++A.cbegin() == ++A.cbegin());
		EXPECT_FALSE(++A.cbegin() == ++(++A.cbegin()));
		{
			const auto x = A.cbegin();
			auto y       = x; // copy
			EXPECT_EQ(*y, 9);
			++y;
			EXPECT_EQ(*y, 1);
			EXPECT_EQ(*x, 9);
			EXPECT_EQ(*++y, 2);
		}
	}
	else
		ADD_FAILURE();
	if constexpr (is_input<decltype(A.rbegin())>)
	{
		static_assert(
			std::is_same_v<decltype(A.rbegin())&, decltype(++A.rbegin())>);
		static_assert(std::is_same_v<int&, decltype(*(++A.rbegin()))>);
		EXPECT_FALSE(A.rbegin() == ++A.rbegin());
		EXPECT_TRUE(++A.rbegin() == ++A.rbegin());
		EXPECT_FALSE(++A.rbegin() == ++(++A.rbegin()));
		EXPECT_EQ(*(++A.rbegin()), 14);
	}
	else
		ADD_FAILURE();
	if constexpr (is_input<decltype(A.crbegin())>)
	{
		static_assert(
			std::is_same_v<decltype(A.crbegin())&, decltype(++A.crbegin())>);
		EXPECT_EQ(*(++A.crbegin()), 14);
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, post_increment)
{
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin()++));
		static_assert(noexcept(A.cbegin()++));
		static_assert(noexcept(A.rbegin()++));
		static_assert(noexcept(A.crbegin()++));
		static_assert(noexcept(cA.begin()++));
		static_assert(noexcept(cA.rbegin()++));

		static_assert(
			std::is_same_v<decltype(A.begin()), decltype(A.begin()++)>);
		static_assert(
			std::is_same_v<decltype(A.cbegin()), decltype(A.cbegin()++)>);
		static_assert(std::is_same_v<int&, decltype(*A.begin()++)>);
		static_assert(std::is_same_v<int const&, decltype(*A.cbegin()++)>);
		static_assert(std::is_same_v<int&, decltype(*A.rbegin()++)>);
		static_assert(std::is_same_v<int const&, decltype(*A.crbegin()++)>);

		// (void)i++ equivalent to (void)++i
		EXPECT_NO_THROW(A.begin()++);
		EXPECT_NO_THROW(A.cbegin()++);
		EXPECT_NO_THROW(A.rbegin()++);
		EXPECT_NO_THROW(A.crbegin()++);

		EXPECT_DEBUG_DEATH(A.end()++, "< full_size");
		EXPECT_DEBUG_DEATH(A.cend()++, "< full_size");
		EXPECT_DEBUG_DEATH(A.rend()++, ">= 0");
		EXPECT_DEBUG_DEATH(A.crend()++, ">= 0");
		{ // Advancing is an error when de container is deleted
			EXPECT_DEBUG_DEATH((Sudoku::Board_iterator<int, 3>())++, "nullptr");
			EXPECT_DEBUG_DEATH((decltype(A.begin())())++, "nullptr");
			EXPECT_DEBUG_DEATH((decltype(A.cbegin())())++, "nullptr");
			EXPECT_DEBUG_DEATH((decltype(A.rbegin())())++, "nullptr");
			EXPECT_DEBUG_DEATH((decltype(A.crbegin())())++, "nullptr");
		}
		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);
		// *i++
		EXPECT_TRUE(A.begin()++ == A.begin());
		EXPECT_TRUE(A.rbegin()++ == A.rbegin());
		EXPECT_TRUE(A.cbegin()++ == A.cbegin());
		EXPECT_TRUE(A.crbegin()++ == A.crbegin());
		EXPECT_EQ(*A.begin()++, 9);
		EXPECT_EQ(*A.cbegin()++, 9);
		EXPECT_EQ(*A.rbegin()++, 15);
		EXPECT_EQ(*A.crbegin()++, 15);
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, equal)
{ // All iterator categories
	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	static_assert(noexcept(A.begin() == A.begin()));
	static_assert(noexcept(A.begin() == A.end()));
	static_assert(noexcept(A.cbegin() == A.cbegin()));
	static_assert(noexcept(A.cbegin() == A.cend()));
	static_assert(noexcept(A.rbegin() == A.rbegin()));
	static_assert(noexcept(A.crbegin() == A.crbegin()));
	static_assert(noexcept(cA.begin() == cA.begin()));
	static_assert(noexcept(cA.begin() == cA.cend()));
	// return type
	static_assert(std::is_same_v<bool, decltype(A.begin() == A.begin())>);
	static_assert(std::is_same_v<bool, decltype(A.cbegin() == A.cbegin())>);
	{ // constexpr
		using itr = ::Sudoku::Board_iterator<int, 3>;
		using L   = Location<3>;
		static_assert(((itr() = L{0}) == (itr() = L{0})));
		static_assert(not((itr() = L{3}) == itr()));
	}
	[[maybe_unused]] bool U{};
	EXPECT_NO_THROW(U = (A.begin() == A.end()));
	EXPECT_NO_THROW(U = (A.end() == A.end()));
	EXPECT_NO_THROW(U = (A.cbegin() == A.cend()));
	EXPECT_NO_THROW(U = (A.rbegin() == A.rend()));
	EXPECT_NO_THROW(U = (A.crbegin() == A.crend()));
	{
		Board<int, 2> B = A;
		Board<int, 2> C{};
		EXPECT_DEBUG_DEATH(U = A.begin() == B.begin(), "is_same_Board");
		EXPECT_DEBUG_DEATH(U = A.begin() == C.begin(), "is_same_Board");
#ifdef NDEBUG
		EXPECT_FALSE(A.begin() == B.begin());
		EXPECT_FALSE(A.begin() == C.begin());
#endif // !_DEBUG
	}
	U = false; // suppress warning: assigned only once
	EXPECT_TRUE(A.begin() == A.begin());
	EXPECT_TRUE(A.cbegin() == A.cbegin());
	EXPECT_TRUE(A.rbegin() == A.rbegin());
	EXPECT_TRUE(A.crbegin() == A.crbegin());
	EXPECT_TRUE(cA.begin() == cA.begin());
	EXPECT_TRUE(cA.cbegin() == cA.begin());
	EXPECT_FALSE(A.begin() == A.end());
	EXPECT_FALSE(A.cbegin() == A.cend());
	EXPECT_FALSE(A.rbegin() == A.rend());
	EXPECT_FALSE(A.crbegin() == A.crend());
	const auto x  = A.begin();
	const auto cx = A.cbegin();
	EXPECT_TRUE(x == A.begin());
	EXPECT_TRUE(cx == A.cbegin());

	// default constructor
	if constexpr (
		is_forward<decltype(A.begin())> && is_forward<decltype(A.cbegin())> &&
		is_forward<decltype(A.rbegin())> && is_forward<decltype(A.crbegin())>)
	{
		const ::Sudoku::Board_iterator<int, 2> Default;
		EXPECT_TRUE((Default == ::Sudoku::Board_iterator<int, 2>{}));
		EXPECT_DEBUG_DEATH(U = Default == A.begin(), "is_same_Board");
		EXPECT_DEBUG_DEATH(U = Default == A.end(), "is_same_Board");
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, not_equal)
{ // InputIterator
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() != A.begin()));
		static_assert(noexcept(A.cbegin() != A.cbegin()));
		static_assert(noexcept(A.rbegin() != A.rbegin()));
		static_assert(noexcept(A.crbegin() != A.crbegin()));
		static_assert(noexcept(cA.begin() != cA.begin()));
		static_assert(noexcept(cA.begin() != cA.cbegin()));
		// return type
		static_assert(std::is_same_v<bool, decltype(A.begin() != A.begin())>);
		static_assert(std::is_same_v<bool, decltype(A.cbegin() != A.cbegin())>);
		{ // constexpr
			using itr = ::Sudoku::Board_iterator<int, 3>;
			using L   = Location<3>;
			static_assert(not((itr() = L{0}) != (itr() = L{0})));
			static_assert((itr() = L{3}) != itr());
		}
		[[maybe_unused]] bool U{};
		EXPECT_NO_THROW(U = (A.begin() != A.end()));
		EXPECT_NO_THROW(U = (A.cbegin() != A.cend()));
		EXPECT_NO_THROW(U = (A.rbegin() != A.rend()));
		EXPECT_NO_THROW(U = (A.crbegin() != A.crend()));
		{
			Board<int, 2> B = A;
			Board<int, 2> C{};
			EXPECT_DEBUG_DEATH(U = A.begin() != B.begin(), "is_same_Board");
			EXPECT_DEBUG_DEATH(U = A.begin() != C.begin(), "is_same_Board");
#ifdef NDEBUG
			EXPECT_TRUE(A.begin() != B.begin());
			EXPECT_TRUE(A.begin() != B.end());
			EXPECT_TRUE(A.begin() != C.begin());
#endif // NDEBUG
		}
		U = false; // suppress warning: assigned only once
		EXPECT_TRUE(A.begin() != A.end());
		EXPECT_FALSE(A.begin() != A.begin());
		EXPECT_TRUE(A.begin() != ++A.begin());
		EXPECT_TRUE(++A.begin() != A.begin());
		EXPECT_TRUE(A.cbegin() != A.cend());
		EXPECT_FALSE(A.cbegin() != A.cbegin());
		EXPECT_TRUE(A.rbegin() != A.rend());
		EXPECT_FALSE(A.rbegin() != A.rbegin());
		EXPECT_TRUE(A.crbegin() != A.crend());
		EXPECT_FALSE(A.crbegin() != A.crbegin());
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, member_access)
{ // input iterator category
	test_elements TE{};
	auto& A = TE.A;
	Board<Options<4>, 2> Opt{};

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		static_assert(noexcept(Opt.begin().operator->()));
		static_assert(noexcept(Opt.cbegin().operator->()));
		static_assert(noexcept(Opt.rbegin().operator->()));
		static_assert(noexcept(Opt.crbegin().operator->()));
		static_assert(noexcept(Opt.begin()->size()));
		static_assert(noexcept(Opt.cbegin()->size()));
		static_assert(noexcept(Opt.rbegin()->size()));
		static_assert(noexcept(Opt.crbegin()->size()));
		static_assert(
			not noexcept(Opt.begin()->remove_option(::Sudoku::Value{1})));
		// return type
		static_assert(
			std::is_same_v<Options<4>*, decltype(Opt.begin().operator->())>);
		static_assert(std::is_same_v<int*, decltype(A.begin().operator->())>);

		EXPECT_NO_THROW(
			[[maybe_unused]] bool U =
				(Opt.begin()->size() == (*Opt.begin()).size())); // equivalent
		EXPECT_EQ((Opt.begin()->size()), (*Opt.begin()).size()); // equivalent
		EXPECT_EQ(*(A.begin().operator->()), A[0][0]);
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, InputIterator)
{
	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	int total{};
	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.rbegin())>)
	{
		EXPECT_NO_THROW(total = std::accumulate(A.begin(), A.end(), 0));
		EXPECT_EQ(total, 129);
		total = 0;
		EXPECT_NO_THROW(total = std::accumulate(A.rbegin(), A.rend(), 0));
		EXPECT_EQ(total, 129);
		total = 0;
	}
	else
		ADD_FAILURE();
	if constexpr (
		is_input<decltype(A.cbegin())> && is_input<decltype(A.crbegin())>)
	{
		EXPECT_NO_THROW(total = std::accumulate(A.cbegin(), A.cend(), 0));
		EXPECT_EQ(total, 129);
		total = 0;
		EXPECT_NO_THROW(total = std::accumulate(cA.rbegin(), cA.rend(), 0));
		EXPECT_EQ(total, 129);
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, OutputIterator)
{
	Board<int, 2> tmp{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";

	// *r = o (return-value not used)
	auto itrA = tmp.begin();
	// pre-condition: iterator is dereferenceable
	static_assert(noexcept(*itrA));
	EXPECT_EQ(*itrA, 0);
	// operation:
	static_assert(noexcept(*tmp.begin() = 1));
	*itrA = 2;
	EXPECT_EQ(tmp[0][0], 2);
	// post-condition: iterator is incrementable
	static_assert(noexcept((*tmp.begin() = 1)++));
	itrA++;
	EXPECT_TRUE(itrA == ++tmp.begin());
	if constexpr (is_forward<decltype(tmp.begin())>)
	{
		*itrA = 5;
		EXPECT_EQ(tmp[0][1], 5);
	}

	[[maybe_unused]] int U{};
	EXPECT_NO_THROW(U = *tmp.begin()); // pre-condition
	static_assert(noexcept(*tmp.begin() = 5));
	EXPECT_NO_THROW(U = *tmp.begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	static_assert(noexcept(*(++tmp.begin()) = 3));
	EXPECT_NO_THROW(*(++tmp.begin()) = 4);
	EXPECT_EQ(tmp[0][1], 4);
	static_assert(noexcept(*tmp.rbegin()));
	EXPECT_NO_THROW(U = *tmp.rbegin()); // pre-condition
	static_assert(noexcept(*tmp.rbegin() = 2));
	EXPECT_NO_THROW(U = *tmp.rbegin() = 9);
	EXPECT_EQ(tmp[3][3], 9);
	EXPECT_NO_THROW(*(++tmp.rbegin()) = 8);
	EXPECT_EQ(tmp[3][2], 8);
	EXPECT_NO_THROW(*(tmp.begin()++) = 2);
	EXPECT_EQ(tmp[0][0], 2);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(*(tmp.rbegin()++) = 7);
	EXPECT_EQ(tmp[3][3], 7);
	EXPECT_EQ(tmp[3][2], 8);
	EXPECT_NO_THROW(++(*tmp.begin() = 0)); // post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.rbegin() = 9));
	EXPECT_EQ(tmp[3][3], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r); // post-condition ++r

	U = 12; // suppress warning: assigned only once
	r = tmp.end();
}

TEST(Board_Iterator, ForwardIterator)
{
	test_elements TE{};
	auto& A = TE.A;

	if constexpr (
		is_forward<decltype(A.begin())> && is_forward<decltype(A.cbegin())> &&
		is_forward<decltype(A.rbegin())> && is_forward<decltype(A.crbegin())>)
	{
		using typeT = Board<int>;
		static_assert(std::is_default_constructible_v<typeT::iterator>);
		static_assert(std::is_default_constructible_v<typeT::const_iterator>);
		static_assert(std::is_default_constructible_v<typeT::reverse_iterator>);
		static_assert(
			std::is_default_constructible_v<typeT::const_reverse_iterator>);

		EXPECT_NO_THROW(A.begin()++);
		EXPECT_EQ(*A.begin()++, A[0][0]);
		EXPECT_EQ(*A.cbegin()++, A[0][0]);
		EXPECT_EQ(*A.rbegin()++, A[3][3]);
		EXPECT_EQ(*A.crbegin()++, A[3][3]);
		EXPECT_EQ(A.begin(), A.begin());
		EXPECT_EQ(++A.begin(), ++A.begin());
		auto r = A.begin();
		auto i = r;
		*r     = 0;
		EXPECT_EQ(*r, 0);
		EXPECT_EQ(*i, 0);
		EXPECT_EQ(*++r, 1);
		EXPECT_EQ(*++r, *++(++i));
		{ // Algorithm requiring forward_iterator
			Board<int, 2> B;
			std::iota(B.begin(), B.end(), 1);
			EXPECT_EQ(B[0][0], 1);
			EXPECT_EQ(B[1][0], 5);
			EXPECT_EQ(B[3][3], 16);
			std::iota(B.rbegin(), B.rend(), 1);
			EXPECT_EQ(B[3][3], 1);
			EXPECT_EQ(B[2][3], 5);
			EXPECT_EQ(B[0][0], 16);
		}
		r = A.end(); // suppress warning: assigned only once
		i = A.end();
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, pre_decrement)
{ // operator--()
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_bidir<decltype(A.begin())> && is_bidir<decltype(A.cbegin())> &&
		is_bidir<decltype(A.rbegin())> && is_bidir<decltype(A.crbegin())>)
	{
		static_assert(noexcept(--A.end()));
		static_assert(noexcept(--A.cend()));
		static_assert(noexcept(--A.rend()));
		static_assert(noexcept(--A.cend()));
		static_assert(noexcept(--cA.end()));
		static_assert(noexcept(--cA.rend()));

		EXPECT_DEBUG_DEATH(--A.begin(), "> 0");
		EXPECT_DEBUG_DEATH(--A.cbegin(), "> 0");
		EXPECT_DEBUG_DEATH(--A.rbegin(), "< full_size");
		EXPECT_DEBUG_DEATH(--A.crbegin(), "< full_size");
		--A.end();
		--A.cend();
		--A.rend();
		--A.crend();

		// return type
		static_assert(std::is_same_v<decltype(A.end())&, decltype(--A.end())>);
		static_assert(std::is_same_v<int&, decltype(*(--A.end()))>);
		static_assert(
			std::is_same_v<decltype(A.cend())&, decltype(--A.cend())>);
		static_assert(std::is_same_v<int&, decltype(*(--A.end()))>);
		static_assert(std::is_same_v<int const&, decltype(*(--A.cend()))>);

		// Sanity-check
		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A[0][1], 1);
		ASSERT_EQ(A.back(), 15);
		ASSERT_EQ(A[3][2], 14);
		// iterator
		EXPECT_FALSE(A.end() == --A.end());
		EXPECT_TRUE(--A.end() == --A.end());
		EXPECT_FALSE(--A.end() == --(--A.end()));
		EXPECT_EQ(*(--A.end()), 15);
		EXPECT_EQ(*(--(--A.end())), 14);
		{ // value-semantics
			auto x = A.end();
			auto y = x; // copy
			--y;
			EXPECT_FALSE(y == A.end());
			EXPECT_TRUE(x == A.end());
			EXPECT_EQ(*y, 15);
			--x;
			EXPECT_TRUE(x == y);
			EXPECT_EQ(*--y, 14);
		}
		// const_iterator
		EXPECT_FALSE(A.cend() == --A.cend());
		EXPECT_TRUE(--A.cend() == --A.cend());
		EXPECT_FALSE(--A.cend() == --(--A.cend()));
		EXPECT_EQ(*(--A.cend()), 15);
		EXPECT_EQ(*(--(--A.cend())), 14);
		// reverse_iterator
		EXPECT_FALSE(A.rend() == --A.rend());
		EXPECT_TRUE(--A.rend() == --A.rend());
		EXPECT_FALSE(--A.rend() == --(--A.rend()));
		EXPECT_EQ(*(--A.rend()), 9);
		EXPECT_EQ(*(--(--A.rend())), 1);
		// const_reverse_iterator
		EXPECT_FALSE(A.crend() == --A.crend());
		EXPECT_TRUE(--A.crend() == --A.crend());
		EXPECT_FALSE(--A.crend() == --(--A.crend()));
		EXPECT_EQ(*(--A.crend()), 9);
		EXPECT_EQ(*(--(--A.crend())), 1);
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, post_decrement)
{
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_bidir<decltype(A.begin())> && is_bidir<decltype(A.cbegin())> &&
		is_bidir<decltype(A.rbegin())> && is_bidir<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin()--));
		static_assert(noexcept(A.cbegin()--));
		static_assert(noexcept(A.rbegin()--));
		static_assert(noexcept(A.crbegin()--));
		static_assert(noexcept(cA.begin()--));
		static_assert(noexcept(cA.rbegin()--));

		// (void)i-- equivalent to(void)-- i
		EXPECT_DEBUG_DEATH(A.begin()--, "> 0");
		EXPECT_DEBUG_DEATH(--A.cbegin(), "> 0");
		EXPECT_DEBUG_DEATH(A.rbegin()--, "< full_size");
		EXPECT_DEBUG_DEATH(--A.crbegin(), "< full_size");
		A.end()--;
		A.cend()--;
		A.rend()--;
		A.crend()--;

		// return type
		static_assert(std::is_same_v<decltype(A.end()), decltype(A.end()--)>);
		static_assert(std::is_same_v<decltype(A.cend()), decltype(A.cend()--)>);
		static_assert(std::is_same_v<int&, decltype(*(A.end()--))>);
		static_assert(std::is_same_v<int const&, decltype(*(A.cend()--))>);
		static_assert(std::is_same_v<int&, decltype(*A.rbegin()--)>);
		static_assert(std::is_same_v<int const&, decltype(*A.crbegin()--)>);

		// Sanity-check
		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A[0][1], 1);
		ASSERT_EQ(A.back(), 15);
		ASSERT_EQ(A[3][2], 14);
		// iterator
		EXPECT_TRUE(A.end() == A.end()--);
		EXPECT_FALSE(A.end()-- == --A.end());
		EXPECT_EQ(*(--(--A.end())), 14);
		{ // value-semantics
			auto x       = A.end();
			auto y       = x; // copy
			const auto z = y--;
			EXPECT_TRUE(x == A.end());
			EXPECT_TRUE(y != A.end());
			EXPECT_EQ(*y, 15);
			x--;
			EXPECT_TRUE(x == y);
			EXPECT_TRUE(z == A.end());
		}
		// const_iterator
		EXPECT_TRUE(A.cend()-- == A.cend()--);
		EXPECT_TRUE(A.cend()-- == A.cend());
		EXPECT_FALSE(A.cend()-- == --A.cend());
		{
			auto x = A.cend()--;
			EXPECT_TRUE(x == A.cend());
			x--;
			EXPECT_TRUE(x != A.cend());
			EXPECT_EQ(*x, 15);
		}
		// reverse_iterator
		EXPECT_TRUE(A.rend()-- == A.rend()--);
		EXPECT_TRUE(A.rend()-- == A.rend());
		EXPECT_FALSE(A.rend()-- == --A.rend());
		{
			auto x = A.rend()--;
			EXPECT_TRUE(x == A.rend());
			x--;
			EXPECT_TRUE(x != A.rend());
			EXPECT_EQ(*x, 9);
		}
		// const_reverse_iterator
		EXPECT_TRUE(A.crend()-- == A.crend()--);
		EXPECT_TRUE(A.crend()-- == A.crend());
		EXPECT_FALSE(A.crend()-- == --A.crend());
		{
			auto x = A.crend()--;
			EXPECT_TRUE(x == A.crend());
			x--;
			EXPECT_TRUE(x != A.crend());
			EXPECT_EQ(*x, 9);
		}
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, BidirectionalIterator)
{
	test_elements TE{};
	auto& A = TE.A;

	if constexpr (
		is_bidir<decltype(A.begin())> && is_bidir<decltype(A.cbegin())> &&
		is_bidir<decltype(A.rbegin())> && is_bidir<decltype(A.crbegin())>)
	{
		// --i (return type: It&)	is decrementable
		EXPECT_EQ(*(--A.end()), 15);
		EXPECT_EQ(*(--(--A.end())), 14);
		EXPECT_EQ(*(--A.cend()), 15);
		EXPECT_EQ(*(--(--A.cend())), 14);
		EXPECT_EQ(*(--A.rend()), 9);
		EXPECT_EQ(*(--(--A.rend())), 1);
		EXPECT_EQ(*(--A.crend()), 9);
		EXPECT_EQ(*(--(--A.crend())), 1);

		EXPECT_EQ(--(++A.begin()), A.begin());
		EXPECT_EQ(*(--(++A.begin())), *A.begin());
		EXPECT_EQ(--(++A.cbegin()), A.cbegin());
		EXPECT_EQ(--(++A.rbegin()), A.rbegin());
		EXPECT_EQ(--(++A.crbegin()), A.crbegin());
		// *i--
		EXPECT_EQ(A.end()--, A.end());
		EXPECT_EQ(*(--A.end())--, 15);
		EXPECT_EQ(A.cend()--, A.cend());
		EXPECT_EQ(*(--A.cend())--, 15);
		EXPECT_EQ(A.rend()--, A.rend());
		EXPECT_EQ(*(--A.rend())--, 9);
		EXPECT_EQ(A.crend()--, A.crend());
		EXPECT_EQ(*(--A.crend())--, 9);

		{ // Algorithms using Bidirectional iterator
			Board<int, 3> B;
			std::copy_backward(A.cbegin(), A.cend(), B.end());
			EXPECT_EQ(B[0][0], 0);
			EXPECT_EQ(B[8][8], 15);
			EXPECT_EQ(B[8][7], 14);
			EXPECT_EQ(B[8][0], 7);
			EXPECT_EQ(B[7][8], 6);
		}
	}
	else
		ADD_FAILURE();
}

// NOLINTNEXTLINE(readability-function-size)
TEST(Board_Iterator, increment_by_integer)
{ // operator+=(int)
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() += 3));
		static_assert(noexcept(A.cbegin() += 12));
		static_assert(noexcept(A.rbegin() += 14));
		static_assert(noexcept(A.crbegin() += 1));
		static_assert(noexcept(cA.begin() += 14));
		static_assert(noexcept(cA.rbegin() += 9));
		static_assert(noexcept(A.end() += -3));
		static_assert(noexcept(A.cend() += -12));
		static_assert(noexcept(A.rend() += -14));
		static_assert(noexcept(A.crend() += -1));
		static_assert(noexcept(cA.end() += -14));
		static_assert(noexcept(cA.rend() += -9));

		static_assert(
			std::is_same_v<decltype(A.begin())&, decltype(A.begin() += 12)>);
		static_assert(
			std::is_same_v<decltype(A.cbegin())&, decltype(A.cbegin() += 12)>);
		static_assert(std::is_same_v<int&, decltype(*(A.begin() += 12))>);
		static_assert(
			std::is_same_v<int const&, decltype(*(A.cbegin() += 12))>);
		static_assert(std::is_same_v<int&, decltype(*(A.rbegin() += 12))>);
		static_assert(
			std::is_same_v<int const&, decltype(*(A.crbegin() += 12))>);

		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);

		EXPECT_DEBUG_DEATH(A.begin() += -1, ">= 0");
		EXPECT_TRUE((A.begin() += -0) == A.begin());
		EXPECT_EQ(*(A.begin() += -0), 9);
		EXPECT_TRUE((A.begin() += 0) == A.begin());
		EXPECT_EQ(*(A.begin() += 0), 9);
		EXPECT_TRUE((A.begin() += 1) == ++A.begin());
		EXPECT_TRUE((A.begin() += 16) == A.end());
		EXPECT_DEBUG_DEATH(A.begin() += 17, "<= full_size");
		EXPECT_DEBUG_DEATH(A.begin() += 50, "<= full_size");

		EXPECT_DEBUG_DEATH(A.end() += 1, "<= full_size");
		EXPECT_TRUE((A.end() += 0) == A.end());
		EXPECT_TRUE((A.end() += -0) == A.end());
		EXPECT_TRUE((A.end() += -1) == --A.end());
		EXPECT_EQ(*(A.end() += -1), 15);
		EXPECT_TRUE((A.end() += -16) == A.begin());
		EXPECT_EQ(*(A.end() += -16), 9);
		EXPECT_DEBUG_DEATH(A.end() += -17, ">= 0");
		EXPECT_DEBUG_DEATH(A.end() += -50, ">= 0");
		EXPECT_EQ(*(A.begin() += 15), 15);
		{ // return reference to iterator
			auto I = A.begin();
			EXPECT_TRUE((I += 1) == ++A.begin());
			EXPECT_TRUE((I += 1) == (A.begin() += 2));
			EXPECT_TRUE((I += 14) == A.end());
			EXPECT_TRUE((I += -1) == --A.end());
			I = A.end(); // suppress warning: assigned only once
		}
		// const_iterator
		EXPECT_DEBUG_DEATH(A.cbegin() += -1, ">= 0");
		EXPECT_TRUE((A.cbegin() += -0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() += 0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() += 1) == ++A.cbegin());
		EXPECT_TRUE((A.cbegin() += 16) == A.cend());
		EXPECT_DEBUG_DEATH(A.cbegin() += 17, "<= full_size");

		EXPECT_DEBUG_DEATH(A.cend() += 1, "<= full_size");
		EXPECT_TRUE((A.cend() += 0) == A.cend());
		EXPECT_TRUE((A.cend() += -0) == A.cend());
		EXPECT_TRUE((A.cend() += -1) == --A.cend());
		EXPECT_TRUE((A.cend() += -16) == A.cbegin());
		EXPECT_DEBUG_DEATH(A.cend() += -17, ">= 0");
		{ // return reference to iterator
			auto I = A.cbegin();
			EXPECT_TRUE((I += 1) == ++A.cbegin());
			EXPECT_TRUE((I += 1) == (A.cbegin() += 2));
			EXPECT_TRUE((I += 14) == A.cend());
			EXPECT_TRUE((I += -1) == --A.cend());
			I = A.cend(); // suppress warning: assigned only once
		}
		// reverse_iterator
		EXPECT_DEBUG_DEATH(A.rbegin() += -1, "< full_size");
		EXPECT_TRUE((A.rbegin() += -0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() += 0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() += 1) == ++A.rbegin());
		EXPECT_TRUE((A.rbegin() += 16) == A.rend());
		EXPECT_DEBUG_DEATH(A.rbegin() += 17, ">= -1");

		EXPECT_DEBUG_DEATH(A.rend() += 1, ">= -1");
		EXPECT_TRUE((A.rend() += 0) == A.rend());
		EXPECT_TRUE((A.rend() += -0) == A.rend());
		EXPECT_TRUE((A.rend() += -1) == --A.rend());
		EXPECT_TRUE((A.rend() += -16) == A.rbegin());
		EXPECT_DEBUG_DEATH(A.rend() += -17, "< full_size");
		{ // return reference to iterator
			auto I = A.rbegin();
			EXPECT_TRUE((I += 1) == ++A.rbegin());
			EXPECT_TRUE((I += 1) == (A.rbegin() += 2));
			EXPECT_TRUE((I += 14) == A.rend());
			EXPECT_TRUE((I += -1) == --A.rend());
			I = A.rend(); // suppress warning: assigned only once
		}
		// const_reverse_iterator
		EXPECT_DEBUG_DEATH(A.crbegin() += -1, "< full_size");
		EXPECT_TRUE((A.crbegin() += -0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() += 0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() += 1) == ++A.crbegin());
		EXPECT_TRUE((A.crbegin() += 16) == A.crend());
		EXPECT_DEBUG_DEATH(A.crbegin() += 17, ">= -1");

		EXPECT_DEBUG_DEATH(A.crend() += 1, ">= -1");
		EXPECT_TRUE((A.crend() += 0) == A.crend());
		EXPECT_TRUE((A.crend() += -0) == A.crend());
		EXPECT_TRUE((A.crend() += -1) == --A.crend());
		EXPECT_TRUE((A.crend() += -16) == A.crbegin());
		EXPECT_DEBUG_DEATH(A.crend() += -17, "< full_size");
		{ // return reference to iterator
			auto I = A.crbegin();
			EXPECT_TRUE((I += 1) == ++A.crbegin());
			EXPECT_TRUE((I += 1) == (A.crbegin() += 2));
			EXPECT_TRUE((I += 14) == A.crend());
			EXPECT_TRUE((I += -1) == --A.crend());
			I = A.crend(); // suppress warning: assigned only once
		}
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, increment_by_integer2)
{ // operator+(int)
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() + 3));
		static_assert(noexcept(A.cbegin() + 12));
		static_assert(noexcept(A.rbegin() + 14));
		static_assert(noexcept(A.crbegin() + 1));
		static_assert(noexcept(cA.begin() + 14));
		static_assert(noexcept(cA.rbegin() + 9));
		static_assert(noexcept(A.end() + -3));
		static_assert(noexcept(A.cend() + -12));
		static_assert(noexcept(A.rend() + -14));
		static_assert(noexcept(A.crend() + -1));
		static_assert(noexcept(cA.end() + -14));
		static_assert(noexcept(cA.rend() + -9));

		static_assert(
			std::is_same_v<decltype(A.begin()), decltype(A.begin() + 12)>);
		static_assert(std::is_same_v<int&, decltype(*(A.begin() + 12))>);

		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);

		{ // Must return a new iterator object
			const auto X = A.begin();
			const auto Y = X + 3;
			EXPECT_TRUE(Y != A.begin());
			ASSERT_TRUE(X == A.begin()); // Stop on failure.
			const auto cX = A.cbegin();
			const auto cY = cX + 3;
			EXPECT_TRUE(cY != A.cbegin());
			ASSERT_TRUE(cX == A.cbegin()); // Stop on failure.
			const auto rX = A.rbegin();
			const auto rY = rX + 3;
			EXPECT_TRUE(rY != A.rbegin());
			ASSERT_TRUE(rX == A.rbegin()); // Stop on failure.
			const auto crX = A.crbegin();
			const auto crY = crX + 3;
			EXPECT_TRUE(crY != A.crbegin());
			ASSERT_TRUE(crX == A.crbegin()); // Stop on failure.
		}
		{
			[[maybe_unused]] const auto I = A.begin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = I + -1, ">= 0");
			EXPECT_TRUE((I + -0) == A.begin());
			EXPECT_TRUE((I + 0) == A.begin());
			EXPECT_TRUE((I + 1) == ++A.begin());
			EXPECT_TRUE((I + 16) == A.end());
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = I + 17, "<= full_size");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = I + 50, "<= full_size");
		}
		{ // const_iterator
			[[maybe_unused]] const auto cI = A.cbegin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = cI + -1, ">= 0");
			EXPECT_TRUE((A.cbegin() + -0) == A.cbegin());
			EXPECT_TRUE((A.cbegin() + 0) == A.cbegin());
			EXPECT_TRUE((A.cbegin() + 1) == ++A.cbegin());
			EXPECT_TRUE((A.cbegin() + 16) == A.cend());
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] const auto U = cI + 17, "<= full_size");
		}
		{ // reverse_iterator
			[[maybe_unused]] const auto rI = A.rbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = rI + -1, "< full_size");
			EXPECT_TRUE((A.rbegin() + -0) == A.rbegin());
			EXPECT_TRUE((A.rbegin() + 0) == A.rbegin());
			EXPECT_TRUE((A.rbegin() + 1) == ++A.rbegin());
			EXPECT_TRUE((A.rbegin() + 16) == A.rend());
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = rI + 17, ">= -1");
		}
		{ // const_reverse_iterator
			[[maybe_unused]] const auto crI = A.crbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = crI + -1, "< full_size");
			EXPECT_TRUE((A.crbegin() + -0) == A.crbegin());
			EXPECT_TRUE((A.crbegin() + 0) == A.crbegin());
			EXPECT_TRUE((A.crbegin() + 1) == ++A.crbegin());
			EXPECT_TRUE((A.crbegin() + 16) == A.crend());
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = crI + 17, ">= -1");
		}
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, increment_by_integer3)
{ // operator+(int, iterator)
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(3 + A.begin()));
		static_assert(noexcept(5 + A.cbegin()));
		static_assert(noexcept(18 + A.rbegin()));
		static_assert(noexcept(1 + A.crbegin()));
		static_assert(noexcept(12 + cA.begin()));
		static_assert(noexcept(8 + cA.rbegin()));
		static_assert(noexcept(-3 + A.end()));
		static_assert(noexcept(-12 + A.cend()));
		static_assert(noexcept(-4 + A.rend()));
		static_assert(noexcept(-11 + A.crend()));
		static_assert(noexcept(-9 + cA.end()));
		static_assert(noexcept(-12 + cA.rend()));

		static_assert(
			std::is_same_v<decltype(A.begin()), decltype(11 + A.begin())>);
		static_assert(std::is_same_v<int&, decltype(*(9 + A.begin()))>);

		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);

		{ // Must return a new iterator object
			const auto X = A.begin();
			const auto Y = 3 + X;
			EXPECT_TRUE(Y != A.begin());
			ASSERT_TRUE(X == A.begin()); // Stop on failure.
			const auto cX = A.cbegin();
			const auto cY = 5 + cX;
			EXPECT_TRUE(cY != A.cbegin());
			ASSERT_TRUE(cX == A.cbegin()); // Stop on failure.
			const auto rX = A.rbegin();
			const auto rY = 9 + rX;
			EXPECT_TRUE(rY != A.rbegin());
			ASSERT_TRUE(rX == A.rbegin()); // Stop on failure.
			const auto crX = A.crbegin();
			const auto crY = 1 + crX;
			EXPECT_TRUE(crY != A.crbegin());
			ASSERT_TRUE(crX == A.crbegin()); // Stop on failure.
		}
		{
			[[maybe_unused]] const auto I = A.begin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = -1 + I, ">= 0");
			EXPECT_TRUE((-0 + I) == A.begin());
			EXPECT_TRUE((0 + I) == A.begin());
			EXPECT_TRUE((1 + I) == ++A.begin());
			EXPECT_TRUE((16 + I) == A.end());
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = 17 + I, "<= full_size");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = 50 + I, "<= full_size");
		}
		{ // const_iterator
			[[maybe_unused]] const auto cI = A.cbegin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = -1 + cI, ">= 0");
			EXPECT_TRUE((-0 + A.cbegin()) == A.cbegin());
			EXPECT_TRUE((0 + A.cbegin()) == A.cbegin());
			EXPECT_TRUE((1 + A.cbegin()) == ++A.cbegin());
			EXPECT_TRUE((16 + A.cbegin()) == A.cend());
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = 17 + cI, "<= full_size");
		}
		{ // reverse_iterator
			[[maybe_unused]] const auto rI = A.rbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = -1 + rI, "< full_size");
			EXPECT_TRUE((-0 + A.rbegin()) == A.rbegin());
			EXPECT_TRUE((0 + A.rbegin()) == A.rbegin());
			EXPECT_TRUE((1 + A.rbegin()) == ++A.rbegin());
			EXPECT_TRUE((16 + A.rbegin()) == A.rend());
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = 17 + rI, ">= -1");
		}
		{ // const_reverse_iterator
			[[maybe_unused]] const auto crI = A.crbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = -1 + crI, "< full_size");
			EXPECT_TRUE((-0 + A.crbegin()) == A.crbegin());
			EXPECT_TRUE((0 + A.crbegin()) == A.crbegin());
			EXPECT_TRUE((1 + A.crbegin()) == ++A.crbegin());
			EXPECT_TRUE((16 + A.crbegin()) == A.crend());
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = 17 + crI, ">= -1");
		}
	}
	else
		ADD_FAILURE();
}

// NOLINTNEXTLINE(readability-function-size)
TEST(Board_Iterator, decrement_by_integer)
{ // operator-=(int)
	test_elements TE{};
	auto& A = TE.A;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.end() -= 3));
		static_assert(noexcept(A.cend() -= 12));
		static_assert(noexcept(A.rend() -= 14));
		static_assert(noexcept(A.crend() -= 1));

		static_assert(
			std::is_same_v<decltype(A.end())&, decltype(A.end() -= 12)>);
		static_assert(
			std::is_same_v<decltype(A.cend())&, decltype(A.cend() -= 12)>);
		static_assert(std::is_same_v<int&, decltype(*(A.end() -= 12))>);
		static_assert(std::is_same_v<int const&, decltype(*(A.cend() -= 12))>);
		static_assert(std::is_same_v<int&, decltype(*(A.rend() -= 12))>);
		static_assert(std::is_same_v<int const&, decltype(*(A.crend() -= 12))>);

		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);

		EXPECT_DEBUG_DEATH(A.end() -= -1, "<= full_size");
		EXPECT_TRUE((A.end() -= -0) == A.end());
		EXPECT_TRUE((A.end() -= 0) == A.end());
		EXPECT_TRUE((A.end() -= 1) == --A.end());
		EXPECT_TRUE((A.end() -= 16) == A.begin());
		EXPECT_DEBUG_DEATH(A.end() -= 17, ">= 0");

		EXPECT_DEBUG_DEATH(A.begin() -= 1, ">= 0");
		EXPECT_TRUE((A.begin() -= 0) == A.begin());
		EXPECT_TRUE((A.begin() -= -0) == A.begin());
		EXPECT_TRUE((A.begin() -= -1) == ++A.begin());
		EXPECT_EQ(*(A.begin() -= -1), 1);
		EXPECT_TRUE((A.begin() -= -16) == A.end());
		EXPECT_EQ(*(A.end() -= 16), 9);
		EXPECT_DEBUG_DEATH(A.begin() -= -17, "<= full_size");
		{ // return reference to iterator
			auto I = A.end();
			EXPECT_TRUE((I -= 1) == --A.end());
			EXPECT_TRUE((I -= 1) == (A.end() -= 2));
			EXPECT_TRUE((I -= 14) == A.begin());
			EXPECT_TRUE((I -= -1) == ++A.begin());
			I = A.begin(); // suppress warning: assigned only once
		}
		// const_iterator
		EXPECT_DEBUG_DEATH(A.cbegin() -= 1, ">= 0");
		EXPECT_TRUE((A.cbegin() -= 0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() -= -0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() -= -1) == ++A.cbegin());
		EXPECT_TRUE((A.cbegin() -= -16) == A.cend());
		EXPECT_DEBUG_DEATH(A.cbegin() -= -17, "<= full_size");

		EXPECT_DEBUG_DEATH(A.cend() -= -1, "<= full_size");
		EXPECT_TRUE((A.cend() -= -0) == A.cend());
		EXPECT_TRUE((A.cend() -= 0) == A.cend());
		EXPECT_TRUE((A.cend() -= 1) == --A.cend());
		EXPECT_TRUE((A.cend() -= 16) == A.cbegin());
		EXPECT_DEBUG_DEATH(A.cend() -= 17, ">= 0");
		{ // return reference to iterator
			auto I = A.cend();
			EXPECT_TRUE((I -= 1) == --A.cend());
			EXPECT_TRUE((I -= 1) == (A.cend() -= 2));
			EXPECT_TRUE((I -= 14) == A.cbegin());
			EXPECT_TRUE((I -= -1) == ++A.cbegin());
			I = A.cbegin(); // suppress warning: assigned only once
		}
		// reverse_iterator
		EXPECT_DEBUG_DEATH(A.rbegin() -= 1, "< full_size");
		EXPECT_TRUE((A.rbegin() -= 0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() -= -0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() -= -1) == ++A.rbegin());
		EXPECT_TRUE((A.rbegin() -= -16) == A.rend());
		EXPECT_DEBUG_DEATH(A.rbegin() -= -17, ">= -1");

		EXPECT_DEBUG_DEATH(A.rend() -= -1, ">= -1");
		EXPECT_TRUE((A.rend() -= -0) == A.rend());
		EXPECT_TRUE((A.rend() -= 0) == A.rend());
		EXPECT_TRUE((A.rend() -= 1) == --A.rend());
		EXPECT_TRUE((A.rend() -= 16) == A.rbegin());
		EXPECT_DEBUG_DEATH(A.rend() -= 17, "< full_size");
		{ // return reference to iterator
			auto I = A.rend();
			EXPECT_TRUE((I -= 1) == --A.rend());
			EXPECT_TRUE((I -= 1) == (A.rend() -= 2));
			EXPECT_TRUE((I -= 14) == A.rbegin());
			EXPECT_TRUE((I -= -1) == ++A.rbegin());
			I = A.rbegin(); // suppress warning: assigned only once
		}
		// const_reverse_iterator
		EXPECT_DEBUG_DEATH(A.crbegin() -= 1, "< full_size");
		EXPECT_TRUE((A.crbegin() -= 0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() -= -0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() -= -1) == ++A.crbegin());
		EXPECT_TRUE((A.crbegin() -= -16) == A.crend());
		EXPECT_DEBUG_DEATH(A.crbegin() -= -17, ">= -1");

		EXPECT_DEBUG_DEATH(A.crend() -= -1, ">= -1");
		EXPECT_TRUE((A.crend() -= -0) == A.crend());
		EXPECT_TRUE((A.crend() -= 0) == A.crend());
		EXPECT_TRUE((A.crend() -= 1) == --A.crend());
		EXPECT_TRUE((A.crend() -= 16) == A.crbegin());
		EXPECT_DEBUG_DEATH(A.crend() -= 17, "< full_size");
		{ // return reference to iterator
			auto I = A.crend();
			EXPECT_TRUE((I -= 1) == --A.crend());
			EXPECT_TRUE((I -= 1) == (A.crend() -= 2));
			EXPECT_TRUE((I -= 14) == A.crbegin());
			EXPECT_TRUE((I -= -1) == ++A.crbegin());
			I = A.crbegin(); // suppress warning: assigned only once
		}
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, decrement_by_integer2)
{ // operator-(int)
	test_elements TE{};
	auto& A                         = TE.A;
	[[maybe_unused]] auto const& cA = TE.cA;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() - -3));
		static_assert(noexcept(A.cbegin() - -12));
		static_assert(noexcept(A.rbegin() - -14));
		static_assert(noexcept(A.crbegin() - -1));
		static_assert(noexcept(cA.begin() - -14));
		static_assert(noexcept(cA.rbegin() - -9));
		static_assert(noexcept(A.end() - 3));
		static_assert(noexcept(A.cend() - 12));
		static_assert(noexcept(A.rend() - 14));
		static_assert(noexcept(A.crend() - 1));
		static_assert(noexcept(cA.end() - 14));
		static_assert(noexcept(cA.rend() - 9));

		static_assert(
			std::is_same_v<decltype(A.begin()), decltype(A.end() - 12)>);
		static_assert(std::is_same_v<int&, decltype(*(A.end() - 12))>);

		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);

		{ // Must return a new iterator object
			const auto X = A.end();
			const auto Y = X - 2;
			EXPECT_TRUE(Y != A.end());
			ASSERT_TRUE(X == A.end()); // Stop on failure.
			const auto cX = A.cend();
			const auto cY = cX - 5;
			EXPECT_TRUE(cY != A.cend());
			ASSERT_TRUE(cX == A.cend()); // Stop on failure.
			const auto rX = A.rend();
			const auto rY = rX - 1;
			EXPECT_TRUE(rY != A.rend());
			ASSERT_TRUE(rX == A.rend()); // Stop on failure.
			const auto crX = A.crend();
			const auto crY = crX - 4;
			EXPECT_TRUE(crY != A.crend());
			ASSERT_TRUE(crX == A.crend()); // Stop on failure.
		}
		{
			[[maybe_unused]] const auto I = A.end();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = I - -1, "<= full_size");
			EXPECT_TRUE((I - -0) == A.end());
			EXPECT_TRUE((I - 0) == A.end());
			EXPECT_TRUE((I - 1) == --A.end());
			EXPECT_TRUE((I - 16) == A.begin());
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = I - 17, ">= 0");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = I - 50, ">= 0");
		}
		{ // const_iterator
			[[maybe_unused]] const auto cI = A.cend();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = cI - -1, "<= full_size");
			EXPECT_TRUE((A.cend() - -0) == A.cend());
			EXPECT_TRUE((A.cend() - 0) == A.cend());
			EXPECT_TRUE((A.cend() - 1) == --A.cend());
			EXPECT_TRUE((A.cend() - 16) == A.cbegin());
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = cI - 17, ">= 0");
		}
		{ // reverse_iterator
			[[maybe_unused]] const auto rI = A.rend();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = rI - -1, ">= -1");
			EXPECT_TRUE((A.rend() - -0) == A.rend());
			EXPECT_TRUE((A.rend() - 0) == A.rend());
			EXPECT_TRUE((A.rend() - 1) == --A.rend());
			EXPECT_TRUE((A.rend() - 16) == A.rbegin());
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = rI - 17, "< full_size");
		}
		{ // const_reverse_iterator
			[[maybe_unused]] const auto crI = A.crend();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = crI - -1, ">= -1");
			EXPECT_TRUE((A.crend() - -0) == A.crend());
			EXPECT_TRUE((A.crend() - 0) == A.crend());
			EXPECT_TRUE((A.crend() - 1) == --A.crend());
			EXPECT_TRUE((A.crend() - 16) == A.crbegin());
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = crI - 17, "< full_size");
		}
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, difference)
{ // operator-(iterator)
	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() - A.end()));
		static_assert(noexcept(A.end() - A.begin()));
		static_assert(noexcept(A.cend() - A.cbegin()));
		static_assert(noexcept(A.rend() - A.rbegin()));
		static_assert(noexcept(A.crend() - A.crbegin()));

		auto B = A;
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.end() - B.begin(), "is_same_Board");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend() - B.cbegin(), "is_same_Board");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.rend() - B.rbegin(), "is_same_Board");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crend() - B.crbegin(), "is_same_Board");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend() - cA.cbegin(), "is_same_Board");

		// Return type:
		static_assert(
			std::is_same_v<std::ptrdiff_t, decltype(A.begin() - A.end())>);
		static_assert(
			std::is_same_v<std::ptrdiff_t, decltype(A.cbegin() - A.cend())>);
		static_assert(
			std::is_same_v<std::ptrdiff_t, decltype(A.rbegin() - A.rend())>);

		ASSERT_EQ(A.size(), 16u);
		EXPECT_EQ(A.begin() - A.begin(), 0);
		EXPECT_EQ(A.end() - A.end(), 0);
		EXPECT_EQ(A.end() - A.begin(), 16);
		EXPECT_EQ(A.begin() - A.end(), -16);
		EXPECT_EQ(++A.begin() - A.begin(), 1);
		EXPECT_EQ(++A.begin() - ++A.begin(), 0);
		{
			const auto X = ++A.begin();
			const auto Y = A.begin() += 5;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
		EXPECT_EQ(A.cbegin() - A.cbegin(), 0);
		EXPECT_EQ(A.cend() - A.cend(), 0);
		EXPECT_EQ(A.cend() - A.cbegin(), 16);
		EXPECT_EQ(A.cbegin() - A.cend(), -16);
		EXPECT_EQ(++A.cbegin() - A.cbegin(), 1);
		EXPECT_EQ(++A.cbegin() - ++A.cbegin(), 0);
		EXPECT_EQ(cA.begin() - cA.end(), -16);
		EXPECT_EQ(cA.cbegin() - cA.end(), -16);
		{
			const auto X = ++A.cbegin();
			const auto Y = A.cbegin() += 5;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
		EXPECT_EQ(A.rbegin() - A.rbegin(), 0);
		EXPECT_EQ(A.rend() - A.rend(), 0);
		EXPECT_EQ(A.rend() - A.rbegin(), 16);
		EXPECT_EQ(A.rbegin() - A.rend(), -16);
		EXPECT_EQ(++A.rbegin() - A.rbegin(), 1);
		EXPECT_EQ(++A.rbegin() - ++A.rbegin(), 0);
		{
			const auto X = ++A.rbegin();
			const auto Y = A.rbegin() += 5;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
		EXPECT_EQ(A.crbegin() - A.crbegin(), 0);
		EXPECT_EQ(A.crend() - A.crend(), 0);
		EXPECT_EQ(A.crend() - A.crbegin(), 16);
		EXPECT_EQ(A.crbegin() - A.crend(), -16);
		EXPECT_EQ(++A.crbegin() - A.crbegin(), 1);
		EXPECT_EQ(++A.crbegin() - ++A.crbegin(), 0);
		{
			const auto X = ++A.crbegin();
			const auto Y = A.crbegin() += 5;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
	}
	else
		ADD_FAILURE();
}

// NOLINTNEXTLINE(readability-function-size)
TEST(Board_Iterator, direct_access)
{ // & operator[](int)
	test_elements TE{};
	auto& A = TE.A;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.rbegin())> &&
		is_random<decltype(A.cbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin()[0]));
		static_assert(noexcept(A.cbegin()[5]));
		static_assert(noexcept(A.rbegin()[3]));
		static_assert(noexcept(A.crbegin()[6]));
		static_assert(noexcept(A.end()[-6]));
		static_assert(noexcept(A.cend()[-6]));
		static_assert(noexcept(A.rend()[-6]));
		static_assert(noexcept(A.crend()[-6]));

		// Return type:
		static_assert(std::is_same_v<int&, decltype(A.begin()[12])>);
		static_assert(std::is_same_v<int const&, decltype(A.cbegin()[12])>);
		static_assert(std::is_same_v<int&, decltype(A.rbegin()[12])>);
		static_assert(std::is_same_v<int const&, decltype(A.crbegin()[12])>);

		ASSERT_EQ(A.size(), 16u);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 15);

		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.begin()[-1], ">= 0");
		EXPECT_EQ(A.begin()[-0], 9);
		EXPECT_EQ(A.begin()[0], 9);
		EXPECT_EQ(A.begin()[1], 1);
		EXPECT_EQ(A.begin()[15], 15);
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.begin()[16], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin()[17], "<= full_size");
		{ // return dereferenced iterator
			const auto I = A.begin();
			EXPECT_TRUE(I[1] == *(++A.begin()));
			EXPECT_TRUE(I[1] == *(++A.begin()));
			EXPECT_TRUE(I[14] == *(A.begin() + 14));
		}
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.end()[1], "<= full_size");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.end()[0], "is_valid");
		EXPECT_EQ(A.end()[-1], 15);
		EXPECT_EQ(A.end()[-16], 9);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.end()[17], "<= full_size");

		// const_iterator
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.cbegin()[-1], ">= 0");
		EXPECT_EQ(A.cbegin()[-0], 9);
		EXPECT_EQ(A.cbegin()[0], 9);
		EXPECT_EQ(A.cbegin()[1], 1);
		EXPECT_EQ(A.cbegin()[15], 15);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cbegin()[16], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cbegin()[17], "<= full_size");
		{ // return dereferenced iterator
			const auto I = A.cbegin();
			EXPECT_TRUE(I[1] == *(++A.cbegin()));
			EXPECT_TRUE(I[1] == *(++A.cbegin()));
			EXPECT_TRUE(I[14] == *(A.cbegin() + 14));
		}
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend()[1], "<= full_size");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.cend()[0], "is_valid");
		EXPECT_EQ(A.cend()[-1], 15);
		EXPECT_EQ(A.cend()[-16], 9);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend()[17], "<= full_size");

		// reverse_iterator
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.rbegin()[-1], "< full_size");
		EXPECT_EQ(A.rbegin()[-0], 15);
		EXPECT_EQ(A.rbegin()[0], 15);
		EXPECT_EQ(A.rbegin()[1], 14);
		EXPECT_EQ(A.rbegin()[15], 9);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.rbegin()[16], "is_valid");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.rbegin()[17], ">= -1");
		{ // return dereferenced iterator
			const auto I = A.rbegin();
			EXPECT_TRUE(I[1] == *(++A.rbegin()));
			EXPECT_TRUE(I[1] == *(++A.rbegin()));
			EXPECT_TRUE(I[14] == *(A.rbegin() + 14));
		}
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.rend()[1], ">= -1");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.rend()[0], "is_valid");
		EXPECT_EQ(A.rend()[-1], 9);
		EXPECT_EQ(A.rend()[-16], 15);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.rend()[-17], "< full_size");

		// const_reverse_iterator
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crbegin()[-1], "< full_size");
		EXPECT_EQ(A.crbegin()[-0], 15);
		EXPECT_EQ(A.crbegin()[0], 15);
		EXPECT_EQ(A.crbegin()[1], 14);
		EXPECT_EQ(A.crbegin()[15], 9);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crbegin()[16], "is_valid");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.crbegin()[17], ">= -1");
		{ // return dereferenced iterator
			const auto I = A.crbegin();
			EXPECT_TRUE(I[1] == *(++A.crbegin()));
			EXPECT_TRUE(I[1] == *(++A.crbegin()));
			EXPECT_TRUE(I[14] == *(A.crbegin() + 14));
		}
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.crend()[1], ">= -1");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.crend()[0], "is_valid");
		EXPECT_EQ(A.crend()[-1], 9);
		EXPECT_EQ(A.crend()[-16], 15);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crend()[-17], "< full_size");

		//====------------------------------------------------------------====//
		// Output
		static_assert(noexcept(A.begin()[0] = 1));
		static_assert(noexcept(A.rbegin()[0] = 1));
		A.begin()[0] = 1;
		EXPECT_EQ(A[0][0], 1);
		A.begin()[11] = 9;
		EXPECT_EQ(A[2][3], 9);
		A.rbegin()[0] = 8;
		EXPECT_EQ(A[3][3], 8);
		A.rbegin()[11] = 2;
		EXPECT_EQ(A[1][0], 2);
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, comparison)
{
	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() < A.end()));
		static_assert(noexcept(A.begin() <= A.end()));
		static_assert(noexcept(A.begin() > A.end()));
		static_assert(noexcept(A.begin() >= A.end()));
		static_assert(noexcept(A.cbegin() < A.cend()));
		static_assert(noexcept(A.cbegin() <= A.cend()));
		static_assert(noexcept(A.cbegin() > A.cend()));
		static_assert(noexcept(A.cbegin() >= A.cend()));
		static_assert(noexcept(A.rbegin() < A.rend()));
		static_assert(noexcept(A.rbegin() <= A.rend()));
		static_assert(noexcept(A.rbegin() > A.rend()));
		static_assert(noexcept(A.rbegin() >= A.rend()));
		static_assert(noexcept(A.crbegin() < A.crend()));
		static_assert(noexcept(A.crbegin() <= A.crend()));
		static_assert(noexcept(A.crbegin() > A.crend()));
		static_assert(noexcept(A.crbegin() >= A.crend()));

		// iterator
		EXPECT_FALSE(A.begin() < A.begin());
		EXPECT_TRUE(A.begin() < A.end());
		EXPECT_FALSE(A.end() < A.begin());
		EXPECT_TRUE(A.begin() <= A.begin());
		EXPECT_TRUE(A.begin() <= A.end());
		EXPECT_FALSE(A.end() <= A.begin());
		EXPECT_FALSE(A.begin() > A.begin());
		EXPECT_FALSE(A.begin() > A.end());
		EXPECT_TRUE(A.end() > A.begin());
		EXPECT_TRUE(A.begin() >= A.begin());
		EXPECT_FALSE(A.begin() >= A.end());
		EXPECT_TRUE(A.end() >= A.begin());
		// const_iterator
		EXPECT_FALSE(A.cbegin() < A.cbegin());
		EXPECT_TRUE(A.cbegin() < A.cend());
		EXPECT_FALSE(A.cend() < A.cbegin());
		EXPECT_TRUE(A.cbegin() <= A.cbegin());
		EXPECT_TRUE(A.cbegin() <= A.cend());
		EXPECT_FALSE(A.cend() <= A.cbegin());
		EXPECT_FALSE(A.cbegin() > A.cbegin());
		EXPECT_FALSE(A.cbegin() > A.cend());
		EXPECT_TRUE(A.cend() > A.cbegin());
		EXPECT_TRUE(A.cbegin() >= A.cbegin());
		EXPECT_FALSE(A.cbegin() >= A.cend());
		EXPECT_TRUE(A.cend() >= A.cbegin());
		// const_iterator
		EXPECT_FALSE(A.rbegin() < A.rbegin());
		EXPECT_TRUE(A.rbegin() < A.rend());
		EXPECT_FALSE(A.rend() < A.rbegin());
		EXPECT_TRUE(A.rbegin() <= A.rbegin());
		EXPECT_TRUE(A.rbegin() <= A.rend());
		EXPECT_FALSE(A.rend() <= A.rbegin());
		EXPECT_FALSE(A.rbegin() > A.rbegin());
		EXPECT_FALSE(A.rbegin() > A.rend());
		EXPECT_TRUE(A.rend() > A.rbegin());
		EXPECT_TRUE(A.rbegin() >= A.rbegin());
		EXPECT_FALSE(A.rbegin() >= A.rend());
		EXPECT_TRUE(A.rend() >= A.rbegin());
		// const_iterator
		EXPECT_FALSE(A.crbegin() < A.crbegin());
		EXPECT_TRUE(A.crbegin() < A.crend());
		EXPECT_FALSE(A.crend() < A.crbegin());
		EXPECT_TRUE(A.crbegin() <= A.crbegin());
		EXPECT_TRUE(A.crbegin() <= A.crend());
		EXPECT_FALSE(A.crend() <= A.crbegin());
		EXPECT_FALSE(A.crbegin() > A.crbegin());
		EXPECT_FALSE(A.crbegin() > A.crend());
		EXPECT_TRUE(A.crend() > A.crbegin());
		EXPECT_TRUE(A.crbegin() >= A.crbegin());
		EXPECT_FALSE(A.crbegin() >= A.crend());
		EXPECT_TRUE(A.crend() >= A.crbegin());
		// combine on const object
		EXPECT_FALSE(cA.begin() < cA.cbegin());
		EXPECT_TRUE(cA.begin() < cA.cend());
		EXPECT_FALSE(cA.end() < cA.cbegin());
		EXPECT_TRUE(cA.begin() <= cA.cbegin());
		EXPECT_TRUE(cA.begin() <= cA.cend());
		EXPECT_FALSE(cA.end() <= cA.cbegin());
		EXPECT_TRUE(cA.begin() >= cA.cbegin());
		EXPECT_FALSE(cA.begin() >= cA.cend());
		EXPECT_TRUE(cA.end() >= cA.cbegin());
		EXPECT_FALSE(cA.begin() > cA.cbegin());
		EXPECT_FALSE(cA.begin() > cA.cend());
		EXPECT_TRUE(cA.end() > cA.cbegin());
		EXPECT_FALSE(cA.cbegin() < cA.begin());
		EXPECT_TRUE(cA.cbegin() < cA.end());
		EXPECT_FALSE(cA.cend() < cA.begin());
		EXPECT_TRUE(cA.cbegin() <= cA.begin());
		EXPECT_TRUE(cA.cbegin() <= cA.end());
		EXPECT_FALSE(cA.cend() <= cA.begin());
		EXPECT_TRUE(cA.cbegin() >= cA.begin());
		EXPECT_FALSE(cA.cbegin() >= cA.end());
		EXPECT_TRUE(cA.cend() >= cA.begin());
		EXPECT_FALSE(cA.cbegin() > cA.begin());
		EXPECT_FALSE(cA.cbegin() > cA.end());
		EXPECT_TRUE(cA.cend() > cA.begin());

		// different boards
		auto B  = A;
		B[1][2] = 0;
		A[1][2] = 8;
		static_assert(noexcept(A.begin() < B.end()));
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() < B.end(), "is_same_Board");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() <= B.end(), "is_same_Board");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() >= B.end(), "is_same_Board");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() > B.end(), "is_same_Board");
#ifdef NDEBUG
		// different from default vector/pointer implementations
		EXPECT_TRUE(A.begin() < B.end());
		EXPECT_TRUE(B.begin() < A.end());
		EXPECT_TRUE(A.begin() <= B.begin());
		EXPECT_TRUE(B.begin() <= A.begin());
		EXPECT_TRUE(A.begin() >= B.begin());
		EXPECT_TRUE(B.begin() >= A.begin());
		EXPECT_FALSE(A.begin() > B.begin());
		EXPECT_FALSE(A.begin() > B.end());
		EXPECT_TRUE(A.end() > B.begin());
		EXPECT_TRUE(B.end() > A.begin());
#endif // NDEBUG
	}
	else
		ADD_FAILURE();
}

TEST(Board_Iterator, RandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff  = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		// shuffle(start, stop) requires a random iterator
		ASSERT_EQ(A, cA);
		std::random_device rd;   // non-deterministic generator
		std::mt19937 urng(rd()); // to seed Mersenne Twister
		EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
		EXPECT_NE(A, cA);
	}
	else
		ADD_FAILURE();
}

template<typename T>
int range_for_sum(T structure)
{
	int total{};
	try
	{
		for (auto& s : structure)
		{
			total += s;
		}
	}
	catch (...)
	{
		ADD_FAILURE() << "rang for sum failed on supplied data";
	}
	return total;
}

TEST(Board_Iterator, IteratorLoop)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	int total{};
	// manual
	for (auto itr = A.begin(); itr < A.end(); ++itr)
	{
		total += *itr;
	}
	EXPECT_EQ(total, 129);

	total = range_for_sum(A);
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(A.row(i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(A.col(i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(A.block(i));
	}
	EXPECT_EQ(total, 129);
	total = range_for_sum(cA);
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(cA.row(i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(cA.col(i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(cA.block(i));
	}
	EXPECT_EQ(total, 129);
}

} // namespace SudokuTests::BoardTest
