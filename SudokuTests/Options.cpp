/**	Unit tests for the template class Sudoku::Options
*
*/
#include <gtest/gtest.h>

// Class under test
#include "../Sudoku/Options.h"
// additional
#include <bitset>
#include <vector>
#include <type_traits>
#include <ostream>

// Teaching gTest how to print Options
namespace Sudoku
{
	template<int N>
	std::ostream& operator<<(std::ostream& os, const Options<N>& Options)
	{
		return os << Options.DebugString();
	}
}

using namespace Sudoku;

namespace SudokuTests::OptionsTest
{
namespace compiletime
{
// Class properties
	using typeT = Sudoku::Options<9>;
	static_assert(std::is_class_v<typeT>, "a class, hiding datarepresentation");
	static_assert(!std::is_empty_v<typeT>, "no datamembers");
	static_assert(std::is_standard_layout_v<typeT>, "standard layout");
	static_assert(!std::is_trivial_v<typeT>, "not user-provided, no virtuals");
	static_assert(!std::is_pod_v<typeT>, "Plain Old Data, both trivial and standard-layout");
	//static_assert(std::has_unique_object_representations_v<typeT>, "");	//C++17

	static_assert(std::is_default_constructible_v<typeT>, "default constructor");
	static_assert(std::is_nothrow_default_constructible_v<typeT>, "notrow default constructor");
	static_assert(!std::is_trivially_default_constructible_v<typeT>, "tribial default constructor");

	static_assert(std::is_destructible_v<typeT>, "destructable");
	static_assert(std::is_nothrow_destructible_v<typeT>, "nothrow destructable");
	static_assert(std::is_trivially_destructible_v<typeT>, "trivially destructable");

	static_assert(std::is_copy_constructible_v<typeT>, "copy constructor");
	static_assert(std::is_nothrow_copy_constructible_v<typeT>, "notrow copy constructor");
	static_assert(std::is_trivially_copy_constructible_v<typeT>, "trivially copy constructor");

	static_assert(std::is_move_constructible_v<typeT>, "move constructor");
	static_assert(std::is_nothrow_move_constructible_v<typeT>, "nothrow move constructor");
	static_assert(std::is_trivially_move_constructible_v<typeT>, "trivially move constructor");

	static_assert(std::is_copy_assignable_v<typeT>, "copy assignable");
	static_assert(std::is_nothrow_copy_assignable_v<typeT>, "notrow copy assignable");
	static_assert(std::is_trivially_copy_assignable_v<typeT>, "trivially copy assignable");

	static_assert(std::is_move_assignable_v<typeT>, "move assignable");
	static_assert(std::is_nothrow_move_assignable_v<typeT>, "move assignable");
	static_assert(std::is_trivially_move_assignable_v<typeT>, "trivially move assignable");

	static_assert(std::is_trivially_copyable_v<typeT>, "trivially copyable");
	static_assert(std::is_swappable_v<typeT>, "swappable");			//C++17
	static_assert(std::is_nothrow_swappable_v<typeT>, "swappable");	//C++17

	static_assert(!std::is_swappable_with<typeT, std::bitset<10>>::value, "");	//C++17
	static_assert(!std::is_nothrow_swappable_with_v<typeT, std::bitset<10>>, "");	//C++17

	// type construction
	static_assert(std::is_constructible_v<Options<3>, const std::bitset<4>&>, "construct from const std::bitset&");
	static_assert(!std::is_constructible_v<Options<3>, const std::bitset<3>&>, "shouldn't accept non matching dimensions_1");
	static_assert(std::is_assignable_v<Options<3>, std::bitset<4>>, "assign from std::bitset");
	static_assert(!std::is_assignable_v<Options<3>, std::bitset<3>>, "shouldn't accept non matching dimensions_2");
	static_assert(std::is_constructible_v<Options<3>, int>, "construct from int");
	static_assert(std::is_constructible_v<Options<3>, unsigned int>, "construct from unsigned int");
	static_assert(std::is_assignable_v<Options<3>, int>, "assign from int");
	static_assert(std::is_assignable_v<Options<3>, unsigned int>, "assign from int");
}
TEST(Options, Construction)
{
	const Options<4> D_0{};
	{
		SCOPED_TRACE("Default Constructor : Options()");
		const Options<4> D;
		EXPECT_EQ(D_0.size(), 5);
		EXPECT_EQ(D_0, D_0)	<< "operator== failed, required";
		EXPECT_EQ(D_0, D);
		EXPECT_EQ(D_0.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Copy Constructor : Options(const Options&)");
		const Options<4> Opt(D_0);
		EXPECT_EQ(Opt.size(), 5);
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Copy Assignment : Options& operator=(const Options&)");
		const Options<4> Opt = D_0;
		EXPECT_EQ(Opt.size(), 5);
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Move Constructor : Options(Options&&)");
		const Options<4> Opt{ D_0 };
		EXPECT_EQ(Opt.size(), 5);
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Move Assignment : Options& operator=(Options&&)");
		const Options<4> Opt = Options<4>{};
		EXPECT_EQ(Opt.size(), 5);
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Options(const bitset&)");	// 0th bit is last in input
		const std::bitset<5> bit{ "10101" };
		const Options<4> Opt{ bit };
		EXPECT_EQ(Opt.size(), 5);
		EXPECT_EQ(Opt.DebugString(), "10101");
	}
	{
		SCOPED_TRACE("Options(bitset&&)");
		const Options<4> A_0{ std::bitset<5>{"01000"} };
		EXPECT_EQ(A_0.size(), 5);
		EXPECT_EQ(A_0.DebugString(), "01000");
	}
	{
		SCOPED_TRACE("Options(int value)");
		const Options<4> A_1{ 2 };		// set() // clear() // add()
		const Options<4> A_2 = 2;
		EXPECT_EQ(A_1.DebugString(), "00100");
		EXPECT_EQ(A_2.DebugString(), "00100");
		EXPECT_EQ(Options<4>{0}.DebugString(), "00001");	// [count-0]
		EXPECT_EQ(Options<4>{4}.DebugString(), "10000");
		EXPECT_EQ(Options<4>{4}.DebugString(), "10000");
	// assertion see deathtests
#ifndef _DEBUG	// debug contains assert( <= )
		EXPECT_THROW(Options<4>{5}, std::out_of_range);
#endif // _DEBUG
		EXPECT_THROW(Options<4>{-5}, std::out_of_range);
	}
	{
		SCOPED_TRACE("Options& operator=(int value)");
		Options<4> TMP{};
		TMP = 1;						// set() // clear() // add()
		EXPECT_EQ(TMP.DebugString(), "00010");
		EXPECT_EQ((TMP = 3).DebugString(), "01000");
		EXPECT_EQ((TMP = 0).DebugString(), "00001");		// [count-0]
		EXPECT_THROW(TMP = 6, std::out_of_range);
		EXPECT_THROW(TMP = -6, std::out_of_range);
	}
	{
		SCOPED_TRACE("Options& operator=(const bitset&)");
		Options<4> B_0{};
		std::bitset<5> bit{ "11011" };
		B_0 = bit;
		EXPECT_EQ(B_0.DebugString(), "11011");
	}
	{
		SCOPED_TRACE("Options& operator=(bitset&&)");
		Options<4> B_1{};
		B_1 = std::bitset<5>{ "11011" };
		EXPECT_EQ(B_1.DebugString(), "11011");
	}
}

struct TestElements
{
	const Options<9> D_0{};								// default
	const Options<4> D_1{};								// default
	const Options<4> A_1{ std::bitset<5>{"00010"} };	// answer = 1
	const Options<4> A_2{ std::bitset<5>{"00100"} };	// answer = 2
	const Options<4> O_1{ std::bitset<5>{"00101"} };	// answer = 2, unanswered
	const Options<4> O_2{ std::bitset<5>{"01011"} };	// 2 options 1 & 3
	const Options<4> O_3{ std::bitset<5>{"11101"} };	// 3 options 2,3,4
	const Options<4> O_4{ std::bitset<5>{"11111"} };	// all options
	const Options<4> E_1{ std::bitset<5>{"00000"} };	// empty
	const Options<4> E_2{ std::bitset<5>{"00001"} };	// empty, no answer
	const Options<4> X_0{ std::bitset<5>{"11110"} };	// answered, alle options
	const Options<4> X_1{ std::bitset<5>{"10100"} };	// answered, 2 options
};
TestElements TE;

TEST(Options, mf_counting)
{
	static_assert(noexcept(TE.D_0.size()), "size() should be noexcept");
	EXPECT_EQ(TE.D_0.size(), 10);
	EXPECT_EQ(TE.D_1.size(), 5);
	EXPECT_EQ(TE.O_1.size(), 5);
	EXPECT_EQ(TE.E_1.size(), 5);
	static_assert(noexcept(TE.O_1.count()), "count() should be noexcept");
	static_assert(noexcept(TE.O_1.count_all()), "count_all() should be noexcept");
	EXPECT_EQ(TE.D_0.count(),		9);
	EXPECT_EQ(TE.D_0.count_all(),	9);
	EXPECT_EQ(TE.A_2.count(),		0);	// different
	EXPECT_EQ(TE.A_2.count_all(),	1);	//
	EXPECT_EQ(TE.O_1.count(),		1);
	EXPECT_EQ(TE.O_1.count_all(),	1);
	EXPECT_EQ(TE.O_2.count(),		2);
	EXPECT_EQ(TE.O_2.count_all(),	2);
	EXPECT_EQ(TE.O_3.count(),		3);
	EXPECT_EQ(TE.O_3.count_all(),	3);
	EXPECT_EQ(TE.E_1.count(),		0);
	EXPECT_EQ(TE.E_1.count_all(),	0);
	EXPECT_EQ(TE.E_2.count(),		0);
	EXPECT_EQ(TE.E_2.count_all(),	0);
	static_assert(noexcept(TE.O_1.all()), "all() should be noexcept");
	EXPECT_TRUE(TE.D_1.all());
	EXPECT_FALSE(TE.O_3.all());
	EXPECT_TRUE(TE.O_4.all());
	EXPECT_FALSE(TE.E_1.all());
	EXPECT_FALSE(TE.X_0.all());
	static_assert(noexcept(TE.O_1.is_empty()), "is_empty() should be noexcept");
	EXPECT_TRUE(TE.E_1.is_empty());
	EXPECT_TRUE(TE.E_2.is_empty());
	EXPECT_FALSE(TE.D_1.is_empty());
	EXPECT_FALSE(TE.O_1.is_empty());
	EXPECT_FALSE(TE.X_1.is_empty());
}
TEST(Options, mf_boolRequest)
{
	static_assert(!noexcept(TE.O_1.test(2)), "test() is NOT noexcept");
	EXPECT_THROW(TE.O_1.test(15), std::out_of_range);
	EXPECT_THROW(TE.O_1.test(-1), std::out_of_range);
	EXPECT_TRUE(TE.O_1.test(2));
	EXPECT_TRUE(TE.O_3.test(2));
	EXPECT_FALSE(TE.O_3.test(1));
	EXPECT_FALSE(TE.E_1.test(1));
	EXPECT_TRUE(TE.X_0.test(1));
	static_assert(noexcept(TE.O_1.is_answer()), "is_answer() should be noexcept");
	EXPECT_TRUE(TE.A_1.is_answer());
	EXPECT_TRUE(TE.A_2.is_answer());
	EXPECT_FALSE(TE.O_1.is_answer());
	EXPECT_FALSE(TE.O_4.is_answer());
	EXPECT_FALSE(TE.E_1.is_answer());
	EXPECT_FALSE(TE.E_2.is_answer());
	EXPECT_FALSE(TE.X_0.is_answer());
	EXPECT_FALSE(TE.X_1.is_answer());

	static_assert(noexcept(TE.O_1.is_answer(1)), "is_answer(int) is noexcept");
	static_assert(noexcept(TE.A_1.is_answer(100)), "is_answer(int) is noexcept");
	// assertion see deathtests
#ifndef _DEBUG
	EXPECT_NO_THROW(TE.A_1.is_answer(15));	// undefined behaviour
	EXPECT_NO_THROW(TE.A_1.is_answer(-5));
#endif // _DEBUG
	EXPECT_NO_THROW(TE.O_1.is_answer(15));	// defined behaviour (== false)
	EXPECT_FALSE(TE.O_1.is_answer(15));	// defined behaviour
	EXPECT_NO_THROW(TE.O_1.is_answer(-1));	// defined behaviour (== false)
	EXPECT_FALSE(TE.O_1.is_answer(-1));	// defined behaviour
	EXPECT_TRUE(TE.A_2.is_answer(2));
	EXPECT_FALSE(TE.A_2.is_answer(3));
	EXPECT_FALSE(TE.A_1.is_answer(0));
	EXPECT_FALSE(TE.O_1.is_answer(2));
	EXPECT_FALSE(TE.O_1.is_answer(2));
	EXPECT_FALSE(TE.E_1.is_answer(0));

	static_assert(noexcept(TE.O_1.is_option(2)), "is_option(int) is noexept");
	// assertion see deathtests
#ifndef _DEBUG
	EXPECT_NO_THROW(TE.O_1.is_option(15));	// undefined behaviour
	EXPECT_NO_THROW(TE.O_1.is_option(-5));
#endif // _DEBUG
	EXPECT_NO_THROW(TE.A_1.is_option(15));	// defined (== false)
	EXPECT_NO_THROW(TE.A_1.is_option(-5));	// defined (== false)
	EXPECT_TRUE(TE.D_1.is_option(4));
	EXPECT_TRUE(TE.O_1.is_option(2));
	EXPECT_FALSE(TE.A_2.is_option(2));
	EXPECT_FALSE(TE.O_3.is_option(1));
	EXPECT_TRUE(TE.O_3.is_option(2));
	EXPECT_TRUE(TE.X_0.is_option(2));	// proper result even with incorrect answer-flag
}
TEST(Options, mf_dataRequest)
{
	std::vector<int> result{};
	//std::vector<int> available() const;	// return available options
	static_assert(!noexcept(TE.O_1.available()), "available() should NOT be noexcept");
	ASSERT_NO_THROW(result = TE.O_2.available());
	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], 1);
	EXPECT_EQ(result[1], 3);
	ASSERT_NO_THROW(result = TE.E_1.available());
	EXPECT_EQ(result.size(), 0);
	ASSERT_NO_THROW(result = TE.E_2.available());
	EXPECT_EQ(result.size(), 0);
	ASSERT_NO_THROW(result = TE.X_1.available());
	EXPECT_EQ(result.size(), 0);	//??? won't work for is_option() ...
	//int get_answer() const noexcept;		// return get_answer or 0
	static_assert(noexcept(TE.O_1.get_answer()), "get_answer() should be noexcept");
	EXPECT_EQ(TE.A_1.get_answer(), 1);
	EXPECT_EQ(TE.O_1.get_answer(), 2);
	EXPECT_EQ(TE.O_2.get_answer(), 0);
	EXPECT_EQ(TE.E_1.get_answer(), 0);
	EXPECT_EQ(TE.E_2.get_answer(), 0);
	EXPECT_EQ(TE.X_0.get_answer(), 0);
	EXPECT_EQ(TE.X_1.get_answer(), 0);
}
TEST(Options, mf_changeAll)
{
	// Setup
	Options<4> TMP{};
	ASSERT_EQ(TMP.size(), 5)	<< "Invalid object";
	ASSERT_TRUE(TMP.all())		<< "All options should be available";
	EXPECT_EQ(TMP.count(), 4);
	EXPECT_EQ(TMP.count_all(), 4);
	ASSERT_EQ(TMP, TE.D_1)		<< "Default set D_1 is not complete";

	static_assert(noexcept(TMP.clear()), "clear() should be noexcept");
	TMP = TE.D_1;	// reset testdata
	ASSERT_EQ(TMP, TE.D_1)		<< "Reset testdata failed";
	TMP.clear();
	EXPECT_TRUE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 0);
	EXPECT_EQ(TMP.count_all(), 0);
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.clear().is_empty());
	static_assert(noexcept(TMP.reset()), "reset() should be noexcept");
	ASSERT_TRUE(TE.E_1.is_empty())	<< "E_1 should be empty";
	TMP = TE.E_1;
	ASSERT_EQ(TMP, TE.E_1)		<< "Reset testdata failed";
	TMP.reset();
	EXPECT_FALSE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 4);
	EXPECT_EQ(TMP.count_all(), 4);
	EXPECT_TRUE(TMP.is_option(2));
	EXPECT_TRUE(TMP.all());
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.reset().all());
	static_assert(noexcept(TMP.flip()), "flip() should be noexcept");
	TMP = TE.D_1;
	ASSERT_TRUE(TMP.all())		<< "Reset testdata failed";
	TMP.flip();
	EXPECT_TRUE(TMP.is_empty());
	EXPECT_EQ(TMP.count_all(), 0);
	TMP.flip();
	EXPECT_TRUE(TMP.all());
	EXPECT_EQ(TMP.count_all(), 4);
}
TEST(Options, mf_remove_option)
{
	Options<4> TMP{};
	static_assert(
		noexcept(TMP.remove_option(3)),
		"remove_option(int) should be noexcept");
	ASSERT_NO_THROW(TMP.remove_option(3));
	//EXPECT_THROW(TMP.remove_option(15), std::out_of_range);
	//EXPECT_THROW(TMP.remove_option(-5), std::out_of_range);
	ASSERT_TRUE(TMP.reset().all())	<< "Reset testdata failed";
	ASSERT_EQ(TMP.size(), 5)		<< "Test requires Options<4> object";
	EXPECT_TRUE(TMP.test(3));
	EXPECT_NO_THROW(TMP.remove_option(3));
	EXPECT_FALSE(TMP.test(3));
	EXPECT_EQ(TMP.count(), 3);
}
TEST(Options, mf_add)
{
	// add(int)
	//Options& add(int value);			// add single option
	Options<4> Opt{ std::bitset<5>{"00000"} };
	static_assert(!noexcept(Opt.add(4)), "add(int) should NOT be noexcept");
	ASSERT_NO_THROW(Opt.add(4)) << "add(int)";
	EXPECT_EQ(Opt.DebugString(), "10000");
	ASSERT_THROW(Opt.add(5), std::out_of_range);
	ASSERT_THROW(Opt.add(-5), std::out_of_range);
	ASSERT_NO_THROW(Opt.add(0));
	EXPECT_EQ(Opt.DebugString(), "10001");	//??? better way?

	ASSERT_EQ(Opt.clear().DebugString(), "00000") << "clear() required";
	constexpr unsigned int u_i{ 2 };
	EXPECT_NO_THROW(Opt.add(u_i)) << "add(unsinged int) failed";
	EXPECT_EQ(Opt.DebugString(), "00100");
	EXPECT_THROW(Opt.add(12), std::out_of_range);
	EXPECT_THROW(Opt.add(-2), std::out_of_range);

	//add_noexcept(int)
	Options<4> TMP{ std::bitset<5>{"00000"} };
	static_assert(noexcept(TMP.add_nocheck(1)), "add_noexcept(int) should be noexcept");
	// assertion see deathtests
#ifndef _DEBUG
	//EXPECT_NO_THROW(TMP.add_nocheck(6));
	//EXPECT_NO_THROW(TMP.add_nocheck(-1));
#endif // !_DEBUG
	EXPECT_NO_THROW(TMP.add_nocheck(3));
	EXPECT_EQ(TMP.DebugString(), "01000");
	EXPECT_FALSE(TMP.count_all() == 0);
	EXPECT_TRUE(TMP.test(3));
	EXPECT_TRUE(TMP.add_nocheck(4).test(4));
}
TEST(Options, mf_set)
{
	// set(int)
	Options<4> TMP{};
	static_assert(!noexcept(TMP.set(4)), "set(int) should NOT be noexcept");
	EXPECT_NO_THROW(TMP.set(4));
	EXPECT_THROW(TMP.set(5), std::out_of_range);
	EXPECT_THROW(TMP.set(-5), std::out_of_range);
	EXPECT_NO_THROW(TMP.set(0));
	EXPECT_TRUE(TMP.set(4).is_answer());
	EXPECT_TRUE(TMP.is_answer(4));
	EXPECT_EQ(TMP.count(), 0);
	EXPECT_TRUE(TMP.set(1).is_answer(1));
	EXPECT_FALSE(TMP.is_answer(4));
	EXPECT_TRUE(TMP.set(0).is_empty());
	EXPECT_EQ(TMP.count_all(), 0);
	EXPECT_EQ(TMP.DebugString(), "00001");

	// set_noexcept(int)
	static_assert(
		noexcept(TMP.set_nocheck(2)), "set_noexcept(int) should be noexcept");
	EXPECT_TRUE(TMP.clear().is_empty());
	EXPECT_NO_THROW(TMP.set_nocheck(1));
	EXPECT_EQ(TMP.DebugString(), "00010");
	EXPECT_TRUE(TMP.set_nocheck(2).is_answer(2));
	// assertion see deathtests
#ifndef _DEBUG
	// EXPECT_NO_THROW(TMP.set_nocheck(15));
	// EXPECT_NO_THROW(TMP.set_nocheck(-5));
#endif // _DEBUG
	EXPECT_NO_THROW(TMP.set_nocheck(0));
}
TEST(Options, mf_booleanComparison)
{
	// operator==(int) const
	static_assert(noexcept(TE.A_1 == 1), "operator==(int) should be noexcept");
	EXPECT_EQ(TE.A_1, 1);
	EXPECT_EQ(TE.A_2, 2);

	//bool operator==(Options<E>&) const
	static_assert(noexcept(TE.O_1 == TE.O_2), "operator== should be noexcept");
	Options<4> TMP{ std::bitset<5>{"00101"} };
	EXPECT_EQ(TE.O_1, TMP);
	EXPECT_FALSE(TE.O_1 == TE.E_1);

	//bool operator!=(Options<E>&) const
	static_assert(noexcept(TE.O_1 != TE.O_2), "operator!= should be noexcept");
	EXPECT_NE(TE.O_1, TE.E_1);
	EXPECT_FALSE(TE.O_1 != TE.O_1);

	static_assert(noexcept(TE.O_1 < TE.O_4), "operator< should be noexcept");
	EXPECT_FALSE(TE.D_1 < TE.O_4) << "both full";
	EXPECT_FALSE(TE.E_1 < std::bitset<5>{"00000"}) << "both empty";
	EXPECT_LT(TE.E_1, TE.D_1) << "empty vs default";
	EXPECT_FALSE(TE.D_1 < TE.E_1) << "empty vs default";
	EXPECT_LT(TE.E_1, TE.A_1) << "empty vs answer";
	EXPECT_FALSE(TE.A_1 < TE.E_1) << "empty vs answer";
	EXPECT_LT(TE.E_1, TE.O_1) << "empty vs options";
	EXPECT_FALSE(TE.O_1 < TE.E_1) << "empty vs options";
	EXPECT_LT(TE.E_1, TE.E_2) << "empty vs unanswered empty";
	EXPECT_FALSE(TE.E_2 < TE.E_1) << "empty vs unanswered empty";
	EXPECT_LT(TE.E_1, TE.O_1) << "empty vs answered options";
	EXPECT_FALSE(TE.O_1 < TE.E_1) << "empty vs answered options";
	EXPECT_LT(TE.E_1, TE.X_0) << "empty vs answered all options";
	EXPECT_FALSE(TE.X_0 < TE.E_1) << "empty vs answered all options";
	EXPECT_LT(TE.E_1, TE.X_1) << "empty vs answered 2 options";
	EXPECT_FALSE(TE.X_1 < TE.E_1) << "empty vs answered 2 options";
	EXPECT_LT(TE.A_2, TE.D_1) << "answer vs default";
	EXPECT_FALSE(TE.D_1 < TE.A_2) << "answer vs default";
	EXPECT_LT(TE.A_1, TE.A_2) << "answer vs answer";
	EXPECT_FALSE(TE.A_2 < TE.A_1) << "answer vs answer";
	EXPECT_LT(TE.A_2, TE.X_0) << "answer vs answered all options";
	EXPECT_FALSE(TE.X_0 < TE.A_2) << "answer vs answered all options";
	EXPECT_LT(TE.A_2, TE.X_1) << "answer vs answered 2 options";
	EXPECT_FALSE(TE.A_2 < TE.A_1) << "both answered";
	EXPECT_LT(TE.O_2, std::bitset<5>{"10011"}) << "compare values";
	EXPECT_LT(Options<6>{std::bitset<7>{"0111111"}}, std::bitset<7>{"1011111"});
	EXPECT_FALSE(Options<6>{std::bitset<7>{"1011111"}} < std::bitset<7>{"0111111"});
	EXPECT_LT(TE.A_2, TE.O_2) << "answer vs unanswered";
	EXPECT_FALSE(TE.O_2 < TE.A_2) << "answer vs unanswered";
	EXPECT_LT(TE.A_2, TE.O_2) << "answer vs unanswered";
	EXPECT_FALSE(TE.O_2 < TE.A_2) << "answer vs unanswered";
	EXPECT_LT(TE.O_3, TE.X_0);
	EXPECT_FALSE(TE.X_0 < TE.O_3);
}
TEST(Options, mf_constOperators)
{
	//Options operator&(Options&) const						shared options
	static_assert(noexcept(TE.O_1 & TE.O_2), "operator& should be noexcept");
	EXPECT_EQ((TE.D_1 & TE.O_3), TE.O_3);
	EXPECT_EQ((TE.O_1 & TE.A_2), TE.A_2);
	EXPECT_EQ((TE.O_2 & TE.E_2), TE.E_2);
	EXPECT_EQ((TE.O_3 & TE.A_2), TE.A_2);
	EXPECT_EQ((TE.E_2 & TE.O_1), TE.E_2);

	//constexpr bool operator[](int) const
	static_assert(noexcept(TE.O_1[2]), "operator[int] should be no except for reading");
	EXPECT_NO_THROW(TE.O_3[2]);
	EXPECT_NO_THROW(TE.O_3[0]);
	// assertion see deathtests
#ifndef _DEBUG
	EXPECT_NO_THROW(TE.O_3[9]);
	EXPECT_NO_THROW(TE.O_3[-1]);
#endif	// _DEBUG
	EXPECT_TRUE(TE.O_1[2]);
	EXPECT_TRUE(TE.A_2[2]);
	EXPECT_TRUE(TE.A_2[1] == false);
}
TEST(Options, Operators)
{
	//TODO rewrite testformat
	// _test-data
	const Options<4> O_1{};								// all options
	const Options<4> O_2{ std::bitset<5>{"11111"} };	// all options
	const Options<4> O_3{ std::bitset<5>{"00101"} };	// single option 2
	const Options<4> E_1{ 0 };							// empty answer "00001"
	const Options<4> E_2{ std::bitset<5>{"00000"} };	// empty
	const Options<4> E_3{ std::bitset<5>{"00001"} };	// empty option
	const Options<4> A_1{ 1 };							// answer 1
	const Options<4> A_2{ std::bitset<5>{"00100"} };	// answer 2
	Options<4> TMP{};									// per test, reset this option

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
	EXPECT_TRUE(E_1 + A_2 == O_3);
	//Options operator-(Options&) const		difference
	static_assert(noexcept(O_1 - O_2), "operator- should be noexcept");


	///// testing the constructors /////
	EXPECT_TRUE(E_1 == E_3);
	// copy-assign
	static_assert(noexcept(TMP.operator=(O_2)), "operator= should be noexcept");
	static_assert(!noexcept(TMP.operator=(1)), "operator=(int) IS NOT noexcept");
	Options<4> TMP1 = A_2;
	EXPECT_TRUE(TMP1.is_answer(2));
	EXPECT_TRUE(TMP1 == A_2);
	Options<4> TMP2 = 3;
	EXPECT_TRUE(TMP2.is_answer(3));
	// move-assign
	static_assert(noexcept(TMP.operator=(std::bitset<5>())), "operator= should be noexcept_2");
	Options<4> O_6 = std::bitset<5>{};	// "00000"
	EXPECT_TRUE(O_6 == E_2);
}
TEST(Options, External)
{
	const Options<4> O_3{ std::bitset<5>{"00101"} };	// single option 2
	const Options<4> E_1{ 0 };							// empty answer "00001"
	const Options<4> E_2{ std::bitset<5>{"00000"} };	// empty
	const Options<4> E_3{ std::bitset<5>{"00001"} };	// empty option
	const Options<4> A_1{ 1 };							// answer 1
	const Options<4> A_2{ std::bitset<5>{"00100"} };	// answer 2
	//XOR(a,b)
	static_assert(noexcept(XOR(O_3, O_3)), "XOR() should be noexcept");
	EXPECT_EQ(XOR(E_3, A_2), O_3);
	EXPECT_EQ(XOR(XOR(E_3, A_2), A_2), E_3);
	//shared(a,b)
	static_assert(noexcept(shared(TE.D_1, TE.O_4)), "share(a,b) should be noexcept");
	EXPECT_EQ(shared(TE.O_4, O_3), O_3);
	EXPECT_EQ(shared(TE.D_1, TE.O_4), TE.D_1);
	EXPECT_EQ(shared(TE.O_4, E_2), E_2);
	EXPECT_EQ(shared(O_3, A_2), A_2);
	EXPECT_EQ(shared(E_3, A_2), E_2);
}
TEST(Options, deathtests)
{
	Options<4> TMP{};
	// Construction
#ifdef _DEBUG
	// Important	exeptrion thrown outside debug-mode
	// assert serves to cach E+1 case
	EXPECT_DEBUG_DEATH({ Options<3>{4}; }, "Assertion failed: .*");
#endif // _DEBUG

	// mf_boolRequest
	EXPECT_DEBUG_DEATH({ TE.A_1.is_answer(15); }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ TE.A_1.is_answer(-5); }, "Assertion failed: .*");
	// 
	EXPECT_DEBUG_DEATH({ TE.O_1.is_option(15); }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ TE.O_1.is_option(-5); }, "Assertion failed: .*");
	// mf_constOperators
	EXPECT_DEBUG_DEATH({ TE.O_3[9]; }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ TE.O_3[-1]; }, "Assertion failed: .*");
	bool a;
	EXPECT_DEBUG_DEATH({ a = TE.O_3[5]; }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ a = TE.O_3[-1]; }, "Assertion failed: .*");
	// operator[]
#ifdef _DEBUG
	//! supposed to be noexcept, and no bounds-checks in release-mode
	Options<4> Opp{std::bitset<5>{"00000"}};
	// EXPECT_DEBUG_DEATH({ Opp[3] == Opp[-2]; }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ Opp[5] = true; }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ Opp[-1] = true; }, "Assertion failed: .*");
	// mf_add
	Options<4> Opt{std::bitset<5>{"00000"}};
	EXPECT_DEBUG_DEATH({ Opt.add_nocheck(5); }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ Opt.add_nocheck(-5); }, "Assertion failed: .*");
	// mf_set
	EXPECT_TRUE(TMP.clear().is_empty());
	EXPECT_DEBUG_DEATH({ TMP.set_nocheck(15); }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH({ TMP.set_nocheck(-5); }, "Assertion failed: .*");
#endif // _DEBUG
}
} // namespace Sudoku_Test
