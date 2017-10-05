//===--	Sudoku/Options.h												--===//
//
//	Data object containing and managing available options.
//	Template with element size.
//	Note: the 0-bit is the inverse answer-bit. if [0]==0 the answer is set.
//
//===---------------------------------------------------------------------===//
#pragma once

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
	Options(const Options&) = default;
	Options& operator=(const Options&) = default;
	Options(Options&&)                 = default;
	Options& operator=(Options&&) = default;
	Options(const bitset&); // 0th bit is last in input
	Options(bitset&&);
	Options(int value);
	Options& operator=(int value);
	Options& operator=(const bitset&) noexcept;
	Options& operator=(bitset&&) noexcept;
	~Options()       = default;

	Options& clear() noexcept; // remove all options
	Options& reset() noexcept; // set all options
	Options& flip() noexcept;
	Options& remove_option(int value) noexcept; // remove single option
	// TODO Options& remove_option(int value, ...);	// remove mentioned
	Options& add(int value);                  // add single option
	Options& set(int value);                  // set to answer
	Options& add_nocheck(int value) noexcept; // add single option
	Options& set_nocheck(int value) noexcept; // set to answer

	constexpr int size() const noexcept;
	int count() const noexcept;      // count available options
	int count_all() const noexcept;  // count all options (incl answer)
	bool all() const noexcept;       // if all options available = all bits set
	bool test(int value) const;      // if an option, or answer
	bool is_answer() const noexcept; // is set to answer
	bool is_answer(int value) const noexcept; // is set to answer value
	bool is_option(int value) const noexcept; // is an option
	bool is_empty() const noexcept;

	int get_answer() const
		noexcept; // return answer or 0 (won't confirm is_answer())
	std::vector<int> available() const; // return available options

	bool operator[](int value) const noexcept;
	auto operator[](int value) noexcept;

	bool operator==(int) const noexcept; // shorthand for is_answer(int)
	bool operator==(const Options<E>&) const noexcept;
	bool operator!=(const Options<E>&) const noexcept;
	bool operator<(const Options<E>&) const noexcept;

	// combine available options
	Options& operator+=(const Options&) noexcept;
	Options operator+(const Options&) const noexcept;
	// xor
	Options& XOR(const Options&) noexcept;
	// return difference
	Options operator-(const Options&) const
		noexcept; // TODO difference; usecase?
	// return shared options
	Options operator&(const Options&)const noexcept;

	// Debug Use Only, don't depend on it's result
	std::string DebugString() const;

private:
	// 0th bit is "need to solve":
	// false if answer has been set = inverse of answer
	bitset data_{};

	int read_next(int start = 0) const noexcept;
	Options& operator&=(const Options&) noexcept;
	// NOTE might be risky while unused; private?

}; // class Options


//	construct with all options
template<int E>
Options<E>::Options() noexcept
{
	data_.set(); // all true
}

template<int E>
inline Options<E>::Options(const bitset& other) : data_(other)
{ // empty constructor
}

template<int E>
inline Options<E>::Options(bitset&& other) : data_{other}
{ // empty constructor
}

//	construct with single option set to answer
template<int E>
Options<E>::Options(int value)
{
	// bitset: exception thrown for: value > E+1
	assert(value <= E); // caches E+1
	set(value);
}

//	set to answer value
template<int E>
inline Options<E>& Options<E>::operator=(int value)
{
	return set(value);
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
inline Options<E>& Options<E>::remove_option(const int value) noexcept
{
	assert(value >= 0 && value <= E);
	assert(!is_answer(value));

	operator[](value) = false;
	return *this;
}

//	add single option
template<int E>
inline Options<E>& Options<E>::add(int value)
{
	data_.set(static_cast<size_t>(value), true);
	return *this;
}

//	add single option
template<int E>
inline Options<E>& Options<E>::add_nocheck(int value) noexcept
{
	assert(value >= 0 && value <= E);
	operator[](value) = true;
	return *this;
}

//	set to answer
template<int E>
inline Options<E>& Options<E>::set(int value)
{
	clear();
	return add(value); // if 0: -> not answer = [0] = true
}

//	set to answer
template<int E>
inline Options<E>& Options<E>::set_nocheck(int value) noexcept
{
	clear();
	return add_nocheck(value);
}

template<int E>
inline constexpr int Options<E>::size() const noexcept
{
	return static_cast<int>(data_.size()); // bits
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
inline bool Options<E>::test(int value) const
{
	return data_.test(static_cast<size_t>(value));
}

//	check if set to answer
template<int E>
inline bool Options<E>::is_answer() const noexcept
{
	return data_.count() == 1 && !data_[0];
}

//	check if set to answer value
//! no input checks!
template<int E>
inline bool Options<E>::is_answer(int value) const noexcept
{
	return (is_answer() && operator[](value));
}

//	check if option available
//! no input checks!
template<int E>
inline bool Options<E>::is_option(int value) const noexcept
{
	assert(value != 0);
	return (!is_answer() && operator[](value));
}

//_Test if no options or answers available
template<int E>
inline bool Options<E>::is_empty() const noexcept
{
	return (data_.none() || (data_.count() == 1 && data_[0] == true));
}

//	determine the answer value, even if not marked
//	use with is_answer() to determine if flaged as anwer
template<int E>
inline int Options<E>::get_answer() const noexcept
{
	// TODO	microbench simpler/faster way to read single value from data_
	//		constexpr?
	//		bit operations?
	if (count_all() == 1)
	{
		return read_next();
	}
	return 0;
}

//	all available options
template<int E>
inline std::vector<int> Options<E>::available() const
{
	std::vector<int> values{};
	values.reserve(static_cast<size_t>(count()));
	if (!is_answer() && !is_empty())
	{
		auto item{0};
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
inline bool Options<E>::operator[](int value) const noexcept
{
	assert(value >= 0 && value <= E);
	return data_[static_cast<size_t>(value)];
}

//	no-check access
template<int E>
inline auto Options<E>::operator[](int value) noexcept
{
	assert(value >= 0 && value <= E);
	return data_[static_cast<size_t>(value)];
}

template<int E>
inline bool Options<E>::operator==(const Options& other) const noexcept
{
	//?? operator== what about the 0th 'is answer' bit?
	return data_ == other.data_;
}

template<int E>
inline bool Options<E>::operator!=(const Options& other) const noexcept
{
	return !(*this == other);
}

template<int E>
inline bool Options<E>::operator==(int value) const noexcept
{
	return is_answer(value);
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
inline Options<E> Options<E>::operator+(const Options& other) const noexcept
{
	Options<E> tmp(*this);
	return tmp += other;
}

//	Per element (binary) XOR, exclusive OR
template<int E>
inline Options<E>& Options<E>::XOR(const Options& other) noexcept
{
	data_ ^= other.data_;
	return *this;
}

//	Retain only shared options (binary AND)
template<int E>
inline Options<E>& Options<E>::operator&=(const Options& other) noexcept
{
	data_ &= other.data_;
	return *this;
}
//	Shared options (binary AND)
template<int E>
inline Options<E> Options<E>::operator&(const Options& other) const noexcept
{
	Options<E> tmp(*this);
	return tmp &= other;
}

template<int E>
inline std::string Options<E>::DebugString() const
{
	return data_.to_string();
}

//	return next option in data
template<int E>
inline int Options<E>::read_next(int start) const noexcept
{ // default value start = 0
	++start;
	for (int i = start; i <= E; ++i)
	{
		if (data_[static_cast<size_t>(i)])
		{
			return i;
		}
	}
	return 0; // never triggered
}

//////////////////////////////
///// external functions /////
//////////////////////////////

// Exclusive OR
template<int E>
inline Options<E> XOR(const Options<E>& A, const Options<E>& B) noexcept
{
	Options<E> tmp{A};
	return tmp.XOR(B);
}

// Shared options
template<int E>
inline Options<E> shared(const Options<E>& A, const Options<E>& B) noexcept
{
	return A & B;
}


} // namespace Sudoku
