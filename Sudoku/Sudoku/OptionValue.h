//====---- Sudoku/OptionValue.h                                       ----====//
// An internally used immutable and bounded version of Sudoku::Value
// Implements range checks in a combination of constexpr and exceptions.
//
//====--------------------------------------------------------------------====//
#pragma once

#include "Concepts.h"
#include "Size.h"
#include "Value.h"
#include "exceptions.h"

namespace Sudoku
{
template<int E>
class OptionValue
{
	OptionValue() noexcept = default;

public:
	template<typename T>
	requires UnsignedNumber<T> explicit constexpr OptionValue(T val)
		: value_(val)
	{
		if (val == T{0U} || val > T{E})
			throw Sudoku::error::invalid_option();
	}
	// NOLINTNEXTLINE(google-explicit-constructor)
	constexpr OptionValue(Value val) : value_(val) // NOLINT(runtime/explicit)
	{
		if (val == Value{0U} || val > Value{E})
			throw Sudoku::error::invalid_option();
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
