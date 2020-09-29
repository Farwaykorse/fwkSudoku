// An internal bounded version of Sudoku::Value
//
#pragma once

#include "Size.h"
#include "Value.h"
#include "exceptions.h"

namespace Sudoku
{
template<typename T>
concept Character = std::is_integral_v<T> &&
					(std::is_same_v<T, char> || std::is_same_v<T, wchar_t> ||
					 std::is_same_v<T, char8_t> ||
					 std::is_same_v<T, char16_t> ||
					 std::is_same_v<T, char32_t> ||
					 std::is_same_v<T, unsigned char> ||
					 std::is_same_v<T, signed char>);
template<typename T>
concept Number =
	std::is_integral_v<T> && not Character<T> && not std::is_same_v<T, bool>;
template<typename T>
concept SignedNumber = Number<T>&& std::is_signed_v<T>;
template<typename T>
concept UnsignedNumber = Number<T>&& std::is_unsigned_v<T>;


template<int E>
class OptionValue
{
	OptionValue() noexcept = default;

public:
	template<typename T>
	requires UnsignedNumber<T> explicit constexpr OptionValue(T val)
		: value_(val)
	{
		if (val == T{0u} || val > T{E})
			throw Sudoku::error::invalid_option();
	}
	// NOLINTNEXTLINE(google-explicit-constructor)
	constexpr OptionValue(Value val) : value_(val) // NOLINT(runtime/explicit)
	{
		if (val == Value{0u} || val > Value{E})
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
	size_t value_;
};


} // namespace Sudoku
