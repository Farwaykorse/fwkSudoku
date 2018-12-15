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

##====--------------------------------------------------------------------====##
# General Configuration
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
# Compiler Specific Configuration
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
    >
    $<$<CXX_COMPILER_ID:Clang>:
      -fno-strict-aliasing
    >
  )
endif()

# Warnings
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(SudokuTests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4
      ${MSVC_Extra_Warnings}
      /wd4365 # stl
      /wd4623 # gtest
      /wd4668 # gsl_byte & gtest
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
      -Wno-unused-comparison
      -Wno-used-but-marked-unused
      -Wno-zero-as-null-pointer-constant
    >
    $<$<CXX_COMPILER_ID:Intel>:
      /W5
      /Qdiag-disable:304,981
    >
  )
  set_source_files_compile_options(
    Board.cpp
    Board_Iterators.cpp
    OPTIONS 
      $<$<CXX_COMPILER_ID:Clang>:-Wno-unevaluated-expression>
  )
  set_source_files_compile_options(
    precompiled.cpp
    OPTIONS
      $<$<CXX_COMPILER_ID:MSVC>:
        /wd4251 # gtest (needs to have dll-interface to be used by clients)
        /wd4275 # gtest (non dll interface class used as base for dll-interface)
      >
      $<$<CXX_COMPILER_ID:Clang>:
        -Wno-deprecated-declarations     # gtest: fopen, strncpy, chdir, ...
        -Wno-language-extension-token
        -Wno-missing-noreturn
        -Wno-shift-sign-overflow
        -Wno-undef
      >
  )
else()
  target_compile_options(SudokuTests PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
      -Weverything
      -Wno-c++98-c++11-c++14-compat
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-covered-switch-default
      -Wno-global-constructors
      -Wno-padded
      -Wno-used-but-marked-unused
      -Wno-weak-vtables                  # exceptions.h
    >
    $<$<CXX_COMPILER_ID:GNU>:
      -Wall
      -Wcast-align        # potential performance issue
      #-Wconversion        # potential data-loss
      -Wdouble-promotion  # since 4.6; float implicit to double
      -Wduplicated-branches # since 7.0; identical if-else branches
      -Wduplicated-cond   # since 6.0; duplicated if-else conditions
      -Wextra
      -Wformat=2
      -Wlogical-op        # suspicious uses of logical operators
      -Wmisleading-indentation # since 6.0
      -Wnull-dereference  # since 6.0; detected paths with erroneous or undefined behaviour
      -Wpedantic          # non-standard C++
      -Wshadow            # variable declaration shadows parent context
      #-Wsign-conversion   # since 4.3
      -Wuseless-cast      # since 4.8; casting to its own type
    >
  )
  set_source_files_compile_options(
    Board.cpp
    Board_Iterators.cpp
    OPTIONS 
      $<$<CXX_COMPILER_ID:Clang>:-Wno-unevaluated-expression>
  )
endif()

##====--------------------------------------------------------------------====##
# custom settings for precompiled headers
set_precompiled_header(SudokuTests "precompiled.h" "precompiled.cpp")

##====--------------------------------------------------------------------====##
#gtest_add_tests(TARGET SudokuTests)
gtest_discover_tests(SudokuTests)

