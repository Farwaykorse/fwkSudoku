##====---- enable_project_warnings.cmake                              ----====##
## Enable as many warnings as possible.
##
## Usage:
## ````cmake
## include(cmake/enable_project_warnings.cmake)
## enable_project_warnings(<target>
##   # disable warnings:
##   1234
##   5999
## )
## ````
##====--------------------------------------------------------------------====##

function(enable_project_warnings Target)
if(${ARGC} GREATER 0)
  set(Disable_List ${ARGN})
  if(${ARGV1} STREQUAL "DISABLE")
    list(REMOVE_AT Disable_List 0)
  else()
    message(AUTHOR_WARNING "Possible options are: DISABLE")
  endif()
endif()

set(lib_name "${Target}_warnings")

add_library(${lib_name} INTERFACE)

if(MSVC)
  set(Warning_List
    # bugged:
    /wd4715 # warns when switch goes over all cases in an enum
    # extra warnings not part of /W4
    /w44062 # in switch: enumerator not handled & no default
    /w34191 # conversion unsafe
    /w44242 # conversion possible loss of data, identifier
    /w44244 # conversion possible loss of data, conversion
    /w44254 # conversion possible loss of data, operator
    /w44263 # member function doesn't override any base class virtual function
    /w44264 # member function is hidden, no override for base class virtual
    /w34265 # no virtual destructor
    /w44266 # function is hidden, no override for base class virtual function
    /w34287 # unsigned/negative constant mismatch
    /we4289 # non-standard extension: loop control variable used outside loop
    /w44296 # expression is always false
    /w14355 # used in base member initializer list
    /w44365 # conversion: signed/unsigned mismatch
    /w14388 # signed/unsigned mismatch
    /w14426 # optimization flags changed after including header
    /w44437 # dynamic_cast from virtual base could fail in some contexts
    /w34444 # '__unaligned' is not implemented in this context
    /w44464 # relative path contains '..'
    /w14545 # expression before comma evaluates to function missing argument
    /w14546 # function call before comma, missing argument list
    /w14547 # operator before comma has no effect, expected side-effect
    /w14548 # expression before comma has no effect, expected side-effect
    /w14549 # operator before comma has no effect
    /w14555 # expression has no effect, expected side-effect
    /w44574 # defined to be '0'; suggesting use of #if
    /w34598 # header in precompiled header does not match current compilation
    /w34599 # command-line argument does not match precompiled header
    /w14605 # '/Dmacro' on command-line, not specified for precompiled header
    /w34619 # pragma warning number does not exist
    /w44623 # implicitly deleted default constructor (follows base class)
    #/w44625 # copy constructor could not be generated, base inaccessible
    # (warns on class containing a std::unique_ptr)
    #/w44626 # assignment operator could not be generated, base inaccessible
    # (warns on all classes with a `const` data-member)
    /w34640 # construction of local static object is not thread-safe
    /w44654 # Code before include of precompiled header line will be ignored.
    /w44668 # not defined pre-processor macro, replacing with '0'
    /w34686 # 'user-defined type': change in return calling convention
    /w34738 # storing 32-bit float result in memory, possible performance loss
    /w44774 # format string expected in argument number, not a string literal
    # note: e.g. instead of printf(name); use printf("%s", name); because
    #   format specifiers in 'name' may pose a security issue
    # note: consider using constexpr specifier for named string literals
    /w14826 # conversion is sign-extended
    /we4861 # non-standard extension: compound member designator in offsetof
    /w44868 # left-to-right evaluation braced init-list may not be enforced
    /w14905 # wide string literal cast to 'LPSTR'
    /w14906 # string literal cast to 'LPWSTR'
    /w14928 # illegal copy-init.; multiple user-defined implicit conversions
    /w14946 # reinterpret_cast used between related classes
    /w44986 # exception specification does not match previous declaration
    /we4987 # non-standard extension: 'throw(...)'
    /w44988 # variable declared outside class/function scope
    /w15022 # multiple move constructors specified
    /w15023 # multiple move assignment operators specified
    /w45024 # move constructor was implicitly defined as deleted
    /w45025 # move assignment operator was implicitly defined as deleted
    /w45026 # move constructor was implicitly defined as deleted
    #/w45027 # move assignment operator was implicitly defined as deleted
    # (hits on all classes with a `const` data-member)
    /we5029 # non-standard extension: alignment attributes do not apply here
    /w45031 # #pragma warning(pop): likely mismatch
    /w45032 # detected #pragma warning(push), no corresponding warning(pop)
  )

  if(DEFINED Disable_List)
    foreach(item ${Disable_List})
      if(${item} MATCHES "^[0-9][0-9][0-9][0-9]$")
        list(FILTER Warning_List EXCLUDE REGEX "/w.${item}")
        list(APPEND Warning_List "/wd${item}")
      else()
        message(AUTHOR_WARNING "expecting MSVC warning code (9999)")
      endif()
    endforeach(item)
  endif(DEFINED Disable_List)

  list(REMOVE_DUPLICATES Warning_List)

  target_compile_options(${lib_name}
    INTERFACE
      /W4     # highest warning level /Wall triggers library warnings
      ${Warning_List}
  )
else()
  target_compile_options(${lib_name}
    INTERFACE
      -Wall
      -Wextra
      -Wshadow
  )
endif(MSVC)

target_link_libraries(${Target}
  PRIVATE
    ${lib_name}
)

endfunction(enable_project_warnings)
