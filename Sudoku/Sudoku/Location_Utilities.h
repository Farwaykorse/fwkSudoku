//===--- Sudoku/Location_Utilities.h                                    ---===//
//
// Utilities for class Sudoku::Location
//===----------------------------------------------------------------------===//
#pragma once

#include "Board_Section_traits.h"
#include "Location.h"
#include "Size.h"
#include "traits.h"
#include <gsl/gsl> // index

#include <vector>
#include <algorithm> // minmax_element, is_sorted, all_of
#include <iterator>  // back_inserter
#include <limits>    // numeric_limits

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku
{
//===-- function declarations ---------------------------------------------===//
template<int N>
constexpr void valid_dimensions() noexcept;

template<int N>
constexpr bool is_valid(Location<N>) noexcept;
template<int N>
constexpr bool is_valid(const std::vector<Location<N>>& locs) noexcept(true);

template<int N>
constexpr bool is_valid_size(gsl::index elem) noexcept;
template<int N>
constexpr bool is_valid_size(gsl::index row, gsl::index col) noexcept;

template<int N>
constexpr bool is_same_row(Location<N>, Location<N>) noexcept;
template<int N>
constexpr bool is_same_col(Location<N>, Location<N>) noexcept;
template<int N>
constexpr bool is_same_block(Location<N>, Location<N>) noexcept;

template<int N, typename ItrT>
constexpr bool is_same_row(ItrT begin, ItrT end) noexcept;
template<int N, typename ItrT>
constexpr bool is_same_col(ItrT begin, ItrT end) noexcept;
template<int N, typename ItrT>
constexpr bool is_same_block(ItrT begin, ItrT end) noexcept;

template<int N>
std::vector<Location<N>> get_same_row(
	const Location<N>, const std::vector<Location<N>>&) noexcept(true);
template<int N>
std::vector<Location<N>> get_same_col(
	const Location<N>, const std::vector<Location<N>>&) noexcept(true);
template<int N>
std::vector<Location<N>> get_same_block(
	const Location<N>, const std::vector<Location<N>>&) noexcept(true);

template<int N, typename S>
constexpr bool is_same_section(S section, Location<N>) noexcept;

template<typename SectionT, int N>
bool is_same_section(SectionT, std::vector<Location<N>>&) noexcept;

template<
	int N,
	typename S,
	typename = std::enable_if_t<
		Board_Section::traits::is_Row_v<S> ||
		Board_Section::traits::is_Col_v<S>>>
bool intersect_block(S section, Location<N> block_loc) noexcept;

//===----------------------------------------------------------------------===//


// Compile-time only Test
template<int N>
constexpr void valid_dimensions() noexcept
{
	// input check
	static_assert((base_size<N>) > 1, "base_size too small");
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
[[nodiscard]] inline constexpr bool is_valid(const Location<N> loc) noexcept
{
	return (loc.element() >= 0 && loc.element() < full_size<N>);
}

// Test if Locations on Board and if sorted (ascending)
template<int N>
[[nodiscard]] inline constexpr bool
	is_valid(const std::vector<Location<N>>& locs) noexcept(true)
{ // std::is_sorted can throw std::bad_alloc
	return (
		!locs.empty() && (std::is_sorted(locs.cbegin(), locs.cend()) &&
						  locs.cbegin()->element() >= 0 &&
						  locs.crbegin()->element() < full_size<N>));
}

// Test row/col/block-element
template<int N>
[[nodiscard]] inline constexpr bool
	is_valid_size(const gsl::index elem) noexcept
{
	return (elem >= 0 && elem < elem_size<N>);
}

// Test if location on Board
template<int N>
[[nodiscard]] inline constexpr bool
	is_valid_size(const gsl::index row, const gsl::index col) noexcept
{
	return is_valid_size<N>(row) && is_valid_size<N>(col);
}


//===----------------------------------------------------------------------===//

// check
template<int N>
[[nodiscard]] inline constexpr bool
	is_same_row(const Location<N> left, const Location<N> right) noexcept
{
	return (is_valid<N>(left) && is_valid<N>(right)) &&
		   left.row() == right.row();
}

// check: all in same row
template<int N, typename ItrT>
[[nodiscard]] constexpr bool
	is_same_row(const ItrT begin, const ItrT end) noexcept
{
	{
		static_assert(traits::is_forward<ItrT>);
	}
	if (begin == end)
		return false;

	const auto itr = begin + 1;
	return std::all_of(
		itr, end, [begin](Location<N> i) { return is_same_row<N>(*begin, i); });
}

// return all in same row
template<int N> // NOLINTNEXTLINE(bugprone-exception-escape)
[[nodiscard]] std::vector<Location<N>> get_same_row(
	const Location<N> left,
	const std::vector<Location<N>>& right) noexcept(true)
{ // std::copy_if could throw std::bad_alloc
	std::vector<Location<N>> output{};
	const auto predicate = [&left](Location<N> loc) {
		return is_same_row(left, loc);
	};
	std::copy_if(
		right.cbegin(), right.cend(), std::back_inserter(output), predicate);
	return output;
}

// check
template<int N>
[[nodiscard]] inline constexpr bool
	is_same_col(const Location<N> left, const Location<N> right) noexcept
{
	return (is_valid<N>(left) && is_valid<N>(right)) &&
		   left.col() == right.col();
}

// check: all in same col
template<int N, typename ItrT>
[[nodiscard]] inline constexpr bool
	is_same_col(const ItrT begin, const ItrT end) noexcept
{
	{
		static_assert(traits::is_forward<ItrT>);
	}
	if (begin == end)
		return false;

	const auto itr = begin + 1;
	return std::all_of(
		itr, end, [begin](Location<N> i) { return is_same_col<N>(*begin, i); });
}

// return all in same col
template<int N> // NOLINTNEXTLINE(bugprone-exception-escape)
[[nodiscard]] std::vector<Location<N>> get_same_col(
	const Location<N> left,
	const std::vector<Location<N>>& right) noexcept(true)
{ // std::copy_if could throw std::bad_alloc
	std::vector<Location<N>> output{};
	const auto predicate = [&left](Location<N> loc) {
		return is_same_col(left, loc);
	};
	std::copy_if(
		right.cbegin(), right.cend(), std::back_inserter(output), predicate);
	return output;
}

// check
template<int N>
[[nodiscard]] inline constexpr bool
	is_same_block(const Location<N> left, const Location<N> right) noexcept
{
	return (is_valid<N>(left) && is_valid<N>(right)) &&
		   left.block() == right.block();
}

// check all in same block
template<int N, typename ItrT>
[[nodiscard]] inline constexpr bool
	is_same_block(const ItrT begin, const ItrT end) noexcept
{
	{
		static_assert(traits::is_forward<ItrT>);
	}
	if (begin == end)
		return false;

	const auto itr = begin + 1;
	return std::all_of(itr, end, [begin](Location<N> i) {
		return is_same_block<N>(*begin, i);
	});
}

// return all in same block
template<int N> // NOLINTNEXTLINE(bugprone-exception-escape)
[[nodiscard]] std::vector<Location<N>> get_same_block(
	const Location<N> left,
	const std::vector<Location<N>>& right) noexcept(true)
{ // std::copy_if could throw std::bad_alloc
	std::vector<Location<N>> output{};
	const auto predicate = [&left](Location<N> loc) {
		return is_same_block(left, loc);
	};
	std::copy_if(
		right.cbegin(), right.cend(), std::back_inserter(output), predicate);
	return output;
}

// check: [loc] is in [section]
template<int N, typename S>
[[nodiscard]] inline constexpr bool
	is_same_section(const S section, const Location<N> loc) noexcept
{
	static_assert(Board_Section::traits::is_Section_v<S>);

	if constexpr (Board_Section::traits::is_Row_v<S>)
		return is_same_row(loc, section.cbegin().location());
	else if constexpr (Board_Section::traits::is_Col_v<S>)
		return is_same_col(loc, section.cbegin().location());
	else if constexpr (Board_Section::traits::is_Block_v<S>)
		return is_same_block(loc, section.cbegin().location());
}

// check: [section] intersects block containing [loc]
template<int N, typename S, typename>
[[nodiscard]] inline bool
	intersect_block(const S section, const Location<N> block_loc) noexcept
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
[[nodiscard]] inline bool is_same_section(
	SectionT const section, std::vector<Location<N>> const& locs) noexcept
{
	return std::any_of(locs.cbegin(), locs.cend(), [section](Location<N> L) {
		return is_same_section(section, L);
	});
}

} // namespace Sudoku
