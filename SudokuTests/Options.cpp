// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
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
#include "print_Options.h" // Debug Output

#include <Sudoku/Options.h> // Class under test
#include <Sudoku/Value.h>   // Helpers

#include <bitset>
#include <string>
#include <vector>

#include <type_traits>

#include <gtest/gtest.h>


namespace SudokuTests::OptionsTest
{
using ::Sudoku::Options;
using ::Sudoku::Value;
using ::Sudoku::OptionValue;

namespace compiletime
{
	using typeT = ::Sudoku::Options<9>;
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
#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(std::is_swappable_v<typeT>);
	static_assert(std::is_nothrow_swappable_v<typeT>);
#endif // __ICL

	// type construction
	// from std::bitset
	static_assert(std::is_constructible_v<Options<3>, const std::bitset<3>&>);
	static_assert(
		std::is_nothrow_constructible_v<Options<3>, const std::bitset<3>&>);
	static_assert(std::is_assignable_v<Options<3>, std::bitset<3>>);
	static_assert(std::is_nothrow_assignable_v<Options<3>, std::bitset<3>>);
	static_assert(noexcept(Options<3>{std::bitset<3>{}}));
	static_assert(std::is_nothrow_assignable_v<Options<3>, std::bitset<3>>);
	static_assert(noexcept(Options<3>() = std::bitset<3>{}));
	// explicit:
	static_assert(std::is_constructible_v<std::bitset<3>, std::string>);
	static_assert(
		not std::is_nothrow_constructible_v<std::bitset<3>, std::string>);
	static_assert(not std::is_constructible_v<typeT, std::string>, "explicit");
	// size:
	static_assert(not std::is_constructible_v<typeT, const std::bitset<8>&>);
	static_assert(not std::is_assignable_v<Options<3>, std::bitset<4>>);

#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(not std::is_swappable_with_v<Options<4>, std::bitset<4>>);
	static_assert(not std::is_nothrow_swappable_with_v<typeT, std::bitset<9>>);
#endif // __ICL

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

	// OptionValue<E>
	static_assert(std::is_constructible_v<Options<9>, OptionValue<9>>);
	static_assert(std::is_nothrow_constructible_v<Options<9>, OptionValue<9>>);
	static_assert(std::is_assignable_v<Options<9>, OptionValue<9>>);
	static_assert(std::is_nothrow_assignable_v<Options<9>, OptionValue<9>>);

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
		static_assert(exp2_(1U) == 0x1U);
		static_assert(exp2_(2U) == 0x2U);
		static_assert(exp2_(3U) == 0x4U);
		static_assert(exp2_(4U) == 0x8U);
		static_assert(exp2_(5U) == 0x10U);
		static_assert(exp2_(6U) == 0x20U);
		static_assert(exp2_(7U) == 0x40U);
		static_assert(exp2_(8U) == 0x80U);
		static_assert(exp2_(9U) == 0x100U);

		static_assert(exp2_<9>(Value{0}) == 0x1U);
		static_assert(exp2_<9>(Value{1}) == 0x1U);
		static_assert(exp2_<9>(Value{2}) == 0x2U);
		static_assert(exp2_<9>(Value{3}) == 0x4U);
		static_assert(exp2_<9>(Value{4}) == 0x8U);
		static_assert(exp2_<9>(Value{5}) == 0x10U);
		static_assert(exp2_<9>(Value{6}) == 0x20U);
		static_assert(exp2_<9>(Value{7}) == 0x40U);
		static_assert(exp2_<9>(Value{8}) == 0x80U);
		static_assert(exp2_<9>(Value{9}) == 0x100U);

		static_assert(all_set(0U) == 0x0U);
		static_assert(all_set(1U) == 0x1U);
		static_assert(all_set(2U) == 0x3U);
		static_assert(all_set(3U) == 0x7U);
		static_assert(all_set(4U) == 0xFU);
		static_assert(all_set(5U) == 0x1FU);
		static_assert(all_set(6U) == 0x3FU);
		static_assert(all_set(7U) == 0x7FU);
		static_assert(all_set(8U) == 0xFFU);
		static_assert(all_set(9U) == 0x1FFU);

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

		EXPECT_EQ(Options<1>().DebugString(), "1");
		EXPECT_EQ(Options<2>().DebugString(), "11");
		EXPECT_EQ(Options<4>().DebugString(), "1111");
		EXPECT_TRUE(is_option(Options<4>{}, Value{1}));

		const Options<4> D;
		EXPECT_EQ(D_0.size(), size_t{4});
		EXPECT_EQ(D_0, D_0) << "operator== failed, required";
		EXPECT_EQ(D_0, D);
		EXPECT_EQ(D_0.DebugString(), "1111");
	}
	{
		SCOPED_TRACE("Copy Constructor : Options(const Options&)");
		static_assert(noexcept(Options<4>(D_0)));
		const Options<4> Opt(D_0);
		EXPECT_EQ(Opt.size(), size_t{4});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "1111");
	}
	{
		SCOPED_TRACE("Copy Assignment : Options& operator=(const Options&)");
		const Options<4> Opt = D_0;
		EXPECT_EQ(Opt.size(), size_t{4});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "1111");
	}
	{
		SCOPED_TRACE("Move Constructor : Options(Options&&)");
		const Options<4> Opt{D_0};
		EXPECT_EQ(Opt.size(), size_t{4});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "1111");
	}
	{
		SCOPED_TRACE("Move Assignment : Options& operator=(Options&&)");
		const Options<4> Opt = Options<4>{};
		EXPECT_EQ(Opt.size(), size_t{4});
		EXPECT_EQ(Opt, D_0);
		EXPECT_EQ(Opt.DebugString(), "1111");
	}
	//===------------------------------------------------------------------===//
	// std::bitset
	{
		SCOPED_TRACE("Options(const bitset&)"); // 0th bit is last in input
		const std::bitset<4> bit{0b1010};
		const Options<4> Opt{bit};
		EXPECT_EQ(Opt.size(), size_t{4});
		EXPECT_EQ(Opt.DebugString(), "1010");
	}
	{
		SCOPED_TRACE("Options(bitset&&)");
		const Options<4> A_0{std::bitset<4>{0b0100}};
		EXPECT_EQ(A_0.size(), size_t{4});
		EXPECT_EQ(A_0.DebugString(), "0100");
	}
	{
		SCOPED_TRACE("Options& operator=(const bitset&)");
		Options<4> B_0{};
		const std::bitset<4> bit{0b1101};
		B_0 = bit;
		EXPECT_EQ(B_0.DebugString(), "1101");
	}
	{
		SCOPED_TRACE("Options& operator=(bitset&&)");
		Options<4> B_1{};
		B_1 = std::bitset<4>{0b1101};
		EXPECT_EQ(B_1.DebugString(), "1101");
	}
	//===------------------------------------------------------------------===//
	{ // Value
		SCOPED_TRACE("Options(Value)");
		const Options<4> A_1{Value{2}}; // set() // clear() // add()
		const auto A_2 = Options<4>{Value{2}};
		EXPECT_EQ(A_1.DebugString(), "0010");
		EXPECT_EQ(A_2.DebugString(), "0010");
		EXPECT_EQ(Options<4>{Value{4}}.DebugString(), "1000");
		EXPECT_EQ(Options<4>{Value{4}}.DebugString(), "1000");
		EXPECT_EQ(Options<9>{Value{9}}.DebugString(), "100000000");
		EXPECT_EQ(Options<9>{Value{2}}.DebugString(), "000000010");
	}
	{
		SCOPED_TRACE("Options& operator=(Value)");
		Options<4> TMP{};
		TMP = Value{1}; // set() // clear() // add()
		EXPECT_EQ(TMP.DebugString(), "0001");
		EXPECT_EQ((TMP = Value{3}).DebugString(), "0100");
	}
}
TEST(OptionsDeathTest, Construction)
{
	// Value
	SCOPED_TRACE("Options(Value)");
	EXPECT_DEBUG_DEATH(Options<4>{Value{0}}, "val > Value");
	{ // catch the E+1 case
		EXPECT_DEBUG_DEATH({ Options<3>{Value{4}}; }, "val <= Value");
		EXPECT_DEBUG_DEATH({ Options<3>{Value{5}}; }, "val <= Value");

		Options<4> TMP{};
		EXPECT_TRUE(TMP.all());
		EXPECT_DEBUG_DEATH(TMP = Value{6}, "val <= Value");
		EXPECT_DEBUG_DEATH(TMP = Value{10}, "val <= Value");
		TMP = Value{4}; // suppress warning: assigned only once
		EXPECT_EQ(TMP.DebugString(), "1000");
	}
}


struct TestElements
{
	const Options<9> D_0{};                       // default
	const Options<4> D_1{};                       // default
	const Options<4> A_1{std::bitset<4>{0b0001}}; // answer = 1
	const Options<4> A_2{std::bitset<4>{0b0010}}; // answer = 2
	const Options<4> O_2{std::bitset<4>{0b0101}}; // 2 options 1 & 3
	const Options<4> O_3{std::bitset<4>{0b1110}}; // 3 options 2,3,4
	const Options<4> O_4{std::bitset<4>{0b1111}}; // all options
	const Options<4> O_5{std::bitset<4>{0b1010}}; // 2 options 2 & 4
	const Options<4> E_1{std::bitset<4>{0b0000}}; // empty
};
static TestElements TE;

TEST(Options, mfCounting)
{
	static_assert(noexcept(TE.D_0.size()));
	EXPECT_EQ(TE.D_0.size(), size_t{9});
	EXPECT_EQ(TE.D_1.size(), size_t{4});
	EXPECT_EQ(TE.A_2.size(), size_t{4});
	EXPECT_EQ(TE.E_1.size(), size_t{4});
	constexpr Options<9> X1{Value{1}};
	static_assert(X1.size() == size_t{9});
	constexpr Options<4> X2{Value{1}};
	static_assert(X2.size() == size_t{4});

	static_assert(noexcept(TE.D_0.count()));
	EXPECT_EQ(TE.D_0.count(), 9U);
	EXPECT_EQ(TE.A_2.count(), 1U);
	EXPECT_EQ(TE.O_2.count(), 2U);
	EXPECT_EQ(TE.O_3.count(), 3U);
	EXPECT_EQ(TE.E_1.count(), 0U);
	static_assert(noexcept(TE.D_0.all()));
	EXPECT_TRUE(TE.D_1.all());
	EXPECT_FALSE(TE.O_3.all());
	EXPECT_TRUE(TE.O_4.all());
	EXPECT_FALSE(TE.E_1.all());
	static_assert(noexcept(TE.D_0.is_empty()));
	EXPECT_TRUE(TE.E_1.is_empty());
	EXPECT_FALSE(TE.D_1.is_empty());
	EXPECT_FALSE(TE.A_2.is_empty());
}

TEST(Options, testValue)
{
	[[maybe_unused]] bool set{};

	constexpr OptionValue<4> value{2U};
	static_assert(noexcept(TE.A_2.test(value)));
	EXPECT_NO_THROW(set = TE.A_2.test(Value{4}));
	EXPECT_NO_THROW(set = Options<9>().test(Value{5}));
	EXPECT_NO_THROW(set = Options<9>().test(Value{9}));
	EXPECT_TRUE(TE.A_2.test(Value{2}));
	EXPECT_TRUE(TE.O_3.test(Value{2}));
	EXPECT_FALSE(TE.O_3.test(Value{1}));
	EXPECT_FALSE(TE.E_1.test(Value{1}));
	set = false; // suppress warning: assigned only once
}
#ifndef NDEBUG
TEST(OptionsDeathTest, testValue)
{
	[[maybe_unused]] bool set{};
	EXPECT_DEBUG_DEATH(set = TE.A_2.test(Value{0}), "val > Value");
	EXPECT_DEBUG_DEATH(set = TE.A_2.test(Value{5}), "val <= Value");
	EXPECT_DEBUG_DEATH(set = TE.A_2.test(Value{13}), "val <= Value");
	EXPECT_DEBUG_DEATH(set = Options<9>().test(Value{10}), "val <= Value");
}
#endif
TEST(Options, isAnswer)
{
	[[maybe_unused]] bool U{};
	{ // member-function
		static_assert(noexcept(TE.O_2.is_answer()));
		EXPECT_TRUE(TE.A_1.is_answer());
		EXPECT_TRUE(TE.A_2.is_answer());
		EXPECT_FALSE(TE.O_4.is_answer());
		EXPECT_FALSE(TE.E_1.is_answer());
	}
	{ // convenience function: exactly the same
		static_assert(noexcept(is_answer(TE.O_2)));
		EXPECT_TRUE(is_answer(TE.A_1));
		EXPECT_TRUE(is_answer(TE.A_2));
		EXPECT_FALSE(is_answer(TE.O_4));
		EXPECT_FALSE(is_answer(TE.E_1));
		static_assert(not noexcept(is_answer(TE.O_2, Value{1})));
		constexpr OptionValue<4> value{1U};
		static_assert(noexcept(is_answer(TE.O_2, value)));
	}
	{ // test for specific answer
		constexpr OptionValue<4> value_1{1U};
		constexpr OptionValue<4> value_4{4U};
		static_assert(noexcept(is_answer(TE.O_2, value_1)));
		static_assert(noexcept(is_answer(TE.A_1, value_4)));
		// assertion see death tests
		EXPECT_TRUE(is_answer(TE.A_2, Value{2}));
		EXPECT_FALSE(is_answer(TE.A_2, Value{3}));
		EXPECT_FALSE(is_answer(TE.O_2, Value{2}));
		EXPECT_FALSE(is_answer(TE.O_3, Value{2}));
	}
}

TEST(Options, isOption)
{
	static_assert(not noexcept(is_option(TE.O_2, Value{2})));
	Sudoku::OptionValue<4> value{Value{2}};
	static_assert(noexcept(is_option(TE.O_2, value)));
	EXPECT_TRUE(is_option(TE.D_1, Value{4}));
	EXPECT_FALSE(is_option(TE.A_2, Value{2}));
	EXPECT_FALSE(is_option(TE.O_3, Value{1}));
	EXPECT_TRUE(is_option(TE.O_3, Value{2}));
}
#ifndef NDEBUG
TEST(OptionsDeathTest, isOption)
{
	EXPECT_DEBUG_DEATH(is_option(TE.O_2, Value{0}), "val > Value");
	EXPECT_DEBUG_DEATH(is_option(TE.O_2, Value{10}), "val <= Value");
}
#endif
TEST(Options, readNext)
{
	// must work on const objects
	static_assert(std::is_const_v<decltype(TE.O_2)>);

	static_assert(noexcept(read_next(TE.O_2)));
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
	// on answer (set)
	EXPECT_EQ(read_next(TE.A_2, Value{0}), Value{2});
	EXPECT_EQ(read_next(TE.A_2), Value{2});
	EXPECT_EQ(read_next(TE.A_2, Value{1}), Value{2});
	EXPECT_EQ(read_next(TE.A_2, Value{2}), Value{0});
}

TEST(Options, available)
{
	std::vector<Value> result{};

	static_assert(noexcept(available(TE.O_2)));
	// allocation can throw std::bad_alloc

	ASSERT_NO_THROW(result = available(TE.O_2));
	EXPECT_EQ(result.size(), size_t{2});
	EXPECT_EQ(result[0], Value{1});
	EXPECT_EQ(result[1], Value{3});
	ASSERT_NO_THROW(result = available(TE.D_1));
	EXPECT_EQ(result.size(), size_t{4});
	// empty
	ASSERT_NO_THROW(result = available(TE.E_1));
	EXPECT_EQ(result.size(), size_t{0});
	// answer set
	ASSERT_NO_THROW(result = available(TE.A_1));
	EXPECT_EQ(result.size(), size_t{0});
}

TEST(Options, getAnswer)
{
	static_assert(noexcept(get_answer(TE.O_2)));
	EXPECT_EQ(get_answer(TE.A_1), Value{1});
	EXPECT_EQ(get_answer(TE.A_2), Value{2});
	EXPECT_EQ(get_answer(TE.O_2), Value{0});
	EXPECT_EQ(get_answer(TE.E_1), Value{0});
}

TEST(Options, toValue)
{
	// specialization for to_Value(..)
	using ::Sudoku::to_Value;

	const Options<9> all{};
	const Options<9> empty{std::bitset<9>{0b000000000}};
	const Options<9> A_5{std::bitset<9>{0b000010000}};

	static_assert(noexcept(to_Value<3>(all)));
	// return type
	static_assert(std::is_same_v<Value, decltype(to_Value<3>(all))>);

	EXPECT_EQ(to_Value<3>(all), Value{0});
	EXPECT_EQ(to_Value<3>(empty), Value{0});
	EXPECT_EQ(to_Value<3>(A_5), Value{5});
	EXPECT_EQ(to_Value<2>(TE.A_1), Value{1});
	EXPECT_EQ(to_Value<2>(TE.A_2), Value{2});
	EXPECT_EQ(to_Value<2>(TE.O_2), Value{0});
	EXPECT_EQ(to_Value<2>(TE.E_1), Value{0});
}

TEST(Options, mfChangeAll)
{
	// Set-up
	Options<4> TMP{};
	ASSERT_EQ(TMP.size(), size_t{4}) << "Invalid object";
	ASSERT_TRUE(TMP.all()) << "All options should be available";
	EXPECT_EQ(TMP.count(), 4U);
	ASSERT_EQ(TMP, TE.D_1) << "Default set D_1 is not complete";

	static_assert(noexcept(TMP.clear()));
	TMP = TE.D_1; // reset test data
	ASSERT_EQ(TMP, TE.D_1) << "Reset test data failed";
	TMP.clear();
	EXPECT_TRUE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 0U);
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.clear().is_empty());
	static_assert(noexcept(TMP.reset()));
	ASSERT_TRUE(TE.E_1.is_empty()) << "E_1 should be empty";
	TMP = TE.E_1;
	ASSERT_EQ(TMP, TE.E_1) << "Reset test data failed";
	TMP.reset();
	EXPECT_FALSE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 4U);
	EXPECT_TRUE(is_option(TMP, Value{2}));
	EXPECT_TRUE(TMP.all());
	TMP = TE.D_1;
	EXPECT_TRUE(TMP.reset().all());
	static_assert(noexcept(TMP.flip()));
	TMP = TE.D_1;
	ASSERT_TRUE(TMP.all()) << "Reset test data failed";
	TMP.flip();
	EXPECT_TRUE(TMP.is_empty());
	EXPECT_EQ(TMP.count(), 0U);
	TMP.flip();
	EXPECT_TRUE(TMP.all());
	EXPECT_EQ(TMP.count(), 4U);
}
TEST(Options, mfRemoveOption)
{
	Options<4> TMP{};
	ASSERT_TRUE(TMP.all());
	// return type
	static_assert(
		std::is_same_v<Options<4>&, decltype(TMP.remove_option(Value{3}))>);

	EXPECT_EQ(TMP.remove_option(Value{1}).DebugString(), "1110");
	EXPECT_EQ(TMP.remove_option(Value{3}).DebugString(), "1010");

	static_assert(not noexcept(TMP.remove_option(Value{3})));

	// already removed: nothing happens
	TMP.remove_option(Value{1});

	ASSERT_TRUE(TMP.reset().all()) << "Reset test data failed";
	EXPECT_EQ(TMP.remove_option(Value{3}).DebugString(), "1011");
}
#ifndef NDEBUG
TEST(OptionsDeathTest, mfRemoveOption)
{
	Options<4> TMP{};
	// check to prevent removing answer
	TMP.set(Value{3});
	EXPECT_DEBUG_DEATH(TMP.remove_option(Value{3}), "not Sudoku::is_answer");
	EXPECT_EQ(TMP.DebugString(), "0100");

	// Invalid input
	EXPECT_DEBUG_DEATH(TMP.remove_option(Value{0}), "val > Value");
	EXPECT_DEBUG_DEATH(TMP.remove_option(Value{5}), "val <= Value");
	EXPECT_EQ(TMP.DebugString(), "0100");
}
#endif
TEST(OptionsTest, mfAdd)
{
	Options<4> Opt{std::bitset<4>{0b0000}};
	ASSERT_TRUE(Opt.is_empty());
	// return type
	static_assert(std::is_same_v<Options<4>&, decltype(Opt.add(Value{3}))>);

	EXPECT_EQ(Opt.add(Value{1}).DebugString(), "0001");
	EXPECT_EQ(Opt.add(Value{4}).DebugString(), "1001");
	EXPECT_EQ(Opt.DebugString(), "1001");
}
#ifndef NDEBUG
TEST(OptionsDeathTest, mfAdd)
{
	Options<4> Opt{std::bitset<4>{0b1001}};
	static_assert(noexcept(Opt.add(Value{4})));

	EXPECT_DEBUG_DEATH(Opt.add(Value{0}), "val > Value");
	EXPECT_DEBUG_DEATH(Opt.add(Value{5}), "val <= Value");

	EXPECT_EQ(Opt.DebugString(), "1001");
}
#endif
TEST(Options, mfSet)
{
	Options<4> TMP{};
	// return type
	static_assert(std::is_same_v<Options<4>&, decltype(TMP.set(Value{3}))>);

	EXPECT_EQ(TMP.set(Value{1}).DebugString(), "0001");
	EXPECT_TRUE(is_answer(TMP.set(Value{2}), Value{2}));
	EXPECT_EQ(TMP.set(Value{4}).DebugString(), "1000");

	static_assert(not noexcept(TMP.set(Value{4})));
}
#ifndef NDEBUG
TEST(OptionsDeathTest, mfSet)
{
	Options<4> TMP{std::bitset<4>{0b1000}};
	ASSERT_EQ(TMP.DebugString(), "1000");
	EXPECT_DEBUG_DEATH(TMP.set(Value{0}), "val > Value");
	EXPECT_DEBUG_DEATH(TMP.set(Value{5}), "val <= Value");
	EXPECT_EQ(TMP.DebugString(), "1000");
}
#endif
TEST(Options, mfBooleanComparison)
{
	// operator==(Value) const
	constexpr OptionValue<4> val{1U};
	static_assert(noexcept(TE.A_1 == val));
	static_assert(noexcept(val == TE.A_1));
	static_assert(not noexcept(TE.A_1 == Value{1}));
	static_assert(not noexcept(Value{1} == TE.A_1));

	EXPECT_EQ(TE.A_1, val);
	EXPECT_EQ(val, TE.A_1);
	EXPECT_EQ(TE.A_1, Value{1});
	EXPECT_EQ(Value{1}, TE.A_1);
	EXPECT_EQ(TE.A_2, Value{2});

	// operator!=(Value) const
	static_assert(noexcept(TE.A_1 != val));
	static_assert(noexcept(val != TE.A_1));
	static_assert(not noexcept(TE.A_1 != Value{1}));
	static_assert(not noexcept(Value{1} != TE.A_1));

	EXPECT_NE(TE.A_1, Value{2});
	EXPECT_NE(Value{2}, TE.A_1);
	EXPECT_NE(TE.A_2, Value{1});

	// bool operator==(Options<E>&) const
	static_assert(noexcept(TE.A_2 == TE.O_2));
	const Options<4> TMP{std::bitset<4>{0b0010}};
	EXPECT_EQ(TE.A_2, TMP);
	EXPECT_FALSE(TE.A_2 == TE.E_1);

	// bool operator!=(Options<E>&) const
	static_assert(noexcept(TE.A_2 != TE.O_2));
	EXPECT_NE(TE.A_2, TE.E_1);
	EXPECT_FALSE(TE.A_2 != TE.A_2);

#if defined(__ICL) && __ICL <= 1900
	static_assert(not noexcept(TE.O_1 < TE.O_4));
#else
	static_assert(noexcept(TE.A_2 < TE.O_4));
#endif // __ICL
	EXPECT_FALSE(TE.D_1 < TE.O_4) << "both full";
	EXPECT_FALSE(TE.E_1 < Options<4>(std::bitset<4>{0b0000})) << "both empty";
	EXPECT_LT(TE.E_1, TE.D_1) << "empty vs default";
	EXPECT_FALSE(TE.D_1 < TE.E_1) << "empty vs default";
	EXPECT_LT(TE.E_1, TE.A_1) << "empty vs answer";
	EXPECT_FALSE(TE.A_1 < TE.E_1) << "empty vs answer";
	EXPECT_LT(TE.E_1, TE.A_2) << "empty vs options";
	EXPECT_FALSE(TE.A_2 < TE.E_1) << "empty vs options";
	EXPECT_LT(TE.E_1, TE.A_2) << "empty vs answered options";
	EXPECT_FALSE(TE.A_2 < TE.E_1) << "empty vs answered options";
	EXPECT_LT(TE.A_2, TE.D_1) << "answer vs default";
	EXPECT_FALSE(TE.D_1 < TE.A_2) << "answer vs default";
	EXPECT_LT(TE.A_1, TE.A_2) << "answer vs answer";
	EXPECT_FALSE(TE.A_2 < TE.A_1) << "answer vs answer";
	EXPECT_FALSE(TE.A_2 < TE.A_1) << "both answered";
	EXPECT_LT(TE.O_2, Options<4>(std::bitset<4>{0b1001})) << "compare values";
	EXPECT_LT(
		Options<6>{std::bitset<6>{0b011111}},
		Options<6>(std::bitset<6>{0b101111}));
	EXPECT_FALSE(
		Options<6>{std::bitset<6>{0b101111}} <
		Options<6>(std::bitset<6>{0b011111}));
	EXPECT_LT(TE.A_2, TE.O_2) << "answer vs unanswered";
	EXPECT_FALSE(TE.O_2 < TE.A_2) << "answer vs unanswered";
	EXPECT_LT(TE.A_2, TE.O_2) << "answer vs unanswered";
	EXPECT_FALSE(TE.O_2 < TE.A_2) << "answer vs unanswered";
}
TEST(OptionsDeathTest, mfBooleanComparison)
{
	[[maybe_unused]] bool U{};

	// operator==(Value) const
	EXPECT_DEBUG_DEATH(
		{ U = operator==(TE.A_1, Value{15}); },
		"val <= Value"); // constructor
	EXPECT_DEBUG_DEATH({ U = operator==(Value{15}, TE.A_1); }, "val <= Value");

	// operator!=(Value) const
	EXPECT_DEBUG_DEATH({ U = operator!=(TE.A_1, Value{15}); }, "val <= Value");
	EXPECT_DEBUG_DEATH({ U = operator!=(Value{15}, TE.A_1); }, "val <= Value");
	U = false; // suppress warning: assigned only once
}
TEST(Options, mfConstOperators)
{
	// Options operator&(Options&) const		shared options
	static_assert(noexcept(TE.A_2 & TE.O_2));
	EXPECT_EQ((TE.D_1 & TE.O_3), TE.O_3);
	EXPECT_EQ((TE.A_2 & TE.A_2), TE.A_2);
	EXPECT_EQ((TE.O_2 & TE.E_1), TE.E_1);
	EXPECT_EQ((TE.O_3 & TE.A_2), TE.A_2);
	EXPECT_EQ((TE.E_1 & TE.A_2), TE.E_1);

	// constexpr bool operator[](int) const
	static_assert(noexcept(TE.A_2[Value{2}]));
	constexpr OptionValue<4> value{2U};
	static_assert(noexcept(TE.A_2[value]));

	constexpr Options<4> x{Value{1}};
	static_assert(x[Value{1}]);
	static_assert(not x[Value{2}]);
	constexpr Options<9> y1{Value{1}};
	static_assert(y1[Value{1}]);
	constexpr Options<9> y9{Value{1}};
	static_assert(y9[Value{1}]);

	EXPECT_TRUE(TE.A_2[Value{2}]);
	EXPECT_TRUE(TE.A_2[Value{1}] == false);
}
TEST(Options, Operators)
{
	// _test-data
	Options<4> TMP{}; // per test, reset this option

	///// non-const operators /////
	static_assert(noexcept(TMP.operator[](Value{2}) = true));
	constexpr OptionValue<4> value{2U};
	static_assert(noexcept(TMP.operator[](value) = true));

	EXPECT_TRUE(TMP[Value{1}] = true);
	EXPECT_TRUE(TMP[Value{1}] == true);
	// std::bitset<N>::reference::flip()
	static_assert(
		std::is_same_v<std::bitset<4>::reference, decltype(TMP[Value{1}])>);
	// Fails with libc++
	// static_assert(std::is_same_v<
	//			  std::bitset<4>::reference&,
	//			  decltype(TMP[Value{0}].flip())>);
	TMP[Value{1}].flip();
	EXPECT_TRUE(TMP[Value{1}] == false);
	// Options& XOR(Options&)			XOR
	static_assert(noexcept(TMP.XOR(TE.A_2)));
	TMP = TE.E_1;
	EXPECT_TRUE(TMP.XOR(TE.A_2) == TE.A_2);
	EXPECT_TRUE((TMP.XOR(TE.A_2)) == TE.E_1);
	// Options& operator+=(Options&)			combine options
	static_assert(noexcept(TMP += TE.O_4));
	TMP = TE.E_1;
	TMP += TE.O_4;
	EXPECT_TRUE(TE.D_1 == TE.O_4);
	// others tested by using operator+()
	// Options operator+(Options&) const		combine options
	static_assert(noexcept(TE.D_1 + TE.O_4));
	EXPECT_TRUE(TE.D_1 + TE.O_4 == TE.D_1);
	EXPECT_TRUE(TE.E_1 + TE.D_1 == TE.D_1);
	EXPECT_TRUE(TE.A_2 + TE.E_1 == TE.A_2);
	EXPECT_TRUE(TE.E_1 + TE.A_2 == TE.A_2);
}

TEST(Options, ConstructorTesting)
{
	Options<4> TMP{};
	// copy-assign
	static_assert(noexcept(TMP.operator=(TE.O_4)));
	static_assert(noexcept(TMP.operator=(Value{1})));
	constexpr OptionValue<4> value{1U};
	static_assert(noexcept(TMP.operator=(value)));

	TMP = TE.A_2;
	EXPECT_TRUE(is_answer(TMP, Value{2}));
	EXPECT_TRUE(TMP == TE.A_2);
	const auto TMP2 = Options<4>{Value{3}};
	EXPECT_TRUE(is_answer(TMP2, Value{3}));

	// move-assign
	static_assert(noexcept(TMP.operator=(std::bitset<4>())));
	const Options<4> TMP3 = Options<4>(std::bitset<4>{}); // "00000"
	EXPECT_TRUE(TMP3 == TE.E_1);
}

TEST(Options, External)
{
	const Options<4> O_3{std::bitset<4>{0b0010}}; // single option 2
	const Options<4> E_2{std::bitset<4>{0b0000}}; // empty
	const Options<4> E_3{E_2};
	[[maybe_unused]] const Options<4> A_1{Value{1}}; // answer 1
	const Options<4> A_2{std::bitset<4>{0b0010}};    // answer 2
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

TEST(Options, operatorMinus)
{
	const Options<9> all{};
	const Options<9> empty{std::bitset<9>{0b000000000}};
	const Options<9> E_1{empty};
	const Options<9> O_0{std::bitset<9>{0b011101101}};
	const Options<9> O_1{std::bitset<9>{0b011101101}};
	const Options<9> O_2{std::bitset<9>{0b101010111}};
	const Options<9> A_0{std::bitset<9>{0b010101000}};
	const Options<9> A_1{A_0};
	const Options<9> A_2{std::bitset<9>{0b100010010}};

	auto B_1 = O_1;
	static_assert(noexcept(B_1 -= O_2));
	static_assert(noexcept(O_1 - O_2));
	// return type
	static_assert(std::is_same_v<Options<9>, decltype(B_1 - O_2)>);
	static_assert(std::is_same_v<Options<9>, decltype(O_0 - O_1)>);

	auto B_0 = O_0;
	EXPECT_EQ(B_0 -= O_1, E_1);
	EXPECT_EQ(O_0 - O_1, E_1);
	B_0 = O_0;
	B_0 -= O_2;
	EXPECT_EQ(B_0, A_0);
	EXPECT_EQ(O_0 - O_2, A_0);
	B_1 = O_1;
	B_1 -= O_2;
	EXPECT_EQ(B_1, A_1);
	EXPECT_EQ(O_1 - O_2, A_1);
	auto B_2 = O_2;
	B_2 -= O_1;
	EXPECT_EQ(B_2, A_2);
	EXPECT_EQ(O_2 - O_1, A_2);
	B_1 = O_1;
	B_1 -= O_1;
	EXPECT_EQ(B_1, empty);
	EXPECT_EQ(O_1 - O_1, empty);
	B_1 = O_1;
	B_1 -= empty;
	EXPECT_EQ(B_1, O_1);
	EXPECT_EQ(O_1 - empty, O_1);
	auto B_3 = empty;
	B_3 -= O_2;
	EXPECT_EQ(B_3, empty);
	EXPECT_EQ(empty - O_2, empty);
}

} // namespace SudokuTests::OptionsTest
