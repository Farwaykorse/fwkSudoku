#pragma once

#include "Board.h"
#include "Location.h"

#include <iterator>

namespace Sudoku
{
template<typename T, size_t N, typename _Board>
class Section
{
public:
	using section_type = Section;
	using value_type = T;
	using const_value_type = const value_type;
	using size_type = unsigned int;
	using size_t = size_type;
	using difference_type = int;
	using self_type = Section;
	using reference = value_type&;
	using const_reference = const_value_type&;
	using pointer = value_type*;
	using const_pointer = const_value_type*;

	using Location = Location<N>;

	static const size_t base_size = Location().base_size;	// default 3
	static const size_t elem_size = Location().elem_size;	// default 9
	static const size_t full_size = Location().full_size;	// default 81

	class iterator;
	class const_iterator;


	// TODO shared code for derived classes: col, row and block
	// CPL: H3.2.2/3 / H20, p 577 / H21
	Section(_Board* owner, size_t id) : owner_(owner), id_(id) {}

	// element access
	T& at(size_t);
	const T& at(size_t) const;
	T& operator[](size_t elem) { return nocheck_at(elem); };
	const T& operator[](size_t elem) const { return const_nocheck_at(elem); };

	iterator begin() { return iterator(this, addressof(0), 0); }
	iterator end() { return iterator(this); }
	const_iterator cbegin() const { return const_iterator(this, addressof(0), 0); }
	const_iterator cend() const { return const_iterator(this); }
	const_iterator begin() const { return cbegin(); }
	const_iterator end() const { return cend(); }
	// rbegin(); rend(); crbegin(); crend();

	// information
	static constexpr size_t size() { return elem_size; }
	size_t id() const { return id_; }

	class iterator
	{
		// knows a pointer to an element in Board and the Location
		// can be passed to other section types, because Location is usable for all
	public:
		using iterator_category = std::random_access_iterator_tag;
			// forward_iterator_tag	// bidirectional_iterator_tag	// random_access_iterator_tag
		using self_type = iterator;
		using value_type = value_type;
		using reference = value_type&;
		using pointer = value_type*;
		using size_type = unsigned int;
		using difference_type = int;

		iterator(section_type* sp, pointer ptr, size_t elem) : elem_(elem), ptr_(ptr), section_(sp) {}
		iterator(section_type* sp, size_t elem) : elem_(elem), ptr_{ sp->addressof(elem) }, section_{ sp } {}
		iterator(section_type* sp) : elem_(elem_size), ptr_(nullptr), section_(sp) {}

		/* All iterator categories */
		iterator(const self_type&) = default;
		self_type& operator=(const self_type& other)
		{
			//ERROR no difference between row/col/block versions!!
			//TEST never tested
			section_ = other.section_;
			elem_ = other.elem_;
			return move_();
		}
		self_type& operator++() { ++elem_; return move_(); }
		self_type operator++(int)
		{
			self_type pre = *this;
			operator++();
			return pre;
		}

		// Input iterator
		bool operator==(const self_type& other) const
		{
			if (elem_ == other.elem_ && ptr_ == other.ptr_)
			{
				return section() == other.section();
			}
			return false;
		}
		bool operator!=(const self_type& other) const { return !(*this == other); }

		// In- & output iterator
		reference operator*() { return *ptr_; }
		pointer operator->() { return ptr_; } // member access; equivalent to (*p).m

		// Forward
		iterator() : elem_(elem_size), ptr_(nullptr), section_(nullptr) {}

		//Bidirectional
		self_type& operator--() { --elem_; return move_(); }
		self_type operator--(int)
		{
			self_type pre = *this;
			operator--();
			return pre;
		}

		//Random Access
		self_type& operator+=(difference_type n)	// compount assignment
		{
			assert(element() + n >= 0);
			elem_ += n;
			return move_();
		}
		self_type& operator-=(difference_type n)
		{
			assert(element() - n >= 0);
			elem_ -= n;
			return move_();
		}
		self_type operator+(difference_type n)
		{
			self_type itr{ *this };
			return itr += n;
		}
		self_type operator-(difference_type n)
		{
			self_type itr{ *this };
			return itr -= n;
		}
		difference_type operator-(const self_type& other)	// distance
		{
			assert(section() == other.section());
			return element() - other.element();
		}
		//TODO move/reverse_iterator<RandomIterator> operator+(difference_type, const move_iterator<RandomIterator>&)
		reference operator[](difference_type n) // offset dereference; p+n; equivalent to *(p+n)
		{
			self_type tmp{ *this };
			return *(tmp += n);
		}
		reference operator[](Location loc)
		{
			return operator[](element(loc)-element());
		}
		bool operator<(const self_type& other)	// inequality comparisons
		{
			assert(section() == other.section());
			return element() < other.element();
		}
		bool operator>(const self_type& other)
		{
			assert(section() == other.section());
			return element() > other.element();
		}
		bool operator<=(const self_type& other) { return !operator>(other); }
		bool operator>=(const self_type& other) { return !operator<(other); }

		/* custom */
		bool operator==(const Location& loc) const { return location() == loc; }
		bool operator!=(const Location& loc) const { return !operator==(loc); }
		Location location() const { return section_->loc(element()); }
		size_t element() const { return elem_; }
		size_t element(Location loc) const { return section_->element(loc); }

	private:
		size_type elem_{};	// id of the element within the section
		pointer ptr_{};		// data pointer
		section_type* section_;

		size_type section() const { return section_->id(); }

		self_type& move_()
		{
			ptr_ = section_->addressof(elem_);
			return *this;
		}
		self_type& move_(size_t to)
		{
			assert(to <= elem_size);
			elem_ = to;
			return move_();
		}
	};	// class Board::Section::iterator

	class const_iterator : public iterator
	{
		// knows a pointer to an element in Board and the Location
		// can be passed to other section types, because Location is usable for all
		// const iterators don't support output-iterator properties
	public:
		using iterator_category = std::random_access_iterator_tag;
			// forward_iterator_tag	// bidirectional_iterator_tag	// random_access_iterator_tag
		using self_type = const_iterator;
		using value_type = value_type;
		using const_value_type = const value_type;
		using reference = value_type&;
		using const_reference = const_value_type&;
		using pointer = value_type*;
		using const_pointer = const_value_type*;
		using size_type = unsigned int;
		using difference_type = int;

		/* constructors - used */
		const_iterator(const section_type* sp, const_pointer ptr, size_t elem) : elem_(elem), ptr_(ptr), section_(sp) {}
		const_iterator(const section_type* sp, size_t elem) : elem_(elem), ptr_{ sp->addressof(elem) }, section_(sp) {}
		const_iterator(const section_type* sp) : elem_(elem_size), ptr_(nullptr), section_(sp) {}

		/* All iterators categories */
		const_iterator(const self_type&) = default;
		self_type& operator=(const self_type& other)
		{
			//ERROR no difference between row/col/block versions!!
			//TEST never tested
			section_ = other.section_;
			elem_ = other.elem_;
			return move_();
		}
		self_type& operator++() { ++elem_; return move_(); }
		self_type operator++(int)	// post-increment, returns a copy holding the old value
		{
			self_type pre = *this;
			operator++();
			return pre;
		}

		// Input iterator
		bool operator==(const self_type& other) const
		{
			if (elem_ == other.elem_ && ptr_ == other.ptr_)
			{
				return section() == other.section();
			}
			return false;
		}
		bool operator!=(const self_type& other) const { return !(*this == other); }

		// In- & output iterator
		const_reference operator*() { return *ptr_; }
		const_pointer operator->() { return ptr_; }

		// Forward
		const_iterator() : elem_(elem_size), ptr_(nullptr), section_(nullptr) {}

		//Bidirectional
		self_type& operator--() { --elem_; return move_(); }
		self_type operator--(int)
		{
			self_type pre = *this;
			operator--();
			return pre;
		}

		//Random Access
		self_type& operator+=(difference_type n)
		{
			assert(elem_ + n >= 0);
			elem_ += n;
			return move_();
		}
		self_type& operator-=(difference_type n)
		{
			assert(elem_ - n >= 0);
			elem_ -= n;
			return move_();
		}
		self_type operator+(difference_type n)
		{
			self_type tmp{ *this };
			return tmp += n;
		}
		self_type operator-(difference_type n)
		{
			self_type tmp{ *this };
			return tmp -= n;
		}
		difference_type operator-(const self_type& other)	// distance
		{
			assert(section() == other.section());
			return element() - other.element();
		}
		//TODO move/reverse_iterator<RandomIterator> operator+(difference_type, const move_iterator<RandomIterator>&)
		const_reference operator[](difference_type n) // offset dereference; p+n; equivalent to *(p+n)
		{
			self_type tmp{ *this };
			return *(tmp += n);
		}
		const_reference operator[](Location loc)
		{
			return operator[](element(loc) - element());
		}

		bool operator<(const self_type& other)	// inequality comparisons
		{
			assert(section() == other.section());
			return element() < other.element();
		}
		bool operator>(const self_type& other)
		{
			assert(section() == other.section());
			return element() > other.element();
		}
		bool operator<=(const self_type& other) { return !operator>(other); }
		bool operator>=(const self_type& other) { return !operator<(other); }

		/* custom */
		bool operator==(const Location& loc) const { return location() == loc; }
		bool operator!=(const Location& loc) const { return !operator==(loc); }
		Location location() const { return section_->loc(elem_); }
		size_t element() const { return elem_; }
		size_t element(Location loc) const { return section_->element(loc); }

	private:
		size_type elem_{};		// id of the element within the section
		const_pointer ptr_{};	// data pointer
		const section_type* section_;

		size_type section() const { return section_->id(); }

		self_type& move_()
		{
			ptr_ = section_->addressof(elem_);
			return *this;
		}
		self_type& move_(size_t to)
		{
			assert(to <= elem_size);
			elem_ = to;
			return move_();
		}
	};	// class Board::Section::const_iterator

protected:
	pointer addressof(size_t elem_id)
	{
		assert(elem_id <= elem_size);
		if (elem_id == elem_size) { return nullptr; }
		return &nocheck_at(elem_id);
	}
	const_pointer addressof(size_t elem_id) const
	{
		assert(elem_id <= elem_size);
		if (elem_id == elem_size) { return nullptr; }
		return &const_nocheck_at(elem_id);
	}

private:
	_Board* const owner_;
	const size_t id_;

	virtual Location loc(size_t) const = 0;	// section type specific movement
	virtual size_t element(Location) const = 0;	// inverse of loc(size_t)
	//NOTE templates as alternative instead of virtual, //tem2//

	T& nocheck_at(size_t elem)
	{
		return owner_->operator[](loc(elem));
	}
	const T& const_nocheck_at(size_t elem) const
	{
		return owner_->operator[](loc(elem));
	}
};	// class Board::Section

// #################################################################################
// Section
template<typename T, size_t N, typename B>
inline T& Section<T, N, B>::at(size_t elem)
{
	if (elem >= size())
	{
		throw std::out_of_range{ "Section::at(size_t)" };
	}
	return nocheck_at(elem);
}

template<typename T, size_t N, typename B>
inline const T& Section<T, N, B>::at(size_t elem) const
{
	if (elem >= size())
	{
		throw std::out_of_range{ "Section::at(size_t) const" };
	}
	return const_nocheck_at(elem);
}

}	// namespace Sudoku