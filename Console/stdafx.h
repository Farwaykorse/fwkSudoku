// -----------------------------------------------------------------------------
// stdafx.h : header for Precompiled Header
// -----------------------------------------------------------------------------
// - Compiled from stdafx.cpp
// - Included in project by compiler configuration
//		Clang:	-include "stdafx.h"
//		VS:		/FI"stdafx.h"
//
// file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
//////////////////////////
#pragma once

#include "targetver.h"

// console
#include <stdio.h>
#include <tchar.h>

#define _setargv 0	// disable commandline arguments
#define _setenvp 0	// disable environmental table

// reference additional headers your program requires here
#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iomanip>
#include <sstream>

//#include "../Sudoku/Board.h"
//#include "../Sudoku/Location.h"
