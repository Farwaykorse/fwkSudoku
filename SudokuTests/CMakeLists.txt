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

target_compile_definitions(SudokuTests
  PRIVATE
    fwkUnitTest
)

set_target_properties(SudokuTests
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

##====--------------------------------------------------------------------====##
# Configuration
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(SudokuTests PRIVATE
    /EHsc
    /MP               # multiprocessor compilation (requires /Gm-)
    /permissive-      # disable non-conforming constructs in VC++
    $<$<CXX_COMPILER_ID:MSVC>:
      /Gm-            # prefer /MP
      /GR-            # disable RTTI (smaller image)
      /GS             # buffer security check
      /sdl
    >
    $<$<CXX_COMPILER_ID:Clang>:
      -fno-strict-aliasing
    >
  )
else()
  target_compile_options(SudokuTests PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
    >
  )
endif()

set_precompiled_header(SudokuTests "precompiled.h" "precompiled.cpp")

##====--------------------------------------------------------------------====##
# Warning Configuration
if(MSVC) # MSVC or simulating MSVC
  enable_project_warnings(SudokuTests)
  target_compile_options(SudokuTests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4
      /wd4365 # stl
      /wd4623 # gtest
      /wd4668 # gsl_byte & gtest
      /wd4715
      /wd4774 # stl
      /wd5026 # system_error & gtest
    >
    $<$<CXX_COMPILER_ID:Clang>:
      -Weverything
      -Werror=return-type
      # disabled warnings
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-covered-switch-default
      -Wno-global-constructors
      -Wno-language-extension-token
      -Wno-missing-noreturn
      -Wno-shift-sign-overflow
      -Wno-undef
      -Wno-unevaluated-expression
      -Wno-unused-comparison
      -Wno-used-but-marked-unused
      -Wno-zero-as-null-pointer-constant
    >
    $<$<CXX_COMPILER_ID:Intel>:
      /W5
      /Qdiag-disable:304,981
    >
  )
else()

endif()

##====--------------------------------------------------------------------====##
#gtest_add_tests(TARGET SudokuTests)
gtest_discover_tests(SudokuTests)

