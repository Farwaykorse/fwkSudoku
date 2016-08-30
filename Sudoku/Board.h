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
public:
	static_assert(N > 1, "Board base_size value too small");

	using value_type = T;	// stl container conventions
	using size_type = unsigned int;
	using size_t = size_type;
	using difference_type = int;
	using self_type = Board<T, N>;
	using reference = value_type&;
	using pointer = value_type*;

	using Location = Location<N>;
	using Block_Loc = Block_Loc<N>;
	class Section;
	class Row;
	class Col;
	class Block;

	static constexpr size_t base_size = N;	// 3 for default 9*9 board
	static constexpr size_t elem_size = base_size * base_size;
	static constexpr size_t full_size = elem_size * elem_size;

	static_assert(base_size < elem_size && base_size < full_size && elem_size < full_size,
				  "Board size out of bounds");

	Board();
	explicit Board(const T&);
	Board(self_type&&) = default;		// move
	Board& operator=(self_type&&) = default;
	Board(const self_type&) = default;	// copy
	Board& operator=(const Board&);
	~Board() = default;

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
	T& at(size_t row, size_t col);
	const T& at(size_t row, size_t col) const;
	T& at(size_t elem);
	const T& at(size_t elem) const;

	/* Iterators */
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	iterator begin();
	const_iterator cbegin() const;
	const_iterator begin() const { return cbegin(); }
	iterator end();
	const_iterator cend() const;
	const_iterator end() const { return cend(); }




	class Section
	{
	public:
		class iterator;
		friend class iterator;
		//using value_type = value_type;
		//using size_type = size_t;
		//using difference_type = int;
		//using reference = value_type&;
		//using pionter = value_type*;


		// TODO shared code for derived classes: col, row and block
		// CPL: H3.2.2/3 / H20, p 577 / H21
		// interface inheritance (= run-time/dynamic polymorphism
		// implementation inheritance - shared facilities

		Section(Board* owner, size_t id) : owner_(owner), id_(id) {}

		// element access / updates loc_ to new location
		T& at(size_t);
		const T& at(size_t) const;
		T& operator[](size_t elem) { return nocheck_at(elem); };
		const T& operator[](size_t elem) const { return const_nocheck_at(elem); };

		iterator begin() { return iterator(this, addressof(0), 0); }
		iterator end()
		{
			//return begin() + elem_size;
			return iterator(this);
		}
		//T& begin(); end(); cbegin(); cend(); rbegin(); rend(); crbegin(); crend();

		// information
		static constexpr size_t size() { return elem_size; }
		constexpr size_t id() const { return id_; }
		//constexpr size_t id_row() const { return loc_.row(); }
		//constexpr size_t id_col() const { return loc_.col(); }
		//constexpr size_t id_block() const { return loc_.block(); }


		class iterator
		{
			// knows a pointer to an elemend in Board and the Location
			// can be passed to other section types, because Location is usable for all
		public:
			using iterator_category = std::forward_iterator_tag;
				// forward_iterator_tag	// bidirectional_iterator_tag	// random_access_iterator_tag
			using self_type = iterator;
			using value_type = value_type;
			using reference = value_type&;
			using pointer = value_type*;
			using size_type = unsigned int;
			using difference_type = int;

			iterator(Section* sp, size_t elem) : section_{sp}, elem_(elem), ptr_{ sp->addressof(elem) } {}
			iterator(Section* sp, pointer ptr, size_t elem) : section_(sp), ptr_(ptr), elem_(elem) {}
			iterator(Section* sp) : section_(sp), ptr_(nullptr), elem_(elem_size) {}
			// All
			iterator(const self_type&) = default; 
			self_type& operator=(const self_type& other)
			{
				//ERROR no difference between row/col/block versions!!
				section_ = other.section_;
				elem_ = other.elem_;
				ptr_ = other.ptr_;
				return *this&;
			}
			self_type& operator++()	// pre-increment, returns a reference
			{
				++elem_;
				return move_();
			}
			self_type operator++(int)	// post-increment, returns a copy holding the old value
			{
				self_type pre = *this;
				++elem_;
				move_();
				return pre;
			}

			// Input iterator
			bool operator==(const self_type& other) const
			{
				if (elem_ == other.elem_ && ptr_ == other.ptr_)
				{
					return true;
					//TODO section_ is not dependable
					//return section_ == other.section_;
				}
				return false;
			}
			bool operator!=(const self_type& other) const { return !(*this == other); }
			// In- & output iterator
			reference operator*() { return *ptr_; }
			pointer operator->() // member access; equivalent to (*p).m
			{
				//TODO template: only possible for non-simple types (not int)
				return ptr_;
			}

			// Forward
			iterator() : elem_(elem_size), ptr_(nullptr), section_(nullptr) {}

			//Bidirectional
			//reference operator--();
			//self_type operator--(int);

			//Random Access
			self_type& operator+=(difference_type n)	// compount assignment
			{
				elem_ += n;
				return move_();
			}
			self_type& operator-=(difference_type n) { elem_ -= n; return move_(); }
			self_type& operator+(difference_type n)
			{
				return operator+=(n);
			}
			self_type& operator-(difference_type n) { return operator -= n; }
			//operator+(b);
			//operator-(b);
			reference operator[](difference_type n) // offset dereference; p+n; equivalent to *(p+n)
			{
				elem_ += n;
				move_();
				return *ptr_;
			}
			//operator<(b);		// inequality comparisons
			//operator<=(b);
			//operator>=(b);
			//operator>(b);


			//Location& advance(size_t elem);	// like p+=n requires at least an input iterator
			//size_t distance(Location, Location); // like x=q-p
			//Location next(Location, size_t); // like q=p+n; p at least forward iterator
			//Location next(Location); // like next(p,1)
			//Location pref(Location, size_t); // like q=p-n; p at least bidirectional iterator
			//Location pref(Location); // like prev(p,1)

		private:
			constexpr size_type section() const { return id(); }
			size_type elem_{};	// id of the element within the section
			pointer ptr_{};
			Section* section_;


			self_type& move_()
			{
				ptr_ = section_->addressof(elem_);
				return *this;
			}
			self_type& move_(size_t to)
			{
				elem_ = to;
				return move_();
			}
		};

		pointer addressof(size_t elem_id)
		{
			assert(elem_id <= elem_size);
			if (elem_id == elem_size) { return nullptr; }
			return &nocheck_at(elem_id);
		}

	private:
		Board* const owner_;
		const size_t id_;

		virtual Location loc(size_t) const = 0;	// section type specific movement
		//NOTE templates as alternative instead of virtual, //tem2//

		T& nocheck_at(size_t elem)
		{
			return owner_->board[loc(elem).element()];
		}
		const T& const_nocheck_at(size_t elem) const
		{
			return owner_->board[loc(elem).element()];
		}
	};


	class Row : public Section
	{
	public:
		using Section::Section;	// inherit constructors
		using iterator = Section::iterator;

		/*
		Pointers and References
		both mechanisms for referring to an object from different places without copying
		/
		Use a pointer if:
		Need to change which object to refere to (pointers can be edited)
		can use = += -= ++ --
		a collection of something that refers to an object
		string* a[] = {&x, &y}; and vector<string*> s = {&x, &y};
		if a no value option is needed (nullptr)
		Use a reference if:
		name always refers to the same object
		use a user-defined (overloaded) operator on a class
		M operator+(const M&, const M&); won't work with pointers
		/
		lvalue reference			objects to change
		int& r1 {1}				needs initializer that is an lvalue of the required type
		extern int& r2			initialized elswhere
		const references			objects not to change
		const double& cdr {1}	needs initializer, but can do implicit conversion
		rvalue reference			temporary objects, won't perserve after use
		string& rr {"Ohoho"}	used to implement a "destructive read" for optimization of what would otherwise require a copy
		moving the data only the new will remain
		/
		Questions:
		why not use a reference to board in place of a const-pointer? (in Section)
		CPL: 7.6 pointers that represent ownership inside handle classes
		*/
	private:
		Location loc(size_t elem) const final
		{
			return Location(id(), elem);
		}
	};

	class Col : public Section
	{
	public:
		using Section::Section;	// inherit constructors
	private:
		Location loc(size_t elem) const final { return Location(elem, id()); }
	};

	class Block : public Section
	{
	public:
		using Section::Section;	// inherit constructors
	private:
		Location loc(size_t elem) const override
		{
			return Location(Block_Loc(id(), elem));
		}
	};

	Row row(size_t id) { return Row(this, id); }
	Col col(size_t id) { return Col(this, id); }
	Block block(size_t id) { return Block(this, id); }

	/*	Element Selection Operator (using a proxy object)
	*	usable as [row][col] where col is processed by the (const_)InBetween
	*/
	class InBetween;
	InBetween operator [] (size_t row);
	class const_InBetween;
	const const_InBetween operator [] (size_t row) const;

private:
	std::vector<T> board{};

	void valid_dimensions() const;
	bool valid_size(const size_t elem) const;
	bool valid_size(const size_t row, const size_t col) const;
};









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

template<typename T, size_t N>
class Board<T, N>::InBetween
{
public:
	friend class Board;
	T& operator[] (size_t col) { return owner->board[Location(row, col).element()]; };
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
	friend class Board;
	const T& operator[] (size_t col) const { return owner->board[location(row, col)]; };
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

template<typename T, size_t N>
inline T& Board<T, N>::Section::at(size_t elem)
{
	if (elem >= size())
	{
		throw std::out_of_range{ "Section::at(size_t)" };
	}
	return nocheck_at(elem);
}

template<typename T, size_t N>
inline const T& Board<T, N>::Section::at(size_t elem) const
{
	if (elem >= size())
	{
		throw std::out_of_range{ "Section::at(size_t) const" };
	}
	return const_nocheck_at(elem);
}
} // namespace Sudoku