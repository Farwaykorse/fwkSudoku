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
-gline-tables-only
NO
`-Wno-c++98-compat`
	6358
`-Wno-c++98-compat-pedantic`
	56	long long
		variadic macros
`-Wno-c++17-extensions`
	192	static_assert with no message is a C++17 extension
		use of the 'maybe_unused' attribute
`-Wno-deprecated-declarations`
	80	gtest; write, read, fopen, fdopen, freopen, strerror, strncpy, close, ...
`-Wno-covered-switch-default`
	46	in EXPECT_DEBUG_DEATH()
`-Wno-global-constructors`
	76
`-Wno-keyword-macro`
	25	keyword is hidden by macro definition (gsl, span, multi_span, string_span)
`-Wno-undef`
	872	not defined macro values
`-Wno-unused-comparison`
	78	Comparissons in gTest *_NO_THROW(...)
`-Wno-unused-value`
	4 	expression result unused	in gTest *_NO_THROW(...)
`-Wno-used-but-marked-unused`
	76	
`-Wno-zero-as-null-pointer-constant`
	429	2x per TEST(...)
`-fno-ms-compatibility`
`-fno-strict-aliasing`

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
