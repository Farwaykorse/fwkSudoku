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


namespace SudokuTests::OptionsTest
{
using ::Sudoku::Options;
using ::Sudoku::Value;

namespace compiletime
{
	// Class properties
	using typeT = ::Sudoku::Options<9>;
	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_empty_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
	static_assert(not std::is_trivial_v<typeT>);

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
	static_assert(
		std::is_nothrow_constructible_v<Options<3>, const std::bitset<4>&>);
	static_assert(std::is_assignable_v<Options<3>, std::bitset<4>>);
	static_assert(std::is_nothrow_assignable_v<Options<3>, std::bitset<4>>);
	static_assert(noexcept(Options<3>{std::bitset<4>{}}));
	static_assert(std::is_nothrow_assignable_v<Options<3>, std::bitset<4>>);
	static_assert(noexcept(Options<3>() = std::bitset<4>{}));
	// explicit:
	static_assert(std::is_constructible_v<std::bitset<4>, std::string>);
	static_assert(
		not std::is_nothrow_constructible_v<std::bitset<4>, std::string>);
	static_assert(not std::is_constructible_v<typeT, std::string>, "explicit");
	// size:
	static_assert(not std::is_constructible_v<typeT, const std::bitset<9>&>);
	static_assert(not std::is_assignable_v<Options<3>, std::bitset<3>>);

	static_assert(not std::is_swappable_with_v<Options<4>, std::bitset<5>>);
	static_assert(not std::is_nothrow_swappable_with_v<typeT, std::bitset<10>>);

	// Value
	static_assert(std::is_constructible_v<Options<3>, Value>);
	static_assert(std::is_nothrow_constructible_v<Options<3>, Value>);
	static_assert(std::is_assignable_v<Options<3>, Value>);
	static_assert(std::is_nothrow_assignable_v<Options<3>, Value>);
	static_assert(noexcept(Options<3>{Value{}}));
	static_assert(noexcept(Options<3>{Value{0}}));
	static_assert(noexcept(Options<3>{Value{9}}));
	static_assert(noexcept(Options<3>{Value{10}}));
	static_assert(noexcept(Options<3>() = Value{}));
	static_assert(noexcept(Options<3>() = Value{0}));
	static_assert(noexcept(Options<3>() = Value{1}));
	static_assert(noexcept(Options<3>() = Value{9}));
	static_assert(noexcept(Options<3>() = Value{10}));

	static_assert(not std::is_constructible_v<Options<3>, int>);
	static_assert(not std::is_assignable_v<Options<3>, int>);
	static_assert(not std::is_constructible_v<Options<3>, unsigned int>);
	static_assert(not std::is_assignable_v<Options<3>, unsigned int>);
	static_assert(not std::is_constructible_v<Options<3>, size_t>);
	static_assert(not std::is_assignable_v<Options<3>, size_t>);

	namespace impl
	{
		using ::Sudoku::impl::exp2_;
		using ::Sudoku::impl::all_set;

		static_assert(exp2_(0U) == 0x1U);
		static_assert(exp2_(1U) == 0x2U);
		static_assert(exp2_(2U) == 0x4U);
		static_assert(exp2_(3U) == 0x8U);
		static_assert(exp2_(4U) == 0x10U);
		static_assert(exp2_(5U) == 0x20U);
		static_assert(exp2_(6U) == 0x40U);
		static_assert(exp2_(7U) == 0x80U);
		static_assert(exp2_(8U) == 0x100U);
		static_assert(exp2_(9U) == 0x200U);

		static_assert(exp2_<9>(Value{0}) == 0x1U);
		static_assert(exp2_<9>(Value{1}) == 0x2U);
		static_assert(exp2_<9>(Value{2}) == 0x4U);
		static_assert(exp2_<9>(Value{3}) == 0x8U);
		static_assert(exp2_<9>(Value{4}) == 0x10U);
		static_assert(exp2_<9>(Value{5}) == 0x20U);
		static_assert(exp2_<9>(Value{6}) == 0x40U);
		static_assert(exp2_<9>(Value{7}) == 0x80U);
		static_assert(exp2_<9>(Value{8}) == 0x100U);
		static_assert(exp2_<9>(Value{9}) == 0x200U);

		static_assert(all_set(0U) == 0x1U);
		static_assert(all_set(1U) == 0x3U);
		static_assert(all_set(2U) == 0x7U);
		static_assert(all_set(3U) == 0xFU);
		static_assert(all_set(4U) == 0x1FU);
		static_assert(all_set(5U) == 0x3FU);
		static_assert(all_set(6U) == 0x7FU);
		static_assert(all_set(7U) == 0xFFU);
		static_assert(all_set(8U) == 0x1FFU);
		static_assert(all_set(9U) == 0x3FFU);

		// Outside defined size
		static_assert(exp2_<9>(Value{10}) == 0U);
		static_assert(exp2_<4>(Value{5}) == 0U);
	} // namespace impl

} // namespace compiletime
TEST(Options, Construction)
{
	const Options<4> D_0{};
	{
		SCOPED_TRACE("Default Constructor : Options()");
		static_assert(noexcept(Options<4>()), "Options() should be noexcept");
		static_assert(noexcept(Options<4>{}), "Options{} should be noexcept");

		EXPECT_EQ(Options<0>().DebugString(), "1");
		EXPECT_EQ(Options<1>().DebugString(), "11");
		EXPECT_EQ(Options<2>().DebugString(), "111");
		EXPECT_EQ(Options<4>().DebugString(), "11111");
		EXPECT_TRUE(is_option(Options<4>{}, Value{1}));

		const Options<4> D;
		EXPECT_EQ(D_0.size(), size_t{5});
		EXPECT_EQ(D_0, D_0) << "operator== failed, required";
		EXPECT_EQ(D_0, D);
		EXPECT_EQ(D_0.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Copy Constructor : Options(const Options&)");
		static_assert(noexcept(Options<4>(D_0)));
		const Options<4> Opt(D_0);
		EXPECT_EQ(Opt.size(), size_t{5});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Copy Assignment : Options& operator=(const Options&)");
		const Options<4> Opt = D_0;
		EXPECT_EQ(Opt.size(), size_t{5});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Move Constructor : Options(Options&&)");
		const Options<4> Opt{D_0};
		EXPECT_EQ(Opt.size(), size_t{5});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	{
		SCOPED_TRACE("Move Assignment : Options& operator=(Options&&)");
		const Options<4> Opt = Options<4>{};
		EXPECT_EQ(Opt.size(), size_t{5});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "11111");
	}
	//===------------------------------------------------------------------===//
	// std::bitset
	{
		SCOPED_TRACE("Options(const bitset&)"); // 0th bit is last in input
		const std::bitset<5> bit{"10101"};
		const Options<4> Opt{bit};
		EXPECT_EQ(Opt.size(), size_t{5});
		EXPECT_EQ(Opt.DebugString(), "10101");
	}
	{
		SCOPED_TRACE("Options(bitset&&)");
		const Options<4> A_0{std::bitset<5>{"01000"}};
		EXPECT_EQ(A_0.size(), size_t{5});
		EXPECT_EQ(A_0.DebugString(), "01000");
	}
	{
		SCOPED_TRACE("Options& operator=(const bitset&)");
		Options<4> B_0{};
		const std::bitset<5> bit{"11011"};
		B_0 = bit;
		EXPECT_EQ(B_0.DebugString(), "11011");
	}
	{
		SCOPED_TRACE("Options& operator=(bitset&&)");
		Options<4> B_1{};
		B_1 = std::bitset<5>{"11011"};
		EXPECT_EQ(B_1.DebugString(), "11011");
	}
	//===------------------------------------------------------------------===//
	{ // Value
		SCOPED_TRACE("Options(Value)");
		const Options<4> A_1{Value{2}}; // set() // clear() // add()
		const Options<4> A_2 = Value{2};
		EXPECT_EQ(A_1.DebugString(), "00100");
		EXPECT_EQ(A_2.DebugString(), "00100");
		EXPECT_EQ(Options<4>{Value{0}}.DebugString(), "00001"); // [count-0]
		EXPECT_EQ(Options<4>{Value{4}}.DebugString(), "10000");
		EXPECT_EQ(Options<4>{Value{4}}.DebugString(), "10000");
		EXPECT_EQ(Options<9>{Value{9}}.DebugString(), "1000000000");
		EXPECT_EQ(Options<9>{Value{2}}.DebugString(), "0000000100");

		static_assert(noexcept(Options<4>{Value{2}}));
		static_assert(noexcept(Options<4>{Value{4}}));
		static_assert(noexcept(Options<4>{Value{8}})); // outside
	}
	{
		SCOPED_TRACE("Options& operator=(Value)");
		Options<4> TMP{};
		TMP = Value{1}; // set() // clear() // add()
		EXPECT_EQ(TMP.DebugString(), "00010");
		EXPECT_EQ((TMP = Value{3}).DebugString(), "01000");
		EXPECT_EQ((TMP = Value{0}).DebugString(), "00001"); // [count-0]
	}
	{ // assert serves to catch E+1 case
		EXPECT_DEBUG_DEATH({ Options<3>{Value{4}}; }, "Assertion failed: .*");
		EXPECT_DEBUG_DEATH({ Options<3>{Value{5}}; }, "Assertion failed: .*");
		// debug contains assert( <= )
		EXPECT_DEBUG_DEATH(Options<4>{Value{5}}, "Assertion failed:");

		Options<4> TMP{};
		EXPECT_TRUE(TMP.all());
		EXPECT_DEBUG_DEATH(TMP = Value{6}, "Assertion failed:");
		EXPECT_DEBUG_DEATH(TMP = Value{10}, "Assertion failed:");
#ifdef NDEBUG
		// outside domain
		EXPECT_TRUE(Options<4>{Value{5}}.is_empty());
		EXPECT_TRUE(Options<9>{Value{10}}.is_empty());
#endif // NDEBUG
		TMP = Value{4}; // suppress warning: assigned only once
		EXPECT_EQ(TMP.DebugString(), "10000");
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
	const Options<4> E_3{Value{0}};                // empty answer "00001"
	const Options<4> X_0{std::bitset<5>{"11110"}}; // answered, all options
	const Options<4> X_1{std::bitset<5>{"10100"}}; // answered, 2 options
};
static TestElements TE;

TEST(Options, mf_counting)
{
	static_assert(noexcept(TE.D_0.size()));
	EXPECT_EQ(TE.D_0.size(), size_t{10});
	EXPECT_EQ(TE.D_1.size(), size_t{5});
	EXPECT_EQ(TE.O_1.size(), size_t{5});
	EXPECT_EQ(TE.E_1.size(), size_t{5});
	constexpr Options<9> X1{Value{1}};
	static_assert(X1.size() == size_t{10});
	constexpr Options<4> X2{Value{1}};
	static_assert(X2.size() == size_t{5});

	static_assert(noexcept(TE.O_1.count()));
	static_assert(noexcept(TE.O_1.count_all()));
	// clang-format off
	EXPECT_EQ(TE.D_0.count(), 9u);
	EXPECT_EQ(TE.D_0.count_all(), 9u);
	EXPECT_EQ(TE.A_2.count(), 0u); // different
	EXPECT_EQ(TE.A_2.count_all(), 1u); //
	EXPECT_EQ(TE.O_1.count(), 1u);
	EXPECT_EQ(TE.O_1.count_all(), 1u);
	EXPECT_EQ(TE.O_2.count(), 2u);
	EXPECT_EQ(TE.O_2.count_all(), 2u);
	EXPECT_EQ(TE.O_3.count(), 3u);
	EXPECT_EQ(TE.O_3.count_all(), 3u);
	EXPECT_EQ(TE.E_1.count(), 0u);
	EXPECT_EQ(TE.E_1.count_all(), 0u);
	EXPECT_EQ(TE.E_2.count(), 0u);
	EXPECT_EQ(TE.E_2.count_all(), 0u);
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
	[[maybe_unused]] bool set{};
	static_assert(not noexcept(TE.O_1.test(Value{2})));
	EXPECT_NO_THROW(set = TE.O_1.test(Value{0}));
	EXPECT_NO_THROW(set = TE.O_1.test(Value{4}));
	EXPECT_THROW(set = TE.O_1.test(Value{5}), std::out_of_range);
	EXPECT_THROW(set = TE.O_1.test(Value{13}), std::out_of_range);
	EXPECT_NO_THROW(set = Options<9>().test(Value{5}));
	EXPECT_NO_THROW(set = Options<9>().test(Value{9}));
	EXPECT_THROW(set = Options<9>().test(Value{10}), std::out_of_range);
	EXPECT_TRUE(TE.O_1.test(Value{2}));
	EXPECT_TRUE(TE.O_3.test(Value{2}));
	EXPECT_FALSE(TE.O_3.test(Value{1}));
	EXPECT_FALSE(TE.E_1.test(Value{1}));
	EXPECT_TRUE(TE.X_0.test(Value{1}));
	set = false; // suppress warning: assigned only once
}

TEST(Options, is_answer)
{
	[[maybe_unused]] bool U{};
	{ // member-function
		static_assert(noexcept(TE.O_1.is_answer()));
		EXPECT_TRUE(TE.A_1.is_answer());
		EXPECT_TRUE(TE.A_2.is_answer());
		EXPECT_FALSE(TE.O_1.is_answer());
		EXPECT_FALSE(TE.O_4.is_answer());
		EXPECT_FALSE(TE.E_1.is_answer());
		EXPECT_FALSE(TE.E_2.is_answer());
		EXPECT_FALSE(TE.X_0.is_answer());
		EXPECT_FALSE(TE.X_1.is_answer());
	}
	{ // convenience function: exactly the same
		static_assert(noexcept(is_answer(TE.O_1)));
		EXPECT_TRUE(is_answer(TE.A_1));
		EXPECT_TRUE(is_answer(TE.A_2));
		EXPECT_FALSE(is_answer(TE.O_1));
		EXPECT_FALSE(is_answer(TE.O_4));
		EXPECT_FALSE(is_answer(TE.E_1));
		EXPECT_FALSE(is_answer(TE.E_2));
		EXPECT_FALSE(is_answer(TE.X_0));
		EXPECT_FALSE(is_answer(TE.X_1));
		static_assert(noexcept(is_answer(TE.O_1, Value{1})));
		EXPECT_DEBUG_DEATH(
			{ U = is_answer(TE.A_1, Value{15}); },
			"Assertion failed: value <= Value.E."); // constructor
	}
	{ // test for specific answer
		static_assert(noexcept(is_answer(TE.O_1, Value{1})));
		static_assert(noexcept(is_answer(TE.A_1, Value{4})));
		// assertion see death tests
		EXPECT_TRUE(is_answer(TE.A_2, Value{2}));
		EXPECT_FALSE(is_answer(TE.A_2, Value{3}));
		EXPECT_FALSE(is_answer(TE.A_1, Value{0}));
		EXPECT_FALSE(is_answer(TE.O_1, Value{2}));
		EXPECT_FALSE(is_answer(TE.O_1, Value{2}));
		EXPECT_FALSE(is_answer(TE.E_1, Value{0}));
	}
	{ // test if answer flag set
		constexpr Options<9> ans{Value{2}};
		static_assert(is_answer_fast(ans), "Failure: constexpr is_answer_fast");
		static_assert(!ans[Value{0}]); // dependencies
		static_assert(!ans[Value{1}]); //
		static_assert(ans[Value{2}]);  //

		static_assert(noexcept(is_answer_fast(TE.O_1)));
		static_assert(noexcept(is_answer_fast(ans)));
		static_assert(noexcept(is_answer_fast(Options<9>{Value{}})));
		static_assert(noexcept(is_answer_fast(Options<9>{Value{0}})));
		static_assert(noexcept(is_answer_fast(Options<9>{Value{1}})));
		static_assert(noexcept(is_answer_fast(Options<9>{Value{9}})));
		static_assert(noexcept(is_answer_fast(Options<9>{Value{10}})));

		static_assert(is_answer_fast(ans), "Failure: constexpr is_answer_fast");
		static_assert(!is_answer_fast(Options<9>{Value{}}));
		static_assert(!is_answer_fast(Options<9>{Value{0}}));
		static_assert(is_answer_fast(Options<9>{Value{1}}));
		static_assert(is_answer_fast(Options<9>{Value{9}}));
#ifndef NDEBUG
		EXPECT_DEATH(
			{ U = is_answer_fast(Options<9>{Value{10}}); },
			"Assertion failed: value <= Value.E.");
#endif // NDEBUG

		EXPECT_TRUE(is_answer_fast(TE.A_1));
		EXPECT_TRUE(is_answer_fast(TE.A_2));
		EXPECT_FALSE(is_answer_fast(TE.O_1));
		EXPECT_FALSE(is_answer_fast(TE.O_4));
		// invalid data:
		EXPECT_TRUE(is_answer_fast(TE.E_1));  // 00000 false for is_answer
		EXPECT_FALSE(is_answer_fast(TE.E_2)); // 00001
		EXPECT_TRUE(is_answer_fast(TE.X_0));  // 11110 false for is_answer
		EXPECT_TRUE(is_answer_fast(TE.X_1));  // 10100 false for is_answer
	}
	U = false; // suppress warning: assigned only once
}

TEST(Options, is_option)
{
	static_assert(not noexcept(is_option(TE.O_1, Value{2})));
	[[maybe_unused]] bool U{};
	EXPECT_DEBUG_DEATH(
		{ U = is_option(TE.O_1, Value{0}); }, "Assertion failed: .*");
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH(
		{ U = is_option(TE.O_1, Value{15}); }, "Assertion failed: .*");
#else
	// std::bitset::test
	EXPECT_THROW(U = is_option(TE.O_1, Value{15}), std::out_of_range);
	EXPECT_TRUE(is_option(TE.O_1, Value{0}));
	EXPECT_FALSE(is_option(TE.A_1, Value{0}));
#endif // NDEBUG

	U = false; // suppress warning: assigned only once

	EXPECT_TRUE(is_option(TE.D_1, Value{4}));
	EXPECT_TRUE(is_option(TE.O_1, Value{2}));
	EXPECT_FALSE(is_option(TE.A_2, Value{2}));
	EXPECT_FALSE(is_option(TE.O_3, Value{1}));
	EXPECT_TRUE(is_option(TE.O_3, Value{2}));
	EXPECT_FALSE(is_option(TE.X_0, Value{2})); // incorrect answer flag
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

	static_assert(noexcept(available(TE.O_1)));
	// allocation can throw std::bad_alloc

	ASSERT_NO_THROW(result = available(TE.O_2));
	EXPECT_EQ(result.size(), size_t{2});
	EXPECT_EQ(result[0], Value{1});
	EXPECT_EQ(result[1], Value{3});
	// empty
	ASSERT_NO_THROW(result = available(TE.E_1));
	EXPECT_EQ(result.size(), size_t{0});
	ASSERT_NO_THROW(result = available(TE.E_2));
	EXPECT_EQ(result.size(), size_t{0});
	// invalid answer-bit
	ASSERT_NO_THROW(result = available(TE.X_1));
	EXPECT_EQ(result.size(), size_t{0}); //??? won't work for is_option() ...
	ASSERT_NO_THROW(result = available(TE.D_1));
	EXPECT_EQ(result.size(), size_t{4});
	// answer set
	ASSERT_NO_THROW(result = available(TE.A_1));
	EXPECT_EQ(result.size(), size_t{0});
	// answer not set
	ASSERT_NO_THROW(result = available(TE.O_1));
	EXPECT_EQ(result.size(), size_t{1});
	ASSERT_NO_THROW(result = available(TE.O_3));
	EXPECT_EQ(result.size(), size_t{3});
	EXPECT_EQ(result[0], Value{2});
	EXPECT_EQ(result[1], Value{3});
	EXPECT_EQ(result[2], Value{4});
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
	// Set-up
	Options<4> TMP{};
	ASSERT_EQ(TMP.size(), size_t{5}) << "Invalid object";
	ASSERT_TRUE(TMP.all()) << "All options should be available";
	EXPECT_EQ(TMP.count(), 4u);
	EXPECT_EQ(TMP.count_all(), 4u);
	ASSERT_EQ(TMP, TE.D_1) << "Default set D_1 is not complete";

	static_assert(noexcept(TMP.clear()));
	TMP = TE.D_1; // reset test data
	ASSERT_EQ(TMP, TE.D_1) << "Reset test data failed";
	TMP.clear();
	EXPECT_TRUE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 0u);
	EXPECT_EQ(TMP.count_all(), 0u);
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.clear().is_empty());
	static_assert(noexcept(TMP.reset()));
	ASSERT_TRUE(TE.E_1.is_empty()) << "E_1 should be empty";
	TMP = TE.E_1;
	ASSERT_EQ(TMP, TE.E_1) << "Reset test data failed";
	TMP.reset();
	EXPECT_FALSE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 4u);
	EXPECT_EQ(TMP.count_all(), 4u);
	EXPECT_TRUE(is_option(TMP, Value{2}));
	EXPECT_TRUE(TMP.all());
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.reset().all());
	static_assert(noexcept(TMP.flip()));
	TMP = TE.D_1;
	ASSERT_TRUE(TMP.all()) << "Reset test data failed";
	TMP.flip();
	EXPECT_TRUE(TMP.is_empty());
	EXPECT_EQ(TMP.count_all(), 0u);
	TMP.flip();
	EXPECT_TRUE(TMP.all());
	EXPECT_EQ(TMP.count_all(), 4u);
}
TEST(Options, mf_remove_option)
{
	Options<4> TMP{};
	static_assert(not noexcept(TMP.remove_option(Value{3})));
	ASSERT_NO_THROW(TMP.remove_option(Value{3}));
#ifndef NDEBUG
	EXPECT_DEATH(
		TMP.remove_option(Value{5}), "Assertion failed: is_valid_option<E>");
#else
	EXPECT_THROW(TMP.remove_option(Value{5}), std::out_of_range);
#endif // NDEBUG
	TMP.set(Value{3});
	EXPECT_DEBUG_DEATH(
		TMP.remove_option(Value{3}), "Assertion failed: not Sudoku::is_answer");

	ASSERT_TRUE(TMP.reset().all()) << "Reset test data failed";
	ASSERT_EQ(TMP.size(), size_t{5}) << "Test requires Options<4> object";
	EXPECT_TRUE(TMP.test(Value{3}));
	EXPECT_NO_THROW(TMP.remove_option(Value{3}));
	EXPECT_FALSE(TMP.test(Value{3}));
	EXPECT_EQ(TMP.count(), 3u);
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
	Opt = std::bitset<5>{"00000"};
	static_assert(noexcept(Opt.add_nocheck(Value{1})));
	// assertion death tests
#ifndef NDEBUG
	EXPECT_DEATH({ Opt.add_nocheck(Value{5}); }, "Assertion failed: .*");
#else
	// EXPECT_NO_THROW(Opt.add_nocheck(Value{6}));
	// EXPECT_NO_THROW(Opt.add_nocheck(-1));
#endif // NDEBUG
	EXPECT_NO_THROW(Opt.add_nocheck(Value{3}));
	EXPECT_EQ(Opt.DebugString(), "01000");
	EXPECT_FALSE(Opt.count_all() == 0u);
	EXPECT_TRUE(Opt.test(Value{3}));
	EXPECT_TRUE(Opt.add_nocheck(Value{4}).test(Value{4}));
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
	EXPECT_EQ(TMP.count(), 0u);
	EXPECT_TRUE(is_answer(TMP.set(Value{1}), Value{1}));
	EXPECT_FALSE(is_answer(TMP, Value{4}));
	EXPECT_TRUE(TMP.set(Value{0}).is_empty());
	EXPECT_EQ(TMP.count_all(), 0u);
	EXPECT_EQ(TMP.DebugString(), "00001");

	// set_noexcept(int)
	static_assert(noexcept(TMP.set_nocheck(Value{2})));
	EXPECT_TRUE(TMP.clear().is_empty());
	EXPECT_NO_THROW(TMP.set_nocheck(Value{1}));
	EXPECT_EQ(TMP.DebugString(), "00010");
	EXPECT_TRUE(is_answer(TMP.set_nocheck(Value{2}), Value{2}));
	// assertion death tests
	EXPECT_TRUE(TMP.clear().is_empty());
#ifndef NDEBUG
	EXPECT_DEATH({ TMP.set_nocheck(Value{15}); }, "Assertion failed: .*");
#endif // NDEBUG
	EXPECT_NO_THROW(TMP.set_nocheck(Value{0}));
	TMP = Value{1}; // suppress warning: assigned only once
}

TEST(Options, mf_booleanComparison)
{
	[[maybe_unused]] bool U{};

	// operator==(Value) const
	static_assert(noexcept(TE.A_1 == Value{1}));
	static_assert(noexcept(Value{1} == TE.A_1));
	EXPECT_DEBUG_DEATH(
		{ U = operator==(TE.A_1, Value{15}); },
		"Assertion failed: .*"); // constructor
	EXPECT_DEBUG_DEATH(
		{ U = operator==(Value{15}, TE.A_1); }, "Assertion failed: .*");
#ifdef NDEBUG
	EXPECT_FALSE(operator==(TE.A_1, Value{15}));
	EXPECT_FALSE(operator==(Value{15}, TE.A_1));
	EXPECT_TRUE(operator==(Value{15}, TE.E_1));
#endif // NDEBUG
	EXPECT_EQ(TE.A_1, Value{1});
	EXPECT_EQ(Value{1}, TE.A_1);
	EXPECT_EQ(TE.A_2, Value{2});

	// operator!=(Value) const
	static_assert(noexcept(TE.A_1 != Value{1}));
	static_assert(noexcept(Value{1} != TE.A_1));
	EXPECT_DEBUG_DEATH(
		{ U = operator!=(TE.A_1, Value{15}); }, "Assertion failed: .*");
	EXPECT_DEBUG_DEATH(
		{ U = operator!=(Value{15}, TE.A_1); }, "Assertion failed: .*");
	U = false; // suppress warning: assigned only once
#ifdef NDEBUG
	EXPECT_TRUE(operator!=(TE.A_1, Value{15}));
	EXPECT_TRUE(operator!=(Value{15}, TE.A_1));
	EXPECT_FALSE(operator!=(Value{15}, TE.E_1));
#endif // NDEBUG
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

	constexpr Options<4> x{Value{1}};
	static_assert(x[Value{1}]);
	static_assert(not x[Value{2}]);
	constexpr Options<9> y1{Value{1}};
	static_assert(y1[Value{1}]);
	constexpr Options<9> y9{Value{1}};
	static_assert(y9[Value{1}]);

#ifndef NDEBUG
	EXPECT_DEATH(
		{ [[maybe_unused]] bool val = TE.O_3[Value{5}]; },
		"Assertion failed: .*");
#else
	//! supposed to be noexcept, and no bounds-checks in release-mode
	//  might still trigger SEH error?
	[[maybe_unused]] bool val{};
	EXPECT_NO_THROW(val = TE.O_3[Value{9}]);
#endif // NDEBUG
	EXPECT_TRUE(TE.O_1[Value{2}]);
	EXPECT_TRUE(TE.A_2[Value{2}]);
	EXPECT_TRUE(TE.A_2[Value{1}] == false);
}

TEST(Options, Operators)
{
	// _test-data
	Options<4> TMP{}; // per test, reset this option

	///// non-const operators /////
	static_assert(noexcept(TMP.operator[](Value{0}) = true));
#ifndef NDEBUG
	EXPECT_DEBUG_DEATH({ TMP[Value{5}] = true; }, "Assertion failed: .*");
#else
	//! supposed to be noexcept, and no bounds-checks in release-mode
	//  might still trigger SEH error?
	// ASSERT_GT(sizeof(TMP), size_t{1});
	// EXPECT_NO_THROW(TMP[Value{7}] = true);
#endif // NDEBUG
	TMP.clear();
	EXPECT_TRUE(TMP[Value{0}] = true);
	EXPECT_TRUE(TMP[Value{0}] == true);
	EXPECT_FALSE(TMP[Value{0}].flip());
	EXPECT_TRUE(TMP[Value{0}] == false);
	// Options& XOR(Options&)			XOR
	static_assert(noexcept(TMP.XOR(TE.O_1)));
	TMP = TE.E_2;
	EXPECT_TRUE(TMP.XOR(TE.A_2) == TE.O_1);
	EXPECT_TRUE((TMP.XOR(TE.A_2)) == TE.E_2);
	// Options& operator+=(Options&)			combine options
	static_assert(noexcept(TMP += TE.O_4));
	TMP = TE.E_1;
	TMP += TE.O_4;
	EXPECT_TRUE(TE.D_1 == TE.O_4);
	// others tested by using operator+()
	// Options operator+(Options&) const		combine options
	static_assert(noexcept(TE.D_1 + TE.O_4));
	EXPECT_TRUE(TE.D_1 + TE.O_4 == TE.D_1);
	EXPECT_TRUE(TE.E_3 + TE.D_1 == TE.D_1);
	EXPECT_TRUE(TE.A_2 + TE.E_2 == TE.O_1);
	EXPECT_TRUE(TE.E_3 + TE.A_2 == TE.O_1);
}

TEST(Options, ConstructorTesting)
{
	Options<4> TMP{};
	///// testing the constructors /////
	EXPECT_TRUE(TE.E_3 == TE.E_2);
	// copy-assign
	static_assert(noexcept(TMP.operator=(TE.O_4)));
	static_assert(noexcept(TMP.operator=(Value{1})));

	TMP = TE.A_2;
	EXPECT_TRUE(is_answer(TMP, Value{2}));
	EXPECT_TRUE(TMP == TE.A_2);
	const Options<4> TMP2 = Value{3};
	EXPECT_TRUE(is_answer(TMP2, Value{3}));

	// move-assign
	static_assert(noexcept(TMP.operator=(std::bitset<5>())));
	const Options<4> TMP3 = Options<4>(std::bitset<5>{}); // "00000"
	EXPECT_TRUE(TMP3 == TE.E_1);
}

TEST(Options, External)
{
	const Options<4> O_3{std::bitset<5>{"00101"}};   // single option 2
	[[maybe_unused]] const Options<4> E_1{Value{0}}; // empty answer "00001"
	const Options<4> E_2{std::bitset<5>{"00000"}};   // empty
	const Options<4> E_3{std::bitset<5>{"00001"}};   // empty option
	[[maybe_unused]] const Options<4> A_1{Value{1}}; // answer 1
	const Options<4> A_2{std::bitset<5>{"00100"}};   // answer 2
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
	const Options<9> all{};
	const Options<9> empty{std::bitset<10>{"0000000000"}};
	constexpr Options<9> E_1{Value{0}};
	const Options<9> O_0{std::bitset<10>{"0111011011"}};
	const Options<9> O_1{std::bitset<10>{"0111011010"}};
	const Options<9> O_2{std::bitset<10>{"1010101110"}};
	const Options<9> A_0{std::bitset<10>{"0101010001"}};
	const Options<9> A_1{std::bitset<10>{"0101010000"}};
	const Options<9> A_2{std::bitset<10>{"1000100100"}};

	static_assert(noexcept(O_1 - O_2));
	EXPECT_DEBUG_DEATH([[maybe_unused]] auto X = O_1 - all, "is_answer");
#ifdef NDEBUG
	EXPECT_EQ(O_1 - all, empty);
#endif // NDEBUG
	EXPECT_EQ(O_0 - O_1, E_1);
	EXPECT_EQ(O_0 - O_2, A_0);
	EXPECT_EQ(O_1 - O_2, A_1);
	EXPECT_EQ(O_2 - O_1, A_2);
	EXPECT_EQ(O_1 - O_1, empty);
	EXPECT_EQ(O_1 - empty, O_1);
	EXPECT_EQ(empty - O_2, empty);
}

} // namespace SudokuTests::OptionsTest
