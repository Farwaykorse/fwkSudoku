##====---- subproject_functions.cmake                                 ----====##
## subproject_cmake_minimum_required()
## Set cmake_minimum_required when not specified or warn on reduction of minimum
## version (configuration error).
## To document and verify in calling configurations.
## This is preferable over the use of `cmake_minimum_required` which overwrites
## all manually set policies.
##
## ````
## Usage:
## ````cmake
## subproject_cmake_minimum_required(<MIN_VERSION>)
## ````
## `<MIN_VERSION>` can be a single version number, like `3.12`, or a
## `<min>...<max>` range as in `cmake_minimum_required`, like: `3.12...3.14`.
##
##====--------------------------------------------------------------------====##
include_guard()

macro(subproject_cmake_minimum_required MIN_VERSION)
  if(NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION)
    cmake_minimum_required(VERSION ${MIN_VERSION})
  elseif(CMAKE_MINIMUM_REQUIRED_VERSION VERSION_LESS ${MIN_VERSION})
    message(AUTHOR_WARNING
      "The minimum required CMake version in the top level configuration\
      (${CMAKE_MINIMUM_REQUIRED_VERSION}) is less than required\
      (${MIN_VERSION}) for the project (@ ${PROJECT_SOURCE_DIR})"
    )
  endif()
endmacro(subproject_cmake_minimum_required)
