//===--- Sudoku/Solver_exception.h                                      ---===//
//
//===----------------------------------------------------------------------===//
#pragma once

#include <string>

#include <stdexcept>


namespace Sudoku::error
{
// Invalid input, unsupported type
struct unsupported_type : public std::domain_error
{
	unsupported_type() noexcept : domain_error{"unsupported_type"} {}
};
// Invalid option, outside expected range
struct invalid_option : public std::domain_error
{
	invalid_option() noexcept : domain_error{"invalid_option"} {}
};

// Conflicting answers, board is not solvable
struct invalid_Board : public std::logic_error
{
	invalid_Board() noexcept : logic_error{"invalid_Board"} {}
};

// Location or equivalent not valid for operation
struct invalid_Location : public std::out_of_range
{
	invalid_Location() noexcept : out_of_range{"invalid_Location"} {}

	explicit invalid_Location(const std::string& message)
		: out_of_range{"invalid_Location"}, where_{message.c_str()}
	{
	}
	explicit invalid_Location(const char* message)
		: out_of_range{"invalid_Location"}, where_{message}
	{
	}

	[[nodiscard]] virtual const char* where() const noexcept { return where_; }

private:
	const char* where_{std::nullptr_t{}};
};

} // namespace Sudoku::error
