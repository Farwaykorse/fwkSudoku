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

namespace Sudoku
{
/* Main object */
template<int N = 3> class SolverBase
{
	using Location = Location<N>;
	static const int base_size = Location().base_size;
	static const int elem_size = Location().elem_size;
	static const int full_size = Location().full_size;

public:
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
	Board<Options<elem_size>, N> getOptions() const;

	/* tmp */
	void single_option(Location loc, int value);
	void solver_unique();

private:
	Board<int, N> start;
	Board<Options<elem_size>, N> options;

	/* Initiate */
	void process_new(const Board<int, N>&);

	/* General functions */
	void setValue(Location loc, int value);

	/* Solvers */
	//void single_option(Location loc, int value);
	//void solver_unique();	// unique in section
	void solver_dual();
	void solver_tripple();
	void solver_dual_hidden();	// pair of values appear 2 times
};

template<int N>
inline SolverBase<N>::SolverBase() :
	start(0)
{
	// empty constructor
}

template<int N>
inline SolverBase<N>::SolverBase(const Board<int, N>& input) :
	start(input)
{
	process_new(input);	// setValue for all elements on options
}

template<int N>
inline SolverBase<N>::SolverBase(const std::vector<int>& input) :
	start(0)
{
	if (input.size() != full_size) { throw std::length_error("input length_error"); }
	if (std::find_if(start.cbegin(), start.cend(), [&](int V) { return V > elem_size || V < 0; }) != start.cend())
	{
		throw std::invalid_argument("elements out of bounds");
	}
	std::copy(input.cbegin(), input.cend(), start.begin());
	process_new(start);
}

template<int N>
inline void SolverBase<N>::setBoard(const Board<int, N>& board)
{
	start = board;
	process_new(board);
	//solve
	//return getResult();
}

template<int N>
inline Board<int, N> SolverBase<N>::getStart() const
{
	return start;
}

template<int N>
inline Board<int, N> SolverBase<N>::getResult() const
{
	Board<int, N> result{};
	for (int i = 0; i < full_size; ++i)
	{
		if (options.at(i).is_answer())
		{
			result.at(i) = options.at(i).answer();
		}
	}
	return result;
}

//template<int N>
//inline Board<Options<elem_size>, N> SolverBase<N>::getOptions() const
//{
//	return options;
//}

template<int N>
inline void SolverBase<N>::process_new(const Board<int, N>& board)
{
	for (int i = 0; i<full_size; ++i)
	{
		int value = board.at(i);
		if (value > 0) { setValue(Location(i), value); }
	}
}

/* Options process answer value */
template<int N> inline
void SolverBase<N>::setValue(const Location loc, const int value)
{
	// valid input
	assert(value <= elem_size);
	Solver<N> S(options);
	if (value > elem_size) { throw std::out_of_range{ "value in setValue(loc, value)" }; }
	if (options.at(loc).is_option(value))
	{
		S.setValue(loc, value);
	}
	else if (!options.at(loc).is_answer(value))
	{ 
		throw std::invalid_argument{ "not an available option, invalid board" };
	}
}

/// remove value from other elements in current row/col/block
/// won't detect a conflict
template<int N> inline
void SolverBase<N>::single_option(const Location loc, const int value)
{
	assert(options.at(loc).count() == 1);
	assert(options.at(loc).is_options(value));
	Solver<N>(options).single_option(loc);
}

template<int N> inline
void SolverBase<N>::solver_unique()
{
	Solver<N> S(options);
	size_t found{ 1 };
	while (found > 0)
	{
		found = 0;
		for (int i = 0; i < elem_size; ++i)
		{
			found += S.unique_section(options.row(i).cbegin(), options.row(i).cend());
			found += S.unique_section(options.col(i).cbegin(), options.col(i).cend());
			found += S.unique_section(options.block(i).cbegin(), options.block(i).cend());
		}
	}
}
}	// namespace Sudoku
