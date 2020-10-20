//====---- Sudoku/Board_Section_traits.h                              ----====//
//
// Type traits for Board_Section classes.
//====--------------------------------------------------------------------====//
#pragma once

#include <type_traits>

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku::Board_Section
{
inline namespace Section_traits
{
	template<typename T>
	struct is_Section : std::false_type
	{
	};
	template<typename T, int N, Section S, bool C>
	struct is_Section<Board_Section_<T, N, S, C>> : std::true_type
	{
	};
	template<typename T>
	inline static constexpr bool is_Section_v = is_Section<T>::value;

	template<typename T>
	struct is_const_Section : std::false_type
	{
	};
	template<typename T, int N, Section S, bool is_const>
	struct is_const_Section<Board_Section_<T, N, S, is_const>>
		: std::integral_constant<bool, is_const>
	{
	};
	template<typename T>
	inline static constexpr bool is_const_Section_v =
		is_const_Section<T>::value;

	template<typename T>
	struct is_const_Row : std::false_type
	{
	};
	template<typename T, int N>
	struct is_const_Row<const_Row<T, N>> : std::true_type
	{
	};

	template<typename T>
	struct is_Row : std::false_type
	{
	};
	template<typename T, int N>
	struct is_Row<Row<T, N>> : std::true_type
	{
	};
	template<typename T, int N>
	struct is_Row<const_Row<T, N>> : std::true_type
	{
	};
	template<typename T>
	inline static constexpr bool is_Row_v = is_Row<T>::value;

	template<typename T>
	struct is_const_Col : std::false_type
	{
	};
	template<typename T, int N>
	struct is_const_Col<const_Col<T, N>> : std::true_type
	{
	};

	template<typename T>
	struct is_Col : std::false_type
	{
	};
	template<typename T, int N>
	struct is_Col<Col<T, N>> : std::true_type
	{
	};
	template<typename T, int N>
	struct is_Col<const_Col<T, N>> : std::true_type
	{
	};
	template<typename T>
	inline static constexpr bool is_Col_v = is_Col<T>::value;

	template<typename T>
	struct is_const_Block : std::false_type
	{
	};
	template<typename T, int N>
	struct is_const_Block<const_Block<T, N>> : std::true_type
	{
	};

	template<typename T>
	struct is_Block : std::false_type
	{
	};
	template<typename T, int N>
	struct is_Block<Block<T, N>> : std::true_type
	{
	};
	template<typename T, int N>
	struct is_Block<const_Block<T, N>> : std::true_type
	{
	};
	template<typename T>
	inline static constexpr bool is_Block_v = is_Block<T>::value;
} // namespace Section_traits
} // namespace Sudoku::Board_Section
