#pragma once
#include "Board.h"
#include "Location.h"
#include "Options.h"

#include <cassert>
#include <bitset>
//#include <set>

namespace Sudoku
{
namespace Solver
{
template<typename _Data, size_t N> void single_option(_Data& options, const Location<N> loc, const val_t value);
template<typename _InItr, size_t N, size_t elem_size> void remove_option_section(Board<Options<elem_size>, N>& Board, _InItr begin, const _InItr end, const Location<N> loc, const val_t value);

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
	//ERROR assumes start with a new board with all options available
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

/* old code example
template<typename _Data, typename _InItr> inline
std::multiset<int> section_options(_Data&, const _InItr begin, const _InItr end)
{
	std::multiset<int> cache{};
	// copy available options to cache
	std::for_each(
		begin, end,
		[&cache](const std::set<int>& elem)
	{
		if (elem.size() > 1)	// = available
		{
			std::copy(elem.cbegin(), elem.cend(), std::inserter(cache, cache.end()));
		}
	}
	);
	return cache;
}
*/

/* old code example
template<typename _Data, typename _InItr> inline
void unique(
	_Data& board,
	const _InItr begin,
	const _InItr end,
	std::multiset<int>::const_iterator itr)
{
	auto found = std::find_if(
		begin,
		end,
		[itr](std::set<int> elem) { return (elem.count(*itr) == 1); }
	);
	assert(found != end);	// cache <-> options out of sync
	if (found->count() != 1)	// not if already done by single_option's cascade
	{
		setValue(board, found.location(), *itr);	// update Board options
	}
}
*/

/* old code example
template<typename _Data, typename _InItr> inline
unsigned int unique_section(_Data& board, const _InItr begin, const _InItr end)
{
	unsigned int result{ 0 };
	std::multiset<unsigned int> cache{ section_options(board, begin, end) };
	if (cache.size() < 1) { return 0; }

	// find and process all unique options in cache
	for (
		std::multiset<unsigned int>::const_iterator itr = cache.lower_bound(1);
		itr != cache.cend();
		++itr)
	{
		if (cache.count(*itr) == 1)	// = unique
		{
			unique(board, begin, end, itr);
			++result;
		}
	}
	return result;
}
*/

template<size_t N, size_t E, typename InItr_> inline
val_t unique_section(Board<Options<E>,N>& board, const InItr_ begin, const InItr_ end)
{
	static_assert(E == N*N, "invalid sizes");
	Options<E> sum(0);
	Options<E> worker(0);
	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr->is_answer())
		{
			//worker.add(itr->answer());	//NOTE inefficiënt
			worker = *itr + worker;	// bypasses not summing answers check
		}
		else
		{
			worker += (sum & *itr);
			sum += *itr;
		}
	}
	worker.flip();
	
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

}	// namespace Sudoku::Solver
}	// namespace Sudoku