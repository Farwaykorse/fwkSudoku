<!-------------------------------------------------------------><a id="top"></a>
# Sudoku #
<!----------------------------------------------------------------------------->
<!-- Description -->
Sudoku is a header-only library, part of the
[fwkSudoku](https://github.com/Farwaykorse/fwkSudoku) project,
providing data-structures and solvers.

<!-- TOC -->
- [General Information](#general)
- [Classes](#elements)
- [Unit-Testing](#test)
- [Other](#other)
  - Natvis

<!---------------------------------------------------------><a id="general"></a>
## General Information ##
<!----------------------------------------------------------------------------->
<!-- Usage -->
The supplied project configurations generate a static library.

<!-- Compilation -->
<!-- libraries -->
This project requires an C++17 conforming STL implementation and the GSL
(Guideline Support Library), available at:
[Microsoft/GSL](https://github.com/Microsoft/GSL).

More on the project and compiler configurations in: 
[Configuration.md](../Configuration.md).

<!--------------------------------------------------------><a id="elements"></a>
## Classes ##
<!----------------------------------------------------------------------------->
- Board  
	Data container object template for a sudoku.
	It provides access to the elements, rows, columns and blocks.
- Options  
	Used for per element data in solvers.
	Implemented on std::bitset, where bit 0 serves as the answer label and the 
	bits [1, N^2] represent possible element values.
- Location
	Implements a `constexpr` helper type to provide the locality relations
	between elements.
- ...

<!------------------------------------------------------------><a id="test"></a>
## Unit-Testing ##
<!----------------------------------------------------------------------------->
Unit tests are written for Google Test and collected in the
[SudokuTests](../SudokuTests) project.


<!-----------------------------------------------------------><a id="other"></a>
## Other ##
<!----------------------------------------------------------------------------->
### .natvis debug views ###
<!----------------------------------------------------------------------------->
Several natvis files are provided in the project.  
These are used by the Visual Studio Debugger to generate custom views for
objects. Documentation on
[docs.microsoft.com](https://docs.microsoft.com/visualstudio/debugger/create-custom-views-of-native-objects).
- Board
- Location
- Options

----
[top](#top)
