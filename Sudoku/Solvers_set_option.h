//===--- Sudoku/Solver_set_option.h                                     ---===//
//
// Helper functions
//===----------------------------------------------------------------------===//
#pragma once

#include "Board.h"
#include "Iterator_Utilities.h"
#include "Location.h"
#include "Location_Utilities.h" // is_same_*
#include "Options.h"
#include "Size.h"
#include "Solvers_find.h"
#include "Solvers_remove_option.h"
#include "Value.h"
#include "exceptions.h"
#include <gsl/gsl>
#include <algorithm>   // find_if
#include <iterator>    // next
#include <stdexcept>   // logic_error
#include <type_traits> // is_base_of
#include <cassert>

// Forward declarations
#include "Solver.fwd.h" // single_option


namespace Sudoku
{
//===----------------------------------------------------------------------===//
template<int N, typename Options = Options<elem_size<N>>>
int set_Value(Board<Options, N>&, Location<N>, Value);
template<int N, typename Options = Options<elem_size<N>>, typename ItrT>
int set_Value(Board<Options, N>&, ItrT begin, ItrT end);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int set_unique(Board<Options, N>&, SectionT, Value);
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int set_uniques(Board<Options, N>&, SectionT, Options worker);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int set_section_locals(
	Board<Options, N>&, SectionT, int rep_count, Options values);
template<int N, typename Options = Options<elem_size<N>>>
int set_section_locals(
	Board<Options, N>&,
	Board_Section::Block<Options, N>,
	int rep_count,
	Options values);

//===----------------------------------------------------------------------===//


// IF a possible option, Make [value] the answer for [loc]
template<int N, typename Options>
inline int set_Value(
	Board<Options, N>& board, const Location<N> loc, const Value value)
{
	assert(is_valid(loc));
	assert(is_valid<N>(value));

	int changes{0};
	auto& elem = board.at(loc);
	if (not elem.test(value))
	{ // value is option nor answer
		throw error::invalid_Board();
	}
	else if (not is_answer(elem))
	{
		changes = gsl::narrow_cast<int>(elem.count_all());
		elem.set_nocheck(value);
	}
	return changes;
}

// set board_ using a transferable container of values
template<int N, typename Options, typename ItrT>
int set_Value(Board<Options, N>& board, const ItrT begin, const ItrT end)
{
	{
		static_assert(Utility_::is_forward<ItrT>);
		assert(end - begin == full_size<N>);
	}
	int changes{0};
	int n{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		const Location<N> loc(n++); // start at 0!

		// handle different input types
		Value value{};
		if constexpr (Utility_::iterator_to<ItrT, Value>)
		{
			value = *itr;
		}
		else
		{
			value = to_Value<3>(*itr);
		}

		if (value != Value{0})
		{
			if (not is_valid<N>(value))
			{
				throw std::domain_error{"Invalid Value"};
			}

			if (is_option(board.at(loc), value))
			{ // update options on board
				changes += single_option(board, loc, value);
			}
			assert(is_answer(board.at(loc), value));
		}
	}
	assert(n == full_size<N>);
	return changes;
}

// Set unique values in section as answer
template<int N, typename Options, typename SectionT>
inline int set_uniques(
	Board<Options, N>& board, const SectionT section, const Options worker)
{
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
		static_assert(std::is_same_v<typename SectionT::value_type, Options>);
	}
	int changes{0};

	if (worker.count_all() > 0)
	{
		for (size_t val{1}; val < worker.size(); ++val)
		{
			if (worker[Value{val}])
			{
				changes += set_unique(board, section, Value{val});
			}
		}
	}
	return changes;
}

// Set unique value in section as answer
template<int N, typename Options, typename SectionT>
inline int set_unique(
	Board<Options, N>& board, const SectionT section, const Value value)
{
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
		static_assert(std::is_same_v<typename SectionT::value_type, Options>);
		static_assert(Utility_::is_input<typename SectionT::iterator>);
		assert(is_valid<N>(value));
	}
	const auto end       = section.cend();
	const auto condition = [value](const Options& O) { return O.test(value); };

	const auto itr = std::find_if(section.cbegin(), end, condition);

	if (itr == end)
	{ // option not available in section
		throw error::invalid_Board();
	}
	assert(&(*itr) == &board[itr.location()]); // section must be part of board
	assert(std::find_if(std::next(itr), end, condition) == end); // unique

	return single_option(board, itr.location(), value);
}

// for [row/col] per value: if all in same block, remove from rest block
template<int N, typename Options, typename SectionT>
inline int set_section_locals(
	Board<Options, N>& board,
	const SectionT section,
	const int rep_count,
	const Options values)
{
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
		static_assert(std::is_same_v<typename SectionT::value_type, Options>);
		assert(rep_count > 1);  // should have been caught by caller
								// use the set_uniques specialization
		assert(rep_count <= N); // won't fit in single block-row/col
		assert(values.count_all() > 0);
	}
	int changes{0};
	std::vector<Location<N>> locations{};

	// start at 1, to skip the answer-bit
	for (size_t val{1}; val < values.size(); ++val)
	{
		const auto value = gsl::narrow_cast<Value>(val);
		if (values[value])
		{
			locations = list_where_option<N>(section, value, rep_count);
			if (locations.size() != gsl::narrow_cast<size_t>(rep_count))
			{
				assert(changes > 0); // changed by earlier value in values
			}
			else if (is_same_block<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of block
				changes += remove_option_section(
					board, board.block(locations[0]), locations, value);
			}
		}
	}
	return changes;
}

// per value in [block]: if all in same row/col, remove from rest row/col
template<int N, typename Options>
inline int set_section_locals(
	Board<Options, N>& board,
	const Board_Section::Block<Options, N> block,
	const int rep_count,
	const Options values)
{
	{
		assert(rep_count > 1);  // should have been caught by caller
								// use the set_uniques specialization
		assert(rep_count <= N); // won't fit in single block-row/col
		assert(values.count_all() > 0);
	}
	int changes{0};
	std::vector<Location<N>> locations{};

	// start at 1, to skip the answer-bit
	for (size_t val{1}; val < values.size(); ++val)
	{
		const auto value = Value{val};
		if (values[value])
		{
			locations = list_where_option<N>(block, value, rep_count);
			if (locations.size() != gsl::narrow_cast<size_t>(rep_count))
			{
				assert(changes > 0); // changed by earlier value in values
			}
			else if (is_same_row<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of row
				changes += remove_option_outside_block(
					board, board.row(locations[0]), locations[0], value);
			}
			else if (is_same_col<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of col
				changes += remove_option_outside_block(
					board, board.col(locations[0]), locations[0], value);
			}
		}
	}
	return changes;
}


} // namespace Sudoku
