//===--- Sudoku/Solver_find.h                                           ---===//
//
// Helper functions, to find available options
//===----------------------------------------------------------------------===//
// list_where_*
// appearance_*: collecting options by appearance count in a given set.
//===----------------------------------------------------------------------===//
#pragma once

#include "Board_Section_traits.h"
#include "Location.h"
#include "Options.h"
#include "Size.h"
#include "Value.h"
#include "traits.h"

#include <gsl/gsl>

#include <array>
#include <vector>

#include <algorithm> // find, find_if
#include <iterator>
#include <type_traits> // is_base_of

#include "Board.fwd.h" // Forward declarations

#include <cassert>


namespace Sudoku
{
//===----------------------------------------------------------------------===//
template<int N, typename SectionT>
[[nodiscard]] auto
	list_where_option(SectionT, Value, ptrdiff_t rep_count = elem_size<N>);

template<int N, typename ItrT>
[[nodiscard]] auto
	list_where_option(ItrT begin, ItrT end, Value, ptrdiff_t rep_count = 0);

template<int N, typename SectionT, typename Options = Options<elem_size<N>>>
[[nodiscard]] auto
	list_where_option(const SectionT, const Options sample) noexcept(true);

template<int N, typename SectionT, typename Options = Options<elem_size<N>>>
[[nodiscard]] auto list_where_equal(SectionT, Options sample) noexcept(true);

template<int N, typename Options = Options<elem_size<N>>>
[[nodiscard]] auto
	list_where_subset(const Board<Options, N>&, const Options) noexcept(true);

template<int N, typename SectionT, typename Options = Options<elem_size<N>>>
[[nodiscard]] auto
	list_where_subset(const SectionT, const Options) noexcept(true);

template<int N, typename SectionT, typename Options = Options<elem_size<N>>>
[[nodiscard]] auto
	list_where_any_option(const SectionT, const Options sample) noexcept(true);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
[[nodiscard]] Options appearance_once(SectionT section) noexcept;

template<int N, typename Options = Options<elem_size<N>>, typename InItr_>
[[nodiscard]] Options
	appearance_once(const InItr_ begin, const InItr_ end) noexcept;

template<int N, typename SectionT>
[[nodiscard]] auto appearance_sets(const SectionT section);

template<int N, typename InItr_>
[[nodiscard]] auto appearance_sets(const InItr_ begin, const InItr_ end);

//===----------------------------------------------------------------------===//


// List locations in [section] where [value] is an option
template<int N, typename SectionT>
inline auto list_where_option(
	const SectionT section,
	Value value,
	const ptrdiff_t rep_count /*= elem_size<N>*/)
{
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
		assert(rep_count > 0 && rep_count <= elem_size<N>);
	}
	const auto begin = section.cbegin();
	const auto end   = section.cend();

	return list_where_option<N>(begin, end, value, rep_count);
}

// List locations where [value] is an option
template<int N, typename ItrT>
auto list_where_option(
	const ItrT begin,
	const ItrT end,
	const Value value,
	ptrdiff_t rep_count /*= 0*/)
{
	using Options = Options<elem_size<N>>;
	{
		static_assert(traits::is_input<ItrT>);
		static_assert(
			traits::iterator_to<ItrT, const Options> ||
			traits::iterator_to<ItrT, Options>);
		assert(is_valid<N>(value));

		if (rep_count == 0)
			rep_count = std::distance(begin, end);
		else
			assert(rep_count > 0 && rep_count <= std::distance(begin, end));
	}

	std::vector<Location<N>> locations{};
	locations.reserve(gsl::narrow_cast<size_t>(rep_count));

	const auto check_option = [value](Options O) {
		return is_option(O, value);
	};

#if true
	// slightly faster, it saves one run of find_if on each execution,
	// but rep_count can be too small
	auto itr = begin;
	for (gsl::index i{0}; i < rep_count; ++i)
	{
		itr = std::find_if(itr, end, check_option);
		if (itr == end)
		{ // rep_count too large
			break;
		}
		locations.push_back(itr.location());
		++itr;
	}
	assert( // rep_count not too low
		itr == end || std::find_if(itr, end, check_option) == end);
#else
	for (auto last{std::find_if(begin, end, check_option)}; last != end;
		 last = std::find_if(last, end, check_option))
	{
		locations.push_back(last.location());
		++last;
	}
	assert( // rep_count not too low
		locations.size() <= gsl::narrow_cast<size_t>(rep_count));
#endif
	return locations;
}

// List locations in [section] containing [sample]
template<int N, typename SectionT, typename Options>
auto list_where_option( // NOLINT(bugprone-exception-escape)
	const SectionT section,
	const Options sample) noexcept(true)
{ // vector creation and growth could potentially throw, out of memory.
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
	}
	std::vector<Location<N>> locations{};
	locations.reserve(size_t{elem_size<N>});

	const auto check_option = [sample](Options O) noexcept {
		return sample == shared(O, sample);
	};
	const auto end = section.cend();

	for (auto last{std::find_if(section.cbegin(), end, check_option)};
		 last != end;
		 last = std::find_if(last, end, check_option))
	{
		if (!is_answer_fast(*last))
		{
			locations.push_back(last.location());
		}
		++last;
	}
	return locations;
}

// List locations in [section] equal to [sample]
template<int N, typename SectionT, typename Options>
auto list_where_equal( // NOLINT(bugprone-exception-escape)
	const SectionT section,
	const Options sample) noexcept(true)
{ // vector creation and growth could potentially throw, out of memory.
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
		static_assert(std::is_same_v<typename SectionT::value_type, Options>);
	}
	std::vector<Location<N>> locations{};
	locations.reserve(size_t{elem_size<N>});

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
auto list_where_subset( // NOLINT(bugprone-exception-escape)
	const Board<Options, N>& board,
	const Options sample) noexcept(true)
{ // vector creation and growth could potentially throw, out of memory.
	using Location = Location<N>;
	std::vector<Location> list{};
	list.reserve(sample.count()); // minimum

	for (int i{}; i < full_size<N>; ++i)
	{
		const auto& other = board[Location(i)];
		if (!is_answer_fast(other) && // exclude processed answers
			(other == sample || shared(sample, other) == other))
		{
			list.push_back(Location(i));
		}
	}
	return list;
}

// All locations where available options are a subset of the sample.
template<int N, typename SectionT, typename Options>
auto list_where_subset( // NOLINT(bugprone-exception-escape)
	const SectionT section,
	const Options sample) noexcept(true)
{ // vector creation and growth could potentially throw, out of memory.
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
	}
	using Location = Location<N>;
	std::vector<Location> list{};
	list.reserve(sample.count());

	const auto end = section.cend();
	for (auto itr = section.cbegin(); itr != end; ++itr)
	{
		const auto& other = *itr;
		if (!is_answer_fast(other) && // exclude processed answers
			(other == sample || shared(sample, other) == other))
		{
			list.push_back(itr.location());
		}
	}
	return list;
}

// All locations containing an option present in [sample]
template<int N, typename SectionT, typename Options>
auto list_where_any_option( // NOLINT(bugprone-exception-escape)
	const SectionT section,
	const Options sample) noexcept(true)
{ // vector creation and growth could potentially throw, out of memory.
	{
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
	}
	using Location = Location<N>;
	std::vector<Location> locations{};
	locations.reserve(sample.count_all());

	const auto check_option = [sample](Options O) noexcept {
		return is_answer_fast(O) ? false
								 : !(shared(O, sample).count_all() == 0u);
	};
	const auto end = section.cend();

	for (auto itr{std::find_if(section.cbegin(), end, check_option)};
		 itr != end;
		 itr = std::find_if(itr, end, check_option))
	{
		locations.push_back(itr.location());
		++itr;
	}
	return locations;
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
Step 3) XOR [n-1]
				|	== empty!	|	000	010	000	|	001	000	000	|	010	000	100
				|	==0x		|	==1x		|	==2x		|	==3x
				worker[n] contains options appearing exactly n times
// Q: What about the answer bit?
// A: Always true (==not answered)
//? Working with more or less than [elem_size] input elements?
// Less than 9: possible worker[0] is not empty
// Less than 3: no use for worker[3]
// More than 9: shouldn't be an issue
*/
	// clang-format on

	using Options = Options<elem_size<N>>;
	{
		static_assert(
			traits::iterator_to<InItr_, Options> ||
			traits::iterator_to<InItr_, const Options>);
	}
	// To limit processing time, counting up to N
	constexpr auto max = size_t{N}; // default: (9x9 board) up-to 3 times
	std::array<Options, max + 1> worker{};
	worker.fill(Options(Value{0}));

	// Collect options by appearance count
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
	for (auto& option_set : worker)
	{
		option_set.flip();
	}
	// TODO test: can trigger on partial sections; improve?
	// fails if not all options exist
	assert(worker[0].count_all() == 0);

	// XOR -> worker[n] options appearing n times
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
{ // referral function, creates iterators
	return appearance_sets<N>(section.cbegin(), section.cend());
}


// return a mask for values with a single appearance
template<int N, typename Options, typename InItr_>
Options appearance_once(const InItr_ begin, const InItr_ end) noexcept
{
	{
		static_assert(traits::is_input<InItr_>);
		static_assert(
			traits::iterator_to<InItr_, Options> ||
			traits::iterator_to<InItr_, const Options>);
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
		static_assert(Board_Section::traits::is_Section_v<SectionT>);
		static_assert(std::is_same_v<typename SectionT::value_type, Options>);
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
