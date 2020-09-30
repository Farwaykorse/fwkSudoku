//====---- Sudoku/Options.h                                           ----====//
//
// Data object containing and managing available options.
//====--------------------------------------------------------------------====//
// Templated with element size.
//
// The 0-bit is the inverse answer-bit. if [0]==0 the answer is set.
//
//====--------------------------------------------------------------------====//
#pragma once

#include "OptionValue.h"
#include "Size.h"
#include "Value.h"

#include <gsl/gsl>

#include <bitset>
#include <string>
#include <vector>

#include <utility>

#include <cassert>
#include <cstddef> // size_t
#include <cstdint> // int64_t, uint32


namespace Sudoku
{
template<int E>
class Options
{
	static_assert(E >= 1);
	using bitset = std::bitset<gsl::narrow_cast<size_t>(E + 1)>;

public:
	Options() noexcept;
	Options(int) = delete; // NOLINT: implicit catch all that convert to bitset
	explicit Options(const bitset&) noexcept; // 0th bit is last in input
	explicit Options(bitset&&) noexcept;
	explicit constexpr Options(OptionValue<E>) noexcept;
	Options& operator=(OptionValue<E>) noexcept;
	Options& operator=(const bitset&) noexcept;
	Options& operator=(bitset&&) noexcept;

	Options& clear() noexcept; // remove all options / invalid state
	Options& reset() noexcept; // set all options
	Options& flip() noexcept;  // can create an invalid state
	Options& remove_option(OptionValue<E>); // remove single option
	Options& add(OptionValue<E>) noexcept;         // add single option
	Options& set(OptionValue<E>);                  // set to answer

	[[nodiscard]] constexpr size_t size() const noexcept;
	[[nodiscard]] size_t count() const noexcept;     // count available options
	[[nodiscard]] size_t count_all() const noexcept; // count all (incl. answer)
	[[nodiscard]] bool all() const noexcept; // test all options available
	[[nodiscard]] constexpr bool test(OptionValue<E>) const noexcept;
	[[nodiscard]] bool is_answer() const noexcept; // is set to answer
	[[nodiscard]] bool is_empty() const noexcept;

	[[nodiscard]] constexpr bool operator[](OptionValue<E>) const noexcept;
	auto operator[](OptionValue<E>) noexcept;

	[[nodiscard]] friend bool
		operator==(const Options<E>& left, const Options<E>& right) noexcept
	{
		//? operator== what about the 0th 'is answer' bit?
		return left.data_ == right.data_;
	}
	[[nodiscard]] friend bool
		operator<(const Options<E>& left, const Options<E>& right)
#if defined(__ICL) && __ICL <= 1900
			noexcept(false)
#else
			noexcept(sizeof(Options<E>) <= sizeof(std::uint64_t))
#endif // __ICL
	{
		if constexpr (sizeof(Options<E>) <= sizeof(std::uint32_t))
		{
			return left.data_.to_ulong() < right.data_.to_ulong();
		}
		else
		{
			return left.data_.to_ullong() < right.data_.to_ullong();
		}
	}

	// combine available options
	Options& operator+=(const Options&) noexcept;
	// XOR
	Options& XOR(const Options&) noexcept;
	// remove options
	Options& operator-=(const Options&) noexcept;

	// Debug Use Only, don't depend on it's result
	[[nodiscard]] std::string DebugString() const;

	// Shared options (binary AND)
	// Prefer: shared(left, right)
	[[nodiscard]] friend Options
		operator&(const Options& left, const Options& right) noexcept
	{
		Options tmp{left};
		tmp.data_ &= right.data_;
		return tmp;
	}

private:
	// 0th bit is "need to solve":
	// false if answer has been set = inverse of answer
	bitset data_{};
}; // class Options

//====---- free-functions ------------------------------------------------====//

template<int E>
[[nodiscard]] bool is_answer(Options<E> const&) noexcept;
template<int E>
[[nodiscard]] bool is_answer(Options<E> const&, OptionValue<E>) noexcept;
template<int E>
constexpr bool is_answer(Options<E> const&, Value);
template<int E>
[[nodiscard]] bool is_option(Options<E> const&, OptionValue<E>) noexcept;
template<int E>
constexpr bool is_option(Options<E> const&, Value);
template<int E>
[[nodiscard]] Value get_answer(Options<E> const&) noexcept;
template<int N>
[[nodiscard]] Value to_Value(Options<elem_size<N>> const&) noexcept;
template<int E> // NOLINTNEXTLINE(bugprone-exception-escape)
[[nodiscard]] std::vector<Value> available(const Options<E>&) noexcept(true);
template<int E>
[[nodiscard]] Value read_next(const Options<E>&, Value = Value{0}) noexcept;

template<int E>
[[nodiscard]] bool operator!=(Options<E> const&, Options<E> const&) noexcept;

template<int E>
[[nodiscard]] bool operator==(Options<E> const&, Value const&) noexcept;
template<int E>
[[nodiscard]] bool operator==(Value const&, Options<E> const&) noexcept;
template<int E>
[[nodiscard]] bool operator!=(Options<E> const&, Value const&) noexcept;
template<int E>
[[nodiscard]] bool operator!=(Value const&, const Options<E>&) noexcept;

template<int E>
[[nodiscard]] Options<E> XOR(Options<E> const& A, Options<E> const& B) noexcept;

template<int E>
[[nodiscard]] Options<E>
	operator+(Options<E> const&, Options<E> const&) noexcept;
template<int E>
[[nodiscard]] Options<E>
	operator-(Options<E> const&, Options<E> const&) noexcept;

// return shared options
template<int E>
[[nodiscard]] Options<E>
	shared(Options<E> const& A, Options<E> const& B) noexcept;

//====--------------------------------------------------------------------====//

namespace impl
{
	[[nodiscard]] inline constexpr size_t exp2_(size_t value) noexcept
	{
		return (value < 1U) ? 1U : (2U * exp2_(--value));
	}
	static_assert(exp2_(0U) == 0x1U);
	static_assert(exp2_(1U) == 0x2U);
	static_assert(exp2_(2U) == 0x4U); //-V112
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	static_assert(exp2_(3U) == 0x8U); // NOLINT(readability-magic-numbers)
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	static_assert(exp2_(4U) == 0x10U); // NOLINT(readability-magic-numbers)
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	static_assert(exp2_(5U) == 0x20U); // NOLINT(readability-magic-numbers)
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	static_assert(exp2_(6U) == 0x40U); // NOLINT(readability-magic-numbers)
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	static_assert(exp2_(7U) == 0x80U); // NOLINT(readability-magic-numbers)
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	static_assert(exp2_(8U) == 0x100U); // NOLINT(readability-magic-numbers)
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
	static_assert(exp2_(9U) == 0x200U); // NOLINT(readability-magic-numbers)

	// generate a bit-mask to use a unique bit per value
	//   empty for Value outside domain.
	template<int E>
	[[nodiscard]] inline constexpr size_t exp2_(Value value) noexcept
	{
		return (value > Value{E}) ? 0U : exp2_(static_cast<size_t>(value));
	}
	// generate a bit-mask to set all bits in std::bitset
	[[nodiscard]] inline constexpr size_t all_set(size_t elements) noexcept
	{
		return (exp2_(++elements) - 1U);
	}
} // namespace impl

//====--------------------------------------------------------------------====//

//	construct with all options set
template<int E>
inline Options<E>::Options() noexcept
{
	data_.flip();
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
inline constexpr Options<E>::Options(OptionValue<E> value) noexcept
	: data_{impl::exp2_<E>(value)}
{
}

//	set to answer value
template<int E>
inline Options<E>& Options<E>::operator=(OptionValue<E> value) noexcept
{
	data_ = impl::exp2_<E>(value);
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
	//???	Note: answer-bit unset too -> answered?
	return *this;
}

//	set all options
template<int E>
inline Options<E>& Options<E>::reset() noexcept
{
	data_.set(); // all true, default starting state
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
inline Options<E>& Options<E>::remove_option(OptionValue<E> const value)
{
	assert(not Sudoku::is_answer(*this, value));

	data_.set(value, false);
	return *this;
}

//	add single option
template<int E>
inline Options<E>& Options<E>::add(OptionValue<E> const value) noexcept
{
	data_[value] = true;
	return *this;
}

//	set to answer
template<int E>
inline Options<E>& Options<E>::set(OptionValue<E> const value)
{
	data_.reset(); // all 0 incl. answer-bit
	return add(value);
}

template<int E>
inline constexpr size_t Options<E>::size() const noexcept
{
	return data_.size(); // bits

	//??? size() -1 better?
	// The current implementation works with size() being 1 past the last
	// element. But this allows for size()-1 options to be stored. The
	// direct value to location implementation is convenient. The 0th
	// element in this implementation is just a flag (more could be added).
}

//	available options
//	if 1, a not processed answer
template<int E>
inline size_t Options<E>::count() const noexcept
{
	if (data_[0])
	{
		return data_.count() - 1U;
	}
	return 0; // NO protection vs incorrect answer bit
}

//	(!) counts options, including set answers == ignores answer-bit
//	Returns 1 if {1 option, set as answer} || {1 option, not set as answer}
//	Returns 0 if {empty} || {no option, but not set as answer}
template<int E>
inline size_t Options<E>::count_all() const noexcept
{
	if (data_[0])
	{
		return data_.count() - 1U;
	}
	return data_.count();
}

// Test if all bits are set
template<int E>
inline bool Options<E>::all() const noexcept
{
	return data_.all();
}

//	if an option, or the answer
template<int E>
constexpr bool Options<E>::test(OptionValue<E> const value) const noexcept
{
	return data_[value];
}

//	check if set to answer
template<int E>
inline bool Options<E>::is_answer() const noexcept
{
	return !data_[0] && data_.count() == 1;
}

// check if set to answer
template<int E>
inline bool is_answer(const Options<E>& options) noexcept
{ // convenience function
	return options.is_answer();
}

// check if set to answer value
template<int E>
inline bool
	is_answer(Options<E> const& options, OptionValue<E> const value) noexcept
{
	return options == Options<E>{value};
}

// check if set to answer value
template<int E>
constexpr bool is_answer(Options<E> const& options, Value const value)
{ // For proper template type deduction for OptionValue
	return is_answer(options, OptionValue<E>{value});
}

// check if option available
template<int E>
inline bool
	is_option(const Options<E>& options, OptionValue<E> const value) noexcept
{
	return (options.test(value) && not is_answer(options));
}

// check if option available
template<int E>
constexpr bool is_option(Options<E> const& options, Value const value)
{ // For proper template type deduction for OptionValue
	return is_option(options, OptionValue<E>{value});
}

// Test if no options or answers available
template<int E>
inline bool Options<E>::is_empty() const noexcept
{
	return data_.none();
}

// determine the answer value, even if not marked
//   use with is_answer() to determine if flagged as answer
template<int E>
inline Value get_answer(const Options<E>& options) noexcept
{
	if (options.count_all() == 1)
	{
		return read_next(options);
	}
	return Value{0};
}

template<int N>
inline Value to_Value(Options<elem_size<N>> const& options) noexcept
{ // specialization of to_Value
	return get_answer(options);
}

// all available options
template<int E> // NOLINTNEXTLINE(bugprone-exception-escape)
inline std::vector<Value> available(Options<E> const& options) noexcept(true)
{ // noexcept: only allocation can throw. Terminate, all is lost anyway.
	const size_t count = options.count();
	std::vector<Value> values{};
	values.reserve(count);

	if (not is_answer(options) && not options.is_empty())
	{
		Value item{0};
		for (size_t i{0}; i < count; ++i)
		{
			item = read_next(options, item);
			values.emplace_back(item);
		}
	}
	return values;
}

//	no-check access read only
template<int E>
constexpr bool Options<E>::operator[](OptionValue<E> const value) const noexcept
{
	return data_[value];
}

//	no-check access
template<int E>
inline auto Options<E>::operator[](OptionValue<E> const value) noexcept
{
	return data_[value];
}

template<int E>
inline bool operator!=(Options<E> const& left, Options<E> const& right) noexcept
{
	return !(left == right);
}

// short for is_answer(value)
template<int E>
inline bool operator==(Options<E> const& left, Value const& value) noexcept
{
	return is_answer(left, value);
}
template<int E>
inline bool operator==(Value const& value, Options<E> const& right) noexcept
{
	return is_answer(right, value);
}
template<int E>
inline bool operator!=(Options<E> const& left, Value const& value) noexcept
{
	return not(is_answer(left, value));
}
template<int E>
inline bool operator!=(Value const& value, Options<E> const& right) noexcept
{
	return not(is_answer(right, value));
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

// Remove options existing also in other
template<int E>
inline Options<E>& Options<E>::operator-=(const Options& other) noexcept
{
	assert(other.is_answer()); // do not remove the answer-bit
	const Options tmp = ::Sudoku::XOR(*this, other);
	data_ &= tmp.data_;
	return *this;
}

// Only in left
template<int E>
inline Options<E>
	operator-(const Options<E>& left, const Options<E>& right) noexcept
{
	Options<E> tmp{left};
	return tmp -= right;
}

template<int E>
#ifndef fwkUnitTest
[[deprecated("Debug use only")]]
#endif
inline std::string
	Options<E>::DebugString() const
{
	return data_.to_string();
}

// return next option in data
template<int E>
inline Value read_next(const Options<E>& options, Value value) noexcept
{ // default value start = 0
	++value;
	for (; value <= Value{E}; ++value)
	{
		if (options[value])
		{
			return value;
		}
	}
	return Value{0};
}

} // namespace Sudoku
