//===--- Sudoku/Board_Utilities.h                                       ---===//
//
// Helper-functions for Sudoku::Board
//===----------------------------------------------------------------------===//
// Non-member functions, included in Board.h
//===----------------------------------------------------------------------===//
#pragma once

#include "Board.fwd.h"

namespace Sudoku
{
template<typename T, int N>
constexpr bool operator!=(const Board<T, N>& left, const Board<T, N>& right)
{
	return !(left == right);
}

template<typename T, int N, typename ownerT>
typename Board_Section::iterator<T, N, ownerT>
	operator+(int offset, Board_Section::iterator<T, N, ownerT> itr) noexcept
{
	return (itr += offset);
}
template<typename T, int N, typename ownerT>
typename Board_Section::const_iterator<T, N, ownerT> operator+(
	int offset, Board_Section::const_iterator<T, N, ownerT> itr) noexcept
{
	return (itr += offset);
}


} // namespace Sudoku
