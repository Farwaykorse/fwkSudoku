//===--- SudokuTests/Value.cpp                                          ---===//
//
//	Unit tests for the template class Sudoku::Value
//===----------------------------------------------------------------------===//
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
//===----------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Value.h>
// Helpers

// aditional
#include <vector>
#include <type_traits>


namespace SudokuTests::ValueTest
{
using namespace ::Sudoku;

namespace compiletime
{
	// Type properties
	using typeT = Value;

	static_assert(noexcept(Value{1}));

	static_assert(std::is_fundamental_v<unsigned int>);
	static_assert(not std::is_fundamental_v<typeT>);
	static_assert(std::is_compound_v<typeT>);
	static_assert(not std::is_scalar_v<typeT>);
	static_assert(not std::is_arithmetic_v<typeT>);
	static_assert(not std::is_signed_v<typeT>);
	static_assert(not std::is_unsigned_v<typeT>);

	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(std::is_trivially_copyable_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
	// can be converted with reinterpret_cast
	static_assert(not std::is_pod_v<typeT>);
	static_assert(not std::is_empty_v<typeT>); // nothing virtual
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);

	// default constructor: typeT()
	static_assert(std::is_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>);
	static_assert(std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(std::is_trivially_copy_constructible_v<typeT>); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(std::is_trivially_move_constructible_v<typeT>);

	// copy assingment
	static_assert(std::is_copy_assignable_v<typeT>);
	static_assert(std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(std::is_trivially_copy_assignable_v<typeT>);

	static_assert(std::is_move_assignable_v<typeT>);
	static_assert(std::is_nothrow_move_assignable_v<typeT>);
	static_assert(std::is_trivially_move_assignable_v<typeT>);

	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(std::is_trivially_destructible_v<typeT>);
	static_assert(not std::has_virtual_destructor_v<typeT>);

	static_assert(std::is_swappable_v<typeT>);         // C++17
	static_assert(std::is_nothrow_swappable_v<typeT>); // C++17

	// other types
	static_assert(std::is_constructible_v<typeT, size_t>);
	// all by implicit conversion to size_t
	static_assert(std::is_constructible_v<typeT, int>);
	static_assert(std::is_constructible_v<typeT, unsigned int>);
	static_assert(std::is_constructible_v<typeT, unsigned long int>);
	static_assert(std::is_constructible_v<typeT, unsigned long long int>);
	static_assert(std::is_constructible_v<typeT, bool>); // ... via size_t
	// explicit construction from Value:
	static_assert(std::is_constructible_v<size_t, typeT>);
	static_assert(std::is_nothrow_constructible_v<size_t, typeT>);
	static_assert(std::is_constructible_v<bool, typeT>);
	static_assert(std::is_nothrow_constructible_v<bool, typeT>);
	static_assert(not std::is_constructible_v<int, typeT>);
	// all others fail, unless same as size_t

	static_assert(not std::is_assignable_v<typeT, int>);
	static_assert(not std::is_assignable_v<int, typeT>);
	static_assert(not std::is_assignable_v<typeT, unsigned int>);
	static_assert(not std::is_assignable_v<unsigned int, typeT>);
	static_assert(not std::is_assignable_v<typeT, size_t>);
	static_assert(not std::is_assignable_v<typeT, bool>);
	static_assert(not std::is_assignable_v<bool, typeT>);

	static_assert(not std::is_swappable_with_v<typeT, int>);          // C++17
	static_assert(not std::is_swappable_with_v<typeT, unsigned int>); // C++17
	static_assert(not std::is_swappable_with_v<typeT, size_t>);       // C++17
	static_assert(not std::is_nothrow_swappable_with_v<typeT, int>);  // C++17
} // namespace compiletime

TEST(Value, comparisons)
{
	static_assert(noexcept(Value{5} == Value{2}));
	static_assert(noexcept(Value{5} != Value{2}));
	static_assert(noexcept(Value{5} < Value{2}));
	static_assert(noexcept(Value{5} > Value{2}));
	static_assert(noexcept(Value{5} <= Value{2}));
	static_assert(noexcept(Value{5} >= Value{2}));

	static_assert(Value{7} == Value{7});
	static_assert(not(Value{2} == Value{7}));
	static_assert(Value{1} != Value{0});
	static_assert(not(Value{0} != Value{0}));
	static_assert(Value{8} > Value{2});
	static_assert(not(Value{2} > Value{2}));
	static_assert(not(Value{1} > Value{2}));
	static_assert(Value{4} >= Value{1});
	static_assert(Value{4} >= Value{4});
	static_assert(not(Value{4} >= Value{6}));
	static_assert(Value{3} <= Value{9});
	static_assert(Value{9} <= Value{9});
	static_assert(not(Value{10} <= Value{9}));
	static_assert(Value{5} < Value{6});
	static_assert(not(Value{5} < Value{5}));
	static_assert(not(Value{6} < Value{5}));

	EXPECT_TRUE(Value{1} == Value{1});
	EXPECT_FALSE(Value{1} == Value{3});
	EXPECT_TRUE(Value{1} != Value{3});
	EXPECT_FALSE(Value{3} != Value{3});
	EXPECT_TRUE(Value{5} < Value{7});
	EXPECT_FALSE(Value{4} < Value{4});
	EXPECT_FALSE(Value{4} < Value{3});
	EXPECT_TRUE(Value{1} <= Value{3});
	EXPECT_TRUE(Value{5} <= Value{5});
	EXPECT_FALSE(Value{7} <= Value{5});
	EXPECT_TRUE(Value{5} >= Value{5});
	EXPECT_TRUE(Value{6} >= Value{5});
	EXPECT_FALSE(Value{6} >= Value{8});
	EXPECT_TRUE(Value{6} > Value{5});
	EXPECT_FALSE(Value{1} > Value{1});
	EXPECT_FALSE(Value{2} > Value{5});
}

TEST(Value, is_valid)
{
	EXPECT_FALSE(is_valid<2>(Value{0}));
	EXPECT_TRUE(is_valid<2>(Value{1}));
	EXPECT_TRUE(is_valid<2>(Value(4)));
	EXPECT_FALSE(is_valid<2>(Value{5}));
	EXPECT_TRUE(is_valid<3>(Value{5}));
	EXPECT_FALSE(is_valid<3>(Value{16}));

	static_assert(noexcept(is_valid<3>(Value{0})));
	static_assert(noexcept(is_valid<3>(Value{1})));
	static_assert(noexcept(is_valid<3>(Value{9})));
	static_assert(noexcept(is_valid<3>(Value{10})));

	// is constexpr
	static_assert(!is_valid<2>(Value{0}));
	static_assert(is_valid<2>(Value{1}));
	static_assert(is_valid<2>(Value{4}));
	static_assert(!is_valid<2>(Value{5}));
	static_assert(!is_valid<3>(Value{0}));
	static_assert(is_valid<3>(Value{1}));
	static_assert(is_valid<3>(Value{9}));
	static_assert(!is_valid<3>(Value{10}));

	static_assert(noexcept(is_valid_option<3>(Value{0})));
	static_assert(noexcept(is_valid_option<3>(Value{1})));
	static_assert(noexcept(is_valid_option<3>(Value{9})));
	static_assert(noexcept(is_valid_option<3>(Value{10})));

	static_assert(!is_valid_option<4>(Value{0}));
	static_assert(is_valid_option<4>(Value{1}));
	static_assert(is_valid_option<4>(Value{4}));
	static_assert(!is_valid_option<4>(Value{5}));
	static_assert(!is_valid_option<9>(Value{0}));
	static_assert(is_valid_option<9>(Value{1}));
	static_assert(is_valid_option<9>(Value{9}));
	static_assert(!is_valid_option<9>(Value{10}));
}

TEST(Value, operator_bool)
{
	static_assert(noexcept(Value{0}));
	static_assert(noexcept(Value{1}));
	static_assert(noexcept(Value{999}));
	static_assert(not Value{0}); // seems to result in an explicit conversion
	static_assert(Value{1});
	static_assert(Value{999});

	EXPECT_EQ(bool{Value{0}}, false); // requires explicit conversion
	EXPECT_EQ(bool{Value{1}}, true);
}

TEST(Value, is_valid_vector)
{
	// vector input
	using list = std::vector<Value>;
	const std::vector<Value> cList{
		Value{1}, Value{2}, Value{3}, Value{4}, Value{3}, Value{1}};
	std::vector<Value> List{cList};
	EXPECT_FALSE(is_valid<2>(list{})) << "can't be empty";
	EXPECT_TRUE(is_valid<2>(List));
	EXPECT_TRUE(is_valid<2>(list{Value{1}}));
	EXPECT_TRUE(is_valid<2>(list{Value{4}}));
	EXPECT_FALSE(is_valid<2>(list{Value{0}}));
	EXPECT_FALSE(is_valid<2>(list{Value{5}}));
	List.at(2) = Value{0}; // 1, 2, 0, 4, 3, 1
	EXPECT_FALSE(is_valid<2>(List));
	// EXPECT_FALSE(is_valid<2>(std::vector<Value>{1, -2, 3, 4, 3, 1}));
	List.at(2) = Value{3}; // 1, 2, 3, 4, 3, 1
	List.at(1) = Value{5}; // 1, 5, 3, 4, 3, 1
	EXPECT_FALSE(is_valid<2>(List));

	static_assert(noexcept(is_valid<2>(cList)));
	static_assert(noexcept(is_valid<2>(List)));
}

TEST(Value, to_Value)
{
	// Input: Value
	static_assert(to_Value<3>(Value{0}) == Value{0});
	static_assert(to_Value<3>(Value{1}) == Value{1});
	static_assert(to_Value<3>(Value{9}) == Value{9});
	static_assert(noexcept(Value{0}));
	// TODO: fails with Clang, works for MSVC and GCC 7.1+
	// static_assert(noexcept(to_Value<3>(Value{0}))); // fails with Clang
	// static_assert(noexcept(to_Value<3>(Value{9}))); // fails with Clang
	static_assert(noexcept(Value{10}));
	static_assert(not(noexcept(to_Value<3>(Value{10}))));
	static_assert(not(noexcept(to_Value<3>(Value{21}))));

	EXPECT_NO_THROW(to_Value<3>(Value{0}));
	EXPECT_NO_THROW(to_Value<3>(Value{9}));
	EXPECT_THROW(to_Value<3>(Value{10}), std::domain_error);

	// Input: int
	static_assert(to_Value<3>(0) == Value{0});
	static_assert(to_Value<3>(1) == Value{1});
	static_assert(to_Value<3>(9) == Value{9});
	// static_assert(noexcept(to_Value<3>(2))); // fails with Clang
	static_assert(not(noexcept(to_Value<3>(-2))));
	static_assert(not(noexcept(to_Value<3>(10))));
	static_assert(not(noexcept(to_Value<3>(21))));

	EXPECT_NO_THROW(to_Value<3>(0));
	EXPECT_NO_THROW(to_Value<3>(9));
	EXPECT_THROW(to_Value<3>(-1), std::domain_error);
	EXPECT_THROW(to_Value<3>(10), std::domain_error);

	// Input: size_t
	static_assert(to_Value<3>(size_t{0}) == Value{0});
	static_assert(to_Value<3>(size_t{1}) == Value{1});
	static_assert(to_Value<3>(size_t{9}) == Value{9});
	static_assert(noexcept(size_t{0}));
	// static_assert(noexcept(to_Value<3>(size_t{0}))); // fails with Clang
	// static_assert(noexcept(to_Value<3>(size_t{9}))); // fails with Clang
	static_assert(noexcept(size_t{10}));
	static_assert(not(noexcept(to_Value<3>(size_t{10}))));
	static_assert(not(noexcept(to_Value<3>(size_t{21}))));

	EXPECT_NO_THROW(to_Value<3>(size_t{0}));
	EXPECT_NO_THROW(to_Value<3>(size_t{9}));
	EXPECT_THROW(to_Value<3>(size_t{10}), std::domain_error);

	// Input: unsigned int
	static_assert(to_Value<3>(0u) == Value{0});
	static_assert(to_Value<3>(1u) == Value{1});
	static_assert(to_Value<3>(9u) == Value{9});
	// static_assert(noexcept(to_Value<3>(2u))); // fails with Clang
	static_assert(not(noexcept(to_Value<3>(10u))));
	static_assert(not(noexcept(to_Value<3>(21u))));

	// Input: long int
	static_assert(to_Value<3>(0L) == Value{0});
	static_assert(to_Value<3>(1L) == Value{1});
	static_assert(to_Value<3>(9L) == Value{9});
	// static_assert(noexcept(to_Value<3>(2L))); // fails with Clang
	static_assert(not(noexcept(to_Value<3>(-1L))));
	static_assert(not(noexcept(to_Value<3>(10L))));
	static_assert(not(noexcept(to_Value<3>(21L))));
}

} // namespace SudokuTests::ValueTest
