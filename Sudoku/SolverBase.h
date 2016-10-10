/**	SolverBase
 *	Sudoku solver implementation using Board<Options>
 */
#pragma once
#include "stdafx.h"
#include "Board.h"
#include "Location.h"
#include "Solver.h"

#include <cassert>
#include <vector>
#include <stdexcept>

using val_t = unsigned int;

namespace Sudoku
{
/* Main object */
template<size_t N = 3> class SolverBase
{
	using Location = Location<N>;
	static const size_t base_size = Location().base_size;
	static const size_t elem_size = Location().elem_size;
	static const size_t full_size = Location().full_size;

public:
	SolverBase();
	SolverBase(const Board<val_t, N>&);
	SolverBase(const std::vector<val_t>& board);	// needed?? only Board might be enough, added value?
	SolverBase(SolverBase&&)			= default;	// move
	SolverBase& operator=(SolverBase&&)	= default;
	SolverBase(SolverBase const&)		= default;	// copy
	SolverBase& operator=(SolverBase const&) = default;
	~SolverBase()					= default;

	/* Input */
	void setBoard(const Board<val_t, N>& board);

	/* Output */
	Board<val_t, N> getStart() const;
	Board<val_t, N> getResult() const;
	Board<Options<elem_size>, N> getOptions() const;

	/* tmp */
	void single_option(Location loc, val_t value);
	void solver_unique();

private:
	Board<val_t, N> start;
	Board<Options<elem_size>, N> options;

	/* Initiate */
	void process_new(const Board<val_t, N>&);

	/* General functions */
	void setValue(Location loc, val_t value);

	/* Solvers */
	//void single_option(Location loc, int value);
	//void solver_unique();	// unique in section
	void solver_dual();
	void solver_tripple();
	void solver_dual_hidden();	// pair of values appear 2 times
};

template<size_t N>
inline SolverBase<N>::SolverBase() :
	start(0)
{
	// empty constructor
}

template<size_t N>
inline SolverBase<N>::SolverBase(const Board<val_t, N>& input) :
	start(input)
{
	process_new(input);	// setValue for all elements on options
}

template<size_t N>
inline SolverBase<N>::SolverBase(const std::vector<val_t>& input) :
	start(0)
{
	if (input.size() != full_size) { throw std::length_error("input length_error"); }
	if (std::find_if(start.cbegin(), start.cend(), [&](val_t V) { return V > elem_size || V < 0; }) != start.cend())
	{
		throw std::invalid_argument("elements out of bounds");
	}
	std::copy(input.cbegin(), input.cend(), start.begin());
	process_new(start);
}

template<size_t N>
inline void SolverBase<N>::setBoard(const Board<val_t, N>& board)
{
	start = board;
	process_new(board);
	//solve
	//return getResult();
}

template<size_t N>
inline Board<val_t, N> SolverBase<N>::getStart() const
{
	return start;
}

template<size_t N>
inline Board<val_t, N> SolverBase<N>::getResult() const
{
	Board<val_t, N> result{};
	for (size_t i = 0; i < full_size; ++i)
	{
		if (options.at(i).is_answer())
		{
			result.at(i) = options.at(i).answer();
		}
	}
	return result;
}

//template<size_t N>
//inline Board<Options<elem_size>, N> SolverBase<N>::getOptions() const
//{
//	return options;
//}

template<size_t N>
inline void SolverBase<N>::process_new(const Board<val_t, N>& board)
{
	for (size_t i = 0; i<full_size; ++i)
	{
		val_t value = board.at(i);
		if (value > 0) { setValue(Location(i), value); }
	}
}

/* Options process answer value */
template<size_t N> inline
void SolverBase<N>::setValue(const Location loc, const val_t value)
{
	// valid input
	assert(value <= elem_size);
	if (value > elem_size) { throw std::out_of_range{ "value in setValue(loc, value)" }; }
	if (options.at(loc).is_option(value))
	{
		Solver<N>::setValue(options, loc, value);
	}
	else if (!options.at(loc).is_answer(value))
	{ 
		throw std::invalid_argument{ "not an available option, invalid board" };
	}
}

/// remove value from other elements in current row/col/block
/// won't detect a conflict
template<size_t N> inline
void SolverBase<N>::single_option(const Location loc, const val_t value)
{
	assert(options.at(loc).count() == 1);
	assert(options.at(loc).is_options(value));
	Solver<N>::single_option(options, loc);
}

template<size_t N> inline
void SolverBase<N>::solver_unique()
{
	size_t found{ 1 };
	while (found > 0)
	{
		found = 0;
		for (size_t i = 0; i < elem_size; ++i)
		{
			found += Solver<N>::unique_section(options, options.row(i).cbegin(), options.row(i).cend());
			found += Solver<N>::unique_section(options, options.col(i).cbegin(), options.col(i).cend());
			found += Solver<N>::unique_section(options, options.block(i).cbegin(), options.block(i).cend());
		}
	}
}
}	// namespace Sudoku
