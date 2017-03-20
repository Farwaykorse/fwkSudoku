/**
*	Data container object template for a sudoku
*
*	accepts only squared dimensions
*	gives full, row, collumn and block access
*	no processing functionality
*/
#pragma once
#include "Location.h"
#include "Section.h"

#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>		// numeric_limits
#include <stdexcept>	// out_of_range

namespace Sudoku
{
template<typename T, int N = 3>
class Board
{
	using Section = Sudoku::Section<T, N, Board<T,N>>;
public:
	static_assert(N > 1, "Board.h base_size value too small");

	using value_type = T;	// stl container conventions
	using const_value_type = const value_type;
	using difference_type = int;
	using self_type = Board<T, N>;
	using reference = value_type&;
	using const_reference = const_value_type&;
	using pointer = value_type*;
	using const_pointer = const_value_type*;

	using Location = Location<N>;

	static constexpr int base_size = Location().base_size; // default 3
	static constexpr int elem_size = Location().elem_size;	// default 9
	static constexpr int full_size = Location().full_size;	// default 81

	class Row;
	class Col;
	class Block;

	Board();
	explicit Board(const T&);
	Board(self_type&&)				= default;	// move
	Board& operator=(self_type&&)	= default;
	Board(const self_type&)			= default;	// copy construct
	Board& operator=(const Board&);				// copy assign
	~Board()						= default;

	void clear();

	/* Query properties */
	bool is_empty() const;	// all elements default value or empty()
	//TODO empty() for different value types
	//{
	//	for (auto& s : board_)
	//	{
	//		if ( !s.empty() ) { return false; }
	//	}
	//	return true;
	//}
	static constexpr int size() { return full_size; };
	bool operator==(const Board&) const;
	bool operator!=(const Board& other) const { return !(*this == other); };

	/* Element access */
	// Checked
	T& at(Location);
	const T& at(Location) const;
	T& at(int row, int col);
	const T& at(int row, int col) const;
	T& at(int elem);
	const T& at(int elem) const;
	// Unchecked
	T& operator[](Location) noexcept;
	const T& operator[](Location) const noexcept;

	/*	Element Selection Operator (using a proxy object)
	*	usable as [row][col] where col is processed by the (const_)InBetween
	*/
	class InBetween;
	InBetween operator [] (int row) noexcept;
	class const_InBetween;
	const const_InBetween operator [] (int row) const noexcept;

	/* Iterators */
	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;

	iterator begin() noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator begin() const noexcept { return cbegin(); }
	iterator end() noexcept;
	const_iterator cend() const noexcept;
	const_iterator end() const noexcept { return cend(); }

	class Row : public Section
	{
	public:
		using Section::Section;	// inherit constructors
		using Section::id;
	private:
		Location loc(int elem) const noexcept final
		{
			return Location(id(), elem);
		}
		int element(Location loc) const noexcept final { return Location(loc).col(); }
	};

	class Col : public Section
	{
	public:
		using Section::Section;	// inherit constructors
		using Section::id;
	private:
		Location loc(int elem) const noexcept final { return Location(elem, id()); }
		int element(Location loc) const noexcept final { return Location(loc).row(); }
	};

	class Block : public Section
	{
		using Block_Loc = Block_Loc<N>;
	public:
		using Section::Section;	// inherit constructors
		using Section::id;
	private:
		Location loc(int elem) const noexcept override
		{
			return Location(Block_Loc(id(), elem));
		}
		int element(Location loc) const noexcept override { return loc.block_elem(); }
	};

	Row row(int id) { return Row(this, id); }
	Col col(int id) { return Col(this, id); }
	Block block(int id) { return Block(this, id); }
	Row row(Location loc) { return Row(this, loc.row()); }	//TODO should just except a Location
	Col col(Location loc) { return Col(this, loc.col()); }	// as above
	Block block(Location loc) { return Block(this, loc.block()); }	// as above

private:
	std::vector<T> board_{};

	constexpr void valid_dimensions() const;
	constexpr bool valid_size(const int elem) const;
	constexpr bool valid_size(const int row, const int col) const;
};	// class Board

// #################################################################################
// Board - memberfunctions
template<typename T, int N>
constexpr void Board<T, N>::valid_dimensions() const
{
	// input check
	static_assert(base_size > 0, "base_size too small");
	static_assert(base_size == 1 ||
		(base_size < elem_size &&
		 elem_size <= full_size &&
		 base_size < std::numeric_limits<int>::max() &&	// <limits>
		 elem_size < std::numeric_limits<int>::max() &&
		 full_size < std::numeric_limits<int>::max()
		 ), "board size out of bounds");
	// logic check
	static_assert(base_size*base_size == elem_size && elem_size*elem_size == full_size, "size calculation broken");
}

template<typename T, int N>
constexpr bool Board<T, N>::valid_size(const int elem) const
{
	return (elem >= 0 && elem < full_size);
}

template<typename T, int N>
constexpr bool Board<T, N>::valid_size(const int row, const int col) const
{
	return (row >= 0 && row < elem_size && col >= 0 && col < elem_size);
}

template<typename T, int N>
Board<T, N>::Board() :
	board_(full_size)
{
	valid_dimensions();
}

template<typename T, int N>
Board<T, N>::Board(const T& def_value) :
	board_(full_size, def_value)
{
	valid_dimensions();
}

// copy constructor
template<typename T, int N>
inline Board<T, N>& Board<T, N>::operator=(const Board<T, N>& source)
{
	std::copy(source.board_.cbegin(), source.board_.cend(), board_.begin());
	return *this;
}

template<typename T, int N>
inline bool Board<T, N>::operator==(const Board& other) const
{
	return board_ == other.board_;
}

template<typename T, int N>
inline T& Board<T, N>::at(Location loc)
{
	return at(loc.element());
}

template<typename T, int N>
inline const T& Board<T, N>::at(Location loc) const
{
	return at(loc.element());
}

template<typename T, int N>
inline T& Board<T, N>::at(int row, int col)
{
	if (!valid_size(row, col))
	{
		throw std::out_of_range{ "Board::at(int row, col)" };	// <stdexcept>
	}
	return at(Location(row, col).element());
}

template<typename T, int N>
inline const T& Board<T, N>::at(int row, int col) const
{
	if (!valid_size(row, col))
	{
		throw std::out_of_range{ "Board::at(row, col) const" };
	}
	return at(Location(row, col).element());
}

template<typename T, int N>
inline T& Board<T, N>::at(const int elem)
{
	if (!valid_size(elem))
	{
		throw std::out_of_range{ "Board::at(int)" };
	}
	return board_.at(static_cast<size_t>(elem));
}

template<typename T, int N>
inline const T& Board<T, N>::at(const int elem) const
{
	if (!valid_size(elem))
	{
		throw std::out_of_range{ "Board::at(int) const" };
	}
	return board_.at(static_cast<size_t>(elem));
}

template<typename T, int N>
inline T& Board<T, N>::operator[](Location loc) noexcept
{
	return board_[static_cast<size_t>(loc.element())];
}

template<typename T, int N>
inline const T& Board<T, N>::operator[](Location loc) const noexcept
{
	return board_[static_cast<size_t>(loc.element())];
}

template<typename T, int N>
inline void Board<T, N>::clear()
{
	/* all elements to the empty value */
	board_.clear();
	board_.resize(full_size);
}

template<typename T, int N>
inline typename Board<T, N>::iterator Board<T, N>::begin() noexcept
{
	return board_.begin();
}

template<typename T, int N>
inline typename Board<T, N>::const_iterator Board<T, N>::cbegin() const noexcept
{
	return board_.cbegin();
}

template<typename T, int N>
inline typename Board<T, N>::iterator Board<T, N>::end() noexcept
{
	return board_.end();
}

template<typename T, int N>
inline typename Board<T, N>::const_iterator Board<T, N>::cend() const noexcept
{
	return board_.cend();
}
// Board - memberfunctions

// #################################################################################
// InBetween
template<typename T, int N>
class Board<T, N>::InBetween
{
public:
	friend class Board<T,N>;
	T& operator[] (int col) { return owner->operator[](Location(row, col)); };
private:
	InBetween(Board* owner, int row) :	// private constructor prevents creation out of class
		owner(owner), row(row) {}
	Board* const owner; // const-pointer
	const int row;
};

template<typename T, int N>
class Board<T, N>::const_InBetween
{
public:
	friend class Board<T,N>;
	const T& operator[] (int col) const { return owner->operator[](Location(row, col)); };
private:
	const_InBetween(const Board* owner, int row) : owner(owner), row(row) {}
	const Board* const owner; // const pointer-to-const
	const int row{};
};

template<typename T, int N>
inline typename Board<T, N>::InBetween Board<T, N>::operator[](int row) noexcept
{
	// Element Selection Operator (using a proxy object)
	return InBetween(this, row);
}

template<typename T, int N>
inline const typename Board<T, N>::const_InBetween Board<T, N>::operator[](int row) const noexcept
{
	return const_InBetween(this, row);
}
// InBetween

} // namespace Sudoku