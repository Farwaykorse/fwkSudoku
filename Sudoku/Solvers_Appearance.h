//===--	Sudoku/Appearance.h												--===//
//
//	Helper functions, collecting options by appearance count in a given set.
//===---------------------------------------------------------------------===//
#pragma once

#include "Options.h"
#include "Location.h"
#include <type_traits>

// Forward declarations
#include "Board.fwd.h"
#include "Solver.fwd.h"


namespace Sudoku::Solvers_
{
//TODO rename: use a generalized function name
//TODO 1. general version taking [int] 1-full_size for appearance count
//TODO 2. default to base_size
//TODO 3. specialize appearance_once version (unique?)

namespace
{
// Local namespace, helper-functions
template<int N>
static constexpr int elem_size = Location<N>().elem_size; // default 9
}

// range-based-for
template<int N, typename T>
auto appearance_once(T section)
{
	using Options = Options<elem_size<N>>;
	using Board   = Board<Options, N>;

	// Board_Sections are based on const_Row
	static_assert(std::is_base_of_v<Board::Section, T>);
	// check iterator traits
	{
		using traits = std::iterator_traits<T::iterator>;
		static_assert(std::is_object_v<traits::iterator_category>);
		static_assert(std::is_same_v<traits::value_type, Options>);
	}

	Options sum(0);    // helper all used
	Options worker(0); // multiple uses OR answer
	for (const Options& item : section)
	{
		//TODO make (lambda)function, shared with begin-end version
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

//	return a mask for values with a single appearance
template<int E, typename InItr_>
Options<E> appearance_once(const InItr_ begin, const InItr_ end)
{
	// TODO determine iterator types, should point to Options...

	Options<E> sum(0);    // helper all used
	Options<E> worker(0); // multiple uses OR answer
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
	return worker.flip(); // multiple uses -> single-use
}

//	returning options collected by appearance count in input-dataset
template<int N, typename InItr_>
auto appearance_sets(const InItr_ begin, const InItr_ end)
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
// A: results all read as answer, unless [n] or less unanswered
//? Working with more or less than [elem_size] input elements?
// Less than 9: possible worker[0] is not empty
// Less than 3: no use for worker[3]
// More than 9: shouldn't be an issue
// TODO Test different inputs
*/
{
	using Options = Sudoku::Options<N * N>;
	// To limit processing time, counting up to N
	constexpr int max = N; // default: (9x9 board) up-to 3 times
	std::array<Options, max + 1> worker{};
	// start with all false
	for (auto& obj : worker) obj.flip();

	// Collect options by appearence count
	// worker[n] contains options appearing more than n times (or answer)
	for (auto elem_itr = begin; elem_itr != end; ++elem_itr)
	{
		if (elem_itr->is_answer())
		{
			// add answer to all
			for (int i{max}; i >= 0; --i)
			{
				worker[i] = *elem_itr + worker[i];
			}
		}
		else
		{
			for (int i{max}; i > 0; --i)
			{
				worker[i] += (worker[i - 1] & *elem_itr); // AND
			}
			worker[0] += *elem_itr;
		}
	}
	// flip -> worker[n] contains options appearing n times or less
	for (auto&& option_set : worker)
	{
		option_set.flip();
	}
	// TODO test: can trigger on smaller sets; better
	assert(worker[0].is_empty()); // fails if not all options exist
								  // won't trigger if answer-bit set

	// xor -> worker[n] options appearing n times
	for (int i{max}; i > 1; --i)
	{
		worker[i].XOR(worker[i - 1]);
	}
	return worker;
}

} // namespace Sudoku::Solvers_
