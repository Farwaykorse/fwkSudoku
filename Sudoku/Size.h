//===--	Sudoku/Size.h													--===//
//
// The size definition that rules them all.
//===---------------------------------------------------------------------===//
#pragma once

#include <limits>

namespace Sudoku
{
template<int N>
struct Size
{
	static constexpr int base = N;           // 3 for default 9*9 board
	static constexpr int elem = base * base; // 9 for default
	static constexpr int full = elem * elem; // 81 for default

	static_assert(
		base < elem && base < full && elem < full,
		"class Location: Board size out of bounds");
	static_assert(
		N < std::numeric_limits<int>::max() / base &&
			N < std::numeric_limits<int>::max() / elem &&
			N < std::numeric_limits<int>::max() / elem / base,
		"struct Size: Board size to big for size system integer");
};


template<int N>
constexpr int base_size = Size<N>::base; // default 3
template<int N>
constexpr int elem_size = Size<N>::elem; // default 9
template<int N>
constexpr int full_size = Size<N>::full; // default 81


} // namespace Sudoku
