//===--- Sudoku/Location.h                                              ---===//
//
// class Location: represent locations within a board,
// and calculate location related properties.
//===----------------------------------------------------------------------===//
//
// Everything is `constexpr` and `noexcept`.
// All values are `const`.
//
// A separate Location_Block class offers block properies.
// There is no implicit conversion between these.
//
//===----------------------------------------------------------------------===//
#pragma once

#include "Size.h"
#include <type_traits> // is_signed


namespace Sudoku
{
template<int N>
class Location
{
	static_assert(N > 1, "Location.h: base_size value too small");

	using Size            = Size<N>;
	using self_type       = Location;
	using value_type      = int;
	using difference_type = int;

	// prefere signed integers for calculations
	static_assert(std::is_signed_v<decltype(Size::base)>);
	static_assert(std::is_signed_v<decltype(Size::elem)>);

	static constexpr int location(int row, int col) noexcept
	{
		return row * Size::elem + col;
	}

public:
	// constructors
	constexpr Location() = default;
	explicit constexpr Location(int elem) noexcept : id_(elem) {}
	constexpr Location(int row, int col) noexcept : id_(location(row, col)) {}

	// information
	constexpr int element() const noexcept { return id_; } // default [0,81)
	constexpr int row() const noexcept { return id_ / Size::elem; } // [0,9)
	constexpr int col() const noexcept { return id_ % Size::elem; } // [0,9)
	constexpr int block() const noexcept
	{
		return row() / Size::base * Size::base + col() / Size::base;
	} // default [0,9)
	constexpr int block_row() const noexcept
	{
		return row() % Size::base;
	} // default [0,3)
	constexpr int block_col() const noexcept
	{
		return col() % Size::base;
	} // default [0,3)
	constexpr int block_elem() const noexcept
	{
		return block_row() * Size::base + block_col();
	} // default [0,9)

	// comparison
	constexpr bool operator==(const Location&) const noexcept;

private:
	const int id_{};
};

//===----------------------------------------------------------------------===//

template<int N>
class Location_Block
{
	static_assert(N > 1, "Location_Block: base_size value too small");

	using Size     = Size<N>;
	using Location = Location<N>;

	// prefere signed integers for calculations
	static_assert(std::is_signed_v<decltype(Size::base)>);

	static constexpr int block_element(int row, int col) noexcept
	{
		return row * Size::base + col;
	}
	static constexpr int block_loc(int id, int elem) noexcept
	{
		const int row{(id / Size::base) * Size::base + elem / Size::base};
		const int col{(id % Size::base) * Size::base + elem % Size::base};
		return Location(row, col).element();
	}
	static constexpr int block_loc(int id, int row, int col) noexcept
	{
		return block_loc(id, block_element(row, col));
	}

public:
	constexpr Location_Block() = default;
	explicit constexpr Location_Block(Location loc) noexcept
		: id_(loc.element())
	{ // empty constructor
	}
	constexpr Location_Block(int id, int elem) noexcept
		: id_(block_loc(id, elem))
	{ // empty constructor
	}
	constexpr Location_Block(int id, int row, int col) noexcept
		: id_(block_loc(id, row, col))
	{ // empty constructor
	}

	constexpr int id() const noexcept { return Location(id_).block(); }
	constexpr int element() const noexcept
	{
		return Location(id_).block_elem();
	}
	constexpr int row() const noexcept { return Location(id_).block_row(); }
	constexpr int col() const noexcept { return Location(id_).block_col(); }

	constexpr operator Location() const noexcept { return Location(id_); }

private:
	const int id_{};
};

//===----------------------------------------------------------------------===//
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


//===----------------------------------------------------------------------===//
// definitions

template<int N>
inline constexpr bool Location<N>::operator==(const Location<N>& right) const
	noexcept
{
	return id_ == right.id_;
}

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
