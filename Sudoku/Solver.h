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
	static const int base_size{ N };
	static const int elem_size{ N*N };
	static const int full_size{ elem_size * elem_size };

	using Location	= Location<N>;
	using Options	= Options<elem_size>;
	using Board		= Sudoku::Board<Options, base_size>;
	using Row		= typename Board::Row;
	using Col		= typename Board::Col;
	using Block		= typename Board::Block;
public:
	Solver(Board&);

	void setValue(Location, int);
	template<typename InItr_>
	void setValue(InItr_ begin, InItr_ end);

	template<typename InItr_>
	int remove_option_section(InItr_ begin, InItr_ end, Location ignore, int value);
	template<typename InItr_>
	int remove_option_section(InItr_ begin, InItr_ end, const std::vector<Location>& ignore, int value);
	template<typename InItr_>
	int remove_option_section(InItr_ begin, InItr_ end, const std::vector<Location>& ignore, const std::vector<int>& value);

	template<typename InItr_>
	int unique_section(InItr_ begin, InItr_ end);

	template<typename InItr_>
	int section_exclusive(InItr_ begin, InItr_ end);
	template<typename InItr_>
	int block_exclusive(InItr_ begin, InItr_ end);
	//TODO simplify calling
	int section_exclusive(Block);



private:
	Board& board_;

	int single_option(Location loc, int value);
	int dual_option(Location);

	template<typename InItr_>
	auto set_uniques(InItr_ begin, InItr_ end, const Options& worker);
	template<typename InItr_>
	int set_section_locals(InItr_ begin, InItr_ end, int rep_count, const Options& worker);
	template<typename InItr_>
	int set_block_locals(InItr_ begin, InItr_ end, int rep_count, const Options& worker);

	template<typename InItr_> inline
	auto appearance_sets(InItr_ begin, InItr_ end) const;
	template<typename InItr_> inline
	auto find_locations(InItr_ begin, InItr_ end, int rep_count, int value) const;

	template<typename InItr_>
	int remove_option_outside_block(InItr_ begin, InItr_ end, Location block, int value);
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
	assert(value > 0 && value <= elem_size);
	assert(board_.at(loc).test(value));	// check if available option, if not invalid board_
	board_.at(loc).set(value);

	// process row / col / block
	single_option(loc, value);
}

///	set board_ using a vector of values
template<int N>
template<typename InItr_> inline
void Solver<N>::setValue(const InItr_ begin, const InItr_ end)
{
	assert(end - begin == full_size);
	int n{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		Location loc(n++);
		if (*itr > 0)
		{
			if (board_.at(loc).is_option(*itr))
			{
				setValue(loc, *itr);
			}
			else
			{
				assert(board_.at(loc).is_answer(*itr));
			}
		}
	}
}

///	Process answer from [loc], update board_ options
template<int N> inline
int Solver<N>::single_option(const Location loc, const int value)
{
	assert(loc.element() >= 0 && loc.element() < full_size);	// loc inside board_
	assert(board_.at(loc).test(value));

	if (board_.at(loc).count() == 1 &&
		board_.at(loc).is_option(value)) { setValue(loc, value); }
	else if (board_.at(loc).is_answer())
	{
		int found{};
		found += remove_option_section(
			board_.row(loc).begin(),
			board_.row(loc).end(),
			loc,
			value
		);
		found += remove_option_section(
			board_.col(loc).begin(),
			board_.col(loc).end(),
			loc,
			value
		);
		found += remove_option_section(
			board_.block(loc).begin(),
			board_.block(loc).end(),
			loc,
			value
		);
		return found;
	}
	return 0;
}

template<int N> inline
int Solver<N>::dual_option(const Location loc)
{
	assert(loc.element() < full_size);	// loc inside board_
	assert(board_[loc].count() == 2);		// shouldn't have been called
	//TODO find exact same in row
		//TODO remove BOTH values from rest row
	//TODO find exact same in col
	//TODO find exact same in block

	return 0;
}

/// remove [value] in [loc] from other elements in section
template<int N>
template<typename InItr_> inline
int Solver<N>::remove_option_section(
	const InItr_ begin,
	const InItr_ end,
	const Location loc,
	const int value)
{
	assert(board_.at(loc).is_answer(value));

	int found{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		if ( !(itr->is_answer()) &&
			itr->remove(value))		// true if applied
		{
			++found;
			found += single_option(itr.location(), itr->answer());	// new answer found -> cascade
		}
	}
	return found;
}

/// remove [value] from element if not [block]
template<int N>
template<typename InItr_> inline
int Solver<N>::remove_option_outside_block(
	const InItr_ begin,
	const InItr_ end,
	const Location block,
	const int value)
{
	int found{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr->is_answer()) { continue; }
		else if (itr.location().block() == block.block()) { continue; }
		else if (itr->remove(value))
		{
			++found;
			found += single_option(itr.location(), itr->answer());
		}
	}
	return found;
}

/// remove [value] from [section] if not part of [loc]s
template<int N>
template<typename InItr_> inline
int Solver<N>::remove_option_section(
	const InItr_ begin,
	const InItr_ end,
	const std::vector<Location>& ignore,
	const int value)
{
	int found{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		if (!itr->is_option(value)) { continue; }
		else if (ignore.cend() == std::find_if(
			ignore.cbegin(),
			ignore.cend(),
			[itr](Location loc) { return itr == loc; }))
		{
			if (itr->remove(value))
			{
				++found;
				found += single_option(itr.location(), itr->answer());	// cascade
			}
		}
	}
	return found;
}

/// remove [value]s from [section] if not part of [loc]s
template<int N>
template<typename InItr_> inline
int Solver<N>::remove_option_section(
	InItr_ begin,
	const InItr_ end,
	const std::vector<Location>& ignore,
	const std::vector<int>& values)
{
	int found{ 0 };
	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr->is_answer()) { continue; }
		else if (ignore.cend() == std::find_if(
			ignore.cbegin(),
			ignore.cend(),
			[itr](Location loc) { return itr == loc; }))
		{
			for (auto v : values)
			{
				if (itr->remove(v))
				{
					++found;
					found += single_option(itr.location(), itr->answer());	// cascade
				}
			}
		}
	}
	return found;
}


template<int N>
template<typename InItr_> inline
int Solver<N>::unique_section(const InItr_ begin, const InItr_ end)
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
auto Solver<N>::set_uniques(InItr_ begin, InItr_ end, const Options& worker)
{
	const size_t count = worker.count_all();
	int found{ 0 };
	if (count > 0)
	{
		for (size_t value{ 1 }; value < worker.size(); ++value)
		{
			if (worker.test(value))
			{
				auto itr = std::find_if(
					begin, end,
					[value](Options O) { return O.is_option(value); }
				);
				assert(itr != end); // doesn't exist
				setValue(itr.location(), value);
				found += single_option(itr.location(), value);
				++found;
			}
		}
	}
	return found;
}

// value appearing 2x or 3x in row/col:
//		IF all in same block -> remove from rest of block
template<int N>
template<typename InItr_> inline
int Solver<N>::section_exclusive(const InItr_ begin, const InItr_ end)
{
	int found{};	// performance counter

	auto appearing = appearance_sets(begin, end);

	int i{ 2 };
	while (i <= N)	// won't run if condition fails
	{
		// unique specialization
		if (appearing[1].count_all() > 0)
		{
			found += set_uniques(begin, end, appearing[1]);
			appearing = appearance_sets(begin, end);
		}
		else if (appearing[i].count_all() > 0)
		{
			found += set_section_locals(begin, end, i, appearing[i]);
			appearing = appearance_sets(begin, end);
			++i;
		}
		else { ++i; }
	}
	return found;
}

template<int N>
template<typename InItr_> inline
int Solver<N>::block_exclusive(const InItr_ begin, const InItr_ end)
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
int Solver<N>::set_section_locals(
	const InItr_ begin,
	const InItr_ end,
	const int rep_count,
	const Options& worker)
{
	const size_t count = worker.count_all();
	assert(count > 0);		// should have been cought by caller

	int found{ 0 };
	for (size_t value{ 1 }; value < worker.size(); ++value)
	{
		if (worker.test(value))
		{
			const auto locations = find_locations(begin, end, rep_count, value);
			assert(locations.size() <= N);	// no use won't fit in a single block-row/col (too much data gathered!)
			assert(locations.size() > 1);	// use the set_uniques specialization

			// for each value check if appearing in same block
			if (locations.cend() == std::find_if_not(
				locations.cbegin(), locations.cend(),
				[&locations](auto L) { return L.block() == locations[0].block(); }))
			{
				found += remove_option_section(
					board_.block(locations[0]).begin(),
					board_.block(locations[0]).end(),
					locations,
					value);
			}
		}
	}
	return found;
}

template<int N>
template<typename InItr_> inline
int Solver<N>::set_block_locals(const InItr_ begin, const InItr_ end, const int rep_count, const Options& worker)
{
	const size_t count = worker.count_all();
	assert(count > 0);		// should have been cought by caller

	int found{ 0 };
	for (size_t value{ 1 }; value < worker.size(); ++value)
	{
		if (worker.test(value))
		{
			const auto locations = find_locations(begin, end, rep_count, value);
			assert(locations.size() <= N);	// no use won't fit in a single block-row/col
			assert(locations.size() > 1);	// use the set_uniques specialization

			// for each value check if appearing in same row/col
			if (locations.cend() == std::find_if_not(
				locations.cbegin(), locations.cend(),
				[&locations](auto L) { return L.row() == locations[0].row(); }))
			{
				found += remove_option_outside_block(
					board_.row(locations[0]).begin(),
					board_.row(locations[0]).end(),
					locations[0],
					value);
			}
			else if (locations.cend() == std::find_if_not(
				locations.cbegin(), locations.cend(),
				[&locations](auto L) { return L.col() == locations[0].col(); }))
			{
				found += remove_option_outside_block(
					board_.col(locations[0]).begin(),
					board_.col(locations[0]).end(),
					locations[0],
					value);
			}
			else { continue; }	// not in same row/col
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
auto Solver<N>::appearance_sets(const InItr_ begin, const InItr_ end) const
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
			for (int i{ N }; i >= 0; --i)
			{
				worker[i] = *elem_itr + worker[i];
			}
		}
		else
		{
			for (int i{ N }; i > 0; --i)
			{
				worker[i] += (worker[i - 1] & *elem_itr);
			}
			worker[0] += *elem_itr;
		}
	}
	//	flip -> worker[n] contains options appearing n times or less
	for (int i{ 0 }; i <= N; ++i)
	{
		worker[i].flip();
	}
	assert(worker[0].is_empty());	// fails if not all options exist
	//	xor -> worker[n] options appearing n times
	//TODO xor isn't a natural interface, is it really more effici�nt?
	for (int i{ N }; i > 1; --i)
	{
		worker[i] ^= worker[i - 1];
	}
	return worker;
}

template<int N>
template<typename InItr_> inline
auto Solver<N>::find_locations(
	const InItr_ begin,
	const InItr_ end,
	const int rep_count,
	const int value				) const
{
	assert(value > 0 && value <= elem_size);
	assert(rep_count > 0 && rep_count <= board_.full_size);

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






}	// namespace Sudoku