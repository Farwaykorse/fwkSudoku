// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/Board_Section_iterator.cpp                    ----====//
//
// Unit tests for the template class Sudoku::Board_Section
//====--------------------------------------------------------------------====//
// Implemented with GoogleTest
//
// Notes:
// gTest is limited for use with multiple template parameters.
// These expressions need to be implemented between extra parentheses.
// gTest tries to print iterators if they use inheritance,
//   if used in *_EQ/NE etc.
//   use an explicit test like EXPECT_TRUE(.. == ..).
//
//====--------------------------------------------------------------------====//
#include <Sudoku/Board.h>
#include <Sudoku/Board_Section.h>          // Class under test
#include <Sudoku/Board_Section_iterator.h> // Class under test
#include <Sudoku/Location.h>
#include <Sudoku/Options.h>
#include <Sudoku/traits.h>

#include <type_traits>

#include <gtest/gtest.h>

#include <cstdint>


namespace SudokuTests::Section_Iterator
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Options;
using ::Sudoku::Value;
using ::Sudoku::Board_iterator;
using ::Sudoku::const_Board_iterator;
using ::Sudoku::Board_Section::Row;
using ::Sudoku::Board_Section::Col;
using ::Sudoku::Board_Section::Block;
using ::Sudoku::Board_Section::const_Row;
using ::Sudoku::Board_Section::Section_iterator;
using ::Sudoku::Board_Section::Row_iterator;
using ::Sudoku::Board_Section::Col_iterator;
using ::Sudoku::Board_Section::Block_iterator;
using ::Sudoku::Board_Section::const_Row_iterator;
using ::Sudoku::Board_Section::const_Col_iterator;
using ::Sudoku::Board_Section::const_Block_iterator;
using ::Sudoku::Board_Section::reverse_Row_iterator;
using ::Sudoku::Board_Section::reverse_Col_iterator;
using ::Sudoku::Board_Section::reverse_Block_iterator;
using ::Sudoku::Board_Section::const_reverse_Row_iterator;
using ::Sudoku::Board_Section::const_reverse_Col_iterator;
using ::Sudoku::Board_Section::const_reverse_Block_iterator;
} // namespace SudokuTests::Section_Iterator

namespace SudokuTests::Section_Iterator::Type
{
using dataT             = int;
constexpr int size      = 3;
using Board             = ::Sudoku::Board<dataT, size>;
using L                 = ::Sudoku::Location<size>;
using Location_Block    = ::Sudoku::Location_Block<size>;
constexpr auto section  = ::Sudoku::Board_Section::Section::row;
constexpr bool is_const = false;
constexpr bool is_rev   = false;

using typeT = Section_iterator<dataT, size, section, is_const, is_rev>;

using Row_Itr             = Row_iterator<dataT, size>;
using Col_Itr             = Col_iterator<dataT, size>;
using Block_Itr           = Block_iterator<dataT, size>;
using const_Row_Itr       = const_Row_iterator<dataT, size>;
using const_Col_Itr       = const_Col_iterator<dataT, size>;
using const_Block_Itr     = const_Block_iterator<dataT, size>;
using rev_Row_Itr         = reverse_Row_iterator<dataT, size>;
using rev_Col_Itr         = reverse_Col_iterator<dataT, size>;
using rev_Block_Itr       = reverse_Block_iterator<dataT, size>;
using const_rev_Row_Itr   = const_reverse_Row_iterator<dataT, size>;
using const_rev_Col_Itr   = const_reverse_Col_iterator<dataT, size>;
using const_rev_Block_Itr = const_reverse_Block_iterator<dataT, size>;

namespace type_properties
{
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
	not(defined(__clang__) && __clang_major__ < 6) &&                          \
	not(defined(__APPLE__) && defined(__clang__) && __clang_major__ < 10)
	static_assert(std::has_unique_object_representations_v<typeT>);
#endif
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
} // namespace type_properties

namespace constructors
{
	// default constructor: typeT()
	static_assert(std::is_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<Row_Itr>);
	static_assert(std::is_nothrow_default_constructible_v<Col_Itr>);
	static_assert(std::is_nothrow_default_constructible_v<Block_Itr>);
	static_assert(std::is_nothrow_default_constructible_v<const_Row_Itr>);
	static_assert(std::is_nothrow_default_constructible_v<const_Col_Itr>);
	static_assert(std::is_nothrow_default_constructible_v<const_Block_Itr>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);
	TEST(SectionItr, constrDefault)
	{
		EXPECT_EQ((Row_iterator<dataT, 3>().location()), L(0, 0));
		EXPECT_EQ((Col_iterator<dataT, 3>().location()), L(0, 0));
		EXPECT_EQ((Block_iterator<dataT, 3>().location()), L(0, 0));
		EXPECT_EQ((reverse_Row_iterator<dataT, 3>().location()), L(0, 8));
		EXPECT_EQ((reverse_Col_iterator<dataT, 3>().location()), L(8, 0));
		EXPECT_EQ((reverse_Block_iterator<dataT, 3>().location()), L(2, 2));
	}
	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>);
	static_assert(std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(std::is_trivially_copy_constructible_v<typeT>); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(std::is_trivially_move_constructible_v<typeT>);

	//====----------------------------------------------------------------====//
	// From Board*:
	static_assert(not std::is_constructible_v<Row_Itr, Board*>);
	static_assert(not std::is_constructible_v<Col_Itr, Board*>);
	static_assert(not std::is_constructible_v<Block_Itr, Board*>);
	static_assert(not std::is_constructible_v<const_Row_Itr, Board*>);
	static_assert(not std::is_constructible_v<const_Col_Itr, Board*>);
	static_assert(not std::is_constructible_v<const_Block_Itr, Board*>);
	// From Board* and section id:
	static_assert(not std::is_constructible_v<Row_Itr, Board*, int>);
	static_assert(not std::is_constructible_v<Col_Itr, Board*, int>);
	static_assert(not std::is_constructible_v<Block_Itr, Board*, int>);
	static_assert(not std::is_constructible_v<const_Row_Itr, Board*, int>);
	static_assert(not std::is_constructible_v<const_Col_Itr, Board*, int>);
	static_assert(not std::is_constructible_v<const_Block_Itr, Board*, int>);
	// From Board&:
	static_assert(not std::is_constructible_v<Row_Itr, Board&>);
	// From Board*, section id and element id:
	static_assert(std::is_constructible_v<Row_Itr, Board*, int, int>);
	static_assert(std::is_constructible_v<Col_Itr, Board*, int, int>);
	static_assert(std::is_constructible_v<Block_Itr, Board*, int, int>);
	static_assert(std::is_constructible_v<const_Row_Itr, Board*, int, int>);
	static_assert(std::is_constructible_v<const_Col_Itr, Board*, int, int>);
	static_assert(std::is_constructible_v<const_Block_Itr, Board*, int, int>);
	static_assert(
		std::is_constructible_v<const_Row_Itr, Board const*, int, int>);
	static_assert(
		std::is_constructible_v<const_Col_Itr, Board const*, int, int>);
	static_assert(
		std::is_constructible_v<const_Block_Itr, Board const*, int, int>);
	static_assert(std::is_nothrow_constructible_v<Row_Itr, Board*, int, int>);
	static_assert(std::is_nothrow_constructible_v<Col_Itr, Board*, int, int>);
	static_assert(std::is_nothrow_constructible_v<Block_Itr, Board*, int, int>);
	static_assert(
		std::is_nothrow_constructible_v<const_Row_Itr, Board*, int, int>);
	static_assert(
		std::is_nothrow_constructible_v<const_Col_Itr, Board*, int, int>);
	static_assert(
		std::is_nothrow_constructible_v<const_Block_Itr, Board*, int, int>);
	static_assert(
		std::is_nothrow_constructible_v<const_Row_Itr, Board const*, int, int>);
	TEST(SectionItr, constrIdElem)
	{
		Board board{};
		EXPECT_EQ((Row_iterator<dataT, 3>(&board, 0, 0).location()), L(0, 0));
		EXPECT_EQ((Row_iterator<dataT, 3>(&board, 3, 5).location()), L(3, 5));
		EXPECT_EQ((Row_iterator<dataT, 3>(&board, 0, 8).location()), L(0, 8));
		EXPECT_EQ((Col_iterator<dataT, 3>(&board, 0, 0).location()), L(0, 0));
		EXPECT_EQ((Col_iterator<dataT, 3>(&board, 1, 6).location()), L(6, 1));
		EXPECT_EQ((Col_iterator<dataT, 3>(&board, 8, 5).location()), L(5, 8));
		EXPECT_EQ((Block_iterator<dataT, 3>(&board, 0, 0).location()), L(0, 0));
		EXPECT_EQ((Block_iterator<dataT, 3>(&board, 1, 7).location()), L(2, 4));
		EXPECT_EQ((Block_iterator<dataT, 3>(&board, 8, 8).location()), L(8, 8));
	}
	// - Blocked implicit conversions:
	static_assert(not std::is_constructible_v<Row_Itr, Board*, bool, bool>);
	static_assert(not std::is_constructible_v<Row_Itr, Board*, int, bool>);
	static_assert(not std::is_constructible_v<Row_Itr, Board*, bool, int>);
	static_assert(not std::is_constructible_v<Row_Itr, Board*, int, float>);
	static_assert(not std::is_constructible_v<Row_Itr, Board*, float, int>);
	// - Re-enabled implicit conversions for [elem]:
	static_assert(std::is_constructible_v<Row_Itr, Board*, char, char>);
	static_assert(
		std::is_constructible_v<Row_Itr, Board*, std::int64_t, std::int32_t>);
	static_assert(std::is_constructible_v<Row_Itr, Board*, size_t, size_t>);

	// Implicit conversion to const_*
	static_assert(std::is_convertible_v<Row_Itr, const_Row_Itr>);
	static_assert(std::is_convertible_v<Col_Itr, const_Col_Itr>);
	static_assert(std::is_convertible_v<Block_Itr, const_Block_Itr>);
	// Construct from non-const
	static_assert(std::is_constructible_v<const_Row_Itr, Row_Itr>);
	static_assert(std::is_constructible_v<const_Col_Itr, Col_Itr>);
	static_assert(std::is_constructible_v<const_Block_Itr, Block_Itr>);
	static_assert(std::is_nothrow_constructible_v<const_Row_Itr, Row_Itr>);
	static_assert(std::is_nothrow_constructible_v<const_Col_Itr, Col_Itr>);
	static_assert(std::is_nothrow_constructible_v<const_Block_Itr, Block_Itr>);

	// explicit conversion from other iterators
	static_assert(not std::is_constructible_v<typeT, Board::iterator>);
	static_assert(not std::is_constructible_v<typeT, Board::const_iterator>);
	static_assert(not std::is_constructible_v<typeT, Board::reverse_iterator>);
	static_assert(
		not std::is_constructible_v<typeT, Board::const_reverse_iterator>);
	// other types
	static_assert(not std::is_constructible_v<typeT, L>);

	// Explicit conversion from typeT to Location:
	static_assert(not std::is_convertible_v<Row_Itr, L>);
	static_assert(not std::is_convertible_v<Col_Itr, L>);
	static_assert(not std::is_convertible_v<Block_Itr, L>);
	static_assert(std::is_constructible_v<L, typeT>);
	static_assert(std::is_nothrow_constructible_v<L, typeT>);
	TEST(SectionItr, Location)
	{
		Board board{};
		const Board cboard{};
		EXPECT_EQ(L(Row_iterator<dataT, 3>(&board, 0, 0)), L(0, 0));
		EXPECT_EQ(L(Row_iterator<dataT, 3>(&board, 3, 5)), L(3, 5));
		EXPECT_EQ(L(Row_iterator<dataT, 3>(&board, 0, 8)), L(0, 8));
		EXPECT_EQ(L(Col_iterator<dataT, 3>(&board, 0, 0)), L(0, 0));
		EXPECT_EQ(L(Col_iterator<dataT, 3>(&board, 1, 6)), L(6, 1));
		EXPECT_EQ(L(Col_iterator<dataT, 3>(&board, 8, 5)), L(5, 8));
		EXPECT_EQ(L(Block_iterator<dataT, 3>(&board, 0, 0)), L(0, 0));
		EXPECT_EQ(L(Block_iterator<dataT, 3>(&board, 1, 7)), L(2, 4));
		EXPECT_EQ(L(Block_iterator<dataT, 3>(&board, 8, 8)), L(8, 8));
		EXPECT_EQ(L(const_Row_iterator<dataT, 3>(&cboard, 3, 5)), L(3, 5));
		EXPECT_EQ(L(const_Col_iterator<dataT, 3>(&cboard, 1, 6)), L(6, 1));
		EXPECT_EQ(L(const_Block_iterator<dataT, 3>(&cboard, 1, 7)), L(2, 4));
	}
	// To other types:
	static_assert(not std::is_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<int, typeT>);
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
	static_assert(std::is_swappable_v<typeT>);
	static_assert(std::is_nothrow_swappable_v<typeT>);

	static_assert(not std::is_swappable_with_v<typeT, int>);
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>);
	static_assert(not std::is_swappable_with_v<typeT, size_t>);
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);
#endif // __ICL
} // namespace swapping

namespace assignment
{
	static_assert(std::is_copy_assignable_v<typeT>);
	static_assert(std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(std::is_trivially_copy_assignable_v<typeT>);

	static_assert(std::is_move_assignable_v<typeT>);
	static_assert(std::is_nothrow_move_assignable_v<typeT>);
	static_assert(std::is_trivially_move_assignable_v<typeT>);

	static_assert(not std::is_assignable_v<L, typeT>);
} // namespace assignment

} // namespace SudokuTests::Section_Iterator::Type

namespace SudokuTests::Section_Iterator::Members
{
using ::Sudoku::traits::is_input;
using ::Sudoku::traits::is_forward;
using ::Sudoku::traits::is_bidir;
using ::Sudoku::traits::is_random;

//====--------------------------------------------------------------------====//
struct test_elements
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
};

//====--------------------------------------------------------------------====//
TEST(SectionItr, beginEnd)
{ // All iterator categories
	test_elements TE{};
	auto& board        = TE.A;
	auto const& cboard = TE.cA;
	Row<int, 2> row(board, 0);
	const const_Row<int, 2> crow(cboard, 0);

	static_assert(noexcept(row.begin()));
	static_assert(noexcept(row.end()));
	static_assert(noexcept(row.cbegin()));
	static_assert(noexcept(row.cend()));
	static_assert(noexcept(row.rbegin()));
	static_assert(noexcept(row.rend()));
	static_assert(noexcept(row.crbegin()));
	static_assert(noexcept(row.crend()));
	static_assert(noexcept(crow.begin()));
	static_assert(noexcept(crow.end()));
	static_assert(noexcept(crow.rbegin()));
	static_assert(noexcept(crow.rend()));

	// return types:
	static_assert(std::is_same_v<decltype(row.begin()), decltype(row.end())>);
	static_assert(std::is_same_v<decltype(row.cbegin()), decltype(row.cend())>);
	static_assert(std::is_same_v<decltype(row.rbegin()), decltype(row.rend())>);
	static_assert(
		std::is_same_v<decltype(row.crbegin()), decltype(row.crend())>);
	// const container -> always const
	static_assert(
		std::is_same_v<decltype(crow.begin()), decltype(crow.cbegin())>);
	static_assert(
		std::is_same_v<decltype(crow.rbegin()), decltype(crow.crbegin())>);
	// no mixing const
	static_assert(
		not std::is_same_v<decltype(row.begin()), decltype(row.cbegin())>);
	static_assert(
		not std::is_same_v<decltype(row.begin()), decltype(row.rbegin())>);
	{ // but declaring makes const
		const Row<int, 2> x(board, 0);
		const const_Row<int, 2> cx(cboard, 0);
		static_assert(
			not std::is_same_v<decltype(x.begin()), decltype(row.end())>);
		static_assert(
			std::is_same_v<decltype(cx.begin()), decltype(crow.end())>);
		static_assert(
			std::is_same_v<decltype(x.begin()), decltype(crow.end())>);
		static_assert(std::is_same_v<decltype(x.begin()), decltype(cx.end())>);
		static_assert(std::is_same_v<decltype(x.begin()), decltype(cx.end())>);
	}

	// TODO not mixing sections:
	Col<int, 2> col(board, 0);
	::Sudoku::Board_Section::const_Col<int, 2> const ccol(cboard, 0);

	// TODO not mixing section ids:

	// Default constructor
	const Row_iterator<int, 2> D1;
	EXPECT_TRUE((D1 == Row_iterator<int, 2>()));

	// Construct
	const Row_iterator<int, 2> I1(&board, 0, 0);
	EXPECT_TRUE(I1 == row.begin());
	const reverse_Row_iterator<int, 2> rI1{&board, 0, 3};
	EXPECT_TRUE(rI1 == row.rbegin());
	{
		const const_Row_iterator<int, 2> cI1{&board, 0, 0};
		EXPECT_TRUE(cI1 == row.cbegin());
		const const_reverse_Row_iterator<int, 2> crI1{&board, 0, 3};
		EXPECT_TRUE(rI1 == row.rbegin());
	}
	const const_Row_iterator<int, 2> cI1{&cboard, 0, 0};
	EXPECT_TRUE(cI1 == crow.cbegin());
	const const_reverse_Row_iterator<int, 2> crI1{&cboard, 0, 3};
	EXPECT_TRUE(crI1 == crow.crbegin());
#ifdef NDEBUG
	EXPECT_TRUE(cI1 != row.cbegin());
	EXPECT_TRUE(crI1 != row.crbegin());
#endif // NDEBUG
	{  // Copy-construct
		const Row_iterator<int, 2> LI{I1};
		EXPECT_TRUE(LI == row.begin());
		const const_Row_iterator<int, 2> cLI{cI1};
		EXPECT_TRUE(cLI == crow.cbegin());
		const reverse_Row_iterator<int, 2> rLI{rI1};
		EXPECT_TRUE(rLI == row.rbegin());
		const const_reverse_Row_iterator<int, 2> crLI{crI1};
		EXPECT_TRUE(crLI == crow.crbegin());
	}
	{ // conversion to const
		const const_Row_iterator<int, 2> cLI{I1};
		EXPECT_TRUE(cLI == row.cbegin());
		const const_reverse_Row_iterator<int, 2> crLI{rI1};
		EXPECT_TRUE(crLI == row.crbegin());
	}
	{
		// assignment
		[[maybe_unused]] Row_iterator<int, 2> LI{};
		LI = I1;
		[[maybe_unused]] const_Row_iterator<int, 2> cLI{};
		cLI = cI1;
		cLI = I1;
		[[maybe_unused]] reverse_Row_iterator<int, 2> rLI{};
		rLI = rI1;
		[[maybe_unused]] const_reverse_Row_iterator<int, 2> crLI{};
		crLI = rI1;
		crLI = crI1;
	}
}

TEST(SectionItr, LocationConversion)
{
	using L = Location<2>;

	test_elements TE{};
	auto& A        = TE.A;
	auto const& cA = TE.cA;

	static_assert(noexcept(Row<int, 2>(A, 0).begin().location()));
	static_assert(noexcept(Row<int, 2>(A, 0).cbegin().location()));
	static_assert(noexcept(Row<int, 2>(A, 0).rbegin().location()));
	static_assert(noexcept(Row<int, 2>(A, 0).crbegin().location()));
	static_assert(noexcept(Location<2>{Row<int, 2>(A, 0).begin()}));
	static_assert(noexcept(Location<2>{Row<int, 2>(A, 0).cbegin()}));
	static_assert(noexcept(Location<2>{Row<int, 2>(A, 0).rbegin()}));
	static_assert(noexcept(Location<2>{Row<int, 2>(A, 0).crbegin()}));

	// Return Location
	static_assert(Row_iterator<int, 2>().location() == Location<2>{0});
	static_assert(reverse_Row_iterator<int, 2>().location() == L{3});
	// Conversion to Location
	static_assert(Location<2>{Row_iterator<int, 2>()} == Location<2>{0});

	// construct Location from iterator
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

TEST(SectionItr, operatorIndirection)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 3);
	const const_Row<int, 2> cA(TE.cA, 3);

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		static_assert(noexcept(*A.begin()));
		static_assert(noexcept(*A.cbegin()));
		static_assert(noexcept(*A.rbegin()));
		static_assert(noexcept(*A.crbegin()));
		static_assert(noexcept(*cA.begin()));

		// result type
		static_assert(std::is_same_v<int&, decltype(*A.begin())>);
		static_assert(std::is_same_v<int const&, decltype(*A.cbegin())>);
		static_assert(std::is_same_v<int&, decltype(*A.rbegin())>);
		static_assert(std::is_same_v<int const&, decltype(*A.crbegin())>);

		ASSERT_EQ(A.id(), 3);
		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 12);
		ASSERT_EQ(A.back(), 15);

		EXPECT_EQ(*A.begin(), 12);
		EXPECT_EQ(*A.cbegin(), 12);
		EXPECT_EQ(*A.rbegin(), 15);
		EXPECT_EQ(*A.crbegin(), 15);
		EXPECT_EQ(*cA.begin(), 12);
		EXPECT_EQ(*cA.rbegin(), 15);
		const auto itr = A.begin();
		EXPECT_EQ(*itr, 12);
		// Input iterator: No guarantee it can be dereferenced more than once
		if constexpr (is_forward<decltype(itr)>)
		{
			EXPECT_EQ(*itr, 12);
		}
		else
			ADD_FAILURE();
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorIndirection)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 3);

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.end(), "");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.cend(), "");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.rend(), "");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *A.crend(), "");
		{ // Dereferencing is an error when de container is deleted
			const Board_iterator<int, 3> X;
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *X, "");
			const const_Board_iterator<int, 3> Y;
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *Y, "");
			const decltype(A.begin()) I;
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *I, "");
			const decltype(A.cbegin()) cI;
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *cI, "");
			const decltype(A.rbegin()) rI;
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *rI, "");
			const decltype(A.crbegin()) crI;
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = *crI, "");
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorPreIncrement)
{ // All iterator categories
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

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
		EXPECT_EQ(*(++A.rbegin()), 2);
	}
	else
		ADD_FAILURE();
	if constexpr (is_input<decltype(A.crbegin())>)
	{
		static_assert(
			std::is_same_v<decltype(A.crbegin())&, decltype(++A.crbegin())>);
		EXPECT_EQ(*(++A.crbegin()), 2);
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorPreIncrement)
{ // All iterator categories
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	EXPECT_DEBUG_DEATH(++A.end(), "< elem_size");
	EXPECT_DEBUG_DEATH(++A.cend(), "< elem_size");
	EXPECT_DEBUG_DEATH(++A.rend(), ">= 0");
	EXPECT_DEBUG_DEATH(++A.crend(), ">= 0");
	{ // Advancing is an error when de container is deleted
		EXPECT_DEBUG_DEATH(++(Board_iterator<int, 3>()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.begin())()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.cbegin())()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.rbegin())()), "nullptr");
		EXPECT_DEBUG_DEATH(++(decltype(A.crbegin())()), "nullptr");
	}
}

TEST(SectionItr, operatorPostIncrement)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

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
		EXPECT_NO_THROW([[maybe_unused]] auto x = A.begin()++);
		EXPECT_NO_THROW([[maybe_unused]] auto x = A.cbegin()++);
		EXPECT_NO_THROW([[maybe_unused]] auto x = A.rbegin()++);
		EXPECT_NO_THROW([[maybe_unused]] auto x = A.crbegin()++);

		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 3);
		// *i++
		EXPECT_TRUE(A.begin()++ == A.begin());
		EXPECT_TRUE(A.rbegin()++ == A.rbegin());
		EXPECT_TRUE(A.cbegin()++ == A.cbegin());
		EXPECT_TRUE(A.crbegin()++ == A.crbegin());
		EXPECT_EQ(*A.begin()++, 9);
		EXPECT_EQ(*A.cbegin()++, 9);
		EXPECT_EQ(*A.rbegin()++, 3);
		EXPECT_EQ(*A.crbegin()++, 3);
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorPostIncrement)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto x = A.end()++, "< elem_size");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto x = A.cend()++, "< elem_size");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto x = A.rend()++, ">= 0");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto x = A.crend()++, ">= 0");
		{ // Advancing is an error when de container is deleted
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto x = (Board_iterator<int, 3>())++,
				"nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto x = (decltype(A.begin())())++, "nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto x = (decltype(A.cbegin())())++,
				"nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto x = (decltype(A.rbegin())())++,
				"nullptr");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto x = (decltype(A.crbegin())())++,
				"nullptr");
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorEqual)
{ // All iterator categories
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

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

	[[maybe_unused]] bool U{};
	EXPECT_NO_THROW(U = (A.begin() == A.end()));
	EXPECT_NO_THROW(U = (A.end() == A.end()));
	EXPECT_NO_THROW(U = (A.cbegin() == A.cend()));
	EXPECT_NO_THROW(U = (A.rbegin() == A.rend()));
	EXPECT_NO_THROW(U = (A.crbegin() == A.crend()));

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
		const Row_iterator<int, 2> Default;
		EXPECT_TRUE((Default == Row_iterator<int, 2>{}));
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorEqual)
{ // All iterator categories
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	Board<int, 2> board{TE.A};
	Row<int, 2> B(board, 0);
	[[maybe_unused]] bool U{};
	EXPECT_DEBUG_DEATH(U = A.begin() == B.begin(), "is_same_Section");
#ifdef NDEBUG
	EXPECT_FALSE(A.begin() == B.begin());
#endif // !_DEBUG

	// default constructor
	if constexpr (
		is_forward<decltype(A.begin())> && is_forward<decltype(A.cbegin())> &&
		is_forward<decltype(A.rbegin())> && is_forward<decltype(A.crbegin())>)
	{
		const Row_iterator<int, 2> Default;
		EXPECT_DEBUG_DEATH(U = Default == A.begin(), "is_same_Section");
		EXPECT_DEBUG_DEATH(U = Default == A.end(), "is_same_Section");
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorNotEqual)
{ // InputIterator
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

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
			using itr = Board_iterator<int, 3>;
			using L   = Location<3>;
			static_assert(not((itr() = L{0}) != (itr() = L{0})));
			static_assert((itr() = L{3}) != itr());
		}
		[[maybe_unused]] bool U{};
		EXPECT_NO_THROW(U = (A.begin() != A.end()));
		EXPECT_NO_THROW(U = (A.cbegin() != A.cend()));
		EXPECT_NO_THROW(U = (A.rbegin() != A.rend()));
		EXPECT_NO_THROW(U = (A.crbegin() != A.crend()));

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

TEST(SectionItrDeathTest, operatorNotEqual)
{ // InputIterator
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.cbegin())> &&
		is_input<decltype(A.rbegin())> && is_input<decltype(A.crbegin())>)
	{
		[[maybe_unused]] bool U{};
		Board<int, 2> board = TE.A;
		Row<int, 2> B(board, 0);
		Board<int, 2> board_c{};
		Row<int, 2> C(board_c, 0);
		EXPECT_DEBUG_DEATH(U = A.begin() != B.begin(), "is_same_Section");
		EXPECT_DEBUG_DEATH(U = A.begin() != C.begin(), "is_same_Section");
#ifdef NDEBUG
		EXPECT_TRUE(A.begin() != B.begin());
		EXPECT_TRUE(A.begin() != B.end());
		EXPECT_TRUE(A.begin() != C.begin());
#endif // NDEBUG
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorMemberOfPointerAccess)
{ // input iterator category
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	Board<Options<4>, 2> board_Opt{};
	Row<Options<4>, 2> Opt(board_Opt, 0);

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
		static_assert(not noexcept(Opt.begin()->remove_option(Value{1})));
		// return type
		static_assert(
			std::is_same_v<Options<4>*, decltype(Opt.begin().operator->())>);
		static_assert(std::is_same_v<int*, decltype(A.begin().operator->())>);

		EXPECT_NO_THROW(
			[[maybe_unused]] bool U =
				(Opt.begin()->size() == (*Opt.begin()).size())); // equivalent
		EXPECT_EQ((Opt.begin()->size()), (*Opt.begin()).size()); // equivalent
		EXPECT_EQ(*(A.begin().operator->()), A[0]);
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, InputIterator)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	int total{};
	if constexpr (
		is_input<decltype(A.begin())> && is_input<decltype(A.rbegin())>)
	{
		EXPECT_NO_THROW(total = std::accumulate(A.begin(), A.end(), 0));
		EXPECT_EQ(total, 15);
		total = 0;
		EXPECT_NO_THROW(total = std::accumulate(A.rbegin(), A.rend(), 0));
		EXPECT_EQ(total, 15);
		total = 0;
	}
	else
		ADD_FAILURE();
	if constexpr (
		is_input<decltype(A.cbegin())> && is_input<decltype(A.crbegin())>)
	{
		EXPECT_NO_THROW(total = std::accumulate(A.cbegin(), A.cend(), 0));
		EXPECT_EQ(total, 15);
		total = 0;
		EXPECT_NO_THROW(total = std::accumulate(cA.rbegin(), cA.rend(), 0));
		EXPECT_EQ(total, 15);
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, OutputIterator)
{
	Board<int, 2> board{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	Row<int, 2> tmp(board, 1);
	EXPECT_EQ(tmp[0], 0);

	// *r = o (return-value not used)
	auto itrA = tmp.begin();
	// pre-condition: iterator is dereferenceable
	static_assert(noexcept(*itrA));
	EXPECT_EQ(*itrA, 0);
	// operation:
	static_assert(noexcept(*tmp.begin() = 1));
	static_assert(std::is_assignable_v<decltype(*tmp.begin()), int>);
	static_assert(std::is_trivially_assignable_v<decltype(*tmp.begin()), int>);
	static_assert(std::is_nothrow_assignable_v<decltype(*tmp.begin()), int>);
	*itrA = 2;
	EXPECT_EQ(tmp[0], 2);
	// post-condition: iterator is incrementable
	static_assert(noexcept((*tmp.begin() = 1)++));
	itrA++;
	EXPECT_TRUE(itrA == ++tmp.begin());
	if constexpr (is_forward<decltype(tmp.begin())>)
	{
		*itrA = 5;
		EXPECT_EQ(tmp[1], 5);
	}

	[[maybe_unused]] int U{};
	EXPECT_NO_THROW(U = *tmp.begin()); // pre-condition
	static_assert(noexcept(*tmp.begin() = 5));
	EXPECT_NO_THROW(U = *tmp.begin() = 5);
	EXPECT_EQ(tmp[0], 5);
	static_assert(noexcept(*(++tmp.begin()) = 3));
	static_assert(std::is_assignable_v<decltype(*(++tmp.begin())), int>);
	static_assert(
		std::is_trivially_assignable_v<decltype(*(++tmp.begin())), int>);
	static_assert(
		std::is_nothrow_assignable_v<decltype(*(++tmp.begin())), int>);
	EXPECT_NO_THROW(*(++tmp.begin()) = 4);
	EXPECT_EQ(tmp[1], 4);
	static_assert(noexcept(*tmp.rbegin()));
	EXPECT_NO_THROW(U = *tmp.rbegin()); // pre-condition
	static_assert(noexcept(*tmp.rbegin() = 2));
	static_assert(std::is_assignable_v<decltype(*tmp.rbegin()), int>);
	static_assert(std::is_trivially_assignable_v<decltype(*tmp.rbegin()), int>);
	static_assert(std::is_nothrow_assignable_v<decltype(*tmp.rbegin()), int>);
	EXPECT_NO_THROW(U = *tmp.rbegin() = 9);
	EXPECT_EQ(tmp[3], 9);
	EXPECT_NO_THROW(*(++tmp.rbegin()) = 8);
	EXPECT_EQ(tmp[2], 8);
	EXPECT_NO_THROW(*(tmp.begin()++) = 2);
	EXPECT_EQ(tmp[0], 2);
	EXPECT_EQ(tmp[1], 4);
	EXPECT_NO_THROW(*(tmp.rbegin()++) = 7);
	EXPECT_EQ(tmp[3], 7);
	EXPECT_EQ(tmp[2], 8);
	EXPECT_NO_THROW(++(*tmp.begin() = 0)); // post-condition
	EXPECT_EQ(tmp[0], 1);
	EXPECT_NO_THROW(++(*tmp.rbegin() = 9));
	EXPECT_EQ(tmp[3], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r); // post-condition ++r

	U = 12; // suppress warning: assigned only once
	r = tmp.end();
}

TEST(SectionItr, ForwardIterator)
{
	test_elements TE{};
	auto& B = TE.A;
	Row<int, 2> A(B, 0);

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
		EXPECT_EQ(*A.begin()++, B[0][0]);
		EXPECT_EQ(*A.cbegin()++, B[0][0]);
		EXPECT_EQ(*A.rbegin()++, B[0][3]);
		EXPECT_EQ(*A.crbegin()++, B[0][3]);
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
			Board<int, 2> C;
			Row<int, 2> D(C, 1);
			Row<int, 2> E(C, 2);
			std::iota(D.begin(), D.end(), 1);
			EXPECT_EQ(C[1][0], 1);
			EXPECT_EQ(C[1][1], 2);
			EXPECT_EQ(C[1][2], 3);
			EXPECT_EQ(C[1][3], 4);
			std::iota(E.rbegin(), E.rend(), 1);
			EXPECT_EQ(C[2][3], 1);
			EXPECT_EQ(C[2][2], 2);
			EXPECT_EQ(C[2][1], 3);
			EXPECT_EQ(C[2][0], 4);
		}
		r = A.end(); // suppress warning: assigned only once
		i = A.end();
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorPreDecrement)
{ // operator--()
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

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

		// return type
		static_assert(std::is_same_v<decltype(A.end())&, decltype(--A.end())>);
		static_assert(std::is_same_v<int&, decltype(*(--A.end()))>);
		static_assert(
			std::is_same_v<decltype(A.cend())&, decltype(--A.cend())>);
		static_assert(std::is_same_v<int const&, decltype(*(--A.cend()))>);

		// Sanity-check
		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A[1], 1);
		ASSERT_EQ(A.back(), 3);
		ASSERT_EQ(A[2], 2);
		// iterator
		EXPECT_FALSE(A.end() == --A.end());
		EXPECT_TRUE(--A.end() == --A.end());
		EXPECT_FALSE(--A.end() == --(--A.end()));
		EXPECT_EQ(*(--A.end()), 3);
		EXPECT_EQ(*(--(--A.end())), 2);
		{ // value-semantics
			auto x = A.end();
			auto y = x; // copy
			--y;
			EXPECT_FALSE(y == A.end());
			EXPECT_TRUE(x == A.end());
			EXPECT_EQ(*y, 3);
			--x;
			EXPECT_TRUE(x == y);
			EXPECT_EQ(*--y, 2);
		}
		// const_iterator
		EXPECT_FALSE(A.cend() == --A.cend());
		EXPECT_TRUE(--A.cend() == --A.cend());
		EXPECT_FALSE(--A.cend() == --(--A.cend()));
		EXPECT_EQ(*(--A.cend()), 3);
		EXPECT_EQ(*(--(--A.cend())), 2);
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

TEST(SectionItrDeathTest, operatorPreDecrement)
{ // operator--()
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_bidir<decltype(A.begin())> && is_bidir<decltype(A.cbegin())> &&
		is_bidir<decltype(A.rbegin())> && is_bidir<decltype(A.crbegin())>)
	{
		EXPECT_DEBUG_DEATH(--A.begin(), "> 0");
		EXPECT_DEBUG_DEATH(--A.cbegin(), "> 0");
		EXPECT_DEBUG_DEATH(--A.rbegin(), "< elem_size");
		EXPECT_DEBUG_DEATH(--A.crbegin(), "< elem_size");
		--A.end();
		--A.cend();
		--A.rend();
		--A.crend();
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorPostDecrement)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

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

		// return type
		static_assert(std::is_same_v<decltype(A.end()), decltype(A.end()--)>);
		static_assert(std::is_same_v<decltype(A.cend()), decltype(A.cend()--)>);
		static_assert(std::is_same_v<int&, decltype(*(A.end()--))>);
		static_assert(std::is_same_v<int const&, decltype(*(A.cend()--))>);
		static_assert(std::is_same_v<int&, decltype(*A.rbegin()--)>);
		static_assert(std::is_same_v<int const&, decltype(*A.crbegin()--)>);

		// Sanity-check
		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A[1], 1);
		ASSERT_EQ(A.back(), 3);
		ASSERT_EQ(A[2], 2);
		// iterator
		EXPECT_TRUE(A.end() == A.end()--);
		EXPECT_FALSE(A.end()-- == --A.end());
		EXPECT_EQ(*(--(--A.end())), 2);
		{ // value-semantics
			auto x       = A.end();
			auto y       = x; // copy
			const auto z = y--;
			EXPECT_TRUE(x == A.end());
			EXPECT_TRUE(y != A.end());
			EXPECT_EQ(*y, 3);
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
			EXPECT_EQ(*x, 3);
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

TEST(SectionItrDeathTest, operatorPostDecrement)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_bidir<decltype(A.begin())> && is_bidir<decltype(A.cbegin())> &&
		is_bidir<decltype(A.rbegin())> && is_bidir<decltype(A.crbegin())>)
	{
		// (void)i-- equivalent to(void)-- i
		EXPECT_DEBUG_DEATH(A.begin()--, "> 0");
		EXPECT_DEBUG_DEATH(--A.cbegin(), "> 0");
		EXPECT_DEBUG_DEATH(A.rbegin()--, "< elem_size");
		EXPECT_DEBUG_DEATH(--A.crbegin(), "< elem_size");
		A.end()--;
		A.cend()--;
		A.rend()--;
		A.crend()--;
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, BidirectionalIterator)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_bidir<decltype(A.begin())> && is_bidir<decltype(A.cbegin())> &&
		is_bidir<decltype(A.rbegin())> && is_bidir<decltype(A.crbegin())>)
	{
		// --i (return type: It&)	is decrementable
		EXPECT_EQ(*(--A.end()), 3);
		EXPECT_EQ(*(--(--A.end())), 2);
		EXPECT_EQ(*(--A.cend()), 3);
		EXPECT_EQ(*(--(--A.cend())), 2);
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
		EXPECT_EQ(*(--A.end())--, 3);
		EXPECT_EQ(A.cend()--, A.cend());
		EXPECT_EQ(*(--A.cend())--, 3);
		EXPECT_EQ(A.rend()--, A.rend());
		EXPECT_EQ(*(--A.rend())--, 9);
		EXPECT_EQ(A.crend()--, A.crend());
		EXPECT_EQ(*(--A.crend())--, 9);

		{ // Algorithms using Bidirectional iterator
			Row<int, 2> B(TE.A, 1);

			std::copy_backward(A.cbegin(), A.cend(), B.end());
			EXPECT_EQ(B[0], 9);
			EXPECT_EQ(B[1], 1);
			EXPECT_EQ(B[2], 2);
			EXPECT_EQ(B[3], 3);
			EXPECT_EQ(TE.A[1][0], 9);
			EXPECT_EQ(TE.A[1][1], 1);
			EXPECT_EQ(TE.A[1][2], 2);
			EXPECT_EQ(TE.A[1][3], 3);
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorIntegerAdditionAssignment)
{ // operator+=(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() += 3));
		static_assert(noexcept(A.cbegin() += 2));
		static_assert(noexcept(A.rbegin() += 3));
		static_assert(noexcept(A.crbegin() += 1));
		static_assert(noexcept(cA.begin() += 3));
		static_assert(noexcept(cA.rbegin() += 2));
		static_assert(noexcept(A.end() += -3));
		static_assert(noexcept(A.cend() += -2));
		static_assert(noexcept(A.rend() += -1));
		static_assert(noexcept(A.crend() += -1));
		static_assert(noexcept(cA.end() += -1));
		static_assert(noexcept(cA.rend() += -3));

		static_assert(
			std::is_same_v<decltype(A.begin())&, decltype(A.begin() += 2)>);
		static_assert(
			std::is_same_v<decltype(A.cbegin())&, decltype(A.cbegin() += 2)>);
		static_assert(std::is_same_v<int&, decltype(*(A.begin() += 2))>);
		static_assert(std::is_same_v<int const&, decltype(*(A.cbegin() += 2))>);
		static_assert(std::is_same_v<int&, decltype(*(A.rbegin() += 2))>);
		static_assert(
			std::is_same_v<int const&, decltype(*(A.crbegin() += 2))>);

		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 3);

		EXPECT_TRUE((A.begin() += -0) == A.begin());
		EXPECT_EQ(*(A.begin() += -0), 9);
		EXPECT_TRUE((A.begin() += 0) == A.begin());
		EXPECT_EQ(*(A.begin() += 0), 9);
		EXPECT_TRUE((A.begin() += 1) == ++A.begin());
		EXPECT_TRUE((A.begin() += 4) == A.end());
		EXPECT_TRUE((A.end() += 0) == A.end());
		EXPECT_TRUE((A.end() += -0) == A.end());
		EXPECT_TRUE((A.end() += -1) == --A.end());
		EXPECT_EQ(*(A.end() += -1), 3);
		EXPECT_TRUE((A.end() += -4) == A.begin());
		EXPECT_EQ(*(A.end() += -4), 9);
		EXPECT_EQ(*(A.begin() += 3), 3);
		{ // return reference to iterator
			auto I = A.begin();
			EXPECT_TRUE((I += 1) == ++A.begin());
			EXPECT_TRUE((I += 1) == (A.begin() += 2));
			EXPECT_TRUE((I += 2) == A.end());
			EXPECT_TRUE((I += -1) == --A.end());
			I = A.end(); // suppress warning: assigned only once
		}
		// const_iterator
		EXPECT_TRUE((A.cbegin() += -0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() += 0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() += 1) == ++A.cbegin());
		EXPECT_TRUE((A.cbegin() += 4) == A.cend());

		EXPECT_TRUE((A.cend() += 0) == A.cend());
		EXPECT_TRUE((A.cend() += -0) == A.cend());
		EXPECT_TRUE((A.cend() += -1) == --A.cend());
		EXPECT_TRUE((A.cend() += -4) == A.cbegin());
		{ // return reference to iterator
			auto I = A.cbegin();
			EXPECT_TRUE((I += 1) == ++A.cbegin());
			EXPECT_TRUE((I += 1) == (A.cbegin() += 2));
			EXPECT_TRUE((I += 2) == A.cend());
			EXPECT_TRUE((I += -1) == --A.cend());
			I = A.cend(); // suppress warning: assigned only once
		}
		// reverse_iterator
		EXPECT_TRUE((A.rbegin() += -0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() += 0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() += 1) == ++A.rbegin());
		EXPECT_TRUE((A.rbegin() += 4) == A.rend());

		EXPECT_TRUE((A.rend() += 0) == A.rend());
		EXPECT_TRUE((A.rend() += -0) == A.rend());
		EXPECT_TRUE((A.rend() += -1) == --A.rend());
		EXPECT_TRUE((A.rend() += -4) == A.rbegin());
		{ // return reference to iterator
			auto I = A.rbegin();
			EXPECT_TRUE((I += 1) == ++A.rbegin());
			EXPECT_TRUE((I += 1) == (A.rbegin() += 2));
			EXPECT_TRUE((I += 2) == A.rend());
			EXPECT_TRUE((I += -1) == --A.rend());
			I = A.rend(); // suppress warning: assigned only once
		}
		// const_reverse_iterator
		EXPECT_TRUE((A.crbegin() += -0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() += 0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() += 1) == ++A.crbegin());
		EXPECT_TRUE((A.crbegin() += 4) == A.crend());

		EXPECT_TRUE((A.crend() += 0) == A.crend());
		EXPECT_TRUE((A.crend() += -0) == A.crend());
		EXPECT_TRUE((A.crend() += -1) == --A.crend());
		EXPECT_TRUE((A.crend() += -4) == A.crbegin());
		{ // return reference to iterator
			auto I = A.crbegin();
			EXPECT_TRUE((I += 1) == ++A.crbegin());
			EXPECT_TRUE((I += 1) == (A.crbegin() += 2));
			EXPECT_TRUE((I += 2) == A.crend());
			EXPECT_TRUE((I += -1) == --A.crend());
			I = A.crend(); // suppress warning: assigned only once
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorIntegerAdditionAssignment)
{ // operator+=(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		EXPECT_DEBUG_DEATH(A.begin() += -1, ">= 0");
		EXPECT_DEBUG_DEATH(A.begin() += 5, "<= elem_size");
		EXPECT_DEBUG_DEATH(A.begin() += 50, "<= elem_size");
		EXPECT_DEBUG_DEATH(A.end() += 1, "<= elem_size");
		EXPECT_DEBUG_DEATH(A.end() += -5, ">= 0");
		EXPECT_DEBUG_DEATH(A.end() += -50, ">= 0");

		// const_iterator
		EXPECT_DEBUG_DEATH(A.cbegin() += -1, ">= 0");
		EXPECT_DEBUG_DEATH(A.cbegin() += 5, "<= elem_size");

		EXPECT_DEBUG_DEATH(A.cend() += 1, "<= elem_size");
		EXPECT_DEBUG_DEATH(A.cend() += -5, ">= 0");

		// reverse_iterator
		EXPECT_DEBUG_DEATH(A.rbegin() += -1, "< elem_size");
		EXPECT_DEBUG_DEATH(A.rbegin() += 5, ">= -1");

		EXPECT_DEBUG_DEATH(A.rend() += -5, "< elem_size");
		EXPECT_DEBUG_DEATH(A.rend() += 1, ">= -1");

		// const_reverse_iterator
		EXPECT_DEBUG_DEATH(A.crbegin() += -1, "< elem_size");
		EXPECT_DEBUG_DEATH(A.crbegin() += 5, ">= -1");

		EXPECT_DEBUG_DEATH(A.crend() += 1, ">= -1");
		EXPECT_DEBUG_DEATH(A.crend() += -5, "< elem_size");
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorIntegerAddition)
{ // operator+(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() + 3));
		static_assert(noexcept(A.cbegin() + 2));
		static_assert(noexcept(A.rbegin() + 4));
		static_assert(noexcept(A.crbegin() + 1));
		static_assert(noexcept(cA.begin() + 4));
		static_assert(noexcept(cA.rbegin() + 3));
		static_assert(noexcept(A.end() + -3));
		static_assert(noexcept(A.cend() + -2));
		static_assert(noexcept(A.rend() + -4));
		static_assert(noexcept(A.crend() + -1));
		static_assert(noexcept(cA.end() + -4));
		static_assert(noexcept(cA.rend() + -3));

		static_assert(
			std::is_same_v<decltype(A.begin()), decltype(A.begin() + 2)>);
		static_assert(std::is_same_v<int&, decltype(*(A.begin() + 2))>);

		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 3);

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
			EXPECT_TRUE((I + -0) == A.begin());
			EXPECT_TRUE((I + 0) == A.begin());
			EXPECT_TRUE((I + 1) == ++A.begin());
			EXPECT_TRUE((I + 4) == A.end());
		}
		{ // const_iterator
			EXPECT_TRUE((A.cbegin() + -0) == A.cbegin());
			EXPECT_TRUE((A.cbegin() + 0) == A.cbegin());
			EXPECT_TRUE((A.cbegin() + 1) == ++A.cbegin());
			EXPECT_TRUE((A.cbegin() + 4) == A.cend());
		}
		{ // reverse_iterator
			EXPECT_TRUE((A.rbegin() + -0) == A.rbegin());
			EXPECT_TRUE((A.rbegin() + 0) == A.rbegin());
			EXPECT_TRUE((A.rbegin() + 1) == ++A.rbegin());
			EXPECT_TRUE((A.rbegin() + 4) == A.rend());
		}
		{ // const_reverse_iterator
			EXPECT_TRUE((A.crbegin() + -0) == A.crbegin());
			EXPECT_TRUE((A.crbegin() + 0) == A.crbegin());
			EXPECT_TRUE((A.crbegin() + 1) == ++A.crbegin());
			EXPECT_TRUE((A.crbegin() + 4) == A.crend());
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorIntegerAddition)
{ // operator+(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		{
			[[maybe_unused]] const auto I = A.begin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = I + -1, ">= 0");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = I + 5, "<= elem_size");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = I + 50, "<= elem_size");
		}
		{ // const_iterator
			[[maybe_unused]] const auto cI = A.cbegin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = cI + -1, ">= 0");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] const auto U = cI + 5, "<= elem_size");
		}
		{ // reverse_iterator
			[[maybe_unused]] const auto rI = A.rbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = rI + -1, "< elem_size");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = rI + 5, ">= -1");
		}
		{ // const_reverse_iterator
			[[maybe_unused]] const auto crI = A.crbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = crI + -1, "< elem_size");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = crI + 5, ">= -1");
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorAddition)
{ // operator+(int, iterator)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(3 + A.begin()));
		static_assert(noexcept(2 + A.cbegin()));
		static_assert(noexcept(3 + A.rbegin()));
		static_assert(noexcept(1 + A.crbegin()));
		static_assert(noexcept(2 + cA.begin()));
		static_assert(noexcept(2 + cA.rbegin()));
		static_assert(noexcept(-3 + A.end()));
		static_assert(noexcept(-2 + A.cend()));
		static_assert(noexcept(-4 + A.rend()));
		static_assert(noexcept(-2 + A.crend()));
		static_assert(noexcept(-3 + cA.end()));
		static_assert(noexcept(-2 + cA.rend()));

		static_assert(
			std::is_same_v<decltype(A.begin()), decltype(2 + A.begin())>);
		static_assert(std::is_same_v<int&, decltype(*(3 + A.begin()))>);

		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 3);

		{ // Must return a new iterator object
			const auto X = A.begin();
			const auto Y = 3 + X;
			EXPECT_TRUE(Y != A.begin());
			ASSERT_TRUE(X == A.begin()); // Stop on failure.
			const auto cX = A.cbegin();
			const auto cY = 2 + cX;
			EXPECT_TRUE(cY != A.cbegin());
			ASSERT_TRUE(cX == A.cbegin()); // Stop on failure.
			const auto rX = A.rbegin();
			const auto rY = 3 + rX;
			EXPECT_TRUE(rY != A.rbegin());
			ASSERT_TRUE(rX == A.rbegin()); // Stop on failure.
			const auto crX = A.crbegin();
			const auto crY = 1 + crX;
			EXPECT_TRUE(crY != A.crbegin());
			ASSERT_TRUE(crX == A.crbegin()); // Stop on failure.
		}
		{
			[[maybe_unused]] const auto I = A.begin();
			EXPECT_TRUE((-0 + I) == A.begin());
			EXPECT_TRUE((0 + I) == A.begin());
			EXPECT_TRUE((1 + I) == ++A.begin());
			EXPECT_TRUE((4 + I) == A.end());
		}
		{ // const_iterator
			EXPECT_TRUE((-0 + A.cbegin()) == A.cbegin());
			EXPECT_TRUE((0 + A.cbegin()) == A.cbegin());
			EXPECT_TRUE((1 + A.cbegin()) == ++A.cbegin());
			EXPECT_TRUE((4 + A.cbegin()) == A.cend());
		}
		{ // reverse_iterator
			EXPECT_TRUE((-0 + A.rbegin()) == A.rbegin());
			EXPECT_TRUE((0 + A.rbegin()) == A.rbegin());
			EXPECT_TRUE((1 + A.rbegin()) == ++A.rbegin());
			EXPECT_TRUE((4 + A.rbegin()) == A.rend());
		}
		{ // const_reverse_iterator
			EXPECT_TRUE((-0 + A.crbegin()) == A.crbegin());
			EXPECT_TRUE((0 + A.crbegin()) == A.crbegin());
			EXPECT_TRUE((1 + A.crbegin()) == ++A.crbegin());
			EXPECT_TRUE((4 + A.crbegin()) == A.crend());
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorAddition)
{ // operator+(int, iterator)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		{
			[[maybe_unused]] const auto I = A.begin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = -1 + I, ">= 0");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = 5 + I, "<= elem_size");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = 50 + I, "<= elem_size");
		}
		{ // const_iterator
			[[maybe_unused]] const auto cI = A.cbegin();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = -1 + cI, ">= 0");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = 5 + cI, "<= elem_size");
		}
		{ // reverse_iterator
			[[maybe_unused]] const auto rI = A.rbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = -1 + rI, "< elem_size");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = 5 + rI, ">= -1");
		}
		{ // const_reverse_iterator
			[[maybe_unused]] const auto crI = A.crbegin();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = -1 + crI, "< elem_size");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = 5 + crI, ">= -1");
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorIntegerSubtractionAssignment)
{ // operator-=(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.end() -= 3));
		static_assert(noexcept(A.cend() -= 2));
		static_assert(noexcept(A.rend() -= 4));
		static_assert(noexcept(A.crend() -= 1));

		static_assert(
			std::is_same_v<decltype(A.end())&, decltype(A.end() -= 2)>);
		static_assert(
			std::is_same_v<decltype(A.cend())&, decltype(A.cend() -= 2)>);
		static_assert(std::is_same_v<int&, decltype(*(A.end() -= 2))>);
		static_assert(std::is_same_v<int const&, decltype(*(A.cend() -= 2))>);
		static_assert(std::is_same_v<int&, decltype(*(A.rend() -= 2))>);
		static_assert(std::is_same_v<int const&, decltype(*(A.crend() -= 2))>);

		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 3);

		EXPECT_TRUE((A.end() -= -0) == A.end());
		EXPECT_TRUE((A.end() -= 0) == A.end());
		EXPECT_TRUE((A.end() -= 1) == --A.end());
		EXPECT_TRUE((A.end() -= 4) == A.begin());

		EXPECT_TRUE((A.begin() -= 0) == A.begin());
		EXPECT_TRUE((A.begin() -= -0) == A.begin());
		EXPECT_TRUE((A.begin() -= -1) == ++A.begin());
		EXPECT_EQ(*(A.begin() -= -1), 1);
		EXPECT_TRUE((A.begin() -= -4) == A.end());
		EXPECT_EQ(*(A.end() -= 4), 9);
		{ // return reference to iterator
			auto I = A.end();
			EXPECT_TRUE((I -= 1) == --A.end());
			EXPECT_TRUE((I -= 1) == (A.end() -= 2));
			EXPECT_TRUE((I -= 2) == A.begin());
			EXPECT_TRUE((I -= -1) == ++A.begin());
			I = A.begin(); // suppress warning: assigned only once
		}
		// const_iterator
		EXPECT_TRUE((A.cbegin() -= 0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() -= -0) == A.cbegin());
		EXPECT_TRUE((A.cbegin() -= -1) == ++A.cbegin());
		EXPECT_TRUE((A.cbegin() -= -4) == A.cend());

		EXPECT_TRUE((A.cend() -= -0) == A.cend());
		EXPECT_TRUE((A.cend() -= 0) == A.cend());
		EXPECT_TRUE((A.cend() -= 1) == --A.cend());
		EXPECT_TRUE((A.cend() -= 4) == A.cbegin());
		{ // return reference to iterator
			auto I = A.cend();
			EXPECT_TRUE((I -= 1) == --A.cend());
			EXPECT_TRUE((I -= 1) == (A.cend() -= 2));
			EXPECT_TRUE((I -= 2) == A.cbegin());
			EXPECT_TRUE((I -= -1) == ++A.cbegin());
			I = A.cbegin(); // suppress warning: assigned only once
		}
		// reverse_iterator
		EXPECT_TRUE((A.rbegin() -= 0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() -= -0) == A.rbegin());
		EXPECT_TRUE((A.rbegin() -= -1) == ++A.rbegin());
		EXPECT_TRUE((A.rbegin() -= -4) == A.rend());

		EXPECT_TRUE((A.rend() -= -0) == A.rend());
		EXPECT_TRUE((A.rend() -= 0) == A.rend());
		EXPECT_TRUE((A.rend() -= 1) == --A.rend());
		EXPECT_TRUE((A.rend() -= 4) == A.rbegin());
		{ // return reference to iterator
			auto I = A.rend();
			EXPECT_TRUE((I -= 1) == --A.rend());
			EXPECT_TRUE((I -= 1) == (A.rend() -= 2));
			EXPECT_TRUE((I -= 2) == A.rbegin());
			EXPECT_TRUE((I -= -1) == ++A.rbegin());
			I = A.rbegin(); // suppress warning: assigned only once
		}
		// const_reverse_iterator
		EXPECT_TRUE((A.crbegin() -= 0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() -= -0) == A.crbegin());
		EXPECT_TRUE((A.crbegin() -= -1) == ++A.crbegin());
		EXPECT_TRUE((A.crbegin() -= -4) == A.crend());

		EXPECT_TRUE((A.crend() -= -0) == A.crend());
		EXPECT_TRUE((A.crend() -= 0) == A.crend());
		EXPECT_TRUE((A.crend() -= 1) == --A.crend());
		EXPECT_TRUE((A.crend() -= 4) == A.crbegin());
		{ // return reference to iterator
			auto I = A.crend();
			EXPECT_TRUE((I -= 1) == --A.crend());
			EXPECT_TRUE((I -= 1) == (A.crend() -= 2));
			EXPECT_TRUE((I -= 2) == A.crbegin());
			EXPECT_TRUE((I -= -1) == ++A.crbegin());
			I = A.crbegin(); // suppress warning: assigned only once
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorIntegerSubtractionAssignment)
{ // operator-=(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		EXPECT_DEBUG_DEATH(A.end() -= -1, "<= elem_size");
		EXPECT_DEBUG_DEATH(A.end() -= 5, ">= 0");

		EXPECT_DEBUG_DEATH(A.begin() -= 1, ">= 0");
		EXPECT_DEBUG_DEATH(A.begin() -= -5, "<= elem_size");

		// const_iterator
		EXPECT_DEBUG_DEATH(A.cbegin() -= 1, ">= 0");
		EXPECT_DEBUG_DEATH(A.cbegin() -= -5, "<= elem_size");

		EXPECT_DEBUG_DEATH(A.cend() -= -1, "<= elem_size");
		EXPECT_DEBUG_DEATH(A.cend() -= 5, ">= 0");

		// reverse_iterator
		EXPECT_DEBUG_DEATH(A.rbegin() -= 1, "< elem_size");
		EXPECT_DEBUG_DEATH(A.rbegin() -= -5, ">= -1");

		EXPECT_DEBUG_DEATH(A.rend() -= -1, ">= -1");
		EXPECT_DEBUG_DEATH(A.rend() -= 5, "< elem_size");

		// const_reverse_iterator
		EXPECT_DEBUG_DEATH(A.crbegin() -= 1, "< elem_size");
		EXPECT_DEBUG_DEATH(A.crbegin() -= -5, ">= -1");

		EXPECT_DEBUG_DEATH(A.crend() -= -1, ">= -1");
		EXPECT_DEBUG_DEATH(A.crend() -= 5, "< elem_size");
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorIntegerUnaryMinus)
{ // operator-(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() - -3));
		static_assert(noexcept(A.cbegin() - -2));
		static_assert(noexcept(A.rbegin() - -4));
		static_assert(noexcept(A.crbegin() - -1));
		static_assert(noexcept(cA.begin() - -4));
		static_assert(noexcept(cA.rbegin() - -3));
		static_assert(noexcept(A.end() - 3));
		static_assert(noexcept(A.cend() - 2));
		static_assert(noexcept(A.rend() - 4));
		static_assert(noexcept(A.crend() - 1));
		static_assert(noexcept(cA.end() - 4));
		static_assert(noexcept(cA.rend() - 3));

		static_assert(
			std::is_same_v<decltype(A.begin()), decltype(A.end() - 2)>);
		static_assert(std::is_same_v<int&, decltype(*(A.end() - 2))>);

		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 3);

		{ // Must return a new iterator object
			const auto X = A.end();
			const auto Y = X - 2;
			EXPECT_TRUE(Y != A.end());
			ASSERT_TRUE(X == A.end()); // Stop on failure.
			const auto cX = A.cend();
			const auto cY = cX - 3;
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
			EXPECT_TRUE((I - -0) == A.end());
			EXPECT_TRUE((I - 0) == A.end());
			EXPECT_TRUE((I - 1) == --A.end());
			EXPECT_TRUE((I - 4) == A.begin());
		}
		{ // const_iterator
			EXPECT_TRUE((A.cend() - -0) == A.cend());
			EXPECT_TRUE((A.cend() - 0) == A.cend());
			EXPECT_TRUE((A.cend() - 1) == --A.cend());
			EXPECT_TRUE((A.cend() - 4) == A.cbegin());
		}
		{ // reverse_iterator
			EXPECT_TRUE((A.rend() - -0) == A.rend());
			EXPECT_TRUE((A.rend() - 0) == A.rend());
			EXPECT_TRUE((A.rend() - 1) == --A.rend());
			EXPECT_TRUE((A.rend() - 4) == A.rbegin());
		}
		{ // const_reverse_iterator
			EXPECT_TRUE((A.crend() - -0) == A.crend());
			EXPECT_TRUE((A.crend() - 0) == A.crend());
			EXPECT_TRUE((A.crend() - 1) == --A.crend());
			EXPECT_TRUE((A.crend() - 4) == A.crbegin());
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorIntegerUnaryMinus)
{ // operator-(int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		{
			[[maybe_unused]] const auto I = A.end();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = I - -1, "<= elem_size");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = I - 5, ">= 0");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = I - 50, ">= 0");
		}
		{ // const_iterator
			[[maybe_unused]] const auto cI = A.cend();
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = cI - -1, "<= elem_size");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = cI - 5, ">= 0");
		}
		{ // reverse_iterator
			[[maybe_unused]] const auto rI = A.rend();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = rI - -1, ">= -1");
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = rI - 5, "< elem_size");
		}
		{ // const_reverse_iterator
			[[maybe_unused]] const auto crI = A.crend();
			EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = crI - -1, ">= -1");
			EXPECT_DEBUG_DEATH(
				[[maybe_unused]] auto U = crI - 5, "< elem_size");
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorUnaryMinus)
{ // operator-(iterator)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		static_assert(noexcept(A.begin() - A.end()));
		static_assert(noexcept(A.end() - A.begin()));
		static_assert(noexcept(A.cend() - A.cbegin()));
		static_assert(noexcept(A.rend() - A.rbegin()));
		static_assert(noexcept(A.crend() - A.crbegin()));

		// Return type:
		static_assert(
			std::is_same_v<std::ptrdiff_t, decltype(A.begin() - A.end())>);
		static_assert(
			std::is_same_v<std::ptrdiff_t, decltype(A.cbegin() - A.cend())>);
		static_assert(
			std::is_same_v<std::ptrdiff_t, decltype(A.rbegin() - A.rend())>);

		ASSERT_EQ(A.size(), 4);
		EXPECT_EQ(A.begin() - A.begin(), 0);
		EXPECT_EQ(A.end() - A.end(), 0);
		EXPECT_EQ(A.end() - A.begin(), 4);
		EXPECT_EQ(A.begin() - A.end(), -4);
		EXPECT_EQ(++A.begin() - A.begin(), 1);
		EXPECT_EQ(++A.begin() - ++A.begin(), 0);
		{
			const auto X = ++A.begin();
			const auto Y = A.begin() += 2;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
		EXPECT_EQ(A.cbegin() - A.cbegin(), 0);
		EXPECT_EQ(A.cend() - A.cend(), 0);
		EXPECT_EQ(A.cend() - A.cbegin(), 4);
		EXPECT_EQ(A.cbegin() - A.cend(), -4);
		EXPECT_EQ(++A.cbegin() - A.cbegin(), 1);
		EXPECT_EQ(++A.cbegin() - ++A.cbegin(), 0);
		EXPECT_EQ(cA.begin() - cA.end(), -4);
		EXPECT_EQ(cA.cbegin() - cA.end(), -4);
		{
			const auto X = ++A.cbegin();
			const auto Y = A.cbegin() += 2;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
		EXPECT_EQ(A.rbegin() - A.rbegin(), 0);
		EXPECT_EQ(A.rend() - A.rend(), 0);
		EXPECT_EQ(A.rend() - A.rbegin(), 4);
		EXPECT_EQ(A.rbegin() - A.rend(), -4);
		EXPECT_EQ(++A.rbegin() - A.rbegin(), 1);
		EXPECT_EQ(++A.rbegin() - ++A.rbegin(), 0);
		{
			const auto X = ++A.rbegin();
			const auto Y = A.rbegin() += 2;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
		EXPECT_EQ(A.crbegin() - A.crbegin(), 0);
		EXPECT_EQ(A.crend() - A.crend(), 0);
		EXPECT_EQ(A.crend() - A.crbegin(), 4);
		EXPECT_EQ(A.crbegin() - A.crend(), -4);
		EXPECT_EQ(++A.crbegin() - A.crbegin(), 1);
		EXPECT_EQ(++A.crbegin() - ++A.crbegin(), 0);
		{
			const auto X = ++A.crbegin();
			const auto Y = A.crbegin() += 2;
			EXPECT_TRUE(Y + (X - Y) == X);
			EXPECT_TRUE(X + (Y - X) == Y);
		}
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorUnaryMinus)
{ // operator-(iterator)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		auto board = TE.A;
		Row<int, 2> B(board, 0);
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.end() - B.begin(), "is_same_Section");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend() - B.cbegin(), "is_same_Section");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.rend() - B.rbegin(), "is_same_Section");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crend() - B.crbegin(),
			"is_same_Section");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend() - cA.cbegin(),
			"is_same_Section");
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorSubscriptMemberAccess)
{ // & operator[](int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

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
		static_assert(std::is_same_v<int&, decltype(A.begin()[2])>);
		static_assert(std::is_same_v<int const&, decltype(A.cbegin()[2])>);
		static_assert(std::is_same_v<int&, decltype(A.rbegin()[2])>);
		static_assert(std::is_same_v<int const&, decltype(A.crbegin()[2])>);

		ASSERT_EQ(A.size(), 4);
		ASSERT_EQ(A.front(), 9);
		ASSERT_EQ(A.back(), 3);

		EXPECT_EQ(A.begin()[-0], 9);
		EXPECT_EQ(A.begin()[0], 9);
		EXPECT_EQ(A.begin()[1], 1);
		EXPECT_EQ(A.begin()[3], 3);
		{ // return dereferenced iterator
			const auto I = A.begin();
			EXPECT_TRUE(I[1] == *(++A.begin()));
			EXPECT_TRUE(I[1] == *(++A.begin()));
			EXPECT_TRUE(I[2] == *(A.begin() + 2));
		}
		EXPECT_EQ(A.end()[-1], 3);
		EXPECT_EQ(A.end()[-4], 9);

		// const_iterator
		EXPECT_EQ(A.cbegin()[-0], 9);
		EXPECT_EQ(A.cbegin()[0], 9);
		EXPECT_EQ(A.cbegin()[1], 1);
		EXPECT_EQ(A.cbegin()[3], 3);
		{ // return dereferenced iterator
			const auto I = A.cbegin();
			EXPECT_TRUE(I[1] == *(++A.cbegin()));
			EXPECT_TRUE(I[1] == *(++A.cbegin()));
			EXPECT_TRUE(I[2] == *(A.cbegin() + 2));
		}
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend()[1], "<= elem_size");
		EXPECT_EQ(A.cend()[-1], 3);
		EXPECT_EQ(A.cend()[-4], 9);

		// reverse_iterator
		EXPECT_EQ(A.rbegin()[-0], 3);
		EXPECT_EQ(A.rbegin()[0], 3);
		EXPECT_EQ(A.rbegin()[1], 2);
		EXPECT_EQ(A.rbegin()[3], 9);
		{ // return dereferenced iterator
			const auto I = A.rbegin();
			EXPECT_TRUE(I[1] == *(++A.rbegin()));
			EXPECT_TRUE(I[1] == *(++A.rbegin()));
			EXPECT_TRUE(I[2] == *(A.rbegin() + 2));
		}
		EXPECT_EQ(A.rend()[-1], 9);
		EXPECT_EQ(A.rend()[-4], 3);

		// const_reverse_iterator
		EXPECT_EQ(A.crbegin()[-0], 3);
		EXPECT_EQ(A.crbegin()[0], 3);
		EXPECT_EQ(A.crbegin()[1], 2);
		EXPECT_EQ(A.crbegin()[3], 9);
		{ // return dereferenced iterator
			const auto I = A.crbegin();
			EXPECT_TRUE(I[1] == *(++A.crbegin()));
			EXPECT_TRUE(I[1] == *(++A.crbegin()));
			EXPECT_TRUE(I[2] == *(A.crbegin() + 2));
		}
		EXPECT_EQ(A.crend()[-1], 9);
		EXPECT_EQ(A.crend()[-4], 3);

		//====------------------------------------------------------------====//
		// Output
		static_assert(noexcept(A.begin()[0] = 1));
		static_assert(std::is_assignable_v<decltype(A.begin()[0]), int>);
		static_assert(
			std::is_trivially_assignable_v<decltype(A.begin()[0]), int>);
		static_assert(
			std::is_nothrow_assignable_v<decltype(A.begin()[0]), int>);
		static_assert(noexcept(A.rbegin()[0] = 1));
		static_assert(std::is_assignable_v<decltype(A.rbegin()[0]), int>);
		static_assert(
			std::is_trivially_assignable_v<decltype(A.rbegin()[0]), int>);
		static_assert(
			std::is_nothrow_assignable_v<decltype(A.rbegin()[0]), int>);

		A.begin()[0] = 1;
		EXPECT_EQ(A[0], 1);
		A.begin()[1] = 9;
		EXPECT_EQ(A[3], 3);
		A.rbegin()[0] = 8;
		EXPECT_EQ(A[3], 8);
		A.rbegin()[1] = 2;
		EXPECT_EQ(A[0], 1);
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorSubscriptMemberAccess)
{ // & operator[](int)
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.rbegin())> &&
		is_random<decltype(A.cbegin())> && is_random<decltype(A.crbegin())>)
	{
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.begin()[-1], ">= 0");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.begin()[4], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin()[5], "<= elem_size");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.end()[1], "<= elem_size");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.end()[0], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.end()[5], "<= elem_size");

		// const_iterator
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.cbegin()[-1], ">= 0");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.cbegin()[4], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cbegin()[5], "<= elem_size");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.cend()[0], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.cend()[5], "<= elem_size");

		// reverse_iterator
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.rbegin()[-1], "< elem_size");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.rbegin()[4], "is_valid");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.rbegin()[5], ">= -1");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.rend()[1], ">= -1");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.rend()[0], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.rend()[-5], "< elem_size");

		// const_reverse_iterator
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crbegin()[-1], "< elem_size");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crbegin()[4], "is_valid");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.crbegin()[5], ">= -1");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.crend()[1], ">= -1");
		EXPECT_DEBUG_DEATH([[maybe_unused]] auto U = A.crend()[0], "is_valid");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.crend()[-5], "< elem_size");
	}
	else
		ADD_FAILURE();
}

TEST(SectionItr, operatorComparison)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

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
	}
	else
		ADD_FAILURE();
}

TEST(SectionItrDeathTest, operatorComparison)
{
	test_elements TE{};
	Row<int, 2> A(TE.A, 0);
	const const_Row<int, 2> cA(TE.cA, 0);

	if constexpr (
		is_random<decltype(A.begin())> && is_random<decltype(A.cbegin())> &&
		is_random<decltype(A.rbegin())> && is_random<decltype(A.crbegin())>)
	{
		// different boards
		Board<int, 2> boardB(TE.A);
		Row<int, 2> B(boardB, 0);
		boardB[1][2] = 0;
		TE.A[1][2]   = 8;
		static_assert(noexcept(A.begin() < B.end()));
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() < B.end(), "is_same_Section");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() <= B.end(), "is_same_Section");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() >= B.end(), "is_same_Section");
		EXPECT_DEBUG_DEATH(
			[[maybe_unused]] auto U = A.begin() > B.end(), "is_same_Section");
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

TEST(SectionItr, IteratorLoop)
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
		total += range_for_sum(Row<int, 2>(A, i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(Col<int, 2>(A, i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(Block<int, 2>(A, i));
	}
	EXPECT_EQ(total, 129);
	total = range_for_sum(cA);
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total += range_for_sum(const_Row<int, 2>(cA, i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total +=
			range_for_sum(::Sudoku::Board_Section::const_Col<int, 2>(cA, i));
	}
	EXPECT_EQ(total, 129);
	total = 0;
	for (int i{}; i < 4; ++i)
	{
		total +=
			range_for_sum(::Sudoku::Board_Section::const_Block<int, 2>(cA, i));
	}
	EXPECT_EQ(total, 129);
}
} // namespace SudokuTests::Section_Iterator::Members
