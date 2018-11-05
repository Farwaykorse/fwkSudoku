//====---- SudokuTests/Board_Section.cpp                              ----====//
//
// Unit tests for the template class Sudoku::Board_Section
//====--------------------------------------------------------------------====//
// Implemented with GoogleTest
//
// Notes:
// gTest is limited for use with multiple template parameters.
// These expressions need to be implemented between extra parentheses.
//
//====--------------------------------------------------------------------====//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Board_Section.h>
// helpers
#include <Sudoku/Board.h>
#include <Sudoku/Value.h>
// library
#include <type_traits>
#include <utility>


namespace SudokuTests::Type
{
namespace properties_Section
{
	using typeT = ::Sudoku::Board_Section::Section;
	// Composite Type Categories
	static_assert(not std::is_fundamental_v<typeT>);
	static_assert(std::is_compound_v<typeT>);
	static_assert(std::is_object_v<typeT>);

	static_assert(std::is_scalar_v<typeT>);
	static_assert(not std::is_arithmetic_v<typeT>);
	static_assert(not std::is_null_pointer_v<typeT>);
	static_assert(not std::is_member_pointer_v<typeT>);
	static_assert(not std::is_pointer_v<typeT>);
	static_assert(std::is_enum_v<typeT>);
	static_assert(not std::is_array_v<typeT>);
	static_assert(not std::is_class_v<typeT>);
	static_assert(not std::is_reference_v<typeT>);
	static_assert(not std::is_function_v<typeT>);
	// Type Properties
	static_assert(not std::is_const_v<typeT>);
	static_assert(not std::is_volatile_v<typeT>);
	static_assert(std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
#if not(defined(__ICL)) // Intel C++ 19.0
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif // __ICL
	static_assert(not std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
#if not(defined(__ICL)) // Intel C++ 19.0
	static_assert(not std::is_aggregate_v<typeT>);
#endif // __ICL
} // namespace properties_Section

// Board_Section
using dataT            = int;
constexpr int size     = 3;
constexpr auto section = ::Sudoku::Board_Section::Section::row;
using Board            = ::Sudoku::Board<dataT, size>;
using Loc              = ::Sudoku::Location<size>;
using L_B              = ::Sudoku::Location_Block<size>;

using typeT =
	::Sudoku::Board_Section::Board_Section_<dataT, size, section, false>;
using constT =
	::Sudoku::Board_Section::Board_Section_<dataT, size, section, true>;

using Row         = typename ::Sudoku::Board_Section::Row<dataT, size>;
using Col         = typename ::Sudoku::Board_Section::Col<dataT, size>;
using Block       = typename ::Sudoku::Board_Section::Block<dataT, size>;
using const_Row   = typename ::Sudoku::Board_Section::const_Row<dataT, size>;
using const_Col   = typename ::Sudoku::Board_Section::const_Col<dataT, size>;
using const_Block = typename ::Sudoku::Board_Section::const_Block<dataT, size>;

namespace type_properties
{ // http://howardhinnant.github.io/TypeHiearchy.pdf
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
	static_assert(not std::is_const_v<constT>);
	static_assert(not std::is_volatile_v<typeT>);
	static_assert(not std::is_trivial_v<typeT>);
#if defined(__clang__)
	static_assert(std::is_trivially_copyable_v<typeT>);
#else
	static_assert(not std::is_trivially_copyable_v<typeT>);
#endif // __clang__
	static_assert(not std::is_standard_layout_v<typeT>);
#if not(defined(__ICL)) // Intel C++ 19.0
	static_assert(not std::is_aggregate_v<typeT>);
#endif // __ICL
	static_assert(not std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
} // namespace type_properties

namespace constructors
{
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

	//====----------------------------------------------------------------====//
	// just a Board
	static_assert(not std::is_constructible_v<typeT, Board&>);

	//====----------------------------------------------------------------====//
	// Board& and element number:
	static_assert(std::is_constructible_v<typeT, Board&, int>);
	static_assert(std::is_nothrow_constructible_v<typeT, Board&, int>);
	static_assert(std::is_constructible_v<constT, Board&, int>);
	static_assert(std::is_nothrow_constructible_v<constT, Board&, int>);
	// Board const&
	static_assert(std::is_constructible_v<constT, Board const&, int>);
	static_assert(std::is_nothrow_constructible_v<constT, Board const&, int>);
	static_assert(not std::is_constructible_v<typeT, Board const&, int>);
	// (copy) construction of Board
#if defined(__ICL) // Intel C++ 19.0
	static_assert(std::is_constructible_v<typeT, Board, int>);
#else
	static_assert(not std::is_constructible_v<typeT, Board, int>);
#endif // __ICL
	// Board*
	static_assert(not std::is_constructible_v<typeT, Board*, int>);
	// Board: Size errors
	static_assert(
		not std::
			is_constructible_v<typeT, Sudoku::Board<dataT, size + 1>&, int>);
	// Board: different data-type
	static_assert(
		not std::is_constructible_v<typeT, Sudoku::Board<float, size>&, int>);
	// section id
	// - blocked implicit conversions:
	static_assert(not std::is_constructible_v<typeT, Board&, bool>);
	// - re-enabled implicit conversions for [id]:
	static_assert(std::is_constructible_v<typeT, Board&, long>);
	static_assert(std::is_constructible_v<typeT, Board&, long long>);
	static_assert(std::is_constructible_v<typeT, Board&, size_t>);
	static_assert(std::is_constructible_v<typeT, Board&, unsigned long>);
	static_assert(std::is_constructible_v<typeT, Board&, unsigned long long>);
	static_assert(not std::is_constructible_v<typeT, Board&, float>);
	static_assert(not std::is_constructible_v<typeT, Board&, double>);
	static_assert(not std::is_constructible_v<typeT, Board&, long double>);

	TEST(Board_Section, constructor_int)
	{
		Board board{};

		EXPECT_DEBUG_DEATH((Row(board, -1)), "is_valid_size");
		[[maybe_unused]] Row row0(board, 0);
		[[maybe_unused]] Row row8(board, 8);
		EXPECT_DEBUG_DEATH((Row(board, 9)), "is_valid_size");

		EXPECT_DEBUG_DEATH((const_Row(board, -1)), "is_valid_size");
		[[maybe_unused]] const_Row crow0(board, 0);
		[[maybe_unused]] const_Row crow8(board, 8);
		EXPECT_DEBUG_DEATH((const_Row(board, 9)), "is_valid_size");
	}

	//====----------------------------------------------------------------====//
	// Board& and Location
	static_assert(std::is_constructible_v<typeT, Board&, Loc>);
	static_assert(std::is_nothrow_constructible_v<typeT, Board&, Loc>);
	static_assert(std::is_constructible_v<typeT, Board&, Loc&>);
	static_assert(std::is_constructible_v<typeT, Board&, Loc&&>);
	static_assert(not std::is_constructible_v<typeT, Board&, Loc*>);
	static_assert(std::is_constructible_v<typeT, Board&, L_B>);
	static_assert(std::is_nothrow_constructible_v<typeT, Board&, L_B>);
	TEST(Board_Section, constructor_Loc)
	{
		Board board{};

		EXPECT_DEBUG_DEATH((Row(board, Loc{-1})), "is_valid");
		[[maybe_unused]] Row row0(board, Loc{0});
		[[maybe_unused]] Row row1(board, Loc{9});
		[[maybe_unused]] Row row1_again(board, Loc{10});
		[[maybe_unused]] Row row8(board, Loc{8, 0});
		EXPECT_DEBUG_DEATH((Row(board, Loc{81})), "is_valid");
	}
	// is_constructible between Board_Section types
	// To const:
	static_assert(std::is_constructible_v<const_Row, Row>);
	static_assert(std::is_constructible_v<const_Col, Col>);
	static_assert(std::is_constructible_v<const_Block, Block>);
	static_assert(std::is_nothrow_constructible_v<const_Row, Row>);
	static_assert(std::is_nothrow_constructible_v<const_Col, Col>);
	static_assert(std::is_nothrow_constructible_v<const_Block, Block>);
	// Implicit convertible to const
	static_assert(std::is_convertible_v<Row, const_Row>);
	static_assert(std::is_convertible_v<Col, const_Col>);
	static_assert(std::is_convertible_v<Block, const_Block>);
	TEST(Board_Section, conversion_to_const)
	{
		Board board{};
		Row row(board, 3);
		constexpr int last = ::Sudoku::elem_size<size> - 1;

		EXPECT_EQ(const_Row(row).id(), 3);
		EXPECT_EQ(const_Row(Row(board, 3)).id(), 3);

		EXPECT_EQ(const_Col(Col(board, 0)).id(), 0);
		EXPECT_EQ(const_Col(Col(board, 3)).id(), 3);
		EXPECT_EQ(const_Col(Col(board, last)).id(), last);

		EXPECT_EQ(const_Block(Block(board, 0)).id(), 0);
		EXPECT_EQ(const_Block(Block(board, last)).id(), last);
	}
	// From const:
	static_assert(not std::is_constructible_v<Row, const_Row>);
	static_assert(not std::is_constructible_v<Col, const_Col>);
	static_assert(not std::is_constructible_v<Block, const_Block>);
	static_assert(not std::is_constructible_v<Row, const_Row, Loc>);
	static_assert(not std::is_constructible_v<Col, const_Col, Loc>);
	static_assert(not std::is_constructible_v<Block, const_Block, Loc>);
	// To other type (maintaining const):
	static_assert(not std::is_constructible_v<Row, Col>);
	static_assert(not std::is_constructible_v<Row, Block>);
	static_assert(not std::is_constructible_v<Col, Row>);
	static_assert(not std::is_constructible_v<Col, Block>);
	static_assert(not std::is_constructible_v<Block, Row>);
	static_assert(not std::is_constructible_v<Block, Col>);
	static_assert(not std::is_constructible_v<const_Row, const_Col>);
	static_assert(not std::is_constructible_v<const_Row, const_Block>);
	static_assert(not std::is_constructible_v<const_Col, const_Row>);
	static_assert(not std::is_constructible_v<const_Col, const_Block>);
	static_assert(not std::is_constructible_v<const_Block, const_Row>);
	static_assert(not std::is_constructible_v<const_Block, const_Col>);
	// - Around a pivot element
	static_assert(std::is_constructible_v<Row, Col, int>);
	static_assert(std::is_constructible_v<Row, Block, int>);
	static_assert(std::is_constructible_v<Col, Row, int>);
	static_assert(std::is_constructible_v<Col, Block, int>);
	static_assert(std::is_constructible_v<Block, Row, int>);
	static_assert(std::is_constructible_v<Block, Col, int>);
	static_assert(std::is_constructible_v<const_Row, const_Col, int>);
	static_assert(std::is_constructible_v<const_Row, const_Block, int>);
	static_assert(std::is_constructible_v<const_Col, const_Row, int>);
	static_assert(std::is_constructible_v<const_Col, const_Block, int>);
	static_assert(std::is_constructible_v<const_Block, const_Row, int>);
	static_assert(std::is_constructible_v<const_Block, const_Col, int>);
	static_assert(std::is_nothrow_constructible_v<Row, Col, int>);
	static_assert(std::is_nothrow_constructible_v<Row, Block, int>);
	static_assert(std::is_nothrow_constructible_v<Col, Row, int>);
	static_assert(std::is_nothrow_constructible_v<Col, Block, int>);
	static_assert(std::is_nothrow_constructible_v<Block, Row, int>);
	static_assert(std::is_nothrow_constructible_v<Block, Col, int>);
	static_assert(std::is_nothrow_constructible_v<const_Row, const_Col, int>);
	static_assert(std::is_nothrow_constructible_v<const_Row, const_Block, int>);
	static_assert(std::is_nothrow_constructible_v<const_Col, const_Row, int>);
	static_assert(std::is_nothrow_constructible_v<const_Col, const_Block, int>);
	static_assert(std::is_nothrow_constructible_v<const_Block, const_Row, int>);
	static_assert(std::is_nothrow_constructible_v<const_Block, const_Col, int>);
	// - Unnecessary
	static_assert(std::is_constructible_v<Row, Row, int>);
	static_assert(std::is_constructible_v<const_Col, const_Col, int>);
	static_assert(std::is_constructible_v<Block, Block, int>);
	TEST(Board_Section, conversion)
	{
		using ::Sudoku::elem_size;

		Board board{};
		constexpr int last = elem_size<size> - 1;
		Row row(board, 2);

		EXPECT_DEBUG_DEATH(Col(row, -1), "is_valid_size"); // in location(elem)
		EXPECT_EQ(Col(row, 0).id(), 0);
		EXPECT_EQ(Col(row, 1).id(), 1);
		EXPECT_EQ(Col(row, 3).id(), 3);
		EXPECT_EQ(Col(row, last).id(), last);
		EXPECT_DEBUG_DEATH(Col(row, elem_size<size>), "is_valid_size");

		EXPECT_DEBUG_DEATH(Block(row, -1), "is_valid_size"); // location(elem)
		EXPECT_DEBUG_DEATH(Block(row, elem_size<size>), "is_valid_size");
		if constexpr (size == 2)
		{
			EXPECT_EQ(Block(row, 0).id(), 2);
			EXPECT_EQ(Block(row, 1).id(), 2);
			EXPECT_EQ(Block(row, 2).id(), 3);
		}
		else if constexpr (size == 3)
		{
			EXPECT_EQ(Row(Row(board, 0), 3).id(), 0);
			EXPECT_EQ(Row(Row(board, 8), 3).id(), 8);
			EXPECT_EQ(Col(row, 0).id(), 0);
			EXPECT_EQ(Col(row, 8).id(), 8);
			EXPECT_EQ(Col(Row(board, 7), 3).id(), 3);
			EXPECT_EQ(Block(row, 0).id(), 0);
			EXPECT_EQ(Block(row, 1).id(), 0);
			EXPECT_EQ(Block(row, 3).id(), 1);
			EXPECT_EQ(Block(row, last).id(), 2);
			EXPECT_EQ(Block(Row(board, 4), 7).id(), 5);
			EXPECT_EQ(Block(Row(board, 7), 3).id(), 7);
			EXPECT_EQ(Block(Row(board, 6), 6).id(), 8);
			EXPECT_EQ(Block(Row(board, 8), 8).id(), 8);

			EXPECT_EQ(Col(Col(board, 0), 3).id(), 0);
			EXPECT_EQ(Col(Col(board, 7), 0).id(), 7);
			EXPECT_EQ(Col(Col(board, 7), 3).id(), 7);
			EXPECT_EQ(Row(Col(board, 7), 3).id(), 3);
			EXPECT_EQ(Row(Col(board, 7), 3).id(), 3);
			EXPECT_EQ(Block(Col(board, 3), 2).id(), 1);
			EXPECT_EQ(Block(Col(board, 3), 3).id(), 4);

			EXPECT_EQ(Row(Block(board, 7), 6).id(), 8);
			EXPECT_EQ(Col(Block(board, 7), 6).id(), 3);
		}
	}
	// To other type (gaining const)
	static_assert(std::is_constructible_v<const_Row, Col, int>);
	static_assert(std::is_constructible_v<const_Row, Block, int>);
	static_assert(std::is_constructible_v<const_Col, Row, int>);
	static_assert(std::is_constructible_v<const_Col, Block, int>);
	static_assert(std::is_nothrow_constructible_v<const_Row, Col, int>);
	static_assert(std::is_nothrow_constructible_v<const_Row, Block, int>);
	static_assert(std::is_nothrow_constructible_v<const_Col, Row, int>);
	static_assert(std::is_nothrow_constructible_v<const_Col, Block, int>);
	TEST(Board_Section, conversion_and_const)
	{
		Board board{};
		constexpr int last = ::Sudoku::elem_size<size> - 1;

		EXPECT_DEBUG_DEATH(const_Col(Row(board, 1), -1), "is_valid_size");
		EXPECT_EQ(const_Col(Row(board, 1), 0).id(), 0);
		EXPECT_EQ(const_Col(Row(board, 1), 1).id(), 1);
		EXPECT_EQ(const_Col(Row(board, 1), 3).id(), 3);
		EXPECT_EQ(const_Col(Row(board, 1), last).id(), last);
		EXPECT_DEBUG_DEATH(const_Col(Row(board, 1), last + 1), "is_valid_size");

		EXPECT_DEBUG_DEATH(const_Block(Row(board, 1), -1), "is_valid_size");
		EXPECT_DEBUG_DEATH(
			const_Block(Row(board, 1), last + 1), "is_valid_size");
		if constexpr (size == 2)
		{
			EXPECT_EQ(const_Block(Row(board, 1), 0).id(), 2);
			EXPECT_EQ(const_Block(Row(board, 1), 1).id(), 2);
			EXPECT_EQ(const_Block(Row(board, 1), 2).id(), 3);
		}
		else if constexpr (size == 3)
		{
			static_assert(last == 8);
			EXPECT_EQ(const_Row(Row(board, 0), 3).id(), 0);
			EXPECT_EQ(const_Row(Row(board, 8), 3).id(), 8);
			EXPECT_EQ(const_Col(Row(board, 1), 0).id(), 0);
			EXPECT_EQ(const_Col(Row(board, 1), 8).id(), 8);
			EXPECT_EQ(const_Col(Row(board, 7), 3).id(), 3);
			EXPECT_EQ(const_Block(Row(board, 1), 0).id(), 0);
			EXPECT_EQ(const_Block(Row(board, 1), 1).id(), 0);
			EXPECT_EQ(const_Block(Row(board, 1), 3).id(), 1);
			EXPECT_EQ(const_Block(Row(board, 1), last).id(), 2);
			EXPECT_EQ(const_Block(Row(board, 4), 7).id(), 5);
			EXPECT_EQ(const_Block(Row(board, 7), 3).id(), 7);
			EXPECT_EQ(const_Block(Row(board, 6), 6).id(), 8);
			EXPECT_EQ(const_Block(Row(board, 8), 8).id(), 8);

			EXPECT_EQ(const_Col(Col(board, 0), 3).id(), 0);
			EXPECT_EQ(const_Col(Col(board, 7), 0).id(), 7);
			EXPECT_EQ(const_Col(Col(board, 7), 3).id(), 7);
			EXPECT_EQ(const_Row(Col(board, 7), 3).id(), 3);
			EXPECT_EQ(const_Block(Col(board, 3), 2).id(), 1);
			EXPECT_EQ(const_Block(Col(board, 3), 3).id(), 4);

			EXPECT_EQ(const_Row(Block(board, 7), 6).id(), 8);
			EXPECT_EQ(const_Col(Block(board, 7), 6).id(), 3);
		}
	}
	// To other type (losing const):
	static_assert(not std::is_constructible_v<Row, const_Col>);
	static_assert(not std::is_constructible_v<Row, const_Block>);
	static_assert(not std::is_constructible_v<Col, const_Row>);
	static_assert(not std::is_constructible_v<Col, const_Block>);
	static_assert(not std::is_constructible_v<Block, const_Row>);
	static_assert(not std::is_constructible_v<Block, const_Col>);
	static_assert(not std::is_constructible_v<Row, const_Col, int>);
	static_assert(not std::is_constructible_v<Row, const_Block, int>);
	static_assert(not std::is_constructible_v<Col, const_Row, int>);
	static_assert(not std::is_constructible_v<Col, const_Block, int>);
	static_assert(not std::is_constructible_v<Block, const_Row, int>);
	static_assert(not std::is_constructible_v<Block, const_Col, int>);
	// From a reference
	static_assert(std::is_constructible_v<Row, Row&>);
	static_assert(std::is_constructible_v<const_Row, Row&>);
	static_assert(std::is_constructible_v<Row, Row&, int>);
	static_assert(std::is_constructible_v<Row, Col&, int>);
	static_assert(std::is_constructible_v<const_Row, Row&, int>);
	static_assert(not std::is_constructible_v<Row, Row*>);
} // namespace constructors

namespace destructors
{
	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(std::is_trivially_destructible_v<typeT>);
	static_assert(not std::has_virtual_destructor_v<typeT>);
} // namespace destructors

namespace swapping
{
#if not(defined(__ICL)) // Intel C++ 19.0
	static_assert(not std::is_swappable_v<typeT>);         // C++17
	static_assert(not std::is_nothrow_swappable_v<typeT>); // C++17

	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
#endif // __ICL
} // namespace swapping

namespace assignment
{
	// Copy assignment
	static_assert(not std::is_copy_assignable_v<typeT>);
	static_assert(not std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(not std::is_trivially_copy_assignable_v<typeT>);

	static_assert(not std::is_move_assignable_v<typeT>);
	static_assert(not std::is_nothrow_move_assignable_v<typeT>);
	static_assert(not std::is_trivially_move_assignable_v<typeT>);

	// Assignment from different const
	static_assert(not std::is_assignable_v<Row, const_Row>);
	static_assert(not std::is_assignable_v<Col, const_Col>);
	static_assert(not std::is_assignable_v<Block, const_Block>);

	static_assert(not std::is_assignable_v<const_Row, Row>);
	static_assert(not std::is_assignable_v<const_Col, Col>);
	static_assert(not std::is_assignable_v<const_Block, Block>);

	// Assignment from other sections
	static_assert(not std::is_assignable_v<Row, Col>);
	static_assert(not std::is_assignable_v<Row, Block>);
	static_assert(not std::is_assignable_v<Col, Row>);
	static_assert(not std::is_assignable_v<Col, Block>);
	// ...
} // namespace assignment
} // namespace SudokuTests::Type

namespace SudokuTests::Members
{
using ::Sudoku::Board;
using ::Sudoku::Board_Section::Row;
using ::Sudoku::Board_Section::Col;
using ::Sudoku::Board_Section::Block;
using ::Sudoku::Board_Section::const_Row;
using ::Sudoku::Board_Section::const_Col;
using ::Sudoku::Board_Section::const_Block;
using ::Sudoku::Location;
using ::Sudoku::Location_Block;

TEST(Board_Section, size)
{
	static_assert(noexcept(Row<int, 3>::size()));
	// return type
	static_assert(std::is_same_v<int, decltype(Row<int, 3>::size())>);

	static_assert(Row<int, 2>::size() == 4);
	static_assert(Row<int, 3>::size() == 9);
}

TEST(Board_Section, id)
{
	using Row   = Row<int, 3>;
	using Col   = Col<int, 3>;
	using Block = Block<int, 3>;
	Board<int, 3> board{};

	static_assert(noexcept(Row(board, 2).id()));
	// return type
	static_assert(std::is_same_v<gsl::index, decltype(Row(board, 4).id())>);
	static_assert(std::is_same_v<gsl::index, decltype(Col(board, 5).id())>);

	EXPECT_EQ(Row(board, 0).id(), 0);
	EXPECT_EQ(Row(board, 1).id(), 1);
	EXPECT_EQ(Row(board, 8).id(), 8);
	EXPECT_EQ(Col(board, 3).id(), Row(board, 3).id());
	EXPECT_EQ(Col(board, 5).id(), Block(board, 5).id());

	static_assert(Row(board, 0).id() == 0);
	static_assert(Row(board, 1).id() == 1);
	static_assert(Row(board, 8).id() == 8);
	static_assert(Col(board, 2).id() == 2);
	static_assert(Block(board, 7).id() == 7);
}

TEST(Board_Section, location)
{
	constexpr int size{3};
	using L     = Location<size>;
	using B     = Location_Block<size>;
	using Row   = Row<int, size>;
	using Col   = Col<int, size>;
	using Block = Block<int, size>;

	Board<int, size> board{};
	Row row(board, 0);
	static_assert(noexcept(row.location(0)));
	// return type
	static_assert(std::is_same_v<L, decltype(Row(board, 4).location(2))>);

	EXPECT_DEBUG_DEATH(
		[[maybe_unused]] L U = row.location(-1), "is_valid_size");
	EXPECT_EQ(Row(board, 0).location(0), L(0, 0));
	EXPECT_EQ(Row(board, 0).location(8), L(0, 8));
	EXPECT_DEBUG_DEATH([[maybe_unused]] L U = row.location(9), "is_valid_size");

	EXPECT_EQ(Row(board, 0).location(0), L(0, 0));
	EXPECT_EQ(Row(board, 1).location(0), L(1, 0));
	EXPECT_EQ(Row(board, 3).location(6), L(3, 6));
	EXPECT_EQ(Col(board, 0).location(0), L(0, 0));
	EXPECT_EQ(Col(board, 1).location(0), L(0, 1));
	EXPECT_EQ(Col(board, 3).location(6), L(6, 3));
	EXPECT_EQ(Block(board, 0).location(0), L(0, 0));
	EXPECT_EQ(Block(board, 5).location(0), B(5, 0));
	EXPECT_EQ(Block(board, 1).location(8), B(1, 8));

	static_assert(Row(board, 0).location(0) == L(0, 0));
	static_assert(Row(board, 1).location(0) == L(1, 0));
	static_assert(Row(board, 3).location(6) == L(3, 6));
	static_assert(Col(board, 0).location(0) == L(0, 0));
	static_assert(Col(board, 1).location(0) == L(0, 1));
	static_assert(Col(board, 3).location(6) == L(6, 3));
	static_assert(Block(board, 0).location(0) == L(0, 0));
	static_assert(Block(board, 5).location(0) == B(5, 0));
	static_assert(Block(board, 1).location(8) == B(1, 8));
}

TEST(Board_Section, unchecked_access)
{
	using Row         = Row<int, 2>;
	using Col         = Col<int, 2>;
	using Block       = Block<int, 2>;
	using const_Row   = const_Row<int, 2>;
	using const_Col   = const_Col<int, 2>;
	using const_Block = const_Block<int, 2>;

	constexpr Board<int, 2> cboard = std::array<int, 16>{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const_Row crow(cboard, 0);
	Board<int, 2> board{};
	Row row(board, 0);

	static_assert(noexcept(row[3]));
	static_assert(noexcept(crow[1]));
	// return type
	static_assert(std::is_same_v<int&, decltype(row[2])>);
	static_assert(std::is_same_v<int const&, decltype(crow[2])>);

	// Read element
	EXPECT_EQ(crow[0], 9);
	EXPECT_EQ(crow[1], 1);
	EXPECT_EQ(crow[2], 2);
	static_assert(const_Row(cboard, 0)[0] == 9);
	static_assert(const_Row(cboard, 1)[1] == 5);
	static_assert(const_Col(cboard, 0)[0] == 9);
	static_assert(const_Col(cboard, 3)[3] == 15);
	static_assert(const_Block(cboard, 1)[3] == 7);
	static_assert(const_Block(cboard, 2)[0] == 8);

	// Edit element
	EXPECT_EQ(row[2], 0);
	row[2] = 8;
	EXPECT_EQ(row[2], 8);
	EXPECT_EQ((Col(board, 2)[0]), 8);
	EXPECT_EQ((Block(board, 1)[0]), 8);
	Col(board, 1)[3] = 6;
	EXPECT_EQ((Row(board, 3)[1]), 6);
	EXPECT_EQ((Block(board, 2)[3]), 6);
}

TEST(Board_Section, front_back)
{
	using Row         = Row<int, 2>;
	using Col         = Col<int, 2>;
	using Block       = Block<int, 2>;
	using const_Row   = const_Row<int, 2>;
	using const_Col   = const_Col<int, 2>;
	using const_Block = const_Block<int, 2>;

	constexpr Board<int, 2> cboard = std::array<int, 16>{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const_Row crow(cboard, 0);
	Board<int, 2> board{};
	Row row(board, 1);

	static_assert(noexcept(row.front()));
	static_assert(noexcept(crow.front()));
	static_assert(noexcept(row.back()));
	static_assert(noexcept(crow.back()));

	static_assert(std::is_same_v<int&, decltype(row.front())>);
	static_assert(std::is_same_v<int const&, decltype(crow.front())>);
	static_assert(std::is_same_v<int&, decltype(row.back())>);
	static_assert(std::is_same_v<int const&, decltype(crow.back())>);

	EXPECT_EQ(crow.front(), crow[0]);
	EXPECT_EQ(crow.back(), crow[3]);
	row.front() = 87;
	EXPECT_EQ(board.at(Location<2>(1, 0)), 87);
	EXPECT_EQ(row.front(), 87);
	EXPECT_EQ(row[0], 87);
	row.back() = 52;
	EXPECT_EQ(board.at(Location<2>(1, 3)), 52);
	EXPECT_EQ(row.back(), 52);
	EXPECT_EQ(row[3], 52);
	static_assert(const_Row(cboard, 0).front() == const_Row(cboard, 0)[0]);
	static_assert(const_Row(cboard, 0).back() == const_Row(cboard, 0)[3]);
	static_assert(const_Row(cboard, 3).front() == const_Row(cboard, 3)[0]);
	static_assert(const_Row(cboard, 3).back() == const_Row(cboard, 3)[3]);

	const_Col ccol(cboard, 0);
	Col col(board, 1);
	EXPECT_EQ(ccol.front(), ccol[0]);
	EXPECT_EQ(ccol.back(), ccol[3]);
	col.front() = 21;
	EXPECT_EQ(board.at(Location<2>(0, 1)), 21);
	EXPECT_EQ(col.front(), 21);
	EXPECT_EQ(col[0], 21);
	EXPECT_NE(col.back(), 21);
	col.back() = 37;
	EXPECT_EQ(board.at(Location<2>(3, 1)), 37);
	EXPECT_EQ(col.back(), 37);
	EXPECT_EQ(col[3], 37);
	static_assert(const_Col(cboard, 0).front() == const_Col(cboard, 0)[0]);
	static_assert(const_Col(cboard, 0).back() == const_Col(cboard, 0)[3]);
	static_assert(const_Col(cboard, 3).front() == const_Col(cboard, 3)[0]);
	static_assert(const_Col(cboard, 3).back() == const_Col(cboard, 3)[3]);

	const_Block cblock(cboard, 0);
	Block block(board, 3);
	EXPECT_EQ(cblock.front(), cblock[0]);
	EXPECT_EQ(cblock.back(), cblock[3]);
	block.front() = 14;
	EXPECT_EQ(board.at(Location_Block<2>(3, 0)), 14);
	EXPECT_EQ(block.front(), 14);
	EXPECT_EQ(block[0], 14);
	EXPECT_NE(block.back(), 14);
	block.back() = 99;
	EXPECT_EQ(board.at(Location_Block<2>(3, 3)), 99);
	EXPECT_EQ(block.back(), 99);
	EXPECT_EQ(block[3], 99);
	static_assert(const_Block(cboard, 0).front() == const_Block(cboard, 0)[0]);
	static_assert(const_Block(cboard, 0).back() == const_Block(cboard, 0)[3]);
	static_assert(const_Block(cboard, 3).front() == const_Block(cboard, 3)[0]);
	static_assert(const_Block(cboard, 3).back() == const_Block(cboard, 3)[3]);
}

TEST(Board_Section, checked_access)
{
	const Board<int, 2> cboard{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	constexpr Board<int, 2> cexprboard = std::array<int, 16>{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const_Row<int, 2> crow(cboard, 0);
	Board<int, 2> board{};
	Row<int, 2> row(board, 0);

	static_assert(not noexcept(row.at(3)));
	static_assert(not noexcept(crow.at(1)));
	// return type
	static_assert(std::is_same_v<int&, decltype(row.at(2))>);
	static_assert(std::is_same_v<int const&, decltype(crow.at(2))>);

	// Read element
	EXPECT_EQ(crow.at(0), 9);
	EXPECT_EQ(crow.at(1), 1);
	EXPECT_EQ(crow.at(2), 2);
	static_assert(const_Row<int, 2>(cexprboard, 0).at(0) == 9);
	static_assert(const_Row<int, 2>(cexprboard, 0).at(1) == 1);
	static_assert(const_Row<int, 2>(cexprboard, 0).at(2) == 2);
	static_assert(const_Col<int, 2>(cexprboard, 0).at(2) == 8);
	static_assert(const_Block<int, 2>(cexprboard, 0).at(2) == 4);

	// Edit element
	EXPECT_EQ(row.at(2), 0);
	row.at(2) = 8;
	EXPECT_EQ(row.at(2), 8);
	EXPECT_EQ((Col<int, 2>(board, 2).at(0)), 8);
	EXPECT_EQ((Block<int, 2>(board, 1).at(0)), 8);
	Col<int, 2>(board, 1).at(3) = 6;
	EXPECT_EQ((Row<int, 2>(board, 3).at(1)), 6);
	EXPECT_EQ((Block<int, 2>(board, 2).at(3)), 6);
	Block<int, 2>(board, 3).at(1) = 31;
	EXPECT_EQ((Row<int, 2>(board, 2).at(3)), 31);
	EXPECT_EQ((Col<int, 2>(board, 3).at(2)), 31);
}

} // namespace SudokuTests::Members

namespace SudokuTests::Iterators
{
using Board       = ::Sudoku::Board<int, 3>;
using Row         = ::Sudoku::Board_Section::Row<int, 3>;
using Col         = ::Sudoku::Board_Section::Col<int, 3>;
using Block       = ::Sudoku::Board_Section::Block<int, 3>;
using const_Row   = ::Sudoku::Board_Section::const_Row<int, 3>;
using const_Col   = ::Sudoku::Board_Section::const_Col<int, 3>;
using const_Block = ::Sudoku::Board_Section::const_Block<int, 3>;

static_assert(noexcept(Row(std::declval<Board&>(), 0).begin()));
static_assert(noexcept(Row(std::declval<Board&>(), 0).cbegin()));
static_assert(noexcept(Row(std::declval<Board&>(), 0).rbegin()));
static_assert(noexcept(Row(std::declval<Board&>(), 0).crbegin()));
static_assert(noexcept(Col(std::declval<Board&>(), 0).begin()));
static_assert(noexcept(Block(std::declval<Board&>(), 0).begin()));
static_assert(noexcept(const_Row(std::declval<Board&>(), 0).begin()));
static_assert(noexcept(const_Row(std::declval<Board&>(), 0).cbegin()));
static_assert(noexcept(const_Row(std::declval<Board&>(), 0).rbegin()));
static_assert(noexcept(const_Row(std::declval<Board&>(), 0).crbegin()));
static_assert(noexcept(Row(std::declval<Board&>(), 0).end()));
} // namespace SudokuTests::Iterators
