//===--	Sudoku/Value.h													--===//
//
//	Value type
//===---------------------------------------------------------------------===//
#pragma once

#include "Size.h"

#include <vector>
#include <algorithm>


namespace Sudoku
{
using value_t = unsigned int;
// allows use as a bitmask


//===---------------------------------------------------------------------===//
template<int N>
constexpr bool is_valid(const value_t);
template<int N>
constexpr bool is_valid(const std::vector<value_t>&);

//===---------------------------------------------------------------------===//

// Test input value
template<int N>
inline constexpr bool is_valid(const value_t value)
{
	return value > 0 && value <= static_cast<value_t>(elem_size<N>);
}

// Test input values
template<int N>
inline constexpr bool is_valid(const std::vector<value_t>& values)
{
	return (
		!values.empty() &&
		std::all_of(values.cbegin(), values.cend(), [](value_t i) {
			return is_valid<N>(i);
		}));
}


} // namespace Sudoku
