##====---- fwkSudoku/SudokuTests/CMakeLists.txt                       ----====##
## Tests for library Sudoku.
##
##====--------------------------------------------------------------------====##
include(add_to_source_file_properties)
include(msvc_warnings_list)
include(set_precompiled_header)
include(subproject_functions)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
  list(APPEND CMAKE_MESSAGE_CONTEXT "SudokuTests")
endif()

subproject_cmake_minimum_required(3.12)

project(fwkSudokuTests LANGUAGES CXX)

include(GoogleTest)

find_package(GTest MODULE REQUIRED)

##====--------------------------------------------------------------------====##
add_executable(SudokuTests)

target_sources(SudokuTests
  PRIVATE
    print_Options.h
#    SudokuTests.cpp
  PRIVATE
    Board.cpp
    Board_Iterators.cpp
    Board_Section.cpp
    Board_Section_iterator.cpp
    Board_Section_traits.cpp
    Decision.cpp
    exceptions.cpp
    Location.cpp
    Normalize.cpp
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
    fwk::Sudoku
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
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED OFF # allow decay to C++17
    CXX_EXTENSIONS NO
    VS_JUST_MY_CODE_DEBUGGING $<IF:$<CONFIG:Debug>,ON,OFF> # v3.15+
)

##====--------------------------------------------------------------------====##
# Compiler Specific Configuration
if(MSVC) # MSVC or simulating MSVC
  target_compile_options(SudokuTests PRIVATE
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
  endif()

  # Linker Configuration
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13) 
    target_link_options(SudokuTests PRIVATE
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
  target_compile_options(SudokuTests PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:
      /W4
      ${MSVC_Extra_Warnings}
      /wd4623 # gtest
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
      -Wno-used-but-marked-unused
      -Wno-zero-as-null-pointer-constant
      $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,7.0>:
        -Wno-return-std-move-in-c++11
      >
      $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,7>:
        -Wno-missing-variable-declarations # warns on inline variables
      >
    >
    $<$<CXX_COMPILER_ID:Intel>:
      /W5
      /Qdiag-disable:304,981
    >
  )
  set_source_files_compile_options(
    Board.cpp
    Board_Iterators.cpp
    Board_Section_iterator.cpp
    OPTIONS 
      $<$<CXX_COMPILER_ID:Clang>:-Wno-unevaluated-expression >
  )
  if(DISABLE_PRECOMPILE_HEADERS OR CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
    target_compile_options(SudokuTests PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:
          /wd4365 # stl xmemory
        >
        /wd4668 # gtest undefined macros
      >
    )
  else()
    set_source_files_compile_options(precompiled.cpp OPTIONS
      $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:
          /wd4365 # stl
        >
        /wd4668 # gtest undefined macros
      >
      $<$<CXX_COMPILER_ID:Clang>:
        -Wno-deprecated # gtest
        -Wno-language-extension-token
        -Wno-missing-noreturn
        -Wno-shift-sign-overflow
        -Wno-undef
        -Wno-unused-parameter # gtest
      >
    )
  endif()
else()
  target_compile_options(SudokuTests PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
      -Weverything
      # disabled warnings
      -Wno-c++98-c++11-c++14-compat
      -Wno-c++98-compat
      -Wno-c++98-compat-pedantic
      -Wno-covered-switch-default
      -Wno-global-constructors
      -Wno-padded
      -Wno-used-but-marked-unused
      -Wno-weak-vtables           # exceptions.h
    >
    $<$<CXX_COMPILER_ID:Clang>:
      $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,7.0>:
        -Wno-return-std-move-in-c++11
      >
      $<$<VERSION_LESS:$<CXX_COMPILER_VERSION>,7>:
        -Wno-missing-variable-declarations # warns on inline variables
      >
      $<$<OR:$<VERSION_EQUAL:$<CXX_COMPILER_VERSION>,5.0>,$<VERSION_EQUAL:$<CXX_COMPILER_VERSION>,5.0.1>>:
        -Wno-missing-braces
      >
    >
    $<$<CXX_COMPILER_ID:AppleClang>:
      $<$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,10.0.1>:
        -Wno-return-std-move-in-c++11
      >
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
    Board_Section_iterator.cpp
    OPTIONS
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
        -Wno-unevaluated-expression
      >
  )
endif()

##====--------------------------------------------------------------------====##
if(CODE_COVERAGE)
  if(MSVC AND CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    if(CMAKE_CXX_FLAGS MATCHES "-m(32|64)")
      # Changed target architecture. Assuming cross-compilation.
      # No matching profile-rt library provided with the official pre-build
      # binaries for Windows.
      message(WARNING "No coverage support for cross-compilation.")
    elseif(DEFINED CMAKE_CXX_COMPILER AND CMAKE_CXX_COMPILER)
      string(REPLACE "/bin/clang-cl.exe" "" LLVM_ROOT "${CMAKE_CXX_COMPILER}")
      set(LLVM_COMPILER-RT_PATH
        # Assumed path relative to the compiler (valid for v7.0.1)
        "${LLVM_ROOT}/lib/clang/${CMAKE_CXX_COMPILER_VERSION}/lib/windows"
      )
      find_library(PROFILE_RT 
        NAMES
          clang_rt.profile-x86_64
          clang_rt.profile-i386
        HINTS ${LLVM_COMPILER-RT_PATH}
      )
    endif()
    if(DEFINED PROFILE_RT AND PROFILE_RT)
      message(STATUS "Found the llvm compiler-rt profile library:\n"
        "${PROFILE_RT}")
      message(STATUS "Compile with code coverage information.")
      target_compile_options(SudokuTests PRIVATE
        /O0
        --coverage
      )
      if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(SudokuTests PRIVATE
          /nodefaultlib:libcmt.lib # Warning LNK4098: conflict
        )
      else()
        target_link_libraries(SudokuTests PRIVATE -nodefaultlib:libcmt.lib)
      endif()
      target_link_libraries(SudokuTests PRIVATE
        ${PROFILE_RT}
      )
    else()
      message(WARNING "Could not find the llvm compiler-rt profile library.")
    endif()
  elseif(NOT MSVC AND
    CMAKE_CXX_COMPILER_ID STREQUAL GNU OR
    CMAKE_CXX_COMPILER_ID STREQUAL Clang OR
    CMAKE_CXX_COMPILER_ID STREQUAL AppleClang
  )
    message(STATUS "Compile with code coverage information.")
    target_compile_options(SudokuTests PRIVATE
      -O0        # no optimization
      -g         # add debug info
      --coverage
      $<$<CXX_COMPILER_ID:GNU>:
        $<$<NOT:$<VERSION_LESS:$<CXX_COMPILER_VERSION>,8>>:
          -fprofile-abs-path # absolute path names in the `.gcno` files
        >
      >
    )
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
      target_link_options(SudokuTests PRIVATE
        --coverage
      )
    else()
      target_link_libraries(SudokuTests PRIVATE
        --coverage
      )
    endif()
  else()
    message(WARNING "CODE_COVERAGE, not configured for current compiler.")
  endif()
endif(CODE_COVERAGE)

##====--------------------------------------------------------------------====##
# custom settings for precompiled headers
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
  target_precompile_headers(SudokuTests PRIVATE precompiled.h)
elseif(NOT DISABLE_PRECOMPILE_HEADERS)
  set_precompiled_header(SudokuTests "precompiled.h" "precompiled.cpp")
endif()

##====--------------------------------------------------------------------====##
gtest_discover_tests(SudokuTests)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
  list(POP_BACK CMAKE_MESSAGE_CONTEXT)
endif()

