// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/OptionValueTest.cpp                            ----====//
//
//	Unit tests for the template class Sudoku::OptionValue
//====--------------------------------------------------------------------====//
#include <Sudoku/Concepts.h> // Class under test

#include <type_traits>

#include <cstdint>

//#include <gtest/gtest.h>

namespace SudokuTests::ConceptsTests
{

namespace CharacterTests
{
	using Sudoku::Character;
	struct Test
	{
		char a;
		template<typename T>
		requires Character<T> explicit constexpr Test(T val) : a(val)
		{
		}
	};

	static_assert(not std::is_constructible_v<Test, void>);
	static_assert(not std::is_constructible_v<Test, bool>);
	static_assert(std::is_constructible_v<Test, std::int8_t>); // C-standard
	static_assert(not std::is_constructible_v<Test, std::int16_t>);
	static_assert(not std::is_constructible_v<Test, int>);
	static_assert(not std::is_constructible_v<Test, std::int64_t>);
	static_assert(std::is_constructible_v<Test, std::uint8_t>); // C-standard
	static_assert(not std::is_constructible_v<Test, std::uint16_t>);
	static_assert(not std::is_constructible_v<Test, unsigned int>);
	static_assert(not std::is_constructible_v<Test, std::uint32_t>);
	static_assert(not std::is_constructible_v<Test, std::uint64_t>);
	static_assert(std::is_constructible_v<Test, char>);
	static_assert(std::is_constructible_v<Test, signed char>);
	static_assert(std::is_constructible_v<Test, unsigned char>);
	static_assert(std::is_constructible_v<Test, wchar_t>);
	static_assert(std::is_constructible_v<Test, char8_t>);
	static_assert(std::is_constructible_v<Test, char16_t>);
	static_assert(std::is_constructible_v<Test, char32_t>);
	static_assert(not std::is_constructible_v<Test, float>);
	static_assert(not std::is_constructible_v<Test, double>);
	static_assert(not std::is_constructible_v<Test, long double>);
} // namespace CharacterTests
namespace NumberTests
{
	using Sudoku::Number;

	struct Test
	{
		int a;
		template<typename T>
		requires Number<T> explicit constexpr Test(T val) : a(val)
		{
		}
	};

	static_assert(not std::is_constructible_v<Test, void>);
	static_assert(not std::is_constructible_v<Test, bool>);
	static_assert(not std::is_constructible_v<Test, std::int8_t>); // C-stand.
	static_assert(std::is_constructible_v<Test, std::int16_t>);
	static_assert(std::is_constructible_v<Test, int>);
	static_assert(std::is_constructible_v<Test, std::int64_t>);
	static_assert(not std::is_constructible_v<Test, std::uint8_t>); // C-stand.
	static_assert(std::is_constructible_v<Test, std::uint16_t>);
	static_assert(std::is_constructible_v<Test, unsigned int>);
	static_assert(std::is_constructible_v<Test, std::uint32_t>);
	static_assert(std::is_constructible_v<Test, std::uint64_t>);
	static_assert(not std::is_constructible_v<Test, char>);
	static_assert(not std::is_constructible_v<Test, signed char>); // int8_t
	static_assert(not std::is_constructible_v<Test, unsigned char>); // uint8_t
	static_assert(not std::is_constructible_v<Test, wchar_t>);
	static_assert(not std::is_constructible_v<Test, char8_t>);
	static_assert(not std::is_constructible_v<Test, char16_t>);
	static_assert(not std::is_constructible_v<Test, char32_t>);
	static_assert(not std::is_constructible_v<Test, float>);
	static_assert(not std::is_constructible_v<Test, double>);
	static_assert(not std::is_constructible_v<Test, long double>);
} // namespace NumberTests
namespace SignedNumberTests
{
	using Sudoku::SignedNumber;
	struct Test
	{
		int a;
		template<typename T>
		requires SignedNumber<T> explicit constexpr Test(T val) : a(val)
		{
		}
	};

	static_assert(not std::is_constructible_v<Test, void>);
	static_assert(not std::is_constructible_v<Test, bool>);
	static_assert(not std::is_constructible_v<Test, std::int8_t>);
	static_assert(std::is_constructible_v<Test, std::int16_t>);
	static_assert(std::is_constructible_v<Test, int>);
	static_assert(std::is_constructible_v<Test, std::int64_t>);
	static_assert(not std::is_constructible_v<Test, std::uint8_t>);
	static_assert(not std::is_constructible_v<Test, std::uint16_t>);
	static_assert(not std::is_constructible_v<Test, unsigned int>);
	static_assert(not std::is_constructible_v<Test, std::uint32_t>);
	static_assert(not std::is_constructible_v<Test, std::uint64_t>);
	static_assert(not std::is_constructible_v<Test, char>);
	static_assert(not std::is_constructible_v<Test, signed char>); // int8_t
	static_assert(not std::is_constructible_v<Test, unsigned char>); // uint8_t
	static_assert(not std::is_constructible_v<Test, wchar_t>);
	static_assert(not std::is_constructible_v<Test, char8_t>);
	static_assert(not std::is_constructible_v<Test, char16_t>);
	static_assert(not std::is_constructible_v<Test, char32_t>);
	static_assert(not std::is_constructible_v<Test, float>);
	static_assert(not std::is_constructible_v<Test, double>);
	static_assert(not std::is_constructible_v<Test, long double>);
} // namespace SignedNumberTests
namespace UnsignedNumberTests
{
	using Sudoku::UnsignedNumber;
	struct Test
	{
		int a;
		template<typename T>
		requires UnsignedNumber<T> explicit constexpr Test(T val) : a(val)
		{
		}
	};

	static_assert(not std::is_constructible_v<Test, void>);
	static_assert(not std::is_constructible_v<Test, bool>);
	static_assert(not std::is_constructible_v<Test, std::int8_t>);
	static_assert(not std::is_constructible_v<Test, std::int16_t>);
	static_assert(not std::is_constructible_v<Test, int>);
	static_assert(not std::is_constructible_v<Test, std::int64_t>);
	static_assert(not std::is_constructible_v<Test, std::uint8_t>);
	static_assert(std::is_constructible_v<Test, std::uint16_t>);
	static_assert(std::is_constructible_v<Test, unsigned int>);
	static_assert(std::is_constructible_v<Test, std::uint32_t>);
	static_assert(std::is_constructible_v<Test, std::uint64_t>);
	static_assert(not std::is_constructible_v<Test, char>);
	static_assert(not std::is_constructible_v<Test, signed char>);
	static_assert(not std::is_constructible_v<Test, unsigned char>);
	static_assert(not std::is_constructible_v<Test, wchar_t>);
	static_assert(not std::is_constructible_v<Test, char8_t>);
	static_assert(not std::is_constructible_v<Test, char16_t>);
	static_assert(not std::is_constructible_v<Test, char32_t>);
	static_assert(not std::is_constructible_v<Test, float>);
	static_assert(not std::is_constructible_v<Test, double>);
	static_assert(not std::is_constructible_v<Test, long double>);
} // namespace UnsignedNumberTests
} // namespace SudokuTests::ConceptsTests
