//	Test boards for unit-test usage
//
#pragma once

namespace Sudoku_Test
{
// Basic value boards, not proper Sudoku's
// No use of iterators
namespace Test_Boards_1
{

// Requirements: basic class Board<int,N>
template<size_t N>
void set_ElemNr(Sudoku::Board<int, N>& target)
{
	constexpr size_t elem{ N*N };
	for (size_t i = 0; i < target.size(); ++i)
	{
		target.at(i) = i % elem;
	}
}

// Requirements: basic class Board<int,N>
template<size_t N>
void set_RowNr_X(Sudoku::Board<int, N>& target, int X)
{
	for (size_t row = 0; row < target.col(0).size(); ++row)
	{
		for (size_t col = 0; col < target.row(0).size(); ++col)
		{
			target.at(row, col) = col + 1;
		}
	}
}
template<size_t N>
void set_RowNr(Sudoku::Board<int, N>& target) { set_RowNr_X(target, 0); }

// Requirements: class Board<int,N>::Inbetween
template<size_t N>
void set_ColNr_X(Sudoku::Board<int, N>& target, int X)
{
	for (size_t row = 0; row < target.col(0).size(); ++row)
	{
		for (size_t col = 0; col < target.row(0).size(); ++col)
		{
			target[row][col] = col + X;
		}
	}
}
template<size_t N>
void set_ColNr(Sudoku::Board<int, N>& target) { set_ColNr_X(target, 0); }




}	// namespace Test_Boards_1
}	// namespace Sudoku_Test