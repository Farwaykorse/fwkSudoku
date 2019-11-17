##====---- set_precompiled_header.cmake                               ----====##
## Perform the precompiled header configuration.
##
## Usage:
## ````cmake
## set_precompiled_header(
##   <target>
##   <HeaderFile> # string i.e. precompiled.h
##   <SourceFile> # string i.e. precompiled.cpp
## )
## ````
##====--------------------------------------------------------------------====##
include_guard()
include(add_to_source_file_properties)

function(set_precompiled_header Target HeaderFile SourceFile)
if(MSVC)
  cmake_minimum_required(VERSION 3.12...3.15) # add_to_source_file_properties()
  set(PCH_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pch")

  target_sources(${Target}
    PRIVATE
      ${SourceFile}
  )
  get_property(Files TARGET ${Target} PROPERTY SOURCES)

  list(FILTER Files INCLUDE REGEX ".*\.cpp$")

  # Workaround Clang's /\-mixing and resulting warning (-Wmicrosoft-include).
  # Lets CMake set the full path.
  # Confirmed for clang 7.0 and 8.0.0
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
      ${CMAKE_CXX_COMPILER_VERSION} EQUAL 7 OR
      ${CMAKE_CXX_COMPILER_VERSION} EQUAL 8
  )
    set(HeaderFile "${CMAKE_CURRENT_SOURCE_DIR}/${HeaderFile}")
  endif()
  # /Workaround

  # Create *.pch
  list(FIND Files ${SourceFile} loc)
  list(REMOVE_AT Files ${loc})
  set_source_files_compile_options(${SourceFile}
    OPTIONS
      /Yc${HeaderFile} # Create precompiled header
  )
  add_to_source_file_properties(${SourceFile}
    PROPERTIES
      OBJECT_OUTPUTS ${PCH_FILE}
  )

  # Configure all other files to use the precompiled header
  set_source_files_compile_options(${Files}
    OPTIONS
      /FI${HeaderFile} # Force-include, required for Clang-cl
      /Yu${HeaderFile} # Use precompiled header
  )
  add_to_source_file_properties(${Files}
    PROPERTIES
      OBJECT_DEPENDS ${PCH_FILE}
  )

  target_compile_options(${Target}
    PRIVATE
      /Fp${PCH_FILE}
  )

endif(MSVC)
endfunction(set_precompiled_header)
