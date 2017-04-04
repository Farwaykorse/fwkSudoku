<!----------------------------------------------------------------><a id="top"></a>
# Sudoku
<!---------------------------------------------------------------->
<!-- Description -->
A project undertaken while learning C++.
The projects offers a personal platform to use while explorering the language,
different techniques and tools.
It is not about creating the solvers (that could be found on the web anyway) or delivering the perfect app.
Some known algorithms might be implemented in time, but the purpose is learning through play.
The code might be frequently rewritten and different techniques and libraries are being tried.

<!-- TOC -->
[General Information](#general)
[Elements](#elements)  
	[Sudoku.lib](#sudoku.lib)
	[Console.exe](#console.exe)
	<!-- win_app -->
[Unit-Testing](#unit-testing)

<!----------------------------------------------------------------><a id="general"></a>
## General Information ##
<!---------------------------------------------------------------->
<!-- installation -->
<!-- usage -->
The code is manly written and compiled using the Visual Studio platform.

External libraries are imported using the [vcpkg project](https://github.com/Microsoft/vcpkg)


<!----------------------------------------------------------------><a id="elements"></a>
## Elements ##
<!---------------------------------------------------------------->

<!----------------------------------------------------------------><a id="sudoku.lib"></a>
### Sudoku.lib ###
<!---------------------------------------------------------------->
The internal processing, datastructure and solvers

Internal structures:
Data
- Board
	Data container object template for a sudoku.
	Organizes access to the elements, rows, collumns and blocks.
	Elements can contain values or if used in solvers: Options
- Options
	The data-type used in the elements of Board for the solvers.
	bitset, bit 0 = answer label. bit [1,N*N] possible element values
	- operation available on these



Section.h
	Used by Board, to access element within rows, collumns and blocks.
Location.h
	Used by Board, to calculate location based relations
Solver.h
	Container for solver functions and their helpers.


<!----------------------------------------------------------------><a id="console.exe"></a>
### Console.exe ###
<!---------------------------------------------------------------->
Console application, interface


<!----------------------------------------------------------------><a id="unit-testing"></a>
## Unit-Testing ##
<!---------------------------------------------------------------->
<!-- description -->

### MStest ###
The first implemented unit-tests apply to the Sudoku solver and data library file.
Gradually replaced by gTest.


### gTest ###
The Google Test unit-testing framework is used on the Sudoku solver and data library.

[gTest](https://github.com/google/googletest/)
[Sudoku.lib](../Sudoku/README.md)

### Catch ###
TODO: try this framework on the Console project.


<!-- Contributing -->
<!-- Credits -->
<!-- License -->
	<!-- none yet. this means default copyright applies -->

[top](#top)
