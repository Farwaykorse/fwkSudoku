//====---- Sudoku/Normalize.h                                         ----====//
//
// Normalize Board, by switching option values, so the first/top row is always
// the same. I.e. 1, 2, ... elem_size<N>
//====--------------------------------------------------------------------====//
#pragma once

#include "Board.h"
#include "Options.h"
#include "Size.h"

#include <algorithm>


namespace Sudoku
{
template<int N>
inline Board<Options<elem_size<N>>, N>& switch_options(
	Board<Options<elem_size<N>>, N>& board, Value const a, Value const b)
{
	assert(is_valid<N>(a));
	assert(is_valid<N>(b));

	const auto transform = [a, b](Options<elem_size<N>> O) {
		return O.switch_options(a, b);
	};
	std::transform(board.cbegin(), board.cend(), board.begin(), transform);
	return board;
}

// sort answers in first/top row
template<int N>
Board<Options<elem_size<N>>, N>&
	normalize(Board<Options<elem_size<N>>, N>& board) noexcept(true)
{ // switch_options can throw std::out_of_range: programming error
	for (int i{0}; i < elem_size<N>; ++i)
	{
		if (Value val{static_cast<size_t>(i + 1)};
			is_answer_fast(board[0][i]) && board[0][i] != val)
		{
			switch_options(board, val, get_answer(board[0][i]));
		}
	}
	return board;
}

} // namespace Sudoku
