cmake_minimum_required(VERSION 3.10.3...3.13)

project(fwkSudokuTests LANGUAGES CXX)

include(GoogleTest)

find_package(GTest MODULE REQUIRED)
message("GTEST_INCLUDE_DIRS = ${GTEST_INCLUDE_DIRS}")
message("GTEST_LIBRARIES = ${GTEST_LIBRARIES}")

add_executable(SudokuTests
  print_options.h
#  SudokuTests.cpp
  Board.cpp
  Board_iterators.cpp
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
#    ${GTEST_LIBRARIES}
)

set_target_properties(SudokuTests
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

#gtest_add_tests(TARGET SudokuTests)
gtest_discover_tests(SudokuTests)
