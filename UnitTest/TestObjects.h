//	Test boards for unit-test usage
//
#pragma once
#include "stdafx.h"
#include "Board.h"
#include "Location.h"

namespace Sudoku_Test
{
namespace Test_Boards
{

// Basic value boards, not proper Sudoku's
// No use of iterators
namespace bN_fill_1
{
// Requirements: basic class Board<int,N>
template<int N>
void set_ElemNr(Sudoku::Board<int, N>& target)
{
	constexpr int elem{ N*N };
	for (int i = 0; i < target.full_size; ++i)
	{
		target.at(i) = i;// % elem;
	}
}

// Requirements: basic class Board<int,N>
template<int N>
void set_RowNr_X(Sudoku::Board<int, N>& target, int X)
{
	for (int row = 0; row < target.elem_size; ++row)
	{
		for (int col = 0; col < target.elem_size; ++col)
		{
			target.at(row, col) = row + X;
		}
	}
}
template<int N>
void set_RowNr(Sudoku::Board<int, N>& target) { set_RowNr_X(target, 0); }

// Requirements: class Board<int,N>::Inbetween
template<int N>
void set_ColNr_X(Sudoku::Board<int, N>& target, int X)
{
	for (int row = 0; row < target.elem_size; ++row)
	{
		for (int col = 0; col < target.elem_size; ++col)
		{
			target[row][col] = col + X;
		}
	}
}
template<int N>
void set_ColNr(Sudoku::Board<int, N>& target) { set_ColNr_X(target, 0); }

template<int N>
void set_BlockNr_X(Sudoku::Board<int, N>& target)
{
	for (int i = 0; i < target.full_size; ++i)
	{
		target.at(i) = Sudoku::Location<N>(i).block_elem();
	}
}

}	// namespace sN_fill

// 4x4 valid boards
namespace b2_valid
{
std::vector<int> full_1
{
	1,2,3,4,
	3,4,1,2,
	2,1,4,3,
	4,3,2,1
};

std::vector<int> two_by_two_1{ 0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
}	// namespace b2_valid
}	// namespace Test_Boards


}	// namespace Sudoku_Test