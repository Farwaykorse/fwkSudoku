//====---- Sudoku/Value.h                                             ----====//
//
// Strongly typed Value type.
// Unsigned for use as a bit-mask.
//====--------------------------------------------------------------------====//
#pragma once

#include "Concepts.h"

#include <gsl/gsl>

#include <compare>

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

	friend constexpr bool
		operator==(const Value left, const Value right) noexcept
	{ // Friend definitions: https://abseil.io/tips/99
		return left.value_ == right.value_;
	}
	friend constexpr std::strong_ordering
		operator<=>(const Value left, const Value right) noexcept
	{
		return left.value_ <=> right.value_;
	}

	constexpr Value& operator++() noexcept;
	constexpr Value operator++(int) noexcept;

private:
	size_t value_{0};
};

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

} // namespace Sudoku
