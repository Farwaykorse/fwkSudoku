cmake_minimum_required(VERSION 3.8...3.13)

project(fwk_Sudoku LANGUAGES CXX)

#message("Supported features = ${CMAKE_CXX_COMPILE_FEATURES}")

# Configuration
#option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
if(CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
  if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0)
  endif()
endif()

# Link this 'library' to configure warnings
add_library(project_warnings INTERFACE)

if(MSVC)
  target_compile_options(project_warnings
    INTERFACE
	  /W4)
else()
  target_compile_options(project_warnings
    INTERFACE
	  -Wall
	  -Wextra
	  -Wshadow
  )
endif()



# Shared dependencies
find_path(GSL_INCLUDE_DIR gsl/gsl)
if(GSL_INCLUDE_DIR STREQUAL "GSL_INCLUDE_DIR-NOTFOUND")
  message(FATAL_ERROR "Could not find Guidelines Support Library.")
endif()
# include by all
include_directories(SYSTEM ${GSL_INCLUDE_DIR})

# The library: Sudoku
# (INTERFACE libraries: header-only)
add_library(Sudoku INTERFACE)

# include by all consumers of Sudoku
target_include_directories(Sudoku
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Sudoku>
)
target_compile_features(Sudoku
  INTERFACE
    cxx_std_17
)

target_compile_options(Sudoku
  INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:
	  /EHsc
	  /permissive-
	>
)


# Console sub-project
add_subdirectory(Console)

# Gtest
enable_testing()
add_subdirectory(SudokuTests)
