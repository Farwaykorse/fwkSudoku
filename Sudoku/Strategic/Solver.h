//===--- Sudoku/Solver.h                                                ---===//
//
//	Solver functions acting on a Board<Options>&
// Declarations are in Solver.fwd.h
//===----------------------------------------------------------------------===//
#pragma once

#include "Solvers_find.h"
#include "Solvers_remove_option.h"
#include "Solvers_set_option.h"

#include "Sudoku/Board.h"
#include "Sudoku/Location.h"
#include "Sudoku/Location_Utilities.h"
#include "Sudoku/Options.h"
#include "Sudoku/Size.h"
#include "Sudoku/Value.h"
#include <gsl/gsl>

#include <vector>

#include <algorithm>   // minmax
#include <type_traits> // is_base_of

#include "Solver.fwd.h" // Forward declarations

#include <cassert>


namespace Sudoku
{
//	Check if only one option remaining
//	IF true: process answer
template<int N, typename Options>
inline int single_option(
	Board<Options, N>& board, // NOLINT(runtime/references)
	const Location<N> loc)
{
	assert(is_valid(loc));

	if (const Value answer{get_answer(board.at(loc))})
	{
		return single_option(board, loc, answer);
	}
	return 0;
}

//	Process answer from [loc], update board_ options
//	Remove option from rest of row, col and block
template<int N, typename Options>
inline int single_option(
	Board<Options, N>& board, // NOLINT(runtime/references)
	const Location<N> loc,
	const Value value)
{
	{
		assert(is_valid(loc));
		assert(is_valid<N>(value));
		assert(board.at(loc).test(value));
	}
	int changes{};
	changes += set_Value(board, loc, value);
	changes += remove_option_section(board, board.row(loc), loc, value);
	changes += remove_option_section(board, board.col(loc), loc, value);
	changes += remove_option_section(board, board.block(loc), loc, value);
	return changes;
}

// if 2 options in element:
// find exact pair in section:
// remove form other elements in section
template<int N, typename Options>
inline int dual_option(
	Board<Options, N>& board, // NOLINT(runtime/references)
	const Location<N> loc)
{
	using Location = Location<N>;
	{
		assert(is_valid(loc));
		assert(board.at(loc).count() == 2);
	}
	const auto sorted_loc = [loc](const Location L)
	{
		const auto result = std::minmax(loc, L);
		return std::vector<Location>{result.first, result.second};
	};

	int changes{};
	const Options& item{board.at(loc)};
	const auto mask = [](Options x) noexcept
	{
		x[Value{0}] = false;
		return x;
	}(item);

	for (int i{}; i < full_size<N>; ++i)
	{
		// find exact same in board
		if (board[Location(i)] == item && Location(i) != loc)
		{
			// Remove values for rest of shared elements
			if (is_same_row(loc, Location(i)))
			{
				changes += remove_option_section(
					board, board.row(loc), sorted_loc(Location(i)), mask);
			}
			else if (is_same_col(loc, Location(i)))
			{
				changes += remove_option_section(
					board, board.col(loc), sorted_loc(Location(i)), mask);
			}
			// run not if already answered
			if (is_same_block(loc, Location(i)) && !item.is_answer())
			{ // NOTE this is slow
				changes += remove_option_section(
					board, board.block(loc), sorted_loc(Location(i)), mask);
			}
		}
	}
	return changes;
}

// Find subsets in any related sections,
// and remove the values from rest of these sections.
template<int N, typename Options>
inline int multi_option(
	Board<Options, N>& board, // NOLINT(runtime/references)
	const Location<N> loc)
{
	assert(is_valid(loc));

	return multi_option(board, loc, board.at(loc).count());
}

// Find subsets in any related section,
// and remove the values from rest of these sections.
template<int N, typename Options>
constexpr int multi_option(
	Board<Options, N>& board, // NOLINT(runtime/references)
	const Location<N> loc,
	const size_t count)
{
	assert(is_valid(loc));

	// set execution domain & specializations
	switch (count)
	{
	case 0: [[fallthrough]];     // already answered
	case elem_size<N>: return 0; // no result anyway
	case 1: return single_option(board, loc);
	case 2: return dual_option(board, loc);
	default: assert(count < elem_size<N>); break;
	}

	int changes{};                      // performance counter
	const Options& item{board.at(loc)}; // input item, to match with
	assert(item.count() == count);
	const auto mask = [](Options x) noexcept
	{
		x[Value{0}] = false;
		return x;
	}(item);

	const auto list = list_where_subset(board, item);

	// Further select per section.
	// When (subset size == #options) then: all answers in this selection.
	// Therefore: Remove values for rest of section.
	if (const auto in_row{get_same_row(loc, list)}; in_row.size() == count)
	{
		changes += remove_option_section(board, board.row(loc), in_row, mask);
	}
	if (const auto in_col{get_same_col(loc, list)}; in_col.size() == count)
	{
		changes += remove_option_section(board, board.col(loc), in_col, mask);
	}
	if (const auto in_block{get_same_block(loc, list)};
		in_block.size() == count)
	{
		// NOTE this is slow
		changes +=
			remove_option_section(board, board.block(loc), in_block, mask);
	}
	return changes;
}

// Solver: Find options appearing only once in a section and set as answer
template<int N, typename Options, typename SectionT>
inline int unique_in_section(
	Board<Options, N>& board, // NOLINT(runtime/references)
	const SectionT section)
{
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
	}
	const auto worker = appearance_once<N>(section);
	return set_uniques(board, section, worker);
}

//	Solver: find and process values appearing 1 to base_size times in section:
//	[row/col] IF all in same block -> remove from rest of block
//	[block] IF all in same row/col -> remove from rest of row/col
template<int N, typename Options, typename SectionT>
inline int section_exclusive(
	Board<Options, N>& board, // NOLINT(runtime/references)
	const SectionT section)
{
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
		static_assert(std::is_same_v<typename SectionT::value_type, Options>);
	}
	int changes{}; // performance counter

	size_t i{2};
	auto appearing             = appearance_sets<N>(section);
	const auto renew_appearing = [&i, &a = appearing, &section]()
	{
		i = 2;
		a = appearance_sets<N>(section);
	};
	while (i < appearing.size()) // won't run if condition fails
	{
		// unique specialization
		if (appearing[1].count_all() > 0)
		{
			changes += set_uniques(board, section, appearing[1]);
			renew_appearing();
			continue;
		}
		if (appearing.at(i).count_all() > 0)
		{ // for [row/col]: if in same block: remove from rest block
			// for [block]: if in same row/col: remove from rest row/col
			if (const int tmp_ = set_section_locals(
					board, section, gsl::narrow_cast<int>(i), appearing.at(i)))
			{
				changes += tmp_;
				renew_appearing();
				continue;
			}
		}
		++i;
	}
	return changes;
}


// cell containing 2 options
//	find in board_	size() == 2
//		find exact same in rest board
//			IF pair in same row/col -> remove values from rest row/col
//			IF same block -> remove values from rest block
//		repeat until end of board_
//	repeat until end of board_


// cell containing 2 options
//	find in board_	size() == 2
//		find exact same in rest row
//			IF found:	remove values from rest row
//		find exact same in rest col
//		find exact same in rest block

} // namespace Sudoku
