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

#include "Board/Iterators.h"
#include "Board/Section.h"
#include "Location.h"
#include "Location_Utilities.h"
#include "Size.h"
#include "exceptions.h"

#include <gsl/gsl>

#include <array>
#include <initializer_list>

#include <algorithm>

#include "Board/Board.fwd.h" // Forward declarations

#include <cassert>


namespace Sudoku
{
template<typename T, int N = 3>
class Board
{
	static_assert(N > 1, "Board.h base_size value too small");
	static constexpr auto Size = size_t{full_size<N>};

	using index       = gsl::index;
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
	using difference_type        = std::ptrdiff_t;
	using reference              = value_type&;
	using const_reference        = value_type const&;
	using pointer                = value_type*;
	using const_pointer          = value_type const*;
	using iterator               = Board_iterator<T, N>;
	using const_iterator         = const_Board_iterator<T, N>;
	using reverse_iterator       = reverse_Board_iterator<T, N>;
	using const_reverse_iterator = const_reverse_Board_iterator<T, N>;

	constexpr Board() noexcept;
	explicit constexpr Board(const T& default_value);
	// NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
	constexpr Board(std::array<T, Size> const& list) // NOLINT(runtime/explicit)
		: board_(list)
	{
	}
	Board(std::initializer_list<T>); // construct from initializer_list

	void clear();

	// Query properties
	[[nodiscard]] static constexpr size_t size() noexcept { return Size; }
	[[nodiscard]] static constexpr size_t max_size() noexcept { return size(); }
	[[nodiscard]] static constexpr bool empty() noexcept { return false; }
	[[nodiscard]] bool operator==(const Board&) const = default;

	// Element access
	[[nodiscard]] constexpr T& front() noexcept { return board_.front(); }
	[[nodiscard]] constexpr T const& front() const noexcept
	{
		return board_.front();
	}
	[[nodiscard]] constexpr T& back() noexcept { return board_.back(); }
	[[nodiscard]] constexpr T const& back() const noexcept
	{
		return board_.back();
	}
	// Checked
	constexpr T& at(Location);
	[[nodiscard]] constexpr T const& at(Location) const;
	constexpr T& at(index row, index col);
	[[nodiscard]] constexpr T const& at(index row, index col) const;
	[[deprecated]] T& at(index elem);
	[[deprecated, nodiscard]] const T& at(index elem) const;
	// Unchecked
	constexpr T& operator[](Location) noexcept;
	constexpr T const& operator[](Location) const noexcept;

	// Element Selection Operator (using a proxy object)
	//   usable as [row][col] where col is processed by the (const_)Row
	Row operator[](index row_id) noexcept { return row(row_id); }
	constexpr const_Row operator[](index row_id) const noexcept
	{
		return row(row_id);
	}

	// Iterators
	constexpr iterator begin() noexcept;
	constexpr iterator end() noexcept;
	[[nodiscard]] constexpr const_iterator cbegin() const noexcept;
	[[nodiscard]] constexpr const_iterator cend() const noexcept;
	[[nodiscard]] constexpr const_iterator begin() const noexcept
	{
		return cbegin();
	}
	[[nodiscard]] constexpr const_iterator end() const noexcept
	{
		return cend();
	}
	constexpr reverse_iterator rbegin() noexcept;
	constexpr reverse_iterator rend() noexcept;
	[[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept;
	[[nodiscard]] constexpr const_reverse_iterator crend() const noexcept;
	[[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
	{
		return crbegin();
	}
	[[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
	{
		return crend();
	}

	// Sections
	// clang-format off
	Row row(index id) noexcept         { return Row(*this, id); }
	[[nodiscard]] constexpr const_Row   row(index id) const noexcept
									   { return const_Row(*this, id); }
	Row row(Location loc) noexcept     { return Row(*this, loc); }
	[[nodiscard]] constexpr const_Row   row(Location loc) const noexcept
									   { return const_Row(*this, loc); }
	Col col(index id) noexcept         { return Col(*this, id); }
	[[nodiscard]] constexpr const_Col   col(index id) const noexcept
									   { return const_Col(*this, id); }
	Col col(Location loc) noexcept     { return Col(*this, loc); }
	[[nodiscard]] constexpr const_Col   col(Location loc) const noexcept
									   { return const_Col(*this, loc); }
	Block block(index id) noexcept     { return Block(*this, id); }
	[[nodiscard]] constexpr const_Block block(index id) const noexcept
									   { return const_Block(*this, id); }
	Block block(Location loc) noexcept { return Block(*this, loc); }
	[[nodiscard]] constexpr const_Block block(Location loc) const noexcept
									   { return const_Block(*this, loc); }
	// clang-format on

private:
	std::array<T, Size> board_{};
}; // class Board

//===----------------------------------------------------------------------===//
// Board - member-functions
//===----------------------------------------------------------------------===//
// Board - Constructors
template<typename T, int N>
constexpr Board<T, N>::Board() noexcept
{
	valid_dimensions<N>();
}

template<typename T, int N>
constexpr Board<T, N>::Board(const T& default_value)
{
	valid_dimensions<N>();
	board_.fill(default_value);
}

template<typename T, int N>
Board<T, N>::Board(std::initializer_list<T> list)
{
	valid_dimensions<N>();
	if (list.size() != size_t{full_size<N>})
		throw std::length_error{"Invalid length initializer_list"};
	std::copy(std::begin(list), std::end(list), std::begin(board_));
}

//===----------------------------------------------------------------------===//
// Board - Operations
template<typename T, int N>
inline void Board<T, N>::clear()
{ // all elements to the empty value
	board_.fill(T{});
}

//===----------------------------------------------------------------------===//
// Board - element access
template<typename T, int N>
constexpr T& Board<T, N>::at(const Location loc)
{
	if (!is_valid<N>(loc))
	{
		throw error::invalid_Location{"Board::at(Location)"};
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
	return board_[gsl::narrow_cast<size_t>(loc.element())];
}

template<typename T, int N>
constexpr T const& Board<T, N>::at(const Location loc) const
{
	if (!is_valid<N>(loc))
	{
		throw error::invalid_Location{"Board::at(Location) const"};
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
	return board_[gsl::narrow_cast<size_t>(loc.element())];
}

template<typename T, int N>
constexpr T& Board<T, N>::at(const index row, const index col)
{
	if (!is_valid_size<N>(row, col))
	{
		throw error::invalid_Location{"Board::at(int row, col)"}; // <stdexcept>
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
	return board_[gsl::narrow_cast<size_t>(Location(row, col).element())];
}

template<typename T, int N>
constexpr T const& Board<T, N>::at(const index row, const index col) const
{
	if (!is_valid_size<N>(row, col))
	{
		throw error::invalid_Location{"Board::at(row, col) const"};
	}
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
	return board_[gsl::narrow_cast<size_t>(Location(row, col).element())];
}

// deprecated
template<typename T, int N>
T& Board<T, N>::at(const index elem)
{
	if (!is_valid_size<N>(elem))
	{
		throw error::invalid_Location{"Board::at(int)"};
	}
	return board_.at(gsl::narrow_cast<size_t>(elem));
}

// deprecated
template<typename T, int N>
const T& Board<T, N>::at(const index elem) const
{
	if (!is_valid_size<N>(elem))
	{
		throw error::invalid_Location{"Board::at(int) const"};
	}
	return board_.at(gsl::narrow_cast<size_t>(elem));
}

template<typename T, int N>
constexpr T& Board<T, N>::operator[](const Location loc) noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
	return board_[gsl::narrow_cast<size_t>(loc.element())];
}

template<typename T, int N>
constexpr T const& Board<T, N>::operator[](const Location loc) const noexcept
{
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
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
constexpr typename Board<T, N>::const_iterator
	Board<T, N>::cbegin() const noexcept
{
	return const_Board_iterator<T, N>(gsl::not_null<Board const*>{this});
}
template<typename T, int N>
constexpr typename Board<T, N>::const_iterator
	Board<T, N>::cend() const noexcept
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
