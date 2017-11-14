//===--	Sudoku/Solver.h													--===//
//
//	Solver function container-class
//	Collects and gives access to solver functions acting on a Board<Options>&
//===---------------------------------------------------------------------===//
#pragma once

#include "Iterator_Utilities.h"
#include "Location_Utilities.h"
#include "Options.h"
#include "Solvers_Appearance.h"

#include <array>
#include <vector>
#include <algorithm> // for_each, minmax, all_of, none_of
#include <stdexcept>
#include <type_traits> // is_base_of
#include <cassert>

// Forward declarations
#include "Board.fwd.h"
#include "Location.fwd.h"
#include "Solver.fwd.h"

/* experiment flags */
//! DO #undef at end of file!
// activate algorithms on removing option
#define DUAL_ON_REMOVE false
#define MULTIPLE_ON_REMOVE false

namespace Sudoku
{
template<int N>
class Solver
{
	using Location       = Location<N>;
	using Location_Block = Location_Block<N>;
	using value_t        = unsigned int;

	using Options = Options<elem_size<N>>;
	using Board   = Sudoku::Board<Options, N>;
	using Row     = typename Board::Row;
	using Col     = typename Board::Col;
	using Block   = typename Board::Block;
	template<typename ItrT>
	using if_forward = std::enable_if_t<Utility_::is_forward<ItrT>>;

public:
	Solver(Board&);

	// Edit Board
	void setValue(Location, value_t);
	template<typename ItrT, typename = if_forward<ItrT>>
	void setValue(ItrT begin, ItrT end);


	// Solvers
	int single_option(Location);
	int single_option(Location, value_t);
	int dual_option(Location);
	int multi_option(Location, size_t = 0);

	template<typename SectionT>
	int unique_in_section(SectionT);
	template<typename SectionT>
	int section_exclusive(SectionT);

private:
	Board& board_;
};

//===-- free functions ---------------------------------------------------===//
template<int N, typename SectionT>
auto find_locations(SectionT, unsigned int value, int rep_count = elem_size<N>);
template<int N, typename ItrT>
auto find_locations(
	ItrT begin, ItrT end, unsigned int value, int rep_count = elem_size<N>);
template<int N, typename SectionT>
auto find_locations(SectionT, Options<elem_size<N>> value);

// remove an option: triggers solvers single_option()
template<int N, typename Options = Options<elem_size<N>>>
int remove_option(Board<Options, N>&, Location<N>, unsigned int value);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_section(
	Board<Options, N>&, SectionT, Location<N> ignore, unsigned int value);
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_section(
	Board<Options, N>&,
	SectionT,
	const std::vector<Location<N>>& ignore,
	unsigned int value);
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_section(
	Board<Options, N>&,
	SectionT,
	const std::vector<Location<N>>& ignore,
	const std::vector<unsigned int>& values);
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int remove_option_outside_block(
	Board<Options, N>&, SectionT, Location<N> block_loc, unsigned int value);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int set_section_locals(
	Board<Options, N>&, SectionT, int rep_count, Options worker);
template<int N, typename Options = Options<elem_size<N>>>
int set_section_locals(
	Board<Options, N>&,
	Board_Section::Block<Options, N>,
	int rep_count,
	Options worker);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
auto set_uniques(Board<Options, N>&, SectionT, Options worker);

//===---------------------------------------------------------------------===//


template<int N>
inline Solver<N>::Solver(Board& options) : board_(options)
{
	// empty constructor
}

//	IF valid, Make [value] the answer for [loc]
//	No processing
template<int N>
inline void Solver<N>::setValue(const Location loc, const value_t value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));

	if (!board_.at(loc).test(value))
	{ // value is option nor answer
		throw std::logic_error{"Invalid Board"};
	}
	board_[loc].set_nocheck(value);
}

//	set board_ using a transferable container of values
template<int N>
template<typename ItrT, typename> // at least forward_iterator
inline void Solver<N>::setValue(const ItrT begin, const ItrT end)
{
	{
		static_assert(Utility_::is_forward<ItrT>);
		static_assert(Utility_::iterator_to<ItrT, int>);
		assert(end - begin == full_size<N>);
	}
	int n{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		Location loc(n++); // start at 0!
		const auto value = static_cast<value_t>(*itr);
		if (value > 0 && board_.at(loc).is_option(value))
		{
			setValue(loc, value);
			single_option(loc, value);
		}
		// check invalid value or conflict
		assert(value == 0 || board_.at(loc).is_answer(value));
	}
	assert(n == full_size<N>);
}

//	remove option from element, make answer if last option
template<int N, typename Options>
int remove_option(
	Board<Options, N>& board, const Location<N> loc, const unsigned int value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));

	int changes{};
	auto& item{board.at(loc)};

	if (item.is_option(value))
	{
		++changes;
		const int count = item.remove_option(value).count();
		assert(count > 0); // never trigger, removed last option

		if (count == 1)
		{
			changes += Solver<N>(board).single_option(loc);
		}
#if DUAL_ON_REMOVE == true
		if (count == 2)
		{
			changes += Solver<N>(board).dual_option(loc);
		}
#endif // dual
#if MULTIPLE_ON_REMOVE == true
		changes += Solver<N>(board).multi_option(loc, count);
#endif // multiple
	}
	return changes;
}

//	Check if only one option remaining
//	IF true: process answer
template<int N>
inline int Solver<N>::single_option(const Location loc)
{
	assert(is_valid(loc));

	if (const value_t answer{board_[loc].get_answer()})
	{
		return single_option(loc, answer);
	}
	return 0;
}

//	Process answer from [loc], update board_ options
//	Remove option from rest of row, col and block
template<int N>
inline int Solver<N>::single_option(const Location loc, const value_t value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));

	assert(board_.at(loc).test(value));
	assert(board_[loc].count_all() == 1);

	int changes{};
	if (!board_[loc].is_answer(value))
	{
		setValue(loc, value);
		++changes;
	}
	changes += remove_option_section(board_, board_.row(loc), loc, value);
	changes += remove_option_section(board_, board_.col(loc), loc, value);
	changes += remove_option_section(board_, board_.block(loc), loc, value);
	return changes;
}

// if 2 options in element:
// find exact pair in section:
// remove form other elements in section
template<int N>
inline int Solver<N>::dual_option(const Location loc)
{
	assert(is_valid(loc));
	assert(board_.at(loc).count() == 2);

	auto sorted_loc = [loc](const Location L) {
		const auto result = std::minmax(loc, L);
		return std::vector<Location>{result.first, result.second};
	};

	int changes{};
	const Options& item{board_[loc]};
	for (int i{}; i < full_size<N>; ++i)
	{
		// find exact same in board
		if (board_[Location(i)] == item && Location(i) != loc)
		{
			// Remove values for rest of shared elements
			if (is_same_row(loc, Location(i)))
			{
				changes += remove_option_section(
					board_,
					board_.row(loc),
					sorted_loc(Location(i)),
					item.available());
			}
			else if (is_same_col(loc, Location(i)))
			{
				changes += remove_option_section(
					board_,
					board_.col(loc),
					sorted_loc(Location(i)),
					item.available());
			}
			// run don't if already answered in one of the previous
			if (is_same_block(loc, Location(i)) && !item.is_answer())
			{
				// NOTE this is slow
				changes += remove_option_section(
					board_,
					board_.block(loc),
					sorted_loc(Location(i)),
					item.available());
			}
		}
	}
	return changes;
}

//	finds equal sets in section:
//	removes form others in section
template<int N>
inline int Solver<N>::multi_option(const Location loc, size_t count)
{
	assert(is_valid(loc));
	assert(count <= elem_size<N>);

	if (count == 0)
	{
		count = static_cast<size_t>(board_[loc].count());
	}
	constexpr auto specialize = 2; // use specialization below and including
	constexpr auto max_size   = elem_size<N> / 2; //?? Assumption, Proof needed
	if (specialize < count && count <= max_size)
	{
		int changes{};                    // performance counter
		const Options& item{board_[loc]}; // input item, to find matches to
		std::vector<Location> list{};     // potential matches

		// traverse whole board
		for (int i{}; i < full_size<N>; ++i)
		{
			const auto& other = board_[Location(i)];
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
					board_, board_.row(loc), in_row, item.available());
			}
			if (const auto in_col{get_same_col(loc, list)};
				in_col.size() == count)
			{
				changes += remove_option_section(
					board_, board_.col(loc), in_col, item.available());
			}
			if (const auto in_block{get_same_block(loc, list)};
				in_block.size() == count)
			{
				// NOTE this is slow
				changes += remove_option_section(
					board_, board_.block(loc), in_block, item.available());
			}
		}
		return changes;
	}
	return 0;
}

//	remove [value] in [ignore] from other elements in set
template<int N, typename Options, typename SectionT>
int remove_option_section(
	Board<Options, N>& board,
	const SectionT section,
	const Location<N> ignore,
	const unsigned int value)
{
	{
		using Board = Board<Options, N>;
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid_value<N>(value));
		assert(is_same_section(section, ignore));
		assert(board.at(ignore).is_answer(value)); // first set as anwer!
	}
	int changes{0};
	const auto end = section.cend();

	for (auto itr = section.cbegin(); itr != end; ++itr)
	{
		if (itr.location() != ignore)
		{
			changes += remove_option(board, itr.location(), value);
		}
	}
	return changes;
}

//	remove [value] from elements not in same block as [block_loc]
template<int N, typename Options, typename SectionT>
int remove_option_outside_block(
	Board<Options, N>& board,
	const SectionT section,
	const Location<N> block_loc,
	const unsigned int value)
{
	{
		using Board = Board<Options, N>;
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		static_assert(
			not std::is_base_of_v<typename Board::const_Block, SectionT>,
			"remove_option_outside_block is useless on bock");
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(block_loc));
		assert(is_valid_value<N>(value));
		assert(intersect_block(section, block_loc));
	}
	int changes{0};
	const auto end = section.cend();

	for (auto itr = section.cbegin(); itr != end; ++itr)
	{
		if (not is_same_block(itr.location(), block_loc))
		{
			changes += remove_option(board, itr.location(), value);
		}
	}
	return changes;
}

//	remove [value] from set if not part of [loc]s
template<int N, typename Options, typename SectionT>
inline int remove_option_section(
	Board<Options, N>& board,
	const SectionT section,
	const std::vector<Location<N>>& ignore,
	const unsigned int value)
{
	{
		static_assert(
			std::is_base_of_v<typename Board<Options, N>::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid_value<N>(value));
		assert(is_same_section(section, ignore));
	}
	int changes{0};
	const auto begin = ignore.cbegin();
	const auto end   = ignore.cend();

	// TODO maybe faster to run get_same_row/col/block first?
	// and than not to check is_option(), since it is already in remove_option

	for (auto itr = section.cbegin(); itr != section.cend(); ++itr)
	{
		// TODO is the is_option check really faster?
		if (itr->is_option(value) &&
			std::none_of(begin, end, [L1 = itr.location()](Location<N> L2) {
				return L1 == L2;
			})) // <algorithm>
		{
			changes += remove_option(board, itr.location(), value);
		}
	}
	return changes;
}

// remove [value]s from [section] if not part of [loc]s
// the ignore Location vector must be sorted
template<int N, typename Options, typename SectionT>
int remove_option_section(
	Board<Options, N>& board,
	const SectionT section,
	const std::vector<Location<N>>& ignore,
	const std::vector<unsigned int>& values)
{
	{
		using Board = Board<Options, N>;
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid_value<N>(values));
		assert(is_same_section(section, ignore));
	}
	int changes{0};
	const auto begin = ignore.cbegin();
	const auto end   = ignore.cend();

	for (auto itr = section.cbegin(); itr != section.cend(); ++itr)
	{
		if (not(itr->is_answer()) and
			std::none_of(begin, end, [L1 = itr.location()](Location<N> L2) {
				return L1 == L2;
			})) // <algorithm>
		{
			for (auto v : values)
			{ //! Cascade (!)
				changes += remove_option(board, itr.location(), v);
			}
		}
	}
	return changes;
}

//	Solver: Find and set options appearing only once in a section as answer
template<int N>
template<typename SectionT>
inline int Solver<N>::unique_in_section(const SectionT section)
{
	static_assert(std::is_base_of_v<typename Board::Section, SectionT>);

	const auto worker = appearance_once<N>(section);
	return set_uniques(board_, section, worker);
}

//	Set unique values in section as answer
template<int N, typename Options, typename SectionT>
inline auto set_uniques(
	Board<Options, N>& board, const SectionT section, const Options worker)
{
	using value_t = unsigned int;
	{
		static_assert(
			std::is_base_of_v<typename Board<Options, N>::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::is_input<iterator>);
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(worker.test(0)); // answer-bit always set (unanswered)
								// not required when using count_all()
	}
	int changes{0};

	if (worker.count_all() > 0) //! performs better (here) than count()
	{
		const auto begin = section.cbegin();
		const auto end   = section.cend();

		for (value_t value{1}; value < static_cast<value_t>(worker.size());
			 ++value)
		{
			if (worker[value])
			{
				const auto itr = std::find_if( // <algorithm>
					begin,
					end,
					[value](Options O) { return O.is_option(value); });
				if (itr != end)
				{
					Solver<N>(board).setValue(itr.location(), value);
					changes +=
						Solver<N>(board).single_option(itr.location(), value);
					++changes;
				}
				else
				{
					assert(false); // value is not an option
				}
			}
		}
	}
	return changes;
}

//	Solver: find and process values appearing 1 to base_size times in section:
//	[row/col] IF all in same block -> remove from rest of block
//	[block] IF all in same row/col -> remove from rest of row/col
template<int N>
template<typename SectionT>
inline int Solver<N>::section_exclusive(const SectionT section)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
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
			changes += set_uniques(board_, section, appearing[1]);
			renew_appearing();
		}
		else if (appearing[i].count_all() > 0)
		{
			// for [row/col]: if in same block: remove from rest block
			// for [block]: if in same row/col: remove from rest row/col
			if (const int tmp_ = set_section_locals(
					board_, section, static_cast<int>(i), appearing[i]))
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

//	for [row/col]: if all in same block, remove [values] from rest block
template<int N, typename Options, typename SectionT>
inline int set_section_locals(
	Board<Options, N>& board,
	const SectionT section,
	const int rep_count,
	const Options worker)
{
	using value_t = unsigned int;
	{
		using Board = Board<Options, N>;
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(rep_count > 1);  // should have been cought by caller
								// use the set_uniques specialization
		assert(rep_count <= N); // won't fit in single block-row/col
		assert(worker.count_all() > 0);
	}
	int changes{0};

	for (value_t value{1}; // start at 1, to skip the answer-bit
		 value < static_cast<value_t>(worker.size());
		 ++value)
	{
		if (worker[value])
		{
			const auto locations = find_locations<N>(section, value, rep_count);
			if (locations.size() != static_cast<size_t>(rep_count))
			{
				assert(changes > 0); // changed by earlier value in worker
			}
			else if (is_same_block<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of block
				changes += remove_option_section(
					board, board.block(locations[0]), locations, value);
			}
		}
	}
	return changes;
}

//	remove found set in block from rest of its row/col
template<int N, typename Options>
inline int set_section_locals(
	Board<Options, N>& board,
	const Board_Section::Block<Options, N> block,
	const int rep_count,
	const Options worker)
{
	using value_t = unsigned int;
	{
		using BlockT   = Board_Section::Block<Options, N>;
		using iterator = typename BlockT::const_iterator;
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(rep_count > 1);  // should have been cought by caller
								// use the set_uniques specialization
		assert(rep_count <= N); // won't fit in single block-row/col
		assert(worker.count_all() > 0);
	}
	int changes{0};

	for (value_t value{1}; // start at 1, to skip the answer-bit
		 value < static_cast<value_t>(worker.size());
		 ++value)
	{
		if (worker[value])
		{
			const auto locations = find_locations<N>(block, value, rep_count);
			if (locations.size() != static_cast<size_t>(rep_count))
			{
				assert(changes > 0); // changed by earlier value in worker
			}
			else if (is_same_row<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of row
				changes += remove_option_outside_block(
					board, board.row(locations[0]), locations[0], value);
			}
			else if (is_same_col<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of col
				changes += remove_option_outside_block(
					board, board.col(locations[0]), locations[0], value);
			}
		}
	}
	return changes;
}

//	List locations in [section] where [value] is an option
template<int N, typename SectionT>
auto find_locations(
	const SectionT section, unsigned int value, const int rep_count)
{
	{
		using Options = Options<elem_size<N>>;
		using Board   = Board<Options, N>;
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		assert(rep_count <= elem_size<N>);
	}
	const auto begin = section.cbegin();
	const auto end   = section.cend();

	return find_locations<N>(begin, end, value, rep_count);
}

//	List locations where [value] is an option
template<int N, typename ItrT>
auto find_locations(
	const ItrT begin,
	const ItrT end,
	const unsigned int value,
	const int rep_count)
{
	using Options = Options<elem_size<N>>;
	{
		static_assert(Utility_::is_input<ItrT>);
		using iterator = typename ItrT::const_iterator;
		static_assert(Utility_::iterator_to<iterator, const Options>);
		assert(is_valid_value<N>(value));
		assert(rep_count > 0 && rep_count <= full_size<N>);
	}
	std::vector<Location<N>> locations{};
	locations.reserve(static_cast<size_t>(rep_count));

	auto last = begin;

	for (int i{0}; i < rep_count; ++i)
	{
		last = std::find_if(
			last, end, [value](Options O) { return O.is_option(value); });
		if (last == end)
		{ // rep_count too large
			break;
		}
		locations.push_back(last.location());
		++last;
	}
	assert(not locations.empty());
	return locations;
}

//	List locations in [section] where [value] is an option
template<int N, typename SectionT>
auto find_locations(const SectionT section, const Options<elem_size<N>> value)
{
	{
		using Options = Options<elem_size<N>>;
		using Board   = Board<Options, N>;
		static_assert(std::is_base_of_v<Board::Section, SectionT>);
	}
	std::vector<Location<N>> locations{};

	auto last      = section.cbegin();
	const auto end = section.cend();

	for (last = std::find(last, end, value); last != end;
		 last = std::find(last, end, value))
	{
		locations.push_back(last.location());
		++last;
	}
	return locations;
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


/*	Remove local macros */
#undef DUAL_ON_REMOVE
#undef MULTIPLE_ON_REMOVE

} // namespace Sudoku
