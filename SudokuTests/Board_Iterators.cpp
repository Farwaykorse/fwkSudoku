/**	Unit tests for the template class Sudoku::Board
 *	***************************************************************************
 *	Implemented with GoogleTest
 *
 *	Notes:
 *	gTest is limited for use with multiple template parameters.
 *	These expressions need to be implemented between extra parentheses
 *	- test elements implementing this are flagged with [gTest]
 *	- not implemented tests are flagged as NEEDTEST [gTest]
 *	gTest tries to print iterators if they use inheritance, if used in *_EQ/NE etc.
 *		use an explicit test like EXPECT_TRUE(.. == ..).
 */
#include <gtest/gtest.h>

// Class under test
#include "../Sudoku/Board.h"
// helpers
#include "../Sudoku/Location.h"
#include "../Sudoku/Options.h"

// library
#include <bitset>
#include <initializer_list>
#include <numeric>		// accumulate
#include <random>		// randomaccess tests
#include <set>
#include <type_traits>

using namespace Sudoku;

namespace SudokuTests::BoardTest
{
namespace iterator
{
	using typeT = Board<int>;

	static_assert(std::is_object_v<typeT::iterator>);
	static_assert(std::is_object_v<typeT::const_iterator>);
	static_assert(std::is_object_v<typeT::reverse_iterator>);
	static_assert(std::is_object_v<typeT::const_reverse_iterator>);
	static_assert(!std::is_const_v<typeT::iterator>);
	static_assert(!std::is_const_v<typeT::const_iterator>);
	static_assert(!std::is_const_v<typeT::reverse_iterator>);
	static_assert(!std::is_const_v<typeT::const_reverse_iterator>);
	// Requirements:
	// All iterator categories
		// CopyConstructible
	static_assert(std::is_copy_constructible_v <typeT::iterator>);
	static_assert(std::is_copy_constructible_v <typeT::const_iterator>);
	static_assert(std::is_copy_constructible_v <typeT::reverse_iterator>);
	static_assert(std::is_copy_constructible_v <typeT::const_reverse_iterator>);
		// CopyAssignable
	static_assert(std::is_copy_assignable_v <typeT::iterator>);
	static_assert(std::is_copy_assignable_v <typeT::const_iterator>);
	static_assert(std::is_copy_assignable_v <typeT::reverse_iterator>);
	static_assert(std::is_copy_assignable_v <typeT::const_reverse_iterator>);
		// Destructible
	static_assert(std::is_destructible_v<typeT::iterator>);
	static_assert(std::is_destructible_v<typeT::const_iterator>);
	static_assert(std::is_destructible_v<typeT::reverse_iterator>);
	static_assert(std::is_destructible_v<typeT::const_reverse_iterator>);
		// Swappable (lvalues)
	//static_assert(std::is_swappable_v<typeT::iterator>);
	//static_assert(std::is_swappable_v<typeT::const_iterator>);
	//static_assert(std::is_swappable_v<typeT::reverse_iterator>);
	//static_assert(std::is_swappable_v<typeT::const_reverse_iterator>);
		// std::iterator_traits<It> has member typedefs value_type, difference_type, reference, pointer, iterator_category
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::value_type>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::difference_type>);

	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::value_type>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::const_iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::difference_type>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator::iterator_category()>>);
	// inherit from vector Might change
	static_assert(std::is_same_v<typeT::iterator, std::vector<int>::iterator>);
	static_assert(std::is_same_v<typeT::const_iterator, std::vector<int>::const_iterator>);
	static_assert(std::is_same_v<typeT::reverse_iterator, std::vector<int>::reverse_iterator>);
	static_assert(std::is_same_v<typeT::const_reverse_iterator, std::vector<int>::const_reverse_iterator>);
	
	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<std::iterator_traits<typeT::iterator>::iterator_category, category>);
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_iterator>::iterator_category, category>);
	static_assert(std::is_same_v<std::iterator_traits<typeT::reverse_iterator>::iterator_category, category>);
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_reverse_iterator>::iterator_category, category>);
	
	using type = Options<6>;
	using baseT = Board<type, 4>;
	//static_assert(std::is_same_v<std::iterator_traits<std::vector<type>::iterator>::value_type, type>);
	static_assert(std::is_same_v<std::iterator_traits<baseT::iterator>::value_type, type>);
	static_assert(std::is_same_v<std::iterator_traits<baseT::const_iterator>::value_type, type>);
	static_assert(std::is_same_v<std::iterator_traits<baseT::reverse_iterator>::value_type, type>);
	static_assert(std::is_same_v<std::iterator_traits<baseT::const_reverse_iterator>::value_type, type>);
}
TEST(Board, iterator)
{
	// All iterator categories
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	EXPECT_NO_THROW(A.begin());
	EXPECT_NO_THROW(A.end());
	EXPECT_NO_THROW(A.cbegin());
	EXPECT_NO_THROW(A.cend());
	EXPECT_NO_THROW(A.rbegin());
	EXPECT_NO_THROW(A.rend());
	EXPECT_NO_THROW(A.crbegin());
	EXPECT_NO_THROW(A.crend());
	EXPECT_NO_THROW(cA.begin());
	EXPECT_NO_THROW(cA.end());
	EXPECT_NO_THROW(cA.rbegin());
	EXPECT_NO_THROW(cA.rend());
	// *r is dereferenceable
	EXPECT_NO_THROW(*A.begin());
	EXPECT_NO_THROW(*A.cbegin());
	EXPECT_NO_THROW(*A.rbegin());
	EXPECT_NO_THROW(*A.crbegin());
	EXPECT_NO_THROW(*cA.begin());
	EXPECT_NO_THROW(*cA.rbegin());
	// ++r is incrementable
	EXPECT_NO_THROW(++A.begin());
	EXPECT_NO_THROW(++A.cbegin());
	EXPECT_NO_THROW(++A.rbegin());
	EXPECT_NO_THROW(++A.crbegin());
}
TEST(Board, InputIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	Board<Options<4>, 2> Opt{};

	// == !=
	EXPECT_NO_THROW(A.begin() == A.end());
	EXPECT_NO_THROW(A.cbegin() == A.cend());
	EXPECT_NO_THROW(A.rbegin() == A.rend());
	EXPECT_NO_THROW(A.crbegin() == A.crend());
	EXPECT_EQ(A.begin(), A.begin());
	EXPECT_FALSE(A.begin() == A.end());
	EXPECT_EQ(A.cbegin(), A.cbegin());
	EXPECT_FALSE(A.cbegin() == A.cend());
	EXPECT_EQ(A.rbegin(), A.rbegin());
	EXPECT_FALSE(A.rbegin() == A.rend());
	EXPECT_EQ(A.crbegin(), A.crbegin());
	EXPECT_FALSE(A.crbegin() == A.crend());
	EXPECT_NO_THROW(A.begin() != A.end());
	EXPECT_NO_THROW(A.cbegin() != A.cend());
	EXPECT_NO_THROW(A.rbegin() != A.rend());
	EXPECT_NO_THROW(A.crbegin() != A.crend());
	EXPECT_NE(A.begin(), A.end());
	EXPECT_FALSE(A.begin() != A.begin());
	EXPECT_NE(A.cbegin(), ++A.cbegin());
	EXPECT_FALSE(A.begin() != A.begin());
	EXPECT_NE(A.rbegin(), A.rend());
	EXPECT_FALSE(A.rbegin() != A.rbegin());
	EXPECT_NE(A.crbegin(), A.crend());
	EXPECT_FALSE(A.crbegin() != A.crbegin());
	// *i
	EXPECT_EQ(*A.begin(), 9);
	EXPECT_EQ(9, *A.begin());
	EXPECT_FALSE(*A.begin() == 5);
	EXPECT_EQ(*A.cbegin(), 9);
	EXPECT_FALSE(*A.cbegin() == 5);
	EXPECT_EQ(*A.rbegin(), 15);
	EXPECT_FALSE(*A.rbegin() == 5);
	EXPECT_EQ(*A.crbegin(), 15);
	EXPECT_FALSE(*A.crbegin() == 5);
	EXPECT_EQ(*cA.begin(), 9);
	EXPECT_FALSE(*cA.begin() == 5);
	EXPECT_EQ(*cA.rbegin(), 15);
	EXPECT_FALSE(*cA.rbegin() == 5);
	// i->member
	EXPECT_NO_THROW(Opt.begin()->size());
	EXPECT_NO_THROW(Opt.cbegin()->size());
	EXPECT_NO_THROW(Opt.rbegin()->size());
	EXPECT_NO_THROW(Opt.crbegin()->size());
	EXPECT_NO_THROW((Opt.begin()->size() == (*Opt.begin()).size()));	// equivalent
	EXPECT_EQ((Opt.begin()->size()), (*Opt.begin()).size());	// equivalent
	// ++i (returntype: It&)	is incrementable
	EXPECT_EQ(*(++A.begin()), 1);
	EXPECT_EQ(*(++A.cbegin()), 1);
	EXPECT_EQ(*(++A.rbegin()), 14);
	EXPECT_EQ(*(++A.crbegin()), 14);
	// (void)i++ equivalent to (void)++i
	EXPECT_NO_THROW(A.begin()++);
	EXPECT_NO_THROW(A.cbegin()++);
	EXPECT_NO_THROW(A.rbegin()++);
	EXPECT_NO_THROW(A.crbegin()++);
	// *i++
	EXPECT_EQ(*A.begin()++, 9);
	EXPECT_EQ(*A.cbegin()++, 9);
	EXPECT_EQ(*A.rbegin()++, 15);
	EXPECT_EQ(*A.crbegin()++, 15);

	int total{};
	EXPECT_NO_THROW(total = std::accumulate(A.begin(), A.end(), 0));
	EXPECT_EQ(total, 129);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.cbegin(), A.cend(), 0));
	EXPECT_EQ(total, 129);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.rbegin(), A.rend(), 0));
	EXPECT_EQ(total, 129);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(cA.rbegin(), cA.rend(), 0));
	EXPECT_EQ(total, 129);
}
TEST(Board, OutputIterator)
{
	Board<int, 2> tmp{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";
	// *r = o (returnvalue not used)
	EXPECT_NO_THROW(*tmp.begin());	// pre-condition
	EXPECT_NO_THROW(*tmp.begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	EXPECT_NO_THROW(*(++tmp.begin()) = 4);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(*tmp.rbegin());	// pre-condition
	EXPECT_NO_THROW(*tmp.rbegin() = 9);
	EXPECT_EQ(tmp[3][3], 9);
	EXPECT_NO_THROW(*(++tmp.rbegin()) = 8);
	EXPECT_EQ(tmp[3][2], 8);
	EXPECT_NO_THROW(*(tmp.begin()++) = 2);
	EXPECT_EQ(tmp[0][0], 2);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(*(tmp.rbegin()++) = 7);
	EXPECT_EQ(tmp[3][3], 7);
	EXPECT_EQ(tmp[3][2], 8);
	EXPECT_NO_THROW(++(*tmp.begin() = 0));	// post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.rbegin() = 9));
	EXPECT_EQ(tmp[3][3], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r);
}
TEST(Board, ForwardIterator)
{
	using typeT = Board<int>;
	static_assert(std::is_default_constructible_v<typeT::iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_iterator>);
	static_assert(std::is_default_constructible_v<typeT::reverse_iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_reverse_iterator>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	EXPECT_NO_THROW(A.begin()++);
	EXPECT_EQ(*A.begin()++, A[0][0]);
	EXPECT_EQ(*A.cbegin()++, A[0][0]);
	EXPECT_EQ(*A.rbegin()++, A[3][3]);
	EXPECT_EQ(*A.crbegin()++, A[3][3]);
	EXPECT_EQ(A.begin(), A.begin());
	EXPECT_EQ(++A.begin(), ++A.begin());
	auto r = A.begin();
	auto i = r;
	*r = 0;
	EXPECT_EQ(*r, 0);
	EXPECT_EQ(*i, 0);
	EXPECT_EQ(*++r, 1);
	EXPECT_EQ(*++r, *++(++i));
}
TEST(Board, BidirectionalIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	
	// --i (returntype: It&)	is decrementable
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
	// i--
	EXPECT_NO_THROW(A.end()--);
	EXPECT_NO_THROW(A.cend()--);
	EXPECT_NO_THROW(A.rend()--);
	EXPECT_NO_THROW(A.crend()--);
	// *i--
	EXPECT_EQ(A.end()--, A.end());
	EXPECT_EQ(*(--A.end())--, 15);
	EXPECT_EQ(A.cend()--, A.cend());
	EXPECT_EQ(*(--A.cend())--, 15);
	EXPECT_EQ(A.rend()--, A.rend());
	EXPECT_EQ(*(--A.rend())--, 9);
	EXPECT_EQ(A.crend()--, A.crend());
	EXPECT_EQ(*(--A.crend())--, 9);
}
TEST(Board, RandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	EXPECT_NO_THROW(A.begin() += 2);
	EXPECT_NO_THROW(A.cbegin() += 2);
	EXPECT_NO_THROW(A.rbegin() += 2);
	EXPECT_NO_THROW(A.crbegin() += 2);
	EXPECT_NO_THROW(A.end() += (-2));
	EXPECT_EQ(*(A.begin()+=15), 15); // returns a reference to the changed object
	EXPECT_EQ(*(A.cbegin()+=2), 2);
	EXPECT_EQ(*(A.rbegin()+=1), 14);
	EXPECT_EQ(*(A.crbegin()+=1), 14);
	EXPECT_EQ(*(A.end() += (-2)), 14);
	EXPECT_EQ(*(A.cbegin()+=0), 9);
	EXPECT_FALSE(*(A.cbegin()+=8) == 5);
	EXPECT_FALSE(*(A.cbegin()+=0) == 5);

	EXPECT_NO_THROW(A.begin() + 2);	// returns a new iterator object
	EXPECT_NO_THROW(A.cbegin() + 2);
	EXPECT_NO_THROW(A.rbegin() + 1);
	EXPECT_NO_THROW(A.crbegin() + 2);
	EXPECT_NO_THROW(A.cend() + (-3));
	EXPECT_NO_THROW(12 + A.begin());
	EXPECT_NO_THROW(12 + A.cbegin());
	EXPECT_NO_THROW(12 + A.rbegin());
	EXPECT_NO_THROW(12 + A.crbegin());
	EXPECT_NO_THROW((-12) + A.rend());
	EXPECT_TRUE((A.begin() + 1) == ++A.begin());
	EXPECT_TRUE((A.cbegin() + 1) == ++A.cbegin());
	EXPECT_TRUE((A.rbegin() + 1) == ++A.rbegin());
	EXPECT_TRUE((A.crbegin() + 1) == ++A.crbegin());
	EXPECT_TRUE((A.begin() + 5) == (A.begin() += 5));
	EXPECT_TRUE((A.cbegin() + 4) == (A.cbegin() += 4));
	EXPECT_TRUE((A.rbegin() + 5) == (A.rbegin() += 5));
	EXPECT_TRUE((A.crbegin() + 8) == (A.crbegin() += 8));
	EXPECT_TRUE((A.end() + (-3)) == (A.begin() += 13));
	EXPECT_FALSE(A.begin() + 5 == ++A.begin());
	EXPECT_TRUE((A.begin() + 7) == (7 + A.begin()));

	EXPECT_NO_THROW(A.end() -= 2);
	EXPECT_NO_THROW(A.cend() -= 2);
	EXPECT_NO_THROW(A.rend() -= 2);
	EXPECT_NO_THROW(A.crend() -= 2);
	EXPECT_NO_THROW(A.crbegin() -= (-3));
	EXPECT_TRUE((A.end() -= 1) == (A.begin() += 15));
	EXPECT_TRUE((A.cend() -= 1) == (A.cbegin() += 15));
	EXPECT_TRUE((A.rend() -= 1) == (A.rbegin() += 15));
	EXPECT_TRUE((A.crend() -= 1) == (A.crbegin() += 15));
	EXPECT_TRUE((A.begin() -= (-10)) == (A.begin() += 10));

	EXPECT_NO_THROW(A.end() - 2);
	EXPECT_NO_THROW(A.cend() - 2);
	EXPECT_NO_THROW(A.rend() - 1);
	EXPECT_NO_THROW(A.crend() - 2);
	EXPECT_NO_THROW(A.begin() - (-12));

	EXPECT_NO_THROW(A.end() - A.begin());
	EXPECT_TRUE((A.end() - A.begin()) == A.size());
	EXPECT_TRUE((A.begin() - A.end()) == -A.size());
	EXPECT_TRUE((A.begin() - A.end()) == -A.size());

	EXPECT_NO_THROW(A.begin()[2]);	// return value_type reference
	EXPECT_NO_THROW(A.cbegin()[2]);
	EXPECT_NO_THROW(A.rbegin()[2]);
	EXPECT_NO_THROW(A.crbegin()[2]);
	EXPECT_EQ(A.begin()[2], 2);
	EXPECT_EQ(A.cbegin()[12], 12);
	EXPECT_EQ(A.rend()[-13], *(A.rend()-13));
	EXPECT_EQ(A.crbegin()[13], 2);

	EXPECT_NO_THROW(A.begin() < A.begin());
	EXPECT_NO_THROW(A.begin() > A.begin());
	EXPECT_NO_THROW(A.begin() >= A.begin());
	EXPECT_NO_THROW(A.begin() <= A.begin());
	EXPECT_TRUE(A.begin() < A.end());
	EXPECT_FALSE(A.begin() < A.begin());
	EXPECT_FALSE(A.end() < A.begin());
	EXPECT_NO_THROW(cA.begin() < cA.begin());
	EXPECT_NO_THROW(cA.begin() > cA.begin());
	EXPECT_NO_THROW(cA.begin() >= cA.begin());
	EXPECT_NO_THROW(cA.begin() <= cA.begin());
	EXPECT_TRUE(cA.begin() < cA.end());
	EXPECT_FALSE(cA.begin() < cA.begin());
	EXPECT_FALSE(cA.end() < cA.begin());

	// shuffle(start, stop) requires a random iterator
	ASSERT_EQ(A, cA);
	std::random_device rd;		// non-deterministic generator
	std::mt19937 urng(rd());	// to seed mersenne twister
	EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
	EXPECT_NE(A, cA);
}

namespace RowIterator
{
	using typeT = Board<int, 3>::Row;
	// all iterator categories
	static_assert(std::is_object_v<typeT::iterator>);
	static_assert(std::is_object_v<typeT::const_iterator>);
	//static_assert(std::is_object_v<typeT::reverse_iterator>);
	//static_assert(std::is_object_v<typeT::const_reverse_iterator>);
	static_assert(!std::is_const_v<typeT::iterator>);
	static_assert(!std::is_const_v<typeT::const_iterator>);
	//static_assert(!std::is_const_v<typeT::reverse_iterator>);
	//static_assert(!std::is_const_v<typeT::const_reverse_iterator>);
		// CopyConstructible
	static_assert(std::is_copy_constructible_v <typeT::iterator>);
	static_assert(std::is_copy_constructible_v <typeT::const_iterator>);
	//static_assert(std::is_copy_constructible_v <typeT::reverse_iterator>);
	//static_assert(std::is_copy_constructible_v <typeT::const_reverse_iterator>);
		// CopyAssignable
	static_assert(std::is_copy_assignable_v <typeT::iterator>);
	static_assert(std::is_copy_assignable_v <typeT::const_iterator>);
	//static_assert(std::is_copy_assignable_v <typeT::reverse_iterator>);
	//static_assert(std::is_copy_assignable_v <typeT::const_reverse_iterator>);
		// Destructible
	static_assert(std::is_destructible_v<typeT::iterator>);
	static_assert(std::is_destructible_v<typeT::const_iterator>);
	//static_assert(std::is_destructible_v<typeT::reverse_iterator>);
	//static_assert(std::is_destructible_v<typeT::const_reverse_iterator>);
		// Swappable (lvalues)
	//static_assert(std::is_swappable_v<typeT::iterator>);
	//static_assert(std::is_swappable_v<typeT::const_iterator>);
	//static_assert(std::is_swappable_v<typeT::reverse_iterator>);
	//static_assert(std::is_swappable_v<typeT::const_reverse_iterator>);

		// std::iterator_traits<It> has member typedefs value_type, difference_type, reference, pointer, iterator_category
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::value_type>);
	static_assert(!std::is_object_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::difference_type>);

	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::value_type>);
	static_assert(!std::is_object_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::const_iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::difference_type>);

	// forward_iterator_tag	// bidirectional_iterator_tag	// random_access_iterator_tag
	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<std::iterator_traits<typeT::iterator>::iterator_category, category>);
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_iterator>::iterator_category, category>);
	//static_assert(std::is_same_v<std::iterator_traits<typeT::reverse_iterator>::iterator_category, category>);
	//static_assert(std::is_same_v<std::iterator_traits<typeT::const_reverse_iterator>::iterator_category, category>);

	// no implementation differences / mistakes
	using type = Options<6>;
	using RowT = Board<type, 4>::Row;
	static_assert(std::is_same_v<std::iterator_traits<RowT::iterator>::value_type, type>);
	static_assert(std::is_same_v<std::iterator_traits<RowT::const_iterator>::value_type, const type>);
	static_assert(std::is_const_v<std::iterator_traits<RowT::const_iterator>::value_type>);
	//static_assert(std::is_same_v<std::iterator_traits<RowT::reverse_iterator>::value_type, type>);
	//static_assert(std::is_same_v<std::iterator_traits<RowT::const_reverse_iterator>::value_type, const type>);

	// no implementation differences / mistakes
	using diff = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_iterator>::difference_type, diff>);
	//static_assert(std::is_same_v<std::iterator_traits<typeT::reverse_iterator>::difference_type, diff>);
	//static_assert(std::is_same_v<std::iterator_traits<typeT::const_reverse_iterator>::difference_type, diff>);
}
TEST(Board, RowIterator)
{
	// All iterator categories
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

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
	EXPECT_NO_THROW((Board<int, 2>::Row::iterator(A.row(2).begin())));
	EXPECT_NO_THROW((Board<int, 2>::Row::const_iterator(A.row(2).cbegin())));
	EXPECT_NO_THROW((Board<int, 2>::Row::const_iterator(cA.row(2).cbegin())));
	// *r is dereferenceable
	EXPECT_NO_THROW(*A.row(1).begin());
	EXPECT_NO_THROW(*A.row(1).cbegin());
	EXPECT_NO_THROW(*A.row(1).rbegin());
	EXPECT_NO_THROW(*A.row(1).crbegin());
	EXPECT_NO_THROW(*(cA.row(1).begin()));
	EXPECT_NO_THROW(*cA.row(1).cbegin());
	EXPECT_NO_THROW(*cA.row(1).rbegin());
	// ++r is incrementable
	EXPECT_NO_THROW(++A.row(1).begin());
	EXPECT_NO_THROW(++A.row(1).cbegin());
	EXPECT_NO_THROW(++cA.row(1).cbegin());
	EXPECT_NO_THROW(++cA.row(1).begin());
	EXPECT_NO_THROW(++A.row(1).rbegin());
	EXPECT_NO_THROW(++A.row(1).crbegin());
}
TEST(Board, RowInputIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	Board<Options<4>, 2> Opt{};
	const Board<Options<4>, 2> cOpt{};

	// == !=
	EXPECT_NO_THROW(A.row(0).begin() == A.row(0).end());
	EXPECT_NO_THROW(A.row(0).cbegin() == A.row(0).cend());
	EXPECT_NO_THROW(A.row(0).rbegin() == A.row(0).rend());
	EXPECT_NO_THROW(A.row(0).crbegin() == A.row(0).crend());
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
	EXPECT_NO_THROW(A.row(0).rbegin() != A.row(0).rend());
	EXPECT_NO_THROW(A.row(0).crbegin() != A.row(0).crend());
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
	EXPECT_NO_THROW(Opt.row(0).begin()->size());
	EXPECT_NO_THROW(Opt.row(0).cbegin()->size());
	EXPECT_NO_THROW(cOpt.row(0).cbegin()->size());
	EXPECT_NO_THROW(Opt.row(0).rbegin()->size());
	EXPECT_NO_THROW(Opt.row(0).crbegin()->size());
	EXPECT_NO_THROW((Opt.row(0).begin()->size() == (*Opt.row(0).begin()).size()));	// equivalent
	EXPECT_EQ((Opt.row(0).begin()->size()), (*Opt.row(0).begin()).size());	// equivalent
	EXPECT_EQ((cOpt.row(0).begin()->size()), (*cOpt.row(0).begin()).size());	// equivalent
	// ++i (returntype: It&)	is incrementable
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
	EXPECT_NO_THROW(total = std::accumulate(A.row(0).cbegin(), A.row(0).cend(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(1).cbegin(), A.row(1).cend(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(2).cbegin(), A.row(2).cend(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(3).cbegin(), A.row(3).cend(), 0));
	EXPECT_EQ(total, 54);
	total = 0;

	EXPECT_NO_THROW(total = std::accumulate(A.row(0).begin(), A.row(0).end(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(1).begin(), A.row(1).end(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(2).begin(), A.row(2).end(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(3).begin(), A.row(3).end(), 0));
	EXPECT_EQ(total, 54);
	total = 0;

	EXPECT_NO_THROW(total = std::accumulate(A.row(0).crbegin(), A.row(0).crend(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(1).crbegin(), A.row(1).crend(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(2).crbegin(), A.row(2).crend(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(3).crbegin(), A.row(3).crend(), 0));
	EXPECT_EQ(total, 54);
	total = 0;

	EXPECT_NO_THROW(total = std::accumulate(A.row(0).rbegin(), A.row(0).rend(), 0));
	EXPECT_EQ(total, 15);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(1).rbegin(), A.row(1).rend(), 0));
	EXPECT_EQ(total, 22);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(2).rbegin(), A.row(2).rend(), 0));
	EXPECT_EQ(total, 38);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.row(3).rbegin(), A.row(3).rend(), 0));
	EXPECT_EQ(total, 54);
	total = 0;
}
TEST(Board, RowOutputIterator)
{
	Board<int, 2> tmp{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";
	// *r = o (returnvalue not used)
	EXPECT_NO_THROW(*tmp.row(0).begin());	// pre-condition
	EXPECT_NO_THROW(*tmp.row(0).begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	EXPECT_NO_THROW(*(++tmp.row(0).begin()) = 4);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(*tmp.row(0).rbegin());	// pre-condition
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
	EXPECT_NO_THROW(++(*tmp.row(0).begin() = 0));	// post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.row(0).rbegin() = 9));
	EXPECT_EQ(tmp[0][3], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r);
}
TEST(Board, RowForwardIterator)
{
	using typeT = Board<int>::Row;
	static_assert(std::is_default_constructible_v<typeT::iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_iterator>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	EXPECT_NO_THROW(Board<int>::Row::iterator());
	EXPECT_NO_THROW((Board<int, 2>::Row::iterator() = A.row(2).begin()));

	EXPECT_NO_THROW(A.row(0).begin()++);
	EXPECT_EQ(*A.row(0).begin()++, A[0][0]);
	EXPECT_EQ(*A.row(0).cbegin()++, A[0][0]);
	EXPECT_EQ(*A.row(0).rbegin()++, A[0][3]);
	EXPECT_EQ(*A.row(0).crbegin()++, A[0][3]);
	EXPECT_TRUE(A.row(0).begin() == A.row(0).begin());
	EXPECT_TRUE(++A.row(0).begin() == ++A.row(0).begin());
	auto r = A.row(0).begin();
	auto i = r;
	*r = 0;
	EXPECT_EQ(*r, 0);
	EXPECT_EQ(*i, 0);
	EXPECT_EQ(*++r, 1);
	EXPECT_EQ(*++r, *++(++i));
}
TEST(Board, RowBidirectionalIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	
	// --i (returntype: It&)	is decrementable
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
TEST(Board, RowRandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	EXPECT_NO_THROW(A.row(0).begin() += 2);
	EXPECT_NO_THROW(A.row(0).cbegin() += 2);
	EXPECT_NO_THROW(A.row(0).rbegin() += 2);
	EXPECT_NO_THROW(A.row(0).crbegin() += 2);
	EXPECT_NO_THROW(A.row(0).end() += (-2));
	EXPECT_EQ(*(A.row(0).begin()+=2), 2); // returns a reference to the changed object
	EXPECT_EQ(*(A.row(0).cbegin()+=2), 2);
	EXPECT_EQ(*(A.row(0).rbegin()+=1), 2);
	EXPECT_EQ(*(A.row(0).crbegin()+=1), 2);
	EXPECT_EQ(*(A.row(0).end() += (-2)), 2);
	EXPECT_EQ(*(A.row(0).cbegin()+=0), 9);
	EXPECT_FALSE(*(A.row(0).cbegin()+=2) == 5);
	EXPECT_FALSE(*(A.row(0).cbegin()+=0) == 5);

	EXPECT_NO_THROW(A.row(0).begin() + 2);	// returns a new iterator object
	EXPECT_NO_THROW(A.row(0).cbegin() + 2);
	EXPECT_NO_THROW(A.row(0).rbegin() + 1);
	EXPECT_NO_THROW(A.row(0).crbegin() + 2);
	EXPECT_NO_THROW(A.row(0).cend() + (-3));

	EXPECT_NO_THROW(2 + (Board<int,2>().row(0).begin()));
	EXPECT_NO_THROW(2 + A.row(0).cbegin());
	EXPECT_NO_THROW(2 + A.row(0).begin());
	EXPECT_NO_THROW(2 + A.row(0).rbegin());
	EXPECT_NO_THROW(2 + A.row(0).crbegin());
	EXPECT_NO_THROW((-2) + A.row(0).rend());

	EXPECT_TRUE((A.row(0).begin() + 1) == (++A.row(0).begin()));
	EXPECT_TRUE((A.row(0).cbegin() + 1) == (++A.row(0).cbegin()));
	EXPECT_TRUE((A.row(0).rbegin() + 1) == (++A.row(0).rbegin()));
	EXPECT_TRUE((A.row(0).crbegin() + 1) == (++A.row(0).crbegin()));
	EXPECT_TRUE((A.row(0).begin() + 3) == (A.row(0).begin() += 3));
	EXPECT_TRUE((A.row(0).cbegin() + 1) == (A.row(0).cbegin() += 1));
	EXPECT_TRUE((A.row(0).rbegin() + 2) == (A.row(0).rbegin() += 2));
	EXPECT_TRUE((A.row(0).crbegin() + 3) == (A.row(0).crbegin() += 3));
	EXPECT_TRUE((A.row(0).end() + (-2)) == (A.row(0).begin() +=2));
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
	EXPECT_NO_THROW(A.row(0).rend() - 1);
	EXPECT_NO_THROW(A.row(0).crend() - 2);
	EXPECT_NO_THROW(A.row(0).begin() - (-2));

	EXPECT_NO_THROW(A.row(0).end() - A.row(0).begin());
	EXPECT_NO_THROW(A.row(0).cend() - A.row(0).cbegin());
	EXPECT_NO_THROW(A.row(0).rend() - A.row(0).rbegin());
	EXPECT_NO_THROW(A.row(0).crend() - A.row(0).crbegin());
	EXPECT_TRUE((A.row(0).end() - A.row(0).begin()) == A.row(0).size());
	EXPECT_TRUE((A.row(0).begin() - A.row(0).end()) == -A.row(0).size());
	EXPECT_TRUE((A.row(0).begin() - A.row(0).end()) == -A.row(0).size());
	EXPECT_TRUE((A.row(0).cend() - A.row(0).cbegin()) == A.row(0).size());

	EXPECT_NO_THROW(A.row(0).begin()[2]);	// return value_type reference
	EXPECT_NO_THROW(A.row(0).cbegin()[2]);
	EXPECT_NO_THROW(A.row(0).rbegin()[2]);
	EXPECT_NO_THROW(A.row(0).crbegin()[2]);
	EXPECT_EQ(A.row(0).begin()[2], 2);
	EXPECT_EQ(A.row(0).cbegin()[3], 3);
	EXPECT_EQ(A.row(0).rend()[-3], *(A.row(0).rend()-3));
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
	std::random_device rd;		// non-deterministic generator
	std::mt19937 urng(rd());	// to seed mersenne twister
	EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
	EXPECT_NE(A, cA);

	EXPECT_TRUE(A.row(0).begin() < ++A.row(0).begin()); // const_iterator should move with iterator
	auto itr = A.row(1).begin();
	*itr = 9;
	EXPECT_TRUE(itr < ++A.row(1).begin());
}

namespace ColIterator
{
	using typeT = Board<int, 3>::Col;
	// all iterator categories
	static_assert(std::is_object_v<typeT::iterator>);
	static_assert(std::is_object_v<typeT::const_iterator>);
	static_assert(!std::is_const_v<typeT::iterator>);
	static_assert(!std::is_const_v<typeT::const_iterator>);
		// CopyConstructible
	static_assert(std::is_copy_constructible_v <typeT::iterator>);
	static_assert(std::is_copy_constructible_v <typeT::const_iterator>);
		// CopyAssignable
	static_assert(std::is_copy_assignable_v <typeT::iterator>);
	static_assert(std::is_copy_assignable_v <typeT::const_iterator>);
		// Destructible
	static_assert(std::is_destructible_v<typeT::iterator>);
	static_assert(std::is_destructible_v<typeT::const_iterator>);
		// Swappable (lvalues)
	//static_assert(std::is_swappable_v<typeT::iterator>);
	//static_assert(std::is_swappable_v<typeT::const_iterator>);

		// std::iterator_traits<It> has member typedefs value_type, difference_type, reference, pointer, iterator_category
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::value_type>);
	static_assert(!std::is_object_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::difference_type>);

	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::value_type>);
	static_assert(!std::is_object_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::const_iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::difference_type>);

	// forward_iterator_tag	// bidirectional_iterator_tag	// random_access_iterator_tag
	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<std::iterator_traits<typeT::iterator>::iterator_category, category>);
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_iterator>::iterator_category, category>);

	// no implementation differences / mistakes
	using type = Options<6>;
	using ColT = Board<type, 4>::Col;
	static_assert(std::is_same_v<std::iterator_traits<ColT::iterator>::value_type, type>);
	static_assert(std::is_same_v<std::iterator_traits<ColT::const_iterator>::value_type, const type>);
	static_assert(std::is_const_v<std::iterator_traits<ColT::const_iterator>::value_type>);

	// no implementation differences / mistakes
	using diff = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_iterator>::difference_type, diff>);
}
TEST(Board, ColIterator)
{
	// All iterator categories
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

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
	EXPECT_NO_THROW((Board<int, 2>::Col::iterator(A.col(2).begin())));
	EXPECT_NO_THROW((Board<int, 2>::Col::const_iterator(A.col(2).cbegin())));
	EXPECT_NO_THROW((Board<int, 2>::Col::const_iterator(cA.col(2).cbegin())));
	// *r is dereferenceable
	EXPECT_NO_THROW(*A.col(1).begin());
	EXPECT_NO_THROW(*A.col(1).cbegin());
	EXPECT_NO_THROW(*A.col(1).rbegin());
	EXPECT_NO_THROW(*A.col(1).crbegin());
	EXPECT_NO_THROW(*(cA.col(1).begin()));
	EXPECT_NO_THROW(*cA.col(1).cbegin());
	EXPECT_NO_THROW(*cA.col(1).rbegin());
	// ++r is incrementable
	EXPECT_NO_THROW(++A.col(1).begin());
	EXPECT_NO_THROW(++A.col(1).cbegin());
	EXPECT_NO_THROW(++cA.col(1).cbegin());
	EXPECT_NO_THROW(++cA.col(1).begin());
	EXPECT_NO_THROW(++A.col(1).rbegin());
	EXPECT_NO_THROW(++A.col(1).crbegin());
}
TEST(Board, ColInputIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	Board<Options<4>, 2> Opt{};
	const Board<Options<4>, 2> cOpt{};

	// == !=
	EXPECT_NO_THROW(A.col(0).begin() == A.col(0).end());
	EXPECT_NO_THROW(A.col(0).cbegin() == A.col(0).cend());
	EXPECT_NO_THROW(A.col(0).rbegin() == A.col(0).rend());
	EXPECT_NO_THROW(A.col(0).crbegin() == A.col(0).crend());
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
	EXPECT_NO_THROW(A.col(0).rbegin() != A.col(0).rend());
	EXPECT_NO_THROW(A.col(0).crbegin() != A.col(0).crend());
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
	EXPECT_NO_THROW(Opt.col(0).begin()->size());
	EXPECT_NO_THROW(Opt.col(0).cbegin()->size());
	EXPECT_NO_THROW(cOpt.col(0).cbegin()->size());
	EXPECT_NO_THROW(Opt.col(0).rbegin()->size());
	EXPECT_NO_THROW(Opt.col(0).crbegin()->size());
	EXPECT_NO_THROW((Opt.col(0).begin()->size() == (*Opt.col(0).begin()).size()));	// equivalent
	EXPECT_EQ((Opt.col(0).begin()->size()), (*Opt.col(0).begin()).size());	// equivalent
	EXPECT_EQ((cOpt.col(0).begin()->size()), (*cOpt.col(0).begin()).size());	// equivalent
	// ++i (returntype: It&)	is incrementable
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
	EXPECT_NO_THROW(total = std::accumulate(A.col(0).cbegin(), A.col(0).cend(), 0));
	EXPECT_EQ(total, 33);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.col(1).cbegin(), A.col(1).cend(), 0));
	EXPECT_EQ(total, 28);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.col(2).cbegin(), A.col(2).cend(), 0));
	EXPECT_EQ(total, 32);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.col(3).cbegin(), A.col(3).cend(), 0));
	EXPECT_EQ(total, 36);
	total = 0;

	EXPECT_NO_THROW(total = std::accumulate(A.col(2).begin(), A.col(2).end(), 0));
	EXPECT_EQ(total, 32);//2+6+10+14=32
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.col(2).cbegin(), A.col(2).cend(), 0));
	EXPECT_EQ(total, 32);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.col(2).rbegin(), A.col(2).rend(), 0));
	EXPECT_EQ(total, 32);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(cA.col(2).rbegin(), cA.col(2).rend(), 0));
	EXPECT_EQ(total, 32);
}
TEST(Board, ColOutputIterator)
{
	Board<int, 2> tmp{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";
	// *r = o (returnvalue not used)
	EXPECT_NO_THROW(*tmp.col(0).begin());	// pre-condition
	EXPECT_NO_THROW(*tmp.col(0).begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	EXPECT_NO_THROW(*(++tmp.col(0).begin()) = 4);
	EXPECT_EQ(tmp[1][0], 4);
	EXPECT_NO_THROW(*tmp.col(0).rbegin());	// pre-condition
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
	EXPECT_NO_THROW(++(*tmp.col(0).begin() = 0));	// post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.col(0).rbegin() = 9));
	EXPECT_EQ(tmp[3][0], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r);
}
TEST(Board, ColForwardIterator)
{
	using typeT = Board<int>::Col;
	static_assert(std::is_default_constructible_v<typeT::iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_iterator>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	EXPECT_NO_THROW(Board<int>::Col::iterator());
	EXPECT_NO_THROW((Board<int, 2>::Col::iterator() = A.col(2).begin()));

	EXPECT_NO_THROW(A.col(0).begin()++);
	EXPECT_EQ(*A.col(0).begin()++, A[0][0]);
	EXPECT_EQ(*A.col(0).cbegin()++, A[0][0]);
	EXPECT_EQ(*A.col(0).rbegin()++, A[3][0]);
	EXPECT_EQ(*A.col(0).crbegin()++, A[3][0]);
	EXPECT_TRUE(A.col(0).begin() == A.col(0).begin());
	EXPECT_TRUE(++A.col(0).begin() == ++A.col(0).begin());
	auto r = A.col(0).begin();
	auto i = r;
	*r = 0;
	EXPECT_EQ(*r, 0);
	EXPECT_EQ(*i, 0);
	EXPECT_EQ(*++r, 4);
	EXPECT_EQ(*++r, *++(++i));
}
TEST(Board, ColBidirectionalIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	
	// --i (returntype: It&)	is decrementable
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
TEST(Board, ColRandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	EXPECT_NO_THROW(A.col(0).begin() += 2);
	EXPECT_NO_THROW(A.col(0).cbegin() += 2);
	EXPECT_NO_THROW(A.col(0).rbegin() += 2);
	EXPECT_NO_THROW(A.col(0).crbegin() += 2);
	EXPECT_NO_THROW(A.col(0).end() += (-2));
	EXPECT_EQ(*(A.col(0).begin()+=2), 8); // returns a reference to the changed object
	EXPECT_EQ(*(A.col(0).cbegin()+=2), 8);
	EXPECT_EQ(*(A.col(0).rbegin()+=1), 8);
	EXPECT_EQ(*(A.col(0).crbegin()+=1), 8);
	EXPECT_EQ(*(A.col(0).end() += (-2)), 8);
	EXPECT_EQ(*(A.col(0).cbegin()+=0), 9);
	EXPECT_FALSE(*(A.col(0).cbegin()+=2) == 5);
	EXPECT_FALSE(*(A.col(0).cbegin()+=0) == 5);

	EXPECT_NO_THROW(A.col(0).begin() + 2);	// returns a new iterator object
	EXPECT_NO_THROW(A.col(0).cbegin() + 2);
	EXPECT_NO_THROW(A.col(0).rbegin() + 1);
	EXPECT_NO_THROW(A.col(0).crbegin() + 2);
	EXPECT_NO_THROW(A.col(0).cend() + (-3));

	EXPECT_NO_THROW(2 + (Board<int,2>().col(0).begin()));
	EXPECT_NO_THROW(2 + A.col(0).cbegin());
	EXPECT_NO_THROW(2 + A.col(0).rbegin());
	EXPECT_NO_THROW(2 + A.col(0).crbegin());
	EXPECT_NO_THROW((-2) + A.col(0).rend());

	EXPECT_TRUE((A.col(0).begin() + 1) == (++A.col(0).begin()));
	EXPECT_TRUE((A.col(0).cbegin() + 1) == (++A.col(0).cbegin()));
	EXPECT_TRUE((A.col(0).rbegin() + 1) == (++A.col(0).rbegin()));
	EXPECT_TRUE((A.col(0).crbegin() + 1) == (++A.col(0).crbegin()));
	EXPECT_TRUE((A.col(0).begin() + 3) == (A.col(0).begin() += 3));
	EXPECT_TRUE((A.col(0).cbegin() + 1) == (A.col(0).cbegin() += 1));
	EXPECT_TRUE((A.col(0).rbegin() + 2) == (A.col(0).rbegin() += 2));
	EXPECT_TRUE((A.col(0).crbegin() + 3) == (A.col(0).crbegin() += 3));
	EXPECT_TRUE((A.col(0).end() + (-2)) == (A.col(0).begin() +=2));
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
	EXPECT_NO_THROW(A.col(0).rend() - 1);
	EXPECT_NO_THROW(A.col(0).crend() - 2);
	EXPECT_NO_THROW(A.col(0).begin() - (-2));

	EXPECT_NO_THROW(A.col(0).end() - A.col(0).begin());
	EXPECT_NO_THROW(A.col(0).cend() - A.col(0).cbegin());
	EXPECT_NO_THROW(A.col(0).rend() - A.col(0).rbegin());
	EXPECT_NO_THROW(A.col(0).crend() - A.col(0).crbegin());
	EXPECT_TRUE((A.col(0).end() - A.col(0).begin()) == A.col(0).size());
	EXPECT_TRUE((A.col(0).begin() - A.col(0).end()) == -A.col(0).size());
	EXPECT_TRUE((A.col(0).begin() - A.col(0).end()) == -A.col(0).size());
	EXPECT_TRUE((A.col(0).cend() - A.col(0).cbegin()) == A.col(0).size());

	EXPECT_NO_THROW(A.col(0).begin()[2]);	// return value_type reference
	EXPECT_NO_THROW(A.col(0).cbegin()[2]);
	EXPECT_NO_THROW(A.col(0).rbegin()[2]);
	EXPECT_NO_THROW(A.col(0).crbegin()[2]);
	EXPECT_EQ(A.col(0).begin()[2], 8);
	EXPECT_EQ(A.col(0).cbegin()[3], 12);
	EXPECT_EQ(A.col(0).rend()[-3], *(A.col(0).rend()-3));
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
	std::random_device rd;		// non-deterministic generator
	std::mt19937 urng(rd());	// to seed mersenne twister
	EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
	EXPECT_NE(A, cA);
}

namespace BlockIterator
{
	using typeT = Board<int, 3>::Block;
	// all iterator categories
	static_assert(std::is_object_v<typeT::iterator>);
	static_assert(std::is_object_v<typeT::const_iterator>);
	static_assert(!std::is_const_v<typeT::iterator>);
	static_assert(!std::is_const_v<typeT::const_iterator>);
		// CopyConstructible
	static_assert(std::is_copy_constructible_v <typeT::iterator>);
	static_assert(std::is_copy_constructible_v <typeT::const_iterator>);
		// CopyAssignable
	static_assert(std::is_copy_assignable_v <typeT::iterator>);
	static_assert(std::is_copy_assignable_v <typeT::const_iterator>);
		// Destructible
	static_assert(std::is_destructible_v<typeT::iterator>);
	static_assert(std::is_destructible_v<typeT::const_iterator>);
		// Swappable (lvalues)
	//static_assert(std::is_swappable_v<typeT::iterator>);
	//static_assert(std::is_swappable_v<typeT::const_iterator>);

		// std::iterator_traits<It> has member typedefs value_type, difference_type, reference, pointer, iterator_category
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::value_type>);
	static_assert(!std::is_object_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::iterator>::difference_type>);

	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::iterator_category>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::value_type>);
	static_assert(!std::is_object_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_lvalue_reference_v<std::iterator_traits<typeT::const_iterator>::reference>);
	static_assert(std::is_pointer_v<std::iterator_traits<typeT::const_iterator>::pointer>);
	static_assert(std::is_object_v<std::iterator_traits<typeT::const_iterator>::difference_type>);

	// forward_iterator_tag	// bidirectional_iterator_tag	// random_access_iterator_tag
	using category = std::random_access_iterator_tag;
	static_assert(std::is_same_v<std::iterator_traits<typeT::iterator>::iterator_category, category>);
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_iterator>::iterator_category, category>);

	// no implementation differences / mistakes
	using type = Options<6>;
	using ColT = Board<type, 4>::Col;
	static_assert(std::is_same_v<std::iterator_traits<ColT::iterator>::value_type, type>);
	static_assert(std::is_same_v<std::iterator_traits<ColT::const_iterator>::value_type, const type>);
	static_assert(std::is_const_v<std::iterator_traits<ColT::const_iterator>::value_type>);

	// no implementation differences / mistakes
	using diff = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_same_v<std::iterator_traits<typeT::const_iterator>::difference_type, diff>);
}
TEST(Board, BlockIterator)
{
	// All iterator categories
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

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
	EXPECT_NO_THROW((Board<int, 2>::Block::iterator(A.block(2).begin())));
	EXPECT_NO_THROW((Board<int, 2>::Block::const_iterator(A.block(2).cbegin())));
//	EXPECT_NO_THROW((Board<int, 2>::Block::const_iterator(cA.block(2).cbegin())));
	// *r is dereferenceable
	EXPECT_NO_THROW(*A.block(1).begin());
	EXPECT_NO_THROW(*A.block(1).cbegin());
	EXPECT_NO_THROW(*A.block(1).rbegin());
	EXPECT_NO_THROW(*A.block(1).crbegin());
	EXPECT_NO_THROW(*(cA.block(1).begin()));
	EXPECT_NO_THROW(*cA.block(1).cbegin());
	EXPECT_NO_THROW(*cA.block(1).rbegin());
	// ++r is incrementable
	EXPECT_NO_THROW(++A.block(1).begin());
	EXPECT_NO_THROW(++A.block(1).cbegin());
	EXPECT_NO_THROW(++cA.block(1).cbegin());
	EXPECT_NO_THROW(++cA.block(1).begin());
	EXPECT_NO_THROW(++A.block(1).rbegin());
	EXPECT_NO_THROW(++A.block(1).crbegin());
}
TEST(Board, BlockInputIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	Board<Options<4>, 2> Opt{};
	const Board<Options<4>, 2> cOpt{};

	// == !=
	EXPECT_NO_THROW(A.block(0).begin() == A.block(0).end());
	EXPECT_NO_THROW(A.block(0).cbegin() == A.block(0).cend());
	EXPECT_NO_THROW(A.block(0).rbegin() == A.block(0).rend());
	EXPECT_NO_THROW(A.block(0).crbegin() == A.block(0).crend());
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
	EXPECT_NO_THROW(A.block(0).rbegin() != A.block(0).rend());
	EXPECT_NO_THROW(A.block(0).crbegin() != A.block(0).crend());
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
	EXPECT_NO_THROW(Opt.block(0).begin()->size());
	EXPECT_NO_THROW(Opt.block(0).cbegin()->size());
	EXPECT_NO_THROW(cOpt.block(0).cbegin()->size());
	EXPECT_NO_THROW(Opt.block(0).rbegin()->size());
	EXPECT_NO_THROW(Opt.block(0).crbegin()->size());
	EXPECT_NO_THROW((Opt.block(0).begin()->size() == (*Opt.block(0).begin()).size()));	// equivalent
	EXPECT_EQ((Opt.block(0).begin()->size()), (*Opt.block(0).begin()).size());	// equivalent
	EXPECT_EQ((cOpt.block(0).begin()->size()), (*cOpt.block(0).begin()).size());	// equivalent
	// ++i (returntype: It&)	is incrementable
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
	EXPECT_NO_THROW(total = std::accumulate(A.block(0).cbegin(), A.block(0).cend(), 0));
	EXPECT_EQ(total, 19);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.block(1).cbegin(), A.block(1).cend(), 0));
	EXPECT_EQ(total, 18);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.block(2).cbegin(), A.block(2).cend(), 0));
	EXPECT_EQ(total, 42);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.block(3).cbegin(), A.block(3).cend(), 0));
	EXPECT_EQ(total, 50);
	total = 0;

	EXPECT_NO_THROW(total = std::accumulate(A.block(2).begin(), A.block(2).end(), 0));
	EXPECT_EQ(total, 42);//2+6+10+14=32
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.block(2).cbegin(), A.block(2).cend(), 0));
	EXPECT_EQ(total, 42);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(A.block(2).rbegin(), A.block(2).rend(), 0));
	EXPECT_EQ(total, 42);
	total = 0;
	EXPECT_NO_THROW(total = std::accumulate(cA.block(2).rbegin(), cA.block(2).rend(), 0));
	EXPECT_EQ(total, 42);
}
TEST(Board, BlockOutputIterator)
{
	Board<int, 2> tmp{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	EXPECT_EQ(tmp[0][0], 0) << "requires InBetween";
	// *r = o (returnvalue not used)
	EXPECT_NO_THROW(*tmp.block(0).begin());	// pre-condition
	EXPECT_NO_THROW(*tmp.block(0).begin() = 5);
	EXPECT_EQ(tmp[0][0], 5);
	EXPECT_NO_THROW(*(++tmp.block(0).begin()) = 4);
	EXPECT_EQ(tmp[0][1], 4);
	EXPECT_NO_THROW(*tmp.block(0).rbegin());	// pre-condition
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
	EXPECT_NO_THROW(++(*tmp.block(0).begin() = 0));	// post-condition
	EXPECT_EQ(tmp[0][0], 1);
	EXPECT_NO_THROW(++(*tmp.block(0).rbegin() = 9));
	EXPECT_EQ(tmp[1][1], 10);

	auto r = tmp.begin();
	EXPECT_TRUE(&r == &++r);
}
TEST(Board, BlockForwardIterator)
{
	using typeT = Board<int>::Block;
	static_assert(std::is_default_constructible_v<typeT::iterator>);
	static_assert(std::is_default_constructible_v<typeT::const_iterator>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	EXPECT_NO_THROW(Board<int>::Block::iterator());
	EXPECT_NO_THROW((Board<int, 2>::Block::iterator() = A.block(2).begin()));

	EXPECT_NO_THROW(A.block(0).begin()++);
	EXPECT_EQ(*A.block(0).begin()++, A[0][0]);
	EXPECT_EQ(*A.block(0).cbegin()++, A[0][0]);
	EXPECT_EQ(*A.block(0).rbegin()++, A[1][1]);
	EXPECT_EQ(*A.block(0).crbegin()++, A[1][1]);
	EXPECT_TRUE(A.block(0).begin() == A.block(0).begin());
	EXPECT_TRUE(++A.block(0).begin() == ++A.block(0).begin());
	auto r = A.block(0).begin();
	auto i = r;
	*r = 0;
	EXPECT_EQ(*r, 0);
	EXPECT_EQ(*i, 0);
	EXPECT_EQ(*++r, 1);
	EXPECT_EQ(*++r, *++(++i));
}
TEST(Board, BlockBidirectionalIterator)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	
	// --i (returntype: It&)	is decrementable
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
TEST(Board, BlockRandomAccessIterator)
{
	using typeT = Board<int, 2>;
	using diff = std::iterator_traits<typeT::iterator>::difference_type;
	static_assert(std::is_integral_v<diff>);

	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	EXPECT_NO_THROW(A.block(0).begin() += 2);
	EXPECT_NO_THROW(A.block(0).cbegin() += 2);
	EXPECT_NO_THROW(A.block(0).rbegin() += 2);
	EXPECT_NO_THROW(A.block(0).crbegin() += 2);
	EXPECT_NO_THROW(A.block(0).end() += (-2));
	EXPECT_EQ(*(A.block(0).begin()+=2), 4); // returns a reference to the changed object
	EXPECT_EQ(*(A.block(0).cbegin()+=2), 4);
	EXPECT_EQ(*(A.block(0).rbegin()+=1), 4);
	EXPECT_EQ(*(A.block(0).crbegin()+=1), 4);
	EXPECT_EQ(*(A.block(0).end() += (-2)), 4);
	EXPECT_EQ(*(A.block(0).cbegin()+=0), 9);
	EXPECT_FALSE(*(A.block(0).cbegin()+=2) == 5);
	EXPECT_FALSE(*(A.block(0).cbegin()+=0) == 5);

	EXPECT_NO_THROW(A.block(0).begin() + 2);	// returns a new iterator object
	EXPECT_NO_THROW(A.block(0).cbegin() + 2);
	EXPECT_NO_THROW(A.block(0).rbegin() + 1);
	EXPECT_NO_THROW(A.block(0).crbegin() + 2);
	EXPECT_NO_THROW(A.block(0).cend() + (-3));

	EXPECT_NO_THROW(2 + (Board<int,2>().block(0).begin()));
	EXPECT_NO_THROW(2 + A.block(0).cbegin());
	EXPECT_NO_THROW(2 + A.block(0).rbegin());
	EXPECT_NO_THROW(2 + A.block(0).crbegin());
	EXPECT_NO_THROW((-2) + A.block(0).rend());

	EXPECT_TRUE((A.block(0).begin() + 1) == (++A.block(0).begin()));
	EXPECT_TRUE((A.block(0).cbegin() + 1) == (++A.block(0).cbegin()));
	EXPECT_TRUE((A.block(0).rbegin() + 1) == (++A.block(0).rbegin()));
	EXPECT_TRUE((A.block(0).crbegin() + 1) == (++A.block(0).crbegin()));
	EXPECT_TRUE((A.block(0).begin() + 3) == (A.block(0).begin() += 3));
	EXPECT_TRUE((A.block(0).cbegin() + 1) == (A.block(0).cbegin() += 1));
	EXPECT_TRUE((A.block(0).rbegin() + 2) == (A.block(0).rbegin() += 2));
	EXPECT_TRUE((A.block(0).crbegin() + 3) == (A.block(0).crbegin() += 3));
	EXPECT_TRUE((A.block(0).end() + (-2)) == (A.block(0).begin() +=2));
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
	EXPECT_NO_THROW(A.block(0).rend() - 1);
	EXPECT_NO_THROW(A.block(0).crend() - 2);
	EXPECT_NO_THROW(A.block(0).begin() - (-2));

	EXPECT_NO_THROW(A.block(0).end() - A.block(0).begin());
	EXPECT_NO_THROW(A.block(0).cend() - A.block(0).cbegin());
	EXPECT_NO_THROW(A.block(0).rend() - A.block(0).rbegin());
	EXPECT_NO_THROW(A.block(0).crend() - A.block(0).crbegin());
	EXPECT_TRUE((A.block(0).end() - A.block(0).begin()) == A.block(0).size());
	EXPECT_TRUE((A.block(0).begin() - A.block(0).end()) == -A.block(0).size());
	EXPECT_TRUE((A.block(0).begin() - A.block(0).end()) == -A.block(0).size());
	EXPECT_TRUE((A.block(0).cend() - A.block(0).cbegin()) == A.block(0).size());

	EXPECT_NO_THROW(A.block(0).begin()[2]);	// return value_type reference
	EXPECT_NO_THROW(A.block(0).cbegin()[2]);
	EXPECT_NO_THROW(A.block(0).rbegin()[2]);
	EXPECT_NO_THROW(A.block(0).crbegin()[2]);
	EXPECT_EQ(A.block(0).begin()[2], 4);
	EXPECT_EQ(A.block(0).cbegin()[3], 5);
	EXPECT_EQ(A.block(0).rend()[-3], *(A.block(0).rend()-3));
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
	std::random_device rd;		// non-deterministic generator
	std::mt19937 urng(rd());	// to seed mersenne twister
	EXPECT_NO_THROW(std::shuffle(A.begin(), A.end(), urng));
	EXPECT_NE(A, cA);
}


template<typename T>
int range_for_sum(T structure)
{
	int total{};
	try
	{
		for (auto& s : structure) { total += s; }
	}
	catch (...)
	{
		ADD_FAILURE() << "rang for sum failed on supplied data";
	}
	return total;
}

TEST(Board, IteratorLoop)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };

	int total{};
	// manual
	for (auto itr = A.begin(); itr < A.end(); ++itr)
	{
		total += *itr;
	}
	EXPECT_EQ(total, 129);

	total = 0;
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
	total = 0;
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

TEST(Board, IteratorDeathtest)
{
	Board<int, 2> A{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Board<int, 2> cA{ 9,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	// iterator
	// compatible_(offset)
	EXPECT_DEBUG_DEATH({ cA.row(0).begin() += 4; },
		"Assertion failed: .*") << "const_row_iterator out of bounds";
	EXPECT_DEBUG_DEATH({ A.row(0).begin() += 4; },
		"Assertion failed: .*") << "row_iterator out of bounds";
	EXPECT_DEBUG_DEATH({ cA.col(3).begin() += 4; },
		"Assertion failed: .*") << "const_col_iterator out of bounds";
	EXPECT_DEBUG_DEATH({ A.col(3).begin() += 4; },
		"Assertion failed: .*") << "col_iterator out of bounds";
	EXPECT_DEBUG_DEATH({ cA.block(1).begin() += 4; },
		"Assertion failed: .*") << "const_block_iterator out of bounds";
	EXPECT_DEBUG_DEATH({ A.block(2).begin() += 4; },
		"Assertion failed: .*") << "block_iterator out of bounds";
}

}