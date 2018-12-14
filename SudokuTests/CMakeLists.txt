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
set_precompiled_header(SudokuTests "precompiled.h" "precompiled.cpp")

if(MSVC) # MSVC or simulating MSVC
  target_compile_options(SudokuTests PRIVATE
    /EHsc
    /GR-              # disable RTTI (smaller image)
    /GS               # buffer security check
    /permissive-      # disable non-conforming constructs in VC++
    $<$<CXX_COMPILER_ID:MSVC>:
      /Gm-            # prefer /MP
      /MP             # multiprocessor compilation (requires /Gm-)
      /sdl
      # warnings
      /W4
      ${MSVC_Extra_Warnings}
      /wd4251 # gtest (non dll interface class used as base for dll-interface)
      /wd4365 # stl
      /wd4623 # gtest
      /wd4668 # gsl_byte & gtest
      /wd4774 # stl
      /wd5026 # system_error & gtest
    >
    $<$<CXX_COMPILER_ID:Clang>:
      -fno-strict-aliasing
      # warnings
      -Weverything
      -Werror=return-type
      # disabled warnings
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-covered-switch-default
      -Wno-deprecated-declarations       # gtest: fopen, strncpy, chdir, ...
      -Wno-global-constructors
      -Wno-language-extension-token
      -Wno-missing-noreturn
      -Wno-shift-sign-overflow
      -Wno-undef
      -Wno-unevaluated-expression        # Board.cpp & Board_Iterators.cpp
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
  target_compile_options(SudokuTests PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
      -Wno-padded
      -Wno-unevaluated-expression        # Board.cpp & Board_Iterators.cpp
      -Wno-weak-vtables
    >
  )
endif()

##====--------------------------------------------------------------------====##
#gtest_add_tests(TARGET SudokuTests)
gtest_discover_tests(SudokuTests)

