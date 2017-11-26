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

#include "targetver.h"

// console
#include <stdio.h>
#include <tchar.h>

#define _setargv 0 // disable commandline arguments
#define _setenvp 0 // disable environmental table

// reference additional headers your program requires here
#include <string>
#include <vector>
#include <iostream>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>

// precompiled dependencies used in Sudoku.lib
#include "../Sudoku/precompiled.h"
