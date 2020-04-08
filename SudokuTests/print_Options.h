//===--- SudokuTests/print_Options.h                                    ---===//
//
// Teaching gTest how to print Options
//===----------------------------------------------------------------------===//
#pragma once

#include <Sudoku/Options.h>

#include <ostream>

#include <gtest/gtest.h>


namespace Sudoku
{
template<int N>
std::ostream& operator<<(std::ostream& os, const Options<N>& Options)
{
	return os << Options.DebugString();
}
} // namespace Sudoku
