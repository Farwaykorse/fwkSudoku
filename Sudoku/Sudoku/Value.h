//====---- Sudoku/Value.h                                             ----====//
//
// Strongly typed Value type.
// Unsigned for use as a bit-mask.
//====--------------------------------------------------------------------====//
#pragma once

#include "Concepts.h"

#include <gsl/gsl>

#include <cassert>
#include <cstddef> // size_t


namespace Sudoku
{
class Value
{
public:
	Value() noexcept = default;
	template<UnsignedNumber T>
	explicit constexpr Value(T val) noexcept : value_(val)
	{
	}
	template<SignedNumber T>
	explicit constexpr Value(T val) noexcept
		: value_(gsl::narrow_cast<size_t>(val))
	{
		if (val < 0)
			assert(false);
	}

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

//====--------------------------------------------------------------------====//
constexpr Value& Value::operator++() noexcept
{ // pre-increment
	++value_;
	return *this;
}
// NOLINTNEXTLINE(readability/casting) false-positive cpplint
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
static_assert(Value{4} == Value{4}); //-V501 //-V112
static_assert(Value{1} != Value{0});
static_assert(Value{3} > Value{2});
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
static_assert(Value{10} >= Value{7}); // NOLINT(readability-magic-numbers)
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
static_assert(Value{8} <= Value{9}); // NOLINT(readability-magic-numbers)
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
static_assert(Value{5} < Value{6}); // NOLINT(readability-magic-numbers)

} // namespace Sudoku
