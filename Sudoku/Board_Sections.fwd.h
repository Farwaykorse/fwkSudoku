/**	Forward declaration of Board_Sections.h
 */
#pragma once

namespace Sudoku::Board_Section
{

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


}	// namespace Sudoku::Baord_Section
