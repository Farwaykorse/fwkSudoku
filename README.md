<!-------------------------------------------------------------><a id="top"></a>
# fwkSudoku
<!----------------------------------------------------------------------------->
<!-- Description -->
This project is mostly focused on the implementation of the data-structures.  
Originally this started as a one of many small experiments while starting to
learn C++. With a mild intrest in properties of sudokus.
It served mostly as a place to figure out the implementation and use of STL
iterators.
When it became a bit more complex, it became a familiar project to come back to.
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
Several C++17 compiler and STL features are used in this project.
The code is mainly written using the Visual Studio platform, and compiled with
the latest stable versions of MSVC and Clang on Windows.
The library and Console projects compile and run with GCC on Linux (in the WSL).

Several static analysers and extensive warning settings are used with each
compiler.  
More on the project and compiler configurations in: 
[Configuration.md](./Configuration.md).

#### Dependencies:
Non-of the external dependencies have been included in the repository.
The project configurations expect these to be available in the include path.
For this project the latest versions of external libraries have been imported
using the [vcpkg project](https://github.com/Microsoft/vcpkg).
- It makes use of elements from the GSL (Guideline Support Library), available
  at: [Microsoft/GSL](https://github.com/Microsoft/GSL).
- The unit-tests for the Sudoku library are implemented with
  [Google Test](https://github.com/google/googletest).


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
This uses the [Google Test](https://github.com/google/googletest).
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


[top](#top)
