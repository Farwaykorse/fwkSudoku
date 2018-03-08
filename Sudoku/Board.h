//===--- Sudoku/Board.h                                                 ---===//
//
// Data container for sudoku puzzles
//===----------------------------------------------------------------------===//
//
// accepts only squared dimensions
// gives full, row, collumn and block access
// no processing functionality
//
//===----------------------------------------------------------------------===//
#pragma once

#include "Board_Sections.h"
#include "Board_Utilities.h"
#include "Location.h"
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

	using Location    = Location<N>;
	using Section     = Board_Section::Section<T, N>;
	using Row         = Board_Section::Row<T, N>;
	using const_Row   = Board_Section::const_Row<T, N>;
	using Col         = Board_Section::Col<T, N>;
	using const_Col   = Board_Section::const_Col<T, N>;
	using Block       = Board_Section::Block<T, N>;
	using const_Block = Board_Section::const_Block<T, N>;

public:
	Board() noexcept;
	// initialize with non-default value
	explicit Board(const T&);
	Board(std::initializer_list<T>); // construct from initializer_list

	void clear();

	// Query properties
	constexpr size_t size() const noexcept { return full_size<N>; }
	bool operator==(const Board&) const;

	// Element access
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
	//   usable as [row][col] where col is processed by the (const_)InBetween
	class InBetween;
	InBetween operator[](int row) noexcept;
	class const_InBetween;
	const const_InBetween operator[](int row) const noexcept;

	// Iterators
	// inherrit all from internal
	using iterator         = typename std::vector<T>::iterator;
	using const_iterator   = typename std::vector<T>::const_iterator;
	using reverse_iterator = typename std::vector<T>::reverse_iterator;
	using const_reverse_iterator =
		typename std::vector<T>::const_reverse_iterator;

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

//===----------------------------------------------------------------------===//
// Board - memberfunctions
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
	assert(list.size() == full_size<N>);
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
	board_.resize(full_size<N>);
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
// Proxy object for Element Selection Operator
template<typename T, int N>
class Board<T, N>::InBetween
{
	friend class Board<T, N>; // access to private constructor
	InBetween(gsl::not_null<Board<T, N>*> owner, int row)
		: owner_(owner), row_(row)
	{ // private constructor prevents creation out of class
	}

public:
	T& operator[](const int col) noexcept
	{
		return owner_->operator[](Location(row_, col));
	}

private:
	Board<T, N>* const owner_; // const-pointer
	const int row_;
};

// Proxy object for Element Selection Operator
template<typename T, int N>
class Board<T, N>::const_InBetween
{
	friend class Board<T, N>; // access to private constructor
	const_InBetween(gsl::not_null<const Board<T, N>*> owner, int row) noexcept
		: owner_(owner), row_(row)
	{ // private constructor prevents creation out of class
	}

public:
	const T& operator[](const int col) const noexcept
	{
		return owner_->operator[](Location(row_, col));
	}

private:
	const Board<T, N>* const owner_; // const pointer-to-const
	const int row_;
};

// Element Selection Operator (using a proxy object)
//   usable as [row][col] where col is processed by the (const_)InBetween
template<typename T, int N>
typename Board<T, N>::InBetween Board<T, N>::operator[](const int row) noexcept
{
	// Element Selection Operator (using a proxy object)
	return InBetween(this, row);
}

// Element Selection Operator (using a proxy object)
//   usable as [row][col] where col is processed by the (const_)InBetween
template<typename T, int N>
const typename Board<T, N>::const_InBetween Board<T, N>::
	operator[](const int row) const noexcept
{
	return const_InBetween(this, row);
}
// InBetween

//===----------------------------------------------------------------------===//
// Board - Iterators
template<typename T, int N>
constexpr typename Board<T, N>::iterator Board<T, N>::begin() noexcept
{
	return board_.begin();
}
template<typename T, int N>
constexpr typename Board<T, N>::iterator Board<T, N>::end() noexcept
{
	return board_.end();
}
template<typename T, int N>
constexpr typename Board<T, N>::const_iterator Board<T, N>::cbegin() const
	noexcept
{
	return board_.cbegin();
}
template<typename T, int N>
constexpr typename Board<T, N>::const_iterator Board<T, N>::cend() const
	noexcept
{
	return board_.cend();
}
template<typename T, int N>
constexpr typename Board<T, N>::reverse_iterator Board<T, N>::rbegin() noexcept
{
	return board_.rbegin();
}
template<typename T, int N>
constexpr typename Board<T, N>::reverse_iterator Board<T, N>::rend() noexcept
{
	return board_.rend();
}
template<typename T, int N>
constexpr typename Board<T, N>::const_reverse_iterator
	Board<T, N>::crbegin() const noexcept
{
	return board_.crbegin();
}
template<typename T, int N>
constexpr typename Board<T, N>::const_reverse_iterator
	Board<T, N>::crend() const noexcept
{
	return board_.crend();
}

} // namespace Sudoku
