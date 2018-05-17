//===--- Sudoku/Solver_exception.h                                      ---===//
//
//===----------------------------------------------------------------------===//
#pragma once

#include <stdexcept>
#include <string>


namespace Sudoku::error
{
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

	virtual const char* where() const noexcept
	{
		return where_;
	}

private:
	const char* where_{std::nullptr_t{}};
};

} // namespace Sudoku::error
