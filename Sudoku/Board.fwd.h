//===--- Sudoku/Board.fwd.h                                             ---===//
//
//	Forward Declared Header of class Board
//===----------------------------------------------------------------------===//
#pragma once

namespace Sudoku
{
template<typename T, int N>
class Board;

namespace Board_Section
{
	template<typename T, int N>
	class Section;

	template<typename T, int N>
	class Row;

	template<typename T, int N>
	class const_Row;

	template<typename T, int N>
	class Col;

	template<typename T, int N>
	class const_Col;

	template<typename T, int N>
	class Block;

	template<typename T, int N>
	class const_Block;

	template<typename T, int N, typename ownerT>
	class const_iterator;

	template<typename T, int N, typename ownerT>
	class iterator;

} // namespace Board_Section

} // namespace Sudoku
