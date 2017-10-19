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
#include "Board.fwd.h"
#include "Location.fwd.h"


namespace Sudoku
{
template<int N>
static constexpr int base_size = Location<N>().base_size; // default 3
template<int N>
static constexpr int elem_size = Location<N>().elem_size; // default 9
template<int N>
static constexpr int full_size = Location<N>().full_size; // default 81

//===---------------------------------------------------------------------===//

// Compile-time only Test
template<int N>
constexpr void valid_dimensions()
{
	// input check
	static_assert(base_size<N> > 1, "base_size too small");
	static_assert(
		base_size<N> < elem_size<N> && elem_size<N> <= full_size<N> &&
			base_size<N> < std::numeric_limits<int>::max() && // <limits>
			elem_size<N> < std::numeric_limits<int>::max() &&
			full_size<N> < std::numeric_limits<int>::max(),
		"board size out of bounds");
	// logic check
	static_assert(
		base_size<N> * base_size<N> == elem_size<N> &&
			elem_size<N> * elem_size<N> == full_size<N>,
		"size calculation broken");
}

// Test if Location on Board
template<int N>
inline constexpr bool is_valid(const Location<N> loc)
{
	return (loc.element() >= 0 && loc.element() < loc.full_size);
}

// Test if Locations on Board and if sorted (ascending)
template<int N>
inline constexpr bool is_valid(const std::vector<Location<N>>& locs)
{
	return (
		!locs.empty() && (std::is_sorted(locs.cbegin(), locs.cend()) &&
						  locs.cbegin()->element() >= 0 &&
						  locs.crbegin()->element() < full_size<N>));
}

// Test row/col/block-element
template<int N>
inline constexpr bool is_valid_size(const int elem)
{
	return (elem >= 0 && elem < elem_size<N>);
}

// Test if location on Board
template<int N>
inline constexpr bool is_valid_size(const int row, const int col)
{
	return is_valid_size<N>(row) && is_valid_size<N>(col);
}

// Test input value
template<int N>
inline constexpr bool is_valid_value(const int value)
{
	return value > 0 && value <= elem_size<N>;
}

// Test input values
template<int N>
inline constexpr bool is_valid_value(const std::vector<int>& values)
{
	return (
		!values.empty() &&
		std::all_of(values.cbegin(), values.cend(), [](int i) {
			return is_valid_value<N>(i);
		}));
}

//===---------------------------------------------------------------------===//

// check
template<int N>
inline constexpr bool
	is_same_row(const Location<N> left, const Location<N> right)
{
	return left.row() == right.row();
}

// check: all in same row
template<int N, typename InItr_>
constexpr bool is_same_row(const InItr_ begin, const InItr_ end)
{
	auto itr = begin + 1;
	return std::all_of(
		itr, end, [begin](Location<N> i) { return is_same_row<N>(*begin, i); });
}

// return all in same row
template<int N>
std::vector<Location<N>>
	get_same_row(const Location<N> left, const std::vector<Location<N>>& right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (is_same_row(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}

// check
template<int N>
inline constexpr bool
	is_same_col(const Location<N> left, const Location<N> right)
{
	return left.col() == right.col();
}

// check: all in same col
template<int N, typename InItr_>
inline constexpr bool is_same_col(const InItr_ begin, const InItr_ end)
{
	auto itr = begin+1;
	return std::all_of(
		itr, end, [begin](Location<N> i) { return is_same_col<N>(*begin, i); });
}

// return all in same col
template<int N>
std::vector<Location<N>>
	get_same_col(const Location<N> left, const std::vector<Location<N>>& right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (is_same_col(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}

// check
template<int N>
inline constexpr bool
	is_same_block(const Location<N> left, const Location<N> right)
{
	return left.block() == right.block();
}

// check all in same block
template<int N, typename InItr_>
inline constexpr bool is_same_block(const InItr_ begin, const InItr_ end)
{
	auto itr = begin + 1;
	return std::all_of(itr, end, [begin](Location<N> i) {
		return is_same_block<N>(*begin, i);
	});
}

// return all in same block
template<int N>
std::vector<Location<N>> get_same_block(
	const Location<N> left, const std::vector<Location<N>>& right)
{
	std::vector<Location<N>> tmp_{};
	for (auto&& loc : right)
	{
		if (is_same_block(left, loc)) { tmp_.push_back(loc); }
	}
	return tmp_;
}

// check: [loc] is in [section]
template<typename T, int N>
inline bool is_same_section(
	const Board_Section::const_Row<T, N> section, const Location<N> loc)
{
	return is_same_row(loc, section.cbegin().location());
}

// check: [loc] is in [section]
template<typename T, int N>
inline bool is_same_section(
	const Board_Section::const_Col<T, N> section, const Location<N> loc)
{
	return is_same_col(loc, section.cbegin().location());
}

// check: [loc] is in [section]
template<typename T, int N>
inline bool is_same_section(
	const Board_Section::const_Block<T, N> section, const Location<N> loc)
{
	return is_same_block(loc, section.cbegin().location());
}

// check: [section] intersects block containing [loc]
template<typename T, int N>
inline bool intersect_block(
	const Board_Section::const_Row<T, N> section, const Location<N> block_loc)
{
	for (auto itr = section.cbegin(); itr != section.cend(); ++itr)
	{
		if (is_same_block(block_loc, itr.location()))
		{
			return true;
		}
	}
	return false;
}

// check: [section] intersects block containing [loc]
template<typename T, int N>
inline bool intersect_block(
	const Board_Section::const_Col<T, N> section, const Location<N> block_loc)
{
	for (auto itr = section.cbegin(); itr != section.cend(); ++itr)
	{
		if (is_same_block(block_loc, itr.location()))
		{
			return true;
		}
	}
	return false;
}

// check: at least one [location] inside [section]
template<typename SectionT, int N>
inline bool
	is_same_section(const SectionT section, const std::vector<Location<N>> locs)
{
	return std::any_of(locs.cbegin(), locs.cend(), [section](Location<N> L) {
		return is_same_section(section, L);
	});
}

} // namespace Sudoku
