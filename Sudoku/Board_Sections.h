//===--- Sudoku/Board_Sections.h                                        ---===//
//
// Section access for Board<T,N>, as if a separate object
// using iterators and operator[]
//===----------------------------------------------------------------------===//
//
// The iterator can return a Location object.
// included by Board.h
//
//===----------------------------------------------------------------------===//
#pragma once

#include "Location.h"
#include "Location_Utilities.h"
#include "Size.h"
#include <gsl/gsl>

#include <iterator> // make_reverse_iterator; *_iterator_tag
#include <memory>   // addressof
#include <cassert>

// Forward declarations
#include "Board.fwd.h" // self

namespace Sudoku::Board_Section
{
template<typename T, int N>
class Section
{ // empty base-class
};

//===----------------------------------------------------------------------===//
template<typename T, int N>
class const_Row : public Section<T, N>
{
	using self_type = const_Row;
	using Location  = Sudoku::Location<N>;

	friend class Board<T, N>; // access to private constructor
	friend class Row<T, N>;
	friend class const_Col<T, N>;
	friend class const_Block<T, N>;
	friend class const_iterator<T, N, self_type>; // access to owner_
	friend class const_iterator<T, N, const_Col<T, N>>;
	friend class const_iterator<T, N, const_Block<T, N>>;

	const_Row(const Board<T, N>& owner, const int row) noexcept
		: owner_(owner), id_(row)
	{
		assert(is_valid_size<N>(row));
	}
	const_Row(const Board<T, N>& owner, const Location loc) noexcept
		: owner_(owner), id_(loc.row())
	{
		assert(is_valid(loc));
	}

public:
	template<typename OtherT>
	const_Row(OtherT) = delete; // stop inheritance parent constructing
	const_Row(Row<T, N> row) noexcept : owner_(row.owner_), id_(row.id_) {}

	constexpr int size() const noexcept { return elem_size<N>; }

	int id() const noexcept { return id_; }

	const T& operator[](const int col) const noexcept
	{
		assert(is_valid_size<N>(col));
		return no_check(Location(id(), col));
	}
	using const_iterator = const_iterator<T, N, self_type>;
	const_iterator cbegin() const noexcept
	{
		return const_iterator(gsl::not_null<decltype(this)>{this});
	}
	const_iterator cend() const noexcept
	{
		return const_iterator(gsl::not_null<decltype(this)>{this}, size());
	}
	const_iterator begin() const noexcept { return cbegin(); }
	const_iterator end() const noexcept { return cend(); }
	constexpr auto crbegin() const
	{
		return std::make_reverse_iterator(cend());
	}
	constexpr auto crend() const
	{
		return std::make_reverse_iterator(cbegin());
	}
	auto rbegin() const noexcept { return crbegin(); }
	auto rend() const noexcept { return crend(); }
	constexpr Location location(const int element) const noexcept
	{
		assert(is_valid_size<N>(element));
		return Location(id(), element);
	}

protected:
	// Data-access for child-objects
	const T& no_check(const Location loc) const noexcept
	{
		assert(is_valid(loc));
		return owner_[loc];
	}

private:
	const Board<T, N>& owner_; // const-pointer-to-const-object
	const int id_;
};

//===----------------------------------------------------------------------===//
template<typename T, int N>
class const_Col : public const_Row<T, N>
{
	using self_type = const_Col;
	using const_Row = const_Row<T, N>;
	using Location  = Sudoku::Location<N>;

	friend class Board<T, N>; // access to private constructor
	friend class Col<T, N>;
	friend class const_iterator<T, N, self_type>;

	const_Col(const Board<T, N>& owner, const int col) noexcept
		: const_Row(owner, col)
	{
	}
	const_Col(const Board<T, N>& owner, const Location loc) noexcept
		: const_Row(owner, loc.col())
	{
		assert(is_valid(loc));
	}
	using const_Row::no_check;

public:
	using const_Row::id;
	using const_Row::size;

	const T& operator[](const int row) const noexcept
	{
		assert(is_valid_size<N>(row));
		return no_check(Location(row, id()));
	}
	using const_iterator = const_iterator<T, N, self_type>;
	const_iterator cbegin() const noexcept
	{
		return const_iterator(gsl::not_null<decltype(this)>{this});
	}
	const_iterator cend() const noexcept
	{
		return const_iterator(gsl::not_null<decltype(this)>{this}, size());
	}
	const_iterator begin() const noexcept { return cbegin(); }
	const_iterator end() const noexcept { return cend(); }
	constexpr auto crbegin() const
	{
		return std::make_reverse_iterator(cend());
	}
	constexpr auto crend() const
	{
		return std::make_reverse_iterator(cbegin());
	}
	auto rbegin() const noexcept { return crbegin(); }
	auto rend() const noexcept { return crend(); }
	constexpr Location location(const int element) const noexcept
	{
		assert(is_valid_size<N>(element));
		return Location(element, id());
	}
};

//===----------------------------------------------------------------------===//
template<typename T, int N>
class const_Block : public const_Row<T, N>
{
	using self_type = const_Block;
	using const_Row = const_Row<T, N>;
	using Location  = Sudoku::Location<N>;

	friend class Board<T, N>;
	friend class Block<T, N>;
	friend class const_iterator<T, N, self_type>;

	const_Block(const Board<T, N>& owner, const int id) noexcept
		: const_Row(owner, id)
	{
	}
	const_Block(const Board<T, N>& owner, const Location loc) noexcept
		: const_Row(owner, loc.block())
	{
		assert(is_valid(loc));
	}
	using const_Row::no_check;

public:
	using const_Row::id;
	using const_Row::size;

	const T& operator[](const int elem) const noexcept
	{
		assert(is_valid_size<N>(elem));
		return no_check(location(elem));
	}
	using const_iterator = const_iterator<T, N, self_type>;
	const_iterator cbegin() const noexcept
	{
		return const_iterator(gsl::not_null<decltype(this)>{this});
	}
	const_iterator cend() const noexcept
	{
		return const_iterator(gsl::not_null<decltype(this)>{this}, size());
	}
	const_iterator begin() const noexcept { return cbegin(); }
	const_iterator end() const noexcept { return cend(); }
	constexpr auto crbegin() const
	{
		return std::make_reverse_iterator(cend());
	}
	constexpr auto crend() const
	{
		return std::make_reverse_iterator(cbegin());
	}
	auto rbegin() const noexcept { return crbegin(); }
	auto rend() const noexcept { return crend(); }
	constexpr Location location(const int element) const noexcept
	{
		assert(is_valid_size<N>(element));
		return Location_Block<N>(id(), element);
	}
};

//===----------------------------------------------------------------------===//
template<typename T, int N>
class Row : public const_Row<T, N>
{
	using self_type = Row;
	using Location  = Sudoku::Location<N>;

	friend class Sudoku::Board<T, N>; // access private constructor
	friend class const_Row<T, N>;
	friend class iterator<T, N, self_type>;
	friend class const_iterator<T, N, self_type>;

	Row(Board<T, N>& owner, int row) noexcept
		: const_Row<T, N>(owner, row), owner_(owner), id_(row)
	{
	}
	Row(Board<T, N>& owner, Location loc) noexcept
		: const_Row<T, N>(owner, loc.row()), owner_(owner), id_(loc.row())
	{
		assert(is_valid(loc));
	}

public:
	using const_Row<T, N>::size;
	using const_Row<T, N>::id;

	T& operator[](const int col) noexcept
	{
		assert(is_valid_size<N>(col));
		return no_check(Location(id(), col));
	}
	using iterator = iterator<T, N, self_type>;
	iterator begin() noexcept
	{
		return iterator(gsl::not_null<decltype(this)>{this});
	}
	iterator end() noexcept
	{
		return iterator(gsl::not_null<decltype(this)>{this}, size());
	}
	constexpr auto rbegin() { return std::make_reverse_iterator(end()); }
	constexpr auto rend() { return std::make_reverse_iterator(begin()); }

private:
	Board<T, N>& owner_; // const-pointer
	const int id_;

	T& no_check(Location loc) noexcept
	{
		assert(is_valid(loc));
		return owner_[loc];
	}
};

//===----------------------------------------------------------------------===//
template<typename T, int N>
class Col : public const_Col<T, N>
{
	using self_type = Col;
	using Location  = Sudoku::Location<N>;
	using const_Col = const_Col<T, N>;

	friend class Sudoku::Board<T, N>; // access private constructor

	friend class iterator<T, N, self_type>;
	friend class const_iterator<T, N, self_type>;

	Col(Board<T, N>& owner, int col) noexcept
		: const_Col(owner, col), owner_(owner)
	{
	}
	Col(Board<T, N>& owner, Location loc) noexcept
		: const_Col(owner, loc.col()), owner_(owner)
	{
		assert(is_valid(loc));
	}

public:
	using const_Col::id;
	using const_Col::size;

	T& operator[](const int row) noexcept
	{
		assert(is_valid_size<N>(row));
		return no_check(Location(row, id()));
	}
	using iterator = iterator<T, N, self_type>;
	iterator begin() noexcept
	{
		return iterator(gsl::not_null<decltype(this)>{this});
	}
	iterator end() noexcept
	{
		return iterator(gsl::not_null<decltype(this)>{this}, size());
	}
	constexpr auto rbegin() { return std::make_reverse_iterator(end()); }
	constexpr auto rend() { return std::make_reverse_iterator(begin()); }

private:
	Board<T, N>& owner_; // const-pointer

	T& no_check(Location loc) noexcept
	{
		assert(is_valid(loc));
		return owner_[loc];
	}
};

//===----------------------------------------------------------------------===//
template<typename T, int N>
class Block : public const_Block<T, N>
{
	using self_type   = Block;
	using const_Block = const_Block<T, N>;
	using Location    = Sudoku::Location<N>;

	friend class Sudoku::Board<T, N>; // access private constructor
	friend class iterator<T, N, self_type>;
	friend class const_iterator<T, N, self_type>;

	Block(Board<T, N>& owner, int id) noexcept
		: const_Block(owner, id), owner_(owner)
	{
	}
	Block(Board<T, N>& owner, Location loc) noexcept
		: const_Block(owner, loc.block()), owner_(owner)
	{
		assert(is_valid(loc));
	}

public:
	using const_Block::id;
	using const_Block::size;

	T& operator[](const int elem) noexcept
	{
		assert(is_valid_size<N>(elem));
		return no_check(Location_Block<N>(id(), elem));
	}
	using iterator = iterator<T, N, self_type>;
	iterator begin() noexcept
	{
		return iterator(gsl::not_null<decltype(this)>{this});
	}
	iterator end() noexcept
	{
		return iterator(gsl::not_null<decltype(this)>{this}, size());
	}
	constexpr auto rbegin() { return std::make_reverse_iterator(end()); }
	constexpr auto rend() { return std::make_reverse_iterator(begin()); }

private:
	Board<T, N>& owner_; // const-pointer

	T& no_check(Location loc) noexcept
	{
		assert(is_valid(loc));
		return owner_[loc];
	}
};


//===----------------------------------------------------------------------===//
// Board_Sections Iterators
//===----------------------------------------------------------------------===//
template<typename T, int N, typename ownerT>
class const_iterator
{
	using self_type  = const_iterator;
	using owner_type = ownerT;

public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type        = const T;
	using difference_type   = int;
	using reference         = value_type&;
	using pointer           = value_type*;

	const_iterator(gsl::not_null<const owner_type*> owner) noexcept
		: owner_(owner)
	{
	}
	const_iterator(gsl::not_null<const owner_type*> owner, int elem) noexcept
		: owner_(owner), elem_(elem)
	{
	}

	// All iterator categories
	self_type& operator++() noexcept
	{
		++elem_;
		return *this;
	}
	const self_type operator++(int) noexcept
	{
		const self_type pre{*this};
		operator++();
		return pre;
	}
	reference operator*() const noexcept
	{
		assert(is_valid_size<N>(elem_));
		return (*owner_)[elem_];
	}

	// Input iterator
	bool operator==(const self_type& other) const noexcept
	{
		assert(is_same(other));
		return elem_ == other.elem_ && is_same(other);
	}
	bool operator!=(const self_type& other) const noexcept
	{
		return (!(*this == other));
	}

	// In- & output iterator
	pointer operator->() const noexcept
	{ // member access; equivalent to (*p).member
		assert(is_valid_size<N>(elem_));
		return std::addressof<value_type>(owner_->operator[](elem_));
	}

	// Forward iterator
	const_iterator() noexcept : owner_()
	{ // construct with null pointer
	}

	// Bidirectional iterator
	self_type& operator--() noexcept // pre-decrement
	{
		--elem_;
		return *this;
	}
	const self_type operator--(int) noexcept // post-decrement
	{
		const self_type pre{*this};
		operator--();
		return pre;
	}

	// RandomAccess iterator
	self_type& operator+=(const difference_type offset) noexcept
	{
		elem_ += offset;
		return *this;
	}
	self_type operator+(const difference_type offset) const noexcept
	{
		self_type tmp{*this};
		return (tmp += offset);
	}
	self_type& operator-=(const difference_type offset) noexcept
	{
		return operator+=(-offset);
	}
	self_type operator-(const difference_type offset) const noexcept
	{
		self_type tmp{*this};
		return (tmp += -offset);
	}
	difference_type operator-(const self_type& other) const noexcept
	{
		assert(is_same(other));
		return elem_ - other.elem_;
	}
	reference operator[](const difference_type offset) const
	{
		return (*(*this + offset));
	}

	bool operator<(const self_type& other) const noexcept
	{
		assert(is_same(other));
		return elem_ < other.elem_;
	}
	bool operator>(const self_type& other) const noexcept
	{
		return (other < *this);
	}
	bool operator<=(const self_type& other) const noexcept
	{
		return (!(other < *this));
	}
	bool operator>=(const self_type& other) const noexcept
	{
		return (!(*this < other));
	}

	Location<N> location() const noexcept { return owner_->location(elem_); }

private:
	const owner_type* owner_;
	int elem_{0}; // element within the section

	bool is_same(const self_type& other) const noexcept
	{
		// compare address of:
		return (&(owner_->owner_) == &(other.owner_->owner_)) &&
			   (owner_->id() == other.owner_->id());
	}
};

//===----------------------------------------------------------------------===//
template<typename T, int N, typename ownerT>
class iterator : public Board_Section::const_iterator<T, N, ownerT>
{
	using self_type      = iterator;
	using owner_type     = ownerT;
	using const_iterator = const_iterator<T, N, ownerT>;

public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type        = T;
	using difference_type   = int;
	using reference         = value_type&;
	using pointer           = value_type*;

	iterator(gsl::not_null<owner_type*> owner) noexcept : owner_(owner) {}
	iterator(gsl::not_null<owner_type*> owner, int elem) noexcept
		: owner_(owner), elem_(elem)
	{
	}

	// All iterator categories
	self_type& operator++() noexcept
	{
		++elem_;
		return *this;
	}
	const self_type operator++(int) noexcept
	{
		const self_type pre = *this;
		operator++();
		return pre;
	}
	reference operator*() const noexcept
	{
		assert(is_valid_size<N>(elem_));
		return owner_->operator[](elem_);
	}

	// Input iterator
	bool operator==(const self_type& other) const noexcept
	{
		assert(is_same(other));
		return elem_ == other.elem_ && is_same(other);
	}
	bool operator!=(const self_type& other) const noexcept
	{
		return !(operator==(other));
	}

	// In- & output iterator
	pointer operator->() const noexcept
	{ // member access; equivalent to (*p).member
		assert(is_valid_size<N>(elem_));
		return std::addressof<value_type>(owner_->operator[](elem_));
	}

	// Forward iterator
	iterator() noexcept : owner_()
	{ // construct with null pointer
	}

	// Bidirectional iterator
	self_type& operator--() noexcept // pre-decrement
	{
		--elem_;
		return *this;
	}
	const self_type operator--(int) noexcept // post-decrement
	{
		const self_type pre{*this};
		operator--();
		return pre;
	}

	// RandomAccess iterator
	self_type& operator+=(const difference_type offset) noexcept
	{
		elem_ += offset;
		return *this;
	}
	self_type operator+(const difference_type offset) const noexcept
	{
		self_type tmp{*this};
		return (tmp += offset);
	}
	self_type& operator-=(const difference_type offset) noexcept
	{
		return operator+=(-offset);
	}
	self_type operator-(const difference_type offset) const noexcept
	{
		self_type tmp{*this};
		return (tmp += -offset);
	}
	difference_type operator-(const self_type& other) const noexcept
	{
		assert(is_same(other));
		return elem_ - other.elem_;
	}
	reference operator[](const difference_type offset) const
	{
		return (*(*this + offset));
	}
	bool operator<(const self_type& other) const noexcept
	{
		assert(is_same(other));
		return elem_ < other.elem_;
	}
	bool operator>(const self_type& other) const noexcept
	{
		return (other < *this);
	}
	bool operator<=(const self_type& other) const noexcept
	{
		return (!(other < *this));
	}
	bool operator>=(const self_type& other) const noexcept
	{
		return (!(*this < other));
	}

	Location<N> location() const noexcept { return owner_->location(elem_); }

private:
	owner_type* owner_;
	int elem_{0}; // element within the section

	bool is_same(const self_type& other) const noexcept
	{
		// compare address of:
		return (&(owner_->owner_) == &(other.owner_->owner_)) &&
			   (owner_->id() == other.owner_->id());
	}
};

} // namespace Sudoku::Board_Section
