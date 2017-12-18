//===--- SudokuTests/SudokuTests.cpp                                    ---===//
//
// custom main()
//===----------------------------------------------------------------------===//

#include <gtest/gtest.h>

GTEST_API_ int main(int argc, char** argv)
{
	// std::cout << "Running main() from SudokuTests.cpp\n";
	testing::InitGoogleTest(&argc, argv); // parse commandline flags
	return RUN_ALL_TESTS();
}
