//===--- Sudoku/Board.h                                                 ---===//
//
// Data container for sudoku puzzles
//===----------------------------------------------------------------------===//
//
// accepts only squared dimensions
// gives full, row, column and block access
// no processing functionality
//
//===----------------------------------------------------------------------===//
#pragma once

#include "Board_Iterators.h"
#include "Board_Section.h"
#include "Location.h"
#include "Location_Utilities.h"
#include "Size.h"
#include "exceptions.h"

#include <gsl/gsl>
#include <initializer_list>
#include <vector>
#include <cassert>

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku
{
template<typename T, int N = 3>
class Board
{
	static_assert(N > 1, "Board.h base_size value too small");
	static constexpr auto Size = size_t{full_size<N>};

	using Location    = ::Sudoku::Location<N>;
	using Row         = Board_Section::Row<T, N>;
	using const_Row   = Board_Section::const_Row<T, N>;
	using Col         = Board_Section::Col<T, N>;
	using const_Col   = Board_Section::const_Col<T, N>;
	using Block       = Board_Section::Block<T, N>;
	using const_Block = Board_Section::const_Block<T, N>;

public:
	using value_type             = T;
	using size_type              = size_t;
	using difference_type        = int;
	using reference              = value_type&;
	using const_reference        = value_type const&;
	using pointer                = value_type*;
	using const_pointer          = value_type const*;
	using iterator               = Board_iterator<T, N>;
	using const_iterator         = const_Board_iterator<T, N>;
	using reverse_iterator       = reverse_Board_iterator<T, N>;
	using const_reverse_iterator = const_reverse_Board_iterator<T, N>;

	Board() noexcept;
	// initialize with non-default value
	explicit Board(const T&);
	Board(std::initializer_list<T>); // construct from initializer_list

	void clear();

	// Query properties
	[[nodiscard]] static constexpr size_t size() noexcept { return Size; }
	[[nodiscard]] static constexpr size_t max_size() noexcept { return size(); }
	[[nodiscard]] static constexpr bool empty() noexcept { return false; }
	[[nodiscard]] bool operator==(const Board&) const;

	// Element access
	[[nodiscard]] T& front() noexcept { return board_[0]; }
	[[nodiscard]] const T& front() const noexcept { return board_[0]; }
	[[nodiscard]] T& back() noexcept { return board_[size() - 1]; }
	[[nodiscard]] const T& back() const noexcept { return board_[size() - 1]; }
	// Checked
	T& at(Location);
	const T& at(Location) const;
	T& at(int row, int col);
	const T& at(int row, int col) const;
	[[deprecated]] T& at(int elem);
	[[deprecated]] const T& at(int elem) const;
	// Unchecked
	T& operator[](Location) noexcept;
	const T& operator[](Location) const noexcept;

	// Element Selection Operator (using a proxy object)
	//   usable as [row][col] where col is processed by the (const_)Row
	Row operator[](int row_id) noexcept { return row(row_id); }
	const_Row operator[](int row_id) const noexcept { return row(row_id); }


	// Iterators
	constexpr iterator begin() noexcept;
	constexpr iterator end() noexcept;
	constexpr const_iterator cbegin() const noexcept;
	constexpr const_iterator cend() const noexcept;
	constexpr const_iterator begin() const noexcept { return cbegin(); }
	constexpr const_iterator end() const noexcept { return cend(); }
	constexpr reverse_iterator rbegin() noexcept;
	constexpr reverse_iterator rend() noexcept;
	constexpr const_reverse_iterator crbegin() const noexcept;
	constexpr const_reverse_iterator crend() const noexcept;
	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return crbegin();
	}
	constexpr const_reverse_iterator rend() const noexcept { return crend(); }

	// Sections
	// clang-format off
	Row         row(int id) noexcept         { return Row(*this, id); }
	const_Row   row(int id) const noexcept   { return const_Row(*this, id); }
	Row         row(Location loc) noexcept   { return Row(*this, loc); }
	const_Row   row(Location loc) const noexcept
											 { return const_Row(*this, loc); }
	Col         col(int id) noexcept         { return Col(*this, id); }
	const_Col   col(int id) const noexcept   { return const_Col(*this, id); }
	Col         col(Location loc) noexcept   { return Col(*this, loc); }
	const_Col   col(Location loc) const noexcept
											 { return const_Col(*this, loc); }
	Block       block(int id) noexcept       { return Block(*this, id); }
	const_Block block(int id) const noexcept { return const_Block(*this, id); }
	Block       block(Location loc) noexcept { return Block(*this, loc); }
	const_Block block(Location loc) const noexcept
											 { return const_Block(*this, loc); }
	// clang-format on

private:
	std::vector<T> board_{};

}; // class Board

//===--- free-functions ---------------------------------------------------===//

template<typename T, int N>
constexpr bool operator!=(Board<T, N> const& left, Board<T, N> const& right)
{
	return !(left == right);
}

//===----------------------------------------------------------------------===//
// Board - member-functions
//===----------------------------------------------------------------------===//
// Board - Constructors
template<typename T, int N>
Board<T, N>::Board() noexcept : board_(full_size<N>)
{
	valid_dimensions<N>();
	assert(board_.size() == size());
	assert(board_.capacity() == size());
}

template<typename T, int N>
Board<T, N>::Board(const T& def_value) : board_(full_size<N>, def_value)
{
	valid_dimensions<N>();
	assert(board_.size() == size());
	assert(board_.capacity() == size());
}

template<typename T, int N>
Board<T, N>::Board(std::initializer_list<T> list) : board_(list)
{
	assert(list.size() == size_t{full_size<N>});
	// TODO exception on invalid length
	valid_dimensions<N>();
	assert(board_.size() == size());
	assert(board_.capacity() == size());
}

//===----------------------------------------------------------------------===//
// properties
template<typename T, int N>
bool Board<T, N>::operator==(const Board& other) const
{
	return board_ == other.board_;
}

//===----------------------------------------------------------------------===//
// Board - Operations
template<typename T, int N>
inline void Board<T, N>::clear()
{ // all elements to the empty value
	board_.clear();
	board_.resize(size());
}

//===----------------------------------------------------------------------===//
// Board - element access
template<typename T, int N>
T& Board<T, N>::at(const Location loc)
{
	if (!is_valid<N>(loc))
	{
		throw error::invalid_Location{"Board::at(Location)"};
	}
	return board_[gsl::narrow_cast<size_t>(loc.element())];
}

template<typename T, int N>
const T& Board<T, N>::at(const Location loc) const
{
	if (!is_valid<N>(loc))
	{
		throw error::invalid_Location{"Board::at(Location) const"};
	}
	return board_[gsl::narrow_cast<size_t>(loc.element())];
}

template<typename T, int N>
T& Board<T, N>::at(const int row, const int col)
{
	if (!is_valid_size<N>(row, col))
	{
		throw error::invalid_Location{"Board::at(int row, col)"}; // <stdexcept>
	}
	return board_[gsl::narrow_cast<size_t>(Location(row, col).element())];
}

template<typename T, int N>
const T& Board<T, N>::at(const int row, const int col) const
{
	if (!is_valid_size<N>(row, col))
	{
		throw error::invalid_Location{"Board::at(row, col) const"};
	}
	return board_[gsl::narrow_cast<size_t>(Location(row, col).element())];
}

// deprecated
template<typename T, int N>
T& Board<T, N>::at(const int elem)
{
	if (!is_valid_size<N>(elem))
	{
		throw error::invalid_Location{"Board::at(int)"};
	}
	return board_.at(gsl::narrow_cast<size_t>(elem));
}

// deprecated
template<typename T, int N>
const T& Board<T, N>::at(const int elem) const
{
	if (!is_valid_size<N>(elem))
	{
		throw error::invalid_Location{"Board::at(int) const"};
	}
	return board_.at(gsl::narrow_cast<size_t>(elem));
}

template<typename T, int N>
T& Board<T, N>::operator[](Location loc) noexcept
{
	return board_[gsl::narrow_cast<size_t>(loc.element())];
}

template<typename T, int N>
const T& Board<T, N>::operator[](Location loc) const noexcept
{
	return board_[gsl::narrow_cast<size_t>(loc.element())];
}

//===----------------------------------------------------------------------===//
// Board - Iterators
template<typename T, int N>
constexpr typename Board<T, N>::iterator Board<T, N>::begin() noexcept
{
	return Board_iterator<T, N>(gsl::not_null<Board*>{this});
}
template<typename T, int N>
constexpr typename Board<T, N>::iterator Board<T, N>::end() noexcept
{
	return Board_iterator<T, N>(
		gsl::not_null<Board*>{this}, Location{full_size<N>});
}
template<typename T, int N>
constexpr typename Board<T, N>::const_iterator Board<T, N>::cbegin() const
	noexcept
{
	return const_Board_iterator<T, N>(gsl::not_null<Board const*>{this});
}
template<typename T, int N>
constexpr typename Board<T, N>::const_iterator Board<T, N>::cend() const
	noexcept
{
	return const_Board_iterator<T, N>(
		gsl::not_null<Board const*>{this}, Location{full_size<N>});
}
template<typename T, int N>
constexpr typename Board<T, N>::reverse_iterator Board<T, N>::rbegin() noexcept
{
	return reverse_Board_iterator<T, N>(gsl::not_null<Board*>{this});
}
template<typename T, int N>
constexpr typename Board<T, N>::reverse_iterator Board<T, N>::rend() noexcept
{
	return reverse_Board_iterator<T, N>(
		gsl::not_null<Board*>{this}, Location{-1});
}
template<typename T, int N>
constexpr typename Board<T, N>::const_reverse_iterator
	Board<T, N>::crbegin() const noexcept
{
	return const_reverse_Board_iterator<T, N>(
		gsl::not_null<Board const*>{this});
}
template<typename T, int N>
constexpr typename Board<T, N>::const_reverse_iterator
	Board<T, N>::crend() const noexcept
{
	return const_reverse_Board_iterator<T, N>(
		gsl::not_null<Board const*>{this}, Location{-1});
}

} // namespace Sudoku
