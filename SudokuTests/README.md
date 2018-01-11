<!----------------------------------------------------------------><a id="top"></a>
# Unit Testing for Sudoku.lib #
<!---------------------------------------------------------------->
Using the GoogleTest Unit-testing framework

[gTest repository](https://github.com/google/googletest/)


Testing Sudoku.lib
Console project called: SudokuTest(s)

<!-- TOC -->
[Visual Studio Setting](#vs_settings)
	Compiler settings
[Code Coverage](#cover)
[Notes](#notes)

<!----------------------------------------------------------------><a id="vs_settings"></a>
## Visual Studio Settings ##
<!---------------------------------------------------------------->
- Add reference to the project under test (context menu)
- Add: Configuration Properties > Linker > Input > Additional Dependencies:
	$(VcpkgRoot)debug\lib\manual-link\gtest.lib			debug-mode (both x86 and x64)
	$(VcpkgRoot)lib\manual-link\gtest.lib				release-mode
	** disabled: **
	$(VcpkgRoot)debug\lib\manual-link\gtest_main.lib		if no custom main()
	$(VcpkgRoot)lib\manual-link\gtest_main.lib				if no custom main()
	note: if gmock is used, the gmock.lib contains all of gtest.lib, therefor ony one of these can be included at any time.

<!--------------------------------------------------------><a id="compiler"></a>
### Compiler settings
**Clang:**
Dissabled warnings:
``````
-Wno-unused-comparison       Every comparrison check ...
-Wno-unused-value
-Wno-zero-as-null-pointer-constant
-Wno-global-constructors     gTest macro
-Wno-used-but-marked-unused  ScopedTrace macro on: Location.cpp
-Wno-sign-conversion         on: Board.cpp
-Wno-covered-switch-default  gTest macro debug-death on: Board.cpp
``````
#### Custom settings for precompiled.cpp
**VC++:**  
```
/wd4619   pragma warning: there is no warning number 'number'
          gTest, warning 4800 was removed in VS2007
		  not working ... preprocessor macros...
```
**Clang:**
Dissabled warnings:
````
-Wno-deprecated-declarations gTest uses deprecated POSIX names (ie chdir)
-Wno-undef
-Wno-language-extension-token
-Wno-missing-noreturn 
````

<!-----------------------------------------------------------><a id="cover"></a>
## Code Coverage ##
<!----------------------------------------------------------------------------->
Code coverage checks using OpenCppCoverage.
https://opencppcoverage.codeplex.com
Run by the Powershell script: coverage.ps1

Generated report available in `SudokuTests\Coverage\[Debug]\index.html`


<!----------------------------------------------------------------><a id="notes"></a>
## Notes ##
<!---------------------------------------------------------------->
Catch debug assert() from <cassert> with:

    EXPECT_DEBUG_DEATH({ [statement]; }, "Assertion failed: .*");


[top](#top)
``````