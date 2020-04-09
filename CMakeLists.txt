##====---- fwkSudoku                                                  ----====##
## Base configuration.
##
##====--------------------------------------------------------------------====##
cmake_minimum_required(VERSION 3.12...3.17)
message(STATUS "CMake version: ${CMAKE_VERSION}")

project(fwk_Sudoku LANGUAGES CXX)

##====--------------------------------------------------------------------====##
# Configuration
##====--------------------------------------------------------------------====##
option(CODE_COVERAGE "Enable coverage reporting" OFF)

##====--------------------------------------------------------------------====##
# External CMake definitions
##====--------------------------------------------------------------------====##
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

##====--------------------------------------------------------------------====##
# The library: Sudoku
##====--------------------------------------------------------------------====##
# (INTERFACE libraries: header-only)
add_library(Sudoku INTERFACE)
add_library(fwk::Sudoku ALIAS Sudoku)

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

