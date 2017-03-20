// Calculate locations within a board
#pragma once
#include <cassert>
#include <limits>

namespace Sudoku
{
template<int N>
class Block_Loc;	// pre-declare for use in Location

template<int N>
class Location
{
	static_assert(N > 1, "Location.h: base_size value too small");

	using Block_Loc = Block_Loc<N>;
	using self_type = Location;
	using value_type = int;
	using difference_type = int;

	static constexpr int location(int row, int col) noexcept { return row * elem_size + col; }
public:
	// size definition
	static constexpr int base_size = N;						// 3 for default 9*9 board
	static constexpr int elem_size = base_size * base_size;	// 9 for default
	static constexpr int full_size = elem_size * elem_size;	// 81 for default
	static_assert(
		base_size < elem_size && base_size < full_size && elem_size < full_size,
		"class Location: Board size out of bounds"
	);
	static_assert(
		N < std::numeric_limits<int>::max() / base_size &&
		N < std::numeric_limits<int>::max() / elem_size &&
		N < std::numeric_limits<int>::max() / elem_size / base_size,
		"class Location: Board size to big for size system integer");

	// constructors
	constexpr Location() = default;
	explicit constexpr Location(int elem) : id_(elem) {}
	constexpr Location(int row, int col) : id_(location(row, col)) {}

	// information
	constexpr int element() const { return id_; }			// default [0,81)
	constexpr int row() const { return id_ / elem_size; }	// default [0,9)
	constexpr int col() const { return id_ % elem_size; }	// default [0,9)
	constexpr int block() const { return row() / base_size * base_size + col() / base_size; }// default [0,9)
	constexpr int block_row() const { return row() % base_size; }	// default [0,3)
	constexpr int block_col() const { return col() % base_size; }	// default [0,3)
	constexpr int block_elem() const { return block_row() * base_size + block_col(); }// default [0,9)

	// comparison
	constexpr bool operator==(const self_type& other) const { return id_ == other.id_; }
	constexpr bool operator==(const Block_Loc& other) const { return id_ == Location(other).element(); }
	constexpr bool operator<(const self_type& other) const { return id_ < other.id_; }

	constexpr bool operator!=(const Location& other) const { return !(*this == other); }
private:
	const int id_{};
};

template<int N>
class Block_Loc
{
	static_assert(N > 1, "Location.h (Block_Loc): base_size value too small");

	using Location = Location<N>;
	static constexpr auto base_size = Location().base_size;
	static constexpr auto elem_size = Location().elem_size;
	static constexpr auto full_size = Location().full_size;

	static constexpr int block_elem(int row, int col) noexcept { return row * base_size + col; }
	static constexpr int block_loc(int id, int elem) noexcept
	{
		return Location(
			(id / base_size) * base_size + elem / base_size,
			(id % base_size) * base_size + elem % base_size).element();
	}
	static constexpr int block_loc(int id, int row, int col) noexcept
	{
		return block_loc(id, block_elem(row, col));
	}
public:
	constexpr Block_Loc(Location loc) : id_(loc.element()) {}
	constexpr Block_Loc(int id, int elem) : id_(block_loc(id, elem)) {}
	constexpr Block_Loc(int id, int row, int col) : id_(block_loc(id, row, col)) {}

	constexpr int id() const { return Location(id_).block(); }
	constexpr int element() const { return Location(id_).block_elem(); }
	constexpr int row() const { return Location(id_).block_row(); }
	constexpr int col() const { return Location(id_).block_col(); }

	constexpr operator Location() const { return Location(id_); }

	// comparison
	constexpr bool operator==(const Block_Loc& other) const noexcept { return id_ == other.id_; }
	constexpr bool operator==(const Location& other) const noexcept { return id_ == other.element(); }
	constexpr bool operator<(const Block_Loc& other) const noexcept
	{
		return ( (id() < other.id()) ? true : (id_ < other.id_) );
	}
private:
	const int id_;
};

template<int N> inline
constexpr bool shared_row(Location<N> left, Location<N> right)
{
	return left.row() == right.row();
}

template<int N> inline
constexpr bool shared_col(Location<N> left, Location<N> right)
{
	return left.col() == right.col();
}
template<int N> inline
constexpr bool shared_block(Location<N> left, Location<N> right)
{
	return left.block() == right.block();
}

}	// namespace Sudoku