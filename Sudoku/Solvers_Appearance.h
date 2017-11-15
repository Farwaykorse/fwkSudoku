//===--	Sudoku/Appearance.h												--===//
//
//	Helper functions, collecting options by appearance count in a given set.
//===---------------------------------------------------------------------===//
#pragma once

#include "Iterator_Utilities.h"
#include "Location.h" // elem_size<N>
#include "Options.h"
#include <iterator>
#include <type_traits> // is_base_of

// Forward declarations
#include "Board.fwd.h"
#include "Solver.fwd.h"


namespace Sudoku
{
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
Options appearance_once(SectionT section);

template<int N, typename Options = Options<elem_size<N>>, typename InItr_>
Options appearance_once(const InItr_ begin, const InItr_ end);

template<int N, typename SectionT>
auto appearance_sets(const SectionT section);

template<int N, typename InItr_>
auto appearance_sets(const InItr_ begin, const InItr_ end);


//===---------------------------------------------------------------------===//


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
	worker.fill(Options(0));

	// Collect options by appearence count
	// worker[n] contains options appearing more than n times (or answer)
	for (auto elem_itr = begin; elem_itr != end; ++elem_itr)
	{
		if (elem_itr->is_answer())
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
				worker[i] += (worker[i - 1] & *elem_itr); // OR( AND )
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
		worker[i].XOR(worker[i - 1]);
		worker[i] += Options(0); // set not-answered
	}
	return worker;
}

//	returning options collected by appearance count in section
template<int N, typename SectionT>
inline auto appearance_sets(const SectionT section)
{ // referal function, creates iterators
	return appearance_sets<N>(section.cbegin(), section.cend());
}


//	return a mask for values with a single appearance
template<int N, typename Options, typename InItr_>
Options appearance_once(const InItr_ begin, const InItr_ end)
{
	{
		static_assert(Utility_::is_input<InItr_>);
		static_assert(Utility_::iterator_to<InItr_, const Options>);
	}
	Options sum(0);    // helper all used
	Options worker(0); // multiple uses OR answer
	for (auto itr = begin; itr != end; ++itr)
	{
		if (itr->is_answer())
		{
			worker = *itr + worker;
		}
		else
		{
			worker += (sum & *itr);
			sum += *itr;
		}
	}
	assert(worker[0] == true);
	return worker.flip(); // multiple uses -> single-use
}

// range-based-for
template<int N, typename Options, typename SectionT>
Options appearance_once(SectionT section)
{
	{
		using Section = typename Board_Section::Section<Options, N>;
		static_assert(std::is_base_of_v<Section, SectionT>);
		using namespace Utility_;
		static_assert(
			iterator_to<typename SectionT::const_iterator, const Options>);
	}
	Options sum(0);    // helper all used
	Options worker(0); // multiple uses OR answer
	for (const Options& item : section)
	{
		if (item.is_answer())
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
