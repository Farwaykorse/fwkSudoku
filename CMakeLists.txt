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
find_package(Microsoft.GSL CONFIG REQUIRED)
message(STATUS "Found the Guidelines Support Library: ${Microsoft.GSL_DIR}")
target_link_libraries(Sudoku INTERFACE Microsoft.GSL::GSL)

##====--------------------------------------------------------------------====##
# Testing Sudoku
##====--------------------------------------------------------------------====##
enable_testing()
add_subdirectory(SudokuTests)

##====--------------------------------------------------------------------====##
# Optional Sub-Projects using Sudoku
##====--------------------------------------------------------------------====##
add_subdirectory(Console)

