﻿//===--- Sudoku/Solver_remove_option.h                                  ---===//
//
// Helper functions, to remove options
//===----------------------------------------------------------------------===//
#pragma once

#include "Board.h"
#include "Iterator_Utilities.h"
#include "Location.h"
#include "Options.h"
#include "Size.h"
#include "Value.h"
#include "exceptions.h"

#include <vector>
#include <algorithm>   // none_of
#include <type_traits> // is_base_of
#include <cassert>

// Forward declarations
#include "Solver.fwd.h" // single_option

/* experiment flags */
//! DO #undef at end of file!
// activate algorithms on removing option
#define DUAL_ON_REMOVE false
#define MULTIPLE_ON_REMOVE false


namespace Sudoku
{
//===----------------------------------------------------------------------===//
template<int N, typename Options = Options<elem_size<N>>>
int remove_option(Board<Options, N>&, Location<N>, Value);
template<int N, typename Options = Options<elem_size<N>>>
int remove_option(Board<Options, N>&, Location<N>, Options mask);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_section(
	Board<Options, N>&, SectionT, Location<N> ignore, Value);
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_section(
	Board<Options, N>&,
	SectionT,
	const std::vector<Location<N>>& ignore,
	Value);
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_section(
	Board<Options, N>&,
	SectionT,
	const std::vector<Location<N>>& ignore,
	const std::vector<Value>&);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_outside_block(
	Board<Options, N>&, SectionT, Location<N> block_loc, Value);

//===----------------------------------------------------------------------===//


//	remove option from element, make answer if last option
template<int N, typename Options>
int remove_option(
	Board<Options, N>& board, const Location<N> loc, const Value value)
{
	assert(is_valid<N>(value));

	int changes{};
	auto& item{board.at(loc)};

	if (is_option(item, value))
	{
		++changes;
		const auto count = item.remove_option(value).count();
		assert(count > 0); // never trigger, removed last option

		if (count == 1)
		{
			changes += single_option(board, loc);
		}
#if DUAL_ON_REMOVE == true
		if (count == 2)
		{
			changes += dual_option(board, loc);
		}
#endif // dual
#if MULTIPLE_ON_REMOVE == true
		changes += multi_option(board, loc, count);
#endif // multiple
	}
	return changes;
}

// remove all options given by the mask
template<int N, typename Options>
int remove_option(
	Board<Options, N>& board, const Location<N> loc, const Options mask)
{
	assert(is_answer_fast(mask)); // don't remove answer-bit

	auto& item{board.at(loc)};
	if (is_answer_fast(item))
	{
		return 0;
	}
	int changes{static_cast<int>(item.count())};

	// remove options
	item = item - mask;

	const auto count = static_cast<int>(item.count_all());
	if (count == 0)
	{ // removed last option
		throw error::invalid_Board();
	}
	changes -= count;

	if (changes && count == 1)
	{
		changes += single_option(board, loc);
	}
	return changes;
}

//	remove [value] in [ignore] from other elements in set
template<int N, typename Options, typename SectionT>
int remove_option_section(
	Board<Options, N>& board,
	const SectionT section,
	const Location<N> ignore,
	const Value value)
{
	{
		using Board_Section = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid<N>(value));
		assert(is_same_section(section, ignore));
		assert(is_answer(board.at(ignore), value)); // first set as anwer!
	}
	int changes{0};
	const auto end = section.cend();

	for (auto itr = section.cbegin(); itr != end; ++itr)
	{
		if (itr.location() != ignore)
		{
			changes += remove_option(board, itr.location(), value);
		}
	}
	return changes;
}

//	remove [value] from set if not part of [loc]s
template<int N, typename Options, typename SectionT>
inline int remove_option_section(
	Board<Options, N>& board,
	const SectionT section,
	const std::vector<Location<N>>& ignore,
	const Value value)
{
	{
		using Board_Section = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid<N>(value));
		assert(is_same_section(section, ignore));
	}
	int changes{0};
	const auto begin = ignore.cbegin();
	const auto end   = ignore.cend();

	// TODO maybe faster to run get_same_row/col/block first?
	// and than not to check is_option(), since it is already in remove_option

	for (auto itr = section.cbegin(); itr != section.cend(); ++itr)
	{
		// TODO is the is_option check really faster?
		if (is_option(*itr, value) &&
			std::none_of(begin, end, [L1 = itr.location()](Location<N> L2) {
				return L1 == L2;
			})) // <algorithm>
		{
			changes += remove_option(board, itr.location(), value);
		}
	}
	return changes;
}

// remove [value]s from [section] if not part of [loc]s
// the ignore Location vector must be sorted
template<int N, typename Options, typename SectionT>
int remove_option_section(
	Board<Options, N>& board,
	const SectionT section,
	const std::vector<Location<N>>& ignore,
	const std::vector<Value>& values)
{
	{
		using Board_Section = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid<N>(values));
		assert(is_same_section(section, ignore));
	}
	int changes{0};
	const auto begin = ignore.cbegin();
	const auto end   = ignore.cend();

	for (auto itr = section.cbegin(); itr != section.cend(); ++itr)
	{
		if (not(itr->is_answer()) and
			std::none_of(begin, end, [L1 = itr.location()](Location<N> L2) {
				return L1 == L2;
			})) // <algorithm>
		{
			for (auto v : values)
			{ //! Cascade (!)
				changes += remove_option(board, itr.location(), v);
			}
		}
	}
	return changes;
}

//	remove [value] from elements not in same block as [block_loc]
template<int N, typename Options, typename SectionT>
int remove_option_outside_block(
	Board<Options, N>& board,
	const SectionT section,
	const Location<N> block_loc,
	const Value value)
{
	{
		using Section_ = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Section_, SectionT>);
		using Block_ = typename Board_Section::const_Block<Options, N>;
		static_assert(
			not std::is_base_of_v<Block_, SectionT>,
			"remove_option_outside_block is useless on bock");
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(block_loc));
		assert(is_valid<N>(value));
		assert(intersect_block(section, block_loc));
	}
	int changes{0};
	const auto end = section.cend();

	for (auto itr = section.cbegin(); itr != end; ++itr)
	{
		if (not is_same_block(itr.location(), block_loc))
		{
			changes += remove_option(board, itr.location(), value);
		}
	}
	return changes;
}

/*	Remove local macros */
#undef DUAL_ON_REMOVE
#undef MULTIPLE_ON_REMOVE

} // namespace Sudoku
