##====---- fwkSudoku/Console/CMakeLists.txt                           ----====##
##
##====--------------------------------------------------------------------====##
cmake_minimum_required(VERSION 3.11.4...3.13)

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
# custom settings for precompiled headers
set_precompiled_header(console "precompiled.h" "precompiled.cpp")

##====--------------------------------------------------------------------====##
# Configuration
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(console PRIVATE
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
      /wd4668 # gsl_byte
      /wd4774 # stl
      /wd5026 # system_error
    >
    $<$<CXX_COMPILER_ID:Clang>:
      -fno-strict-aliasing
      # warnings
      -Weverything
      -Werror=return-type
      # disabled warnings
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-undef                 # ms-gsl
    >
    $<$<CXX_COMPILER_ID:Intel>:
      /W5
      /Qdiag-disable:304,981
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
    >
  )
endif()
