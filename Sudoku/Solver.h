#pragma once
#include "Board.h"
#include "Location.h"
#include <set>

namespace Sudoku
{
namespace Solver
{
template<typename _Data, size_t N>
void single_option(_Data&, const Location<N>);

template<typename _InItr, size_t N> inline
void single_option_section(
	Board<std::set<int>, N>& Board,
	_InItr begin,
	const _InItr end,
	const Location<N> loc)
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

template<typename _Data, size_t N> inline
void single_option(_Data& options, const Location<N> loc)
{
	single_option_section(
		options,
		options.row(loc).begin(),
		options.row(loc).end(),
		loc
	);
	single_option_section(
		options,
		options.col(loc).begin(),
		options.col(loc).end(),
		loc
	);
	single_option_section(
		options,
		options.block(loc).begin(),
		options.block(loc).end(),
		loc
	);
}


}	// namespace Sudoku::Solver
}	// namespace Sudoku