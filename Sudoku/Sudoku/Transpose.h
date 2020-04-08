//===--- Sudoku/Transpose.h                                             ---===//
//
// Exchange/reorder Board by section types
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#pragma once

#include "Board.h"
#include "Location.h"
#include "Size.h"

#include <type_traits>
#include <utility> // std::move // std::swap


namespace Sudoku
{
// Reorder Board elements so columns are rows.
template<typename T, int N>
Board<T, N> transpose_row_col(const Board<T, N>& input) noexcept(
#if defined(__ICL) && __ICL <= 1900
	false)
#else
	std::is_nothrow_move_constructible_v<Board<T, N>>&&
		std::is_nothrow_swappable_v<T>)
#endif // __ICL
{
	if constexpr (
#if defined(__ICL) && __ICL <= 1900
		false)
#else
		std::is_nothrow_move_constructible_v<Board<T, N>> &&
		std::is_nothrow_swappable_v<T>)
#endif // __ICL
	{
		Board<T, N> result{input};
		return transpose_row_col(std::move(result));
	}
	else
	{
		Board<T, N> result{};
		for (gsl::index i{}; i < elem_size<N>; ++i)
		{
			for (gsl::index j{}; j < elem_size<N>; ++j)
			{
				result[Location<N>{i, j}] = input[Location<N>{j, i}];
			}
		}
		return result;
	}
}

// Reorder Board elements so blocks are rows.
template<typename T, int N>
Board<T, N> transpose_row_block(const Board<T, N>& input) noexcept(
#if defined(__ICL) && __ICL <= 1900
	false)
#else
	std::is_nothrow_move_constructible_v<Board<T, N>>&&
		std::is_nothrow_swappable_v<T>)
#endif // __ICL
{
	if constexpr (
#if defined(__ICL) && __ICL <= 1900
		false)
#else
		std::is_nothrow_move_constructible_v<Board<T, N>> &&
		std::is_nothrow_swappable_v<T>)
#endif // __ICL
	{
		Board<T, N> result{input};
		return transpose_row_block(std::move(result));
	}
	else
	{
		Board<T, N> result{};
		for (gsl::index section{}; section < elem_size<N>; ++section)
		{
			for (gsl::index elem{}; elem < elem_size<N>; ++elem)
			{
				result[Location<N>{section, elem}] =
					input[Location_Block<N>{section, elem}];
			}
		}
		return result;
	}
}

// Reorder Board elements so columns are rows.
template<typename T, int N>
Board<T, N> transpose_row_col(Board<T, N>&& board) noexcept(
#if defined(__ICL) && __ICL <= 1900
	false)
#else
	std::is_nothrow_swappable_v<T>)
#endif // __ICL
{
#if not(defined(__ICL) && __ICL <= 1900)
	static_assert(std::is_swappable_v<T>);
#endif // __ICL

	for (gsl::index i{0}; i < elem_size<N>; ++i)
	{
		for (gsl::index j{i + 1}; j < elem_size<N>; ++j)
		{
			std::swap(board[Location<N>{i, j}], board[Location<N>{j, i}]);
		}
	}
	return std::move(board);
}

// Reorder Board elements so blocks are rows.
template<typename T, int N>
Board<T, N> transpose_row_block(Board<T, N>&& board) noexcept(
#if defined(__ICL) && __ICL <= 1900
	false)
#else
	std::is_nothrow_swappable_v<T>)
#endif // __ICL
{
	for (int i{0}; i < elem_size<N>; ++i)
	{
		const auto start = [](int x) constexpr
		{
			return (x % base_size<N> + 1) * base_size<N>;
		};
		for (gsl::index j = start(i); j < elem_size<N>; ++j)
		{
			std::swap(board[Location_Block<N>{i, j}], board[Location<N>{i, j}]);
		}
	}
	return std::move(board);
}


} // namespace Sudoku
