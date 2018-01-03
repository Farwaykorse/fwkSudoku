//===--- SudokuTests/Options.cpp                                        ---===//
//
// Unit tests for the template class Sudoku::Options
//===----------------------------------------------------------------------===//
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
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Options.h>
// Helpers
#include <Sudoku/Value.h>
// Debug Output
#include "print_Options.h"

// additional
#include <bitset>
#include <string>
#include <vector>
#include <type_traits>

using namespace Sudoku;

namespace SudokuTests::OptionsTest
{
namespace compiletime
{
	// Class properties
	using typeT = Sudoku::Options<9>;
	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_empty_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>, "standard layout");
	static_assert(
		not std::is_trivial_v<typeT>, "not user-provided, no virtuals");
	static_assert(not std::is_pod_v<typeT>);
	// static_assert(std::has_unique_object_representations_v<typeT>); //C++17

	static_assert(std::is_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);

	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(std::is_trivially_destructible_v<typeT>);

	static_assert(std::is_copy_constructible_v<typeT>);
	static_assert(std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(std::is_trivially_copy_constructible_v<typeT>);

	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(std::is_trivially_move_constructible_v<typeT>);

	static_assert(std::is_copy_assignable_v<typeT>);
	static_assert(std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(std::is_trivially_copy_assignable_v<typeT>);

	static_assert(std::is_move_assignable_v<typeT>);
	static_assert(std::is_nothrow_move_assignable_v<typeT>);
	static_assert(std::is_trivially_move_assignable_v<typeT>);

	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_swappable_v<typeT>);
	static_assert(std::is_nothrow_swappable_v<typeT>);

	// type construction
	// from std::bitset
	static_assert(std::is_constructible_v<Options<3>, const std::bitset<4>&>);
	static_assert(std::is_assignable_v<Options<3>, std::bitset<4>>);
	static_assert(noexcept(Options<3>{std::bitset<4>{}}));
	static_assert(noexcept(Options<3>() = std::bitset<4>{}));
	// explicit:
	static_assert(std::is_constructible_v<std::bitset<4>, std::string>);
	static_assert(not std::is_constructible_v<typeT, std::string>, "explicit");
	// size:
	static_assert(not std::is_constructible_v<typeT, const std::bitset<9>&>);
	static_assert(not std::is_assignable_v<Options<3>, std::bitset<3>>);

	static_assert(not std::is_swappable_with_v<Options<4>, std::bitset<5>>);
	static_assert(not std::is_nothrow_swappable_with_v<typeT, std::bitset<10>>);

	// Value
	static_assert(std::is_constructible_v<Options<3>, Value>);
	static_assert(std::is_assignable_v<Options<3>, Value>);
	static_assert(noexcept(Options<3>{Value{}}));
	static_assert(noexcept(Options<3>() = Value{}));

	static_assert(not std::is_constructible_v<Options<3>, int>);
	static_assert(not std::is_assignable_v<Options<3>, int>);
	static_assert(not std::is_constructible_v<Options<3>, unsigned int>);
	static_assert(not std::is_assignable_v<Options<3>, unsigned int>);
	static_assert(not std::is_constructible_v<Options<3>, size_t>);
	static_assert(not std::is_assignable_v<Options<3>, size_t>);

} // namespace compiletime
TEST(Options, Construction)
{
	const Options<4> D_0{};
	{
		SCOPED_TRACE("Default Constructor : Options()");
		static_assert(noexcept(Options<4>()), "Options() should be noexcept");
		static_assert(noexcept(Options<4>{}), "Options{} should be noexcept");
		const Options<4> D;
		EXPECT_EQ(D_0.size(), 5);
		EXPECT_EQ(D_0, D_0) << "operator== failed, required";
		EXPECT_EQ(D_0, D);
		EXPECT_EQ(D_0.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Copy Constructor : Options(const Options&)");
		static_assert(noexcept(Options<4>(D_0)));
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
		const Options<4> Opt{D_0};
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
		SCOPED_TRACE("Options(const bitset&)"); // 0th bit is last in input
		const std::bitset<5> bit{"10101"};
		const Options<4> Opt{bit};
		EXPECT_EQ(Opt.size(), 5);
		EXPECT_EQ(Opt.DebugString(), "10101");
	}
	{
		SCOPED_TRACE("Options(bitset&&)");
		const Options<4> A_0{std::bitset<5>{"01000"}};
		EXPECT_EQ(A_0.size(), 5);
		EXPECT_EQ(A_0.DebugString(), "01000");
	}
	{
		SCOPED_TRACE("Options(Value)");
		const Options<4> A_1{Value{2}}; // set() // clear() // add()
		Options<4> A_2 = Value{2};
		EXPECT_EQ(A_1.DebugString(), "00100");
		EXPECT_EQ(A_2.DebugString(), "00100");
		EXPECT_EQ(Options<4>{Value{0}}.DebugString(), "00001"); // [count-0]
		EXPECT_EQ(Options<4>{Value{4}}.DebugString(), "10000");
		EXPECT_EQ(Options<4>{Value{4}}.DebugString(), "10000");
		EXPECT_EQ(Options<9>{Value{9}}.DebugString(), "1000000000");
		EXPECT_EQ(Options<9>{Value{2}}.DebugString(), "0000000100");
		// assertion see deathtests
	}
	{
		SCOPED_TRACE("Options& operator=(int value)");
		Options<4> TMP{};
		TMP = Value{1}; // set() // clear() // add()
		EXPECT_EQ(TMP.DebugString(), "00010");
		EXPECT_EQ((TMP = Value{3}).DebugString(), "01000");
		EXPECT_EQ((TMP = Value{0}).DebugString(), "00001"); // [count-0]
	}
	{
		SCOPED_TRACE("Options& operator=(const bitset&)");
		Options<4> B_0{};
		std::bitset<5> bit{"11011"};
		B_0 = bit;
		EXPECT_EQ(B_0.DebugString(), "11011");
	}
	{
		SCOPED_TRACE("Options& operator=(bitset&&)");
		Options<4> B_1{};
		B_1 = std::bitset<5>{"11011"};
		EXPECT_EQ(B_1.DebugString(), "11011");
	}
}

struct TestElements
{
	const Options<9> D_0{};                        // default
	const Options<4> D_1{};                        // default
	const Options<4> A_1{std::bitset<5>{"00010"}}; // answer = 1
	const Options<4> A_2{std::bitset<5>{"00100"}}; // answer = 2
	const Options<4> O_1{std::bitset<5>{"00101"}}; // answer = 2, unanswered
	const Options<4> O_2{std::bitset<5>{"01011"}}; // 2 options 1 & 3
	const Options<4> O_3{std::bitset<5>{"11101"}}; // 3 options 2,3,4
	const Options<4> O_4{std::bitset<5>{"11111"}}; // all options
	const Options<4> E_1{std::bitset<5>{"00000"}}; // empty
	const Options<4> E_2{std::bitset<5>{"00001"}}; // empty, no answer
	const Options<4> X_0{std::bitset<5>{"11110"}}; // answered, alle options
	const Options<4> X_1{std::bitset<5>{"10100"}}; // answered, 2 options
};
TestElements TE;

TEST(Options, mf_counting)
{
	static_assert(noexcept(TE.D_0.size()));
	EXPECT_EQ(TE.D_0.size(), 10);
	EXPECT_EQ(TE.D_1.size(), 5);
	EXPECT_EQ(TE.O_1.size(), 5);
	EXPECT_EQ(TE.E_1.size(), 5);
	static_assert(noexcept(TE.O_1.count()));
	static_assert(noexcept(TE.O_1.count_all()));
	// clang-format off
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
	// clang-format on
	static_assert(noexcept(TE.O_1.all()));
	EXPECT_TRUE(TE.D_1.all());
	EXPECT_FALSE(TE.O_3.all());
	EXPECT_TRUE(TE.O_4.all());
	EXPECT_FALSE(TE.E_1.all());
	EXPECT_FALSE(TE.X_0.all());
	static_assert(noexcept(TE.O_1.is_empty()));
	EXPECT_TRUE(TE.E_1.is_empty());
	EXPECT_TRUE(TE.E_2.is_empty());
	EXPECT_FALSE(TE.D_1.is_empty());
	EXPECT_FALSE(TE.O_1.is_empty());
	EXPECT_FALSE(TE.X_1.is_empty());
}
TEST(Options, test_Value)
{
	static_assert(not noexcept(TE.O_1.test(Value{2})));
	EXPECT_THROW(TE.O_1.test(Value{15}), std::out_of_range);
	EXPECT_TRUE(TE.O_1.test(Value{2}));
	EXPECT_TRUE(TE.O_3.test(Value{2}));
	EXPECT_FALSE(TE.O_3.test(Value{1}));
	EXPECT_FALSE(TE.E_1.test(Value{1}));
	EXPECT_TRUE(TE.X_0.test(Value{1}));
}

TEST(Options, is_answer)
{
	static_assert(noexcept(TE.O_1.is_answer()));
	EXPECT_TRUE(TE.A_1.is_answer());
	EXPECT_TRUE(TE.A_2.is_answer());
	EXPECT_FALSE(TE.O_1.is_answer());
	EXPECT_FALSE(TE.O_4.is_answer());
	EXPECT_FALSE(TE.E_1.is_answer());
	EXPECT_FALSE(TE.E_2.is_answer());
	EXPECT_FALSE(TE.X_0.is_answer());
	EXPECT_FALSE(TE.X_1.is_answer());
	// convenience function: exactly the same
	static_assert(noexcept(is_answer(TE.O_1)));
	EXPECT_TRUE(is_answer(TE.A_1));
	EXPECT_TRUE(is_answer(TE.A_2));
	EXPECT_FALSE(is_answer(TE.O_1));
	EXPECT_FALSE(is_answer(TE.O_4));
	EXPECT_FALSE(is_answer(TE.E_1));
	EXPECT_FALSE(is_answer(TE.E_2));
	EXPECT_FALSE(is_answer(TE.X_0));
	EXPECT_FALSE(is_answer(TE.X_1));

	static_assert(not noexcept(is_answer(TE.O_1, Value{1})));
	static_assert(not noexcept(is_answer(TE.A_1, Value{100})));
	// assertion see deathtests
	EXPECT_TRUE(is_answer(TE.A_2, Value{2}));
	EXPECT_FALSE(is_answer(TE.A_2, Value{3}));
	EXPECT_FALSE(is_answer(TE.A_1, Value{0}));
	EXPECT_FALSE(is_answer(TE.O_1, Value{2}));
	EXPECT_FALSE(is_answer(TE.O_1, Value{2}));
	EXPECT_FALSE(is_answer(TE.E_1, Value{0}));
}

TEST(Options, is_option)
{
	static_assert(not noexcept(is_option(TE.O_1, Value{2})));
	// assertion see deathtests
#ifndef _DEBUG
	EXPECT_THROW(is_option(TE.O_1, Value{15}), std::out_of_range);
	EXPECT_THROW(is_option(TE.A_1, Value{15}), std::out_of_range);
#endif // _DEBUG
	EXPECT_TRUE(is_option(TE.D_1, Value{4}));
	EXPECT_TRUE(is_option(TE.O_1, Value{2}));
	EXPECT_FALSE(is_option(TE.A_2, Value{2}));
	EXPECT_FALSE(is_option(TE.O_3, Value{1}));
	EXPECT_TRUE(is_option(TE.O_3, Value{2}));
	EXPECT_TRUE(is_option(TE.X_0, Value{2})); // proper result even with
											  // incorrect answer-flag
}

TEST(Options, read_next)
{
	// must work on const objects
	static_assert(std::is_const_v<decltype(TE.O_1)>);

	static_assert(noexcept(read_next(TE.O_1)));
	// on all available, walk over it
	// Value = 0
	EXPECT_EQ(read_next(TE.D_0, Value{0}), Value{1});
	// Value not set
	EXPECT_EQ(read_next(TE.D_0), Value{1});
	// Value = E-1
	EXPECT_EQ(read_next(TE.D_0, Value{8}), Value{9});
	// Value > E-1
	EXPECT_EQ(read_next(TE.D_0, Value{9}), Value{0});
	// on multiple available (1 & 3)
	EXPECT_EQ(read_next(TE.O_2, Value{0}), Value{1});
	EXPECT_EQ(read_next(TE.O_2), Value{1});
	EXPECT_EQ(read_next(TE.O_2, Value{1}), Value{3});
	EXPECT_EQ(read_next(TE.O_2, Value{2}), Value{3});
	EXPECT_EQ(read_next(TE.O_2, Value{3}), Value{0});
	EXPECT_EQ(read_next(TE.O_2, Value{4}), Value{0});
	// on multiple available (2,3,4)
	EXPECT_EQ(read_next(TE.O_3, Value{0}), Value{2});
	EXPECT_EQ(read_next(TE.O_3), Value{2});
	EXPECT_EQ(read_next(TE.O_3, Value{2}), Value{3});
	EXPECT_EQ(read_next(TE.O_3, Value{3}), Value{4});
	EXPECT_EQ(read_next(TE.O_3, Value{4}), Value{0});
	// on empty
	EXPECT_EQ(read_next(TE.E_1, Value{0}), Value{0});
	EXPECT_EQ(read_next(TE.E_1), Value{0});
	EXPECT_EQ(read_next(TE.E_1, Value{3}), Value{0});
	EXPECT_EQ(read_next(TE.E_2, Value{0}), Value{0});
	EXPECT_EQ(read_next(TE.E_2), Value{0});
	// on answer (set)
	EXPECT_EQ(read_next(TE.A_2, Value{0}), Value{2});
	EXPECT_EQ(read_next(TE.A_2), Value{2});
	EXPECT_EQ(read_next(TE.A_2, Value{1}), Value{2});
	EXPECT_EQ(read_next(TE.A_2, Value{2}), Value{0});
	// on answer (not set)
	EXPECT_EQ(read_next(TE.O_1, Value{0}), Value{2});
	EXPECT_EQ(read_next(TE.O_1), Value{2});
	EXPECT_EQ(read_next(TE.O_1, Value{3}), Value{0});
	EXPECT_EQ(read_next(TE.O_1, Value{4}), Value{0});
	// answer-bit fault
	EXPECT_EQ(read_next(TE.X_0, Value{0}), Value{1});
	EXPECT_EQ(read_next(TE.X_0), Value{1});
	EXPECT_EQ(read_next(TE.X_0, Value{1}), Value{2});
}

TEST(Options, available)
{
	std::vector<Value> result{};

	static_assert(not noexcept(available(TE.O_1)));
	ASSERT_NO_THROW(result = available(TE.O_2));
	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], Value{1});
	EXPECT_EQ(result[1], Value{3});
	ASSERT_NO_THROW(result = available(TE.E_1));
	EXPECT_EQ(result.size(), 0);
	ASSERT_NO_THROW(result = available(TE.E_2));
	EXPECT_EQ(result.size(), 0);
	ASSERT_NO_THROW(result = available(TE.X_1));
	EXPECT_EQ(result.size(), 0); //??? won't work for is_option() ...
}

TEST(Options, get_answer)
{
	static_assert(noexcept(get_answer(TE.O_1)));
	EXPECT_EQ(get_answer(TE.A_1), Value{1});
	EXPECT_EQ(get_answer(TE.O_1), Value{2});
	EXPECT_EQ(get_answer(TE.O_2), Value{0});
	EXPECT_EQ(get_answer(TE.E_1), Value{0});
	EXPECT_EQ(get_answer(TE.E_2), Value{0});
	EXPECT_EQ(get_answer(TE.X_0), Value{0});
	EXPECT_EQ(get_answer(TE.X_1), Value{0});
}

TEST(Options, mf_changeAll)
{
	// Setup
	Options<4> TMP{};
	ASSERT_EQ(TMP.size(), 5) << "Invalid object";
	ASSERT_TRUE(TMP.all()) << "All options should be available";
	EXPECT_EQ(TMP.count(), 4);
	EXPECT_EQ(TMP.count_all(), 4);
	ASSERT_EQ(TMP, TE.D_1) << "Default set D_1 is not complete";

	static_assert(noexcept(TMP.clear()));
	TMP = TE.D_1; // reset testdata
	ASSERT_EQ(TMP, TE.D_1) << "Reset testdata failed";
	TMP.clear();
	EXPECT_TRUE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 0);
	EXPECT_EQ(TMP.count_all(), 0);
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.clear().is_empty());
	static_assert(noexcept(TMP.reset()));
	ASSERT_TRUE(TE.E_1.is_empty()) << "E_1 should be empty";
	TMP = TE.E_1;
	ASSERT_EQ(TMP, TE.E_1) << "Reset testdata failed";
	TMP.reset();
	EXPECT_FALSE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 4);
	EXPECT_EQ(TMP.count_all(), 4);
	EXPECT_TRUE(is_option(TMP, Value{2}));
	EXPECT_TRUE(TMP.all());
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.reset().all());
	static_assert(noexcept(TMP.flip()));
	TMP = TE.D_1;
	ASSERT_TRUE(TMP.all()) << "Reset testdata failed";
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
	static_assert(not noexcept(TMP.remove_option(Value{3})));
	ASSERT_NO_THROW(TMP.remove_option(Value{3}));
	// EXPECT_THROW(TMP.remove_option(Value{15}), std::out_of_range);
	// EXPECT_THROW(TMP.remove_option(-5), std::out_of_range);
	ASSERT_TRUE(TMP.reset().all()) << "Reset testdata failed";
	ASSERT_EQ(TMP.size(), 5) << "Test requires Options<4> object";
	EXPECT_TRUE(TMP.test(Value{3}));
	EXPECT_NO_THROW(TMP.remove_option(Value{3}));
	EXPECT_FALSE(TMP.test(Value{3}));
	EXPECT_EQ(TMP.count(), 3);
}
TEST(Options, mf_add)
{
	// add(int)
	// Options& add(int value);			// add single option
	Options<4> Opt{std::bitset<5>{"00000"}};
	static_assert(not noexcept(Opt.add(Value{4})));
	ASSERT_NO_THROW(Opt.add(Value{4})) << "add(Value)";
	EXPECT_EQ(Opt.DebugString(), "10000");
	ASSERT_THROW(Opt.add(Value{5}), std::out_of_range);
	ASSERT_NO_THROW(Opt.add(Value{0}));
	EXPECT_EQ(Opt.DebugString(), "10001"); //??? better way?

	ASSERT_EQ(Opt.clear().DebugString(), "00000") << "clear() required";
	constexpr Value u_i{2};
	EXPECT_NO_THROW(Opt.add(u_i)) << "add(Value) failed";
	EXPECT_EQ(Opt.DebugString(), "00100");
	EXPECT_THROW(Opt.add(Value{12}), std::out_of_range);

	// add_noexcept(int)
	Options<4> TMP{std::bitset<5>{"00000"}};
	static_assert(noexcept(TMP.add_nocheck(Value{1})));
	// assertion see deathtests
#ifndef _DEBUG
	// EXPECT_NO_THROW(TMP.add_nocheck(Value{6}));
	// EXPECT_NO_THROW(TMP.add_nocheck(-1));
#endif // !_DEBUG
	EXPECT_NO_THROW(TMP.add_nocheck(Value{3}));
	EXPECT_EQ(TMP.DebugString(), "01000");
	EXPECT_FALSE(TMP.count_all() == 0);
	EXPECT_TRUE(TMP.test(Value{3}));
	EXPECT_TRUE(TMP.add_nocheck(Value{4}).test(Value{4}));
}
TEST(Options, mf_set)
{
	// set(int)
	Options<4> TMP{};
	static_assert(!noexcept(TMP.set(Value{4})));
	EXPECT_NO_THROW(TMP.set(Value{4}));
	EXPECT_THROW(TMP.set(Value{5}), std::out_of_range);
	EXPECT_NO_THROW(TMP.set(Value{0}));
	EXPECT_TRUE(is_answer(TMP.set(Value{4})));
	EXPECT_TRUE(is_answer(TMP, Value{4}));
	EXPECT_EQ(TMP.count(), 0);
	EXPECT_TRUE(is_answer(TMP.set(Value{1}), Value{1}));
	EXPECT_FALSE(is_answer(TMP, Value{4}));
	EXPECT_TRUE(TMP.set(Value{0}).is_empty());
	EXPECT_EQ(TMP.count_all(), 0);
	EXPECT_EQ(TMP.DebugString(), "00001");

	// set_noexcept(int)
	static_assert(noexcept(TMP.set_nocheck(Value{2})));
	EXPECT_TRUE(TMP.clear().is_empty());
	EXPECT_NO_THROW(TMP.set_nocheck(Value{1}));
	EXPECT_EQ(TMP.DebugString(), "00010");
	EXPECT_TRUE(is_answer(TMP.set_nocheck(Value{2}), Value{2}));
	// assertion see deathtests
#ifndef _DEBUG
	// EXPECT_NO_THROW(TMP.set_nocheck(Value{15}));
	// EXPECT_NO_THROW(TMP.set_nocheck(-5));
#endif // _DEBUG
	EXPECT_NO_THROW(TMP.set_nocheck(Value{0}));
}
TEST(Options, mf_booleanComparison)
{
	// operator==(int) const
	static_assert(noexcept(TE.A_1 == Value{1}));
	static_assert(noexcept(Value{1} == TE.A_1));
	EXPECT_EQ(TE.A_1, Value{1});
	EXPECT_EQ(Value{1}, TE.A_1);
	EXPECT_EQ(TE.A_2, Value{2});
	static_assert(noexcept(TE.A_1 != Value{1}));
	static_assert(noexcept(Value{1} != TE.A_1));
	EXPECT_NE(TE.A_1, Value{2});
	EXPECT_NE(Value{2}, TE.A_1);
	EXPECT_NE(TE.A_2, Value{1});

	// bool operator==(Options<E>&) const
	static_assert(noexcept(TE.O_1 == TE.O_2));
	const Options<4> TMP{std::bitset<5>{"00101"}};
	EXPECT_EQ(TE.O_1, TMP);
	EXPECT_FALSE(TE.O_1 == TE.E_1);

	// bool operator!=(Options<E>&) const
	static_assert(noexcept(TE.O_1 != TE.O_2));
	EXPECT_NE(TE.O_1, TE.E_1);
	EXPECT_FALSE(TE.O_1 != TE.O_1);

	static_assert(noexcept(TE.O_1 < TE.O_4));
	EXPECT_FALSE(TE.D_1 < TE.O_4) << "both full";
	EXPECT_FALSE(TE.E_1 < Options<4>(std::bitset<5>{"00000"})) << "both empty";
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
	EXPECT_LT(TE.O_2, Options<4>(std::bitset<5>{"10011"})) << "compare values";
	EXPECT_LT(
		Options<6>{std::bitset<7>{"0111111"}},
		Options<6>(std::bitset<7>{"1011111"}));
	EXPECT_FALSE(
		Options<6>{std::bitset<7>{"1011111"}} <
		Options<6>(std::bitset<7>{"0111111"}));
	EXPECT_LT(TE.A_2, TE.O_2) << "answer vs unanswered";
	EXPECT_FALSE(TE.O_2 < TE.A_2) << "answer vs unanswered";
	EXPECT_LT(TE.A_2, TE.O_2) << "answer vs unanswered";
	EXPECT_FALSE(TE.O_2 < TE.A_2) << "answer vs unanswered";
	EXPECT_LT(TE.O_3, TE.X_0);
	EXPECT_FALSE(TE.X_0 < TE.O_3);
}
TEST(Options, mf_constOperators)
{
	// Options operator&(Options&) const		shared options
	static_assert(noexcept(TE.O_1 & TE.O_2));
	EXPECT_EQ((TE.D_1 & TE.O_3), TE.O_3);
	EXPECT_EQ((TE.O_1 & TE.A_2), TE.A_2);
	EXPECT_EQ((TE.O_2 & TE.E_2), TE.E_2);
	EXPECT_EQ((TE.O_3 & TE.A_2), TE.A_2);
	EXPECT_EQ((TE.E_2 & TE.O_1), TE.E_2);

	// constexpr bool operator[](int) const
	static_assert(noexcept(TE.O_1[Value{2}]));
	EXPECT_NO_THROW(TE.O_3[Value{2}]);
	EXPECT_NO_THROW(TE.O_3[Value{0}]);
	// assertion see deathtests
#ifndef _DEBUG
	EXPECT_NO_THROW(TE.O_3[Value{9}]);
#endif // _DEBUG
	EXPECT_TRUE(TE.O_1[Value{2}]);
	EXPECT_TRUE(TE.A_2[Value{2}]);
	EXPECT_TRUE(TE.A_2[Value{1}] == false);
}
TEST(Options, Operators)
{
	// TODO rewrite testformat
	// _test-data
	const Options<4> O_1{};                        // all options
	const Options<4> O_2{std::bitset<5>{"11111"}}; // all options
	const Options<4> O_3{std::bitset<5>{"00101"}}; // single option 2
	const Options<4> E_1{Value{0}};                // empty answer "00001"
	const Options<4> E_2{std::bitset<5>{"00000"}}; // empty
	const Options<4> E_3{std::bitset<5>{"00001"}}; // empty option
	const Options<4> A_1{Value{1}};                // answer 1
	const Options<4> A_2{std::bitset<5>{"00100"}}; // answer 2
	Options<4> TMP{}; // per test, reset this option

	///// non-const operators /////
	// NEEDTEST constexpr bool operator[](int)
	static_assert(noexcept(TMP.operator[](Value{0}) = true));
	TMP.clear();
	EXPECT_TRUE(TMP[Value{0}] = true);
	EXPECT_TRUE(TMP[Value{0}] == true);
	EXPECT_FALSE(TMP[Value{0}].flip());
	EXPECT_TRUE(TMP[Value{0}] == false);
	// Options& XOR(Options&)			XOR
	static_assert(noexcept(TMP.XOR(O_3)));
	TMP = E_3;
	EXPECT_TRUE(TMP.XOR(A_2) == O_3);
	EXPECT_TRUE((TMP.XOR(A_2)) == E_3);
	// Options& operator+=(Options&)			combine options
	static_assert(noexcept(TMP += O_2));
	TMP = E_2;
	TMP += O_2;
	EXPECT_TRUE(O_1 == O_2);
	// others tested by using operator+()
	// Options operator+(Options&) const		combine options
	static_assert(noexcept(O_1 + O_2));
	EXPECT_TRUE(O_1 + O_2 == O_1);
	EXPECT_TRUE(E_1 + O_1 == O_1);
	EXPECT_TRUE(A_2 + E_3 == O_3);
	EXPECT_TRUE(E_1 + A_2 == O_3);

	///// testing the constructors /////
	EXPECT_TRUE(E_1 == E_3);
	// copy-assign
	static_assert(noexcept(TMP.operator=(O_2)));
	static_assert(noexcept(TMP.operator=(Value{1})));
	Options<4> TMP1 = A_2;
	EXPECT_TRUE(is_answer(TMP1, Value{2}));
	EXPECT_TRUE(TMP1 == A_2);
	const Options<4> TMP2 = Value{3};
	EXPECT_TRUE(is_answer(TMP2, Value{3}));
	// move-assign
	static_assert(noexcept(TMP.operator=(std::bitset<5>())));
	const Options<4> O_6 = Options<4>(std::bitset<5>{}); // "00000"
	EXPECT_TRUE(O_6 == E_2);
}
TEST(Options, External)
{
	const Options<4> O_3{std::bitset<5>{"00101"}}; // single option 2
	const Options<4> E_1{Value{0}};                // empty answer "00001"
	const Options<4> E_2{std::bitset<5>{"00000"}}; // empty
	const Options<4> E_3{std::bitset<5>{"00001"}}; // empty option
	const Options<4> A_1{Value{1}};                // answer 1
	const Options<4> A_2{std::bitset<5>{"00100"}}; // answer 2
	// XOR(a,b)
	static_assert(noexcept(XOR(O_3, O_3)));
	EXPECT_EQ(XOR(E_3, A_2), O_3);
	EXPECT_EQ(XOR(XOR(E_3, A_2), A_2), E_3);
	// shared(a,b)
	static_assert(noexcept(shared(TE.D_1, TE.O_4)));
	EXPECT_EQ(shared(TE.O_4, O_3), O_3);
	EXPECT_EQ(shared(TE.D_1, TE.O_4), TE.D_1);
	EXPECT_EQ(shared(TE.O_4, E_2), E_2);
	EXPECT_EQ(shared(O_3, A_2), A_2);
	EXPECT_EQ(shared(E_3, A_2), E_2);
}
TEST(Options, operator_min)
{
	const Options<9> O_1{std::bitset<10>{"0111011011"}};
	const Options<9> O_2{std::bitset<10>{"1010101111"}};
	const Options<9> A_1{std::bitset<10>{"0101010000"}};
	const Options<9> A_2{std::bitset<10>{"1000100100"}};

	static_assert(noexcept(O_1 - O_2));
	EXPECT_EQ(O_1 - O_2, A_1);
	EXPECT_EQ(O_2 - O_1, A_2);
}
TEST(Options, deathtests)
{
	Options<4> TMP{};
	// Construction
#ifdef _DEBUG
	// Important	exeptrion thrown outside debug-mode
	// assert serves to cach E+1 case
	EXPECT_DEBUG_DEATH({ Options<3>{Value{4}}; }, "Assertion failed: .*");
	// debug contains assert( <= )
	EXPECT_DEBUG_DEATH(Options<4>{Value{5}}, "Assertion failed:");

	// TODO won't trigger for constexpr, test required
	// EXPECT_DEBUG_DEATH(TMP = 6, "Assertion failed:");
	// EXPECT_DEBUG_DEATH(TMP = -6, "Assertion failed:");
#endif // _DEBUG

	// mf_boolRequest
#ifdef _DEBUG
	EXPECT_DEATH({ is_answer(TE.A_1, Value{15}); }, "Assertion failed: .*");

	EXPECT_DEBUG_DEATH(
		{ is_option(TE.O_1, Value{15}); }, "Assertion failed: .*");
	// mf_constOperators
	EXPECT_DEBUG_DEATH({ TE.O_3[Value{9}]; }, "Assertion failed: .*");
	bool a{};
	EXPECT_DEBUG_DEATH({ a = TE.O_3[Value{5}]; }, "Assertion failed: .*");
	// operator[]

	//! supposed to be noexcept, and no bounds-checks in release-mode
	Options<4> Opp{std::bitset<5>{"00000"}};
	// EXPECT_DEBUG_DEATH({ Opp[Value{3}] == Opp[-2]; }, "Assertion failed:
	// .*");
	EXPECT_DEBUG_DEATH({ Opp[Value{5}] = true; }, "Assertion failed: .*");
	// mf_add
	Options<4> Opt{std::bitset<5>{"00000"}};
	EXPECT_DEBUG_DEATH({ Opt.add_nocheck(Value{5}); }, "Assertion failed: .*");
	// mf_set
	EXPECT_TRUE(TMP.clear().is_empty());
	EXPECT_DEBUG_DEATH({ TMP.set_nocheck(Value{15}); }, "Assertion failed: .*");
#endif // _DEBUG
}
} // namespace SudokuTests::OptionsTest
