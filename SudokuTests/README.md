<!----------------------------------------------------------------><a id="top"></a>
# Unit Testing for Sudoku.lib #
<!---------------------------------------------------------------->
Using the GoogleTest Unit-testing framework

[gTest repository](https://github.com/google/googletest/)


Testing Sudoku.lib
Console project called: SudokuTest(s)

<!----------------------------------------------------------------><a id="vs_settings"></a>
## Visual Studio Settings ##
<!---------------------------------------------------------------->
- Console Project, No precompiled headers
- Add reference to the project under test (context menu)
- Add: Configuration Properties > Linker > Input > Additional Dependencies:
	$(VcpkgRoot)debug\lib\manual-link\gtest.lib			debug-mode (both x86 and x64)
	$(VcpkgRoot)debug\lib\manual-link\gtest_main.lib		if no custom main()
	$(VcpkgRoot)lib\manual-link\gtest.lib				release-mode
	$(VcpkgRoot)lib\manual-link\gtest_main.lib				if no custom main()


### Compiler settings
**Dissabled:**
  **VC++:**
```
2017/04
/w14619			pragma warning: there is no warning number 'number'
```
**Clang:**
`-Wno-unused-comparison`	Comparissons in gTest *_No_THROW(...)
`-Wno-unused-value`			in gTest *_NO_THROW(...)

<!---------------------------------------------------------------->
Catch debug assert() from <cassert> with:

    EXPECT_DEBUG_DEATH({ [statement]; }, "Assertion failed: .*");


[top](#top)
