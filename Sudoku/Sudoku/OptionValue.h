//====---- Sudoku/OptionValue.h                                       ----====//
// An internally used immutable and bounded version of Sudoku::Value
//
//====--------------------------------------------------------------------====//
#pragma once

#include "Concepts.h"
#include "Value.h"

#include <cassert>

namespace Sudoku
{
template<int E>
class OptionValue
{
public:
	template<typename T>
	requires UnsignedNumber<T> explicit constexpr OptionValue(T val) noexcept
		: value_(val)
	{
		assert(val > T{0U} && val <= T{E});
	}
	// NOLINTNEXTLINE(google-explicit-constructor)
	constexpr OptionValue(Value val) noexcept // NOLINT(runtime/explicit)
		: value_(val)
	{
		assert(val > Value{0U} && val <= Value{E});
	}

	// Implicit conversions
	// NOLINTNEXTLINE(google-explicit-constructor)
	constexpr operator Value() const noexcept { return Value{value_}; }
	template<typename T>
	// NOLINTNEXTLINE(google-explicit-constructor)
	requires UnsignedNumber<T> constexpr operator T() const noexcept
	{
		return T{value_};
	}
	explicit constexpr operator int() const noexcept
	{
		return static_cast<int>(value_);
	}

	friend constexpr bool
		operator==(OptionValue<E> const& a, OptionValue<E> const& b) noexcept
	{
		return a.value_ == b.value_;
	}
	friend constexpr bool
		operator<(OptionValue<E> const& a, OptionValue<E> const& b) noexcept
	{
		return a.value_ < b.value_;
	}

private:
	const size_t value_;
};

} // namespace Sudoku
