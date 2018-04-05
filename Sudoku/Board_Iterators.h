//====---- Sudoku/Board_Iterators.h                                   ----====//
//
//	Location aware iterator implementation for Board
//====--------------------------------------------------------------------====//
//
// The iterator can return a Location object.
// included by Board.h
//
//====--------------------------------------------------------------------====//
#pragma once

#include "Location.h"
#include <gsl/gsl>
#include <iterator>
#include <type_traits>

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku
{
template<typename T, int N, bool is_const = false, bool is_reverse = false>
class Board_iterator
{
	using owner_type =
		std::conditional_t<is_const, Board<T, N> const, Board<T, N>>;
	using Location = ::Sudoku::Location<N>;

public:
	// member types
	using difference_type   = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = std::conditional_t<is_const, T const*, T*>;
	using reference         = std::conditional_t<is_const, T const&, T&>;
	using iterator_category = std::random_access_iterator_tag;
	using _Unchecked_type   = pointer; // MSVC STL implementation specific

	// Constructors
	constexpr Board_iterator() noexcept
	{ // defaults to begin()
		if constexpr (is_reverse) elem_ = full_size<N> - 1;
	}
	explicit constexpr Board_iterator(gsl::not_null<owner_type*> owner) noexcept
		: board_(owner)
	{ // defaults to begin()
		if constexpr (is_reverse) elem_ = (full_size<N> - 1);
	}
	explicit constexpr Board_iterator(
		gsl::not_null<owner_type*> owner, Location loc) noexcept
		: board_(owner), elem_(loc.element())
	{
	}
	// Assignment
	constexpr Board_iterator& operator=(Location const loc) noexcept
	{
		assert(is_valid(loc));
		elem_ = loc.element();
		return (*this);
	}

	//====----------------------------------------------------------------====//
	[[nodiscard]] constexpr Location location() const noexcept
	{
		return Location{gsl::narrow_cast<int>(elem_)};
	}
	// Allows for conversion to Location
	explicit constexpr operator Location() const noexcept { return location(); }

	//====----------------------------------------------------------------====//
	[[nodiscard]] constexpr reference operator*() const noexcept
	{
		assert(board_ != nullptr && dereferenceable_location());
		return board_->operator[](location());
	}
	[[nodiscard]] constexpr pointer operator->() const noexcept
	{ // member access; equivalent to (*p).member
		return std::pointer_traits<pointer>::pointer_to(**this);
	}

	//====----------------------------------------------------------------====//
	constexpr Board_iterator& operator++() noexcept
	{ // pre-increment
		assert(board_ != nullptr);
		if constexpr (is_reverse)
		{
			assert(elem_ >= 0);
			--elem_;
		}
		else
		{
			assert(elem_ < full_size<N>);
			++elem_;
		}
		return (*this);
	}
	constexpr Board_iterator operator++(int) noexcept
	{ // post-increment
		const Board_iterator pre{*this};
		operator++();
		return pre;
	}
	constexpr Board_iterator& operator--() noexcept
	{ // pre-decrement
		assert(board_ != nullptr);
		if constexpr (is_reverse)
		{
			assert(elem_ < full_size<N> - 1);
			++elem_;
		}
		else
		{
			assert(elem_ > 0);
			--elem_;
		}
		return (*this);
	}
	constexpr Board_iterator operator--(int) noexcept
	{ // post-decrement
		const Board_iterator pre{*this};
		operator--();
		return pre;
	}

	//====----------------------------------------------------------------====//
	constexpr Board_iterator& operator+=(const difference_type offset) noexcept
	{
		assert(offset == 0 || board_ != nullptr);
		if constexpr (is_reverse)
		{
			elem_ -= offset;
			assert(elem_ >= -1);
			assert(elem_ < full_size<N>);
		}
		else
		{
			elem_ += offset;
			assert(elem_ >= 0);
			assert(elem_ <= full_size<N>);
		}
		return (*this);
	}
	constexpr Board_iterator& operator-=(const difference_type offset) noexcept
	{
		return operator+=(-offset);
	}
	[[nodiscard]] friend constexpr difference_type operator-(
		Board_iterator const left, Board_iterator const right) noexcept
	{ // difference
		assert(left.is_same_address(right));
		if constexpr (is_reverse)
			return right.elem_ - left.elem_;
		else
			return left.elem_ - right.elem_;
	}

	[[nodiscard]] constexpr reference
		operator[](const difference_type offset) const noexcept
	{
		return (*(*this + offset));
	}

	//====----------------------------------------------------------------====//
	[[nodiscard]] friend constexpr bool operator==(
		Board_iterator const left, Board_iterator const right) noexcept
	{
		assert(left.is_same_address(right));
		return left.is_same_address(right) && left.elem_ == right.elem_;
	}
	[[nodiscard]] friend constexpr bool operator<(
		Board_iterator const left, Board_iterator const right) noexcept
	{
		assert(left.is_same_address(right));
		if constexpr (is_reverse)
			return left.elem_ > right.elem_;
		else
			return left.elem_ < right.elem_;
	}

private:
	owner_type* board_{nullptr};
	difference_type elem_{0};

	constexpr bool is_same_address(const Board_iterator other) const noexcept
	{ // compare address of:
		return board_ == other.board_;
	}

	// Dereferenceable locations
	static constexpr bool
		dereferenceable_location(const difference_type x) noexcept
	{
		return !(x < 0 || x >= full_size<N>);
	}
	constexpr bool dereferenceable_location() const noexcept
	{
		return dereferenceable_location(elem_);
	}
};

template<typename T, int N>
using const_Board_iterator = Board_iterator<T, N, true, false>;

template<typename T, int N>
using reverse_Board_iterator = Board_iterator<T, N, false, true>;

template<typename T, int N>
using const_reverse_Board_iterator = Board_iterator<T, N, true, true>;

//====--------------------------------------------------------------------====//

template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] constexpr auto operator+(
	Board_iterator<T, N, is_const, is_reverse> left,
	typename Board_iterator<T, N, is_const, is_reverse>::difference_type const
		offset) noexcept
{
	return (left += offset);
}
template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] constexpr auto operator-(
	Board_iterator<T, N, is_const, is_reverse> left,
	typename Board_iterator<T, N, is_const, is_reverse>::difference_type const
		offset) noexcept
{
	return (left += -offset);
}

//====--------------------------------------------------------------------====//

template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] inline constexpr bool operator!=(
	Board_iterator<T, N, is_const, is_reverse> const left,
	Board_iterator<T, N, is_const, is_reverse> const right) noexcept
{
	return !(left == right);
}
template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] inline constexpr bool operator>(
	Board_iterator<T, N, is_const, is_reverse> const left,
	Board_iterator<T, N, is_const, is_reverse> const right) noexcept
{
	return (right < left);
}
template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] inline constexpr bool operator<=(
	Board_iterator<T, N, is_const, is_reverse> const left,
	Board_iterator<T, N, is_const, is_reverse> const right) noexcept
{
	return !(right < left);
}
template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] inline constexpr bool operator>=(
	Board_iterator<T, N, is_const, is_reverse> const left,
	Board_iterator<T, N, is_const, is_reverse> const right) noexcept
{
	return !(left < right);
}

//====--------------------------------------------------------------------====//

template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] inline constexpr auto operator+(
	typename Board_iterator<T, N, is_const, is_reverse>::difference_type const
		offset,
	Board_iterator<T, N, is_const, is_reverse> itr) noexcept
{
	return (itr += offset);
}

} // namespace Sudoku
