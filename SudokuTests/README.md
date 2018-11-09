<!-------------------------------------------------------------><a id="top"></a>
# Unit Testing for Sudoku #
<!----------------------------------------------------------------------------->
Unit-tests for the [Sudoku](../Sudoku) library.
Using the [Google Test](https://github.com/google/googletest/) Unit-testing
framework.
This file documents project specific properties used in addition to those
documented in [docs/Configuration.md](../docs/Configuration.md).
Most of these are related to the use of Google Test.

<!-- TOC -->
- [Visual Studio Setting](#vs_settings)
- [Compiler settings](#compiler)
- [Static Analysers](#analysers)
  - [Clang-Tidy](#tidy)
- [Code Coverage](#cover)
- [Notes](#notes)

<!-----------------------------------------------------><a id="vs_settings"></a>
## Visual Studio Settings
<!----------------------------------------------------------------------------->
Instructions to set-up a unit-testing project, in conjunction with
[vcpkg](https://github.com/Microsoft/vcpkg):
- Add reference to the project under test (context menu)
- Add: Configuration Properties > Linker > Input > Additional Dependencies:
  `$(VcpkgRoot)debug\lib\manual-link\gtestd.lib` // Debug-mode (note the 'd')
  `$(VcpkgRoot)lib\manual-link\gtest.lib`        // Release-mode
  ** disabled: **
  `$(VcpkgRoot)debug\lib\manual-link\gtest_maind.lib` // If no custom main()
  `$(VcpkgRoot)lib\manual-link\gtest_main.lib`        // If no custom main()
  *Note*: if gmock is used, `gmock.lib` contains all of `gtest.lib`,
  therefore only one of these can be included at any time.

<!--------------------------------------------------------><a id="compiler"></a>
## Compiler settings
<!----------------------------------------------------------------------------->
The preprocessor macro `fwkUnitTest` needs to be defined to disable the
`[[deprecated]]` attribute on code meant only for testing purposes.

**Clang:**
Disabled warnings:
``````
-Wno-c++98-compat
-Wno-c++98-compat-pedantic
-Wno-unused-comparison       Every comparrison check ...
-Wno-zero-as-null-pointer-constant
-Wno-global-constructors     gTest macro
-Wno-used-but-marked-unused  gTest macros
-Wno-covered-switch-default  gTest macro debug-death on: Board.cpp
``````
Promote to errors:
````````
-Werror=return-type
````````
#### Custom settings for precompiled.cpp
**VC++:**  
```
/wd4619   pragma warning: there is no warning number 'number'
          gTest, warning 4800 was removed in VS2007
          not working ... preprocessor macros...
```
**Clang:**
Disabled warnings:
````
-Wno-deprecated-declarations gTest uses deprecated POSIX names (i.e. chdir)
-Wno-undef
-Wno-language-extension-token
-Wno-missing-noreturn
-Wno-shift-sign-overflow
````
#### Custom settings per file
**Clang:**
Disabled warnings:  
Board.cpp & Board_Iterators.cpp
````
-Wno-unevaluated-expression  Assignment in constexpr
````

<!-------------------------------------------------------><a id="analysers"></a>
## Static Analysers ##
<!----------------------------------------------------------------------------->
<!------------------------------------------------------------><a id="tidy"></a>
### Clang-Tidy ###
<!----------------------------------------------------------------------------->
To reduce the errors exclusively generated in GTest additional checks have been
disabled for this project.
##### Disabled tests:
- `cppcoreguidelines-pro-type-vararg`
  EXPECT_EQ() and ASSERT_EQ(): do not call c-style vararg functions 
- `hicpp-vararg` == `cppcoreguidelines-pro-type-vararg`
- `cppcoreguidelines-owning-memory`
  Every TEST: initializing non-owner argument of type
  `internal::TestFactoryBase *` with a newly created `gsl::owner<>` 
- `cppcoreguidelines-special-member-functions`
  Every TEST: defines a copy constructor and a copy assignment operator but
  does not define a destructor, a move constructor or a move assignment
  operator.
- `hicpp-special-member-functions`
  == `cppcoreguidelines-special-member-functions`
- `cppcoreguidelines-pro-bounds-array-to-pointer-decay`
  `EXPECT_DEBUG_DEATH()`: do not implicitly decay an array into a pointer
  consider using gsl::array_view or an explicit cast instead
- `modernize-use-equals-delete`
  TEST: use '= delete' to prohibit calling of a special member function 
- `hicpp-use-equals-delete`
  == modernize-use-equals-delete
- `cert-err58-cpp`
  TEST: initialization of 'test_info_' with static storage duration may throw
  an exception that cannot be caught 


<!-----------------------------------------------------------><a id="cover"></a>
## Code Coverage ##
<!----------------------------------------------------------------------------->
Code coverage checks using OpenCppCoverage.
https://opencppcoverage.codeplex.com
Run by the PowerShell script: coverage.ps1

Generated report available in `SudokuTests\Coverage\[Debug]\index.html`


<!-----------------------------------------------------------><a id="notes"></a>
## Notes ##
<!----------------------------------------------------------------------------->
Catch debug `assert()` from `<cassert>` with:
```
    EXPECT_DEBUG_DEATH({ [statement]; }, "Assertion failed: .*");
```

----
[top](#top)
