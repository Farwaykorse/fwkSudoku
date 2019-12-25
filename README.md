<!-------------------------------------------------------------><a id="top"></a>
# fwkSudoku
<!----------------------------------------------------------------------------->
<!-- Badges -->
[![Build Status][Travis-badge]][Travis-link]
[![Build status][AppVeyor-badge]][AppVeyor-link]
[![codecov][Codecov-badge]][Codecov-link]
[![CodeFactor][CodeFactor-badge]][CodeFactor-link]
<!-- Description -->
Originally this started as a one of many small experiments while starting to
learn C++. With a mild interest in properties of sudokus. It was mainly focused
on the implementation of the data-structure and served to figure out the
implementation and use of STL iterators.  
It has become a familiar project to come back to.
Suitable for experimentation while exploring the language and trying different
techniques and tools.

Not many solvers have been implemented (they could be found on the web anyway).
And no real user interface has been added.
These might be implemented in time, but the main purpose of this project
is experimentation and learning through play.

<!-- TOC -->
- [General Information](#general)
- [Elements](#elements)  
  - [Sudoku - Library](#sudoku)
  - [Console - executable](#console)
- [Unit-Testing](#unit-testing)

<!---------------------------------------------------------><a id="general"></a>
## General Information ##
<!----------------------------------------------------------------------------->
<!-- installation -->
<!-- usage -->
The code is mainly written using the Visual Studio platform, and compiled with
the latest stable versions of MSVC, Clang and Intel on Windows, and GCC on Linux
(in the WSL). See the [AppVeyor][AppVeyor-link] and [Travis CI][Travis-link]
continuous integration suites for an up-to-date selection of supported compilers
and standard libraries (MS-STL, GNU libstdc++ and libc++).

Both MSBuild and CMake configurations are maintained.
The MSBuild projects are updated to the last stable Visual Studio version,
with configurations for Clang on Windows and the Intel C++ compilers.  
The CMake configuration applies the same settings but for a wider variety of
tool-chains and support for older compiler versions.

Several static analysers and extensive warning settings are used with each
compiler.  
More on the project and compiler configurations in: 
[docs/Configuration](./docs/Configuration.md).

#### Requirements:
Compiler with C++17 support.
A recent CMake release (see [cmake_minimum_required][CMake_version]).
Or Visual Studio 2019 to use the supplied solution.

#### Dependencies:
No external dependencies have been included in the repository.
The project configurations expect these to be available on the include path.
Development and CI testing is using the latest versions of external libraries
imported using the [vcpkg project][github-vcpkg], but any alternative method
would suffice.
- It makes use of elements from the GSL (Guideline Support Library), available
  at: [Microsoft/GSL][github-ms-gsl].
- The unit-tests for the Sudoku library are implemented with
  [Google Test][github-GTest].


<!--------------------------------------------------------><a id="elements"></a>
## Elements ##
<!----------------------------------------------------------------------------->
<!----------------------------------------------------------><a id="sudoku"></a>
### Sudoku - Library ###
<!----------------------------------------------------------------------------->
The main element is the Sudoku library.
A header-only library, containing the data-structures and solver
implementations.

See: [Sudoku/README](./Sudoku/README.md)


<!---------------------------------------------------------><a id="console"></a>
### Console - executable ###
<!----------------------------------------------------------------------------->
Console implements a basic console application.
No interface has been implemented.
The current implementation executes a few (partial) solvers on several boards,
to supply timings and detect performance regressions.


<!----------------------------------------------------><a id="unit-testing"></a>
## Unit-Testing ##
<!----------------------------------------------------------------------------->
<!-- description -->

### SudokuTest ###
The SudokuTest project contains the unit-tests for the Sudoku library.
This uses the [Google Test][github-GTest].
 unit-testing framework.

Note that the tests are in general extremely verbose.
Especially the compile-time evaluations.
This serves to generate an understanding of the implementation nuances.
And additionally, it alerts on differences and changes in external factors.
Such as library and compiler implementations.

See: [SudokuTest/README](./SudokuTest/README.md)

### UnitTest ###
Earlier tests for the Sudoku library were implemented with MStest.
These have been replaced by Google Test and are no longer part of the
implementation.


<!----------------------------------------------------><a id="contributing"></a>

<!----------------------------------------------------------------------------->


<!---------------------------------------------------------><a id="license"></a>
## License
<!----------------------------------------------------------------------------->
Code licensed under the [MIT License](./LICENSE).

[top](#top)

[AppVeyor-badge]: https://ci.appveyor.com/api/projects/status/fuasqqstakl49tfb/branch/master?svg=true
[AppVeyor-link]:  https://ci.appveyor.com/project/Farwaykorse/fwksudoku/branch/master
[Codecov-badge]:  https://codecov.io/gh/Farwaykorse/fwkSudoku/branch/master/graph/badge.svg
[Codecov-link]:   https://codecov.io/gh/Farwaykorse/fwkSudoku
[CodeFactor-badge]: https://www.codefactor.io/repository/github/farwaykorse/fwksudoku/badge/master
[CodeFactor-link]:  https://www.codefactor.io/repository/github/farwaykorse/fwksudoku/overview/master
[Travis-badge]:   https://travis-ci.com/Farwaykorse/fwkSudoku.svg?branch=master
[Travis-link]:    https://travis-ci.com/Farwaykorse/fwkSudoku/branches

[github-GTest]:   https://github.com/google/googletest
[github-ms-gsl]:  https://github.com/Microsoft/GSL
[github-vcpkg]:   https://github.com/Microsoft/vcpk

[CMake_version]:  https://github.com/Farwaykorse/fwkSudoku/CMakeLists.txt#L5
