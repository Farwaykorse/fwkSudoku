#pragma once
#include "Board.h"
#include "Location.h"

#include <cassert>
#include <set>

namespace Sudoku
{
namespace Solver
{
template<size_t N> inline
void setValue(
	Board<std::set<int>, N>& board,
	const Location<N> loc,
	const int value)
{
	assert(value > 0);
	assert(value <= board.elem_size);
	assert(board.at(loc).count(value) == 1);	// value is not an available option, invalid board
	board.at(loc).clear();
	board.at(loc).insert(value);

	// process row / col / block
	single_option(board, loc);
}

template<size_t N, typename _InItr> inline
void setValue(
	Board<std::set<int>, N>& board,
	_InItr begin, _InItr end)
{
	//ERROR assumes start with a new board with all options available
	assert(end - begin == board.full_size);
	size_t n{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		Location<N> loc(n++);
		if (*itr > 0)
		{
			if (board.at(loc).size() > 1) { setValue(board, loc, *itr); }
			else
			{
				assert(*(board.at(loc).cbegin()) == *itr);
			}
		}
	}
}

template<typename _Data, size_t N> inline
void single_option(_Data& options, const Location<N> loc)
{
	remove_option_section(
		options,
		options.row(loc).begin(),
		options.row(loc).end(),
		loc
	);
	remove_option_section(
		options,
		options.col(loc).begin(),
		options.col(loc).end(),
		loc
	);
	remove_option_section(
		options,
		options.block(loc).begin(),
		options.block(loc).end(),
		loc
	);
}

template<typename _InItr, size_t N> inline
void remove_option_section(
	Board<std::set<int>, N>& Board,
	_InItr begin,
	const _InItr end,
	const Location<N> loc)	// ingnore loc
{
	assert(Board[loc].size() == 1);
	const int value = *(begin[loc].cbegin());
	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr != loc)
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
	if (found->size() != 1)	// not if already done by single_option's cascade
	{
		setValue(board, found.location(), *itr);	// update Board options
	}
}

template<typename _Data, typename _InItr> inline
int unique_section(_Data& board, const _InItr begin, const _InItr end)
{
	int result{ 0 };
	std::multiset<int> cache{ section_options(board, begin, end) };
	if (cache.size() < 1) { return 0; }

	// find and process all unique options in cache
	for (
		std::multiset<int>::const_iterator itr = cache.lower_bound(1);
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

}	// namespace Sudoku::Solver
}	// namespace Sudoku