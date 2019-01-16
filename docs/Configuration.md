<!-------------------------------------------------------------><a id="top"></a>
# Configuration #
<!----------------------------------------------------------------------------->
<!-- Description -->
This document tries to document the used compiler and project configuration
settings, related to C++ compilation.
Supplying the motivation and discussion and to keep track of any exceptions.
No historic data is kept (since it is part of the git log), unless a setting is
considered a temporary fix.

This document reflects the MSBuild project configurations, supporting only the
most recent versions of the used toolchains at the time of commit.

The CMake configurations are kept in sync.
With additional support for the toolchains used on the CI environments.  
See:
[AppVeyor][AppVeyor-link] _(master branch)_ for the Windows builds and
[TravisCI][Travis-link] for Linux and Apple OSX builds.

Project specific settings are documented in their individual README files.
Any other deviations should be considered configuration errors.

<!-- TOC -->
- [General Information](#general)
- [Formatting](#format)
- [Compiler Settings](#compilers)
  - [MSVC](#msvc)
  - [Clang](#clang)
  - [Intel C++](#intel)
- [Static Analysers](#analysers)
  - [MSVC static analyser]
  - [Clang-Tidy](#tidy)
  - [PVS-Studio]
  - [Cppcheck]

<!---------------------------------------------------------><a id="general"></a>
## General Information ##
<!----------------------------------------------------------------------------->
Targets:
- Standard conforming (currently: C++17)
- Cross-platform (currently: Windows, Linux and OSX)
- Compiling with any conforming compiler
  (currently: MSVC, LLVM/clang, Intel C++, GCC and AppleClang)
- Being warning free, while enabling as many warnings as possible
- Increase execution performance (with a preference for x64 and MSVC)


<!----------------------------------------------------------><a id="format"></a>
## Formatting ##
<!----------------------------------------------------------------------------->
All .h and .cpp files are formatted with the latest stable Clang-Format release.
Using the supplied [.clang-format](../.clang-format) configuration file.
<!-- discussion -->


<!-------------------------------------------------------><a id="compilers"></a>
## Compiler Settings ##
<!----------------------------------------------------------------------------->
<!-- different supported compilers and their use -->
Everything is configured for compilation with [MSVC](#msvc) and [Clang](#clang)
on Windows. Using MS Build as a build system.

The configuration for GCC (on a remote Linux host) has not been included in this
project.

<!------------------------------------------------------------><a id="msvc"></a>
### MSVC ###
<!----------------------------------------------------------------------------->
MS Build configurations:
- Debug x64/x86
- Release x64/x86

#### Basic Settings ####
<!----------------------------------------------------------------------------->
**The IDE:**  
Add the Sudoku project as a reference to each project for IntelliSense support.

##### All configurations
```
/std:c++17         ISO C++ 17 standard
/sdl               Enable SDL checks, additional warnings focused on security
/MP                Multiprocessor compilation (Do not use /Gm)
/permissive-       Disable non-conforming constructs in Visual C++:
                   Enables: /Zc:rvaluecast /Zc:strictStrings and more.
                   (Updated compiler conformance.)
/GR-               Disable rtti (run-time type information) smaller image.
                   Turn on to use dynamic_cast or typeid.
/Yu"precompiled.h" Use precompiled headers
/FI"precompiled.h" Force include, add precompiled to all files

---- implicitly set flags:
/GS                Buffer Security Check. (default)
/FC                Full-path of source code file in diagnostics (default)
/Gm-               Prefer /MP (default)
```
##### Debug configurations
`````
---- implicitly set flags:
/ZI              Program database for edit-and-continue (sets /Gy and /FC)
                 (default for Debug)
/Gy              Function-level linking. (set by /ZI)
/INCREMENTAL     Enable incremental linking (default for Debug in linker)
/DEBUG:FASTLINK  Debug information format, allowing for faster linking.
                 Enable generation of full-program database is needed for
                 OpenCppCoverage. (default for Debug in linker)
`````
##### Release configurations
```
/MT        Runtime library for linking: Multi-threaded (static library, *.lib)
/guard:cf  Control Flow Guard, compiler analyses control flow for indirect calls
           at compile time and records the results in the compiled binary.
           During runtime Windows checks before every indirect call and raises
           an exception if any check fails at runtime.
           Can't be used with /ZI (edit and continue).

---- implicitly set flags:
/O2        Maximize Speed (default Release)
/GL        Whole program optimization (default Release)
```
#### Useful settings for incidental use ####
<!----------------------------------------------------------------------------->
```
/nologo    Disable to see the command-line input
/E
/EP
/showIncludes
```
#### Links ####
<!----------------------------------------------------------------------------->
- [Visual C++ compiler options (documentation)](https://docs.microsoft.com/en-gb/cpp/build/reference/compiler-options-listed-by-category)
- [Visual C++ conformance mode, using the permissive- switch](https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/)
  - Adds [two-phase name lookup](https://blogs.msdn.microsoft.com/vcblog/2017/09/11/two-phase-name-lookup-support-comes-to-msvc/)
- [Security features](https://blogs.msdn.microsoft.com/vcblog/2017/06/28/security-features-in-microsoft-visual-c/)
- [Speed up builds](https://blogs.msdn.microsoft.com/vcblog/2016/10/26/recommendations-to-speed-c-builds-in-visual-studio/)

#### Enable Warnings: ####
<!----------------------------------------------------------------------------->
These warning settings are used for all configurations.  
Documentation:
[VC++ Compiler Warnings](https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings)

##### All configurations
````
/W4        Warning level 4 (=highest) (/Wall triggers library warnings)
````

##### DisableSpecificWarnings
- C4715  bugged, warns when all switch over all cases in an enum.

**Additional command-line options** are used almost exclusively to enable
[compiler warnings that are off by default](https://docs.microsoft.com/en-gb/cpp/preprocessor/compiler-warnings-that-are-off-by-default).  

**Warning level configuration flag format**:
```
/w19999	sets the warning level for "9999" to 1
/wd9999	disables warning "9999"
/we9999	promotes warning "9999" to an error
/wo9999	reports warning "9999" only once
```
Because warning level 4 is used, most warnings are enabled at their default
levels. "Nonstandard extension" warnings are promoted to errors.

Warnings for precompiled.cpp are less strict were needed for external libraries.

**Document format**:
Flags currently not enabled are marked with a `--`.  
Flags not set for all translation units are marked with a `*`.  
````
/w44062 enumerator 'identifier' in switch of enum 'enumeration' is not handled
        and there is no default label (C4061 would always require a case label).
/w34191 'operator/operation': unsafe conversion from 'type of expression' to
        'type required'
/w44242 'identifier': conversion from 'type1' to 'type1', possible loss of data
/w44244 'conversion' conversion from 'type1' to 'type2', possible loss of data
/w44254 'operator': conversion from 'type1:field_bits' to 'type2:field_bits',
        possible loss of data
/w44263 'function': member function does not override any base class virtual
        member function
/w14264 'virtual_function': no override available for virtual member function
        from base 'class'; function is hidden
/w34265 'classname': class has virtual functions, but destructor is not virtual
        instances of this class may not be destructed correctly
/w44266 'function': no override available for virtual member function from base
        'type'; function is hidden
/w34287 'operator': unsigned/negative constant mismatch
/we4289 nonstandard extension used: 'variable': loop control variable declared
        in the for-loop is used outside the for-loop scope (level 3)
/w44296 'operator': expression is always false
/w14355 'this': used in base member initializer list
*/w44365 'action': conversion from 'type_1' to 'type_2', signed/unsigned
         mismatch
         223 hits, in stl headers: but most int -> size_t issue;
         that should be possible to improve on
         (*/precompiled.cpp)
/w14388 signed/unsigned mismatch
/w14426 optimization flags changed after including header, may be due to #pragma
        optimize()
/w44437 dynamic_cast from virtual base 'class1' to 'class2' could fail in some
          contexts
/w34444 top level '__unaligned' is not implemented in this context
/w44464 relative path contains '..'
/w14545 expression before comma evaluates to a function which is missing an
        argument list
/w14546 function call before comma evaluates to a function which is missing an
        argument list
/w14547 'operator': operator before comma has no effect; expected operator with
        side-effect
/w14548 expression before comma has no effect; expected expression with
        side-effect
/w14549 'operator': operator before comma has no effect; did you intend
        'operator'?
/w14555 expression has no effect; expected expression with side-effect
/w44574 'identifier' is defined to be '0': did you mean to use'#if identifier'?
/w34598 '#include "header"': header in the precompiled header does not match
        current compilation at that position
/w34599 'option path': command-line argument does not match pre-compiled
        header
/w14605 '/Dmacro' specified on current command line, but was not specified when
        precompiled header was built
/w34619 pragma warning: there is no warning number 'number'
*/w44623 'derived class': default constructor was implicitly defined as deleted.
         because a base class default constructor is inaccessible or deleted.
         (SudokuTests/precompiled.cpp | gtest, in <xtree> from gtest-internal.h)
--/w44625 'derived class': copy constructor could not be generated because a base
         base class copy constructor is inaccessible
         (*/precompiled.cpp | warns on stl)
         (warns on class containing a std::unique_ptr)
--/w44626 'derived class': assignment operator could not be generated because a
          base assignment operator is inaccessible
          (*/precompiled.cpp | warns on gsl/multi_span and stl)
          (warns on all classes with a `const` data-member)
/w34640 'instance': construction of local static object is not thread-safe
/w44654 Code placed before include of precompiled header line will be ignored.
*/w44668 'symbol' is not defined as a pre-processor macro, replacing with '0'
        for 'directives'
        (*/precompiled.cpp | hits on gtest & gsl/gsl_byte)
/w34686 'user-defined type': possible change in behaviour, change in UDT return
        calling convention
/w34738 storing 32-bit float result in memory, possible loss of performance
*/w44774 'string': format string expected in argument number is not a string
         literal
         note: e.g. instead of printf(name); use printf("%s", name); because
         format specifiers in 'name' may pose a security issue
         note: consider using constexpr specifier for named string literals
         (*/precompiled.cpp | hits in stl headers: string, xlocnum)
/w14826 Conversion from 'type1' to 'type_2' is sign-extended. This may cause
        unexpected runtime behaviour.
/we4861 non-standard extension used: compound member designator used in offsetof
        (default: level 4)
/w44868 'file(line_number)' compiler may not enforce left-to-right evaluation
        order in braced initialization list
/w14905 wide string literal cast to 'LPSTR'
/w14906	string literal cast to 'LPWSTR'
/w14928 illegal copy-initialization; more than one user-defined conversion has
        been implicitly applied
/w14946 reinterpret_cast used between related classes: 'class1' and 'class2'
/w44986 'symbol': exception specification does not match previous declaration
/we4987 nonstandard extension used: 'throw(...)' (default: level 4)
/w44988 'symbol': variable declared outside class/function scope
/w15022 'type': multiple move constructors specified
/w15023 'type': multiple move assignment operators specified
/w45024 'type': move constructor was implicitly defined as deleted
/w45025 'type': move assignment operator was implicitly defined as deleted
*/w45026 'type': move constructor was implicitly defined as deleted
         (*/precompiled.cpp | system_error & gtest)
--/w45027 'type': move assignment operator was implicitly defined as deleted
          (hits on system_error & gsl/multi_span)
          (hits on all classes with a `const` data-member)
/we5029 nonstandard extension used: alignment attributes in C++ apply to
        variables, data members and tag types only (default: level 4)
/w45031 #pragma warning(pop): likely mismatch, popping warning state pushed in
        different file
/w45032 detected #pragma warning(push) with no corresponding #pragma
        warning(pop)
````

<!-----------------------------------------------------------><a id="clang"></a>
### Clang-cl ###
<!----------------------------------------------------------------------------->
MS Build configurations:
- LLVM_Debug x64/x86
- LLVM_Release x64/x86

[The latest user manual](https://clang.llvm.org/docs/UsersManual.html)
Lists all options supported by Clang-cl.  
Use `-###` to output the commands that actually reach the compiler, per file.  
    `-v`   Verbose mode (as above?)  
Use `-Xclang ` before a command to actually force it to the compiler.  
*Note: Inactive settings are indented.*

##### All configurations
`````
/std:c++latest        sets: -std=c++2a
  /std:c++17          sets: -std=c++17
  -Xclang -std=c++17  Set language version to C++17
/GR-                  sets: -fno-rtti
/GS                   Buffer Security Check. (default)
-fno-strict-aliasing
  -Xclang -fms-compatibility-version=19.12
  // Since v6.0.0: the full version number is inherited from VC.
  // Upgrade to actual VS version, override `-fmsc-version=1912` as set in the
  // toolset for actual version see macro `_MSC_FULL_VER`
  -fms-compatibility Excepting enough invalid C++ to parse most MS headers
  -fno-ms-compatibility
`````
##### Debug configurations
`````
Support Just My Code = false
  // Not supported in v7.0.0. (Enabled by default for debug builds.)
/Zi        alias for /Z7 CodeView debug information in object files (no pdb)
`````
##### Release configurations
Compiler:  Debug information format: none; drastically reduce binary size.  
Linker:    Generate debug info:      no
```
/MT        Use static run-time
```

#### Enable Warnings
<!----------------------------------------------------------------------------->
[lefticus/cppbestpractices](https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#gcc--clang)  
Some warnings are explicitly set so they stay active when an encompassing
setting is disabled.  
[Clang diagnostics reference](https://clang.llvm.org/docs/DiagnosticsReference.html)
````
/Wall                Enables -Weverything
-Weverything         All warnings enabled
  -Wall              
  -Wextra            
  -Wpedantic         ISO C/C++ conformance (warn on extensions)
  -Wconversion       Enable warnings for unexpected conversions losing data on
                     fundamental types, enums and strings.
  -Wshadow-all       Uncaptured locals; Variable declaration overshadows one
                     from the parent context. (Same name.)
  -Wold-style-cast   Warn for c-style casts
  -Wdouble-promotion `float` implicit promoted to `double`
````
##### Promote to errors
````````
-Werror=return-type
````````
##### Disable (temporary) unwanted/incompatible warnings
``````
-Wno-c++98-compat    (Weverything) Compatibility with C++98 is not needed
-Wno-c++98-compat-pedantic
``````

<!-----------------------------------------------------------><a id="intel"></a>
### Intel C++ ###
<!----------------------------------------------------------------------------->
MS Build configurations:
- Intel_Debug x64/x86
- Intel_Release x64/x86

#### Basic Settings ####
<!----------------------------------------------------------------------------->
##### All configurations
```
/MP             Multiprocessor compilation
/GR-            Disable RTTI
/GS             Buffer security check (default)
/permissive-
/Qstd=c++17     Intel C++17 implementation.
```
##### Debug configurations
`````
/debug:expr-source-pos
`````
##### Release configurations
````
/O3         Highest optimization level
/Oi         Enable intrinsics
/guard:cf   Enable control-flow guards
/MT         Multi-threaded static runtime library
````
#### Enable Warnings: ####
<!----------------------------------------------------------------------------->
##### All configurations
````
/W5         Warning level 5  Intel specific.
/W4         (precompiled.cpp)
````
##### DisableSpecificWarnings
`/Qdiag-disable:<id>[,<id>]`
- 304   Message: access control not specified ("public" by default in `struct`)
- 981   Message: operands are evaluated in unspecified order

**precompiled.cpp**
- 1292  Message: unknown attribute "gsl::suppress"
- 3924  Warning: attribute namespace "gsl" is unrecognised

<!-------------------------------------------------------><a id="analysers"></a>
## Static Analysers ##
<!----------------------------------------------------------------------------->
- [MSVC static analyser]
- [Clang-Tidy](#tidy)
- [PVS-Studio]
- [Cppcheck]

<!------------------------------------------------------------><a id="tidy"></a>
### Clang-Tidy ###
<!----------------------------------------------------------------------------->
General configuration: [.clang-tidy](../.clang-tidy).  
For SudokuTests a modified configuration has been created:
[SudokuTests/.clang-tidy](../SudokuTests/.clang-tidy).

##### Disabled checks
Notes on the motivation behind the disabling of some of the checks.
- `-cert-dcl21-cpp`
  Advice: run this check incidentally.
  Useful to warn on use of a reference type, but with value types following this
  advice to return const object breaks repeated use of the postfix increment and
  decrement operators.
  (See discussion: 
  [Matt Godbolt on Twitter](https://twitter.com/mattgodbolt/status/981269382092468226))  
  Documentation:
  [cert-dcl21-cpp](https://clang.llvm.org/extra/clang-tidy/checks/cert-dcl21-cpp.html)

----
[top](#top)

[AppVeyor-link]:  https://ci.appveyor.com/project/Farwaykorse/fwksudoku/branch/master
[Travis-link]:    https://travis-ci.com/Farwaykorse/fwkSudoku/branches
