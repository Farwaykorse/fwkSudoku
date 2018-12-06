##====---- fwkSudoku                                                  ----====##
## Base configuration.
##
##====--------------------------------------------------------------------====##
cmake_minimum_required(VERSION 3.8...3.13)

project(fwk_Sudoku LANGUAGES CXX)

##====--------------------------------------------------------------------====##
# Configuration
##====--------------------------------------------------------------------====##
#option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
if(CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
  if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0)
  endif()
endif()

##====--------------------------------------------------------------------====##
# CMake Configuration Functions
##====--------------------------------------------------------------------====##
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/enable_project_warnings.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/set_precompiled_header.cmake)

##====--------------------------------------------------------------------====##
# The library: Sudoku
##====--------------------------------------------------------------------====##
# (INTERFACE libraries: header-only)
add_library(Sudoku INTERFACE)

# include by all consumers of Sudoku
target_include_directories(Sudoku
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sudoku>
)

#message("Supported features = ${CMAKE_CXX_COMPILE_FEATURES}")
target_compile_features(Sudoku
  INTERFACE
    cxx_std_17
)

target_compile_options(Sudoku
  INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:
	  /EHsc
	  /permissive-    # disable non-conforming constructs in VC++
	  /GR-            # disable RTTI (smaller image)
	>
)

# Dependencies
##====--------------------------------------------------------------------====##
find_path(GSL_INCLUDE_DIR gsl/gsl)
if(GSL_INCLUDE_DIR STREQUAL "GSL_INCLUDE_DIR-NOTFOUND")
  message(FATAL_ERROR "Could not find Guidelines Support Library.")
endif()
target_include_directories(Sudoku
  SYSTEM
    INTERFACE ${GSL_INCLUDE_DIR}
)

##====--------------------------------------------------------------------====##
# Testing Sudoku
##====--------------------------------------------------------------------====##
enable_testing()
add_subdirectory(SudokuTests)

##====--------------------------------------------------------------------====##
# Sub-projects
##====--------------------------------------------------------------------====##
add_subdirectory(Console)

