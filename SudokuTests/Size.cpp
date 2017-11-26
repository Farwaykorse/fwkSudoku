//===--	SudokuTests/Size.cpp										--===//
//
//	Unit tests for the template class Sudoku::Size
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
#include "../Sudoku/Size.h"

// Helpers

// aditional


using namespace Sudoku;

namespace SudokuTests::SizeTest
{
namespace compiletime
{ // Type properties
	using typeT = Size<3>;
	static_assert(
		std::is_class<typeT>::value, "a class, hiding datarepresentation");
	static_assert(std::is_trivial<typeT>::value); // ++
	// trivial default constructors & trivially copyable
	static_assert(std::is_trivially_copyable<typeT>::value); // ++
	// compatible with std::memcpy & binary copy from/to files
	static_assert(std::is_standard_layout<typeT>::value); // ++
	// StandardLayoutType can be converted with reinterpret_cast
	static_assert(std::is_pod<typeT>::value); // ++
	// Plain Old Data, both trivial and standard-layout, C compatible");
	// static_assert(std::has_unique_object_representations<typeT>::value);
	// C++17	trivially_copyable same object representation
	static_assert(std::is_empty<typeT>::value);
	// class with datamembers, nothing virtual
	static_assert(!std::is_polymorphic<typeT>::value); // --
	// inherits atleast one virtual function
	static_assert(!std::is_final<typeT>::value); // cannot be used as base class
	static_assert(!std::is_abstract<typeT>::value); // --
	// inherits or declares at least one pure virtual function

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

	// copy assingment
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

	static_assert(std::is_swappable<typeT>::value);         // C++17
	static_assert(std::is_nothrow_swappable<typeT>::value); // C++17

	static_assert(!std::is_swappable_with<typeT, int>::value);          // C++17
	static_assert(!std::is_swappable_with<typeT, unsigned int>::value); // C++17

	static_assert(Size<2>::base == 2);
	static_assert(Size<3>::base == 3);
	static_assert(Size<3>::elem == 9);
	static_assert(Size<3>::full == 81);
	static_assert(Size<3>::base == base_size<3>);
	static_assert(Size<3>::elem == elem_size<3>);
	static_assert(Size<3>::full == full_size<3>);

	static_assert(std::is_scalar_v<decltype(Size<3>::base)>);
	static_assert(std::is_scalar_v<decltype(Size<3>::elem)>);
	static_assert(std::is_scalar_v<decltype(Size<3>::full)>);
	static_assert(std::is_signed_v<decltype(Size<3>::base)>);

} // namespace compiletime

} // namespace SudokuTests::SizeTest
