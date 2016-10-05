#pragma once
#include "Board.h"
#include "Location.h"
#include "Options.h"

#include <cassert>
#include <bitset>

namespace Sudoku
{
namespace Solver
{
template<typename Data_, size_t N> void single_option(Data_& options, const Location<N> loc, const val_t value);
template<typename InItr_, size_t N, size_t elem_size> void remove_option_section(Board<Options<elem_size>, N>& Board, InItr_ begin, const InItr_ end, const Location<N> loc, const val_t value);

///	Make [value] the answer for [loc] and process
template<size_t base_size, size_t elem_size> inline
void setValue(
	Board<Options<elem_size>, base_size>& board,
	const Location<base_size> loc,
	const val_t value)
{
	static_assert(elem_size == Location<base_size>().elem_size, "setValue<B,E> B & E don't match");
	assert(value <= board.elem_size);
	assert(board.at(loc)[value] == true);	// check if available option, if not invalid board
	board.at(loc).set(value);

	// process row / col / block
	single_option(board, loc, value);
}

///	set board using a vector of values
template<size_t N, size_t elem_size, typename InItr_> inline
void setValue(
	Board<Options<elem_size>, N>& board,
	InItr_ begin, InItr_ end)
{
	static_assert(elem_size == N*N, "setValue<N,E,itr>(board, begin, end) N:E size mismatch");
	assert(end - begin == board.full_size);
	size_t n{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		Location<N> loc(n++);
		if (*itr > 0)
		{
			if (board.at(loc).count() > 1) { setValue(board, loc, *itr); }
			else
			{
				assert(board.at(loc).is_answer(*itr));
			}
		}
	}
}

///	Process answer from [loc], update board options
template<typename Data_, size_t N> inline
void single_option(Data_& options, const Location<N> loc, const val_t value)
{
	assert(loc.element() < loc.full_size);	// loc inside board
	assert(options.at(loc).is_answer());		// contains anwer
	assert(options.at(loc).is_answer(value));	// answer and correct value
	remove_option_section(
		options,
		options.row(loc).begin(),
		options.row(loc).end(),
		loc,
		value
	);
	remove_option_section(
		options,
		options.col(loc).begin(),
		options.col(loc).end(),
		loc,
		value
	);
	remove_option_section(
		options,
		options.block(loc).begin(),
		options.block(loc).end(),
		loc,
		value
	);
}

/// remove [value] in [loc] from other elements in section
template<typename InItr_, size_t N, size_t elem_size> inline
void remove_option_section(
	Board<Options<elem_size>, N>& Board,
	InItr_ begin,
	const InItr_ end,
	const Location<N> loc,
	const val_t value)	// ingnore loc
{
	static_assert(elem_size == Location<N>().elem_size, "remove_option_section<if, N, E> N & E mismatch");
	assert(Board[loc].is_answer(value));

	for (auto itr = begin; itr != end; ++itr)
	{
		if ( !(itr->is_answer()) )
		{
			if (itr->remove(value)) // true if applied
			{
				if (itr->count() == 1)	// new answer found=>cascade
				{
					Solver::setValue(Board, itr.location(), itr->answer());
				}
			}
		}
	}
}

/*
template<typename _InItr, size_t N> inline
void remove_option_section(
	Board<std::set<int>, N>& Board,
	_InItr begin,
	const _InItr end,
	const std::vector<Location<N>> loc,	// ignore elements in loc
	const int value)
{
	for (auto itr = begin; itr != end; ++itr)
	{
		if (loc.cend() == std::find_if(
			loc.cbegin(),
			loc.cend(),
			[itr](Location<N> loc) { return itr != loc; }))
		{
			if (itr->erase(value))
			{
				if (itr->size() == 1)	// cascade
				{
					Solver::single_option(Board, itr.location());
				}
			}
		}
	}
}
*/

/// available options in a section
template<typename Data_, typename InItr_, size_t elem_size> inline
Options<elem_size> section_options(Data_&, const InItr_ begin, const InItr_ end)
{
	Options<elem_size> available{};
	available.clear();
	for (InItr_ itr = begin; itr != end; ++itr)
	{
		available += *itr;
	}
	return available;
}

template<size_t N, size_t E, typename InItr_> inline
val_t unique_section(Board<Options<E>,N>& board, const InItr_ begin, const InItr_ end)
{
	static_assert(E == N*N, "invalid sizes");
	Options<E> sum(0);		// helper all used
	Options<E> worker(0);	// multiple uses OR answer
	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr->is_answer())
		{
			worker = *itr + worker;
		}
		else
		{
			worker += (sum & *itr);
			sum += *itr;
		}
	}
	worker.flip();	// multiple uses -> single-use
	worker.add(0);	// set answer flag for correct behaviour count()

	// process uniques
	const size_t count = worker.count();
	if (count > 0)
	{
		std::vector<val_t> uniques{};
		uniques.reserve(count);
		uniques = worker.available();
		for (val_t value : uniques)
		{
			Location<N> loc = std::find_if(
				begin, end,
				[value](Options<E> O) { return O.is_option(value); }
			).location();
			setValue(board, loc, value);
		}
	}
	return count;
}

/*
2x in block + zelfde block-row/col
=> remove from rest row/col
*/
/*template<typename _InItr, size_t N> inline
void block_exclusive(Board<std::set<int>,N> board, _InItr begin, _InItr end)
{
	std::multiset<int> cache{ section_options(board, begin, end) };

	for (
		std::multiset<int>::const_iterator itr = cache.lower_bound(1);
		itr != cache.cend();
		++itr)
	{
		if (cache.count(*itr) == 1)	// = unique
		{
			unique(board, begin, end, itr);
		}
		else if (cache.count(*itr) <= N)
		{
			auto lambda = [itr](std::set<int> elem) {return (elem.count(*itr) == 1); };
			// check if all in same row or col
			std::vector<Location<N>> items{};
			auto found = std::find_if(begin, end, lambda);
			items.push_back(found.location());
			for (size_t i = 1; i < cache.count(*itr); ++i)
			{
				found = std::find_if(found, end, lambda);
				items.push_back(found.location());
			}
			bool is_row{ true };
			bool is_col{ true };
			for (Location<N>& loc : items)
			{
				if (loc.row() != items[1].row()) { is_row = false; }
				if (loc.col() != items[1].col()) { is_col = false; }
			}
			assert( !(is_row && is_col) );	// can't both be true
			if (is_row)
			{
				//setValue(board, items);
				remove_option_section(
					board,
					board.row(items[0]).begin(),
					board.row(items[0]).end(),
					items, *itr); 
			}
			// if is_col
		}
	}
}
*/

}	// namespace Sudoku::Solver
}	// namespace Sudoku