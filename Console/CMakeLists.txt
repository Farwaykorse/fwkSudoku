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
# Configuration
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(console PRIVATE
    /EHsc
    /permissive-      # disable non-conforming constructs in VC++
    $<$<CXX_COMPILER_ID:MSVC>:
      /Gm-            # prefer /MP
      /GR-            # disable RTTI (smaller image)
      /GS             # buffer security check
      /MP             # multiprocessor compilation (requires /Gm-)
      /sdl
    >
    $<$<CXX_COMPILER_ID:Clang>:
      -fno-strict-aliasing
    >
  )
else()

endif()

##====--------------------------------------------------------------------====##
# custom settings for precompiled headers
set_precompiled_header(console "precompiled.h" "precompiled.cpp")


##====--------------------------------------------------------------------====##
# Warning Configuration
if(MSVC) # MSVC or simulating MSVC
  enable_project_warnings(console)
  target_compile_options(console PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4
      /wd4668 # gsl_byte
      /wd4774 # stl
      /wd5026 # system_error
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
