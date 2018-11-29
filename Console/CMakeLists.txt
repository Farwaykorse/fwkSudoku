cmake_minimum_required(VERSION 3.11.4...3.13)

project(SudokuConsole LANGUAGES CXX)

add_executable(console
  Console.cpp
)

target_link_libraries(console
  PRIVATE
    Sudoku
)

set_target_properties(console
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS OFF
)

# custom settings for precompiled headers
set_precompiled_header(console "precompiled.h" "precompiled.cpp")

enable_project_warnings(console
  DISABLE
    4668
    4774
    5026
)
