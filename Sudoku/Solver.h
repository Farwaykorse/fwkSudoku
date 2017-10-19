//===--	Sudoku/Solver.h													--===//
//
//	Solver function container-class
//	Collects and gives access to solver functions acting on a Board<Options>&
//===---------------------------------------------------------------------===//
#pragma once

#include "Location_Utilities.h"
#include "Options.h"
#include "Solver_Utilities.h"
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
	using Location = Location<N>;

	static constexpr int base_size = Location().base_size; // default 3
	static constexpr int elem_size = Location().elem_size; // default 9
	static constexpr int full_size = Location().full_size; // default 81

	using Options = Options<elem_size>;
	using Board   = Sudoku::Board<Options, base_size>;
	using Row     = typename Board::Row;
	using Col     = typename Board::Col;
	using Block   = typename Board::Block;
	template<typename Itr>
	using if_forward = std::enable_if_t<Solvers_::is_forward<Itr>>;

public:
	Solver(Board&);

	// Edit Board
	void setValue(Location, int);
	template<typename InItr_, typename = if_forward<InItr_>>
	void setValue(InItr_ begin, InItr_ end);

	// remove an option: triggers solvers single_option()
	int remove_option(Location, int value);
	template<typename SectionT>
	int remove_option_section(SectionT, Location ignore, int value);
	template<typename SectionT>
	int remove_option_section(
		SectionT, const std::vector<Location>& ignore, int value);
	template<typename SectionT>
	int remove_option_section(
		SectionT,
		const std::vector<Location>& ignore,
		const std::vector<int>& value);
	template<typename SectionT>
	int remove_option_outside_block(SectionT, Location block, int value);

	// Solvers
	int single_option(Location);
	int single_option(Location, int value);
	int dual_option(Location);
	int multi_option(Location, size_t = 0);

	template<typename SectionT>
	int unique_in_section(SectionT);
	template<typename SectionT>
	auto set_uniques(SectionT, const Options& worker);
	template<typename SectionT>
	int section_exclusive(SectionT);
	int section_exclusive(Block);

private:
	Board& board_;

	template<typename SectionT>
	int set_section_locals(SectionT, int rep_count, const Options& worker);
	int set_section_locals(Block, int rep_count, const Options& worker);

	template<typename SectionT>
	auto find_locations(SectionT, int rep_count, int value) const;
	//? deprecated ?? use on full board?
	template<typename InItr_>
	auto find_locations(
		InItr_ begin, InItr_ end, int rep_count, int value) const;
};


template<int N>
inline Solver<N>::Solver(Board& options) : board_(options)
{
	// empty constructor
}

//	IF valid, Make [value] the answer for [loc]
//	No processing
template<int N>
inline void Solver<N>::setValue(const Location loc, const int value)
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
template<typename InItr_, typename> // at least forward_iterator
inline void Solver<N>::setValue(const InItr_ begin, const InItr_ end)
{
	{
		static_assert(Solvers_::is_forward<InItr_>);
		static_assert(Solvers_::iterator_to<InItr_, int>);
		assert(end - begin == full_size);
	}
	int n{0};
	for (auto itr = begin; itr != end; ++itr)
	{
		Location loc(n++); // start at 0!
		if (*itr > 0 && board_.at(loc).is_option(*itr))
		{
			setValue(loc, *itr);
			single_option(loc, *itr);
		}
		// check invalid value or conflict
		assert(*itr == 0 || board_.at(loc).is_answer(*itr));
	}
	assert(n == full_size);
}

//	remove option from element, make answer if last option
template<int N>
inline int Solver<N>::remove_option(const Location loc, const int value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));

	int changes{};
	auto& item{board_.at(loc)};

	if (item.is_option(value))
	{
		++changes;
		const int count = item.remove_option(value).count();
		assert(count > 0); // never trigger, removed last option

		if (count == 1)
		{
			changes += single_option(loc);
		}
#if DUAL_ON_REMOVE == true
		if (count == 2)
		{
			changes += dual_option(loc);
		}
#endif // dual
#if MULTIPLE_ON_REMOVE == true
		changes += multi_option(loc, count);
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

	if (const int answer{board_[loc].get_answer()})
	{
		return single_option(loc, answer);
	}
	return 0;
}

//	Process answer from [loc], update board_ options
template<int N>
inline int Solver<N>::single_option(const Location loc, const int value)
{
	assert(is_valid(loc));
	assert(is_valid_value<N>(value));

	assert(board_.at(loc).test(value));
	assert(board_[loc].count_all() == 1);

	if (!board_[loc].is_answer(value))
	{
		setValue(loc, value);
	}
	int changes{};
	changes += remove_option_section(board_.row(loc), loc, value);
	changes += remove_option_section(board_.col(loc), loc, value);
	changes += remove_option_section(board_.block(loc), loc, value);
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
	for (int i{}; i < full_size; ++i)
	{
		// find exact same in board
		if (board_[Location(i)] == item && Location(i) != loc)
		{
			// Remove values for rest of shared elements
			if (is_same_row(loc, Location(i)))
			{
				changes += remove_option_section(
					board_.row(loc), sorted_loc(Location(i)), item.available());
			}
			else if (is_same_col(loc, Location(i)))
			{
				changes += remove_option_section(
					board_.col(loc), sorted_loc(Location(i)), item.available());
			}
			// run don't if already answered in one of the previous
			if (is_same_block(loc, Location(i)) && !item.is_answer())
			{
				// NOTE this is slow
				changes += remove_option_section(
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

	if (!count)
	{
		count = static_cast<size_t>(board_[loc].count());
	}
	constexpr auto specialize = 2; // use specialization below and including
	constexpr auto max_size   = elem_size / 2; //?? Assumption, Proof needed
	if (specialize < count && count <= max_size)
	{
		int changes{};                    // performance counter
		const Options& item{board_[loc]}; // input item, to find matches to
		std::vector<Location> list{};     // potential matches

		// traverse whole board
		for (int i{}; i < full_size; ++i)
		{
			const auto& other = board_[Location(i)];
			// find exact same in board
			// TODO rework to also pick-up cels containing [2,n) values
			if (other.count() > 0 && // include not processed answers
				other.count() <= base_size &&
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
			auto in_row{get_same_row(loc, list)};
			auto in_col{get_same_col(loc, list)};
			auto in_block{get_same_block(loc, list)};
			// Remove values for rest of shared elements
			if (in_row.size() == count)
			{
				changes += remove_option_section(
					board_.row(loc), in_row, item.available());
			}
			if (in_col.size() == count)
			{
				changes += remove_option_section(
					board_.col(loc), in_col, item.available());
			}
			if (in_block.size() == count)
			{
				// NOTE this is slow
				changes += remove_option_section(
					board_.block(loc), in_block, item.available());
			}
		}
		return changes;
	}
	return 0;
}

//	remove [value] in [loc] from other elements in set
template<int N>
template<typename SectionT>
inline int Solver<N>::remove_option_section(
	const SectionT section, const Location loc, const int value)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::is_input<iterator>);
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(is_valid(loc));
		assert(is_valid_value<N>(value));
		assert(is_same_section(section, loc));
		assert(board_.at(loc).is_answer(value)); // first set as anwer!
	}
	int changes{0};
	const auto begin = section.cbegin();
	const auto end   = section.cend();

	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr.location() != loc)
		{
			changes += remove_option(itr.location(), value);
		}
	}
	return changes;
}

//	remove [value] from elements not in same block as [block_loc]
template<int N>
template<typename SectionT>
inline int Solver<N>::remove_option_outside_block(
	const SectionT section, const Location block_loc, const int value)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		static_assert(
			!std::is_base_of_v<typename Board::const_Block, SectionT>,
			"remove_option_outside_block is useless on bock");
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::is_input<iterator>);
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(is_valid(block_loc));
		assert(is_valid_value<N>(value));
		assert(intersect_block(section, block_loc));
	}
	int changes{0};
	const auto begin = section.cbegin();
	const auto end   = section.cend();

	for (auto itr = begin; itr != end; ++itr)
	{
		if (!(is_same_block(itr.location(), block_loc)))
		{
			changes += remove_option(itr.location(), value);
		}
	}
	return changes;
}

//	remove [value] from set if not part of [loc]s
template<int N>
template<typename SectionT>
inline int Solver<N>::remove_option_section(
	const SectionT section,
	const std::vector<Location>& ignore,
	const int value)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::is_input<iterator>);
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid_value<N>(value));
		assert(is_same_section(section, ignore));
	}
	int changes{0};
	const auto begin = section.cbegin();
	const auto end   = section.cend();

	// TODO maybe faster to run get_same_row/col/block first?
	// and than not to check is_option(), since it is already in remove_option

	for (auto itr = begin; itr != end; ++itr)
	{
		// TODO is the is_option check really faster?
		if (itr->is_option(value) &&
			std::none_of(ignore.cbegin(), ignore.cend(), [itr](Location loc) {
				return itr.location() == loc;
			})) // <algorithm>
		{
			changes += remove_option(itr.location(), value);
		}
	}
	return changes;
}

// remove [value]s from [section] if not part of [loc]s
// the ignore Location vector must be sorted
template<int N>
template<typename SectionT>
inline int Solver<N>::remove_option_section(
	const SectionT section,
	const std::vector<Location>& ignore,
	const std::vector<int>& values)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::is_input<iterator>);
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(is_valid(ignore));
		assert(is_valid_value<N>(values));
		assert(is_same_section(section, ignore));
	}
	int changes{0};
	const auto begin = section.cbegin();
	const auto end   = section.cend();

	for (auto itr = begin; itr != end; ++itr)
	{
		if (!(itr->is_answer()) &&
			std::none_of(ignore.cbegin(), ignore.cend(), [itr](Location loc) {
				return itr.location() == loc;
			})) // <algorithm>
		{
			for (auto v : values)
			{ //! Cascade (!)
				changes += remove_option(itr.location(), v);
			}
		}
	}
	return changes;
}

//	Solver: Find and process options appearing only once in a section
template<int N>
template<typename SectionT>
inline int Solver<N>::unique_in_section(const SectionT section)
{
	static_assert(std::is_base_of_v<typename Board::Section, SectionT>);

	const auto worker = Solvers_::appearance_once<N>(section);
	return set_uniques(section, worker);
}

//	process unique values in section
template<int N>
template<typename SectionT>
inline auto
	Solver<N>::set_uniques(const SectionT section, const Options& worker)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::is_input<iterator>);
		static_assert(Solvers_::iterator_to<iterator, const Options>);
	}
	int changes{0};

	if (worker.count_all() > 0)
	{
		const auto begin = section.cbegin();
		const auto end   = section.cend();

		for (int value{1}; value < worker.size(); ++value)
		{
			if (worker[value])
			{
				const auto itr = std::find_if( // <algorithm>
					begin,
					end,
					[value](Options O) { return O.is_option(value); });
				assert(itr != end); // doesn't exist
				setValue(itr.location(), value);
				changes += single_option(itr.location(), value);
				++changes;
			}
		}
	}
	return changes;
}

//	Solver: find and process values appearing 2x or 3x in row/col:
//	IF all in same block -> remove from rest of block
template<int N>
template<typename SectionT>
inline int Solver<N>::section_exclusive(const SectionT section)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::iterator_to<iterator, const Options>);
	}
	int changes{}; // performance counter

	auto appearing       = appearance_sets(section);
	auto renew_appearing = [&]() { appearing = appearance_sets(section); };

	size_t i{2};
	while (i < appearing.size()) // won't run if condition fails
	{
		// unique specialization
		if (appearing[1].count_all() > 0)
		{
			changes += set_uniques(section, appearing[1]);
			renew_appearing();
		}
		else if (appearing[i].count_all() > 0)
		{
			changes +=
				set_section_locals(section, static_cast<int>(i), appearing[i]);
			renew_appearing();
			++i;
		}
		else
		{
			++i;
		}
	}
	return changes;
}

//	Solver: find and process values by appearance count in Block
//	IF all in same row/col -> remove from rest
template<int N>
inline int Solver<N>::section_exclusive(const Block block)
{
	{
		using iterator = typename Block::const_iterator;
		static_assert(Solvers_::iterator_to<iterator, const Options>);
	}
	int changes{}; // performance counter

	auto appearing       = Solvers_::appearance_sets<N>(block);
	auto renew_appearing = [&]() {
		appearing = Solvers_::appearance_sets<N>(block);
	};

	size_t i{2};
	while (i < appearing.size()) // won't run if condition fails
	{
		// unique in block specialization
		if (appearing[1].count_all() > 0)
		{
			changes += set_uniques(block, appearing[1]);
			renew_appearing();
		}
		else if (appearing[i].count_all() > 0)
		{
			changes +=
				set_section_locals(block, static_cast<int>(i), appearing[i]);
			renew_appearing();
			++i;
		}
		else
		{
			++i;
		}
	}
	return changes;
	// TODO can this be added/used?
	// 2 values only appear in 2 cells -> remove rest from cells
}

//	for [row/col]: if all in same block, remove [values] from rest block
template<int N>
template<typename SectionT>
inline int Solver<N>::set_section_locals(
	const SectionT section, const int rep_count, const Options& worker)
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(worker.count_all() > 0); // should have been cought by caller
	}
	int changes{0};

	for (size_t value{1}; value < worker.size(); ++value)
	{
		if (worker[value])
		{
			const auto locations = find_locations(section, rep_count, value);
			assert(locations.size() <= N); // won't fit in single block-row/col
			assert(locations.size() > 1);  // use the set_uniques specialization

			if (is_same_block(locations.cbegin(), locations.cend()))
			{ // remove from rest of block
				changes += remove_option_section(
					board_.block(locations[0]), locations, value);
			}
		}
	}
	return changes;
}

//	remove found set in block from rest of its row/col
template<int N>
inline int Solver<N>::set_section_locals(
	const Block block, const int rep_count, const Options& worker)
{
	{
		using iterator = typename Block::const_iterator;
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(worker.count_all() > 0); // should have been cought by caller
	}
	int changes{0};

	for (int value{1}; value < worker.size(); ++value)
	{
		if (worker[value])
		{
			const auto locations = find_locations(block, rep_count, value);
			assert(locations.size() <= N); // won't fit in single block-row/col
			assert(locations.size() > 1);  // use the set_uniques specialization

			// check if all in same row/col
			if (is_same_row<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of row
				changes += remove_option_outside_block(
					board_.row(locations[0]), locations[0], value);
			}
			else if (is_same_col<N>(locations.cbegin(), locations.cend()))
			{ // remove from rest of col
				changes += remove_option_outside_block(
					board_.col(locations[0]), locations[0], value);
			}
			else
			{
				continue;
			} // not in same row/col
		}
	}
	return changes;
}

//	List locations containing value
template<int N>
template<typename SectionT>
inline auto Solver<N>::find_locations(
	const SectionT section, const int rep_count, const int value) const
{
	{
		static_assert(std::is_base_of_v<typename Board::Section, SectionT>);
		using iterator = typename SectionT::const_iterator;
		static_assert(Solvers_::is_input<iterator>);
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(is_valid_value<N>(value));
		assert(rep_count > 0 && rep_count <= board_.full_size);
	}
	const auto begin = section.cbegin();
	const auto end   = section.cend();
	std::vector<Location> locations{};
	auto last = begin;

	for (int i{0}; i < rep_count; ++i)
	{
		last = std::find_if(
			last, end, [value](Options O) { return O.is_option(value); });
		assert(last != end); // incorrect rep_count
		locations.emplace_back(last.location());
		++last;
	}
	return locations;
}

//	List locations containing value
template<int N>
template<typename InItr_>
inline auto Solver<N>::find_locations(
	const InItr_ begin,
	const InItr_ end,
	const int rep_count,
	const int value) const
{
	{
		static_assert(Solvers_::is_input<InItr_>);
		using iterator = typename InItr_::const_iterator;
		static_assert(Solvers_::iterator_to<iterator, const Options>);
		assert(is_valid_value<N>(value));
		assert(rep_count > 0 && rep_count <= board_.full_size);
	}
	std::vector<Location> locations{};
	auto last = begin;

	for (int i{0}; i < rep_count; ++i)
	{
		last = std::find_if(
			last, end, [value](Options O) { return O.is_option(value); });
		assert(last != end); // incorrect rep_count
		locations.emplace_back(last.location());
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
