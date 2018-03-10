//===--- Sudoku/Solver_find.h                                           ---===//
//
// Helper functions, to find available options
//===----------------------------------------------------------------------===//
// find_locations
// Apperance_*: collecting options by appearance count in a given set.
//===----------------------------------------------------------------------===//
#pragma once

#include "Iterator_Utilities.h"
#include "Location.h"
#include "Options.h"
#include "Size.h"
#include "Value.h"

#include <gsl/gsl>
#include <array>
#include <vector>
#include <algorithm> // find, find_if
#include <iterator>
#include <type_traits> // is_base_of
#include <cassert>

// Forward declarations
#include "Board.fwd.h"


namespace Sudoku
{
//===----------------------------------------------------------------------===//
template<int N, typename SectionT>
auto find_locations(SectionT, Value, int rep_count = elem_size<N>);

template<int N, typename ItrT>
auto find_locations(ItrT begin, ItrT end, Value, int rep_count = elem_size<N>);

template<int N, typename SectionT, typename Options = Options<elem_size<N>>>
auto find_locations(SectionT, Options);

template<int N, typename Options = Options<elem_size<N>>>
auto subset_locations(const Board<Options, N>&, const Options) noexcept(true);

template<int N, typename SectionT, typename Options = Options<elem_size<N>>>
auto subset_locations(const SectionT, const Options) noexcept(true);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
Options appearance_once(SectionT section) noexcept;

template<int N, typename Options = Options<elem_size<N>>, typename InItr_>
Options appearance_once(const InItr_ begin, const InItr_ end) noexcept;

template<int N, typename SectionT>
auto appearance_sets(const SectionT section);

template<int N, typename InItr_>
auto appearance_sets(const InItr_ begin, const InItr_ end);

//===----------------------------------------------------------------------===//


//	List locations in [section] where [value] is an option
template<int N, typename SectionT>
auto find_locations(const SectionT section, Value value, const int rep_count)
{
	{
		using Options       = Options<elem_size<N>>;
		using Board_Section = Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
		assert(rep_count <= elem_size<N>);
	}
	const auto begin = section.cbegin();
	const auto end   = section.cend();

	return find_locations<N>(begin, end, value, rep_count);
}

//	List locations where [value] is an option
template<int N, typename ItrT>
auto find_locations(
	const ItrT begin, const ItrT end, const Value value, const int rep_count)
{
	using Options = Options<elem_size<N>>;
	{
		static_assert(Utility_::is_input<ItrT>);
		static_assert(Utility_::iterator_to<ItrT, const Options>);
		assert(is_valid<N>(value));
		assert(rep_count > 0 && rep_count <= full_size<N>);
	}
	std::vector<Location<N>> locations{};
	locations.reserve(gsl::narrow_cast<size_t>(rep_count));

	auto last = begin;

	for (int i{0}; i < rep_count; ++i)
	{
		last = std::find_if(last, end, [value](Options O) noexcept {
			return is_option(O, value);
		});
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

//	List locations in [section] equal to [sample]
template<int N, typename SectionT, typename Options>
auto find_locations(const SectionT section, const Options sample)
{
	{
		using Board_Section = Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
	}
	std::vector<Location<N>> locations{};

	const auto end = section.cend();

	for (auto last{std::find(section.cbegin(), end, sample)}; last != end;
		 last = std::find(last, end, sample))
	{
		locations.push_back(last.location());
		++last;
	}
	return locations;
}

// All locations where available options are a subset of the sample.
template<int N, typename Options>
auto subset_locations(
	const Board<Options, N>& board, const Options sample) noexcept(true)
{ // vector creation and growth could potentially throw, out of memory.
	using Location = Location<N>;
	std::vector<Location> list{};
	list.reserve(sample.count()); // minimum

	for (int i{}; i < full_size<N>; ++i)
	{
		const auto& other  = board[Location(i)];
		if (!is_answer_fast(other) && // exclude processed answers
			// containing a subset of the values in [item]
			(other == sample || shared(sample, other) == other))
		{
			list.push_back(Location(i));
		}
	}
	return list;
}

// All locations where available options are a subset of the sample.
template<int N, typename SectionT, typename Options>
auto subset_locations(
	const SectionT section, const Options sample) noexcept(true)
{ // vector creation and growth could potentially throw, out of memory.
	{
		using Board_Section = Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Board_Section, SectionT>);
	}
	using Location = Location<N>;
	std::vector<Location> list{};
	list.reserve(sample.count());

	const auto end = section.cend();
	for (auto itr = section.cbegin(); itr != end; ++itr)
	{
		const auto& other = *itr;
		if (!is_answer_fast(other) && // exclude processed answers
			// containing a subset of the values in [item]
			(other == sample || shared(sample, other) == other))
		{
			list.push_back(itr.location());
		}
	}
	return list;
}

//===----------------------------------------------------------------------===//

//	returning options collected by appearance count in input-dataset
template<int N, typename InItr_>
auto appearance_sets(const InItr_ begin, const InItr_ end)
{
	// clang-format off
/*
Example illustration
Step 1) Collect
input			worker[n]
				|	[0]			|	[1]			|	[2]			|	[3]
1	100 000 001	|	100	000	001	|	000	000	000	|				|
2	110 100 010	|	110	100	011	|	100	000	000	|	000	000	000	|
3	010 000 011	|	110	100	011	|	110	000	011	|	000	000	000	|	000	000	000
4	000 000 011	|	110	100	011	|	110	000	011	|	000	000	011	|	000	000	000
5	101 100	111	|	111	100	111	|	110	100	011	|	100	000	011	|	000	000	011
6 A	000	001	000	|	111	101	111	|	110	101	011	|	100	001	011	|	000	001	011
7	101	100	110	|	111	101	111	|	111	101	111	|	100	101	011	|	100	001	011
8	100	110	010	|	111	111	111	|	111	101	111	|	100	101	011	|	100	101	011
9	010	000	101	|				|	111	101	111	|	110	101	111	|	100	101	011
				worker[n] contains options appearing more than n times
Step 2) flip
				|	000	000	000	|	000	010	000	|	001	010	000	|	011	010	100
532	419	365		|	==0x		|	==1x		|	<=2x		|	<=3x
				worker[n] contains options appearing n times or less
Step 3) xor [n-1]
				|	== empty!	|	000	010	000	|	001	000	000	|	010	000	100
				|	==0x		|	==1x		|	==2x		|	==3x
				worker[n] contains options appearing exactly n times
// Q: What about the answer bit?
// A: Always true (==not anwered)
//? Working with more or less than [elem_size] input elements?
// Less than 9: possible worker[0] is not empty
// Less than 3: no use for worker[3]
// More than 9: shouldn't be an issue
*/
	// clang-format on

	using Options = Options<elem_size<N>>;
	{
		static_assert(Utility_::iterator_to<InItr_, const Options>);
	}
	// To limit processing time, counting up to N
	constexpr size_t max = N; // default: (9x9 board) up-to 3 times
	std::array<Options, max + 1> worker{};
	worker.fill(Options(Value{0}));

	// Collect options by appearence count
	// worker[n] contains options appearing more than n times (or answer)
	for (auto elem_itr = begin; elem_itr != end; ++elem_itr)
	{
		if (is_answer(*elem_itr))
		{
			// add answer to all
			for (auto& set : worker)
			{
				set += *elem_itr;
			} // OR
		}
		else
		{
			for (size_t i{max}; i > 0; --i)
			{
				worker.at(i) += (worker.at(i - 1) & *elem_itr); // OR( AND )
			}
			worker[0] += *elem_itr; // OR
		}
	}
	// flip -> worker[n] contains options appearing n times or less
	for (auto&& option_set : worker)
	{
		option_set.flip();
	}
	// TODO test: can trigger on partial sections; improve?
	// fails if not all options exist
	assert(worker[0].count_all() == 0);

	// xor -> worker[n] options appearing n times
	for (size_t i{max}; i > 1; --i)
	{
		worker.at(i).XOR(worker.at(i - 1));
		worker.at(i) += Options(Value{0}); // set not-answered
	}
	return worker;
}

//	returning options collected by appearance count in section
template<int N, typename SectionT>
inline auto appearance_sets(const SectionT section)
{ // referal function, creates iterators
	return appearance_sets<N>(section.cbegin(), section.cend());
}


// return a mask for values with a single appearance
template<int N, typename Options, typename InItr_>
Options appearance_once(const InItr_ begin, const InItr_ end) noexcept
{
	{
		static_assert(Utility_::is_input<InItr_>);
		static_assert(Utility_::iterator_to<InItr_, const Options>);
	}
	Options sum(Value{0});    // helper all used
	Options worker(Value{0}); // multiple uses OR answer
	for (auto itr = begin; itr != end; ++itr)
	{
		if (is_answer(*itr))
		{
			worker = *itr + worker;
		}
		else
		{
			worker += (sum & *itr);
			sum += *itr;
		}
	}
	return worker.flip(); // multiple uses -> single-use
}

// range-based-for
template<int N, typename Options, typename SectionT>
Options appearance_once(SectionT section) noexcept
{
	{
		using Section = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Section, SectionT>);
		using ::Sudoku::Utility_::iterator_to;
		static_assert(
			iterator_to<typename SectionT::const_iterator, const Options>);
	}
	Options sum(Value{0});    // helper all used
	Options worker(Value{0}); // multiple uses OR answer
	for (const Options& item : section)
	{
		if (is_answer(item))
		{
			worker = item + worker; // OR
		}
		else
		{
			worker += (sum & item); // OR ( AND )
			sum += item;
		}
	}
	return worker.flip(); // multiple uses -> single-use
}

} // namespace Sudoku
