##====---- add_to_source_file_properties.cmake                        ----====##
## Collection of functions to add source file specific settings.
##
## Specialized functions (preferred):
## - set_source_files_compile_definitions(<file-list> DEFINITIONS <item-list>)
##   Add definitions to use when compiler the file(s).
## - set_source_files_compile_options(<file-list> OPTIONS <item-list>)
##   Add any flag to the compiler.
## General functions, free to set any property:
## - add_to_source_file_properties(<file-list> PROPERTIES <property> <item-list>)
##
## Offers behaviour like the equivalent `target_*` functions.
## Implemented on top of `set_source_files_properties()`.
## 
## + All list syntaxes work, "a;b;c", or spaces and line-breaks, etc.
## + Append items to the property.
## = Can apply simultaneously to multiple files.
## - Limited to a single property per call.
## = Arguments may use "generator expressions" (with the syntax `$<...>`).
##
##====--------------------------------------------------------------------====##
## Usage:
## ````cmake
## set_source_files_compile_definitions(<file-list> DEFINITIONS <options-list>)
## set_source_files_compile_definitions(
##   <file_1>
##   DEFINITIONS
##     <def1> # string e.g. NDEBUG or -DNDEBUG or /DWIN32 (-D or /D is optional)
##     <def2> # string or ;-separated-list
## )
## ````

## ````cmake
## set_source_files_compile_options(<file-list> PROPERTIES <option-list>)
## set_source_files_compile_options(
##   <file_1>
##   <file_2>
##   PROPERTIES
##     <Options1> # string i.e. -Weverything
##     <Options2>
## )
## ````
##
## ````cmake
## add_to_source_file_properties(
##   <file_1>
##   <file_2>
##   PROPERTIES
##     <PROPERTY>  # string i.e. COMPILE_OPTIONS
##       <Options> # i.e. true or -Weverything, ...
## )
## ````
##====--------------------------------------------------------------------====##
include_guard()


function(set_source_files_compile_definitions)
  cmake_minimum_required(VERSION 3.12...3.13) # list(SUBLIST ...
  list(FIND ARGN DEFINITIONS prop_loc)
  if(${prop_loc} EQUAL -1)
    message(SEND_ERROR "Missing keyword DEFINITIONS.")
    return()
  elseif(${prop_loc} LESS 1)
    message(SEND_ERROR "Expecting file names before DEFINITIONS.")
  endif()

  list(SUBLIST ARGN 0 ${prop_loc} files)
  list(SUBLIST ARGN ${prop_loc} -1 definitions)
  list(REMOVE_AT definitions 0) # DEFINITIONS
  list(TRANSFORM definitions STRIP)
  list(TRANSFORM definitions REPLACE "^[-/]D[ ]?(.*)" "\\1")
  list(FILTER definitions INCLUDE REGEX ".+") # Remove empty
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    list(TRANSFORM definitions PREPEND "/D")
  else()
    list(TRANSFORM definitions PREPEND "-D")
  endif()
  list(APPEND new_list COMPILE_DEFINITIONS ${prop_loc} ${files} ${definitions})
  __internal_add_source_file_properties(${new_list})

endfunction(set_source_files_compile_definitions)


##====--------------------------------------------------------------------====##
function(set_source_files_compile_options)
  list(FIND ARGN OPTIONS prop_loc)
  if(${prop_loc} EQUAL -1)
    message(SEND_ERROR "Missing keyword OPTIONS.")
    return()
  elseif(${prop_loc} LESS 1)
    message(SEND_ERROR "Expecting file names before OPTIONS.")
  endif()

  list(REMOVE_AT ARGN ${prop_loc})
  list(APPEND new_list COMPILE_OPTIONS ${prop_loc} ${ARGN})
  __internal_add_source_file_properties(${new_list})

endfunction(set_source_files_compile_options)


##====--------------------------------------------------------------------====##
# Add any property to a source file.
function(add_to_source_file_properties)
  cmake_minimum_required(VERSION 3.12...3.13) # list(SUBLIST ...
  list(FIND ARGN PROPERTIES prop_loc)
  if(${prop_loc} EQUAL -1)
    message(SEND_ERROR "Missing keyword PROPERTIES.")
    return()
  endif()

  list(SUBLIST ARGN 0 ${prop_loc} list_files)
  list(SUBLIST ARGN ${prop_loc} -1 list_items)
  list(LENGTH list_items input_length)
  if(${input_length} LESS 3)
    message(SEND_ERROR "Invalid input.")
    return()
  endif()
  list(GET list_items 1 property_name)
  list(REMOVE_AT list_items 0 1) # PROPERTIES <property_name>

  list(APPEND new_list ${property_name} ${prop_loc} ${list_files} ${list_items})
  __internal_add_source_file_properties(${new_list})

endfunction(add_to_source_file_properties)


##====--------------------------------------------------------------------====##
# Internal shared implementation.
function(__internal_add_source_file_properties property_name num_files)
  cmake_minimum_required(VERSION 3.12...3.13) # list(SUBLIST ...
  list(SUBLIST ARGN 0 ${num_files} list_files)
  list(SUBLIST ARGN ${num_files} -1 list_items)

  list(LENGTH list_items input_length)
  if(${input_length} LESS 1)
    message(SEND_ERROR "Invalid input.")
    return()
  endif()

  ##------------------------------------temporary
  #message("Files:    ${list_files}")
  #message("Property: ${property_name}")
  #message("Items:    ${list_items}")
  ##------------------------------------temporary

  foreach(file_name ${list_files})
    # Checks existence of file. To catch typos.
    get_source_file_property(unused ${file_name} LOCATION)

    get_source_file_property(list_options ${file_name} ${property_name})
    list(REMOVE_ITEM list_options NOTFOUND)

    list(APPEND list_options ${list_items})
    set_source_files_properties(${file_name}
      PROPERTIES
       ${property_name} "${list_options}"
    )
  endforeach(file_name)

endfunction(__internal_add_source_file_properties)

