cmake_minimum_required(VERSION 3.0...3.13)

project(SudokuConsole LANGUAGES CXX)

add_executable(console
  Console.cpp
)

target_link_libraries(console
  Sudoku
)

set_target_properties(console
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


