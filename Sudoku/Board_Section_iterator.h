//====---- Sudoku/Board_Section_iterator.h                            ----====//
//
// Location aware iterator implementation for Sections
//====--------------------------------------------------------------------====//
//
// The iterator can return a Location object.
// Included by Board_Section.h
//
//====--------------------------------------------------------------------====//
#pragma once

#include "Location.h"
#include "Location_Utilities.h"
#include "traits.h"

#include <iterator>
#include <type_traits>
#include <cassert>

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku::Board_Section
{
template<typename T, int N, Section S, bool is_const, bool is_reverse>
class Section_iterator;

//====--- Aliases --------------------------------------------------------====//
template<typename T, int N>
using Row_iterator = Section_iterator<T, N, Section::row, false, false>;
template<typename T, int N>
using Col_iterator = Section_iterator<T, N, Section::col, false, false>;
template<typename T, int N>
using Block_iterator = Section_iterator<T, N, Section::block, false, false>;
template<typename T, int N>
using const_Row_iterator = Section_iterator<T, N, Section::row, true, false>;
template<typename T, int N>
using const_Col_iterator = Section_iterator<T, N, Section::col, true, false>;
template<typename T, int N>
using const_Block_iterator =
	Section_iterator<T, N, Section::block, true, false>;
template<typename T, int N>
using reverse_Row_iterator = Section_iterator<T, N, Section::row, false, true>;
template<typename T, int N>
using reverse_Col_iterator = Section_iterator<T, N, Section::col, false, true>;
template<typename T, int N>
using reverse_Block_iterator =
	Section_iterator<T, N, Section::block, false, true>;
template<typename T, int N>
using const_reverse_Row_iterator =
	Section_iterator<T, N, Section::row, true, true>;
template<typename T, int N>
using const_reverse_Col_iterator =
	Section_iterator<T, N, Section::col, true, true>;
template<typename T, int N>
using const_reverse_Block_iterator =
	Section_iterator<T, N, Section::block, true, true>;

//====--------------------------------------------------------------------====//
template<typename T, int N, Section S, bool is_const, bool is_reverse>
class Section_iterator
{
	using owner_type =
		std::conditional_t<is_const, Board<T, N> const, Board<T, N>>;
	using Location       = ::Sudoku::Location<N>;
	using Location_Block = ::Sudoku::Location_Block<N>;

public:
	// member types
	using difference_type   = int;
	using value_type        = T;
	using pointer           = std::conditional_t<is_const, T const*, T*>;
	using reference         = std::conditional_t<is_const, T const&, T&>;
	using iterator_category = std::random_access_iterator_tag;
	using _Unchecked_type   = pointer; // MSVC STL implementation specific

	// Constructors
	constexpr Section_iterator() noexcept
	{ // defaults to [r]begin()
		if constexpr (is_reverse) elem_ = elem_size<N> - 1;
	}
	template<
		typename idT,
		typename elT,
		typename = std::enable_if_t<is_int_v<idT> && is_int_v<elT>>>
	constexpr Section_iterator(owner_type* owner, idT id, elT elem) noexcept
		: board_(owner), id_(id), elem_(elem)
	{
		assert(is_valid_size<N>(id));
	}
	// [[implicit]] Type Conversion to const_(reverse_)iterator
	constexpr operator Section_iterator<T, N, S, true, is_reverse>() const
		noexcept
	{
		return Section_iterator<T, N, S, true, is_reverse>(board_, id_, elem_);
	}

	//====----------------------------------------------------------------====//
	[[nodiscard]] constexpr Location location() const noexcept
	{
		assert(is_valid_size<N>(elem_));
		using ::Sudoku::Board_Section::Section;
		switch (S)
		{
		case Section::row: return Location(id_, elem_); break;
		case Section::col: return Location(elem_, id_); break;
		case Section::block: return Location_Block(id_, elem_); break;
		}
	}
	// Allows for conversion to Location
	explicit constexpr operator Location() const noexcept { return location(); }

	//====----------------------------------------------------------------====//
	[[nodiscard]] constexpr reference operator*() const noexcept
	{ // Only valid in a dereferenceable location:
		assert(board_ != nullptr && is_valid_size<N>(elem_));
		return board_->operator[](location());
	}
	[[nodiscard]] constexpr pointer operator->() const noexcept
	{ // member access; equivalent to (*p).member
		return std::pointer_traits<pointer>::pointer_to(**this);
	}

	//====----------------------------------------------------------------====//
	constexpr Section_iterator& operator++() noexcept;
	constexpr Section_iterator operator++(int) noexcept;
	constexpr Section_iterator& operator--() noexcept;
	constexpr Section_iterator operator--(int) noexcept;
	constexpr Section_iterator&
		operator+=(const difference_type offset) noexcept;
	constexpr Section_iterator&
		operator-=(const difference_type offset) noexcept;

	[[nodiscard]] friend constexpr difference_type operator-(
		Section_iterator const& left, Section_iterator const& right) noexcept
	{ // difference
		assert(is_same_Section(left, right));
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
		Section_iterator const& left, Section_iterator const& right) noexcept
	{
		assert(is_same_Section(left, right));
		return is_same_Section(left, right) && left.elem_ == right.elem_;
	}
	[[nodiscard]] friend constexpr bool operator<(
		Section_iterator const& left, Section_iterator const& right) noexcept
	{
		assert(is_same_Section(left, right));
		if constexpr (is_reverse)
			return left.elem_ > right.elem_;
		else
			return left.elem_ < right.elem_;
	}

private:
	owner_type* board_{nullptr};
	difference_type id_{0};
	difference_type elem_{0};

	friend constexpr bool is_same_Section(
		Section_iterator const& A, Section_iterator const& B) noexcept
	{ // compare address of:
		return A.board_ == B.board_ && A.id_ == B.id_;
	}
};


//====--------------------------------------------------------------------====//
template<typename T, int N, Section S, bool C, bool is_reverse>
constexpr Section_iterator<T, N, S, C, is_reverse>&
	Section_iterator<T, N, S, C, is_reverse>::operator++() noexcept
{ // pre-increment
	assert(board_ != nullptr);
	if constexpr (is_reverse)
	{
		assert(elem_ >= 0);
		--elem_;
	}
	else
	{
		assert(elem_ < elem_size<N>);
		++elem_;
	}
	return (*this);
}
template<typename T, int N, Section S, bool C, bool R>
constexpr Section_iterator<T, N, S, C, R> Section_iterator<T, N, S, C, R>::
	operator++(int) noexcept
{ // post-increment
	const Section_iterator pre{*this};
	operator++();
	return pre;
}
template<typename T, int N, Section S, bool C, bool is_reverse>
constexpr Section_iterator<T, N, S, C, is_reverse>&
	Section_iterator<T, N, S, C, is_reverse>::operator--() noexcept
{ // pre-decrement
	assert(board_ != nullptr);
	if constexpr (is_reverse)
	{
		assert(elem_ < elem_size<N> - 1);
		++elem_;
	}
	else
	{
		assert(elem_ > 0);
		--elem_;
	}
	return (*this);
}
template<typename T, int N, Section S, bool C, bool R>
constexpr Section_iterator<T, N, S, C, R> Section_iterator<T, N, S, C, R>::
	operator--(int) noexcept
{ // post-decrement
	const Section_iterator pre{*this};
	operator--();
	return pre;
}
template<typename T, int N, Section S, bool C, bool is_reverse>
constexpr Section_iterator<T, N, S, C, is_reverse>&
	Section_iterator<T, N, S, C, is_reverse>::
		operator+=(const difference_type offset) noexcept
{
	assert(offset == 0 || board_ != nullptr);
	if constexpr (is_reverse)
	{
		elem_ -= offset;
		assert(elem_ >= -1);
		assert(elem_ < elem_size<N>);
	}
	else
	{
		elem_ += offset;
		assert(elem_ >= 0);
		assert(elem_ <= elem_size<N>);
	}
	return (*this);
}
template<typename T, int N, Section S, bool C, bool is_reverse>
constexpr Section_iterator<T, N, S, C, is_reverse>&
	Section_iterator<T, N, S, C, is_reverse>::
		operator-=(const difference_type offset) noexcept
{
	return operator+=(-offset);
}

//====--------------------------------------------------------------------====//
// Free functions (non-friend)
//====--------------------------------------------------------------------====//
template<typename T, int N, Section S, bool is_const, bool is_reverse>
[[nodiscard]] constexpr auto operator+(
	Section_iterator<T, N, S, is_const, is_reverse> left,
	typename Section_iterator<T, N, S, is_const, is_reverse>::
		difference_type const offset) noexcept
{ // itr + offset
	return (left += offset);
}
template<typename T, int N, Section S, bool is_const, bool is_reverse>
[[nodiscard]] inline constexpr auto operator+(
	typename Section_iterator<T, N, S, is_const, is_reverse>::
		difference_type const offset,
	Section_iterator<T, N, S, is_const, is_reverse> itr) noexcept
{ // offset + itr
	return (itr += offset);
}
template<typename T, int N, Section S, bool is_const, bool is_reverse>
[[nodiscard]] constexpr auto operator-(
	Section_iterator<T, N, S, is_const, is_reverse> left,
	typename Section_iterator<T, N, S, is_const, is_reverse>::
		difference_type const offset) noexcept
{ // itr - offset
	return (left += -offset);
}

template<typename T, int N, Section S, bool is_const, bool is_reverse>
[[nodiscard]] inline constexpr bool operator!=(
	Section_iterator<T, N, S, is_const, is_reverse> const& left,
	Section_iterator<T, N, S, is_const, is_reverse> const& right) noexcept
{
	return !(left == right);
}
template<typename T, int N, Section S, bool is_const, bool is_reverse>
[[nodiscard]] inline constexpr bool operator>(
	Section_iterator<T, N, S, is_const, is_reverse> const& left,
	Section_iterator<T, N, S, is_const, is_reverse> const& right) noexcept
{
	return (right < left);
}
template<typename T, int N, Section S, bool is_const, bool is_reverse>
[[nodiscard]] inline constexpr bool operator<=(
	Section_iterator<T, N, S, is_const, is_reverse> const& left,
	Section_iterator<T, N, S, is_const, is_reverse> const& right) noexcept
{
	return !(right < left);
}
template<typename T, int N, Section S, bool is_const, bool is_reverse>
[[nodiscard]] inline constexpr bool operator>=(
	Section_iterator<T, N, S, is_const, is_reverse> const& left,
	Section_iterator<T, N, S, is_const, is_reverse> const& right) noexcept
{
	return !(left < right);
}

} // namespace Sudoku::Board_Section
