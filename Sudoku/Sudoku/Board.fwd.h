//====---- Sudoku/Board.fwd.h                                         ----====//
//
// Forward Declarations for class Board.
//====--------------------------------------------------------------------====//
// To separate implementation details of Board into multiple files without
// cyclic dependencies.
// Only #include into header files #include-ed into Board.h.
//====--------------------------------------------------------------------====//
#pragma once

namespace Sudoku
{
template<typename T, int N>
class Board;

namespace Board_Section
{
	enum class Section { row, col, block };

	template<typename T, int N, Section, bool is_const>
	class Board_Section_;
	template<typename T, int N>
	using Row = Board_Section_<T, N, Section::row, false>;
	template<typename T, int N>
	using const_Row = Board_Section_<T, N, Section::row, true>;
	template<typename T, int N>
	using Col = Board_Section_<T, N, Section::col, false>;
	template<typename T, int N>
	using const_Col = Board_Section_<T, N, Section::col, true>;
	template<typename T, int N>
	using Block = Board_Section_<T, N, Section::block, false>;
	template<typename T, int N>
	using const_Block = Board_Section_<T, N, Section::block, true>;
} // namespace Board_Section
} // namespace Sudoku
