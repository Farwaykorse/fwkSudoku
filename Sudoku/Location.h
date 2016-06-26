// Determine location within a board

#pragma once
#include <vector>

namespace Sudoku
{
	template<size_t N>
	class Location
	{
		static constexpr size_t base_size = N;	// 3 for default 9*9 board
		static constexpr size_t elem_size = base_size * base_size;
		static constexpr size_t full_size = elem_size * elem_size;
		static_assert(base_size < elem_size && base_size < full_size && elem_size < full_size,
			  "class Location: Board size out of bounds");

		static constexpr size_t location(size_t row, size_t col)
		{
			return row * elem_size + col;
		}
		static constexpr size_t block_loc(size_t id, size_t elem)
		{
			return location((id / base_size) * base_size + elem / base_size,
				(id % base_size) * base_size + elem % base_size);
		}
	public:
		using self_type = Location;
		using value_type = size_t;
		using difference_type = int;

		explicit constexpr Location(size_t elem) : id_(elem) {}
		constexpr Location(size_t row, size_t col) : id_(location(row, col)) {}
		Location(self_type&&)				= default;	// move
		Location& operator=(self_type&&)	= default;
		Location(const self_type&)			= default;	// copy
		Location& operator=(const self_type&) = default;
		~Location()							= default;

		// information
		constexpr size_t element() const { return id_; }
		constexpr size_t col() const { return id_ % elem_size; }
		constexpr size_t row() const { return (id_ - col()) / elem_size; }
		constexpr size_t block() const { return row() / base_size * base_size + col() / base_size; }

		// comparison
		bool operator==(const self_type& other) const { return id_ == other.id_; }
		bool operator<(const self_type& other) const { return id_ < other.id_; }

		// change
		//NOTE functional programming won't allow changing, it would create a new object
		//		might this be doable? would it offer an advantage?
		//self_type move(size_t elem) { id_ = elem; return *this; }
	private:
		size_t id_;
	};
}