//===--	Sudoku/Location.h												--===//
//
// Calculate locations within a board
//===---------------------------------------------------------------------===//
#pragma once

#include "Location_Utilities.h"
#include "Size.h"

#include <limits>

// Forward declarations
#include "Location.fwd.h"

namespace Sudoku
{
template<int N>
class Location
{
	static_assert(N > 1, "Location.h: base_size value too small");

	using Size = Size<N>;
	using self_type = Location;
	using value_type = int;
	using difference_type = int;

	static constexpr int location(int row, int col) noexcept
	{
		return row * Size::elem + col;
	}

public:
	// constructors
	constexpr Location() = default;
	explicit constexpr Location(int elem) : id_(elem) {}
	constexpr Location(int row, int col) : id_(location(row, col)) {}

	// information
	constexpr int element() const { return id_; }          // default [0,81)
	constexpr int row() const { return id_ / Size::elem; } // default [0,9)
	constexpr int col() const { return id_ % Size::elem; } // default [0,9)
	constexpr int block() const
	{
		return row() / Size::base * Size::base + col() / Size::base;
	} // default [0,9)
	constexpr int block_row() const
	{
		return row() % Size::base;
	} // default [0,3)
	constexpr int block_col() const
	{
		return col() % Size::base;
	} // default [0,3)
	constexpr int block_elem() const
	{
		return block_row() * Size::base + block_col();
	} // default [0,9)

	// comparison
	constexpr bool operator==(const Location&) const;
	constexpr bool operator<(const Location&) const;

private:
	const int id_{};
};

template<int N>
class Location_Block
{
	static_assert(N > 1, "Location_Block: base_size value too small");

	using Size = Size<N>;
	using Location = Location<N>;

	static constexpr int block_elem(int row, int col) noexcept
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
		return block_loc(id, block_elem(row, col));
	}
public:
	explicit constexpr Location_Block(Location loc) : id_(loc.element()) {}
	constexpr Location_Block(int id, int elem) : id_(block_loc(id, elem)) {}
	constexpr Location_Block(int id, int row, int col)
		: id_(block_loc(id, row, col))
	{
	}

	constexpr int id() const { return Location(id_).block(); }
	constexpr int element() const { return Location(id_).block_elem(); }
	constexpr int row() const { return Location(id_).block_row(); }
	constexpr int col() const { return Location(id_).block_col(); }

	constexpr operator Location() const { return Location(id_); }

	// comparison
	constexpr bool operator==(const Location_Block&) const;
	constexpr bool operator==(const Location&) const;
	constexpr bool operator<(const Location_Block&) const;

private:
	const int id_;
};

template<int N>
inline constexpr bool
	Location<N>::operator==(const Location<N>& right) const
{
	return id_ == right.id_;
}

template<int N>
inline constexpr bool
	Location_Block<N>::operator==(const Location_Block<N>& right) const
{
	return id_ == right.id_;
}

template<int N>
inline constexpr bool
	Location_Block<N>::operator==(const Location& loc) const
{
	return id_ == loc.element();
}

template<int N>
inline constexpr bool
	operator==(const Location<N>& loc, const Location_Block<N>& block)
{
	return block == loc;
}

template<int N>
inline constexpr bool Location<N>::operator<(const Location<N>& right) const
{
	return id_ < right.id_;
}

template<int N>
inline constexpr bool Location_Block<N>::
	operator<(const Location_Block<N>& right) const
{
	return ( (id() < right.id()) ? true : (id_ < right.id_) );
//	return (
//		(left.id() < right.id()) ? true
//								 : (Location<N>(left) < Location<N>(right)));
}

template<int N>
inline constexpr bool
	operator!=(const Location<N>& left, const Location<N>& right)
{
	return !(left == right);
}

} // namespace Sudoku
