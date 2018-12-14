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

function(set_precompiled_header Target HeaderFile SourceFile)
if(MSVC)
  set(PCH_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pch")

  target_sources(${Target}
    PRIVATE
      ${SourceFile}
  )
  get_property(Files TARGET ${Target} PROPERTY SOURCES)

  # Workaround Clang's /\-mixing and resulting warning (-Wmicrosoft-include).
  # Lets CMake set the full path.
  # Confirmed for clang 7.0
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND
      ${CMAKE_CXX_COMPILER_VERSION} EQUAL 7
  )
    set(HeaderFile "${CMAKE_CURRENT_SOURCE_DIR}/${HeaderFile}")
  endif()
  # /Workaround

  foreach(file_name ${Files})
    if(NOT ${file_name} MATCHES ".*\.cpp$")
      continue()
    endif()
    if(${file_name} STREQUAL ${SourceFile})
      set_source_files_properties(${file_name}
        PROPERTIES
          COMPILE_OPTIONS "/Yc${HeaderFile}"
          OBJECT_OUTPUTS "${PCH_FILE}"
      )
    else()
      set_source_files_properties(${file_name}
        PROPERTIES
          COMPILE_OPTIONS "/FI${HeaderFile};/Yu${HeaderFile}"
          OBJECT_DEPENDS "${PCH_FILE}"
      )
    endif()
  endforeach(file_name)

  target_compile_options(${Target}
    PRIVATE
      /Fp${PCH_FILE}
  )

endif(MSVC)
endfunction(set_precompiled_header)
