<!----------------------------------------------------------------><a id="top"></a>
# Using the GoogleTest Unit-testing framework #
<!---------------------------------------------------------------->

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


<!---------------------------------------------------------------->
Catch debug assert() from <cassert> with:

    EXPECT_DEBUG_DEATH({ [statement]; }, "Assertion failed: .*");


[top](#top)
