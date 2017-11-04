//===--	Sudoku/Sudoku.h													--===//
//
//
//===---------------------------------------------------------------------===//
#pragma once

#include "Board.h"
#include "Location.h"
#include "Options.h"

#include <vector>
#include <optional>
#include <cassert>

namespace Sudoku
{
template<int N = 3>
class Sudoku
{
public:
	using Location = Location<N>;

	static constexpr int base_size = Location().base_size; // default 3
	static constexpr int elem_size = Location().elem_size; // default 9
	static constexpr int full_size = Location().full_size; // default 81

	using Options = Options<elem_size>;

	Sudoku();
	Sudoku(Board<int, N>);
	Sudoku(std::vector<int>);

	// input
	void setValue(Location, int);
	void removeValue(Location);
	void setOption(Location, int);
	void removeOption(Location, int);

	// output
	const Board<int, N>& start() const;
	const Board<int, N>& input() const;
	const Board<Options, N>& options() const; // all user set options
	std::vector<int> options(Location) const;

	std::optional<Board<int, N>> answer(); // solve and return answer
	std::optional<void> hint(); // return hints (generate list of hints)
								// hint-format?

	void undo(); // undo previous action
	void update_options();


private:
	int hint_level_{};
	bool auto_update_{};
	const Board<int, N> start_{};       // initial user-input
	Board<int, N> input_{};             // user input
	Board<Options, N> input_options_{}; // options marked by user undo/history


}; // class Sudoku

// constructors


template<int N>
inline void Sudoku<N>::setValue(Location loc, int value)
{
	assert(value > 0 && value <= elem_size);
	input_.at(loc) = value;
	// retain any marked options
}

template<int N>
inline void Sudoku<N>::removeValue(Location loc)
{
	input_.at(loc) = 0;
}

template<int N>
inline void Sudoku<N>::setOption(Location loc, int value)
{
	assert(value > 0 && value <= elem_size);
	input_options_.at(loc).add(value);
}

template<int N>
inline void Sudoku<N>::removeOption(Location loc, int value)
{
	assert(value > 0 && value <= elem_size);
	input_options_.at(loc).remove_option(value);
}


} // namespace Sudoku
