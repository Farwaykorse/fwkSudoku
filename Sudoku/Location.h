// Determine location within a board

#pragma once
#include <cassert>

namespace Sudoku
{
template<int N>
class Block_Loc
{
	static_assert(N > 1, "Location.h (Block_Loc): base_size value too small");

	static constexpr int base_size = N;
	static constexpr int block_elem(int row, int col)
	{
		assert(row < base_size && col < base_size);
		return row * base_size + col;
	}
public:
	constexpr Block_Loc(int id, int elem) : id(id), elem(elem) {}
	constexpr Block_Loc(int id, int row, int col) : id(id), elem(block_elem(row, col)) {}
	
	const int id{};
	const int elem{};
};

template<int N>
class Location
{
	static_assert(N > 1, "Location.h: base_size value too small");

	using Block_Loc = Block_Loc<N>;

	static constexpr int location(int row, int col) { return row * elem_size + col; }
	static constexpr int block_loc(int id, int elem)
	{
		return location((id / base_size) * base_size + elem / base_size,
			(id % base_size) * base_size + elem % base_size);
	}
	static constexpr int block_loc(Block_Loc B) { return block_loc(B.id, B.elem); }

public:
	using self_type = Location;
	using value_type = int;
	using difference_type = int;

	// constructors
	constexpr Location() : id_(0) {}
	explicit constexpr Location(int elem) : id_(elem) {}
	constexpr Location(int row, int col) : id_(location(row, col)) {}
	constexpr Location(Block_Loc block) : id_(block_loc(block)) {}

	// size definition
	static constexpr int base_size = N;						// 3 for default 9*9 board
	static constexpr int elem_size = base_size * base_size;	// 9 for default
	static constexpr int full_size = elem_size * elem_size;	// 81 for default
	static_assert(
		base_size < elem_size && base_size < full_size && elem_size < full_size,
		"class Location: Board size out of bounds"
	);

	// information
	constexpr int element() const { return id_; }			// default [0,81)
	constexpr int row() const { return id_ / elem_size; }	// default [0,9)
	constexpr int col() const { return id_ % elem_size; }	// default [0,9)
	constexpr int block() const { return row() / base_size * base_size + col() / base_size; }// default [0,9)
	constexpr int block_row() const { return row() % base_size; }	// default [0,3)
	constexpr int block_col() const { return col() % base_size; }	// default [0,3)
	constexpr int block_elem() const { return block_row() * base_size + block_col(); }// default [0,9)

	// comparison
	bool operator==(const self_type& other) const { return id_ == other.id_; }
	bool operator<(const self_type& other) const { return id_ < other.id_; }

	// change
	//NOTE functional programming won't allow changing, it would create a new object
private:
	const int id_;
};
}	// namespace Sudoku