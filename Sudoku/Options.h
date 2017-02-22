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
	constexpr bool is_empty() const noexcept;

	int get_answer() const noexcept;	// return answer or 0 (use with is_answer())
	std::vector<int> available() const;	// return available options

	constexpr bool operator[](int value) const noexcept;
	//ERROR auto operator[](int value) noexcept;	//ERROR crashes Clang compiler

	bool operator==(int) const;			// shorthand for is_answer(int)
	bool operator==(const Options<E>&) const noexcept;
	bool operator!=(const Options<E>&) const noexcept;
	bool operator< (const Options<E>&) const noexcept;	//TODO allow sorting; usecase?

	// combine available options
	Options& operator+=(const Options&) noexcept;
	Options operator+(const Options&) const noexcept;
	// xor
	Options& operator^=(const Options&) noexcept;
	// return difference
	Options operator-(const Options&) const noexcept;	//TODO difference; usecase?
	// return shared options
	Options operator&(const Options&) const noexcept;

private:
	// 0th bit is "need to solve": false if answer has been set = inverse of answer
	std::bitset<E + 1> m_data{};

	int read_next() const noexcept;
	int read_next(int) const noexcept;
	Options& operator&=(const Options&) noexcept;	//NOTE might be risky while unused; private?
};

/// default constructor
template<int E>
Options<E>::Options() noexcept
{
	m_data.set(); // all true
}

template<int E> inline
Options<E>::Options(const std::bitset<E + 1>& other) :
	m_data(other)
{
	// empty constructor
}

template<int E>
inline Options<E>::Options(std::bitset<E + 1>&& other) :
	m_data{ other }
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
	std::swap(m_data, other);
	return *this;
}

/// remove all options
template<int E> inline
Options<E>& Options<E>::clear() noexcept
{
	m_data.reset(); // all false
	return *this;
}

/// set all options
template<int E> inline
Options<E>& Options<E>::reset() noexcept
{
	m_data.set(); // all true
	return *this;
}

template<int E> inline
Options<E>& Options<E>::flip() noexcept
{
	// retain answer flag value
	if (m_data[0]) { m_data.flip(0); }

	m_data.flip();
	return *this;
}

/// remove single option
template<int E> inline
bool Options<E>::remove(int value)
{
	if (m_data[value])
	{
		assert(!is_answer());	// don't apply on answers
		m_data.reset(value);
		return true;
	}
	return false;
}

/// add single option
template<int E> inline
Options<E>& Options<E>::add(int value)
{
	m_data.set(value, true);
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
	return m_data.size();	// bits
}

/// available options
/// if 1, a not processed answer
template<int E> inline
int Options<E>::count() const noexcept
{
	if (m_data[0]) { return m_data.count() - 1; }
	return 0;
}

///	(!) counts options, including set answers
template<int E> inline
int Options<E>::count_all() const noexcept
{
	if (m_data[0]) { return m_data.count() - 1; }
	return m_data.count();
}

// Test if all bits are set
template<int E> inline
bool Options<E>::all() const noexcept
{
	return m_data.all();
}

// if an option, or the answer
template<int E>
inline bool Options<E>::test(int value) const
{
	return m_data.test(value);
}

/// check if set to answer
template<int E> inline
bool Options<E>::is_answer() const noexcept
{
	return !(m_data[0] || m_data.none());
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

// Test if no options or answers available
template<int E> inline
constexpr bool Options<E>::is_empty() const noexcept
{
	return (m_data.none() || (m_data.count() == 1 && m_data[0] == true));
}

// determine the answer value, even if not marked
// use with is_answer() to determine if flaged as anwer
template<int E> inline
int Options<E>::get_answer() const noexcept
{
	//TODO	microbench simpler/faster way to read single value from m_data
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
	//?? static_assert(std::is_unsigned<int>(), "use unsigned to prevent undefined behaviour");
	assert(value <= E);
	return m_data[value];
}

///// no-check access
//template<int E> inline
//auto Options<E>::operator[](int value) noexcept
//{
//	static_assert(std::is_unsigned<int>(), "use unsigned to prevent undefined behaviour");
//	assert(value <= E);
//	return m_data[value];
//}

template<int E> inline
bool Options<E>::operator==(const Options<E>& other) const noexcept
{
	//?? what about the 0th 'is answer' bit?
	return m_data == other.m_data;
}

template<int E> inline
bool Options<E>::operator!=(const Options<E>& other) const noexcept
{
	return !(*this == other);
}

template<int E> inline
bool Options<E>::operator==(int value) const
{
	return is_answer(value);
}

///	available options, won't add an answer
template<int E> inline
Options<E>& Options<E>::operator+=(const Options& other) noexcept
{
	if (!other.is_answer())
	{
		m_data |= (other.m_data);
	}
	return *this;
}
template<int E> inline
Options<E> Options<E>::operator+(const Options& other) const noexcept
{
	Options<E> tmp(*this);
	return tmp += other;
}

// per element xor
template<int E> inline
Options<E>& Options<E>::operator^=(const Options& other) noexcept
{
	m_data ^= other.m_data;
	return *this;
}

template<int E> inline
Options<E>& Options<E>::operator&=(const Options& other) noexcept
{
	m_data &= other.m_data;
	return *this;
}

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
		if (m_data[i]) { return i; }
	}
	return 0;
}
}	// namespace Sudoku