//===--- Sudoku/Solver.fwd.h                                            ---===//
//
//	Forward declaration header for Solver.h
//===----------------------------------------------------------------------===//
#pragma once

namespace Sudoku
{
//===-- Solver.h ----------------------------------------------------------===//
template<int N, typename Options = Options<elem_size<N>>>
int single_option(Board<Options, N>&, Location<N>);
template<int N, typename Options = Options<elem_size<N>>>
int single_option(Board<Options, N>&, Location<N>, Value);
template<int N, typename Options = Options<elem_size<N>>>
int dual_option(Board<Options, N>&, Location<N>);
template<int N, typename Options = Options<elem_size<N>>>
int multi_option(Board<Options, N>&, Location<N>, size_t = 0);

template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int unique_in_section(Board<Options, N>&, SectionT);
template<int N, typename Options = Options<elem_size<N>>, typename SectionT>
int section_exclusive(Board<Options, N>&, SectionT);


} // namespace Sudoku
