//===--- Sudoku/Iterator_Utilities.h                                    ---===//
//
//	Helper-functions, to check iterator properties
//===----------------------------------------------------------------------===//
#pragma once

#include <iterator>
#include <type_traits>


namespace Sudoku::Utility_
{
// test iterator-type
template<typename Itr>
using Category_ = typename std::iterator_traits<Itr>::iterator_category;

// random iterator
template<typename Itr>
constexpr bool is_random =
	std::is_same_v<Category_<Itr>, std::random_access_iterator_tag>;
// bidirectional iterator or better
template<typename Itr>
constexpr bool is_bidir =
	std::is_same_v<Category_<Itr>, std::bidirectional_iterator_tag> ||
	is_random<Itr>;
// forward iterator or better
template<typename Itr>
constexpr bool is_forward =
	std::is_same_v<Category_<Itr>, std::forward_iterator_tag> || is_bidir<Itr>;
// input iterator or better
template<typename Itr>
constexpr bool is_input =
	std::is_same_v<Category_<Itr>, std::input_iterator_tag> || is_forward<Itr>;

// iterator pointing to type
template<typename ItrT, typename TargetT>
constexpr bool iterator_to =
	std::is_same_v<typename std::iterator_traits<ItrT>::value_type, TargetT>;

} // namespace Sudoku::Utility_
