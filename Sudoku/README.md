<!----------------------------------------------------------------><a id="top"></a>
# Sudoku #
<!---------------------------------------------------------------->
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

<!----------------------------------------------------------------><a id="general"></a>
## General Information ##
<!---------------------------------------------------------------->
<!-- Usage -->
This project generates a static library.

<!-- Compilation -->
<!-- libraries -->
This project uses the STL and GSL.

<!---------------------------------------------------------------->
### Compiler Settings ###

#### VC++ ####
Don't use:
/Gm				Prefere /MP

Not sure:
/guard:cf		Control Flow Guard, compiler analizes control flow for indirect calls at compile time and records the results in the compiled binary. During runtime Windows checks before every indirect call and raises an exception if any check fails at runtime. 
	can't be used with /ZI (edit and continue)
Usefull:
/E
/EP
/showIncludes

All:
DiagnosticsFormat: Column
/std:c++17			
/MP					Multiprocessor compilation
/W4					Warning level 4 (=highest) (/Wall triggers to-many libraray warnings)
/GS					Buffer Security Check.
/sdl				Enable SDL checks, additional warnings focused on security
/Zc:rvaluecast		Enforce type-conversion rules (C++11) Needed for conformance
/Yu"precompiled.h"	Use precompiled headers
/FI"precompiled.h"	Force include, add precompiled to all files

Linker: (in projects using this library)
	/incremental	enable incremental linking
	/debug:fastlink	Debug information format, allowing for faster linking
					Enable generation of full-program database is needed for OpenCppCoverage

Debug:
/MTd		Run-time library for linking: Multi-threaded debug (use .lib)
/ZI			Program database for edit-and-continue (sets /Gy and /FC)
/Gy			Function-level linking
/FC			Full-path of source code file in diagnostics

Release:
/O2			Maximize Speed
/GL			Whole program optimization
/MT			Runtime library for linking: Multi-threaded (use static libraries .lib)


##### Additional command-line options: #####
/permissive-	dissable non-conforming constructs in Visual C++: [1]
				Enables: /Zc:rvaluecast /Zc:strictStrings and more! (updated compiler conformance)
/w14242			'identfier': conversion from 'type1' to 'type1', possible loss of data
/w14254			'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
/w14263			'function': member function does not override any base class virtual member function
/w14265			'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
/w14287			'operator': unsigned/negative constant mismatch
/we4289			nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
/w14296			'operator': expression is always 'boolean_value'
/w14311			'variable': pointer truncation from 'type1' to 'type2'
/w14545			expression before comma evaluates to a function which is missing an argument list
/w14546			function call before comma missing argument list
/w14547			'operator': operator before comma has no effect; expected operator with side-effect
/w14549			'operator': operator before comma has no effect; did you intend 'operator'?
/w14555			expression has no effect; expected expression with side-effect
/w14619			pragma warning: there is no warning number 'number'
/w14640			Enable warning on thread un-safe static member initialization
/w14826			Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
/w14905			wide string literal cast to 'LPSTR'
/w14906			string literal cast to 'LPWSTR'
/w14928			illegal copy-initialization; more than one user-defined conversion has been implicitly applied


#### Clang/C2 ####
```
`-Wmisleading-indentation` not supported
-std=c++1z				Enable suported C++17 features
-Weverything			Enable ALL warnings (dissable unwanted)
	-Wno-c++98-compat		Dissabled since compatibility is not needed

-Wextra
-Wshadow
-Wnon-virtual-dtor
-Wold-style-cast
-Wcast-align
-Wunused
-Woverloaded-virtual
-pedantic
-Wconversion
-Wsign-conversion
-Weffc++
-Wformat=2
-fno-common
-Wreorder
 ```




<!----------------------------------------------------------------><a id="elements"></a>
## Classes ##
<!---------------------------------------------------------------->
- Board  
	public data structure
- Options
	the per element data
- Location
- ...

<!----------------------------------------------------------------><a id="test"></a>
## Unit-Testing ##
<!---------------------------------------------------------------->
Unit tests are written for gTest and collected in the SudokuTests project.


<!----------------------------------------------------------------><a id="other"></a>
## Other ##
<!---------------------------------------------------------------->
### .natvis ###
<!---------------------------------------------------------------->
Natvis files in the project.
Generate custom views for objects in the Visual Studio debugger.
- Board
- Location
- Options (TODO)

<!---------------------------------------------------------------->


<!----------------------------------------------------------------><a id="link"></a>
## Links ##
<!---------------------------------------------------------------->
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


[top](#top)
