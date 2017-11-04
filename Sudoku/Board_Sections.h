/**	Section access for Board<T,N>
 *	included by Board.h
 */
#pragma once

#include <gsl/gsl>
#include <iterator>
#include <memory>
#include <cassert>

// Forward declarations
#include "Board.fwd.h" // self
#include "Location.fwd.h"

namespace Sudoku::Board_Section
{
template<typename T, int N>
class Section
{
	//empty base-class
};

template<typename T, int N>
class const_Row
	: public Section<T, N>
{
	using self_type = const_Row;
	using Location = Sudoku::Location<N>;

	friend class Board<T, N>;		// access to private constructor
	friend class Row<T, N>;
	friend class const_Col<T, N>;
	friend class const_Block<T, N>;
	friend class const_iterator<T,N,self_type>;			// access to owner_
	friend class const_iterator<T,N,const_Col<T,N>>;
	friend class const_iterator<T,N,const_Block<T,N>>;

	const_Row(gsl::not_null<const Board<T, N>*> owner, const int row)
		:	owner_(owner), id_(row)
	{
		assert(is_valid_size<N>(row));
	}
	const_Row(gsl::not_null<const Board<T, N>*> owner, const Location loc)
		:	owner_(owner), id_(loc.row())
	{
		assert(is_valid(loc));
	}
public:
	constexpr int size() const noexcept { return Location().elem_size; }

	int id() const { return id_; }

	const T& operator[] (int col) const noexcept
	{
		assert(is_valid_size<N>(col));
		return no_check(Location(id(), col));
	}
	using const_iterator = const_iterator<T,N, self_type>;
	const_iterator cbegin() const	{ return const_iterator(this); }
	const_iterator cend() const		{ return const_iterator(this, size()); }
	const_iterator begin() const	{ return cbegin(); }
	const_iterator end() const		{ return cend(); }
	constexpr auto crbegin() const	{ return std::make_reverse_iterator(cend()); }
	constexpr auto crend() const	{ return std::make_reverse_iterator(cbegin()); }
	auto rbegin() const				{ return crbegin(); }
	auto rend() const				{ return crend(); }

	constexpr Location location(int element) const
	{
		assert(is_valid_size<N>(element));
		return Location(id(), element);
	}
protected:
	// Data-access for child-objects
	const T& no_check(const Location& loc) const noexcept
	{
		assert(is_valid(loc));
		return owner_->operator[](loc);
	}
private:
	const Board<T, N>* const owner_; // const-pointer-to-const-object
	const int id_;
};


template<typename T, int N>
class const_Col
	: public const_Row<T, N>
{
	using self_type = const_Col;
	using const_Row = const_Row<T, N>;
	using Location = Sudoku::Location<N>;

	friend class Board<T, N>;		// access to private constructor
	friend class Col<T,N>;
	friend class const_iterator<T,N,self_type>;

	const_Col(gsl::not_null<const Board<T, N>*> owner, const int col)
		: const_Row(owner, col)
	{
	}
	const_Col(gsl::not_null<const Board<T, N>*> owner, const Location loc)
		: const_Row(owner, loc.col())
	{
		assert(is_valid(loc));
	}
	using const_Row::no_check;
public:
	using const_Row::id;
	using const_Row::size;

	const T& operator[] (int row) const noexcept
	{
		assert(is_valid_size<N>(row));
		return no_check(Location(row, id()));
	}

	using const_iterator = const_iterator<T,N, self_type>;
	const_iterator cbegin() const	{ return const_iterator(this); }
	const_iterator cend() const		{ return const_iterator(this, size()); }
	const_iterator begin() const	{ return cbegin(); }
	const_iterator end() const		{ return cend(); }
	constexpr auto crbegin() const	{ return std::make_reverse_iterator(cend()); }
	constexpr auto crend() const	{ return std::make_reverse_iterator(cbegin()); }
	auto rbegin() const				{ return crbegin(); }
	auto rend() const				{ return crend(); }

	constexpr Location location(int element) const
	{
		assert(is_valid_size<N>(element));
		return Location(element, id());
	}
};


template<typename T, int N>
class const_Block
	: public const_Row<T, N>
{
	using self_type = const_Block;
	using const_Row = const_Row<T, N>;
	using Location = Sudoku::Location<N>;

	friend class Board<T, N>;
	friend class Block<T,N>;
	friend class const_iterator<T,N,self_type>;

	const_Block(gsl::not_null<const Board<T, N>*> owner, const int id)
		: const_Row(owner, id)
	{
	}
	const_Block(gsl::not_null<const Board<T, N>*> owner, const Location loc)
		: const_Row(owner, loc.block())
	{
		assert(is_valid(loc));
	}
	using const_Row::no_check;
public:
	using const_Row::id;
	using const_Row::size;

	const T& operator[] (int elem) const noexcept
	{
		assert(is_valid_size<N>(elem));
		//return no_check(Location_Block<N>(id(), elem));
		return no_check(location(elem));
	}

	using const_iterator = const_iterator<T,N, self_type>;
	const_iterator cbegin() const	{ return const_iterator(this); }
	const_iterator cend() const		{ return const_iterator(this, size()); }
	const_iterator begin() const	{ return cbegin(); }
	const_iterator end() const		{ return cend(); }
	constexpr auto crbegin() const	{ return std::make_reverse_iterator(cend()); }
	constexpr auto crend() const	{ return std::make_reverse_iterator(cbegin()); }
	auto rbegin() const				{ return crbegin(); }
	auto rend() const				{ return crend(); }

	constexpr Location location(int element) const
	{
		assert(is_valid_size<N>(element));
		return Location_Block<N>(id(), element);
	}
};


template<typename T, int N>
class Row
	: public const_Row<T, N>
{
	using self_type = Row;
	using const_Row = const_Row<T, N>;
	using Location = Sudoku::Location<N>;

	friend class Sudoku::Board<T,N>;		// access private constructor
	friend class iterator<T,N,self_type>;
	friend class const_iterator<T,N, self_type>;

	Row(gsl::not_null<Board<T, N>*> owner, int row)
		: const_Row(owner, row), owner_(owner)
	{
	}
	Row(gsl::not_null<Board<T, N>*> owner, Location loc)
		: const_Row(owner, loc.row()), owner_(owner)
	{
		assert(is_valid(loc));
	}
public:
	using const_Row::size;
	using const_Row::id;

	T& operator[] (const int col) noexcept
	{
		assert(is_valid_size<N>(col));
		return no_check(Location(id(), col));
	}

	using iterator = iterator<T,N, self_type>;
	iterator begin()		{ return iterator(this); }
	iterator end()			{ return iterator(this, size()); }
	constexpr auto rbegin() { return std::make_reverse_iterator(end()); }
	constexpr auto rend()	{ return std::make_reverse_iterator(begin()); }

private:
	Board<T, N>* const owner_; // const-pointer

	T& no_check(Location loc) noexcept
	{
		assert(is_valid(loc));
		return owner_->operator[](loc);
	}
};



template<typename T, int N>
class Col
	:	public const_Col<T,N>
{
	using self_type = Col;
	using Location = Sudoku::Location<N>;
	using const_Col = const_Col<T,N>;

	friend class Sudoku::Board<T,N>;		// access private constructor

	friend class iterator<T,N, self_type>;
	friend class const_iterator<T,N, self_type>;

	Col(gsl::not_null<Board<T, N>*> owner, int col)
		:	const_Col(owner, col),
			owner_(owner)
	{
	}
	Col(gsl::not_null<Board<T, N>*> owner, Location loc)
		:	const_Col(owner, loc.col()),
			owner_(owner)
	{
		assert(is_valid(loc));
	}
public:
	using const_Col::id;
	using const_Col::size;

	T& operator[] (const int row) noexcept
	{
		assert(is_valid_size<N>(row));
		return no_check(Location(row, id()));
	}

	using iterator = iterator<T,N, self_type>;
	iterator begin()		{ return iterator(this); }
	iterator end()			{ return iterator(this, size()); }
	constexpr auto rbegin() { return std::make_reverse_iterator(end()); }
	constexpr auto rend()	{ return std::make_reverse_iterator(begin()); }
private:
	Board<T, N>* const owner_; // const-pointer

	T& no_check(Location loc) noexcept
	{
		assert(is_valid(loc));
		return owner_->operator[](loc);
	}
};


template<typename T, int N>
class Block
	:	public const_Block<T,N>
{
	using self_type = Block;
	using const_Block = const_Block<T,N>;
	using Location = Sudoku::Location<N>;

	friend class Sudoku::Board<T,N>;		// access private constructor
	friend class iterator<T,N, self_type>;
	friend class const_iterator<T,N, self_type>;

	Block(gsl::not_null<Board<T, N>*> owner, int id)
		:	const_Block(owner, id),
			owner_(owner)
	{
	}
	Block(gsl::not_null<Board<T, N>*> owner, Location loc)
		:	const_Block(owner, loc.block()),
			owner_(owner)
	{
		assert(is_valid(loc));
	}
public:
	using const_Block::id;
	using const_Block::size;

	T& operator[] (const int elem) noexcept
	{
		assert(is_valid_size<N>(elem));
		return no_check(Location_Block<N>(id(), elem));
	}

	using iterator = iterator<T,N, self_type>;
	iterator begin()		{ return iterator(this); }
	iterator end()			{ return iterator(this, size()); }
	constexpr auto rbegin() { return std::make_reverse_iterator(end()); }
	constexpr auto rend()	{ return std::make_reverse_iterator(begin()); }
private:
	Board<T, N>* const owner_; // const-pointer

	T& no_check(Location loc) noexcept
	{
		assert(is_valid(loc));
		return owner_->operator[](loc);
	}
};


template<typename T, int N, typename ownerT>
class const_iterator
{
	using self_type = const_iterator;
	using owner_type = ownerT;
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = const T;
	using difference_type = int;
	using reference = value_type&;
	using pointer = value_type*;

	const_iterator(gsl::not_null<const owner_type*> owner)
		: owner_(owner), elem_(0)
	{
	}
	const_iterator(gsl::not_null<const owner_type*> owner, int elem)
		: owner_(owner), elem_(elem)
	{
	}

	// All iterator categories
	const_iterator(const self_type&) = default;
	self_type& operator=(const self_type&) = default;
	self_type& operator++() { ++elem_; return *this; }
	self_type operator++(int)
	{
		self_type pre = *this;
		operator++();
		return pre;
	}
	reference operator*() const
	{
		return (*owner_)[elem_];
	}

	// Input iterator
	bool operator==(const self_type& other) const
	{
		return 
			elem_ == other.elem_ &&
			owner_->id() == other.owner_->id() &&
			owner_->owner_ == other.owner_->owner_;
	}
	bool operator!=(const self_type& other) const
	{
		return (!(*this == other));
	}

	// In- & output iterator
	pointer operator->() const noexcept
	{ // member access; equivalent to (*p).member
		return std::addressof<value_type>(owner_->operator[](elem_));
	}

	// Forward iterator
	const_iterator() : owner_(), elem_{}
	{
		// construct with null pointer
	}

	// Bidirectional iterator
	self_type& operator--() { --elem_; 	return *this; }	//predecrement
	self_type operator--(int)	// postdecrement
	{
		self_type pre{ *this };
		operator--();
		return pre;
	}

	// RandomAccess iterator
	self_type& operator+=(difference_type offset)
	{
		compatible_(offset);
		elem_ += offset; return *this;
	}
	self_type operator+(difference_type offset) const
	{
		self_type tmp{ *this };
		return (tmp += offset);
	}
	self_type& operator-=(difference_type offset)
	{
		return operator+=(-offset);
	}
	self_type operator-(difference_type offset) const
	{
		self_type tmp{ *this };
		return (tmp += -offset);
	}
	difference_type operator-(const self_type& other) const
	{
		compatible_(other);
		return elem_ - other.elem_;
	}
	reference operator[](difference_type offset) const
	{
		return (*(*this + offset));
	}

	bool operator<(const self_type& other) const
	{
		compatible_(other);
		return elem_ < other.elem_;
	}
	bool operator> (const self_type& other) const { return (other < *this); }
	bool operator<=(const self_type& other) const { return (!(other < *this)); }
	bool operator>=(const self_type& other) const { return (!(*this < other)); }

	Location<N> location() const
	{
		return owner_->location(elem_);
	}
private:
	const owner_type* owner_;

protected:
	// accessible to child class: iterator
	int elem_;	// element within the section

	[[maybe_unused]] void compatible_(
		[[maybe_unused]] difference_type offset) const
	{
		assert(elem_ + offset >= 0 && elem_ + offset < owner_->size());
	}
	[[maybe_unused]] void compatible_(
		[[maybe_unused]] const self_type& other) const
	{
		assert(owner_->owner_ == other.owner_->owner_);
		assert(owner_->id() == other.owner_->id());
	}
};



template<typename T, int N, typename ownerT>
class iterator
	: public Board_Section::const_iterator<T,N,ownerT>
{
	using self_type = iterator;
	using owner_type = ownerT;
	using const_iterator = const_iterator<T,N,ownerT>;
	
	using const_iterator::elem_;
	using const_iterator::compatible_;
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
	using difference_type = int;
	using reference = value_type&;
	using pointer = value_type*;

	iterator(gsl::not_null<owner_type*> owner)
		:	const_iterator(owner, 0),
			owner_(owner)
	{
	}
	iterator(gsl::not_null<owner_type*> owner, int elem)
		:	const_iterator(owner, elem),
			owner_(owner)
	{
	}

	// All iterator categories
	iterator(const self_type&) = default;
	self_type& operator=(const self_type&) = default;
	self_type& operator++() { ++elem_; 	return *this; }
	self_type operator++(int)
	{
		self_type pre = *this;
		operator++();
		return pre;
	}
	reference operator*() const
	{
		return owner_->operator[](elem_);
	}

	// Input iterator
	bool operator==(const self_type& other) const
	{
		return
			elem_ == other.elem_ &&
			owner_->id() == other.owner_->id() &&
			owner_->owner_ == other.owner_->owner_;
	}
	bool operator!=(const self_type& other) const
	{
		return !(operator==(other));
	}

	// In- & output iterator
	pointer operator->() const noexcept	// member access; equivalent to (*p).member
	{
		return std::addressof<value_type>(owner_->operator[](elem_));
	}

	// Forward iterator
	iterator()
		:	const_iterator(),
			owner_()
	{	// construct with null pointer
	}

	// Bidirectional iterator
	self_type& operator--() { --elem_; return *this; }	//predecrement 
	self_type operator--(int)	// postdecrement
	{
		self_type pre{ *this };
		operator--();
		return pre;
	}

	// RandomAccess iterator
	self_type& operator+=(difference_type offset)
	{
		compatible_(offset);
		elem_ += offset;
		return *this;
	}
	self_type operator+(difference_type offset) const
	{
		self_type tmp{ *this };
		return (tmp += offset);
	}
	self_type& operator-=(difference_type offset) { return operator+=(-offset); }
	self_type operator-(difference_type offset) const
	{
		self_type tmp{ *this };
		return (tmp += -offset);
	}
	difference_type operator-(const self_type& other) const
	{
		compatible_(other);
		return elem_ - other.elem_;
	}
	reference operator[](difference_type offset) const
	{
		return (*(*this + offset));
	}

private:
	owner_type* owner_;
};

}	// namespace Sudoku
