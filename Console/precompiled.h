//===--	Console/precompiled.h											--===//
//
// - Compiled from precompiled.cpp
// - Included in project by compiler configuration
//		Clang:	-include "precompiled.h"
//		VS:		/FI"precompiled.h"
//
// file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//===---------------------------------------------------------------------===//
#pragma once

#if defined(_WIN64)
#include "targetver.h"
#endif // _WIN64

// console
#include <cstdio>

#define _setargv 0 // disable command-line arguments
#define _setenvp 0 // disable environmental table

// reference additional headers your program requires here
#include <string>
#include <vector>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

// dependencies used in Sudoku
#include <Sudoku/precompiled_basic.h>
