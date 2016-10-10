/**	Options.h
 *	===
 *	Data object containing and managing available options.
 *	Template with element size.
 *	
 *	set(0)	->	is_answer() = true ; is_answer(0) = false ; count() = 0;
 */
#pragma once

#include "Location.h"

#include <cassert>
#include <bitset>
#include <vector>

namespace Sudoku
{
using val_t = unsigned int;

template<size_t E>
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
	Options(val_t value);
	Options& operator=(val_t value);
	Options& operator=(const std::bitset<E + 1>&) noexcept;
	Options& operator=(std::bitset<E + 1>&&) noexcept;
	~Options() = default;

	Options& clear() noexcept;			// remove all options
	Options& reset() noexcept;			// set all options
	Options& flip() noexcept;
	bool remove(val_t value);			// remove single option, return if needed
	//Options& remove(size_t value, ...); // TODO remove mentioned
	Options& add(val_t value);			// add single option
	Options& set(val_t value);			// set to answer

	size_t count() const noexcept;		// count available options
	size_t count_all() const noexcept;	// count all options (incl answer)
	bool test(val_t value) const;		// if an option, or answer
	bool is_answer() const noexcept;	// is set to answer
	bool is_answer(val_t value) const;	// is set to answer value
	bool is_option(val_t value) const;	// is an option
	constexpr bool is_empty() const noexcept;

	val_t answer() const noexcept;		// return answer or 0
	std::vector<val_t> available() const;	// return available options

	constexpr bool operator[](val_t value) const noexcept;
	//auto operator[](val_t value) noexcept;	//ERROR crashes Clang compiler

	bool operator==(const Options<E>&) const noexcept;
	bool operator!=(const Options<E>&) const noexcept;
	bool operator< (const Options<E>&) const noexcept;	//TODO allow sorting; usecase?
	/// shorthand for is_answer(val_t)
	bool operator==(val_t) const;
	// combine available options
	Options& operator+=(const Options&) noexcept;
	Options operator+(const Options&) const noexcept;
	// return difference
	Options operator-(const Options&) const noexcept;	//TODO difference; usecase?
	// return shared options
	Options operator&(const Options&) const noexcept;

private:
	/// 0th bit is true IF not yet set as answer
	std::bitset<E + 1> m_data{};

	val_t read_next() const noexcept;
	val_t read_next(val_t) const noexcept;
	Options& operator&=(const Options&) noexcept;	//NOTE might be risky while unused; private?
};

/// default constructor
template<val_t E>
Options<E>::Options() noexcept
{
	m_data.set(); // all true
}

template<size_t E> inline
Options<E>::Options(const std::bitset<E + 1>& other) :
	m_data(other)
{
	// empty constructor
}

template<size_t E>
inline Options<E>::Options(std::bitset<E + 1>&& other) :
	m_data{ other }
{
	// empty constructor
}

/// construct with single option set
template<size_t E>
Options<E>::Options(val_t value)
{
	assert(value <= E);
	set(value);
}

/// set to answer value
template<size_t E> inline
Options<E>& Options<E>::operator=(val_t value)
{
	return set(value);
}

template<size_t E> inline
Options<E>& Options<E>::operator=(const std::bitset<E + 1>& other) noexcept
{
	//TEST not triggered
	return Options(other);
}

template<size_t E> inline
Options<E>& Options<E>::operator=(std::bitset<E + 1>&& other) noexcept
{
	//TEST not triggered
	std::swap(m_data, other);
	return *this;
}

/// remove all options
template<size_t E> inline
Options<E>& Options<E>::clear() noexcept
{
	m_data.reset(); // all false
	return *this;
}

/// set all options
template<size_t E> inline
Options<E>& Options<E>::reset() noexcept
{
	m_data.set(); // all true
	return *this;
}

template<size_t E> inline
Options<E>& Options<E>::flip() noexcept
{
	// retain answer flag value
	if (m_data[0]) { m_data.flip(0); }

	m_data.flip();
	return *this;
}

/// remove single option
template<size_t E> inline
bool Options<E>::remove(val_t value)
{
	assert(!is_answer());	// don't apply on answers
	if (m_data[value])
	{
		m_data.reset(value);
		return true;
	}
	return false;
}

/// add single option
template<size_t E> inline
Options<E>& Options<E>::add(val_t value)
{
	m_data.set(value, true);
	return *this;
}

/// set to answer
template<size_t E> inline
Options<E>& Options<E>::set(val_t value)
{
	clear();
	add(value);	// if 0: -> not answer = [0] = true
	return *this;
}

/// available options
/// if 1, a not processed answer
template<size_t E> inline
size_t Options<E>::count() const noexcept
{
	if (m_data[0]) { return m_data.count() - 1; }
	return 0;
}

///	(!) counts options, including set answers
template<size_t E> inline
size_t Options<E>::count_all() const noexcept
{
	if (m_data[0]) { return m_data.count() -1; }
	return m_data.count();
}

/// if an options, or the answer
template<size_t E>
inline bool Options<E>::test(val_t value) const
{
	return m_data.test(value);
}

/// check if set to answer
template<size_t E> inline
bool Options<E>::is_answer() const noexcept
{
	//NOTE	edge case: will return true on empty
	// might be useful to support partial boards
	return !(m_data[0]);
}

/// check if set to answer value
template<size_t E> inline
bool Options<E>::is_answer(val_t value) const
{
	return (is_answer() && test(value));
}

/// check if an option
template<size_t E> inline
bool Options<E>::is_option(val_t value) const
{
	assert(value != 0);
	return (!is_answer() && test(value));
}

template<size_t E> inline
constexpr bool Options<E>::is_empty() const noexcept
{
	return (m_data.none() || (m_data.count() == 1 && m_data[0] == true) );
}

/// determine the anser value
template<size_t E> inline
val_t Options<E>::answer() const noexcept
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

template<size_t E> inline
std::vector<val_t> Options<E>::available() const
{
	std::vector<size_t> values{};
	values.reserve(count());
	if (!is_answer() && !is_empty())
	{
		val_t loc{ 0 };
		for (size_t i = 0; i < count(); ++i)
		{
			loc = read_next(loc);
			values.emplace_back(loc);
		}
	}
	return values;
}

/// no-check access read only
template<size_t E> inline
constexpr bool Options<E>::operator[](val_t value) const noexcept
{
	static_assert(std::is_unsigned<val_t>(), "use unsigned to prevent undefined behaviour");
	assert(value <= E);
	return m_data[value];
}

///// no-check access
//template<size_t E> inline
//auto Options<E>::operator[](val_t value) noexcept
//{
//	static_assert(std::is_unsigned<val_t>(), "use unsigned to prevent undefined behaviour");
//	assert(value <= E);
//	return m_data[value];
//}

template<size_t E> inline
bool Options<E>::operator==(const Options<E>& other) const noexcept
{
	//?? what about the 0th 'is answer' bit?
	return m_data == other.m_data;
}

template<size_t E> inline
bool Options<E>::operator!=(const Options<E>& other) const noexcept
{
	return !(*this==other);
}

template<size_t E> inline
bool Options<E>::operator==(val_t value) const
{
	return is_answer(value);
}

///	available options, won't add an answer
template<size_t E> inline
Options<E>& Options<E>::operator+=(const Options& other) noexcept
{
	if (!other.is_answer())
	{
		m_data |= (other.m_data);
	}
	return *this;
}
template<size_t E> inline
Options<E> Options<E>::operator+(const Options& other) const noexcept
{
	Options<E> tmp(*this);
	return tmp += other;
}

template<size_t E> inline
Options<E> & Options<E>::operator&=(const Options& other) noexcept
{
	m_data &= other.m_data;
	return *this;
}

template<size_t E> inline
Options<E> Options<E>::operator&(const Options& other) const noexcept
{
	Options<E> tmp(*this);
	return tmp &= other;
}

///	return next option in data
template<size_t E> inline
val_t Options<E>::read_next() const noexcept
{
	return read_next(0);
}

///	return next option in data
template<size_t E> inline
val_t Options<E>::read_next(val_t start) const noexcept
{
	++start;
	for (val_t i = start; i <= E; ++i)
	{
		if (m_data[i]) { return i; }
	}
	return 0;
}
}	// namespace Sudoku