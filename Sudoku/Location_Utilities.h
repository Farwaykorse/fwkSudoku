//===--	Sudoku/Location_Utilities.h										--===//
//
//	Utilities for class Sudoku::Location
//	Included in Location.h
//===---------------------------------------------------------------------===//
#pragma once

#include <vector>
#include <algorithm> // minmax_element, is_sorted, all_of
#include <limits>    // numeric_limits

// Forward declarations
#include "Location.fwd.h"


namespace Sudoku
{
template<int N>
constexpr bool is_same_row(const Location<N> left, const Location<N> right)
{
	return left.row() == right.row();
}

template<int N>
std::vector<Location<N>>
	is_same_row(const Location<N> left, const std::vector<Location<N>>& right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (is_same_row(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}

template<int N>
constexpr bool is_same_col(const Location<N> left, const Location<N> right)
{
	return left.col() == right.col();
}

template<int N>
std::vector<Location<N>>
	is_same_col(const Location<N> left, const std::vector<Location<N>>& right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (is_same_col(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}

template<int N>
constexpr bool is_same_block(const Location<N> left, const Location<N> right)
{
	return left.block() == right.block();
}

template<int N>
std::vector<Location<N>>
	is_same_block(const Location<N> left, const std::vector<Location<N>>& right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (is_same_block(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}


// ############################################################################

// Compile-time only Test
template<int N>
constexpr void valid_dimensions()
{
	constexpr Location<N> L{};
	// input check
	static_assert(L.base_size > 1, "base_size too small");
	static_assert(
		L.base_size < L.elem_size &&
		L.elem_size <= L.full_size &&
		L.base_size < std::numeric_limits<int>::max() && // <limits>
		L.elem_size < std::numeric_limits<int>::max() &&
		L.full_size < std::numeric_limits<int>::max(),
		"board size out of bounds");
	// logic check
	static_assert(
		L.base_size*L.base_size == L.elem_size &&
		L.elem_size*L.elem_size == L.full_size,
		"size calculation broken");
}

// Test if Location on Board
template<int N>
constexpr bool is_valid(const Location<N> loc)
{
	return (loc.element() >= 0 && loc.element() < loc.full_size);
}

// Test if Locations on Board and if sorted (ascending)
template<int N>
constexpr bool is_valid(const std::vector<Location<N>>& locs)
{
	return (
		locs.empty() || (std::is_sorted(locs.cbegin(), locs.cend()) &&
						 locs.cbegin()->element() >= 0 &&
						 locs.crbegin()->element() < Location<N>().full_size));
}

// Test row/col/block-element
template<int N>
constexpr bool is_valid_size(const int elem)
{
	return (elem >= 0 && elem < Location<N>().elem_size);
}

// Test if location on Board
template<int N>
constexpr bool is_valid_size(const int row, const int col)
{
	return is_valid_size<N>(row) && is_valid_size<N>(col);
}

// Test input value
template<int N>
constexpr bool is_valid_value(const int value)
{
	return value > 0 && value <= Location<N>().elem_size;
}

// Test input values
template<int N>
constexpr bool is_valid_value(const std::vector<int>& values)
{
	return std::all_of(values.cbegin(), values.cend(), [](int i) {
		return is_valid_value<N>(i);
	});
}

} // namespace Sudoku
