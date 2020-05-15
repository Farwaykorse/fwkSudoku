##====---- fwkSudoku/Console/CMakeLists.txt                           ----====##
##
##====--------------------------------------------------------------------====##
include(add_to_source_file_properties)
include(msvc_warnings_list)
include(set_precompiled_header)
include(subproject_functions)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
  list(APPEND CMAKE_MESSAGE_CONTEXT "console")
endif()

subproject_cmake_minimum_required(3.12)

project(SudokuConsole LANGUAGES CXX)

##====--------------------------------------------------------------------====##
add_executable(console
  Console.cpp
)

target_link_libraries(console
  PRIVATE
    fwk::Sudoku
)

set_target_properties(console
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED OFF # allow decay to C++17
    CXX_EXTENSIONS OFF
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>" # v3.15+
    VS_JUST_MY_CODE_DEBUGGING $<IF:$<CONFIG:Debug>,ON,OFF> # v3.15+
)

##====--------------------------------------------------------------------====##
# Compiler Specific Configuration
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(console PRIVATE
    /EHsc        # exception handling configuration
    /GR-         # disable RTTI (smaller image)
    /GS          # buffer security check
    /permissive- # disable non-conforming constructs in VC++
    /utf-8       # source files are UTF-8 encoded
    /Gy          # function level linking
    $<$<CONFIG:Release>:
      /Oi        # use intrinsics
    >
    $<$<CXX_COMPILER_ID:MSVC>:
      /Gm-       # prefer /MP
      /MP        # multiprocessor compilation (requires /Gm-)
      /sdl       # SDL checks and code generation
      $<$<CONFIG:Debug>:
        /ZI       # PDB for edit-and-continue
      >
      $<$<CONFIG:Release>:
        /guard:cf # control flow guard
        /GL       # whole program optimization
        /Zi       # separate PDB
      >
    >
    $<$<CXX_COMPILER_ID:Clang>:
      $<$<CONFIG:Debug>:
        /Z7       # enable debug information
      >
      $<$<CONFIG:Release>:
        $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,7.0>:
          /guard:cf # control flow guard
        >
      >
    >
  )
  # Ninja-build only,
  # Remove conflicting flags set by CMake (MSVC warning D9025)
  string(REGEX REPLACE "(/GR|/EHsc) ?" ""
    CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}"
  )
  string(REGEX REPLACE "/Z[7iI] ?" ""
    CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}"
  )
  if(CMAKE_VERSION VERSION_LESS 3.15) 
    string(REGEX REPLACE "/W[0-4] ?" ""
      CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}"
    )
    string(REGEX REPLACE "/MD" "/MT"
      CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}"
    )
  endif()

  # Linker Configuration
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13) 
    target_link_options(console PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Release>:
          /LTCG     # link-time code generation (goes with /GL)
          /OPT:REF  # eliminate unreferenced COMDATs
          /OPT:ICF  # identical COMDAT folding
        >
      >
    )
  else()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      string(APPEND LINK_FLAGS_RELEASE " /LTCG /OPT:REF /OPT:ICF")
    endif()
  endif()
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
      $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,7>:
        -Wno-missing-variable-declarations # warns on inline variables
      >
    >
    $<$<CXX_COMPILER_ID:Intel>:
      /W5
      /Qdiag-disable:304,981
    >
  )
  if(NOT DISABLE_PRECOMPILE_HEADERS)
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
      target_compile_options(console PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:
          $<$<CONFIG:Debug>:
            /wd4365 # stl xmemory
          >
          /wd4668 # gsl: multi_span (fixed upstream 2020-04-09)
          $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,1920>: # VS2017
            /wd4774 # stl string
            /wd5026 # system_error
          >
        >
      )
    else()
      set_source_files_compile_options(precompiled.cpp OPTIONS
        $<$<CXX_COMPILER_ID:MSVC>:
          $<$<CONFIG:Debug>:
            /wd4365 # stl
          >
          /wd4668 # gsl: multi_span (fixed upstream 2020-04-09)
          $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,1920>: # VS2017
            /wd4774 # stl string
            /wd5026 # system_error
          >
        >
      )
    endif()
  endif()
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
      $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,7>:
        -Wno-missing-variable-declarations # warns on inline variables
      >
      $<$<OR:$<VERSION_EQUAL:$<CXX_COMPILER_VERSION>,5.0>,$<VERSION_EQUAL:$<CXX_COMPILER_VERSION>,5.0.1>>:
        -Wno-missing-braces
      >
    >
    $<$<CXX_COMPILER_ID:AppleClang>:
      $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,10.0.1>:
        -Wno-missing-variable-declarations # warns on inline variables
      >
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
      $<$<NOT:$<VERSION_LESS:$<CXX_COMPILER_VERSION>,10>>:
        #-Wmismatched-tags # since 10; mixed usage of struct and class
      >
      -Wnull-dereference  # since 6.0; detected paths with erroneous or undefined behaviour
      -Wpedantic          # non-standard C++
      $<$<NOT:$<VERSION_LESS:$<CXX_COMPILER_VERSION>,10>>:
        -Wredundant-tags  # since 10; class- and enum-key can be eliminated
      >
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
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
  target_precompile_headers(console PRIVATE precompiled.h)
elseif(NOT DISABLE_PRECOMPILE_HEADERS)
  set_precompiled_header(console "precompiled.h" "precompiled.cpp")
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
  list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endif()

