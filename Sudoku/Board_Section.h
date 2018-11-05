//====---- Sudoku/Board_Section.h                                     ----====//
//
// Internal in-between object for provide access to a section of Board.
//====--------------------------------------------------------------------====//
#pragma once

#include "Board_Section_iterator.h"
#include "Location.h"
#include "Location_Utilities.h"
#include "exceptions.h"
#include "traits.h"
#include <gsl/gsl> // index

#include <type_traits>
#include <cassert>

// Convenience include
#include "Board_Section_traits.h"

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku::Board_Section
{
template<typename T, int N, Section S, bool is_const>
class [[nodiscard]] Board_Section_
{
	using index          = gsl::index;
	using Location       = ::Sudoku::Location<N>;
	using Location_Block = ::Sudoku::Location_Block<N>;
	using OwnerT =
		std::conditional_t<is_const, Board<T, N> const&, Board<T, N>&>;

	// explicit friends for conversion
	friend class Board_Section_<T, N, Section::row, true>;
	friend class Board_Section_<T, N, Section::row, false>;
	friend class Board_Section_<T, N, Section::col, true>;
	friend class Board_Section_<T, N, Section::col, false>;
	friend class Board_Section_<T, N, Section::block, true>;
	friend class Board_Section_<T, N, Section::block, false>;

public:
	using value_type             = T;
	using pointer                = std::conditional_t<is_const, T const*, T*>;
	using reference              = std::conditional_t<is_const, T const&, T&>;
	using iterator               = Section_iterator<T, N, S, is_const, false>;
	using const_iterator         = Section_iterator<T, N, S, true, false>;
	using reverse_iterator       = Section_iterator<T, N, S, is_const, true>;
	using const_reverse_iterator = Section_iterator<T, N, S, true, true>;

	template<typename idT, typename = std::enable_if_t<is_int_v<idT>>>
	constexpr Board_Section_(OwnerT board, idT id) noexcept
		: board_(board), id_(id)
	{
		assert(is_valid_size<N>(id));
	}
	constexpr Board_Section_(OwnerT board, Location loc) noexcept
		: board_(board), id_(to_id(loc))
	{
		assert(is_valid<N>(loc));
	}
	// Conversion to other Section type (maintaining or gaining const)
	template<
		Section Sx,
		bool B,
		typename = std::enable_if_t<!B || B == is_const>>
	constexpr Board_Section_(
		Board_Section_<T, N, Sx, B> other, index pivot_elem) noexcept
		: board_(other.board_), id_(to_id(other.location(pivot_elem)))
	{
	}
	// Implicit conversion to const
	constexpr operator Board_Section_<T, N, S, true>() const noexcept
	{
		return Board_Section_<T, N, S, true>(board_, id_);
	}

	[[nodiscard]] static constexpr int size() noexcept { return elem_size<N>; }
	[[nodiscard]] constexpr index id() const noexcept { return id_; }
	[[nodiscard]] constexpr Location location(index elem) const noexcept;

	[[nodiscard]] constexpr reference front() noexcept { return (*this)[0]; }
	[[nodiscard]] constexpr reference back() noexcept
	{
		return (*this)[size() - 1];
	}
	[[nodiscard]] constexpr reference operator[](const index elem) noexcept
	{
		return board_[location(elem)];
	}
	[[nodiscard]] constexpr T const& operator[](const index elem) const noexcept
	{
		return board_[location(elem)];
	}
	// Checked access
	[[nodiscard]] constexpr reference at(const index elem);

	constexpr iterator begin() noexcept { return iterator(&board_, id_, 0); }
	constexpr iterator end() noexcept { return iterator(&board_, id_, size()); }
	constexpr const_iterator cbegin() const noexcept
	{
		return const_iterator(&board_, id_, 0);
	}
	constexpr const_iterator cend() const noexcept
	{
		return const_iterator(&board_, id_, size());
	}
	constexpr const_iterator begin() const noexcept { return cbegin(); }
	constexpr const_iterator end() const noexcept { return cend(); }
	constexpr reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(&board_, id_, size() - 1);
	}
	constexpr reverse_iterator rend() noexcept
	{
		return reverse_iterator(&board_, id_, -1);
	}
	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(&board_, id_, size() - 1);
	}
	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(&board_, id_, -1);
	}
	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return crbegin();
	}
	constexpr const_reverse_iterator rend() const noexcept { return crend(); }

private:
	OwnerT board_;
	const gsl::index id_{};

	// Internal helper
	static constexpr gsl::index to_id(Location loc) noexcept;
};

//====--------------------------------------------------------------------====//
// Member Functions

// Checked access
template<typename T, int N, Section S, bool is_const>
[[nodiscard]] inline constexpr
	typename Board_Section_<T, N, S, is_const>::reference
	Board_Section_<T, N, S, is_const>::at(const index elem)
{
	if (!is_valid_size<N>(elem))
	{
		throw error::invalid_Location{"Board_Section::at(elem)"};
	}
	return board_.at(location(elem));
}

template<typename T, int N, Section S, bool is_const>
[[nodiscard]] constexpr Location<N>
	Board_Section_<T, N, S, is_const>::location(index elem) const noexcept
{
	assert(is_valid_size<N>(elem));
	switch (S)
	{
	case Section::row: return Location(id_, elem);
	case Section::col: return Location(elem, id_);
	case Section::block: return Location_Block(id_, elem);
	}
}

// Internal helper
template<typename T, int N, Section S, bool is_const>
inline constexpr gsl::index
	Board_Section_<T, N, S, is_const>::to_id(Location loc) noexcept
{
	switch (S)
	{
	case Section::row: return loc.row();
	case Section::col: return loc.col();
	case Section::block: return loc.block();
	}
}

} // namespace Sudoku::Board_Section
