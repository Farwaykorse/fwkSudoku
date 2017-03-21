// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

	/////////////////////
	/////	NOTES	/////
	/////////////////////
//	Precomiled headers
//	Configurations:
//	- Project:
//		Configuration Properties > C/C++ > Precompiled Headers
//		Precompiled header:	Use (/Yu)
//	- stdafx.cpp:
//		Configuration Properties > C/C++ > Precompiled Headers
//		Precompiled header:	Create (/Yc)
//////////////////////////

#pragma once

#include "targetver.h"

// console
#include <stdio.h>
#include <tchar.h>

#define _setargv 0	// disable commandline arguments
#define _setenvp 0	// disable environmental table

// TODO: reference additional headers your program requires here
#include <iostream>
#include <chrono>
#include <algorithm>
