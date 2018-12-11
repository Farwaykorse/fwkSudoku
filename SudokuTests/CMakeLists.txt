##====---- fwkSudoku/SudokuTests/CMakeLists.txt                       ----====##
## Tests for library Sudoku.
##
##====--------------------------------------------------------------------====##
cmake_minimum_required(VERSION 3.10.3...3.13)

project(fwkSudokuTests LANGUAGES CXX)

include(GoogleTest)

find_package(GTest MODULE REQUIRED)
message("GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")
message("GTEST_LIBRARIES = ${GTEST_LIBRARIES}")

##====--------------------------------------------------------------------====##
add_executable(SudokuTests)

target_sources(SudokuTests
  PRIVATE
    print_Options.h
#    SudokuTests.cpp
  PRIVATE
    Board.cpp
    Board_Iterators.cpp
    Board_Section_iterator.cpp
    Board_Section_traits.cpp
    exceptions.cpp
    Location.cpp
    Options.cpp
    Size.cpp
    Solver.cpp
    Solvers_find.cpp
    Solvers_remove_option.cpp
    Solvers_set_option.cpp
    traits.cpp
    Transpose.cpp
    Value.cpp
)

target_link_libraries(SudokuTests
  PRIVATE
    Sudoku
    GTest::GTest
    GTest::Main
)

set_target_properties(SudokuTests
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

##====--------------------------------------------------------------------====##
set_precompiled_header(SudokuTests "precompiled.h" "precompiled.cpp")

enable_project_warnings(SudokuTests
  DISABLE
    4365 # stl
    4623 # gtest
    4668 # gsl_byte & gtest
    4774 # stl
    5026 # system_error & gtest
)

##====--------------------------------------------------------------------====##
#gtest_add_tests(TARGET SudokuTests)
gtest_discover_tests(SudokuTests)

