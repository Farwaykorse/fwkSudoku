/**	Utilities for class Sudoku::Location
 *	Non-member functions, included in Board.h
 */
#pragma once

#include <vector>
#include <limits>	// numeric_limits

// Forward declarations
#include "Location.fwd.h"


namespace Sudoku
{
template<int N>
constexpr bool shared_row(Location<N> left, Location<N> right)
{
	return left.row() == right.row();
}

template<int N>
std::vector<Location<N>> shared_row(Location<N> left, std::vector<Location<N>> right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (shared_row(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}

template<int N>
constexpr bool shared_col(Location<N> left, Location<N> right)
{
	return left.col() == right.col();
}

template<int N>
std::vector<Location<N>> shared_col(Location<N> left, std::vector<Location<N>> right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (shared_col(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}

template<int N>
constexpr bool shared_block(Location<N> left, Location<N> right)
{
	return left.block() == right.block();
}

template<int N>
std::vector<Location<N>> shared_block(Location<N> left, std::vector<Location<N>> right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (shared_block(left, loc)) { tmp_.push_back(loc); }
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
		L.base_size < std::numeric_limits<int>::max() &&	// <limits>
		L.elem_size < std::numeric_limits<int>::max() &&
		L.full_size < std::numeric_limits<int>::max(),
		"board size out of bounds");
	// logic check
	static_assert(
		L.base_size*L.base_size == L.elem_size &&
		L.elem_size*L.elem_size == L.full_size,
		"size calculation broken");
}

// Test row/col/block-element
template<int N>
constexpr bool valid_size(const int elem)
{
	return (elem >= 0 && elem < Location<N>().full_size);
}

// Test
template<int N>
constexpr bool valid_size(const int row, const int col)
{
	return (
		row >= 0 && row < Location<N>().elem_size &&
		col >= 0 && col < Location<N>().elem_size);
}


}	// namespace Sudoku
