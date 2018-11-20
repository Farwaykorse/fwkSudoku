//===--- Sudoku/precompiled_basic.h                                     ---===//
//
// Lists the standard system includes consumed by this library,
// or project specific include files that are used frequently, but
// are changed infrequently.
//
// - To be used as part of a precompiled header configuration for a project
//   consuming this library.
// - Where frequent changes to the Sudoku library code are expected.
// - Not required when the library itself is added to a precompiled header.
//
//===----------------------------------------------------------------------===//
#pragma once

// STL containers
#include <array>
#include <bitset>
#include <vector>
// STL
#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

// Libraries
#include <gsl/gsl>
