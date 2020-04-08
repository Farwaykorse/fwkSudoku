//====---- Sudoku/Location.h                                          ----====//
//
// class Location: represent locations within a board,
// and calculate location related properties.
//====--------------------------------------------------------------------====//
//
// Everything is `constexpr` and `noexcept`.
// All values are `const`.
//
// A separate Location_Block class offers block properties.
// There is no implicit conversion between these.
//
//====--------------------------------------------------------------------====//
#pragma once

#include "Size.h"
#include <gsl/gsl>     // index
#include <type_traits> // is_signed


namespace Sudoku
{
template<int N>
class Location
{
	using Size  = ::Sudoku::Size<N>;
	using index = gsl::index;

	// prefer signed integers for calculations
	static_assert(std::is_signed_v<decltype(Size::base)>);
	static_assert(std::is_signed_v<decltype(Size::elem)>);

	static constexpr gsl::index location(index row, index col) noexcept
	{
		return row * Size::elem + col; // [0,full)
	}

public:
	// constructors
	constexpr Location() = default;
	explicit constexpr Location(index element) noexcept : id_(element) {}
	constexpr Location(index row, index col) noexcept : id_(location(row, col))
	{
	}

	// information
	[[nodiscard]] constexpr index element() const noexcept
	{
		return id_; // default [0,full)
	}
	[[nodiscard]] constexpr index row() const noexcept
	{
		return id_ / Size::elem; //[0,elem)
	}
	[[nodiscard]] constexpr index col() const noexcept
	{
		return id_ % Size::elem; //[0,elem)
	}
	[[nodiscard]] constexpr index block() const noexcept
	{
		return row() / Size::base * Size::base + col() / Size::base; // [0,elem)
	}
	[[nodiscard]] constexpr index block_row() const noexcept
	{
		return row() % Size::base; // [0,base)
	}
	[[nodiscard]] constexpr index block_col() const noexcept
	{
		return col() % Size::base; // [0,base)
	}
	[[nodiscard]] constexpr index block_elem() const noexcept
	{
		return block_row() * Size::base + block_col(); // [0,elem)
	}

	// comparison
	friend constexpr bool
		operator==(const Location& a, const Location& b) noexcept
	{ // Friend definitions: abseil.io/tips/99
		return a.id_ == b.id_;
	}

private:
	const index id_{};
};

//====--------------------------------------------------------------------====//

template<int N>
class Location_Block
{
	using Size     = ::Sudoku::Size<N>;
	using Location = ::Sudoku::Location<N>;

	// prefer signed integers for calculations
	static_assert(std::is_signed_v<decltype(Size::base)>);

	static constexpr gsl::index
		block_element(gsl::index row, gsl::index col) noexcept
	{
		return row * Size::base + col; // [0,elem)
	}
	static constexpr Location
		block_loc(gsl::index id, gsl::index element) noexcept
	{
		const gsl::index row{
			(id / Size::base) * Size::base + element / Size::base};
		const gsl::index col{
			(id % Size::base) * Size::base + element % Size::base};
		return Location(row, col);
	}
	static constexpr Location
		block_loc(gsl::index id, gsl::index row, gsl::index col) noexcept
	{
		return block_loc(id, block_element(row, col));
	}

public:
	constexpr Location_Block() = default;
	explicit constexpr Location_Block(Location loc) noexcept : loc_(loc)
	{ // empty constructor
	}
	constexpr Location_Block(gsl::index id, gsl::index element) noexcept
		: loc_(block_loc(id, element))
	{ // empty constructor
	}
	constexpr Location_Block(
		gsl::index id, gsl::index row, gsl::index col) noexcept
		: loc_(block_loc(id, row, col))
	{ // empty constructor
	}

	[[nodiscard]] constexpr gsl::index id() const noexcept
	{
		return loc_.block(); // [0,elem)
	}
	[[nodiscard]] constexpr gsl::index element() const noexcept
	{
		return loc_.block_elem(); // [0,elem)
	}
	[[nodiscard]] constexpr gsl::index row() const noexcept
	{
		return loc_.block_row(); // [0,base)
	}
	[[nodiscard]] constexpr gsl::index col() const noexcept
	{
		return loc_.block_col(); // [0,base)
	}

	// [[implicit]]
	// NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
	constexpr operator Location() const noexcept { return loc_; }

private:
	const Location loc_{};
};

//====--------------------------------------------------------------------====//
// free-function declarations
template<int N>
constexpr bool
	operator==(const Location_Block<N>&, const Location_Block<N>&) noexcept;
template<int N>
constexpr bool
	operator==(const Location<N>&, const Location_Block<N>&) noexcept;
template<int N>
constexpr bool
	operator==(const Location_Block<N>&, const Location<N>&) noexcept;

template<int N, typename typeB>
constexpr bool operator!=(const Location<N>&, const typeB&) noexcept;
template<int N, typename typeB>
constexpr bool operator!=(const Location_Block<N>&, const typeB&) noexcept;

template<int N>
constexpr bool operator<(const Location<N>&, const Location<N>&) noexcept;
template<int N>
constexpr bool
	operator<(const Location_Block<N>&, const Location_Block<N>&) noexcept;
template<int N>
constexpr bool operator<=(const Location<N>&, const Location<N>&) noexcept;
template<int N>
constexpr bool
	operator<=(const Location_Block<N>&, const Location_Block<N>&) noexcept;
template<int N>
constexpr bool operator>=(const Location<N>&, const Location<N>&) noexcept;
template<int N>
constexpr bool
	operator>=(const Location_Block<N>&, const Location_Block<N>&) noexcept;
template<int N>
constexpr bool operator>(const Location<N>&, const Location<N>&) noexcept;
template<int N>
constexpr bool
	operator>(const Location_Block<N>&, const Location_Block<N>&) noexcept;


//====--------------------------------------------------------------------====//
// definitions

template<int N>
constexpr bool operator==(
	const Location_Block<N>& left, const Location_Block<N>& right) noexcept
{
	return Location<N>{left} == Location<N>{right};
}

template<int N>
constexpr bool
	operator==(const Location_Block<N>& block, const Location<N>& loc) noexcept
{
	return Location<N>{block} == loc;
}

template<int N>
inline constexpr bool
	operator==(const Location<N>& loc, const Location_Block<N>& block) noexcept
{
	return block == loc;
}

template<int N, typename typeB>
inline constexpr bool
	operator!=(const Location<N>& left, const typeB& right) noexcept
{
	static_assert(
		std::is_same_v<Location<N>, typeB> ||
		std::is_same_v<Location_Block<N>, typeB>);
	return !(left == right);
}

template<int N, typename typeB>
inline constexpr bool
	operator!=(const Location_Block<N>& left, const typeB& right) noexcept
{
	static_assert(
		std::is_same_v<Location<N>, typeB> ||
		std::is_same_v<Location_Block<N>, typeB>);
	return !(left == right);
}

template<int N>
constexpr bool
	operator<(const Location<N>& left, const Location<N>& right) noexcept
{
	return left.element() < right.element();
}

template<int N>
constexpr bool
	operator<=(const Location<N>& left, const Location<N>& right) noexcept
{
	return left.element() <= right.element();
}

template<int N>
constexpr bool
	operator>=(const Location<N>& left, const Location<N>& right) noexcept
{
	return left.element() >= right.element();
}

template<int N>
constexpr bool
	operator>(const Location<N>& left, const Location<N>& right) noexcept
{
	return left.element() > right.element();
}

// Sorted by block first.
template<int N>
constexpr bool operator<(
	const Location_Block<N>& left, const Location_Block<N>& right) noexcept
{
	return (left.id() == right.id()) ? (left.element() < right.element())
									 : left.id() < right.id();
}

template<int N>
constexpr bool operator<=(
	const Location_Block<N>& left, const Location_Block<N>& right) noexcept
{
	return (left.id() == right.id()) ? (left.element() <= right.element())
									 : left.id() < right.id();
}

template<int N>
constexpr bool operator>=(
	const Location_Block<N>& left, const Location_Block<N>& right) noexcept
{
	return (left.id() == right.id()) ? (left.element() >= right.element())
									 : left.id() > right.id();
}

template<int N>
constexpr bool operator>(
	const Location_Block<N>& left, const Location_Block<N>& right) noexcept
{
	return (left.id() == right.id()) ? (left.element() > right.element())
									 : left.id() > right.id();
}

} // namespace Sudoku
