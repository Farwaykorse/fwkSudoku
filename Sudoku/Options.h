/**	Options.h
*	===
*	Data object containing and managing available options.
*	Template with element size.
*
*	Note: the 0-bit is the inverse answer-bit. if [0]==0 the answer is set.
*
*/
#pragma once

#include "Location.h"

#include <cassert>
#include <bitset>
#include <vector>

namespace Sudoku
{
template<int E>
class Options
{
public:
	Options() noexcept;
	Options(const Options&) = default;
	Options& operator=(const Options&) = default;
	Options(Options&&) = default;
	Options& operator=(Options&&) = default;
	Options(const std::bitset<E + 1>&);	// 0th bit is last in input
	Options(std::bitset<E + 1>&&);
	Options(int value);
	Options& operator=(int value);
	Options& operator=(const std::bitset<E + 1>&) noexcept;
	Options& operator=(std::bitset<E + 1>&&) noexcept;
	~Options() = default;

	Options& clear() noexcept;			// remove all options
	Options& reset() noexcept;			// set all options
	Options& flip() noexcept;
	bool remove(int value);				// remove single option, return if needed
	//TODO Options& remove(int value, ...);	// remove mentioned
	Options& add(int value);			// add single option
	Options& set(int value);			// set to answer

	constexpr int size() const noexcept;
	int count() const noexcept;			// count available options
	int count_all() const noexcept;		// count all options (incl answer)
	bool all() const noexcept;			// if all options available = all bits set
	bool test(int value) const;			// if an option, or answer
	bool is_answer() const noexcept;	// is set to answer
	bool is_answer(int value) const;	// is set to answer value
	bool is_option(int value) const;	// is an option
	bool is_empty() const noexcept;

	int get_answer() const noexcept;	// return answer or 0 (won't confirm is_answer())
	std::vector<int> available() const;	// return available options

	constexpr bool operator[](int value) const noexcept;
	//ERROR auto operator[](int value) noexcept;	//ERROR crashes Clang compiler

	bool operator==(int) const noexcept;// shorthand for is_answer(int)
	bool operator==(const Options<E>&) const noexcept;
	bool operator!=(const Options<E>&) const noexcept;
	bool operator< (const Options<E>&) const noexcept;	//TODO allow sorting; usecase?

	// combine available options
	Options& operator+=(const Options&) noexcept;
	Options operator+(const Options&) const noexcept;
	// xor
	Options& XOR(const Options&) noexcept;
	// return difference
	Options operator-(const Options&) const noexcept;	//TODO difference; usecase?
	// return shared options
	Options operator&(const Options&) const noexcept;

private:
	// 0th bit is "need to solve": false if answer has been set = inverse of answer
	std::bitset<E + 1> data_{};

	int read_next() const noexcept;
	int read_next(int) const noexcept;
	Options& operator&=(const Options&) noexcept;	//NOTE might be risky while unused; private?
};

/// default constructor
template<int E>
Options<E>::Options() noexcept
{
	data_.set(); // all true
}

template<int E> inline
Options<E>::Options(const std::bitset<E + 1>& other) :
	data_(other)
{
	// empty constructor
}

template<int E>
inline Options<E>::Options(std::bitset<E + 1>&& other) :
	data_{ other }
{
	// empty constructor
}

/// construct with single option set
template<int E>
Options<E>::Options(int value)
{
	assert(value <= E);
	set(value);
}

/// set to answer value
template<int E> inline
Options<E>& Options<E>::operator=(int value)
{
	return set(value);
}

template<int E> inline
Options<E>& Options<E>::operator=(const std::bitset<E + 1>& other) noexcept
{
	//TEST not triggered
	return Options(other);
}

template<int E> inline
Options<E>& Options<E>::operator=(std::bitset<E + 1>&& other) noexcept
{
	//TEST not triggered
	std::swap(data_, other);
	return *this;
}

/// remove all options
template<int E> inline
Options<E>& Options<E>::clear() noexcept
{
	data_.reset(); // all false
	return *this;
}

/// set all options
template<int E> inline
Options<E>& Options<E>::reset() noexcept
{
	data_.set(); // all true
	return *this;
}

template<int E> inline
Options<E>& Options<E>::flip() noexcept
{
	// retain answer flag value
	if (data_[0]) { data_.flip(0); }

	data_.flip();
	return *this;
}

/// remove single option
template<int E> inline
bool Options<E>::remove(int value)
{
	if (data_[static_cast<size_t>(value)])
	{
		assert(!is_answer());	// don't apply on answers
		data_.reset(static_cast<size_t>(value));
		return true;
	}
	return false;
}

/// add single option
template<int E> inline
Options<E>& Options<E>::add(int value)
{
	data_.set(static_cast<size_t>(value), true);
	return *this;
}

/// set to answer
template<int E> inline
Options<E>& Options<E>::set(int value)
{
	clear();
	add(value);	// if 0: -> not answer = [0] = true
	return *this;
}

template<int E> inline
constexpr int Options<E>::size() const noexcept
{
	return static_cast<int>(data_.size());	// bits
}

/// available options
/// if 1, a not processed answer
template<int E> inline
int Options<E>::count() const noexcept
{
	if (data_[0]) { return static_cast<int>(data_.count()) - 1; }
	return 0;
}

///	(!) counts options, including set answers
template<int E> inline
int Options<E>::count_all() const noexcept
{
	if (data_[0]) { return static_cast<int>(data_.count()) - 1; }
	return static_cast<int>(data_.count());
}

//_Test if all bits are set
template<int E> inline
bool Options<E>::all() const noexcept
{
	return data_.all();
}

// if an option, or the answer
template<int E>
inline bool Options<E>::test(int value) const
{
	return data_.test(static_cast<size_t>(value));
}

/// check if set to answer
template<int E> inline
bool Options<E>::is_answer() const noexcept
{
	return !(data_[0] || data_.none());
}

/// check if set to answer value
template<int E> inline
bool Options<E>::is_answer(int value) const
{
	return (is_answer() && test(value));
}

/// check if an option
template<int E> inline
bool Options<E>::is_option(int value) const
{
	assert(value != 0);
	return (!is_answer() && test(value));
}

//_Test if no options or answers available
template<int E> inline
bool Options<E>::is_empty() const noexcept
{
	return (data_.none() || (data_.count() == 1 && data_[0] == true));
}

// determine the answer value, even if not marked
// use with is_answer() to determine if flaged as anwer
template<int E> inline
int Options<E>::get_answer() const noexcept
{
	//TODO	microbench simpler/faster way to read single value from data_
	//		constexpr? 
	//		bit operations?
	if (is_answer() || count() == 1)
	{
		return read_next();
	}
	return 0;
}

template<int E> inline
std::vector<int> Options<E>::available() const
{
	std::vector<int> values{};
	values.reserve(count());
	if (!is_answer() && !is_empty())
	{
		int loc{ 0 };
		for (int i = 0; i < count(); ++i)
		{
			loc = read_next(loc);
			values.emplace_back(loc);
		}
	}
	return values;
}

/// no-check access read only
template<int E> inline
constexpr bool Options<E>::operator[](int value) const noexcept
{
	assert(value <= E);
	return data_[value];
}

///// no-check access
//template<int E> inline
//auto Options<E>::operator[](int value) noexcept
//{
//	static_assert(std::is_unsigned<int>(), "use unsigned to prevent undefined behaviour");
//	assert(value <= E);
//	return data_[value];
//}

template<int E> inline
bool Options<E>::operator==(const Options<E>& other) const noexcept
{
	//?? operator== what about the 0th 'is answer' bit?
	return data_ == other.data_;
}

template<int E> inline
bool Options<E>::operator!=(const Options<E>& other) const noexcept
{
	return !(*this == other);
}

template<int E> inline
bool Options<E>::operator==(int value) const noexcept
{
	return is_answer(value);
}

///	available options, won't add an answer
template<int E> inline
Options<E>& Options<E>::operator+=(const Options& other) noexcept
{
	if (!other.is_answer())
	{
		data_ |= (other.data_);
	}
	return *this;
}
template<int E> inline
Options<E> Options<E>::operator+(const Options& other) const noexcept
{
	Options<E> tmp(*this);
	return tmp += other;
}

// Per element XOR, exclusive OR
template<int E> inline
Options<E>& Options<E>::XOR(const Options& other) noexcept
{
	data_ ^= other.data_;
	return *this;
}

// Retain only shared options
template<int E> inline
Options<E>& Options<E>::operator&=(const Options& other) noexcept
{
	data_ &= other.data_;
	return *this;
}
// Shared options
template<int E> inline
Options<E> Options<E>::operator&(const Options& other) const noexcept
{
	Options<E> tmp(*this);
	return tmp &= other;
}

///	return next option in data
template<int E> inline
int Options<E>::read_next() const noexcept
{
	return read_next(0);
}

///	return next option in data
template<int E> inline
int Options<E>::read_next(int start) const noexcept
{
	++start;
	for (int i = start; i <= E; ++i)
	{
		if (data_[static_cast<size_t>(i)]) { return i; }
	}
	return 0;
}

//////////////////////////////
///// external functions /////
//////////////////////////////

// Exclusive OR
template<int E> inline
Options<E> XOR(const Options<E>& A, const Options<E>& B) noexcept
{
	Options<E> tmp{ A };
	return tmp.XOR(B);
}

// Shared options
template<int E> inline
Options<E> shared(const Options<E>& A, const Options<E>& B) noexcept
{
	return A & B;
}



}	// namespace Sudoku