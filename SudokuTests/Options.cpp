﻿/**	Unit tests for the template class Sudoku::Options
*
*/
#include "gtest/gtest.h"

// Class under test
#include "../Sudoku/Options.h"
// additional
#include <type_traits>

using namespace Sudoku;

namespace SudokuTests::Sudoku_Test
{
namespace compiletime_Location
{
// Class properties
using typeT = Sudoku::Options<9>;
static_assert(std::is_class<typeT>(), "a class, hiding datarepresentation");
static_assert(!std::is_empty<typeT>(), "no datamembers");
static_assert(std::is_standard_layout<typeT>(), "standard layout");
static_assert(!std::is_trivial<typeT>(), "not user-provided, no virtuals");
static_assert(!std::is_pod<typeT>(), "Plain Old Data, both trivial and standard-layout");
//static_assert(std::has_unique_object_representations<typeT>(), "");	//C++17

static_assert(std::is_default_constructible<typeT>(), "default constructor");
static_assert(std::is_nothrow_default_constructible<typeT>(), "notrow default constructor");
static_assert(!std::is_trivially_default_constructible<typeT>(), "tribial default constructor");

static_assert(std::is_destructible<typeT>(), "destructable");
static_assert(std::is_nothrow_destructible<typeT>(), "nothrow destructable");
static_assert(std::is_trivially_destructible<typeT>(), "trivially destructable");

static_assert(std::is_copy_constructible<typeT>(), "copy constructor");
static_assert(std::is_nothrow_copy_constructible<typeT>(), "notrow copy constructor");
static_assert(std::is_trivially_copy_constructible<typeT>(), "trivially copy constructor");

static_assert(std::is_move_constructible<typeT>(), "move constructor");
static_assert(std::is_nothrow_move_constructible<typeT>(), "nothrow move constructor");
static_assert(std::is_trivially_move_constructible<typeT>(), "trivially move constructor");

static_assert(std::is_copy_assignable<typeT>(), "copy assignable");
static_assert(std::is_nothrow_copy_assignable<typeT>(), "notrow copy assignable");
static_assert(std::is_trivially_copy_assignable<typeT>(), "trivially copy assignable");

static_assert(std::is_move_assignable<typeT>(), "move assignable");
static_assert(std::is_nothrow_move_assignable<typeT>(), "move assignable");
static_assert(std::is_trivially_move_assignable<typeT>(), "trivially move assignable");

static_assert(std::is_trivially_copyable<typeT>(), "trivially copyable");
static_assert(std::is_swappable<typeT>(), "swappable");			//C++17
static_assert(std::is_nothrow_swappable<typeT>(), "swappable");	//C++17

static_assert(!std::is_swappable_with<typeT, std::bitset<10>>(), "");	//C++17
static_assert(!std::is_nothrow_swappable_with<typeT, std::bitset<10>>(), "");	//C++17

																				// type construction
static_assert(std::is_constructible<Options<3>, const std::bitset<4>&>(), "construct from const std::bitset&");
static_assert(!std::is_constructible<Options<3>, const std::bitset<3>&>(), "shouldn't accept non matching dimensions_1");
static_assert(std::is_assignable<Options<3>, std::bitset<4>>(), "assign from std::bitset");
static_assert(!std::is_assignable<Options<3>, std::bitset<3>>(), "shouldn't accept non matching dimensions_2");
static_assert(std::is_constructible<Options<3>, int>(), "construct from int");
static_assert(std::is_constructible<Options<3>, unsigned int>(), "construct from unsigned int");
static_assert(std::is_assignable<Options<3>, int>(), "assign from int");
static_assert(std::is_assignable<Options<3>, unsigned int>(), "assign from int");
}
TEST(Options, Construction)
{
	EXPECT_NO_THROW(Sudoku::Options<9> two{});
	EXPECT_NO_THROW(Sudoku::Options<4> O_3{ std::bitset<5>() });
	EXPECT_NO_THROW(Sudoku::Options<4> O_3{ std::bitset<5>{"00100"} });
	EXPECT_NO_THROW(Sudoku::Options<4> O_4{ 2 });
}
TEST(Options, MemberFunctions)
{
	// _test-data
	Sudoku::Options<9> O_1{};
	Sudoku::Options<9> O_2{};
	const Sudoku::Options<4> O_3{ std::bitset<5>{"00100"} };	// answer = 2
	const Sudoku::Options<4> O_4{ std::bitset<5>{"01011"} };	// 2 options 1 & 3
	const Sudoku::Options<4> E_1{ std::bitset<5>{"00000"} };	// empty
	const Sudoku::Options<4> E_2{ std::bitset<5>{"00001"} };	// empty, no answer
	Sudoku::Options<9> TMP{};

	///// Const Memberfunctions /////
	// size() const
	static_assert(noexcept(O_1.size()), "size() should be noexcept");
	EXPECT_TRUE(O_1.size() == 10);
	EXPECT_TRUE(E_1.size() == 5);
	// count() const
	static_assert(noexcept(O_1.count()), "count() should be noexcept");
	EXPECT_TRUE(O_1.count() == 9);
	EXPECT_TRUE(O_3.count() == 0);
	EXPECT_TRUE(O_4.count() == 2);
	EXPECT_TRUE(E_1.count() == 0);
	EXPECT_TRUE(E_2.count() == 0);
	// count_all() const
	static_assert(noexcept(O_1.count_all()), "count_all() should be noexcept");
	EXPECT_TRUE(O_1.count_all() == 9);
	EXPECT_TRUE(O_3.count_all() == 1);
	EXPECT_TRUE(O_4.count_all() == 2);
	EXPECT_TRUE(E_1.count_all() == 0);
	EXPECT_TRUE(E_2.count_all() == 0);
	// all() const
	static_assert(noexcept(O_1.all()), "all() should be noexcept");
	EXPECT_TRUE(O_1.all());
	EXPECT_FALSE(O_3.all());
	//_test(int) const
	static_assert(!noexcept(O_1.test(2)), "test() is NOT noexcept");
	EXPECT_TRUE(O_1.test(2));
	EXPECT_TRUE(O_3.test(2));
	EXPECT_FALSE(O_3.test(1));
	try { if (O_1.test(15) || true) { FAIL(); } }
	catch (const std::out_of_range&) {} // caught
	try { if (O_1.test(-1) || true) { FAIL() << "test(-1) out-of-range expected"; } }
	catch (const std::out_of_range&) {} // caught
	catch (...) { FAIL() << "test() unexpected exception"; }
	// is_answer() const
	static_assert(noexcept(O_1.is_answer()), "is_answer() should be noexcept");
	EXPECT_TRUE(O_3.is_answer());
	EXPECT_FALSE(O_1.is_answer());
	EXPECT_FALSE(O_4.is_answer());
	EXPECT_FALSE(E_2.is_answer());
	// is_answer(int) const
	static_assert(noexcept(O_1.is_answer(1)), "is_answer(int) is NOT noexcept");
	EXPECT_TRUE(O_3.is_answer(2));
	EXPECT_FALSE(O_3.is_answer(1));
	EXPECT_FALSE(O_1.is_answer(2));
	EXPECT_FALSE(O_1.is_answer(17));
	EXPECT_FALSE(O_1.is_answer(0));
	EXPECT_FALSE(O_1.is_answer(-1));
	// empty with answer flag
	EXPECT_FALSE(E_1.is_answer());
	EXPECT_FALSE(E_1.is_answer(0));
	// is_option(int) const
	static_assert(noexcept(O_1.is_option(2)), "is_option(int) is NOT noexept");
	EXPECT_TRUE(O_1.is_option(1));
	EXPECT_FALSE(O_3.is_option(1));
	EXPECT_FALSE(O_3.is_option(2));
	// is_empty() const
	//constexpr bool is_empty() const noexcept;
	static_assert(noexcept(O_1.is_empty()), "is_empty() should be noexcept");
	EXPECT_TRUE(E_1.is_empty());
	EXPECT_FALSE(O_1.is_empty());
	EXPECT_TRUE(E_2.is_empty());
	// available() const
	//std::vector<int> available() const;	// return available options
	static_assert(!noexcept(O_1.available()), "available() should NOT be noexcept");
	std::vector<int> result{};
	try { result = O_4.available(); }
	catch (...) { FAIL() << "available() failed"; }
	EXPECT_TRUE(result.size() == 2);
	EXPECT_TRUE(result[0] == 1);
	EXPECT_TRUE(result[1] == 3);
	try { result = E_1.available(); }
	catch (...) { FAIL() << "available() on empty failed"; }
	EXPECT_TRUE(result.size() == 0);
	try { result = E_2.available(); }
	catch (...) { FAIL() << "available() on empty failed_2"; }
	EXPECT_TRUE(result.size() == 0);
	// get_answer() const
	//int get_answer() const noexcept;		// return get_answer or 0
	static_assert(noexcept(O_1.get_answer()), "get_answer() should be noexcept");
	EXPECT_TRUE(O_3.get_answer() == 2);
	EXPECT_TRUE(O_2.get_answer() == 0);
	EXPECT_TRUE(E_1.get_answer() == 0);

	///// NON-CONST Memberfunctions /////
	// clear()
	static_assert(noexcept(O_1.clear()), "clear() should be noexcept");
	O_2.clear();
	EXPECT_TRUE(O_2.is_empty());
	EXPECT_TRUE(O_2.count() == 0);
	EXPECT_FALSE(O_2.is_answer());
	EXPECT_FALSE(O_2.is_answer(2));
	EXPECT_FALSE(O_2.is_option(2));
	// reset()
	static_assert(noexcept(O_1.reset()), "reset() should be noexcept");
	O_2.reset();
	EXPECT_FALSE(O_2.is_empty());
	EXPECT_TRUE(O_2.count() == 9);
	EXPECT_TRUE(O_2.count_all() == 9);
	EXPECT_FALSE(O_2.is_answer());
	EXPECT_FALSE(O_2.is_answer(2));
	EXPECT_TRUE(O_2.is_option(2));
	EXPECT_TRUE(O_2.all());
	// flip()
	static_assert(noexcept(O_1.flip()), "flip() should be noexcept");
	O_2.flip();
	EXPECT_TRUE(O_2.is_empty());
	EXPECT_TRUE(O_2.count_all() == 0);
	// remove_option(int)
	TMP.reset();
	static_assert(!noexcept(TMP.remove_option(3)), "remove_option(int) should be noexcept");
	EXPECT_TRUE(TMP.remove_option(3));
	EXPECT_TRUE(TMP.count() == 8);
	EXPECT_FALSE(TMP.remove_option(3));
	// add(int)
	//Options& add(int value);			// add single option
	static_assert(!noexcept(O_1.add(4)), "add(int) should NOT be noexcept");
	try { O_1.add(4); }
	catch (...) { FAIL() << "add(int) failed"; }
	try { O_1.add(12); }
	catch (const std::out_of_range&) {} // caught
	catch (...) { FAIL() << "add(high val) exception wasn't caught"; }
	// set(int)
	static_assert(!noexcept(O_1.set(4)), "set(int) should NOT be noexcept");
	EXPECT_TRUE(O_1.set(4).is_answer());
	EXPECT_TRUE(O_1.is_answer(4));
	EXPECT_TRUE(O_1.count() == 0);
	EXPECT_TRUE(O_1.set(1).is_answer(1));
	EXPECT_FALSE(O_1.is_answer(4));
	EXPECT_TRUE(O_1.set(0).is_empty());
	//add_noexcept(int)
	TMP.clear();
	static_assert(noexcept(TMP.add_nocheck(1)), "add_noexcept(int) should be noexcept");
	TMP.add_nocheck(3);
	EXPECT_FALSE(TMP.is_empty());
	EXPECT_FALSE(TMP.count_all() == 0);
	EXPECT_TRUE(TMP.test(3));
	EXPECT_TRUE(TMP.add_nocheck(4).test(4));
	//set_noexcept(int)
	static_assert(noexcept(O_1.set_nocheck(2)), "set_noexcept(int) should be noexcept");
	EXPECT_TRUE(O_1.set_nocheck(2).is_answer(2));
}
	TEST(Options, Operators)
	{
		// _test-data
		const Sudoku::Options<4> O_1{};								// all options
		const Sudoku::Options<4> O_2{ std::bitset<5>{"11111"} };	// all options
		const Sudoku::Options<4> O_3{ std::bitset<5>{"00101"} };	// single option 2
		const Sudoku::Options<4> E_1{ 0 };							// empty answer "00001"
		const Sudoku::Options<4> E_2{ std::bitset<5>{"00000"} };	// empty
		const Sudoku::Options<4> E_3{ std::bitset<5>{"00001"} };	// empty option
		const Sudoku::Options<4> A_1{ 1 };							// answer 1
		const Sudoku::Options<4> A_2{ std::bitset<5>{"00100"} };	// answer 2
		Sudoku::Options<4> TMP{};									// per test, reset this option

																	///// CONST operators /////
																	// operator==(int) const
		static_assert(noexcept(A_1 == 1), "operator==(int) should be noexcept");
		EXPECT_TRUE(A_1 == 1);
		EXPECT_TRUE(A_2 == 2);
		//bool operator==(Options<E>&) const
		static_assert(noexcept(O_1 == O_2), "operator== should be noexcept");
		EXPECT_TRUE(O_1 == O_2);
		EXPECT_FALSE(O_1 == E_1);
		//bool operator!=(Options<E>&) const
		static_assert(noexcept(O_1 != O_2), "operator!= should be noexcept");
		EXPECT_TRUE(O_1 != E_1);
		EXPECT_FALSE(O_1 != O_2);
		//NEEDTEST bool operator< (Options<E>&) const			sorting
		//Options operator&(Options&) const						shared options
		static_assert(noexcept(O_1 & O_2), "operator& should be noexcept");
		EXPECT_TRUE((O_2 & O_3) == O_3);
		EXPECT_TRUE((O_1 & O_2) == O_1);
		EXPECT_TRUE((O_2 & E_2) == E_2);
		EXPECT_TRUE((O_3 & A_2) == A_2);
		EXPECT_TRUE((E_3 & A_2) == E_2);
		//constexpr bool operator[](int) const
		static_assert(noexcept(O_1[2]), "operator[int] should be no except for reading");
		static_assert(noexcept(O_1[14]), "operator[out of range] should not throw an exception");
		EXPECT_TRUE(O_1[2]);
		EXPECT_TRUE(A_2[2]);
		EXPECT_TRUE(A_2[1] == false);

		///// non-const operators /////
		//NEEDTEST constexpr bool operator[](int)
		static_assert(noexcept(TMP.operator[](0) = true), "operator[] should be noexcept");
		TMP.clear();
		EXPECT_TRUE(TMP[0] = true);
		EXPECT_TRUE(TMP[0] == true);
		EXPECT_FALSE(TMP[0].flip());
		EXPECT_TRUE(TMP[0] == false);
		//Options& XOR(Options&)			XOR
		static_assert(noexcept(TMP.XOR(O_3)), "XOR() should be noexcept");
		TMP = E_3;
		EXPECT_TRUE(TMP.XOR(A_2) == O_3);
		EXPECT_TRUE((TMP.XOR(A_2)) == E_3);
		//Options& operator+=(Options&)			combine options
		static_assert(noexcept(TMP += O_2), "operator+= should be noexcept");
		TMP = E_2;
		TMP += O_2;
		EXPECT_TRUE(O_1 == O_2);
		// others tested by using operator+()
		//Options operator+(Options&) const		combine options
		static_assert(noexcept(O_1 + O_2), "operator+ should be noexcept");
		EXPECT_TRUE(O_1 + O_2 == O_1);
		EXPECT_TRUE(E_1 + O_1 == O_1);
		EXPECT_TRUE(A_2 + E_3 == O_3);
		EXPECT_FALSE(E_1 + A_2 == O_3);
		//Options operator-(Options&) const		difference
		static_assert(noexcept(O_1 - O_2), "operator- should be noexcept");


		///// testing the constructors /////
		EXPECT_TRUE(E_1 == E_3);
		// copy-assign
		static_assert(noexcept(TMP.operator=(O_2)), "operator= should be noexcept");
		static_assert(!noexcept(TMP.operator=(1)), "operator=(int) IS NOT noexcept");
		Sudoku::Options<4> TMP1 = A_2;
		EXPECT_TRUE(TMP1.is_answer(2));
		EXPECT_TRUE(TMP1 == A_2);
		Sudoku::Options<4> TMP2 = 3;
		EXPECT_TRUE(TMP2.is_answer(3));
		// move-assign
		static_assert(noexcept(TMP.operator=(std::bitset<5>())), "operator= should be noexcept_2");
		Sudoku::Options<4> O_6 = std::bitset<5>{};	// "00000"
		EXPECT_TRUE(O_6 == E_2);
	}
	TEST(Options, External)
	{
		const Sudoku::Options<4> O_1{};								// all options
		const Sudoku::Options<4> O_2{ std::bitset<5>{"11111"} };	// all options
		const Sudoku::Options<4> O_3{ std::bitset<5>{"00101"} };	// single option 2
		const Sudoku::Options<4> E_1{ 0 };							// empty answer "00001"
		const Sudoku::Options<4> E_2{ std::bitset<5>{"00000"} };	// empty
		const Sudoku::Options<4> E_3{ std::bitset<5>{"00001"} };	// empty option
		const Sudoku::Options<4> A_1{ 1 };							// answer 1
		const Sudoku::Options<4> A_2{ std::bitset<5>{"00100"} };	// answer 2
																	//XOR(a,b)
		static_assert(noexcept(XOR(O_3, O_3)), "XOR() should be noexcept");
		EXPECT_TRUE(XOR(E_3, A_2) == O_3);
		EXPECT_TRUE(XOR(XOR(E_3, A_2), A_2) == E_3);
		//shared(a,b)
		static_assert(noexcept(shared(O_1, O_2), "share(a,b) should be noexcept"));
		EXPECT_TRUE(shared(O_2, O_3) == O_3);
		EXPECT_TRUE(shared(O_1, O_2) == O_1);
		EXPECT_TRUE(shared(O_2, E_2) == E_2);
		EXPECT_TRUE(shared(O_3, A_2) == A_2);
		EXPECT_TRUE(shared(E_3, A_2) == E_2);
	}
}	//namespace Sudoku_Test
