//====---- Sudoku/traits.h                                            ----====//
//
// Type traits.
//====--------------------------------------------------------------------====//
#pragma once

#include <type_traits>

namespace Sudoku
{
inline namespace traits
{
	template<typename T>
	struct is_int : std::integral_constant<bool, std::is_integral_v<T>> {};
	template<>
	struct is_int<bool> : std::false_type {};
	template<typename T>
	inline static constexpr bool is_int_v = is_int<T>::value;
} // namespace traits
} // namespace Sudoku
