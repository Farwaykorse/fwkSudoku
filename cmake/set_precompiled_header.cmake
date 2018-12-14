##====---- set_precompiled_header.cmake                               ----====##
## Perform the precompiled header configuration.
## MSVC only.
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

  set_target_properties(${Target}
    PROPERTIES
      COMPILE_OPTIONS "/Fp${PCH_FILE}"
  )
endif(MSVC)
endfunction(set_precompiled_header)
