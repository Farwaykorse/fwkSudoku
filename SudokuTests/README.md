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
- Console Project, No precompiled headers
- Add reference to the project under test (context menu)
- Add: Configuration Properties > Linker > Input > Additional Dependencies:
	$(VcpkgRoot)debug\lib\manual-link\gtest.lib			debug-mode (both x86 and x64)
	$(VcpkgRoot)lib\manual-link\gtest.lib				release-mode
	** disabled: **
	$(VcpkgRoot)debug\lib\manual-link\gtest_main.lib		if no custom main()
	$(VcpkgRoot)lib\manual-link\gtest_main.lib				if no custom main()
	note: if gmock is used, the gmock.lib contains all of gtest.lib, therefor ony one of these can be included at any time.

### Compiler settings
**Disabled:**
  **VC++:**
```
2017/04
/w14619			pragma warning: there is no warning number 'number'
```
**Clang:**
`-Wno-unused-comparison`	Comparissons in gTest *_No_THROW(...)
`-Wno-unused-value`			in gTest *_NO_THROW(...)


<!----------------------------------------------------------------><a id="cover"></a>
## Code Coverage ##
<!---------------------------------------------------------------->
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
