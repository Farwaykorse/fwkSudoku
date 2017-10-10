//===--	Sudoku/Solver_Utilities.h										--===//
//
//	Helper-functions for Solvers
//===---------------------------------------------------------------------===//
#pragma once

//#include "Solver.fwd.h"
#include <iterator>
#include <type_traits>
#include "Location.fwd.h"


namespace Sudoku::Solvers_
{
// Local namespace, helper-functions
template<int N>
static constexpr int elem_size = Location<N>().elem_size; // default 9

// test iteratortype
template<typename Itr>
using Category = typename std::iterator_traits<Itr>::iterator_category;

template<typename Itr>
constexpr bool is_random =
	std::is_same_v<Category<Itr>, std::random_access_iterator_tag>;
template<typename Itr>
constexpr bool is_bidir =
	std::is_same_v<Category<Itr>, std::bidirectional_iterator_tag> ||
	is_random<Itr>;
template<typename Itr>
constexpr bool is_forward =
	std::is_same_v<Category<Itr>, std::forward_iterator_tag> || is_bidir<Itr>;
template<typename Itr>
constexpr bool is_input =
	std::is_same_v<Category<Itr>, std::input_iterator_tag> || is_forward<Itr>;

// iterator pointing to type
template<typename Object, typename Target>
constexpr bool iterator_to = std::is_same_v<
	typename std::iterator_traits<Object>::value_type,
	Target>;

} // namespace Sudoku::Solvers_
