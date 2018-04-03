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
	using self_type = Board_iterator;
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

	using _Unchecked_type = pointer; // MSVC STL implementation specific

	// Constructors
	constexpr Board_iterator() noexcept = default;
	explicit constexpr Board_iterator(gsl::not_null<owner_type*> owner) noexcept
		: board_(owner)
	{
	}
	explicit constexpr Board_iterator(
		gsl::not_null<owner_type*> owner, Location loc) noexcept
		: board_(owner), elem_(loc.element())
	{
	}
	// Assignment
	constexpr Board_iterator& operator=(Location loc) noexcept
	{
		assert(is_valid(loc));
		elem_ = loc.element();
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
	constexpr self_type& operator++() noexcept
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
	constexpr self_type operator++(int) noexcept
	{ // post-increment
		const self_type pre{*this};
		operator++();
		return pre;
	}
	constexpr self_type& operator--() noexcept
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
	constexpr self_type operator--(int) noexcept
	{ // post-decrement
		const self_type pre{*this};
		operator--();
		return pre;
	}

	//====----------------------------------------------------------------====//
	constexpr self_type& operator+=(const difference_type offset) noexcept
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
	[[nodiscard]] constexpr self_type
		operator+(const difference_type offset) const noexcept
	{
		self_type tmp{*this};
		return (tmp += offset);
	}
	constexpr self_type& operator-=(const difference_type offset) noexcept
	{
		return operator+=(-offset);
	}
	[[nodiscard]] constexpr self_type
		operator-(const difference_type offset) const noexcept
	{
		self_type tmp{*this};
		return (tmp += -offset);
	}

	[[nodiscard]] constexpr difference_type
		operator-(const self_type& other) const noexcept
	{ // difference
		assert(is_same_address(other));
		if constexpr (is_reverse)
			return other.elem_ - elem_;
		else
			return elem_ - other.elem_;
	}

	[[nodiscard]] constexpr reference
		operator[](const difference_type offset) const noexcept
	{
		return (*(*this + offset));
	}

	//====----------------------------------------------------------------====//
	[[nodiscard]] constexpr bool operator==(const self_type& other) const
		noexcept
	{
		assert(is_same_address(other));
		return is_same_address(other) && elem_ == other.elem_;
	}
	[[nodiscard]] constexpr bool operator!=(const self_type& other) const
		noexcept
	{
		return (!operator==(other));
	}
	[[nodiscard]] constexpr bool operator<(const self_type& other) const
		noexcept
	{
		assert(is_same_address(other));
		if constexpr (is_reverse)
			return elem_ > other.elem_;
		else
			return elem_ < other.elem_;
	}
	[[nodiscard]] constexpr bool operator>(const self_type& other) const
		noexcept
	{
		return (other < *this);
	}
	[[nodiscard]] constexpr bool operator<=(const self_type& other) const
		noexcept
	{
		return (!(other < *this));
	}
	[[nodiscard]] constexpr bool operator>=(const self_type& other) const
		noexcept
	{
		return (!(*this < other));
	}

private:
	owner_type* board_{nullptr};
	difference_type elem_{0};

	constexpr bool is_same_address(const self_type& other) const noexcept
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


template<typename T, int N, bool is_const = false, bool is_reverse = false>
[[nodiscard]] inline constexpr Board_iterator<T, N, is_const, is_reverse>
	operator+(
		typename Board_iterator<T, N, is_const>::difference_type const offset,
		Board_iterator<T, N, is_const, is_reverse> itr) noexcept
{
	return (itr += offset);
}

} // namespace Sudoku
