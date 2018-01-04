<!-------------------------------------------------------------><a id="top"></a>
# Sudoku #
<!----------------------------------------------------------------------------->
<!-- Group > Package(this) > Component -->
<!-- Description -->
Data structure and solvers.

<!-- TOC -->
[General Information](#general)
	Compiler Settings
[Classes](#elements)
	<!-- win_app -->
[Unit-Testing](#test)
[Other](#other)
	Natvis
[Links](#link)

<!---------------------------------------------------------><a id="general"></a>
## General Information ##
<!----------------------------------------------------------------------------->
<!-- Usage -->
This project generates a static library.

<!-- Compilation -->
<!-- libraries -->
This project uses the STL and GSL.

<!----------------------------------------------------------------------------->
### Compiler Settings ###

#### VC++ ####
Don't use:
```
/Gm				Prefere /MP
```

Not sure:
```
/guard:cf		Control Flow Guard, compiler analizes control flow for indirect calls at compile time and records the results in the compiled binary. During runtime Windows checks before every indirect call and raises an exception if any check fails at runtime. 
				can't be used with /ZI (edit and continue)
```
Usefull:
```
/E
/EP
/showIncludes
```

All:
```
/diagnostics:caret DiagnosticsFormat: caret
/std:c++17
/MP					Multiprocessor compilation
/W4					Warning level 4 (=highest) (/Wall triggers to-many libraray
					warnings)
/GS					Buffer Security Check.
/sdl				Enable SDL checks, additional warnings focused on security
/permissive-       dissable non-conforming constructs in Visual C++: [1]
                   Enables: /Zc:rvaluecast /Zc:strictStrings and more!
				   (updated compiler conformance)
/Zc:rvaluecast     Enforce type-conversion rules (C++11) Needed for conformance
/GR-               Dissable rtti (run-time type information) smaller image
                   Turn on (default) to use dynameic_cast or typeid
/Yu"precompiled.h" Use precompiled headers
/FI"precompiled.h" Force include, add precompiled to all files
```

Linker: (in projects using this library)
```
/incremental    enable incremental linking
/debug:fastlink Debug information format, allowing for faster linking
                Enable generation of full-program database is needed for
                OpenCppCoverage
```

Debug:
`````
/MTd	Run-time library for linking: Multi-threaded debug (use .lib)
/ZI		Program database for edit-and-continue (sets /Gy and /FC)
/Gy		Function-level linking
/FC		Full-path of source code file in diagnostics
`````

Release:
```
/O2		Maximize Speed
/GL		Whole program optimization
/MT		Runtime library for linking: Multi-threaded
        (use static libraries .lib)
```


##### Additional command-line options: #####
Documentation:
[VC++ Compiler Marnings](https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings)

Warning level settings format:
```
/w19999	sets the warning level for "9999" to 1
/wd9999	dissables warning "9999"
/we9999	promotes warning "9999" to an error
/wo9999	reports warning "9999" only once
```
[Warnings That are Off by Default](https://docs.microsoft.com/en-gb/cpp/preprocessor/compiler-warnings-that-are-off-by-default)  
Because warning level 4 is used, most warnings are enabled at their default
levels. "Nonstandard extension" warnings are promoted to errors.
````
/w44062 enumerator 'identifier' in switch of enum 'enumeration'is not handled
        and there is no defaut label (C4061 would always require a case label).
/w34191 'operator/operation': unsave conversion from 'type of exression' to
        'type required'
/w44242 'identfier': conversion from 'type1' to 'type1', possible loss of data
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
--*/w44365 'action': conversion from 'type_1' to 'type_2', signed/unsigned
        mismatch
        223 hist, in stl headers: but most int -> size_t issue;
		that should be possible to improve on
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
/w34598 '#include "header"': header number number in the precompiled header does
        not match current compilation at that position
/w34599 'option path': command-line argument number number does not match
        pre-compiled header
/w14605 '/Dmacro' specified on current command line, but was not specified when
        precompiled header was built
/w34619 pragma warning: there is no warning number 'number'
--*/w44623 'derived class': default constructor was implicitly defined as
        deleted because a base class default constructor is inaccessible or
        deleted
		1 hit
--*/w44625 'derived class': copy constructor could not be generated because a
        base class copy constructor is inaccessible
	hits on stl
--*/w44626 'derived class': assignment operator could not be generated because a
        base assignment operator is inaccessible
	hits on gsl and stl
/w34640 'instance': construction of local static object is not thread-safe
/w44654 Code placed before include of precompiled header line will be ignored.
--*/w44668 'symbol' is not defined as a preprocessor macro, replacing with '0'
        for 'directives'
	hits on gtest
/w34686 'user-defined type': possible change in behavior, change in UDT return
        calling convention
/w34738 storing 32-bit float result in memory, possible loss of performance
--*/w44774 'string': format string expected in argument number is not a string
        literal
        note: e.g. instead of printf(name); use printf("%s", name); because
        format specifiers in'name' may pose a security issue
        note: consider using constexpr specifier for named string literals
        24 hits, in stl headers: string, xlocnum
/w14826 Conversion from 'type1' to 'type_2' is sign-extended. This may cause
        unexpected runtime behavior.
/we4861 non-standard extension used: compound member designator used in offsetof
        (level 4)
/w44868 'file(line_number)' compiler may not enforce left-to-right evaluation
        order in braced initialization list
/w14905 wide string literal cast to 'LPSTR'
/w14906	string literal cast to 'LPWSTR'
/w14928 illegal copy-initialization; more than one user-defined conversion has
        been implicitly applied
/w14946 reinterpret_cast used between related classes: 'class1' and 'class2'
/w44986 'symbol': exception specification does not match previous declaration
/we4987 nonstandard extension used: 'throw(...)' (level 4)
/w44988 'symbol': variable declared outside class/function scope
/w15022 'type': multiple move constructors specified
/w15023 'type': multiple move assignment operators specified
/w45024 'type': move constructor was implicitly defined as deleted
/w45025 'type': move assignment operator was implicitly defined as deleted
--*/w45026 'type': move constructor was implicitly defined as deleted
--*/w45027 'type': move assignment operator was implicitly defined as deleted
/we5029 nonstandard extension used: alignment attributes in C++ apply to
        variables, data members and tag types only (level 4)
/w45031 #pragma warning(pop): likely mismatch, popping warning state pushed in
        different file
/w45032 detected #pragma warning(push) with no corresponding #pragma
        warning(pop)
````

#### Clang-cl ####
Inactive settings are indented.  
Use `-###` to output the commands that actually reach the compiler.  
Use `-Xclang ` before a command to actually force it to the clang.
`````
-Xclang -std=c++17    Set language version to C++17
-fno-strict-aliasing
-Xclang -fms-compatibility-version=19.10  upgrade to actual vs version
  -fms-compatibility Excepting enough invalid C++ to parse most MS headers
  -fno-ms-compatibility

Debug:
...
Release:
...
`````
Enable warnings:  
[lefiticus/cppbestpractices](https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#gcc--clang)  
Some warnings are explicitly mentioned so they stay active when an encompassing
set is dissabled.
````
-Weverything         Enable all warnings (includes -pedantic)
/W4                  Enable -Wall and -Wextra
-Wall                standard
-Wextra              standard
-Xclang -pedantic    using non-standard language extensions

-Weffc++             
-Wshadow             A variable declaration overshadows one from the parent
                     context. (Same name.)
-Wunused             Anything declared but unused.
-Wold-style-cast     Warn for c-style casts
-Wconversion         Type conversions that may lose data
-Wsign-conversion    Sign conversion
-Wnull-dereference   Null dereference detected
-Wdouble-promotion   `float` implicit promoted to `double`
-Wformat=2           Security issues in output formating (ie printf)
-Woverloaded-virtual Overloading (not overriding) a virtual function
-Wnon-virtual-dtor   Class with virtual member functions has a non-virtual
                     destructor. Can result in memory errors.
-Wreorder            
-Wcast-align         

Unknown warnings:
  -Wmisleading-indentation Indentation implies blocks that don't exist
  -Wduplicated-cond        if/else chain has duplicated conditions
  -Wduplicated-branches    if/else branches have duplicated code
  -Wlogical-op             Logical operations used where probably bitwise is wanted
  -Wuseless-cast           Casting to the same type
````
Disable (temporary) unwanted/uncompatible warnings:
``````
-Wno-c++98-compat   (Weverything) Compatibility with C++98 is not needed
-Wno-c++98-compat-pedantic
-Wno-keyword-macro  GSL: keyword hidden by macro definition
``````

<!--------------------------------------------------------><a id="elements"></a>
## Classes ##
<!----------------------------------------------------------------------------->
- Board  
	public data structure
- Options
	the per element data
- Location
- ...

<!------------------------------------------------------------><a id="test"></a>
## Unit-Testing ##
<!----------------------------------------------------------------------------->
Unit tests are written for gTest and collected in the SudokuTests project.


<!-----------------------------------------------------------><a id="other"></a>
## Other ##
<!----------------------------------------------------------------------------->
### .natvis ###
<!----------------------------------------------------------------------------->
Natvis files in the project.
Generate custom views for objects in the Visual Studio debugger.
- Board
- Location
- Options (TODO)



<!------------------------------------------------------------><a id="link"></a>
## Links ##
<!----------------------------------------------------------------------------->
Visual C++ compiler options (documentation):
	[0](https://docs.microsoft.com/en-gb/cpp/build/reference/compiler-options-listed-by-category)
Visual C++ conformance mode, using the permissive- switch
	[1](https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/)
	Adds two-phase name lookup:
		[2](https://blogs.msdn.microsoft.com/vcblog/2017/09/11/two-phase-name-lookup-support-comes-to-msvc/)
	[3](https://blogs.msdn.microsoft.com/vcblog/2017/06/28/security-features-in-microsoft-visual-c/)
Speed up builds:
[4](https://blogs.msdn.microsoft.com/vcblog/2016/10/26/recommendations-to-speed-c-builds-in-visual-studio/)

https://blogs.msdn.microsoft.com/vcblog/2016/11/16/permissive-switch/


----
[top](#top)
