//===--- Sudoku/Options.h												---===//
//
// Data object containing and managing available options.
//===----------------------------------------------------------------------===//
// Templated with element size.
//
// The 0-bit is the inverse answer-bit. if [0]==0 the answer is set.
//
//===----------------------------------------------------------------------===//
#pragma once

#include "Value.h"
#include <gsl/gsl>
#include <bitset>
#include <vector>
#include <utility>
#include <cassert>

namespace Sudoku
{
template<int E>
class Options
{
	using bitset = std::bitset<E + 1>;

public:
	Options() noexcept;
	explicit Options(const bitset&) noexcept; // 0th bit is last in input
	explicit Options(bitset&&) noexcept;
	constexpr Options(Value) noexcept;
	Options& operator=(Value) noexcept;
	Options& operator=(const bitset&) noexcept;
	Options& operator=(bitset&&) noexcept;

	Options& clear() noexcept; // remove all options
	Options& reset() noexcept; // set all options
	Options& flip() noexcept;
	Options& remove_option(Value) noexcept; // remove single option
	// TODO Options& remove_option(int value, ...);	// remove mentioned
	Options& add(Value);                  // add single option
	Options& set(Value);                  // set to answer
	Options& add_nocheck(Value) noexcept; // add single option
	Options& set_nocheck(Value) noexcept; // set to answer

	constexpr size_t size() const noexcept;
	int count() const noexcept;      // count available options
	int count_all() const noexcept;  // count all options (incl answer)
	bool all() const noexcept;       // if all options available = all bits set
	bool test(Value) const;          // if an option, or answer
	bool is_answer() const noexcept; // is set to answer
	bool is_answer(Value) const noexcept; // is set to answer value
	bool is_option(Value) const noexcept; // is an option
	bool is_empty() const noexcept;

	Value get_answer() const noexcept;    // return answer or 0
										  // (won't confirm is_answer())
	std::vector<Value> available() const; // return available options

	bool operator[](Value) const noexcept;
	auto operator[](Value) noexcept;

	bool operator==(const Options<E>&) const noexcept;
	bool operator<(const Options<E>&) const noexcept;

	// combine available options
	Options& operator+=(const Options&) noexcept;
	// xor
	Options& XOR(const Options&) noexcept;

	// Debug Use Only, don't depend on it's result
	std::string DebugString() const;

private:
	// 0th bit is "need to solve":
	// false if answer has been set = inverse of answer
	bitset data_{};

	Value read_next(Value start = Value{0}) const noexcept;
	Options& operator&=(const Options&) noexcept; // NOTE might be risky
	template<int E>
	friend Options<E> operator&(const Options<E>&, const Options<E>&)noexcept;

}; // class Options

//===-- free-functions ---------------------------------------------------===//

template<int E>
bool operator!=(const Options<E>&, const Options<E>&) noexcept;

template<int E>
bool operator==(const Options<E>&, Value) noexcept;
template<int E>
bool operator==(Value, const Options<E>&) noexcept;
template<int E>
bool operator!=(const Options<E>&, Value) noexcept;
template<int E>
bool operator!=(Value, const Options<E>&) noexcept;

template<int E>
inline Options<E> XOR(Options<E>& A, Options<E>& B) noexcept;

template<int E>
Options<E> operator+(const Options<E>&, const Options<E>&) noexcept;

template<int E>
Options<E> operator-(const Options<E>&, const Options<E>&) noexcept;

// return shared options
template<int E>
inline Options<E> shared(Options<E>& A, Options<E>& B) noexcept;
template<int E>
Options<E> operator&(const Options<E>&, const Options<E>&)noexcept;

//===---------------------------------------------------------------------===//

namespace
{
	// convert to a number for use in std::bitset to use a unique bit per value
	constexpr size_t exp2_(size_t value) noexcept
	{
		return (value < 1) ? 1 : (2 * exp2_(--value));
	}
	static_assert(exp2_(0) == 1);
	static_assert(exp2_(1) == 2);
	static_assert(exp2_(2) == 4);
	static_assert(exp2_(3) == 8);
	static_assert(exp2_(4) == 16);
	static_assert(exp2_(5) == 32);
	static_assert(exp2_(6) == 64);
	static_assert(exp2_(7) == 128);
	static_assert(exp2_(8) == 256);
	static_assert(exp2_(9) == 512);
} // namespace

//===---------------------------------------------------------------------===//

//	construct with all options
template<int E>
Options<E>::Options() noexcept
{
	data_.set(); // all true
}

template<int E>
inline Options<E>::Options(const bitset& other) noexcept : data_(other)
{ // empty constructor
}

template<int E>
inline Options<E>::Options(bitset&& other) noexcept : data_{other}
{ // empty constructor
}

//	construct with single option set to answer
template<int E>
constexpr Options<E>::Options(const Value value) noexcept
	: data_{exp2_(size_t(value))}
{
	assert(value <= Value{E});
}

//	set to answer value
template<int E>
inline Options<E>& Options<E>::operator=(const Value value) noexcept
{
	data_ = exp2_(size_t(value));
	return *this;
}

template<int E>
inline Options<E>& Options<E>::operator=(const bitset& other) noexcept
{
	data_ = other;
	return *this;
}

template<int E>
inline Options<E>& Options<E>::operator=(bitset&& other) noexcept
{
	std::swap(data_, other); // use <utility>
	return *this;
}

//	remove all options
template<int E>
inline Options<E>& Options<E>::clear() noexcept
{
	data_.reset(); // all false
	//???	Note: anwerbit unset too -> answered?
	return *this;
}

//	set all options
template<int E>
inline Options<E>& Options<E>::reset() noexcept
{
	data_.set(); // all true
	return *this;
}

template<int E>
inline Options<E>& Options<E>::flip() noexcept
{
	// retain answer flag value
	data_[0].flip(); // noexcept

	data_.flip(); // noexcept
	return *this;
}

//	remove single option
template<int E>
inline Options<E>& Options<E>::remove_option(const Value value) noexcept
{
	assert(value <= Value{E});
	assert(!is_answer(value));

	data_[size_t(value)] = false;
	return *this;
}

//	add single option
template<int E>
inline Options<E>& Options<E>::add(const Value value)
{
	data_.set(value, true);
	return *this;
}

//	add single option
template<int E>
inline Options<E>& Options<E>::add_nocheck(const Value value) noexcept
{
	assert(value <= Value{E});
	data_[size_t(value)] = true;
	return *this;
}

//	set to answer
template<int E>
inline Options<E>& Options<E>::set(const Value value)
{
	clear();
	return add(value); // if 0: -> not answer = [0] = true
}

//	set to answer
template<int E>
inline Options<E>& Options<E>::set_nocheck(const Value value) noexcept
{
	clear();
	return add_nocheck(value);
}

template<int E>
inline constexpr size_t Options<E>::size() const noexcept
{
	return data_.size(); // bits

	//??? size() -1 better?
	// The current implementation works with size() being 1 past the last
	// element But the usage allows for size()-1 options to be stored Where the
	// direct value to location implementation is just convenient The 0th
	// element in this immplementation is just a flag (more could be added)
}

//	available options
//	if 1, a not processed answer
template<int E>
inline int Options<E>::count() const noexcept
{
	if (data_[0])
	{
		return static_cast<int>(data_.count()) - 1;
	}
	return 0; // NO protection vs incorrect answer bit
}

//	(!) counts options, including set answers == ingores answer-bit
//	Returns 1 if {1 option, set as answer} || {1 option, not set as anwer}
//	Returns 0 if {empty} || {no option, but not set as answer}
template<int E>
inline int Options<E>::count_all() const noexcept
{
	if (data_[0])
	{
		return static_cast<int>(data_.count()) - 1;
	}
	return static_cast<int>(data_.count());
}

//_Test if all bits are set
template<int E>
inline bool Options<E>::all() const noexcept
{
	return data_.all();
}

//	if an option, or the answer
template<int E>
inline bool Options<E>::test(const Value value) const
{
	return data_.test(size_t(value));
}

//	check if set to answer
template<int E>
inline bool Options<E>::is_answer() const noexcept
{
	return !data_[0] && data_.count() == 1;
}

//	check if set to answer value
//! no input checks!
template<int E>
inline bool Options<E>::is_answer(const Value value) const noexcept
{
	assert(value <= Value{E});
	return (is_answer() && operator[](value));
	// return *this == Options<E>(value);
	// return data_ == std::bitset<E + 1>{static_cast<unsigned long
	// long>(exp2_(value))};
}

//	check if option available
//! no input checks!
template<int E>
inline bool Options<E>::is_option(const Value value) const noexcept
{
	assert(value != Value{0} && value <= Value{E});
	// benched: this order is ~1.3 times faster
	return (operator[](value) && !is_answer());

	//! ... much slower ...
	// constexpr auto not_answered =
	//	std::bitset<E + 1>{static_cast<unsigned long long>(1)};
	// return data_ ==
	//	   ((std::bitset<E + 1>{static_cast<unsigned long long>(exp2_(value))}
	//|= 		 not_answered) |= data_);
}

//_Test if no options or answers available
template<int E>
inline bool Options<E>::is_empty() const noexcept
{
	// return (data_.none() || (data_.count() == 1 && data_[0] == true));
	return (data_.none() || data_ == std::bitset<E + 1>{1});
}

//	determine the answer value, even if not marked
//	use with is_answer() to determine if flaged as anwer
template<int E>
inline Value Options<E>::get_answer() const noexcept
{
	// TODO	microbench simpler/faster way to read single value from data_
	//		constexpr?
	//		bit operations?
	if (count_all() == 1)
	{
		return read_next();
	}
	return Value{0};
}

//	all available options
template<int E>
inline std::vector<Value> Options<E>::available() const
{
	std::vector<Value> values{};
	values.reserve(static_cast<size_t>(count()));
	if (!is_answer() && !is_empty())
	{
		Value item{0};
		for (auto i{0}; i < count(); ++i)
		{
			item = read_next(item);
			values.emplace_back(item);
		}
	}
	return values;
}

//	no-check access read only
template<int E>
inline bool Options<E>::operator[](const Value value) const noexcept
{
	assert(value <= Value{E});
	return data_[size_t(value)];
}

//	no-check access
template<int E>
inline auto Options<E>::operator[](const Value value) noexcept
{
	assert(value <= Value{E});
	return data_[size_t(value)];
}

template<int E>
inline bool Options<E>::operator==(const Options& other) const noexcept
{
	//?? operator== what about the 0th 'is answer' bit?
	return data_ == other.data_;
}

template<int E>
inline bool operator!=(const Options<E>& left, const Options<E>& right) noexcept
{
	return !(left == right);
}

// short for is_answer(value)
template<int E>
inline bool operator==(const Options<E>& left, const Value value) noexcept
{
	return left.is_answer(value);
}
template<int E>
inline bool operator==(const Value value, const Options<E>& right) noexcept
{
	return right.is_answer(value);
}
template<int E>
inline bool operator!=(const Options<E>& left, const Value value) noexcept
{
	return not(left.is_answer(value));
}
template<int E>
inline bool operator!=(const Value value, const Options<E>& right) noexcept
{
	return not(right.is_answer(value));
}

//	Basis for sorting
template<int E>
bool Options<E>::operator<(const Options<E>& other) const noexcept
{
	return data_.to_ulong() < other.data_.to_ulong();
}

//	Combine available options (binary OR)
template<int E>
inline Options<E>& Options<E>::operator+=(const Options& other) noexcept
{
	data_ |= (other.data_);
	return *this;
}
//	Combine (binary OR)
template<int E>
inline Options<E>
	operator+(const Options<E>& left, const Options<E>& right) noexcept
{
	Options<E> tmp{left};
	return tmp += right;
}

//	Per element (binary) XOR, exclusive OR
template<int E>
inline Options<E>& Options<E>::XOR(const Options& other) noexcept
{
	data_ ^= other.data_;
	return *this;
}

// Only in left
template<int E>
inline Options<E>
	operator-(const Options<E>& left, const Options<E>& right) noexcept
{
	return left & XOR(left, right);
}

// Exclusive OR
template<int E>
inline Options<E> XOR(const Options<E>& A, const Options<E>& B) noexcept
{
	Options<E> tmp{A};
	return tmp.XOR(B);
}

//	Retain only shared options (binary AND)
template<int E>
inline Options<E>& Options<E>::operator&=(const Options& other) noexcept
{
	data_ &= other.data_;
	return *this;
}
//	Shared options (binary AND)
//	Prefere: shared(left, right)
template<int E>
inline Options<E>
	operator&(const Options<E>& left, const Options<E>& right) noexcept
{
	Options<E> tmp{left};
	return tmp &= right;
}

// Shared options
template<int E>
inline Options<E> shared(const Options<E>& A, const Options<E>& B) noexcept
{
	return A & B;
}

template<int E>
inline std::string Options<E>::DebugString() const
{
	return data_.to_string();
}

//	return next option in data
template<int E>
inline Value Options<E>::read_next(Value start) const noexcept
{ // default value start = 0
	unsigned int i{start};
	++i;
	for (; i <= static_cast<unsigned int>(E); ++i)
	{
		if (data_[i])
		{
			return Value{i};
		}
	}
	return Value{0}; // never triggered
}

} // namespace Sudoku
