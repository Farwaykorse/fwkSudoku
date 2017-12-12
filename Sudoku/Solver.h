//===--	Sudoku/Solver.h													--===//
//
//	Solver functions acting on a Board<Options>&
// Declarations are in Solver.fwd.h
//===---------------------------------------------------------------------===//
#pragma once

#include "Board.h"
#include "Iterator_Utilities.h"
#include "Location.h"
#include "Location_Utilities.h"
#include "Options.h"
#include "Size.h"
#include "Solvers_find.h"
#include "Solvers_remove_option.h"
#include "Solvers_set_option.h"
#include "Value.h"

#include <vector>
#include <algorithm>   // minmax
#include <type_traits> // is_base_of
#include <cassert>

// Forward declarations
#include "Solver.fwd.h"


namespace Sudoku
{
//	Check if only one option remaining
//	IF true: process answer
template<int N, typename Options>
inline int single_option(Board<Options, N>& board, const Location<N> loc)
{
	assert(is_valid(loc));

	if (const Value answer{board[loc].get_answer()})
	{
		return single_option(board, loc, answer);
	}
	return 0;
}

//	Process answer from [loc], update board_ options
//	Remove option from rest of row, col and block
template<int N, typename Options>
inline int single_option(
	Board<Options, N>& board, const Location<N> loc, const Value value)
{
	assert(is_valid(loc));
	assert(is_valid<N>(value));

	assert(board.at(loc).test(value));
	assert(board[loc].count_all() == 1);

	int changes{};
	if (!board[loc].is_answer(value))
	{
		setValue(board, loc, value);
		++changes;
	}
	changes += remove_option_section(board, board.row(loc), loc, value);
	changes += remove_option_section(board, board.col(loc), loc, value);
	changes += remove_option_section(board, board.block(loc), loc, value);
	return changes;
}

// if 2 options in element:
// find exact pair in section:
// remove form other elements in section
template<int N, typename Options>
inline int dual_option(Board<Options, N>& board, const Location<N> loc)
{
	using Location = Location<N>;
	{
		assert(is_valid(loc));
		assert(board.at(loc).count() == 2);
	}
	auto sorted_loc = [loc](const Location L) {
		const auto result = std::minmax(loc, L);
		return std::vector<Location>{result.first, result.second};
	};

	int changes{};
	const Options& item{board[loc]};
	for (int i{}; i < full_size<N>; ++i)
	{
		// find exact same in board
		if (board[Location(i)] == item && Location(i) != loc)
		{
			// Remove values for rest of shared elements
			if (is_same_row(loc, Location(i)))
			{
				changes += remove_option_section(
					board,
					board.row(loc),
					sorted_loc(Location(i)),
					item.available());
			}
			else if (is_same_col(loc, Location(i)))
			{
				changes += remove_option_section(
					board,
					board.col(loc),
					sorted_loc(Location(i)),
					item.available());
			}
			// run don't if already answered in one of the previous
			if (is_same_block(loc, Location(i)) && !item.is_answer())
			{
				// NOTE this is slow
				changes += remove_option_section(
					board,
					board.block(loc),
					sorted_loc(Location(i)),
					item.available());
			}
		}
	}
	return changes;
}

//	finds equal sets in section:
//	removes form others in section
template<int N, typename Options>
inline int
	multi_option(Board<Options, N>& board, const Location<N> loc, size_t count)
{
	using Location = Location<N>;
	{
		assert(is_valid(loc));
		assert(count <= elem_size<N>);
	}
	if (count == 0)
	{
		count = static_cast<size_t>(board[loc].count());
	}
	constexpr auto specialize = 2; // use specialization below and including
	constexpr auto max_size   = elem_size<N> / 2; //?? Assumption, Proof needed
	if (specialize < count && count <= max_size)
	{
		int changes{};                   // performance counter
		const Options& item{board[loc]}; // input item, to find matches to
		std::vector<Location> list{};    // potential matches

		// traverse whole board
		for (int i{}; i < full_size<N>; ++i)
		{
			const auto& other = board[Location(i)];
			// find exact same in board
			// TODO rework to also pick-up cels containing [2,n) values
			if (other.count() > 0 && // include not processed answers
				other.count() <= base_size<N> &&
				(other == item || (item & other).count() == other.count()))
			{
				list.push_back(Location(i));
			}
		}
		//	This algorithm is supprizingly robust
		//	Example, showing operation if no specializations where applied
		//			start	end	/	start	end		/	start	end
		//	item =	1,2,3	3	/	1,2,3	1,2,3	/	1,2,3	1,2,3
		//	list1	1,2		1	/	1,2,3	1,2,3	/	1,2		1,2
		//	list2	2		2	/	1,2		1,2		/	1,2		1,2
		//	extern	1,2,3,4	4	/	1,2,3,4	4		/	1,2,3,4	4
		//
		//	item	1,2,3,4		1,2,3,4
		//	list1	1,2,3		1,2,3
		//	list2	1,2,3		1,2,3
		//	list3	1,2,3		1,2,3
		//	other	1,2,3,4,5	5
		//	Only the 2nd example could only be processed by this method
		//
		if (list.size() >= count)
		{
			// find if: count amount of items share an element
			// Remove values for rest of shared elements
			if (const auto in_row{get_same_row(loc, list)};
				in_row.size() == count)
			{
				changes += remove_option_section(
					board, board.row(loc), in_row, item.available());
			}
			if (const auto in_col{get_same_col(loc, list)};
				in_col.size() == count)
			{
				changes += remove_option_section(
					board, board.col(loc), in_col, item.available());
			}
			if (const auto in_block{get_same_block(loc, list)};
				in_block.size() == count)
			{
				// NOTE this is slow
				changes += remove_option_section(
					board, board.block(loc), in_block, item.available());
			}
		}
		return changes;
	}
	return 0;
}

//	Solver: Find and set options appearing only once in a section as answer
template<int N, typename Options, typename SectionT>
inline int unique_in_section(Board<Options, N>& board, const SectionT section)
{
	{
		using Board_Section = Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
	}
	const auto worker = appearance_once<N>(section);
	return set_uniques(board, section, worker);
}

//	Solver: find and process values appearing 1 to base_size times in section:
//	[row/col] IF all in same block -> remove from rest of block
//	[block] IF all in same row/col -> remove from rest of row/col
template<int N, typename Options, typename SectionT>
inline int section_exclusive(Board<Options, N>& board, const SectionT section)
{
	{
		using Board_Section = Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::iterator_to<iterator, const Options>);
	}
	int changes{}; // performance counter

	size_t i{2};
	auto appearing             = appearance_sets<N>(section);
	const auto renew_appearing = [&i, &a = appearing, &section]() {
		i = 2;
		a = appearance_sets<N>(section);
	};
	while (i < appearing.size()) // won't run if condition fails
	{
		// unique specialization
		if (appearing[1].count_all() > 0)
		{
			changes += set_uniques(board, section, appearing[1]);
			renew_appearing();
		}
		else if (appearing.at(i).count_all() > 0)
		{
			// for [row/col]: if in same block: remove from rest block
			// for [block]: if in same row/col: remove from rest row/col
			if (const int tmp_ = set_section_locals(
					board, section, static_cast<int>(i), appearing.at(i)))
			{
				changes += tmp_;
				renew_appearing();
			}
			else
			{
				++i;
			}
		}
		else
		{
			++i;
		}
	}
	return changes;
}


// cell containing 2 options
//	find in board_	size() == 2
//		find exact same in rest board
//			IF pair in same row/col -> remove values from rest row/col
//			IF same block -> remove values from rest block
//		repeat until end of board_
//	repeat until end of board_


// cell containing 2 options
//	find in board_	size() == 2
//		find exact same in rest row
//			IF found:	remove values from rest row
//		find exact same in rest col
//		find exact same in rest block

} // namespace Sudoku
