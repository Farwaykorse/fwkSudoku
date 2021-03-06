// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/traits.cpp                                     ----====//
//
// Unit tests for type traits defined in Sudoku/traits.h.
//====--------------------------------------------------------------------====//
#include <Sudoku/traits.h> // Class under test

#include <bitset>

#include <type_traits>

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <cwchar>


namespace SudokuTests::Traits
{
// inline namespace 'traits'
static_assert(
	std::is_same_v<::Sudoku::traits::is_int<int>, ::Sudoku::is_int<int>>);


using ::Sudoku::traits::is_int;
using ::Sudoku::traits::is_int_v;

// return type
static_assert(not std::is_same_v<bool, decltype(is_int<int>::value)>);
static_assert(std::is_same_v<
			  decltype(std::true_type::value),
			  decltype(is_int<int>::value)>);
static_assert(std::is_same_v<
			  decltype(std::false_type::value),
			  decltype(is_int<int>::value)>);
static_assert(
	not std::is_same_v<std::false_type, decltype(is_int<int>::value)>);
static_assert(not std::is_same_v<bool, decltype(is_int_v<int>)>);
static_assert(
	std::is_same_v<decltype(std::true_type::value), decltype(is_int_v<int>)>);


// use with primitive types
static_assert(is_int_v<int>);
static_assert(is_int_v<short int>);     // NOLINT
static_assert(is_int_v<long int>);      // NOLINT
static_assert(is_int_v<long long int>); // NOLINT
static_assert(is_int_v<char>);
static_assert(is_int_v<unsigned char>);
static_assert(is_int_v<unsigned int>);

static_assert(not is_int_v<bool>);
static_assert(is_int_v<char>);
static_assert(not is_int_v<float>);
static_assert(not is_int_v<double>);
static_assert(not is_int_v<long double>);

// cstdint
static_assert(is_int_v<std::int8_t>);
static_assert(is_int_v<std::int16_t>);
static_assert(is_int_v<std::int32_t>);
static_assert(is_int_v<std::uint8_t>);
static_assert(is_int_v<std::uint16_t>);
static_assert(is_int_v<std::uint32_t>);

// cstddef
static_assert(is_int_v<std::size_t>);
static_assert(is_int_v<std::ptrdiff_t>);
#if not(defined(__ICL) && __ICL <= 1900) &&                                    \
	not(defined(__APPLE__) && defined(__clang__) &&                            \
		(__clang_major__ < 10 ||                                               \
		 (__clang_major__ == 9 && __clang_minor__ < 1)))
static_assert(not is_int_v<std::byte>);
#endif // __ICL

// cwchar
static_assert(is_int_v<std::wint_t>);

// bitset
static_assert(not is_int_v<std::bitset<5>>);

} // namespace SudokuTests::Traits
