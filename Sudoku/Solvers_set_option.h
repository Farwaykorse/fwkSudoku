//===--	Sudoku/Solver_set_option.h										--===//
//
// Helper functions
//===---------------------------------------------------------------------===//
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
#include <gsl/gsl>
#include <algorithm>   // find_if
#include <stdexcept>   // logic_error
#include <type_traits> // is_base_of
#include <cassert>

// Forward declarations
#include "Solver.fwd.h" // single_option


namespace Sudoku
{
//===---------------------------------------------------------------------===//
template<int N, typename Options = Options<elem_size<N>>>
void setValue(Board<Options, N>&, Location<N>, Value);
template<int N, typename Options = Options<elem_size<N>>, typename ItrT>
void setValue(Board<Options, N>&, ItrT begin, ItrT end);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
auto set_uniques(Board<Options, N>&, SectionT, Options worker);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int set_section_locals(
	Board<Options, N>&, SectionT, int rep_count, Options worker);
template<int N, typename Options = Options<elem_size<N>>>
int set_section_locals(
	Board<Options, N>&,
	Board_Section::Block<Options, N>,
	int rep_count,
	Options worker);

//===---------------------------------------------------------------------===//


//	IF valid, Make [value] the answer for [loc]
//	No processing
template<int N, typename Options>
inline void
	setValue(Board<Options, N>& board, const Location<N> loc, const Value value)
{
	assert(is_valid(loc));
	assert(is_valid<N>(value));

	if (!board.at(loc).test(value))
	{ // value is option nor answer
		throw std::logic_error{"Invalid Board"};
	}
	board[loc].set_nocheck(value);
}

//	set board_ using a transferable container of values
template<int N, typename Options, typename ItrT>
inline void setValue(Board<Options, N>& board, const ItrT begin, const ItrT end)
{
	{
		static_assert(Utility_::is_forward<ItrT>);
		assert(end - begin == full_size<N>);
	}
	int n{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		Location<N> loc(n++); // start at 0!
		Value value{};
		if constexpr (Utility_::iterator_to<ItrT, Value>)
		{
			value = *itr;
		}
		else
		{
			value = to_Value(*itr);
		}
		if (value > Value{0} && board.at(loc).is_option(value))
		{
			setValue(board, loc, value);
			single_option(board, loc, value);
		}
		// check invalid value or conflict
		assert(value == Value{0} || board.at(loc).is_answer(value));
	}
	assert(n == full_size<N>);
}

//	Set unique values in section as answer
template<int N, typename Options, typename SectionT>
inline auto set_uniques(
	Board<Options, N>& board, const SectionT section, const Options worker)
{
	{
		using Section_ = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Section_, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
	}
	int changes{0};

	if (worker.count_all() > 0)
	{
		const auto begin = section.cbegin();
		const auto end   = section.cend();

		for (size_t val{1}; val < worker.size(); ++val)
		{
			const auto value = gsl::narrow_cast<Value>(val);
			if (worker[value])
			{
				const auto itr = std::find_if( // <algorithm>
					begin,
					end,
					[value](Options O) { return O.test(value); });
				if (itr != end)
				{
					setValue(board, itr.location(), value);
					changes += single_option(board, itr.location(), value);
					++changes;
				}
				else
				{
					assert(false); // value is not an option
				}
			}
		}
	}
	return changes;
}

//	for [row/col]: if all in same block, remove [values] from rest block
template<int N, typename Options, typename SectionT>
inline int set_section_locals(
	Board<Options, N>& board,
	const SectionT section,
	const int rep_count,
	const Options worker)
{
	{
		using Section_ = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Section_, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(rep_count > 1);  // should have been cought by caller
								// use the set_uniques specialization
		assert(rep_count <= N); // won't fit in single block-row/col
		assert(worker.count_all() > 0);
	}
	int changes{0};

	// start at 1, to skip the answer-bit
	for (size_t val{1}; val < worker.size(); ++val)
	{
		const auto value = gsl::narrow_cast<Value>(val);
		if (worker[value])
		{
			const auto locations = find_locations<N>(section, value, rep_count);
			if (locations.size() != static_cast<size_t>(rep_count))
			{
				assert(changes > 0); // changed by earlier value in worker
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

//	remove found set in block from rest of its row/col
template<int N, typename Options>
inline int set_section_locals(
	Board<Options, N>& board,
	const Board_Section::Block<Options, N> block,
	const int rep_count,
	const Options worker)
{
	{
		using BlockT   = Board_Section::Block<Options, N>;
		using iterator = typename BlockT::const_iterator;
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(rep_count > 1);  // should have been cought by caller
								// use the set_uniques specialization
		assert(rep_count <= N); // won't fit in single block-row/col
		assert(worker.count_all() > 0);
	}
	int changes{0};

	// start at 1, to skip the answer-bit
	for (size_t val{1}; val < worker.size(); ++val)
	{
		const auto value = gsl::narrow_cast<Value>(val);
		if (worker[value])
		{
			const auto locations = find_locations<N>(block, value, rep_count);
			if (locations.size() != static_cast<size_t>(rep_count))
			{
				assert(changes > 0); // changed by earlier value in worker
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
