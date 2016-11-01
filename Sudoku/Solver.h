/**	Container class for solver functions
*/
#pragma once
#include "Board.h"
#include "Location.h"
#include "Options.h"

#include <cassert>
#include <array>
#include <vector>

namespace Sudoku
{
template<int N>
class Solver
{
	static const int base_size = N;
	static const int elem_size = N*N;

	using Location	= Location<N>;
	using Options	= Options<elem_size>;
	using Board		= Sudoku::Board<Options, base_size>;
	using Row		= typename Board::Row;
	using Col		= typename Board::Col;
	using Block		= typename Board::Block;
public:
	Solver(Board&);

	void setValue(const Location, const int);
	template<typename InItr_>
	void setValue(InItr_ begin, InItr_ end);

	template<typename InItr_>
	void remove_option_section(InItr_ begin, const InItr_ end, const Location loc, const int value);

	template<typename InItr_>
	int unique_section(const InItr_ begin, const InItr_ end);

	template<typename InItr_>
	int block_exclusive(InItr_ begin, InItr_ end); // redundant
	//TODO simplify calling
	int block_exclusive(Block);

private:
	Board& board_;


	void single_option(const Location loc, const int value);

	template<typename InItr_>
	auto set_uniques(InItr_ begin, InItr_ end, const Options& worker);
	template<typename InItr_>
	int set_block_locals(InItr_ begin, InItr_ end, const int rep_count, const Options& worker);

	template<typename InItr_> inline
	auto appearance_sets(InItr_ begin, InItr_ end);
	template<typename InItr_> inline
	auto find_locations(InItr_ begin, const InItr_ end, const int rep_count, const size_t value);

	template<typename InItr_>
	void remove_option_outside_block(InItr_ begin, const InItr_ end, const Location block, const int value);
};

template<int N> inline
Solver<N>::Solver(Board& options) : board_(options)
{
	// empty constructor
}

///	Make [value] the answer for [loc] and process
template<int N> inline
void Solver<N>::setValue(
	const Location loc,
	const int value)
{
	assert(value <= board_.elem_size);
	assert(board_.at(loc)[value] == true);	// check if available option, if not invalid board_
	board_.at(loc).set(value);

	// process row / col / block
	single_option(loc, value);
}

///	set board_ using a vector of values
template<int N>
template<typename InItr_> inline
void Solver<N>::setValue(
	InItr_ begin,
	InItr_ end)
{
	assert(end - begin == board_.full_size);
	int n{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		Location loc(n++);
		if (*itr > 0)
		{
			if (board_.at(loc).count() > 1) { setValue(loc, *itr); }
			else
			{
				assert(board_.at(loc).is_answer(*itr));
			}
		}
	}
}

///	Process answer from [loc], update board_ options
template<int N> inline
void Solver<N>::single_option(
	const Location loc,
	const int value)
{
	assert(loc.element() < loc.full_size);	// loc inside board_
	assert(board_.at(loc).is_answer());		// contains anwer
	assert(board_.at(loc).is_answer(value));	// answer and correct value
	remove_option_section(
		board_.row(loc).begin(),
		board_.row(loc).end(),
		loc,
		value
	);
	remove_option_section(
		board_.col(loc).begin(),
		board_.col(loc).end(),
		loc,
		value
	);
	remove_option_section(
		board_.block(loc).begin(),
		board_.block(loc).end(),
		loc,
		value
	);
}

/// remove [value] in [loc] from other elements in section
template<int N>
template<typename InItr_> inline
void Solver<N>::remove_option_section(
	InItr_ begin,
	const InItr_ end,
	const Location loc,
	const int value)	// ingnore loc
{
	assert(board_[loc].is_answer(value));

	for (auto itr = begin; itr != end; ++itr)
	{
		if ( !(itr->is_answer()) )
		{
			if (itr->remove(value)) // true if applied
			{
				if (itr->count() == 1)	// new answer found=>cascade
				{
					Solver::setValue(itr.location(), itr->answer());
				}
			}
		}
	}
}

template<int N>
template<typename InItr_> inline
void Solver<N>::remove_option_outside_block(
	InItr_ begin,
	const InItr_ end,
	const Location block,
	const int value)
{
	for (auto itr = begin; itr != end; ++itr)
	{
		if (!(itr->is_answer()))
		{
			if (itr.location().block() != block.block())
			{
				if (itr->remove(value))
				{
					if (itr->count() == 1)
					{
						Solver::setValue(itr.location(), itr->answer());
					}
				}
			}
		}
	}
}

/*
template<typename _InItr, int N> inline
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

template<int N>
template<typename InItr_> inline
int Solver<N>::unique_section(
	const InItr_ begin,
	const InItr_ end)
{
	Options sum(0);		// helper all used
	Options worker(0);	// multiple uses OR answer
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

	// process uniques
	return set_uniques(begin, end, worker);
}


template<int N>
template<typename InItr_> inline
int Solver<N>::block_exclusive(InItr_ begin, InItr_ end)
{
	//TODO ensure it's a block?
	//TODO type-checking on board_

	int found{};	// performance counter

	int i{ 1 };
	auto appearing = appearance_sets(begin, end);
	while (i <= N)	// won't run if condition fails
	{
		// unique in block specialization
		if (appearing[1].count_all() > 0)
		{
			found += set_uniques(begin, end, appearing[1]);
			appearing = appearance_sets(begin, end);
		}
		else if (appearing[i].count_all() > 0)
		{
			found += set_block_locals(begin, end, i, appearing[i]);
			appearing = appearance_sets(begin, end);
			++i;
		}
		else { ++i; }
	}
	return found;
	//TODO can this be added/used?
	// 2 values only appear in 2 cells -> remove rest from cells
}

template<int N>
template<typename InItr_> inline
int Solver<N>::set_block_locals(InItr_ begin, const InItr_ end, const int rep_count, const Options& worker)
{
	const size_t count = worker.count_all();
	assert(count > 0);		// should have been cought by caller

	int found{ 0 };
	for (size_t value{ 1 }; value < worker.size(); ++value)
	{
		if (worker[value] == 1)
		{
			const auto locations = find_locations(begin, end, rep_count, value);
			assert(locations.size() <= N);	// no use won't fit in a single block-row/col
			assert(locations.size() > 1);	// use the set_uniques specialization

			// for each value check if appearing in same row/col
			if (locations.cend() == std::find_if_not(
				locations.cbegin(), locations.cend(),
				[&locations](auto L) { return L.row() == locations[0].row(); }))
			{
				remove_option_outside_block(
					board_.row(locations[0]).begin(),
					board_.row(locations[0]).end(),
					locations[0],
					value);
			}
			else if (locations.cend() == std::find_if_not(
				locations.cbegin(), locations.cend(),
				[&locations](auto L) { return L.col() == locations[0].col(); }))
			{
				remove_option_outside_block(
					board_.col(locations[0]).begin(),
					board_.col(locations[0]).end(),
					locations[0],
					value);
			}
			else { continue; }	// not in same row/col
		}
	} // end uniques
	return found;
}

template<int N>
template<typename InItr_> inline
auto Solver<N>::set_uniques(InItr_ begin, InItr_ end, const Options& worker)
{
	const size_t count = worker.count_all();
	int found{ 0 };
	if (count > 0)
	{
		for (size_t value{ 1 }; value < worker.size(); ++value)
		{
			if (worker[value] == 1)
			{
				auto itr = std::find_if(
					begin, end,
					[value](Options O) { return O.is_option(value); }
				);
				assert(itr != end); // doesn't exist
				setValue(itr.location(), value);
				++found;
			}
		}
	}
	return found;
}

/*
input			|	[0]			|	[1]			|	[2]			|	[3]
1	100 000 001	|	100	000	001	|	000	000	000	|				|	
2	110 100 010	|	110	100	011	|	100	000	000	|	000	000	000	|	
3	010 000 011	|	110	100	011	|	110	000	011	|	000	000	000	|	000	000	000
4	000 000 011	|	110	100	011	|	110	000	011	|	000	000	011	|	000	000	000
5	101 100	111	|	111	100	111	|	110	100	011	|	100	000	011	|	000	000	011
6 A	000	001	000	|	111	101	111	|	110	101	011	|	100	001	011	|	000	001	011
7	101	100	110	|	111	101	111	|	111	101	111	|	100	101	011	|	100	001	011
8	100	110	010	|	111	111	111	|	111	101	111	|	100	101	011	|	100	101	011
9	010	000	101	|				|	111	101	111	|	110	101	111	|	100	101	011
inv				|	000	000	000	|	000	010	000	|	001	010	000	|	011	010	100
	532	419	365		0 keer			1x				<=2x			<=3x
- [n-1]								000	010	000	|	001	000	000	|	010	000	100
													==2x			==3x
What about the answer bit?
*/
template<int N>
template<typename InItr_> inline
auto Solver<N>::appearance_sets(InItr_ begin, InItr_ end)
{
	std::array<Options, N + 1> worker{};
	for (int i{ 0 }; i <= N; ++i) { worker[i].set(0); }
	//	Collect options by appearence count
	//	worker[n] contains options appearing more than n times (or answer)
	for (auto elem_itr = begin; elem_itr != end; ++elem_itr)
	{
		if (elem_itr->is_answer())
		{
			// add answer to all
			for (int i{ N }; i > 0; --i)
			{
				worker[i] = *elem_itr + worker[i];
			}
		}
		else
		{
			for (int i{ N }; i > 0; --i)
			{
				//PERF	faster/slower when useing this test?
				//if (!worker[i].all())
				//{
					worker[i] += (worker[i - 1] & *elem_itr);
				//}
			}
			worker[0] += *elem_itr;
		}
	}
	//	flip -> worker[n] contains options appearing n times or less
	for (int i{ 1 }; i <= N; ++i)
	{
		worker[i].flip();
	}
	//	xor -> worker[n] options appearing n times
	//TODO xor isn't a natural interface, is it really more efficiënt?
	for (int i{ N }; i > 1; --i)
	{
		worker[i] ^= worker[i - 1];
	}
	return worker;
}

template<int N>
template<typename InItr_> inline
auto Solver<N>::find_locations(InItr_ begin, const InItr_ end, const int rep_count, const size_t value)
{
	std::vector<Location> locations{};
	auto last = begin;
	for (int i{ 0 }; i < rep_count; ++i)
	{
		last = std::find_if(
			last, end,
			[value](Options O) { return O.is_option(value); }
		);
		assert(last != end);	// incorrect rep_count
		locations.emplace_back(last.location());
		++last;
	}
	return locations;
}

/*
// 2x in block + zelfde block-row/col
// => remove from rest row/col
template<typename InItr_, int N> inline
void block_exclusive(Board<std::set<int>,N> board_, InItr_ begin, InItr_ end)
{
	std::multiset<int> cache{ section_options(board_, begin, end) };

	for (
		std::multiset<int>::const_iterator itr = cache.lower_bound(1);
		itr != cache.cend();
		++itr)
	{
	if (cache.count(*itr) == 1)	// = unique
	{
		unique(board_, begin, end, itr);
	}
	else if (cache.count(*itr) <= N)
	{
		auto lambda = [itr](std::set<int> elem) {return (elem.count(*itr) == 1); };
		// check if all in same row or col
		std::vector<Location<N>> items{};
		auto found = std::find_if(begin, end, lambda);
		items.push_back(found.location());
		for (int i = 1; i < cache.count(*itr); ++i)
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
			//setValue(board_, items);
			remove_option_section(
				board_,
				board_.row(items[0]).begin(),
				board_.row(items[0]).end(),
				items, *itr);
		}
		// if is_col
	}
}
}
*/
}	// namespace Sudoku