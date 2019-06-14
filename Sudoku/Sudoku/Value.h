//====---- Sudoku/Value.h                                             ----====//
//
// Strongly typed Value type.
// Unsigned for use as a bit-mask.
//====--------------------------------------------------------------------====//
#pragma once

#include "Size.h"

#include <gsl/gsl>
#include <vector>
#include <algorithm> // all_of
#include <limits>
#include <stdexcept>
#include <type_traits> // is_same
#include <cstddef>     // size_t


namespace Sudoku
{
class Value
{
public:
	Value() noexcept = default;
	explicit constexpr Value(size_t val) noexcept : value_(val) {}

	explicit constexpr operator size_t() const noexcept { return value_; }
	explicit constexpr operator bool() const noexcept { return value_ != 0; }
	// note: static_assert performs an explicit conversion to bool

	friend constexpr bool operator==(const Value& a, const Value& b) noexcept
	{ // Friend definitions: https://abseil.io/tips/99
		return a.value_ == b.value_;
	}
	friend constexpr bool operator<(const Value& a, const Value& b) noexcept
	{
		return a.value_ < b.value_;
	}

	constexpr Value& operator++() noexcept;
	constexpr Value operator++(int) noexcept;

private:
	size_t value_{0};
};

//====--------------------------------------------------------------------====//
// Free-function declarations
constexpr bool operator!=(const Value&, const Value&) noexcept;
constexpr bool operator>(const Value&, const Value&) noexcept;
constexpr bool operator<=(const Value&, const Value&) noexcept;
constexpr bool operator>=(const Value&, const Value&) noexcept;

template<typename T, int N>
constexpr Value to_Value(T val);

template<int N>
constexpr bool is_valid(const Value&) noexcept;
template<int E>
inline constexpr bool is_valid_option(const Value&) noexcept;
template<int N>
bool is_valid(const std::vector<Value>&) noexcept;

//====--------------------------------------------------------------------====//

// Test input value
template<int N>
inline constexpr bool is_valid(const Value& value) noexcept
{
	return is_valid_option<elem_size<N>>(value);
}

// Test input value, for use with Options<E>
template<int E>
inline constexpr bool is_valid_option(const Value& value) noexcept
{
	return value > Value{0} && value <= Value{E};
}

// Test input values
template<int N>
bool is_valid(const std::vector<Value>& values) noexcept
{
	return (
		!values.empty() &&
		std::all_of(values.cbegin(), values.cend(), [](Value i) {
			return is_valid<N>(i);
		}));
}

//====--------------------------------------------------------------------====//
// Checked conversion to Value
template<int N, typename T>
inline constexpr Value to_Value(T val)
{
	if constexpr (std::is_same_v<Value, T>)
	{
		if (val > static_cast<Value>(elem_size<N>))
		{
			throw std::domain_error("Value input too large");
		}
		return val;
	}
	else if constexpr (std::is_unsigned_v<T>)
	{
		static_assert(elem_size<N> < std::numeric_limits<T>::max());
		return to_Value<N>(static_cast<Value>(val));
	}
	else if constexpr (std::is_integral_v<T>)
	{
		static_assert(std::numeric_limits<T>::max() > elem_size<N>);
		if (val < 0)
		{
			throw std::domain_error("Value can not be negative");
		}
		return to_Value<N>(gsl::narrow_cast<size_t>(val));
	}
}

//====--------------------------------------------------------------------====//
constexpr Value& Value::operator++() noexcept
{ // pre-increment
	++value_;
	return *this;
}
constexpr Value Value::operator++(int) noexcept
{ // post-increment
	const Value pre{*this};
	operator++();
	return pre;
}

inline constexpr bool operator!=(const Value& left, const Value& right) noexcept
{
	return !(left == right);
}
inline constexpr bool operator<=(const Value& left, const Value& right) noexcept
{
	return !(right < left);
}
inline constexpr bool operator>(const Value& left, const Value& right) noexcept
{
	return right < left;
}
inline constexpr bool operator>=(const Value& left, const Value& right) noexcept
{
	return !(left < right);
}
static_assert(Value{7} == Value{7});
static_assert(Value{1} != Value{0});
static_assert(Value{8} > Value{2});
static_assert(Value{10} >= Value{4});
static_assert(Value{3} <= Value{9});
static_assert(Value{5} < Value{6});

} // namespace Sudoku
