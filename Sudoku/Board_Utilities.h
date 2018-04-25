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

} // namespace Sudoku
