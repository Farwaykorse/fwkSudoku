//===--- SudokuTests/precompiled.h                                      ---===//
//
// - Compiled from precompiled.cpp
// - Included in project by compiler configuration
//		Clang:	-include "precompiled.h"
//		VS:		/FI"precompiled.h"
//
// file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//===----------------------------------------------------------------------===//
#pragma once

// Common includes
#include <bitset>
#include <vector>

#include <numeric>
#include <random>
#include <type_traits>

#include <gtest/gtest.h> // Main library

#include <cstdio> // console requirements

// precompiled dependencies specified for Sudoku.lib
#include <Sudoku/precompiled_basic.h>
