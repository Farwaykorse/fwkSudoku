/**
*	Data container object template for a sudoku
*
*	accepts only squared dimensions
*	gives full, row, collumn and block access
*	no processing functionality
*/
#pragma once
#include "stdafx.h"
#include "Location.h"
#include "Section.h"

#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>

namespace Sudoku
{
template<typename T, size_t N = 3>
class Board
{
	using Section = Sudoku::Section<T, N, Board<T,N>>;
public:
	static_assert(N > 1, "Board.h base_size value too small");

	using value_type = T;	// stl container conventions
	using const_value_type = const value_type;
	using size_type = unsigned int;
	using size_t = size_type;
	using difference_type = int;
	using self_type = Board<T, N>;
	using reference = value_type&;
	using const_reference = const_value_type&;
	using pointer = value_type*;
	using const_pointer = const_value_type*;

	using Location = Location<N>;

	static const size_t base_size = Location().base_size;	// default 3
	static const size_t elem_size = Location().elem_size;	// default 9
	static const size_t full_size = Location().full_size;	// default 81

	class Row;
	class Col;
	class Block;

	Board();
	explicit Board(const T&);
	Board(self_type&&)				= default;	// move
	Board& operator=(self_type&&)	= default;
	Board(const self_type&)			= default;	// copy
	Board& operator=(const Board&);
	~Board()						= default;

	void clear();

	/* Query properties */
	//bool empty() const	// all elements default value or empty()
	//TODO empty() for different value types
	//{
	//	for (auto& s : board)
	//	{
	//		if ( !s.empty() ) { return false; }
	//	}
	//	return true;
	//}
	static constexpr size_type size() { return full_size; };
	bool operator==(const Board&) const;
	bool operator!=(const Board& other) const { return !(*this == other); };

	/* Element access */
	// Checked
	T& at(Location loc);
	const T& at(Location loc) const;
	T& at(size_t row, size_t col);
	const T& at(size_t row, size_t col) const;
	T& at(size_t elem);
	const T& at(size_t elem) const;
	// Unchecked
	T& operator[](Location);
	const T& operator[](Location) const;

	/*	Element Selection Operator (using a proxy object)
	*	usable as [row][col] where col is processed by the (const_)InBetween
	*/
	class InBetween;
	InBetween operator [] (size_t row);
	class const_InBetween;
	const const_InBetween operator [] (size_t row) const;

	/* Iterators */
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	iterator begin();
	const_iterator cbegin() const;
	const_iterator begin() const { return cbegin(); }
	iterator end();
	const_iterator cend() const;
	const_iterator end() const { return cend(); }

	class Row : public Section
	{
	public:
		using Section::Section;	// inherit constructors
		using Section::id;
	private:
		Location loc(size_t elem) const final 
		{
			return Location(id(), elem);
		}
		size_t element(Location loc) const final { return loc.col(); }
	};

	class Col : public Section
	{
	public:
		using Section::Section;	// inherit constructors
		using Section::id;
	private:
		Location loc(size_t elem) const final { return Location(elem, id()); }
		size_t element(Location loc) const final { return loc.row(); }
	};

	class Block : public Section
	{
		using Block_Loc = Block_Loc<N>;
	public:
		using Section::Section;	// inherit constructors
		using Section::id;
	private:
		Location loc(size_t elem) const override
		{
			return Location(Block_Loc(id(), elem));
		}
		size_t element(Location loc) const override { return loc.block_elem(); }
	};

	Row row(size_t id) { return Row(this, id); }
	Col col(size_t id) { return Col(this, id); }
	Block block(size_t id) { return Block(this, id); }
	Row row(Location loc) { return Row(this, loc.row()); }
	Col col(Location loc) { return Col(this, loc.col()); }
	Block block(Location loc) { return Block(this, loc.block()); }

private:
	std::vector<T> board{};

	void valid_dimensions() const;
	bool valid_size(const size_t elem) const;
	bool valid_size(const size_t row, const size_t col) const;
};	// class Board

// #################################################################################
// Board - memberfunctions
template<typename T, size_t N>
inline void Board<T, N>::valid_dimensions() const
{
	// input check
	static_assert(base_size > 0, "base_size too small");
	static_assert(base_size == 1 ||
		(base_size < elem_size &&
		 elem_size <= full_size &&
		 base_size < std::numeric_limits<int>::max() &&
		 elem_size < std::numeric_limits<int>::max() &&
		 full_size < std::numeric_limits<int>::max()
		 ), "board size out of bounds");
	// logic check
	static_assert(base_size*base_size == elem_size && elem_size*elem_size == full_size, "size calculation broken");
}

template<typename T, size_t N>
inline bool Board<T, N>::valid_size(const size_t elem) const
{
	return elem < full_size;
}

template<typename T, size_t N>
inline bool Board<T, N>::valid_size(const size_t row, const size_t col) const
{
	if (row < elem_size)
	{
		return col < elem_size;
	}
	return false;
}

template<typename T, size_t N>
Board<T, N>::Board() :
	board(full_size)
{
	valid_dimensions();
}

template<typename T, size_t N>
Board<T, N>::Board(const T& def_value) :
	board(full_size, def_value)
{
	valid_dimensions();
}

// copy constructor
template<typename T, size_t N>
inline Board<T, N>& Board<T, N>::operator=(const Board<T, N>& source)
{
	std::copy(source.board.cbegin(), source.board.cend(), board.begin());
	return *this;
}

template<typename T, size_t N>
inline bool Board<T, N>::operator==(const Board& other) const
{
	if (base_size == other.base_size)
	{
		return board == other.board;
	}
	return false;
}

template<typename T, size_t N>
inline T& Board<T, N>::at(Location loc)
{
	return at(loc.element());
}

template<typename T, size_t N>
inline const T& Board<T, N>::at(Location loc) const
{
	return at(loc.element());
}

template<typename T, size_t N>
inline T& Board<T, N>::at(size_t row, size_t col)
{
	if (row >= elem_size || col >= elem_size)
	{
		throw std::out_of_range{ "Board::at(size_t row, col)" };
	}
	return at(Location(row, col).element());
}

template<typename T, size_t N>
inline const T& Board<T, N>::at(size_t row, size_t col) const
{
	if (row >= elem_size || col >= elem_size)
	{
		throw std::out_of_range{ "Board::at(row, col) const" };
	}
	return at(Location(row, col).element());
}

template<typename T, size_t N>
inline T& Board<T, N>::at(const size_t elem)
{
	if (elem >= size())
	{
		throw std::out_of_range{ "Board::at(size_t)" };
	}
	return board.at(elem);
}

template<typename T, size_t N>
inline const T& Board<T, N>::at(const size_t elem) const
{
	if (elem >= size())
	{
		throw std::out_of_range{ "Board::at(size_t) const" };
	}
	return board.at(elem);
}

template<typename T, size_t N>
inline T& Board<T, N>::operator[](Location loc)
{
	return board[loc.element()];
}

template<typename T, size_t N>
inline const T& Board<T, N>::operator[](Location loc) const
{
	return board[loc.element()];
}

template<typename T, size_t N>
inline void Board<T, N>::clear()
{
	/* all elements to the empty value */
	board.clear();
	board.resize(full_size);
}

template<typename T, size_t N>
inline typename Board<T, N>::iterator Board<T, N>::begin()
{
	return board.begin();
}

template<typename T, size_t N>
inline typename Board<T, N>::const_iterator Board<T, N>::cbegin() const
{
	return board.cbegin();
}

template<typename T, size_t N>
inline typename Board<T, N>::iterator Board<T, N>::end()
{
	return board.end();
}

template<typename T, size_t N>
inline typename Board<T, N>::const_iterator Board<T, N>::cend() const
{
	return board.cend();
}
// Board - memberfunctions

// #################################################################################
// InBetween
template<typename T, size_t N>
class Board<T, N>::InBetween
{
public:
	friend class Board<T,N>;
	T& operator[] (size_t col) { return owner->operator[](Location(row, col)); };
private:
	InBetween(Board* owner, size_t row) :	// private constructor prevents creation out of class
		owner(owner), row(row) {}
	Board* const owner; // const-pointer
	const size_t row;
};

template<typename T, size_t N>
class Board<T, N>::const_InBetween
{
public:
	friend class Board<T,N>;
	const T& operator[] (size_t col) const { return owner->operator[](Location(row, col)); };
private:
	const_InBetween(const Board* owner, size_t row) : owner(owner), row(row) {}
	const Board* const owner; // const pointer-to-const
	const size_t row{};
};

template<typename T, size_t N>
inline typename Board<T, N>::InBetween Board<T, N>::operator[](size_t row)
{
	// Element Selection Operator (using a proxy object)
	return InBetween(this, row);
}

template<typename T, size_t N>
inline const typename Board<T, N>::const_InBetween Board<T, N>::operator[](size_t row) const
{
	return const_InBetween(this, row);
}
// InBetween

} // namespace Sudoku