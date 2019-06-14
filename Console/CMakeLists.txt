##====---- fwkSudoku/Console/CMakeLists.txt                           ----====##
##
##====--------------------------------------------------------------------====##
cmake_minimum_required(VERSION 3.12...3.13)

project(SudokuConsole LANGUAGES CXX)

##====--------------------------------------------------------------------====##
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

##====--------------------------------------------------------------------====##
# Compiler Specific Configuration
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(console PRIVATE
    /EHsc
    /GR-         # disable RTTI (smaller image)
    /GS          # buffer security check
    /permissive- # disable non-conforming constructs in VC++
    $<$<CXX_COMPILER_ID:MSVC>:
      /Gm-       # prefer /MP
      /MP        # multiprocessor compilation (requires /Gm-)
      /sdl
    >
  )
  # Ninja-build only,
  # Remove conflicting flags set by CMake (MSVC warning D9025)
  string(REGEX REPLACE "(/GR|/W[0-4]) ?" ""
    CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}"
  )
endif()

##====--------------------------------------------------------------------====##
# Maximize Warnings-level per compiler
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(console PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4
      ${MSVC_Extra_Warnings}
    >
    $<$<CXX_COMPILER_ID:Clang>:
      -Weverything
      -Werror=return-type
      # disabled warnings
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
    >
    $<$<CXX_COMPILER_ID:Intel>:
      /W5
      /Qdiag-disable:304,981
    >
  )
  set_source_files_compile_options(
    precompiled.cpp
    OPTIONS
      $<$<CXX_COMPILER_ID:MSVC>:
        /wd4668 # gsl_byte
        /wd4774 # stl
        /wd5026 # system_error
      >
  )
else()
  target_compile_options(console PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
      -Weverything
      -Werror=return-type
      # disabled warnings
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-weak-vtables   # exceptions
    >
    $<$<CXX_COMPILER_ID:Clang>:
      $<$<VERSION_EQUAL:$<CXX_COMPILER_VERSION>,5.0>:
        -Wno-missing-braces
      >
    >
    $<$<CXX_COMPILER_ID:AppleClang>:
      $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,10>:
        -Wno-missing-braces
      >
    >
    $<$<CXX_COMPILER_ID:GNU>:
      -Wall
      -Wcast-align        # potential performance issue
      $<$<NOT:$<VERSION_LESS:$<CXX_COMPILER_VERSION>,8>>:
        -Wcast-align=strict # since 8.0; increase alignment (i.e. char* to int*)
        -Wconversion      # since 8.0; potential data-loss
      >
      -Wdouble-promotion  # since 4.6; float implicit to double
      -Wduplicated-branches # since 7.0; identical if-else branches
      -Wduplicated-cond   # since 6.0; duplicated if-else conditions
      -Wextra
      -Wformat=2
      -Wformat-overflow=2 # since 7.0
      -Wformat-signedness # since 5.0; sign change on input of format functions
      -Wformat-truncation=2 # since 7.0
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
    Console.cpp
    OPTIONS
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        -Wno-padded
      >
  )
endif()

##====--------------------------------------------------------------------====##
# Custom settings for precompiled headers
set_precompiled_header(console "precompiled.h" "precompiled.cpp")

