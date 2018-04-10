//====---- SudokuTests/Board_Section_Iterators.cpp                    ----====//
//
// Unit tests for the iterators operating on Board_Sections
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
// helpers
#include <Sudoku/Iterator_Utilities.h>
#include <Sudoku/Options.h>
// Debug Output
#include "print_Options.h"
// library
#include <numeric> // accumulate
#include <random>  // random-access tests
#include <type_traits>


namespace SudokuTests::BoardTest
{
using namespace ::Sudoku;
using ::Sudoku::Utility_::is_input;
using ::Sudoku::Utility_::is_forward;
using ::Sudoku::Utility_::is_bidir;
using ::Sudoku::Utility_::is_random;

namespace RowIterator
{
	using typeT = typename Board_Section::Row<int, 3>;
	// all iterator categories
	static_assert(std::is_object_v<typeT::iterator>);
	static_assert(std::is_object_v<typeT::const_iterator>);
	// static_assert(std::is_object_v<typeT::reverse_iterator>);
	// static_assert(std::is_object_v<typeT::const_reverse_iterator>);
	static_assert(!std::is_const_v<typeT::iterator>);
	static_assert(!std::is_const_v<typeT::const_iterator>);
	// static_assert(!std::is_const_v<typeT::reverse_iterator>);
	// static_assert(!std::is_const_v<typeT::const_reverse_iterator>);
	// CopyConstructible
	static_assert(std::is_copy_constructible_v<typeT::iterator>);
	static_assert(std::is_copy_constructible_v<typeT::const_iterator>);
	// static_assert(std::is_copy_constructible_v <typeT::reverse_iterator>);
	// static_assert(std::is_copy_constructible_v
	// <typeT::const_reverse_iterator>);
	// CopyAssignable
	static_assert(std::is_copy_assignable_v<typeT::iterator>);
	static_assert(std::is_copy_assignable_v<typeT::const_iterator>);
	// static_assert(std::is_copy_assignable_v <typeT::reverse_iterator>);
	// static_assert(std::is_copy_assignable_v <typeT::const_reverse_iterator>);
	// Destructible
	static_assert(std::is_destructible_v<typeT::iterator>);
	static_assert(std::is_destructible_v<typeT::const_iterator>);
	// static_assert(std::is_destructible_v<typeT::reverse_iterator>);
	// static_assert(std::is_destructible_v<typeT::const_reverse_iterator>);
	// Swappable (lvalues)
	// static_assert(std::is_swappable_v<typeT::iterator>);
	// static_assert(std::is_swappable_v<typeT::const_iterator>);
	// static_assert(std::is_swappable_v<typeT::reverse_iterator>);
	// static_assert(std::is_swappable_v<typeT::const_reverse_iterator>);

	// std::iterator_traits<It> has member typedefs:
	// value_type, difference_type, reference, pointer, iterator_category
	using traits = typename std::iterator_traits<typeT::iterator>;
	static_assert(std::is_object_v<traits::iterator_category>);
	static_assert(std::is_object_v<traits::value_type>);
	static_assert(!std::is_object_v<traits::reference>);
	static_assert(std::is_reference_v<traits::reference>);
	static_assert(std::is_lvalue_reference_v<traits::reference>);
	static_assert(std::is_pointer_v<traits::pointer>);
	static_assert(std::is_object_v<traits::difference_type>);

	using ctraits = typename std::iterator_traits<typeT::const_iterator>;
	static_assert(std::is_object_v<ctraits::iterator_category>);
	static_assert(std::is_object_v<ctraits::value_type>);
	static_assert(!std::is_object_v<ctraits::reference>);
	static_assert(std::is_reference_v<ctraits::reference>);
	static_assert(std::is_lvalue_reference_v<ctraits::reference>);
	static_assert(std::is_pointer_v<ctraits::pointer>);
	static_assert(std::is_object_v<ctraits::difference_type>);

	// forward_iterator_tag	// bidirectional_iterator_tag	//
	// random_access_iterator_tag
	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<traits::iterator_category, category>);
	static_assert(std::is_same_v<ctraits::iterator_category, category>);
	// static_assert(std::is_same_v<std::iterator_traits<typeT::reverse_iterator>::iterator_category,
	// category>);
	// static_assert(std::is_same_v<std::iterator_traits<typeT::const_reverse_iterator>::iterator_category,
	// category>);

	// no implementation differences / mistakes
	using type = Options<6>;
	using RowT = typename Board_Section::Row<type, 4>;
	static_assert(
		std::is_same_v<std::iterator_traits<RowT::iterator>::value_type, type>);
	static_assert(std::is_same_v<
				  std::iterator_traits<RowT::const_iterator>::value_type,
				  const type>);
	static_assert(std::is_const_v<
				  std::iterator_traits<RowT::const_iterator>::value_type>);
	// static_assert(std::is_same_v<std::iterator_traits<RowT::reverse_iterator>::value_type,
	// type>);
	// static_assert(std::is_same_v<std::iterator_traits<RowT::const_reverse_iterator>::value_type,
	// const type>);

	// no implementation differences / mistakes
	using diff = traits::difference_type;
	static_assert(std::is_same_v<ctraits::difference_type, diff>);
	// static_assert(std::is_same_v<std::iterator_traits<typeT::reverse_iterator>::difference_type,
	// diff>);
	// static_assert(std::is_same_v<std::iterator_traits<typeT::const_reverse_iterator>::difference_type,
	// diff>);
} // namespace RowIterator

TEST(Section_Iterator, RowIterator)
{
	// All iterator categories
	using typeT = typename Board_Section::Row<int, 2>;
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	EXPECT_NO_THROW(A.row(1).begin());
	EXPECT_NO_THROW(A.row(1).end());
	EXPECT_NO_THROW(Board<int>().row(0).cbegin());
	EXPECT_NO_THROW(A.row(1).cend());
	EXPECT_NO_THROW(A.row(1).rbegin());
	EXPECT_NO_THROW(A.row(1).rend());
	EXPECT_NO_THROW(A.row(1).crbegin());
	EXPECT_NO_THROW(A.row(1).crend());
	EXPECT_NO_THROW(cA.row(1).cbegin());
	EXPECT_NO_THROW(cA.row(1).cend());
	EXPECT_NO_THROW(cA.row(1).begin());
	EXPECT_NO_THROW(cA.row(1).end());
	EXPECT_NO_THROW(cA.row(1).rbegin());
	EXPECT_NO_THROW(cA.row(1).rend());
	EXPECT_NO_THROW([[maybe_unused]] typeT::iterator X(A.row(2).begin()));
	EXPECT_NO_THROW(
		[[maybe_unused]] typeT::const_iterator X(A.row(2).cbegin()));
	EXPECT_NO_THROW(
		[[maybe_unused]] typeT::const_iterator X(cA.row(2).cbegin()));
	// *r is dereferenceable
	[[maybe_unused]] int U {};
	EXPECT_NO_THROW(*A.row(1).begin());
	EXPECT_NO_THROW(*A.row(1).cbegin());
	EXPECT_NO_THROW(U = *A.row(1).rbegin());
	EXPECT_NO_THROW(U = *A.row(1).crbegin());
	EXPECT_NO_THROW(*(cA.row(1).begin()));
	EXPECT_NO_THROW(*cA.row(1).cbegin());
	EXPECT_NO_THROW(U = *cA.row(1).rbegin());
	// ++r is incrementable
	EXPECT_NO_THROW(++A.row(1).begin());
	EXPECT_NO_THROW(++A.row(1).cbegin());
	EXPECT_NO_THROW(++cA.row(1).cbegin());
	EXPECT_NO_THROW(++cA.row(1).begin());
	EXPECT_NO_THROW(++A.row(1).rbegin());
	EXPECT_NO_THROW(++A.row(1).crbegin());
}
TEST(Section_Iterator, RowInputIterator)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	Board<Options<4>, 2> Opt{};
	const Board<Options<4>, 2> cOpt{};

	// == !=
	[[maybe_unused]] bool U {};
	EXPECT_NO_THROW(A.row(0).begin() == A.row(0).end());
	EXPECT_NO_THROW(A.row(0).cbegin() == A.row(0).cend());
	EXPECT_NO_THROW(U = A.row(0).rbegin() == A.row(0).rend());
	EXPECT_NO_THROW(U = A.row(0).crbegin() == A.row(0).crend());
	EXPECT_NO_THROW(cA.row(0).cbegin() == cA.row(0).cend());
	EXPECT_NO_THROW(cA.row(0).begin() == cA.row(0).end());
	EXPECT_TRUE(A.row(0).begin() == A.row(0).begin());
	EXPECT_TRUE(cA.row(0).begin() == cA.row(0).begin());
	EXPECT_FALSE(A.row(0).begin() == A.row(0).end());
	EXPECT_TRUE(A.row(0).cbegin() == A.row(0).cbegin());
	EXPECT_FALSE(A.row(0).cbegin() == A.row(0).cend());
	EXPECT_TRUE(A.row(0).rbegin() == A.row(0).rbegin());
	EXPECT_FALSE(A.row(0).rbegin() == A.row(0).rend());
	EXPECT_TRUE(A.row(0).crbegin() == A.row(0).crbegin());
	EXPECT_FALSE(A.row(0).crbegin() == A.row(0).crend());
	EXPECT_NO_THROW(A.row(0).begin() != A.row(0).end());
	EXPECT_NO_THROW(A.row(0).cbegin() != A.row(0).cend());
	EXPECT_NO_THROW(cA.row(0).cbegin() != cA.row(0).cend());
	EXPECT_NO_THROW(cA.row(0).begin() != cA.row(0).end());
	EXPECT_NO_THROW(U = A.row(0).rbegin() != A.row(0).rend());
	EXPECT_NO_THROW(U = A.row(0).crbegin() != A.row(0).crend());
	EXPECT_TRUE(A.row(0).begin() != A.row(0).end());
	EXPECT_FALSE(A.row(0).begin() != A.row(0).begin());
	EXPECT_TRUE(A.row(0).cbegin() != ++A.row(0).cbegin());
	EXPECT_FALSE(A.row(0).begin() != A.row(0).begin());
	EXPECT_TRUE(cA.row(0).begin() != cA.row(0).end());
	EXPECT_TRUE(cA.row(0).cbegin() != ++cA.row(0).cbegin());
	EXPECT_TRUE(A.row(0).rbegin() != A.row(0).rend());
	EXPECT_FALSE(A.row(0).rbegin() != A.row(0).rbegin());
	EXPECT_TRUE(A.row(0).crbegin() != A.row(0).crend());
	EXPECT_FALSE(A.row(0).crbegin() != A.row(0).crbegin());
	// *i
	EXPECT_EQ(*A.row(0).begin(), 9);
	EXPECT_EQ(*cA.row(0).begin(), 9);
	EXPECT_EQ(9, *A.row(0).begin());
	EXPECT_FALSE(*A.row(0).begin() == 5);
	EXPECT_EQ(*A.row(0).cbegin(), 9);
	EXPECT_FALSE(*A.row(0).cbegin() == 5);
	EXPECT_EQ(*A.row(0).rbegin(), 3);
	EXPECT_FALSE(*A.row(0).rbegin() == 5);
	EXPECT_EQ(*A.row(0).crbegin(), 3);
	EXPECT_FALSE(*A.row(0).crbegin() == 5);
	EXPECT_EQ(*cA.row(0).begin(), 9);
	EXPECT_FALSE(*cA.row(0).begin() == 5);
	EXPECT_EQ(*cA.row(0).rbegin(), 3);
	EXPECT_FALSE(*cA.row(0).rbegin() == 5);
	// i->member
	static_assert(noexcept(Opt.row(0).begin()->size()));
	static_assert(noexcept(Opt.row(0).cbegin()->size()));
	static_assert(noexcept(cOpt.row(0).cbegin()->size()));
	static_assert(not(noexcept(Opt.row(0).rbegin()->size())));
	static_assert(not(noexcept(Opt.row(0).crbegin()->size())));
	EXPECT_NO_THROW(
		U =
			(Opt.row(0).begin()->size() ==
			 (*Opt.row(0).begin()).size())); // equivalent
	EXPECT_EQ(
		(Opt.row(0).begin()->size()),
		(*Opt.row(0).begin()).size()); // equivalent
	EXPECT_EQ(
		(cOpt.row(0).begin()->size()),
		(*cOpt.row(0).begin()).size()); // equivalent
	// ++i (return type: It&)	is incrementable
	EXPECT_EQ(*(++A.row(0).begin()), 1);
	EXPECT_EQ(*(++A.row(0).cbegin()), 1);
	EXPECT_EQ(*(++cA.row(0).cbegin()), 1);
	EXPECT_EQ(*(++cA.row(0).begin()), 1);
	EXPECT_EQ(*(++A.row(0).rbegin()), 2);
	EXPECT_EQ(*(++A.row(0).crbegin()), 2);
	// (void)i++ equivalent to (void)++i
	EXPECT_NO_THROW(A.row(0).begin()++);
	EXPECT_NO_THROW(A.row(0).cbegin()++);
	EXPECT_NO_THROW(cA.row(0).cbegin()++);
	EXPECT_NO_THROW(A.row(0).rbegin()++);
	EXPECT_NO_THROW(A.row(0).crbegin()++);
	// *i++
	EXPECT_EQ(*A.row(0).begin()++, 9);
	EXPECT_EQ(*A.row(0).cbegin()++, 9);
	EXPECT_EQ(*cA.row(0).cbegin()++, 9);
	EXPECT_EQ(*cA.row(0).begin()++, 9);
	EXPECT_EQ(*A.row(0).rbegin()++, 3);
	EXPECT_EQ(*A.row(0).crbegin()++, 3);
	// std::accumulate
	int total{};
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(0).cbegin(), A.row(0).cend(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(1).cbegin(), A.row(1).cend(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(2).cbegin(), A.row(2).cend(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(3).cbegin(), A.row(3).cend(), 0));
	EXPECT_EQ(total, 54);
	total = 0;

	EXPECT_NO_THROW(
		total = std::accumulate(A.row(0).begin(), A.row(0).end(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(1).begin(), A.row(1).end(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(2).begin(), A.row(2).end(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(3).begin(), A.row(3).end(), 0));
	EXPECT_EQ(total, 54);
	total = 0;

	EXPECT_NO_THROW(
		total = std::accumulate(A.row(0).crbegin(), A.row(0).crend(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(1).crbegin(), A.row(1).crend(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(2).crbegin(), A.row(2).crend(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(3).crbegin(), A.row(3).crend(), 0));
	EXPECT_EQ(total, 54);
	total = 0;

	EXPECT_NO_THROW(
		total = std::accumulate(A.row(0).rbegin(), A.row(0).rend(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(1).rbegin(), A.row(1).rend(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(2).rbegin(), A.row(2).rend(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.row(3).rbegin(), A.row(3).rend(), 0));
	EXPECT_EQ(total, 54);
}

TEST(Section_Iterator, RowOutputIterator)
{
	Board<int, 2> tmp{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";
	// *r = o (return-value not used)
	[[maybe_unused]] int U {};
	EXPECT_NO_THROW(*tmp.row(0).begin()); // pre-condition
	EXPECT_NO_THROW(*tmp.row(0).begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	EXPECT_NO_THROW(*(++tmp.row(0).begin()) = 4);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(U = *tmp.row(0).rbegin()); // pre-condition
	EXPECT_NO_THROW(*tmp.row(0).rbegin() = 9);
	EXPECT_EQ(tmp[0][3], 9);
	EXPECT_NO_THROW(*(++tmp.row(0).rbegin()) = 8);
	EXPECT_EQ(tmp[0][2], 8);
	EXPECT_NO_THROW(*(tmp.row(0).begin()++) = 2);
	EXPECT_EQ(tmp[0][0], 2);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(*(tmp.row(0).rbegin()++) = 7);
	EXPECT_EQ(tmp[0][3], 7);
	EXPECT_EQ(tmp[0][2], 8);
	EXPECT_NO_THROW(++(*tmp.row(0).begin() = 0)); // post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.row(0).rbegin() = 9));
	EXPECT_EQ(tmp[0][3], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r);
}

TEST(Section_Iterator, RowForwardIterator)
{
	using typeT = typename Board_Section::Row<int, 2>;
	static_assert(std::is_default_constructible_v<typeT::iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_iterator>);

	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	EXPECT_NO_THROW(typeT::iterator());
	EXPECT_NO_THROW(typeT::iterator() = A.row(2).begin());

	EXPECT_NO_THROW(A.row(0).begin()++);
	EXPECT_EQ(*A.row(0).begin()++, A[0][0]);
	EXPECT_EQ(*A.row(0).cbegin()++, A[0][0]);
	EXPECT_EQ(*A.row(0).rbegin()++, A[0][3]);
	EXPECT_EQ(*A.row(0).crbegin()++, A[0][3]);
	EXPECT_TRUE(A.row(0).begin() == A.row(0).begin());
	EXPECT_TRUE(++A.row(0).begin() == ++A.row(0).begin());
	auto r = A.row(0).begin();
	auto i = r;
	*r     = 0;
	EXPECT_EQ(*r, 0);
	EXPECT_EQ(*i, 0);
	EXPECT_EQ(*++r, 1);
	r = A.row(0).begin();
	i = r;
	EXPECT_EQ(*++r, *(++i));
}

TEST(Section_Iterator, RowBidirectionalIterator)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	// --i (return type: It&)	is decrementable
	EXPECT_NO_THROW(--A.end());
	EXPECT_NO_THROW(--A.cend());
	EXPECT_NO_THROW(--A.rend());
	EXPECT_NO_THROW(--A.crend());
	EXPECT_EQ(*(--A.row(0).end()), 3);
	EXPECT_EQ(*(--(--A.row(0).end())), 2);
	EXPECT_EQ(*(--A.row(0).cend()), 3);
	EXPECT_EQ(*(--(--A.row(0).cend())), 2);
	EXPECT_EQ(*(--A.row(0).rend()), 9);
	EXPECT_EQ(*(--(--A.row(0).rend())), 1);
	EXPECT_EQ(*(--A.row(0).crend()), 9);
	EXPECT_EQ(*(--(--A.row(0).crend())), 1);

	EXPECT_TRUE(--(++A.row(0).begin()) == A.row(0).begin());
	EXPECT_EQ(*(--(++A.row(0).begin())), *A.row(0).begin());
	EXPECT_TRUE(--(++A.row(0).cbegin()) == A.row(0).cbegin());
	EXPECT_TRUE(--(++A.row(0).rbegin()) == A.row(0).rbegin());
	EXPECT_TRUE(--(++A.row(0).crbegin()) == A.row(0).crbegin());
	// i--
	EXPECT_NO_THROW(A.row(0).end()--);
	EXPECT_NO_THROW(A.row(0).cend()--);
	EXPECT_NO_THROW(A.row(0).rend()--);
	EXPECT_NO_THROW(A.row(0).crend()--);
	// *i--
	EXPECT_TRUE(A.row(0).end()-- == A.row(0).end());
	EXPECT_EQ(*(--A.row(0).end())--, 3);
	EXPECT_TRUE(A.row(0).cend()-- == A.row(0).cend());
	EXPECT_EQ(*(--A.row(0).cend())--, 3);
	EXPECT_TRUE(A.row(0).rend()-- == A.row(0).rend());
	EXPECT_EQ(*(--A.row(0).rend())--, 9);
	EXPECT_TRUE(A.row(0).crend()-- == A.row(0).crend());
	EXPECT_EQ(*(--A.row(0).crend())--, 9);
}

TEST(Section_Iterator, RowRandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff =
		typename std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	[[maybe_unused]] auto U = A.row(0).rbegin();
	[[maybe_unused]] auto V = A.row(0).crbegin();
	[[maybe_unused]] int W {};
	EXPECT_NO_THROW(A.row(0).begin() += 2);
	EXPECT_NO_THROW(A.row(0).cbegin() += 2);
	EXPECT_NO_THROW(A.row(0).rbegin() += 2);
	EXPECT_NO_THROW(A.row(0).crbegin() += 2);
	EXPECT_NO_THROW(A.row(0).end() += (-2));
	EXPECT_EQ(*(A.row(0).begin() += 2), 2); // returns a reference to the
											// changed object
	EXPECT_EQ(*(A.row(0).cbegin() += 2), 2);
	EXPECT_EQ(*(A.row(0).rbegin() += 1), 2);
	EXPECT_EQ(*(A.row(0).crbegin() += 1), 2);
	EXPECT_EQ(*(A.row(0).end() += (-2)), 2);
	EXPECT_EQ(*(A.row(0).cbegin() += 0), 9);
	EXPECT_FALSE(*(A.row(0).cbegin() += 2) == 5);
	EXPECT_FALSE(*(A.row(0).cbegin() += 0) == 5);

	EXPECT_NO_THROW(A.row(0).begin() + 2); // returns a new iterator object
	EXPECT_NO_THROW(A.row(0).cbegin() + 2);
	EXPECT_NO_THROW(U = A.row(0).rbegin() + 1);
	EXPECT_NO_THROW(V = A.row(0).crbegin() + 2);
	EXPECT_NO_THROW(A.row(0).cend() + (-3));

	EXPECT_NO_THROW(2 + (Board<int, 2>().row(0).begin()));
	EXPECT_NO_THROW(2 + A.row(0).cbegin());
	EXPECT_NO_THROW(2 + A.row(0).begin());
	EXPECT_NO_THROW(U = 2 + A.row(0).rbegin());
	EXPECT_NO_THROW(V = 2 + A.row(0).crbegin());
	EXPECT_NO_THROW(U = (-2) + A.row(0).rend());

	EXPECT_TRUE((A.row(0).begin() + 1) == (++A.row(0).begin()));
	EXPECT_TRUE((A.row(0).cbegin() + 1) == (++A.row(0).cbegin()));
	EXPECT_TRUE((A.row(0).rbegin() + 1) == (++A.row(0).rbegin()));
	EXPECT_TRUE((A.row(0).crbegin() + 1) == (++A.row(0).crbegin()));
	EXPECT_TRUE((A.row(0).begin() + 3) == (A.row(0).begin() += 3));
	EXPECT_TRUE((A.row(0).cbegin() + 1) == (A.row(0).cbegin() += 1));
	EXPECT_TRUE((A.row(0).rbegin() + 2) == (A.row(0).rbegin() += 2));
	EXPECT_TRUE((A.row(0).crbegin() + 3) == (A.row(0).crbegin() += 3));
	EXPECT_TRUE((A.row(0).end() + (-2)) == (A.row(0).begin() += 2));
	EXPECT_FALSE((A.row(0).begin() + 3) == (++A.row(0).begin()));

	EXPECT_TRUE((A.row(0).begin() + 2) == (2 + A.row(0).begin()));

	EXPECT_NO_THROW(A.row(0).end() -= 2);
	EXPECT_NO_THROW(A.row(0).cend() -= 2);
	EXPECT_NO_THROW(A.row(0).rend() -= 2);
	EXPECT_NO_THROW(A.row(0).crend() -= 2);
	EXPECT_NO_THROW(A.row(0).crbegin() -= (-3));
	EXPECT_TRUE((A.row(0).end() -= 1) == (A.row(0).begin() += 3));
	EXPECT_TRUE((A.row(0).cend() -= 1) == (A.row(0).cbegin() += 3));
	EXPECT_TRUE((A.row(0).rend() -= 1) == (A.row(0).rbegin() += 3));
	EXPECT_TRUE((A.row(0).crend() -= 1) == (A.row(0).crbegin() += 3));
	EXPECT_TRUE((A.row(0).begin() -= (-1)) == (A.row(0).begin() += 1));

	EXPECT_NO_THROW(A.row(0).end() - 2);
	EXPECT_NO_THROW(A.row(0).cend() - 2);
	EXPECT_NO_THROW(U = A.row(0).rend() - 1);
	EXPECT_NO_THROW(V = A.row(0).crend() - 2);
	EXPECT_NO_THROW(A.row(0).begin() - (-2));

	EXPECT_NO_THROW(A.row(0).end() - A.row(0).begin());
	EXPECT_NO_THROW(A.row(0).cend() - A.row(0).cbegin());
	EXPECT_NO_THROW(W = A.row(0).rend() - A.row(0).rbegin());
	EXPECT_NO_THROW(W = A.row(0).crend() - A.row(0).crbegin());
	EXPECT_TRUE((A.row(0).end() - A.row(0).begin()) == A.row(0).size());
	EXPECT_TRUE((A.row(0).begin() - A.row(0).end()) == -A.row(0).size());
	EXPECT_TRUE((A.row(0).begin() - A.row(0).end()) == -A.row(0).size());
	EXPECT_TRUE((A.row(0).cend() - A.row(0).cbegin()) == A.row(0).size());

	EXPECT_NO_THROW(A.row(0).begin()[2]); // return value_type reference
	EXPECT_NO_THROW(A.row(0).cbegin()[2]);
	EXPECT_NO_THROW(W = A.row(0).rbegin()[2]);
	EXPECT_NO_THROW(W = A.row(0).crbegin()[2]);
	EXPECT_EQ(A.row(0).begin()[2], 2);
	EXPECT_EQ(A.row(0).cbegin()[3], 3);
	EXPECT_EQ(A.row(0).rend()[-3], *(A.row(0).rend() - 3));
	EXPECT_EQ(A.row(0).crbegin()[2], 1);

	EXPECT_NO_THROW(A.row(0).begin() < A.row(0).begin());
	EXPECT_NO_THROW(A.row(0).cbegin() < A.row(0).cbegin());
	EXPECT_NO_THROW(A.row(0).begin() > A.row(0).begin());
	EXPECT_NO_THROW(A.row(0).cbegin() > A.row(0).cbegin());
	EXPECT_NO_THROW(A.row(0).begin() >= A.row(0).begin());
	EXPECT_NO_THROW(A.row(0).cbegin() >= A.row(0).cbegin());
	EXPECT_NO_THROW(A.row(0).begin() <= A.row(0).begin());
	EXPECT_NO_THROW(A.row(0).cbegin() <= A.row(0).cbegin());
	EXPECT_FALSE(A.row(0).cbegin() < A.row(0).cbegin());
	EXPECT_LT(A.row(0).cbegin(), A.row(0).cend());
	EXPECT_FALSE(A.row(0).cend() < A.row(0).cbegin());

	// shuffle(start, stop) requires a random iterator
	ASSERT_EQ(A, cA);
	std::random_device rd;   // non-deterministic generator
	std::mt19937 urng(rd()); // to seed mersenne twister
	EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
	EXPECT_NE(A, cA);

	EXPECT_TRUE(A.row(0).begin() < ++A.row(0).begin()); // const_iterator should
														// move with iterator
	const auto itr = A.row(1).begin();
	*itr           = 9;
	EXPECT_TRUE(itr < ++A.row(1).begin());
}

namespace ColIterator
{
	using typeT = typename Board_Section::Col<int, 3>;
	// all iterator categories
	static_assert(std::is_object_v<typeT::iterator>);
	static_assert(std::is_object_v<typeT::const_iterator>);
	static_assert(!std::is_const_v<typeT::iterator>);
	static_assert(!std::is_const_v<typeT::const_iterator>);
	// CopyConstructible
	static_assert(std::is_copy_constructible_v<typeT::iterator>);
	static_assert(std::is_copy_constructible_v<typeT::const_iterator>);
	// CopyAssignable
	static_assert(std::is_copy_assignable_v<typeT::iterator>);
	static_assert(std::is_copy_assignable_v<typeT::const_iterator>);
	// Destructible
	static_assert(std::is_destructible_v<typeT::iterator>);
	static_assert(std::is_destructible_v<typeT::const_iterator>);
	// Swappable (lvalues)
	// static_assert(std::is_swappable_v<typeT::iterator>);
	// static_assert(std::is_swappable_v<typeT::const_iterator>);

	// std::iterator_traits<It> has member typedefs:
	// value_type, difference_type, reference, pointer, iterator_category
	using traits = typename std::iterator_traits<typeT::iterator>;
	static_assert(std::is_object_v<traits::iterator_category>);
	static_assert(std::is_object_v<traits::value_type>);
	static_assert(!std::is_object_v<traits::reference>);
	static_assert(std::is_reference_v<traits::reference>);
	static_assert(std::is_lvalue_reference_v<traits::reference>);
	static_assert(std::is_pointer_v<traits::pointer>);
	static_assert(std::is_object_v<traits::difference_type>);

	using ctraits = typename std::iterator_traits<typeT::const_iterator>;
	static_assert(std::is_object_v<ctraits::iterator_category>);
	static_assert(std::is_object_v<ctraits::value_type>);
	static_assert(!std::is_object_v<ctraits::reference>);
	static_assert(std::is_reference_v<ctraits::reference>);
	static_assert(std::is_lvalue_reference_v<ctraits::reference>);
	static_assert(std::is_pointer_v<ctraits::pointer>);
	static_assert(std::is_object_v<ctraits::difference_type>);

	// forward_iterator_tag	// bidirectional_iterator_tag	//
	// random_access_iterator_tag
	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<traits::iterator_category, category>);
	static_assert(std::is_same_v<ctraits::iterator_category, category>);

	// no implementation differences / mistakes
	using type = Options<6>;
	using ColT = typename Board_Section::Col<type, 4>;
	static_assert(
		std::is_same_v<std::iterator_traits<ColT::iterator>::value_type, type>);
	static_assert(std::is_same_v<
				  std::iterator_traits<ColT::const_iterator>::value_type,
				  const type>);
	static_assert(std::is_const_v<
				  std::iterator_traits<ColT::const_iterator>::value_type>);

	// no implementation differences / mistakes
	using diff = traits::difference_type;
	static_assert(std::is_same_v<ctraits::difference_type, diff>);
} // namespace ColIterator

TEST(Section_Iterator, ColIterator)
{
	// All iterator categories
	using typeT = typename Board_Section::Col<int, 2>;
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	[[maybe_unused]] int U {};
	EXPECT_NO_THROW(A.col(1).begin());
	EXPECT_NO_THROW(A.col(1).end());
	EXPECT_NO_THROW(Board<int>().col(0).cbegin());
	EXPECT_NO_THROW(A.col(1).cend());
	EXPECT_NO_THROW(A.col(1).rbegin());
	EXPECT_NO_THROW(A.col(1).rend());
	EXPECT_NO_THROW(A.col(1).crbegin());
	EXPECT_NO_THROW(A.col(1).crend());
	EXPECT_NO_THROW(cA.col(1).cbegin());
	EXPECT_NO_THROW(cA.col(1).cend());
	EXPECT_NO_THROW(cA.col(1).begin());
	EXPECT_NO_THROW(cA.col(1).end());
	EXPECT_NO_THROW(cA.col(1).rbegin());
	EXPECT_NO_THROW(cA.col(1).rend());
	EXPECT_NO_THROW([[maybe_unused]] typeT::iterator X(A.col(2).begin()));
	EXPECT_NO_THROW(
		[[maybe_unused]] typeT::const_iterator X(A.col(2).cbegin()));
	EXPECT_NO_THROW(
		[[maybe_unused]] typeT::const_iterator X(cA.col(2).cbegin()));
	// *r is dereferenceable
	EXPECT_NO_THROW(*A.col(1).begin());
	EXPECT_NO_THROW(*A.col(1).cbegin());
	EXPECT_NO_THROW(U = *A.col(1).rbegin());
	EXPECT_NO_THROW(U = *A.col(1).crbegin());
	EXPECT_NO_THROW(*(cA.col(1).begin()));
	EXPECT_NO_THROW(*cA.col(1).cbegin());
	EXPECT_NO_THROW(U = *cA.col(1).rbegin());
	// ++r is incrementable
	EXPECT_NO_THROW(++A.col(1).begin());
	EXPECT_NO_THROW(++A.col(1).cbegin());
	EXPECT_NO_THROW(++cA.col(1).cbegin());
	EXPECT_NO_THROW(++cA.col(1).begin());
	EXPECT_NO_THROW(++A.col(1).rbegin());
	EXPECT_NO_THROW(++A.col(1).crbegin());
}

TEST(Section_Iterator, ColInputIterator)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	Board<Options<4>, 2> Opt{};
	const Board<Options<4>, 2> cOpt{};

	// == !=
	[[maybe_unused]] bool U {};
	EXPECT_NO_THROW(A.col(0).begin() == A.col(0).end());
	EXPECT_NO_THROW(A.col(0).cbegin() == A.col(0).cend());
	EXPECT_NO_THROW(U = A.col(0).rbegin() == A.col(0).rend());
	EXPECT_NO_THROW(U = A.col(0).crbegin() == A.col(0).crend());
	EXPECT_NO_THROW(cA.col(0).cbegin() == cA.col(0).cend());
	EXPECT_NO_THROW(cA.col(0).begin() == cA.col(0).end());
	EXPECT_TRUE(A.col(0).begin() == A.col(0).begin());
	EXPECT_TRUE(cA.col(0).begin() == cA.col(0).begin());
	EXPECT_FALSE(A.col(0).begin() == A.col(0).end());
	EXPECT_TRUE(A.col(0).cbegin() == A.col(0).cbegin());
	EXPECT_FALSE(A.col(0).cbegin() == A.col(0).cend());
	EXPECT_TRUE(A.col(0).rbegin() == A.col(0).rbegin());
	EXPECT_FALSE(A.col(0).rbegin() == A.col(0).rend());
	EXPECT_TRUE(A.col(0).crbegin() == A.col(0).crbegin());
	EXPECT_FALSE(A.col(0).crbegin() == A.col(0).crend());
	EXPECT_NO_THROW(A.col(0).begin() != A.col(0).end());
	EXPECT_NO_THROW(A.col(0).cbegin() != A.col(0).cend());
	EXPECT_NO_THROW(cA.col(0).cbegin() != cA.col(0).cend());
	EXPECT_NO_THROW(cA.col(0).begin() != cA.col(0).end());
	EXPECT_NO_THROW(U = A.col(0).rbegin() != A.col(0).rend());
	EXPECT_NO_THROW(U = A.col(0).crbegin() != A.col(0).crend());
	EXPECT_TRUE(A.col(0).begin() != A.col(0).end());
	EXPECT_FALSE(A.col(0).begin() != A.col(0).begin());
	EXPECT_TRUE(A.col(0).cbegin() != ++A.col(0).cbegin());
	EXPECT_FALSE(A.col(0).begin() != A.col(0).begin());
	EXPECT_TRUE(cA.col(0).begin() != cA.col(0).end());
	EXPECT_TRUE(cA.col(0).cbegin() != ++cA.col(0).cbegin());
	EXPECT_TRUE(A.col(0).rbegin() != A.col(0).rend());
	EXPECT_TRUE(cA.col(0).rbegin() == cA.col(0).rbegin());
	EXPECT_FALSE(A.col(0).rbegin() != A.col(0).rbegin());
	EXPECT_TRUE(A.col(0).crbegin() != A.col(0).crend());
	EXPECT_FALSE(A.col(0).crbegin() != A.col(0).crbegin());
	// *i
	EXPECT_EQ(*A.col(0).begin(), 9);
	EXPECT_EQ(*A.col(1).begin(), 1);
	EXPECT_EQ(*cA.col(0).begin(), 9);
	EXPECT_EQ(*cA.col(1).begin(), 1);
	EXPECT_EQ(9, *A.col(0).begin());
	EXPECT_FALSE(*A.col(0).begin() == 5);
	EXPECT_EQ(*A.col(0).cbegin(), 9); // == *A.row(0).cbegin()
	EXPECT_FALSE(*A.col(0).cbegin() == 5);
	EXPECT_EQ(*A.col(1).cbegin(), 1); // == *A.row(0).cbegin()
	EXPECT_EQ(*A.col(0).rbegin(), 12);
	EXPECT_FALSE(*A.col(0).rbegin() == 5);
	EXPECT_EQ(*A.col(0).crbegin(), 12);
	EXPECT_FALSE(*A.col(0).crbegin() == 5);
	EXPECT_EQ(*cA.col(0).begin(), 9);
	EXPECT_FALSE(*cA.col(0).begin() == 5);
	EXPECT_EQ(*cA.col(0).rbegin(), 12);
	EXPECT_FALSE(*cA.col(0).rbegin() == 5);
	// i->member
	static_assert(noexcept(Opt.col(0).begin()->size()));
	static_assert(noexcept(Opt.col(0).cbegin()->size()));
	static_assert(noexcept(cOpt.col(0).cbegin()->size()));
	static_assert(not(noexcept(Opt.col(0).rbegin()->size())));
	static_assert(not(noexcept(Opt.col(0).crbegin()->size())));
	EXPECT_NO_THROW(
		U =
			(Opt.col(0).begin()->size() ==
			 (*Opt.col(0).begin()).size())); // equivalent
	EXPECT_EQ(
		(Opt.col(0).begin()->size()),
		(*Opt.col(0).begin()).size()); // equivalent
	EXPECT_EQ(
		(cOpt.col(0).begin()->size()),
		(*cOpt.col(0).begin()).size()); // equivalent
	// ++i (return type: It&)	is incrementable
	EXPECT_EQ(*(++A.col(0).begin()), 4);
	EXPECT_EQ(*(++A.col(0).cbegin()), 4);
	EXPECT_EQ(*(++cA.col(0).cbegin()), 4);
	EXPECT_EQ(*(++cA.col(0).begin()), 4);
	EXPECT_EQ(*(++A.col(0).rbegin()), 8);
	EXPECT_EQ(*(++A.col(0).crbegin()), 8);
	// (void)i++ equivalent to (void)++i
	EXPECT_NO_THROW(A.col(0).begin()++);
	EXPECT_NO_THROW(A.col(0).cbegin()++);
	EXPECT_NO_THROW(cA.col(0).cbegin()++);
	EXPECT_NO_THROW(A.col(0).rbegin()++);
	EXPECT_NO_THROW(A.col(0).crbegin()++);
	// *i++
	EXPECT_EQ(*A.col(0).begin()++, 9);
	EXPECT_EQ(*A.col(0).cbegin()++, 9);
	EXPECT_EQ(*cA.col(0).cbegin()++, 9);
	EXPECT_EQ(*cA.col(0).begin()++, 9);
	EXPECT_EQ(*A.col(0).rbegin()++, 12);
	EXPECT_EQ(*A.col(0).crbegin()++, 12);
	// std::accumulate
	int total{};
	EXPECT_NO_THROW(
		total = std::accumulate(A.col(0).cbegin(), A.col(0).cend(), 0));
	EXPECT_EQ(total, 33);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.col(1).cbegin(), A.col(1).cend(), 0));
	EXPECT_EQ(total, 28);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.col(2).cbegin(), A.col(2).cend(), 0));
	EXPECT_EQ(total, 32);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.col(3).cbegin(), A.col(3).cend(), 0));
	EXPECT_EQ(total, 36);
	total = 0;

	EXPECT_NO_THROW(
		total = std::accumulate(A.col(2).begin(), A.col(2).end(), 0));
	EXPECT_EQ(total, 32); // 2+6+10+14=32
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.col(2).cbegin(), A.col(2).cend(), 0));
	EXPECT_EQ(total, 32);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.col(2).rbegin(), A.col(2).rend(), 0));
	EXPECT_EQ(total, 32);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(cA.col(2).rbegin(), cA.col(2).rend(), 0));
	EXPECT_EQ(total, 32);
}

TEST(Section_Iterator, ColOutputIterator)
{
	Board<int, 2> tmp{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";
	// *r = o (return-value not used)
	[[maybe_unused]] int U {};
	EXPECT_NO_THROW(*tmp.col(0).begin()); // pre-condition
	EXPECT_NO_THROW(*tmp.col(0).begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	EXPECT_NO_THROW(*(++tmp.col(0).begin()) = 4);
	EXPECT_EQ(tmp[1][0], 4);
	EXPECT_NO_THROW(U = *tmp.col(0).rbegin()); // pre-condition
	EXPECT_NO_THROW(*tmp.col(0).rbegin() = 9);
	EXPECT_EQ(tmp[3][0], 9);
	EXPECT_NO_THROW(*(++tmp.col(0).rbegin()) = 8);
	EXPECT_EQ(tmp[2][0], 8);
	EXPECT_NO_THROW(*(tmp.col(0).begin()++) = 2);
	EXPECT_EQ(tmp[0][0], 2);
	EXPECT_EQ(tmp[1][0], 4);
	EXPECT_NO_THROW(*(tmp.col(0).rbegin()++) = 7);
	EXPECT_EQ(tmp[3][0], 7);
	EXPECT_EQ(tmp[2][0], 8);
	EXPECT_NO_THROW(++(*tmp.col(0).begin() = 0)); // post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.col(0).rbegin() = 9));
	EXPECT_EQ(tmp[3][0], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r);
}

TEST(Section_Iterator, ColForwardIterator)
{
	using typeT = typename Board_Section::Col<int, 2>;
	static_assert(std::is_default_constructible_v<typeT::iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_iterator>);

	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	EXPECT_NO_THROW(typeT::iterator());
	EXPECT_NO_THROW(typeT::iterator() = A.col(2).begin());

	EXPECT_NO_THROW(A.col(0).begin()++);
	EXPECT_EQ(*A.col(0).begin()++, A[0][0]);
	EXPECT_EQ(*A.col(0).cbegin()++, A[0][0]);
	EXPECT_EQ(*A.col(0).rbegin()++, A[3][0]);
	EXPECT_EQ(*A.col(0).crbegin()++, A[3][0]);
	EXPECT_TRUE(A.col(0).begin() == A.col(0).begin());
	EXPECT_TRUE(++A.col(0).begin() == ++A.col(0).begin());
	auto r = A.col(0).begin();
	auto i = r;
	*r     = 0;
	EXPECT_EQ(*r, 0);
	EXPECT_EQ(*i, 0);
	EXPECT_EQ(*++r, 4);
	EXPECT_EQ(*r, *(++i));
}

TEST(Section_Iterator, ColBidirectionalIterator)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	// --i (return type: It&)	is decrementable
	EXPECT_NO_THROW(--A.end());
	EXPECT_NO_THROW(--A.cend());
	EXPECT_NO_THROW(--A.rend());
	EXPECT_NO_THROW(--A.crend());
	EXPECT_EQ(*(--A.col(0).end()), 12);
	EXPECT_EQ(*(--(--A.col(0).end())), 8);
	EXPECT_EQ(*(--A.col(0).cend()), 12);
	EXPECT_EQ(*(--(--A.col(0).cend())), 8);
	EXPECT_EQ(*(--A.col(0).rend()), 9);
	EXPECT_EQ(*(--(--A.col(0).rend())), 4);
	EXPECT_EQ(*(--A.col(0).crend()), 9);
	EXPECT_EQ(*(--(--A.col(0).crend())), 4);

	EXPECT_TRUE(--(++A.col(0).begin()) == A.col(0).begin());
	EXPECT_EQ(*(--(++A.col(0).begin())), *A.col(0).begin());
	EXPECT_TRUE(--(++A.col(0).cbegin()) == A.col(0).cbegin());
	EXPECT_TRUE(--(++A.col(0).rbegin()) == A.col(0).rbegin());
	EXPECT_TRUE(--(++A.col(0).crbegin()) == A.col(0).crbegin());
	// i--
	EXPECT_NO_THROW(A.col(0).end()--);
	EXPECT_NO_THROW(A.col(0).cend()--);
	EXPECT_NO_THROW(A.col(0).rend()--);
	EXPECT_NO_THROW(A.col(0).crend()--);
	// *i--
	EXPECT_TRUE(A.col(0).end()-- == A.col(0).end());
	EXPECT_EQ(*(--A.col(0).end())--, 12);
	EXPECT_TRUE(A.col(0).cend()-- == A.col(0).cend());
	EXPECT_EQ(*(--A.col(0).cend())--, 12);
	EXPECT_TRUE(A.col(0).rend()-- == A.col(0).rend());
	EXPECT_EQ(*(--A.col(0).rend())--, 9);
	EXPECT_TRUE(A.col(0).crend()-- == A.col(0).crend());
	EXPECT_EQ(*(--A.col(0).crend())--, 9);
}

TEST(Section_Iterator, ColRandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff  = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	[[maybe_unused]] auto U = A.col(0).rbegin();
	[[maybe_unused]] auto V = A.col(0).crbegin();
	[[maybe_unused]] int W {};
	EXPECT_NO_THROW(A.col(0).begin() += 2);
	EXPECT_NO_THROW(A.col(0).cbegin() += 2);
	EXPECT_NO_THROW(A.col(0).rbegin() += 2);
	EXPECT_NO_THROW(A.col(0).crbegin() += 2);
	EXPECT_NO_THROW(A.col(0).end() += (-2));
	EXPECT_EQ(*(A.col(0).begin() += 2), 8); // returns a reference to the
											// changed object
	EXPECT_EQ(*(A.col(0).cbegin() += 2), 8);
	EXPECT_EQ(*(A.col(0).rbegin() += 1), 8);
	EXPECT_EQ(*(A.col(0).crbegin() += 1), 8);
	EXPECT_EQ(*(A.col(0).end() += (-2)), 8);
	EXPECT_EQ(*(A.col(0).cbegin() += 0), 9);
	EXPECT_FALSE(*(A.col(0).cbegin() += 2) == 5);
	EXPECT_FALSE(*(A.col(0).cbegin() += 0) == 5);

	EXPECT_NO_THROW(A.col(0).begin() + 2); // returns a new iterator object
	EXPECT_NO_THROW(A.col(0).cbegin() + 2);
	EXPECT_NO_THROW(U = A.col(0).rbegin() + 1);
	EXPECT_NO_THROW(V = A.col(0).crbegin() + 2);
	EXPECT_NO_THROW(A.col(0).cend() + (-3));

	EXPECT_NO_THROW(2 + (Board<int, 2>().col(0).begin()));
	EXPECT_NO_THROW(2 + A.col(0).cbegin());
	EXPECT_NO_THROW(U = 2 + A.col(0).rbegin());
	EXPECT_NO_THROW(V = 2 + A.col(0).crbegin());
	EXPECT_NO_THROW(U = (-2) + A.col(0).rend());

	EXPECT_TRUE((A.col(0).begin() + 1) == (++A.col(0).begin()));
	EXPECT_TRUE((A.col(0).cbegin() + 1) == (++A.col(0).cbegin()));
	EXPECT_TRUE((A.col(0).rbegin() + 1) == (++A.col(0).rbegin()));
	EXPECT_TRUE((A.col(0).crbegin() + 1) == (++A.col(0).crbegin()));
	EXPECT_TRUE((A.col(0).begin() + 3) == (A.col(0).begin() += 3));
	EXPECT_TRUE((A.col(0).cbegin() + 1) == (A.col(0).cbegin() += 1));
	EXPECT_TRUE((A.col(0).rbegin() + 2) == (A.col(0).rbegin() += 2));
	EXPECT_TRUE((A.col(0).crbegin() + 3) == (A.col(0).crbegin() += 3));
	EXPECT_TRUE((A.col(0).end() + (-2)) == (A.col(0).begin() += 2));
	EXPECT_FALSE((A.col(0).begin() + 3) == (++A.col(0).begin()));

	EXPECT_TRUE((A.col(0).begin() + 2) == (2 + A.col(0).begin()));

	EXPECT_NO_THROW(A.col(0).end() -= 2);
	EXPECT_NO_THROW(A.col(0).cend() -= 2);
	EXPECT_NO_THROW(A.col(0).rend() -= 2);
	EXPECT_NO_THROW(A.col(0).crend() -= 2);
	EXPECT_NO_THROW(A.col(0).crbegin() -= (-3));
	EXPECT_TRUE((A.col(0).end() -= 1) == (A.col(0).begin() += 3));
	EXPECT_TRUE((A.col(0).cend() -= 1) == (A.col(0).cbegin() += 3));
	EXPECT_TRUE((A.col(0).rend() -= 1) == (A.col(0).rbegin() += 3));
	EXPECT_TRUE((A.col(0).crend() -= 1) == (A.col(0).crbegin() += 3));
	EXPECT_TRUE((A.col(0).begin() -= (-1)) == (A.col(0).begin() += 1));

	EXPECT_NO_THROW(A.col(0).end() - 2);
	EXPECT_NO_THROW(A.col(0).cend() - 2);
	EXPECT_NO_THROW(U = A.col(0).rend() - 1);
	EXPECT_NO_THROW(V = A.col(0).crend() - 2);
	EXPECT_NO_THROW(A.col(0).begin() - (-2));

	EXPECT_NO_THROW(A.col(0).end() - A.col(0).begin());
	EXPECT_NO_THROW(A.col(0).cend() - A.col(0).cbegin());
	EXPECT_NO_THROW(W = A.col(0).rend() - A.col(0).rbegin());
	EXPECT_NO_THROW(W = A.col(0).crend() - A.col(0).crbegin());
	EXPECT_TRUE((A.col(0).end() - A.col(0).begin()) == A.col(0).size());
	EXPECT_TRUE((A.col(0).begin() - A.col(0).end()) == -A.col(0).size());
	EXPECT_TRUE((A.col(0).begin() - A.col(0).end()) == -A.col(0).size());
	EXPECT_TRUE((A.col(0).cend() - A.col(0).cbegin()) == A.col(0).size());

	EXPECT_NO_THROW(A.col(0).begin()[2]); // return value_type reference
	EXPECT_NO_THROW(A.col(0).cbegin()[2]);
	EXPECT_NO_THROW(W = A.col(0).rbegin()[2]);
	EXPECT_NO_THROW(W = A.col(0).crbegin()[2]);
	EXPECT_EQ(A.col(0).begin()[2], 8);
	EXPECT_EQ(A.col(0).cbegin()[3], 12);
	EXPECT_EQ(A.col(0).rend()[-3], *(A.col(0).rend() - 3));
	EXPECT_EQ(A.col(0).crbegin()[2], 4);

	EXPECT_NO_THROW(A.col(0).begin() < A.col(0).begin());
	EXPECT_NO_THROW(A.col(0).cbegin() < A.col(0).cbegin());
	EXPECT_NO_THROW(A.col(0).begin() > A.col(0).begin());
	EXPECT_NO_THROW(A.col(0).cbegin() > A.col(0).cbegin());
	EXPECT_NO_THROW(A.col(0).begin() >= A.col(0).begin());
	EXPECT_NO_THROW(A.col(0).cbegin() >= A.col(0).cbegin());
	EXPECT_NO_THROW(A.col(0).begin() <= A.col(0).begin());
	EXPECT_NO_THROW(A.col(0).cbegin() <= A.col(0).cbegin());
	EXPECT_FALSE(A.col(0).cbegin() < A.col(0).cbegin());
	EXPECT_LT(A.col(0).cbegin(), A.col(0).cend());
	EXPECT_FALSE(A.col(0).cend() < A.col(0).cbegin());

	// shuffle(start, stop) requires a random iterator
	ASSERT_EQ(A, cA);
	std::random_device rd;   // non-deterministic generator
	std::mt19937 urng(rd()); // to seed mersenne twister
	EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
	EXPECT_NE(A, cA);
}

namespace BlockIterator
{
	using typeT = typename Board_Section::Block<int, 3>;
	// all iterator categories
	static_assert(std::is_object_v<typeT::iterator>);
	static_assert(std::is_object_v<typeT::const_iterator>);
	static_assert(!std::is_const_v<typeT::iterator>);
	static_assert(!std::is_const_v<typeT::const_iterator>);
	// CopyConstructible
	static_assert(std::is_copy_constructible_v<typeT::iterator>);
	static_assert(std::is_copy_constructible_v<typeT::const_iterator>);
	// CopyAssignable
	static_assert(std::is_copy_assignable_v<typeT::iterator>);
	static_assert(std::is_copy_assignable_v<typeT::const_iterator>);
	// Destructible
	static_assert(std::is_destructible_v<typeT::iterator>);
	static_assert(std::is_destructible_v<typeT::const_iterator>);
	// Swappable (lvalues)
	// static_assert(std::is_swappable_v<typeT::iterator>);
	// static_assert(std::is_swappable_v<typeT::const_iterator>);

	// std::iterator_traits<It> has member typedefs
	// value_type, difference_type, reference, pointer, iterator_category
	using traits = typename std::iterator_traits<typeT::iterator>;
	static_assert(std::is_object_v<traits::iterator_category>);
	static_assert(std::is_object_v<traits::value_type>);
	static_assert(!std::is_object_v<traits::reference>);
	static_assert(std::is_reference_v<traits::reference>);
	static_assert(std::is_lvalue_reference_v<traits::reference>);
	static_assert(std::is_pointer_v<traits::pointer>);
	static_assert(std::is_object_v<traits::difference_type>);

	using ctraits = typename std::iterator_traits<typeT::const_iterator>;
	static_assert(std::is_object_v<ctraits::iterator_category>);
	static_assert(std::is_object_v<ctraits::value_type>);
	static_assert(!std::is_object_v<ctraits::reference>);
	static_assert(std::is_reference_v<ctraits::reference>);
	static_assert(std::is_lvalue_reference_v<ctraits::reference>);
	static_assert(std::is_pointer_v<ctraits::pointer>);
	static_assert(std::is_object_v<ctraits::difference_type>);

	// forward_iterator_tag	// bidirectional_iterator_tag	//
	// random_access_iterator_tag
	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<traits::iterator_category, category>);
	static_assert(std::is_same_v<ctraits::iterator_category, category>);

	// no implementation differences / mistakes
	using type = Options<6>;
	using ColT = typename Board_Section::Col<type, 4>;
	static_assert(
		std::is_same_v<std::iterator_traits<ColT::iterator>::value_type, type>);
	static_assert(std::is_same_v<
				  std::iterator_traits<ColT::const_iterator>::value_type,
				  const type>);
	static_assert(std::is_const_v<
				  std::iterator_traits<ColT::const_iterator>::value_type>);

	// no implementation differences / mistakes
	using diff = traits::difference_type;
	static_assert(std::is_same_v<ctraits::difference_type, diff>);
} // namespace BlockIterator

TEST(Section_Iterator, BlockIterator)
{
	// All iterator categories
	using typeT = typename Board_Section::Block<int, 2>;
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	EXPECT_NO_THROW(A.block(1).begin());
	EXPECT_NO_THROW(A.block(1).end());
	EXPECT_NO_THROW(Board<int>().block(0).cbegin());
	EXPECT_NO_THROW(A.block(1).cend());
	EXPECT_NO_THROW(A.block(1).rbegin());
	EXPECT_NO_THROW(A.block(1).rend());
	EXPECT_NO_THROW(A.block(1).crbegin());
	EXPECT_NO_THROW(A.block(1).crend());
	EXPECT_NO_THROW(cA.block(1).cbegin());
	EXPECT_NO_THROW(cA.block(1).cend());
	EXPECT_NO_THROW(cA.block(1).begin());
	EXPECT_NO_THROW(cA.block(1).end());
	EXPECT_NO_THROW(cA.block(1).rbegin());
	EXPECT_NO_THROW(cA.block(1).rend());
	EXPECT_NO_THROW([[maybe_unused]] typeT::iterator X(A.block(2).begin()));
	EXPECT_NO_THROW(
		[[maybe_unused]] typeT::const_iterator X(A.block(2).cbegin()));
	// EXPECT_NO_THROW((Board<int,
	// 2>::Block::const_iterator(cA.block(2).cbegin()))); *r is dereferenceable
	[[maybe_unused]] int U {};
	EXPECT_NO_THROW(*A.block(1).begin());
	EXPECT_NO_THROW(*A.block(1).cbegin());
	EXPECT_NO_THROW(U = *A.block(1).rbegin());
	EXPECT_NO_THROW(U = *A.block(1).crbegin());
	EXPECT_NO_THROW(*(cA.block(1).begin()));
	EXPECT_NO_THROW(*cA.block(1).cbegin());
	EXPECT_NO_THROW(U = *cA.block(1).rbegin());
	// ++r is incrementable
	EXPECT_NO_THROW(++A.block(1).begin());
	EXPECT_NO_THROW(++A.block(1).cbegin());
	EXPECT_NO_THROW(++cA.block(1).cbegin());
	EXPECT_NO_THROW(++cA.block(1).begin());
	EXPECT_NO_THROW(++A.block(1).rbegin());
	EXPECT_NO_THROW(++A.block(1).crbegin());
}

TEST(Section_Iterator, BlockInputIterator)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	Board<Options<4>, 2> Opt{};
	const Board<Options<4>, 2> cOpt{};

	// == !=
	[[maybe_unused]] bool U {};
	EXPECT_NO_THROW(A.block(0).begin() == A.block(0).end());
	EXPECT_NO_THROW(A.block(0).cbegin() == A.block(0).cend());
	EXPECT_NO_THROW(U = A.block(0).rbegin() == A.block(0).rend());
	EXPECT_NO_THROW(U = A.block(0).crbegin() == A.block(0).crend());
	EXPECT_NO_THROW(cA.block(0).cbegin() == cA.block(0).cend());
	EXPECT_NO_THROW(cA.block(0).begin() == cA.block(0).end());
	EXPECT_TRUE(A.block(0).begin() == A.block(0).begin());
	EXPECT_TRUE(cA.block(0).begin() == cA.block(0).begin());
	EXPECT_FALSE(A.block(0).begin() == A.block(0).end());
	EXPECT_TRUE(A.block(0).cbegin() == A.block(0).cbegin());
	EXPECT_FALSE(A.block(0).cbegin() == A.block(0).cend());
	EXPECT_TRUE(A.block(0).rbegin() == A.block(0).rbegin());
	EXPECT_FALSE(A.block(0).rbegin() == A.block(0).rend());
	EXPECT_TRUE(A.block(0).crbegin() == A.block(0).crbegin());
	EXPECT_FALSE(A.block(0).crbegin() == A.block(0).crend());
	EXPECT_NO_THROW(A.block(0).begin() != A.block(0).end());
	EXPECT_NO_THROW(A.block(0).cbegin() != A.block(0).cend());
	EXPECT_NO_THROW(cA.block(0).cbegin() != cA.block(0).cend());
	EXPECT_NO_THROW(cA.block(0).begin() != cA.block(0).end());
	EXPECT_NO_THROW(U = A.block(0).rbegin() != A.block(0).rend());
	EXPECT_NO_THROW(U = A.block(0).crbegin() != A.block(0).crend());
	EXPECT_TRUE(A.block(0).begin() != A.block(0).end());
	EXPECT_FALSE(A.block(0).begin() != A.block(0).begin());
	EXPECT_TRUE(A.block(0).cbegin() != ++A.block(0).cbegin());
	EXPECT_FALSE(A.block(0).begin() != A.block(0).begin());
	EXPECT_TRUE(cA.block(0).begin() != cA.block(0).end());
	EXPECT_TRUE(cA.block(0).cbegin() != ++cA.block(0).cbegin());
	EXPECT_TRUE(A.block(0).rbegin() != A.block(0).rend());
	EXPECT_TRUE(cA.block(0).rbegin() == cA.block(0).rbegin());
	EXPECT_FALSE(A.block(0).rbegin() != A.block(0).rbegin());
	EXPECT_TRUE(A.block(0).crbegin() != A.block(0).crend());
	EXPECT_FALSE(A.block(0).crbegin() != A.block(0).crbegin());
	// *i
	EXPECT_EQ(*A.block(0).begin(), 9);
	EXPECT_EQ(*A.block(1).begin(), 2);
	EXPECT_EQ(*cA.block(0).begin(), 9);
	EXPECT_EQ(*cA.block(1).begin(), 2);
	EXPECT_EQ(9, *A.block(0).begin());
	EXPECT_FALSE(*A.block(0).begin() == 4);
	EXPECT_EQ(*A.block(0).cbegin(), 9); // == *A.row(0).cbegin()
	EXPECT_FALSE(*A.block(0).cbegin() == 4);
	EXPECT_EQ(*A.block(1).cbegin(), 2); // == *A.row(0).cbegin()
	EXPECT_EQ(*A.block(0).rbegin(), 5);
	EXPECT_FALSE(*A.block(0).rbegin() == 3);
	EXPECT_EQ(*A.block(0).crbegin(), 5);
	EXPECT_FALSE(*A.block(0).crbegin() == 3);
	EXPECT_EQ(*cA.block(0).begin(), 9);
	EXPECT_FALSE(*cA.block(0).begin() == 3);
	EXPECT_EQ(*cA.block(0).rbegin(), 5);
	EXPECT_FALSE(*cA.block(0).rbegin() == 3);
	// i->member
	static_assert(noexcept(Opt.block(0).begin()->size()));
	static_assert(noexcept(Opt.block(0).cbegin()->size()));
	static_assert(noexcept(cOpt.block(0).cbegin()->size()));
	static_assert(not(noexcept(Opt.block(0).rbegin()->size())));
	static_assert(not(noexcept(Opt.block(0).crbegin()->size())));
	EXPECT_NO_THROW(
		U = (Opt.block(0).begin()->size() ==
		 (*Opt.block(0).begin()).size())); // equivalent
	EXPECT_EQ(
		(Opt.block(0).begin()->size()),
		(*Opt.block(0).begin()).size()); // equivalent
	EXPECT_EQ(
		(cOpt.block(0).begin()->size()),
		(*cOpt.block(0).begin()).size()); // equivalent
	// ++i (return type: It&)	is incrementable
	EXPECT_EQ(*(++A.block(0).begin()), 1);
	EXPECT_EQ(*(++A.block(0).cbegin()), 1);
	EXPECT_EQ(*(++cA.block(0).cbegin()), 1);
	EXPECT_EQ(*(++cA.block(0).begin()), 1);
	EXPECT_EQ(*(++A.block(0).rbegin()), 4);
	EXPECT_EQ(*(++A.block(0).crbegin()), 4);
	// (void)i++ equivalent to (void)++i
	EXPECT_NO_THROW(A.block(0).begin()++);
	EXPECT_NO_THROW(A.block(0).cbegin()++);
	EXPECT_NO_THROW(cA.block(0).cbegin()++);
	EXPECT_NO_THROW(A.block(0).rbegin()++);
	EXPECT_NO_THROW(A.block(0).crbegin()++);
	// *i++
	EXPECT_EQ(*A.block(0).begin()++, 9);
	EXPECT_EQ(*A.block(0).cbegin()++, 9);
	EXPECT_EQ(*cA.block(0).cbegin()++, 9);
	EXPECT_EQ(*cA.block(0).begin()++, 9);
	EXPECT_EQ(*A.block(0).rbegin()++, 5);
	EXPECT_EQ(*A.block(0).crbegin()++, 5);
	// std::accumulate
	int total{};
	EXPECT_NO_THROW(
		total = std::accumulate(A.block(0).cbegin(), A.block(0).cend(), 0));
	EXPECT_EQ(total, 19);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.block(1).cbegin(), A.block(1).cend(), 0));
	EXPECT_EQ(total, 18);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.block(2).cbegin(), A.block(2).cend(), 0));
	EXPECT_EQ(total, 42);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.block(3).cbegin(), A.block(3).cend(), 0));
	EXPECT_EQ(total, 50);
	total = 0;

	EXPECT_NO_THROW(
		total = std::accumulate(A.block(2).begin(), A.block(2).end(), 0));
	EXPECT_EQ(total, 42); // 2+6+10+14=32
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.block(2).cbegin(), A.block(2).cend(), 0));
	EXPECT_EQ(total, 42);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(A.block(2).rbegin(), A.block(2).rend(), 0));
	EXPECT_EQ(total, 42);
	total = 0;
	EXPECT_NO_THROW(
		total = std::accumulate(cA.block(2).rbegin(), cA.block(2).rend(), 0));
	EXPECT_EQ(total, 42);
}

TEST(Section_Iterator, BlockOutputIterator)
{
	Board<int, 2> tmp{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";
	// *r = o (return-value not used)
	[[maybe_unused]] int U {};
	EXPECT_NO_THROW(*tmp.block(0).begin()); // pre-condition
	EXPECT_NO_THROW(*tmp.block(0).begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	EXPECT_NO_THROW(*(++tmp.block(0).begin()) = 4);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(U = *tmp.block(0).rbegin()); // pre-condition
	EXPECT_NO_THROW(*tmp.block(0).rbegin() = 9);
	EXPECT_EQ(tmp[1][1], 9);
	EXPECT_NO_THROW(*(++tmp.block(0).rbegin()) = 8);
	EXPECT_EQ(tmp[1][0], 8);
	EXPECT_NO_THROW(*(tmp.block(0).begin()++) = 2);
	EXPECT_EQ(tmp[0][0], 2);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(*(tmp.block(0).rbegin()++) = 7);
	EXPECT_EQ(tmp[1][1], 7);
	EXPECT_EQ(tmp[1][0], 8);
	EXPECT_NO_THROW(++(*tmp.block(0).begin() = 0)); // post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.block(0).rbegin() = 9));
	EXPECT_EQ(tmp[1][1], 10);
	{
		auto r = tmp.begin();
		EXPECT_TRUE(&r == &++r);
	}
}

TEST(Section_Iterator, BlockForwardIterator)
{
	using typeT = typename Board_Section::Block<int, 2>;
	static_assert(std::is_default_constructible_v<typeT::iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_iterator>);

	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	EXPECT_NO_THROW(typeT::iterator());
	EXPECT_NO_THROW(typeT::iterator() = A.block(2).begin());

	EXPECT_NO_THROW(A.block(0).begin()++);
	EXPECT_EQ(*A.block(0).begin()++, A[0][0]);
	EXPECT_EQ(*A.block(0).cbegin()++, A[0][0]);
	EXPECT_EQ(*A.block(0).rbegin()++, A[1][1]);
	EXPECT_EQ(*A.block(0).crbegin()++, A[1][1]);
	EXPECT_TRUE(A.block(0).begin() == A.block(0).begin());
	EXPECT_TRUE(++A.block(0).begin() == ++A.block(0).begin());
	auto r = A.block(0).begin();
	auto i = r;
	*r     = 0;
	EXPECT_EQ(*r, 0);
	EXPECT_EQ(*i, 0);
	EXPECT_EQ(*++r, 1);
	EXPECT_EQ(*r, *(++i));
}

TEST(Section_Iterator, BlockBidirectionalIterator)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	// --i (return type: It&)	is decrementable
	EXPECT_NO_THROW(--A.end());
	EXPECT_NO_THROW(--A.cend());
	EXPECT_NO_THROW(--A.rend());
	EXPECT_NO_THROW(--A.crend());
	EXPECT_EQ(*(--A.block(0).end()), 5);
	EXPECT_EQ(*(--(--A.block(0).end())), 4);
	EXPECT_EQ(*(--A.block(0).cend()), 5);
	EXPECT_EQ(*(--(--A.block(0).cend())), 4);
	EXPECT_EQ(*(--A.block(0).rend()), 9);
	EXPECT_EQ(*(--(--A.block(0).rend())), 1);
	EXPECT_EQ(*(--A.block(0).crend()), 9);
	EXPECT_EQ(*(--(--A.block(0).crend())), 1);

	EXPECT_TRUE(--(++A.block(0).begin()) == A.block(0).begin());
	EXPECT_EQ(*(--(++A.block(0).begin())), *A.block(0).begin());
	EXPECT_TRUE(--(++A.block(0).cbegin()) == A.block(0).cbegin());
	EXPECT_TRUE(--(++A.block(0).rbegin()) == A.block(0).rbegin());
	EXPECT_TRUE(--(++A.block(0).crbegin()) == A.block(0).crbegin());
	// i--
	EXPECT_NO_THROW(A.block(0).end()--);
	EXPECT_NO_THROW(A.block(0).cend()--);
	EXPECT_NO_THROW(A.block(0).rend()--);
	EXPECT_NO_THROW(A.block(0).crend()--);
	// *i--
	EXPECT_TRUE(A.block(0).end()-- == A.block(0).end());
	EXPECT_EQ(*(--A.block(0).end())--, 5);
	EXPECT_TRUE(A.block(0).cend()-- == A.block(0).cend());
	EXPECT_EQ(*(--A.block(0).cend())--, 5);
	EXPECT_TRUE(A.block(0).rend()-- == A.block(0).rend());
	EXPECT_EQ(*(--A.block(0).rend())--, 9);
	EXPECT_TRUE(A.block(0).crend()-- == A.block(0).crend());
	EXPECT_EQ(*(--A.block(0).crend())--, 9);
}

TEST(Section_Iterator, BlockRandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff  = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	[[maybe_unused]] auto U = A.block(0).rbegin();
	[[maybe_unused]] auto V = A.block(0).crbegin();
	[[maybe_unused]] int W {};
	EXPECT_NO_THROW(A.block(0).begin() += 2);
	EXPECT_NO_THROW(A.block(0).cbegin() += 2);
	EXPECT_NO_THROW(A.block(0).rbegin() += 2);
	EXPECT_NO_THROW(A.block(0).crbegin() += 2);
	EXPECT_NO_THROW(A.block(0).end() += (-2));
	EXPECT_EQ(*(A.block(0).begin() += 2), 4); // returns a reference
	EXPECT_EQ(*(A.block(0).cbegin() += 2), 4);
	EXPECT_EQ(*(A.block(0).rbegin() += 1), 4);
	EXPECT_EQ(*(A.block(0).crbegin() += 1), 4);
	EXPECT_EQ(*(A.block(0).end() += (-2)), 4);
	EXPECT_EQ(*(A.block(0).cbegin() += 0), 9);
	EXPECT_FALSE(*(A.block(0).cbegin() += 2) == 5);
	EXPECT_FALSE(*(A.block(0).cbegin() += 0) == 5);

	EXPECT_NO_THROW(A.block(0).begin() + 2); // returns a new iterator object
	EXPECT_NO_THROW(A.block(0).cbegin() + 2);
	EXPECT_NO_THROW(U = A.block(0).rbegin() + 1);
	EXPECT_NO_THROW(V = A.block(0).crbegin() + 2);
	EXPECT_NO_THROW(A.block(0).cend() + (-3));

	EXPECT_NO_THROW(2 + (Board<int, 2>().block(0).begin()));
	EXPECT_NO_THROW(2 + A.block(0).cbegin());
	EXPECT_NO_THROW(U = 2 + A.block(0).rbegin());
	EXPECT_NO_THROW(V = 2 + A.block(0).crbegin());
	EXPECT_NO_THROW(U = (-2) + A.block(0).rend());

	EXPECT_TRUE((A.block(0).begin() + 1) == (++A.block(0).begin()));
	EXPECT_TRUE((A.block(0).cbegin() + 1) == (++A.block(0).cbegin()));
	EXPECT_TRUE((A.block(0).rbegin() + 1) == (++A.block(0).rbegin()));
	EXPECT_TRUE((A.block(0).crbegin() + 1) == (++A.block(0).crbegin()));
	EXPECT_TRUE((A.block(0).begin() + 3) == (A.block(0).begin() += 3));
	EXPECT_TRUE((A.block(0).cbegin() + 1) == (A.block(0).cbegin() += 1));
	EXPECT_TRUE((A.block(0).rbegin() + 2) == (A.block(0).rbegin() += 2));
	EXPECT_TRUE((A.block(0).crbegin() + 3) == (A.block(0).crbegin() += 3));
	EXPECT_TRUE((A.block(0).end() + (-2)) == (A.block(0).begin() += 2));
	EXPECT_FALSE((A.block(0).begin() + 3) == (++A.block(0).begin()));

	EXPECT_TRUE((A.block(0).begin() + 2) == (2 + A.block(0).begin()));

	EXPECT_NO_THROW(A.block(0).end() -= 2);
	EXPECT_NO_THROW(A.block(0).cend() -= 2);
	EXPECT_NO_THROW(A.block(0).rend() -= 2);
	EXPECT_NO_THROW(A.block(0).crend() -= 2);
	EXPECT_NO_THROW(A.block(0).crbegin() -= (-3));
	EXPECT_TRUE((A.block(0).end() -= 1) == (A.block(0).begin() += 3));
	EXPECT_TRUE((A.block(0).cend() -= 1) == (A.block(0).cbegin() += 3));
	EXPECT_TRUE((A.block(0).rend() -= 1) == (A.block(0).rbegin() += 3));
	EXPECT_TRUE((A.block(0).crend() -= 1) == (A.block(0).crbegin() += 3));
	EXPECT_TRUE((A.block(0).begin() -= (-1)) == (A.block(0).begin() += 1));

	EXPECT_NO_THROW(A.block(0).end() - 2);
	EXPECT_NO_THROW(A.block(0).cend() - 2);
	EXPECT_NO_THROW(U = A.block(0).rend() - 1);
	EXPECT_NO_THROW(V = A.block(0).crend() - 2);
	EXPECT_NO_THROW(A.block(0).begin() - (-2));

	EXPECT_NO_THROW(A.block(0).end() - A.block(0).begin());
	EXPECT_NO_THROW(A.block(0).cend() - A.block(0).cbegin());
	EXPECT_NO_THROW(W = A.block(0).rend() - A.block(0).rbegin());
	EXPECT_NO_THROW(W = A.block(0).crend() - A.block(0).crbegin());
	EXPECT_TRUE((A.block(0).end() - A.block(0).begin()) == A.block(0).size());
	EXPECT_TRUE((A.block(0).begin() - A.block(0).end()) == -A.block(0).size());
	EXPECT_TRUE((A.block(0).begin() - A.block(0).end()) == -A.block(0).size());
	EXPECT_TRUE((A.block(0).cend() - A.block(0).cbegin()) == A.block(0).size());

	EXPECT_NO_THROW(A.block(0).begin()[2]); // return value_type reference
	EXPECT_NO_THROW(A.block(0).cbegin()[2]);
	EXPECT_NO_THROW(W = A.block(0).rbegin()[2]);
	EXPECT_NO_THROW(W = A.block(0).crbegin()[2]);
	EXPECT_EQ(A.block(0).begin()[2], 4);
	EXPECT_EQ(A.block(0).cbegin()[3], 5);
	EXPECT_EQ(A.block(0).rend()[-3], *(A.block(0).rend() - 3));
	EXPECT_EQ(A.block(0).crbegin()[2], 1);

	EXPECT_NO_THROW(A.block(0).begin() < A.block(0).begin());
	EXPECT_NO_THROW(A.block(0).cbegin() < A.block(0).cbegin());
	EXPECT_NO_THROW(A.block(0).begin() > A.block(0).begin());
	EXPECT_NO_THROW(A.block(0).cbegin() > A.block(0).cbegin());
	EXPECT_NO_THROW(A.block(0).begin() >= A.block(0).begin());
	EXPECT_NO_THROW(A.block(0).cbegin() >= A.block(0).cbegin());
	EXPECT_NO_THROW(A.block(0).begin() <= A.block(0).begin());
	EXPECT_NO_THROW(A.block(0).cbegin() <= A.block(0).cbegin());
	EXPECT_FALSE(A.block(0).cbegin() < A.block(0).cbegin());
	EXPECT_LT(A.block(0).cbegin(), A.block(0).cend());
	EXPECT_FALSE(A.block(0).cend() < A.block(0).cbegin());

	// shuffle(start, stop) requires a random iterator
	ASSERT_EQ(A, cA);
	std::random_device rd;   // non-deterministic generator
	std::mt19937 urng(rd()); // to seed mersenne twister
	EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
	EXPECT_NE(A, cA);
}

TEST(Section_Iterator, deathtest)
{
	Board<int, 2> A{9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	const Board<int, 2> cA{
		9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	// move out of range
	// forward and random-access require different steps
	EXPECT_NO_FATAL_FAILURE(A.row(0).begin() += 5);
	EXPECT_NO_FATAL_FAILURE(cA.row(0).begin() += 5);

	// check asserts that check if same board
	Board<int, 2> B{};
	const Board<int, 2> cB{};
	EXPECT_NO_FATAL_FAILURE(A.row(0).begin() == A.row(0).begin());
	EXPECT_DEBUG_DEATH(A.row(0).begin() == B.row(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() == B.row(2).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.col(0).begin() == B.col(2).end(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.block(0).begin() == B.block(2).end(), ".*: is_same");
	EXPECT_DEBUG_DEATH(cA.row(0).begin() == cB.row(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() != B.row(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(cA.row(0).begin() == A.row(0).cbegin(), ".*: is_same");
#ifndef _DEBUG
	EXPECT_NE(A.row(0).begin(), B.row(0).begin());
	EXPECT_NE(A.row(0).begin(), A.row(2).begin());
#endif // !_DEBUG
	// assert(owner_->owner_ == other.owner_->owner_);
	EXPECT_NO_FATAL_FAILURE(A.row(0).begin() < A.row(0).begin());
	EXPECT_DEBUG_DEATH(A.row(0).begin() < B.row(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.col(0).begin() < B.col(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.block(0).begin() < B.block(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(cA.row(0).begin() < cB.row(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(cA.row(0).begin() < B.row(0).cbegin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() > B.row(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() <= B.row(0).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() <= B.row(0).begin(), ".*: is_same");

	// assert(owner_->id() == other.owner_->id());
	EXPECT_NO_FATAL_FAILURE(A.row(0).begin() < A.row(0).end());
	EXPECT_DEBUG_DEATH(A.row(0).begin() < A.row(1).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.col(0).begin() < A.col(1).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() > A.row(1).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() <= A.row(1).begin(), ".*: is_same");
	EXPECT_DEBUG_DEATH(A.row(0).begin() >= A.row(1).begin(), ".*: is_same");

	// invalid location on dereferencing out-of-bounds iterator
	int tmp_{};
	size_t tmp2{};
	EXPECT_NO_FATAL_FAILURE(tmp_ = *((cA.row(0).cbegin())--));
	EXPECT_NO_FATAL_FAILURE(tmp_ = *(--(cA.row(0).cend()))++);
#ifdef _DEBUG
	EXPECT_DEBUG_DEATH(tmp_ = *(--(A.row(0).begin())), ".*: is_valid_size");
	EXPECT_DEBUG_DEATH(tmp_ = *(A.row(0).end()), ".*: is_valid_size");
	EXPECT_DEBUG_DEATH(tmp_ = *(cA.row(0).end()), ".*: is_valid_size");
	EXPECT_DEBUG_DEATH(tmp_ = *(--(cA.row(0).cbegin())), ".*: is_valid_size");
	EXPECT_DEBUG_DEATH(tmp_ = *((A.row(0).begin()) += 8), ".*: is_valid_size");
#endif // _DEBUG
	Board<Options<4>, 2> Opt{};
	EXPECT_NO_FATAL_FAILURE(tmp2 = (Opt.row(0).begin())->count());
#ifdef _DEBUG
	EXPECT_DEBUG_DEATH(
		tmp2 = (--(Opt.row(0).begin()))->count(), ".*: is_valid_size");
#endif // _DEBUG
}

} // namespace SudokuTests::BoardTest
