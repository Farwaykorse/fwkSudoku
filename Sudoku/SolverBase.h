/**	SolverBase
 *	Sudoku solver implementation using Board<std::set<int>>
 */
#pragma once
#include "stdafx.h"
#include "Board.h"
#include "Location.h"
#include "Solver.h"

#include <cassert>
#include <set>
#include <vector>
#include <stdexcept>

namespace Sudoku
{
/* Main object */
template<size_t N = 3> class SolverBase
{
public:
	using Location = Location<N>;

	SolverBase();
	SolverBase(const Board<int, N>&);
	SolverBase(const std::vector<int>& board);	// needed?? only Board might be enough, added value?
	SolverBase(SolverBase&&)			= default;	// move
	SolverBase& operator=(SolverBase&&)	= default;
	SolverBase(SolverBase const&)		= default;	// copy
	SolverBase& operator=(SolverBase const&) = default;
	~SolverBase()					= default;

	/* Input */
	void setBoard(const Board<int, N>& board);

	/* Output */
	Board<int, N> getStart() const;
	Board<int, N> getResult() const;
	Board<std::set<int>, N> getOptions() const;

	/* tmp */
	void single_option(Location loc, int value);
	void solver_unique();

private:
	Board<int, N> start;
	Board<std::set<int>, N> options;

	static const size_t base_size = Location().base_size;
	static const size_t elem_size = Location().elem_size;
	static const size_t full_size = Location().full_size;

	static std::set<int> characters()
	{
		//TODO replace, always same result with template N
		std::set<int> chars;
		std::generate_n(
			std::inserter(chars, chars.begin()), elem_size,
			[x = 0]()mutable{ return ++x; }
		);
		return chars;
	}

	/* Initiate */
	void process_new(const Board<int, N>&);

	/* General functions */
	void setValue(Location loc, int value);

	/* Solvers */
	//void single_option(Location loc, int value);
	//void single_option_section(Board<int,N>::Row::iterator begin, Section::iterator end, Location loc, int value);
	//void solver_unique();	// unique in section
	void solver_dual();
	void solver_tripple();
	void solver_dual_hidden();	// pair of values appear 2 times

	void related(Location, void (*f)(std::set<int>, Location));
};

template<size_t N>
inline SolverBase<N>::SolverBase() :
	start(0),
	options(characters())
{
	// empty constructor
}

template<size_t N>
inline SolverBase<N>::SolverBase(const Board<int, N>& input) :
	start(input),
	options(characters())
{
	process_new(input);	// setValue for all elements on options
}

template<size_t N>
inline SolverBase<N>::SolverBase(const std::vector<int>& input) :
	start(0),
	options(characters())
{
	if (input.size() != full_size) { throw std::length_error("input length_error"); }
	if (std::find_if(start.cbegin(), start.cend(), [&](int V) { return V > elem_size || V < 0; }) != start.cend())
	{
		throw std::invalid_argument("elements out of bounds");
	}
	std::copy(input.cbegin(), input.cend(), start.begin());
	process_new(start);
}

template<size_t N>
inline void SolverBase<N>::setBoard(const Board<int, N>& board)
{
	start = board;
	process_new(board);
	//solve
	//return getResult();
}

template<size_t N>
inline Board<int, N> SolverBase<N>::getStart() const
{
	return start;
}

template<size_t N>
inline Board<int, N> SolverBase<N>::getResult() const
{
	Board<int, N> result{};
	for (size_t i = 0; i < full_size; ++i)
	{
		if (options.at(i).size() == 1)
		{
			result.at(i) = *options.at(i).cbegin();
		}
	}
	return result;
}

template<size_t N>
inline Board<std::set<int>, N> SolverBase<N>::getOptions() const
{
	return options;
}

template<size_t N>
inline void SolverBase<N>::process_new(const Board<int, N>& board)
{
	for (size_t i = 0; i<full_size; ++i)
	{
		auto& value = board.at(i);
		if (value > 0) { setValue(Location(i), value); }
	}
}

/* Options process answer value */
template<size_t N>
inline void SolverBase<N>::setValue(Location loc, int value)
{
	// valid input
	assert(value > 0);	// not for inverse (solver only)
	assert(value <= elem_size);
	if (value > elem_size) { throw std::out_of_range{ "value in setValue(loc, value)" }; }
	assert(options.at(loc).count(value) == 1);	// value is not an available option, invalid board
if (options.at(loc).count(value) == 0) { throw std::invalid_argument{ "value conflict" }; }
options.at(loc).clear();
options.at(loc).insert(value);

// process row / col / block
single_option(loc, value);
}

/// remove value from other elements in current row/col/block
/// won't detect a conflict
template<size_t N> inline
void SolverBase<N>::single_option(const Location loc, const int value)
{
	assert(options.at(loc).size() == 1);
	assert(*options.at(loc).cbegin() == value);
	Solver::single_option(options, loc);
}

template<size_t N>
inline void SolverBase<N>::solver_unique()
{
	//TODO assert()
	//Solver::unique();

	//TODO for Col and Block
	size_t i = 0;
	//TODO for all elements in section

	//TODO make a function: name(const itr begin, const itr end) {
	const auto begin = options.row(i).cbegin();
	const auto end = options.row(i).cend();
	//TODO const_iterator versions; Board::Section
	std::multiset<int> cache{};
	// copy available options
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

	// find and process all unique options in cache
	for (
		std::multiset<int>::const_iterator itr = cache.lower_bound(1);
		itr != cache.cend();
		++itr)
	{
		if (cache.count(*itr) == 1)	// = unique
		{
			auto found = std::find_if(
				begin,
				end,
				[itr](std::set<int> elem) { return (elem.count(*itr) == 1); }
			);
			assert(found != end);	// cache <-> options out of sync
			// update Board options
			// not if already done by single_option's cascade
			if (found->size() != 1) { setValue(found.location(), *itr); }
		}
	}

	if (cache.size() > 0)	// section not solved
	{
		int value{ 0 };
		while (value < elem_size)
		{
			value = *cache.lower_bound(value);
			if (cache.count(value) == 1)
			{
				std::find_if(
					options.row(i).begin(),
					options.row(i).end(),
					[&value](std::set<int> elem) { return (elem.count(value) == 1); }
				);
			}
			++value;
		}
	}
}

}	// namespace Sudoku
