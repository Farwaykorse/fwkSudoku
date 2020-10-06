//====---- Sudoku/Concepts.h                                          ----====//
// C++ 20 Concepts
//
// Note: int8_t & uint8_t, can be C-standard typedef for char and wchar.
//
//====--------------------------------------------------------------------====//
#pragma once

#include <type_traits>

namespace Sudoku
{
template<typename T>
concept Character = std::is_integral_v<T> &&
					(std::is_same_v<T, char> || std::is_same_v<T, wchar_t> ||
					 std::is_same_v<T, char8_t> ||
					 std::is_same_v<T, char16_t> ||
					 std::is_same_v<T, char32_t> ||
					 std::is_same_v<T, unsigned char> ||
					 std::is_same_v<T, signed char>);
template<typename T>
concept Number = std::is_integral_v<T> && not std::is_same_v<T, bool> &&
				 (not Character<T> || std::is_same_v<T, std::int8_t> ||
				  std::is_same_v<T, std::uint8_t>);
template<typename T>
concept SignedNumber = Number<T>&& std::is_signed_v<T>;
template<typename T>
concept UnsignedNumber = Number<T>&& std::is_unsigned_v<T>;


} // namespace Sudoku
