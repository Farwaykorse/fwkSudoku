##====---- fwkSudoku                                                  ----====##
## Base configuration.
##
##====--------------------------------------------------------------------====##
cmake_minimum_required(VERSION 3.12...3.13)

project(fwk_Sudoku LANGUAGES CXX)

##====--------------------------------------------------------------------====##
# Configuration
##====--------------------------------------------------------------------====##
option(CODE_COVERAGE "Enable coverage reporting" OFF)

##====--------------------------------------------------------------------====##
# External CMake definitions
##====--------------------------------------------------------------------====##
# Functions
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_to_source_file_properties.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/set_precompiled_header.cmake)
# Properties
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/msvc_warnings_list.cmake)

##====--------------------------------------------------------------------====##
# The library: Sudoku
##====--------------------------------------------------------------------====##
# (INTERFACE libraries: header-only)
add_library(Sudoku INTERFACE)

target_include_directories(Sudoku
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sudoku>
)

target_compile_features(Sudoku
  INTERFACE
    cxx_std_17
)

# Dependencies Sudoku
##====--------------------------------------------------------------------====##
find_path(GSL_INCLUDE_DIR gsl/gsl)
if(GSL_INCLUDE_DIR)
  message(STATUS "Found the Guidelines Support Library: ${GSL_INCLUDE_DIR}")
else()
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
# Optional Sub-Projects using Sudoku
##====--------------------------------------------------------------------====##
add_subdirectory(Console)

