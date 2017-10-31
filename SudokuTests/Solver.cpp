﻿//===--	SudokuTests/Solver.cpp											--===//
//
//	Unit tests for the template class Sudoku::Solver
//===---------------------------------------------------------------------===//
//	Implemented with GoogleTest
//
//	Notes:
//	gTest is limited for use with multiple template parameters.
//	These expressions need to be implemented between extra parentheses
//	- test elements implementing this are flagged with [gTest]
//	- not implemented tests are flagged as NEEDTEST [gTest]
//	gTest tries to print iterators if they use inheritance,
//		if used in *_EQ/NE etc.
//		use an explicit test like EXPECT_TRUE(.. == ..).
//
//===---------------------------------------------------------------------===//
#include <gtest/gtest.h>

// Class under test
#include "../Sudoku/Solver.h"
#include "../Sudoku/Solvers_Appearance.h"
// helpers
#include "../Sudoku/Board.h"
#include "../Sudoku/Location.h"
#include "../Sudoku/Location_Utilities.h"
#include "../Sudoku/Options.h"
// library
#include <bitset>
#include <vector>
#include <type_traits>


using namespace Sudoku;

namespace SudokuTests::SolversTest
{
namespace compiletime
{
	// try initializing a board in different ways
	// will trigger compile-time errors

	// Type properties
	//===-----------------------------------------------------------------===//
	using typeT = Solver<3>;
	static_assert(std::is_class_v<typeT>, "class, hiding datarepresentation");
	static_assert(!std::is_trivial_v<typeT>, "is trivial");
	//	default constructors & trivially copyable
	//! different between VC++ / Clang
	// static_assert(!std::is_trivially_copyable_v<typeT>, "trivial copy");
	//	compatible with std::memcpy & binary copy from/to files
	static_assert(!std::is_standard_layout_v<typeT>, "standard layout");
	//	can be converted with reinterpret_cast
	static_assert(!std::is_pod_v<typeT>, "++ Plain Old Data");
	//	both trivial and standard - layout, C compatible
	// static_assert(std::has_unique_object_representations_v<typeT>, "");
	//	C++17	trivially_copyable same object representation
	static_assert(!std::is_empty_v<typeT>, "empty");
	//	class with no datamembers & nothing virtual
	static_assert(!std::is_polymorphic_v<typeT>, "polymorphic");
	//	inherits atleast one virtual function");
	static_assert(!std::is_final_v<typeT>, "cannot be used as base class");
	static_assert(!std::is_abstract_v<typeT>, "abstract function");
	//	inherits or declares at least one pure virtual function

	// clang-format off
	// default constructor: typeT()
	static_assert(!std::is_default_constructible_v<typeT>, "default constr");
	static_assert(!std::is_nothrow_default_constructible_v<typeT>,
		"notrow default constructor");
	static_assert(!std::is_trivially_default_constructible_v<typeT>,
		"++ default, nothing virtual");

	// copy constructor: typeT(const typeT&)
	static_assert(std::is_copy_constructible_v<typeT>, "-- copy constructor");
	static_assert(std::is_nothrow_copy_constructible_v<typeT>,
		"-- notrow copy constructor");
	static_assert(std::is_trivially_copy_constructible_v<typeT>,
		"-- trivially copy constructor"); // = default

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>, "-- move constructor");
	static_assert(std::is_nothrow_move_constructible_v<typeT>,
		"-- nothrow move constructor");
	static_assert(std::is_trivially_move_constructible_v<typeT>,
		"-- trivially move constructor");

	// copy assingment
	static_assert(!std::is_copy_assignable_v<typeT>, "-- copy assignable");
	static_assert(!std::is_nothrow_copy_assignable_v<typeT>,
		"-- notrow copy assignable");
	static_assert(!std::is_trivially_copy_assignable_v<typeT>,
		"-- trivially copy assignable");

	static_assert(!std::is_move_assignable_v<typeT>, "-- move assignable");
	static_assert(!std::is_nothrow_move_assignable_v<typeT>,
		"-- move assignable");
	static_assert(!std::is_trivially_move_assignable_v<typeT>,
		"-- trivially move assignable");

	static_assert(std::is_destructible_v<typeT>, "-- destructable");
	static_assert(std::is_nothrow_destructible_v<typeT>,
		"-- nothrow destructable");
	static_assert(std::is_trivially_destructible_v<typeT>,
		"-- trivially destructable");
	static_assert(!std::has_virtual_destructor_v<typeT>,
		"-- virtual destructor");

	// C++17 swappable
	//TODO no Clang support
	//static_assert(!std::is_swappable_v<typeT>, "-- swappable");
	//static_assert(!std::is_nothrow_swappable_v<typeT>, "-- nothrow swappable");
	//static_assert(!std::is_swappable_with_v<typeT, Options<9>>, "");
	//static_assert(!std::is_nothrow_swappable_with_v<typeT, Options<9>>, "");

	//===---------------------------------------------------------------------===//
	// is_constructible from different types
	// set to non-default value at initialization
	//===---------------------------------------------------------------------===//
	static_assert(!std::is_constructible_v<typeT, int>,
		"-- should not construct from int");
	static_assert(!std::is_constructible_v<typeT, unsigned int>,
		"-- construct from unsigned int");
	static_assert(!std::is_constructible_v<typeT, size_t>,
		"-- construct from size_t");
	static_assert(
		!std::is_constructible_v<typeT, std::initializer_list<int>>, "");
	static_assert(std::is_constructible_v<typeT, Board<Options<9>, 3>&>,
		"Constructor takes Board<Options>&");
	static_assert(!std::is_constructible_v<typeT, Board<Options<9>, 3>>,
		"Requires reference");
	static_assert(!std::is_constructible_v<typeT, Board<int>&>,
		"Don't accept non-Options Board!");
	static_assert(std::is_constructible_v<Solver<2>, Board<Options<4>, 2>&>,
		"Constructor takes Board<Options, 2>&");
	static_assert(std::is_constructible_v<Solver<4>, Board<Options<16>, 4>&>,
		"Constructor takes Board<Options, 4>&");
	// clang-format on

	static_assert(!std::is_assignable_v<typeT, Board<Options<9>, 3>>);
	static_assert(!std::is_assignable_v<typeT, Board<Options<9>, 3>&>);
	static_assert(!std::is_assignable_v<typeT, int>, "NOT assignable from int");
	static_assert(!std::is_assignable_v<typeT, std::initializer_list<int>>);
	static_assert(
		!std::is_assignable_v<typeT, std::initializer_list<Options<3>>>);

} // namespace compiletime
TEST(Solver, setValue)
{
	using L = Location<2>;
	// clang-format off
	//	4		1234	3		1234
	//	134		1234	1234	1234
	//	1234	1234	1234	1234
	//	1234	1234	1234	4
	// clang-format on

	// Set single value
	Board<Options<4>, 2> board;
	Solver<2> S1{board};
	ASSERT_EQ(board[1][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(S1.setValue(L(2), 3));
	EXPECT_EQ(board[0][2], 3);
	EXPECT_NO_THROW(S1.setValue(L(0), 4));
	EXPECT_EQ(board[0][0], 4);
	// test: already set to THIS answer (allow to overwrite with same)
	EXPECT_NO_THROW(S1.setValue(L(15), 4));
	ASSERT_EQ(board[3][3], 4);
	EXPECT_NO_THROW(S1.setValue(L(3, 3), 4));
	// test: value is not an option
	board[1][1] = std::bitset<5>{"11011"}; // options: 1,3,4
	EXPECT_THROW(S1.setValue(L(1, 1), 2), std::logic_error);
	// test: already set to another answer
	EXPECT_THROW(S1.setValue(L(0), 1), std::logic_error);

	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,2, 0,0,	// 1	2	3	4
		4,0, 0,0,	// 4	3	1,2	1,2
		0,1, 4,0,	// 2,3	1	4	2,3
		0,0, 0,0	// 2,3	4	1,2	1,2,
	}; // clang-format on
	// Copy data from vector
	Board<Options<4>, 2> B2;
	EXPECT_NO_THROW(Solver<2>(B2).setValue(v1.cbegin(), v1.cend()));
	EXPECT_EQ(B2[0][1], 2);
	EXPECT_EQ(B2[1][0], 4);
	EXPECT_EQ(B2[2][1], 1);
	EXPECT_EQ(B2[2][2], 4);
	// check if processed single option cells:
	EXPECT_EQ(B2[0][3], 4);
	EXPECT_EQ(B2[3][1], 4);
	// from vector didn't cascade over single option cells
	EXPECT_EQ(B2[0][0], 1);
	EXPECT_EQ(B2[0][2], 3);
	EXPECT_EQ(B2[1][1], 3);
	EXPECT_EQ(B2[3][1], 4);
}
TEST(Solvers_, appearance_once)
{
	using namespace Solvers_;
	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,0, 1,0,	// 
		1,0, 0,0,	// 
		0,1, 0,0,	// 
		0,0, 0,0	//					//	0	0	0	1
	}; // clang-format on
	Board<Options<4>, 2> B1;
	Options<4> result{};

	// Using iterators
	EXPECT_NO_THROW(Solver<2>(B1).setValue(v1.cbegin(), v1.cend()));
	EXPECT_EQ(B1[3][3].count(), 4);
	EXPECT_TRUE(B1[3][3].is_option(1));
	// on row
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.row(3).cbegin(), B1.row(3).cend()));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// on col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.col(3).cbegin(), B1.col(3).cend()));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// on block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.block(3).cbegin(), B1.block(3).cend()));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// nothing
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(
		result = appearance_once<2>(B1.row(0).cbegin(), B1.row(0).cend()));
	EXPECT_EQ(result.count_all(), 0);
	EXPECT_FALSE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed

	// Using Sections
	// Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(3)));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// Col
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.col(3)));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// Block
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.block(3)));
	EXPECT_EQ(result.count_all(), 1);
	EXPECT_TRUE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	// nothing, Row
	result.clear(); // reset
	ASSERT_TRUE(result.is_empty());
	EXPECT_NO_THROW(result = appearance_once<2>(B1.row(0)));
	EXPECT_EQ(result.count_all(), 0);
	EXPECT_FALSE(result.test(1));
	EXPECT_FALSE(result.is_answer()); // NOT needed
	//===-----------------------------------------------------------------===//
	// also in the test: appearance_sets
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	{
		Board<Options<4>, 2> B4{};
		const std::vector<int> v4{
			0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
		Solver<2> S4{B4};
		S4.setValue(v4.cbegin(), v4.cend());
		EXPECT_EQ(B4[0][0].count(), 3);
		EXPECT_EQ(B4[0][1].count(), 3);
		EXPECT_EQ(B4[0][2].count(), 3);
		EXPECT_EQ(B4[0][3].count(), 4);
		EXPECT_EQ(B4[2][1].count(), 4);
		auto result4 = appearance_once<2>(B4.row(0));
		EXPECT_TRUE(result4[0]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[1]);
		result4 = appearance_once<2>(B4.row(1));
		EXPECT_TRUE(result4[0]); // answer bit
		EXPECT_EQ(result4.count(), 0);
		EXPECT_TRUE(result4.is_empty());
		result4 = appearance_once<2>(B4.row(2));
		EXPECT_TRUE(result4[0]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[1]);
		result4 = appearance_once<2>(B4.col(3));
		EXPECT_TRUE(result4[0]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[1]);
		result4 = appearance_once<2>(B4.block(1));
		EXPECT_TRUE(result4[0]); // answer bit
		EXPECT_EQ(result4.count(), 1);
		EXPECT_TRUE(result4[1]);
	}
}
TEST(Solvers_, appearance_sets)
{
	using namespace Solvers_;

	// Example as shown in implementation
	Board<Options<9>, 3> board{};
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][1] = std::bitset<10>{"1101000101"};
	board[0][2] = std::bitset<10>{"0100000111"};
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][6] = std::bitset<10>{"1011001101"};
	board[0][7] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"};
	const Options<9> Ans_0{std::bitset<10>{"0000000001"}}; // complete, 1 answer
	const Options<9> Ans_1{std::bitset<10>{"0000100001"}}; // 5 and 1 answer
	const Options<9> Ans_2{std::bitset<10>{"0010000001"}}; // 7, <2 answers
	const Options<9> Ans_3{std::bitset<10>{"0100001001"}}; // 3 8, <3 answers

	// Using iterators
	{
		auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// reorder, should give same result
	// set answer last
	board[0][8] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][5] = std::bitset<10>{"0100001011"};
	{
		auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<10>{"1001100101"};
	board[0][7] = std::bitset<10>{"0000010000"}; // ans 4
	{
		auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set value appearing 3 times last
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][7] = std::bitset<10>{"1101000101"}; // 1 -> 7
	board[0][6] = std::bitset<10>{"0100000111"}; // 2 -> 6
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][2] = std::bitset<10>{"1011001101"};
	board[0][1] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"}; // 8 -> 8
	{
		auto result =
			appearance_sets<3>(board.row(0).cbegin(), board.row(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// Using Sections
	board[0][0] = std::bitset<10>{"1000000011"};
	board[0][1] = std::bitset<10>{"1101000101"};
	board[0][2] = std::bitset<10>{"0100000111"};
	board[0][3] = std::bitset<10>{"0000000111"};
	board[0][4] = std::bitset<10>{"1011001111"};
	board[0][5] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][6] = std::bitset<10>{"1011001101"};
	board[0][7] = std::bitset<10>{"1001100101"};
	board[0][8] = std::bitset<10>{"0100001011"};
	{
		auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// reorder, should give same result
	// set answer last
	board[0][8] = std::bitset<10>{"0000010000"}; // ans 4
	board[0][5] = std::bitset<10>{"0100001011"};
	{
		auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	// set unique value last
	board[0][8] = std::bitset<10>{"1001100101"};
	board[0][7] = std::bitset<10>{"0000010000"}; // ans 4
	{
		auto result = appearance_sets<3>(board.row(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===-----------------------------------------------------------------===//
	// Col
	// Example as shown in implementation
	Board<Options<9>, 3> B2{};
	B2[0][0] = std::bitset<10>{"1000000011"};
	B2[1][0] = std::bitset<10>{"1101000101"};
	B2[2][0] = std::bitset<10>{"0100000111"};
	B2[3][0] = std::bitset<10>{"0000000111"};
	B2[4][0] = std::bitset<10>{"1011001111"};
	B2[5][0] = std::bitset<10>{"0000010000"}; // ans 4
	B2[6][0] = std::bitset<10>{"1011001101"};
	B2[7][0] = std::bitset<10>{"1001100101"};
	B2[8][0] = std::bitset<10>{"0100001011"};
	{ // Using iterators
		auto result = appearance_sets<3>(B2.col(0).cbegin(), B2.col(0).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	{ // using Col
		auto result = appearance_sets<3>(B2.col(0));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===-----------------------------------------------------------------===//
	// Block
	using B = Location_Block<3>;
	Board<Options<9>, 3> B3{};
	B3[B{2, 0}] = std::bitset<10>{"1000000011"};
	B3[B(2, 1)] = std::bitset<10>{"1101000101"};
	B3[B(2, 2)] = std::bitset<10>{"0100000111"};
	B3[B(2, 3)] = std::bitset<10>{"0000000111"};
	B3[B(2, 4)] = std::bitset<10>{"1011001111"};
	B3[B(2, 5)] = std::bitset<10>{"0000010000"}; // ans 4
	B3[B(2, 6)] = std::bitset<10>{"1011001101"};
	B3[B(2, 7)] = std::bitset<10>{"1001100101"};
	B3[B(2, 8)] = std::bitset<10>{"0100001011"};
	{ // using iterators
		auto result =
			appearance_sets<3>(B3.block(2).cbegin(), B3.block(2).cend());
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	{ // using Block
		auto result = appearance_sets<3>(B3.block(2));
		EXPECT_EQ(result[0], Ans_0);
		EXPECT_EQ(result[1], Ans_1);
		EXPECT_EQ(result[2], Ans_2);
		EXPECT_EQ(result[3], Ans_3);
	}
	//===-----------------------------------------------------------------===//
	// also in the test: appearance_once
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	{
		Board<Options<4>, 2> B4{};
		const std::vector<int> v4{
			0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
		Solver<2> S4{B4};
		S4.setValue(v4.cbegin(), v4.cend());
		EXPECT_EQ(B4[0][0].count(), 3);
		EXPECT_EQ(B4[0][1].count(), 3);
		EXPECT_EQ(B4[0][2].count(), 3);
		EXPECT_EQ(B4[0][3].count(), 4);
		EXPECT_EQ(B4[2][1].count(), 4);
		auto result = appearance_sets<2>(B4.row(0));
		EXPECT_EQ(result.size(), 3); // max = N
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_TRUE(result[1][0]); // answer bit
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][1]);
		EXPECT_EQ(result[2].count(), 0);
		result = appearance_sets<2>(B4.row(1));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 0);
		EXPECT_TRUE(result[1].is_empty());
		EXPECT_TRUE(result[2].is_empty());
		result = appearance_sets<2>(B4.row(2));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][1]);
		result = appearance_sets<2>(B4.col(3));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][1]);
		result = appearance_sets<2>(B4.block(1));
		EXPECT_TRUE(result[0].is_empty());
		EXPECT_EQ(result[1].count(), 1);
		EXPECT_TRUE(result[1][1]);
	}
}
TEST(Solver, find_locations)
{
	using set = std::bitset<5>;
	using loc = Location<2>;
	Board<Options<4>, 2> B{};
	B[0][0] = set{"00100"}; // ans 2
	B[0][1] = set{"11011"};
	B[0][2] = set{"10011"};
	B[0][3] = set{"11011"};
	B[1][0] = set{"01011"};
	B[1][1] = set{"11001"};
	B[2][0] = set{"11011"};
	B[3][0] = set{"10011"};
	std::vector<loc> list{};
	// row/col/block
	EXPECT_NO_FATAL_FAILURE(list = Solver<2>(B).find_locations(B.row(0), 3, 1));
	EXPECT_EQ(list.size(), 3);
	EXPECT_EQ(list[0], loc(1));
	EXPECT_EQ(list[1], loc(2));
	EXPECT_EQ(list[2], loc(3));
	EXPECT_NO_FATAL_FAILURE(list = Solver<2>(B).find_locations(B.col(0), 3, 1));
	EXPECT_EQ(list.size(), 3);
	EXPECT_EQ(list[0], loc(1, 0));
	EXPECT_EQ(list[1], loc(2, 0));
	EXPECT_EQ(list[2], loc(3, 0));
	EXPECT_NO_FATAL_FAILURE(
		list = Solver<2>(B).find_locations(B.block(0), 2, 1));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[0], loc(0, 1));
	EXPECT_EQ(list[1], loc(1, 0));
	// incorrect rep_count: too low
	EXPECT_NO_FATAL_FAILURE(list = Solver<2>(B).find_locations(B.row(0), 2, 1));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[1], loc(2));
	// partial section
	EXPECT_NO_FATAL_FAILURE(
		list = Solver<2>(B).find_locations(
			B.row(0).cbegin() + 2, B.row(0).cend(), 2, 1));
	EXPECT_EQ(list.size(), 2);
	EXPECT_EQ(list[0], loc(2));
	EXPECT_EQ(list[1], loc(3));

	// find Options
	Options<4> value{set{"10011"}};
	EXPECT_NO_FATAL_FAILURE(
		list = Solver<2>(B).find_locations(B.row(0), value));
	EXPECT_EQ(list.size(), 1);
	EXPECT_EQ(list[0], loc(2));
}
TEST(Solver, set_section_locals)
{
	// called by: section_exclusive
	//	find and process values appearing 2 or 3x in row/col

	// int set_section_locals(section, rep_count, worker)
	using set = std::bitset<5>;
	using S   = Solver<2>;
	Board<Options<4>, 2> B{};
	Board<Options<4>, 2> cB{B};
	//
	//	12		1234	12		1234
	//	1234	1234	1234	1234
	//	34		34		1234	1234
	//	1234	1234	1234	1234
	//
	// Row
	//	2 waarden in rij, niet inzelfde block
	B[0][0] = set{"00111"};
	B[0][2] = set{"00111"};
	Options<4> worker{set{"11001"}};
	ASSERT_TRUE(B[0][1].all());
	ASSERT_TRUE(B[0][3].all());
	EXPECT_EQ(S(B).set_section_locals(B.row(0), 2, worker), 0);
	EXPECT_EQ(B[0][0].count(), 2);
	EXPECT_TRUE(B[0][1].all());
	EXPECT_EQ(B[0][2].count(), 2);
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][0].all());
	EXPECT_TRUE(B[1][1].all());
	//	2 waarden in rij, wel inzelfde block
	B[2][0] = set{"11001"};
	B[2][1] = set{"11001"};
	worker  = set{"00111"};
	ASSERT_TRUE(B[2][2].all());
	ASSERT_TRUE(B[2][3].all());
	ASSERT_TRUE(B[3][2].all());
	EXPECT_EQ(S(B).set_section_locals(B.row(2), 2, worker), 4);
	EXPECT_TRUE(B[3][0].all());
	EXPECT_EQ(B[2][2].count(), 4); // self
	EXPECT_EQ(B[2][3].count(), 4); // self
	EXPECT_EQ(B[3][2].count(), 2); // rest block
	EXPECT_EQ(B[1][2].count(), 4);
	// Col
	//
	//	12		1234	1234	1234
	//	1234	1234	1234	1234
	//	12		1234	1234	34
	//	1234	1234	1234	34
	//
	B       = cB; // reset board
	B[0][0] = set{"00111"};
	B[2][0] = set{"00111"};
	worker  = set{"11001"};
	EXPECT_EQ(S(B).set_section_locals(B.col(0), 2, worker), 0);
	//
	B[2][3] = set{"11001"};
	B[3][3] = set{"11001"};
	worker  = set{"00111"};
	EXPECT_EQ(S(B).set_section_locals(B.row(2), 2, worker), 4);
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_TRUE(B[0][1].all());
	// Block
	//
	//	12		1234	1234	1234
	//	1234	12		1234	1234
	//	34		34		1234	1234
	//	1234	1234	1234	1234
	//
	B       = cB; // reset board
	B[0][0] = set{"00111"};
	B[1][1] = set{"00111"};
	worker  = set{"11001"};
	EXPECT_EQ(S(B).set_section_locals(B.block(0), 2, worker), 0);
	EXPECT_TRUE(B[0][1].all());
	EXPECT_TRUE(B[0][3].all());
	EXPECT_TRUE(B[1][0].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_TRUE(B[3][0].all());
	// same block, same row
	B[2][0] = set{"11001"};
	B[2][1] = set{"11001"};
	worker  = set{"00111"};
	EXPECT_EQ(S(B).set_section_locals(B.block(2), 2, worker), 4);
	EXPECT_TRUE(B[3][0].all());    // self block
	EXPECT_TRUE(B[3][1].all());    // self block
	EXPECT_EQ(B[3][2].count(), 2); // rest row
	EXPECT_EQ(B[3][3].count(), 2); // rest row
	EXPECT_TRUE(B[0][1].all());    // rest col
	EXPECT_TRUE(B[0][2].all());
	EXPECT_TRUE(B[0][3].all());
	// same block, same col
	B       = cB; // reset board
	B[0][1] = set{"00111"};
	B[1][1] = set{"00111"};
	worker  = set{"11001"};
	EXPECT_EQ(S(B).set_section_locals(B.block(0), 2, worker), 4);
	EXPECT_TRUE(B[0][0].all());    // self block
	EXPECT_EQ(B[0][1].count(), 2); // self block
	EXPECT_TRUE(B[0][2].all());    // rest row
	EXPECT_TRUE(B[0][3].all());    // rest row
	EXPECT_TRUE(B[1][0].all());    // self block
	EXPECT_EQ(B[1][1].count(), 2); // self block
	EXPECT_TRUE(B[1][2].all());
	EXPECT_TRUE(B[1][3].all());
	EXPECT_EQ(B[2][0].count(), 2); // rest col
	EXPECT_TRUE(B[2][1].all());
	EXPECT_TRUE(B[2][2].all());
	EXPECT_TRUE(B[2][3].all());
	EXPECT_EQ(B[3][0].count(), 2); // rest col
	EXPECT_TRUE(B[3][1].all());
	EXPECT_TRUE(B[3][2].all());
	EXPECT_TRUE(B[3][3].all());
	Board<Options<9>, 3> B3{};
	const Board<Options<9>, 3> cB3{};
	Solver<3> S3{B3};
	Options<9> worker3{};
	// 3 option in same block & row
	//
	//	0	0	0
	//	4	5	6
	//	7	8	9
	//
	B3[1][0] = 4;
	B3[1][1] = 5;
	B3[1][2] = 6;
	B3[2][0] = 7;
	B3[2][1] = 8;
	B3[2][3] = 9;
	ASSERT_TRUE(B3[0][0].all());
	ASSERT_TRUE(B3[0][5].all());
	ASSERT_TRUE(B3[1][0].is_answer(4));
	ASSERT_TRUE(B3[1][8].all());
	worker3 = std::bitset<10>{"0000001111"};
	EXPECT_EQ(S3.set_section_locals(B3.block(0), 3, worker3), 3 * 6);
	EXPECT_EQ(B3[0][0].count(), 9);
	EXPECT_EQ(B3[0][3].count(), 6);
	EXPECT_EQ(B3[0][8].count(), 6);
	EXPECT_TRUE(B3[1][5].all());
	EXPECT_TRUE(B3[2][7].all());
	EXPECT_TRUE(B3[3][0].all());
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_TRUE(B3[3][2].all());
	EXPECT_TRUE(B3[8][2].all());
	EXPECT_TRUE(B3[5][5].all());
	// TODO 3 options in same block & col
	//
	//	0	2	3
	//	0	5	6
	//	0	8	9
	//
	B3       = cB3;
	B3[0][1] = 2;
	B3[0][2] = 3;
	B3[1][1] = 5;
	B3[1][2] = 6;
	B3[2][1] = 8;
	B3[2][3] = 9;
	worker3  = std::bitset<10>{"0010010011"};
	EXPECT_EQ(S3.set_section_locals(B3.block(0), 3, worker3), 3 * 6);
	EXPECT_EQ(B3[0][0].count(), 9);
	EXPECT_TRUE(B3[0][4].all()); // rest row
	EXPECT_TRUE(B3[0][8].all()); // rest row
	EXPECT_TRUE(B3[1][0].all()); // self
	EXPECT_TRUE(B3[1][1].is_answer(5));
	EXPECT_EQ(B3[3][0].count(), 6); // rest col
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_EQ(B3[4][0].count(), 6); // rest col
	EXPECT_EQ(B3[8][0].count(), 6); // rest col
	EXPECT_TRUE(B3[8][1].all());
	// 2 in same block & col, one in different col
	//
	//	0	2	3
	//	0	5	6
	//	7	0	9
	//
	B3       = cB3;
	B3[0][1] = 2;
	B3[0][2] = 3;
	B3[1][1] = 5;
	B3[1][2] = 6;
	B3[2][0] = 7;
	B3[2][3] = 9;
	worker3  = std::bitset<10>{"0100010011"};
	EXPECT_EQ(S3.set_section_locals(B3.block(0), 3, worker3), 0);
	EXPECT_TRUE(B3[0][0].all()); // self
	EXPECT_TRUE(B3[0][4].all()); // rest row
	EXPECT_TRUE(B3[0][8].all()); // rest row
	EXPECT_TRUE(B3[1][0].all()); // self
	EXPECT_TRUE(B3[1][1].is_answer(5));
	EXPECT_TRUE(B3[2][1].all()); // self
	EXPECT_TRUE(B3[3][0].all()); // rest col
	EXPECT_TRUE(B3[3][1].all());
	EXPECT_TRUE(B3[4][0].all()); // rest col
	EXPECT_TRUE(B3[8][0].all()); // rest col
	EXPECT_TRUE(B3[8][1].all());
}

TEST(Solver, set_uniques)
{
	// Solver<N>::set_uniques(const SectionT section, const Options& worker)
	//===-----------------------------------------------------------------===//
	// processing only one value per section.
	// also in the test: appearance_sets and appearance_once
	//
	//	0	0	0	0	234	234	234	1234
	//	1	0	0	0	1	234	234	234
	//	0	0	0	0	234	1234234	234
	//	0	0	1	0	234	234	1	234
	//
	Board<Options<4>, 2> B1{};
	const std::vector<int> v1{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
	Solver<2> S1{B1};
	S1.setValue(v1.cbegin(), v1.cend());
	const Board<Options<4>, 2> cB1{B1}; // to reset B1
	// verify setup
	ASSERT_EQ(B1[0][0].count(), 3);
	ASSERT_EQ(B1[0][1].count(), 3);
	ASSERT_EQ(B1[0][2].count(), 3);
	ASSERT_EQ(B1[0][3].count(), 4);
	ASSERT_EQ(B1[2][1].count(), 4);

	// worker empty -> return 0, no execution
	EXPECT_EQ(S1.set_uniques(B1.row(0), std::bitset<5>{"00001"}), 0);
	EXPECT_EQ(B1[0][0].count(), 3);
	EXPECT_EQ(B1[0][3].count(), 4);

	// Row (one value)
	auto worker = Solvers_::appearance_once<2>(B1.row(0));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_NO_FATAL_FAILURE(S1.set_uniques(B1.row(0), worker));
	EXPECT_EQ(B1[0][0].count(), 3); // no change
	EXPECT_EQ(B1[0][1].count(), 3); // no change
	EXPECT_EQ(B1[0][2].count(), 3); // no change
	EXPECT_TRUE(B1[0][3].is_answer(1));
	worker = Solvers_::appearance_once<2>(B1.row(2));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_EQ(S1.set_uniques(B1.row(2), worker), 1);
	EXPECT_TRUE(B1[2][1].is_answer(1));
	// Col (one value)
	B1 = cB1; // reset
	ASSERT_EQ(B1[0][3].count(), 4);
	ASSERT_EQ(B1[2][1].count(), 4);
	worker = Solvers_::appearance_once<2>(B1.col(1));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_EQ(S1.set_uniques(B1.col(1), worker), 1);
	EXPECT_EQ(B1[0][1].count(), 3); // no change
	EXPECT_EQ(B1[1][1].count(), 3); // no change
	EXPECT_TRUE(B1[2][1].is_answer(1));
	EXPECT_EQ(B1[3][1].count(), 3); // no change
	// Block (one value)
	B1 = cB1; // reset
	ASSERT_EQ(B1[2][1].count(), 4);
	worker = Solvers_::appearance_once<2>(B1.block(2));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"00011"}});
	EXPECT_EQ(S1.set_uniques(B1.block(2), worker), 1);
	EXPECT_EQ(B1[2][0].count(), 3); // no change
	EXPECT_TRUE(B1[2][1].is_answer(1));
	EXPECT_EQ(B1[3][0].count(), 3); // no change
	EXPECT_EQ(B1[3][1].count(), 3); // no change

	//===-----------------------------------------------------------------===//
	// multiple values in a single go
	// 12	24	324	24
	Board<Options<4>, 2> B2{};
	B2[0][0] = std::bitset<5>{"00111"};
	B2[0][1] = std::bitset<5>{"10101"};
	B2[0][2] = std::bitset<5>{"11101"};
	B2[0][3] = std::bitset<5>{"10101"};
	worker   = Solvers_::appearance_once<2>(B2.row(0));
	EXPECT_EQ(worker, Options<4>{std::bitset<5>{"01011"}});
	EXPECT_EQ(Solver<2>(B2).set_uniques(B2.row(0), worker), 10);
	// 10 = ans(0){1} + col(0){3} + block(0){1}
	//		ans(2){1} + col(2){3} + block(1){1}
	EXPECT_TRUE(B2[0][0].is_answer(1));
	EXPECT_TRUE(B2[0][2].is_answer(3));
	EXPECT_EQ(B2[0][1].count(), 2); // no change
	EXPECT_EQ(B2[0][3].count(), 2); // no change
	EXPECT_FALSE(B2[3][0].is_option(1));
}
TEST(Solver, unique_in_section)
{
	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,2, 0,0,	// 1	2	3	4
		4,0, 0,0,	// 4	3	1,2	1,2
		0,1, 4,0,	// 2,3	1	4	2,3
		0,0, 0,0	// 2,3	4	1,2	1,2,3
	}; // clang-format on
	Board<Options<4>, 2> B1;
	ASSERT_EQ(B1[0][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(Solver<2>(B1).setValue(v1.cbegin(), v1.cend()));
	const Board<Options<4>, 2> cB1{B1}; // copy to compare with
	EXPECT_EQ(B1[0][1], 2) << "setup error";
	EXPECT_EQ(B1[1][0], 4) << "setup error";
	EXPECT_EQ(B1[2][2], 4) << "setup error";
	EXPECT_EQ(B1, cB1) << "copy error";
	// single row
	EXPECT_NO_THROW(Solver<2>(B1).unique_in_section(B1.row(0)));
	EXPECT_EQ(B1, cB1) << "row 0 was completely fixed by setValue";

	// clang-format off
	const std::vector<int> v2
	{
		//start		// after setValue
		3,2, 0,0,	// 3	2	14	14		3	2	14	14
		0,0, 0,0,	// 14	14	3	2		14	14	3	2
		0,0, 2,0,	// 14	134	2	134		14	134	2	134
		0,0, 0,0	// 12.4	134	134	134		2	134	14	134
	}; // clang-format on

	Board<Options<4>, 2> B2{}; // working copy
	EXPECT_NO_THROW(Solver<2>(B2).setValue(v2.cbegin(), v2.cend()));
	const Board<Options<4>, 2> cB2{B2}; // copy to compare with
	// single row 0
	EXPECT_NO_THROW(Solver<2>(B2).unique_in_section(B2.row(0)));
	EXPECT_EQ(B2, cB2) << "row 0 was completely fixed by setValue";
	// single row 1
	EXPECT_NO_THROW(Solver<2>(B2).unique_in_section(B2.row(3)));
	EXPECT_NE(B2, cB2) << "row 3 should have changed";
	// full board
	for (int i = 0; i < elem_size<2>; ++i)
	{
		EXPECT_NO_THROW(Solver<2>(B2).unique_in_section(B2.row(i)));
	}
	EXPECT_EQ(B2[0][0], 3) << "input values are lost";
	EXPECT_EQ(B2[0][1], 2);
	EXPECT_EQ(B2[2][2], 2);
	EXPECT_EQ(B2[1][3], 2) << "not all uniques found in rows";
	EXPECT_EQ(B2[3][0], 2);

	// clang-format off
	const std::vector<int> v3
	{
		// start	// after setValue
		0,0, 1,0,	// 
		1,0, 0,0,	// 
		0,1, 0,0,	// 
		0,0, 0,0	//					//	0	0	0	1
	}; // clang-format on
	Board<Options<4>, 2> B3;
	// reset
	auto reset_B3 = [&]() {
		B3.clear();
		ASSERT_TRUE(B3[0][0].all());
		EXPECT_NO_THROW(Solver<2>(B3).setValue(v3.cbegin(), v3.cend()));
	};
	reset_B3();
	EXPECT_EQ(B3[3][3].count(), 4);
	EXPECT_TRUE(B3[3][3].is_option(1));
	// Row()
	EXPECT_NO_THROW(Solver<2>(B3).unique_in_section(B3.row(3)));
	EXPECT_TRUE(B3[3][3].is_answer(1));
	// Col()
	reset_B3();
	EXPECT_NO_THROW(Solver<2>(B3).unique_in_section(B3.col(3)));
	EXPECT_TRUE(B3[3][3].is_answer(1));
	// BLock()
	reset_B3();
	EXPECT_NO_THROW(Solver<2>(B3).unique_in_section(B3.block(3)));
	EXPECT_TRUE(B3[3][3].is_answer(1));
}

TEST(Solver, section_exclusive)
{
	// section_exclusive(SectionT)
	// section_exclusive(Block)
}

TEST(Solver, block_exclusive)
{
	// reproduce functionality of unique
	//===-----------------------------------------------------------------===//
	// clang-format off
	static const std::vector<int> V1
	{
		// start	// after setValue	// unique_block
		0,0, 1,0,	// 
		1,0, 0,0,	// 
		0,1, 0,0,	// 
		0,0, 0,0	//					//	0	0	0	1
	}; // clang-format on
	Board<Options<4>, 2> B1{};
	Solver<2>(B1).setValue(V1.cbegin(), V1.cend());
	// check input:
	{
		EXPECT_FALSE(B1[0][0].is_answer());
		EXPECT_FALSE(B1[0][1].is_answer());
		EXPECT_EQ(B1[0][2], 1);
		EXPECT_FALSE(B1[0][3].is_answer());
		EXPECT_EQ(B1[1][0], 1);
		EXPECT_FALSE(B1[1][1].is_answer());
		EXPECT_FALSE(B1[1][2].is_answer());
		EXPECT_FALSE(B1[1][3].is_answer());
		EXPECT_FALSE(B1[2][0].is_answer());
		EXPECT_EQ(B1[2][1], 1);
		EXPECT_FALSE(B1[2][2].is_answer());
		EXPECT_FALSE(B1[2][3].is_answer());
		EXPECT_FALSE(B1[3][0].is_answer());
		EXPECT_FALSE(B1[3][1].is_answer());
		EXPECT_FALSE(B1[3][2].is_answer());
		EXPECT_FALSE(B1[3][3].is_answer());
	}
	// block_exclusive:
	EXPECT_EQ(1, Solver<2>(B1).section_exclusive(B1.block(3)))
		<< "section_exclusive(Block) should find 1 value";
	EXPECT_EQ(B1[3][3], 1) << "section_exclusive(Block) unique value failed";
	int found1{0};
	for (int i{0}; i < elem_size<2>; ++i)
	{
		found1 += Solver<2>(B1).section_exclusive(B1.block(i));
	}
	EXPECT_EQ(found1, 0) << "shouldn't find any others";

	// TODO working for more than 1 unique (use set_section_locals)
	//===-----------------------------------------------------------------===//
	// clang-format off
	const std::vector<int> V2
	{
	/*	start board					answer board
	*	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
	*	|		| 		|   1 2 |	| 6 7 3	| 8 9 4	| 5 1 2 |
	*	|		|	3 5 |		|	| 9 1 2	| 7 3 5 | 4 8 6	|
	*	| _ _ _ | 6 _ _ | _ 7 _ |	|_8_4_5_|_6_1_2_|_9_7_3_|
	*	| 7		|		| 3		|	| 7 9 8	| 2 6 1	| 3 5 4 |
	*	|		| 4		| 8		|	| 5 2 6	| 4 7 3	| 8 9 1	|
	*	| 1 _ _ | _ _ _	| _ _ _ |	|_1_3_4_|_5_8_9_|_2_6_7_|
	*	| 		| 1 2	|		|	| 4 6 9	| 1 2 8 | 7 3 5	|
	*	|	8	|		|	4	|	| 2 8 7	| 3 5 6	| 1 4 9	|
	*	|_ _5_ _|_ _ _ _|_6_ _ _|	|_3_5_1_|_9_4_7_|_6_2_8_|
	*/
		0, 0, 0,	0, 0, 0,	0, 1, 2,	//	
		0, 0, 0,	0, 3, 5,	0, 0, 0,	//	. . .	2 . .
		0, 0, 0,	6, 0, 0,	0, 7, 0,	//	. . .	. 1 12	0 . [3]

		7, 0, 0,	0, 0, 0,	3, 0, 0,	//
		0, 0, 0,	4, 0, 0,	8, 0, 0,	//
		1, 0, 0,	0, 0, 0,	0, 0, 0,	//

		0, 0, 0,	1, 2, 0,	0, 0, 0,
		0, 8, 0,	0, 0, 0,	0, 4, 0,	//	
		0, 5, 0,	0, 0, 0,	6, 0, 0		//	. 5 [1] |	
	};
	// clang-format on
	Sudoku::Board<Options<9>, 3> B2{};
	Solver<3>(B2).setValue(V2.cbegin(), V2.cend());
	// block 0
	//		nothing
	// block 1
	//		double: 1 in row 2;	2 not paired
	// block 2
	//		unique: 8=3
	int count_s = Solver<3>(B2).section_exclusive(B2.block(2));

	EXPECT_GE(count_s, 1)
		<< "section_exclusive(block) should find at least 1 value";
	EXPECT_EQ(B2[2][8], 3)
		<< "section_exclusive(block) unique value failed N=3";
	for (int i = 0; i < elem_size<3>; ++i)
	{
		EXPECT_NO_THROW(Solver<3>(B2).section_exclusive(B2.block(i)));
	}
}

TEST(Solver, single_option)
{
	using L = Location<2>;
	Board<Options<4>, 2> B1;
	ASSERT_EQ(B1[1][0], Options<4>{}) << "incorrect instantiation";

	// is single option (loc, value)
	B1[0][1] = std::bitset<5>{"00101"}; // 2
	ASSERT_FALSE(B1[0][1].is_answer(2));
	ASSERT_TRUE(B1[0][1].is_option(2));
	EXPECT_EQ(B1[0][0].count(), 4);
	EXPECT_TRUE(B1[0][2].count() == 4 && B1[0][3].count() == 4);
	EXPECT_EQ(B1[1][1].count(), 4);
	EXPECT_TRUE(B1[2][1].count() == 4 && B1[3][1].count() == 4);
	EXPECT_NO_THROW(Solver<2>(B1).single_option(L(1), 2));
	EXPECT_EQ(B1[0][0].count(), 3);
	EXPECT_FALSE(B1[0][0].is_option(2));
	EXPECT_FALSE(B1[0][2].is_option(2) || B1[0][3].is_option(2));
	// is answer (loc, value)
	B1[1][0] = 4;
	ASSERT_TRUE(B1[1][0].is_answer(4));
	EXPECT_EQ(B1[1][2].count(), 4);
	EXPECT_EQ(B1[2][0].count(), 4);
	EXPECT_NO_THROW(Solver<2>(B1).single_option(L(1, 0), 4));
	EXPECT_EQ(B1[0][0].count(), 2);
	EXPECT_EQ(B1[1][2].count(), 3);
	// cascase
	B1[2][1] = 1;
	ASSERT_TRUE(B1[2][1].is_answer(1));
	EXPECT_EQ(B1[0][0].count(), 2);
	EXPECT_EQ(B1[1][1].count(), 2);
	EXPECT_NO_THROW(Solver<2>(B1).single_option(L(2, 1), 1));
	EXPECT_TRUE(B1[1][1].is_answer(3));
	EXPECT_TRUE(B1[0][0].is_answer(1));
	EXPECT_TRUE(B1[3][1].is_answer(4));

	// single_option(Location)
	Board<Options<4>, 2> B2;
	ASSERT_EQ(B2[1][0], Options<4>{}) << "incorrect instantiation";
	// more than 1 option available
	ASSERT_NO_THROW(Solver<2>(B2).single_option(L(1)));
	EXPECT_EQ(Solver<2>(B2).single_option(L(1)), 0);
	B2[0][1] = std::bitset<5>{"00101"}; // 2
	ASSERT_FALSE(B2[0][1].is_answer(2));
	ASSERT_TRUE(B2[0][1].is_option(2));
	EXPECT_EQ(B2[0][0].count(), 4);
	EXPECT_TRUE(B2[0][2].count() == 4 && B2[0][3].count() == 4);
	EXPECT_EQ(B2[1][1].count(), 4);
	EXPECT_TRUE(B2[2][1].count() == 4 && B2[3][1].count() == 4);
	EXPECT_EQ(Solver<2>(B2).single_option(L(1)), 7);
	EXPECT_EQ(B2[0][0].count(), 3);
	EXPECT_FALSE(B2[0][0].is_option(2));
	EXPECT_FALSE(B2[0][2].is_option(2) || B2[0][3].is_option(2));
	// is answer (loc, value)
	B2[1][0] = 4;
	ASSERT_TRUE(B2[1][0].is_answer(4));
	EXPECT_EQ(B2[1][2].count(), 4);
	EXPECT_EQ(B2[2][0].count(), 4);
	EXPECT_EQ(Solver<2>(B2).single_option(L(1, 0)), 6);
	EXPECT_EQ(B2[0][0].count(), 2);
	EXPECT_EQ(B2[1][2].count(), 3);
	// cascase
	B2[2][1] = 1;
	ASSERT_TRUE(B2[2][1].is_answer(1));
	EXPECT_EQ(B2[0][0].count(), 2);
	EXPECT_EQ(B2[1][1].count(), 2);
	EXPECT_EQ(Solver<2>(B2).single_option(L(2, 1)), 14);
	EXPECT_TRUE(B2[1][1].is_answer(3));
	EXPECT_TRUE(B2[0][0].is_answer(1));
	EXPECT_TRUE(B2[3][1].is_answer(4));

	// set_value, with single_option should give the same result
	Board<Options<4>, 2> B3;
	// clang-format off
	const std::vector<int> v1
	{
		// start	// after setValue
		0,2, 0,0,	// 1	2	34	34
		4,0, 0,0,	// 4	3	12	12
		0,1, 0,0,	// 23	1	234	234
		0,0, 0,0	// 23	4	123	123
	}; // clang-format on
	ASSERT_EQ(B3[1][0], Options<4>{}) << "incorrect instantiation";
	EXPECT_NO_THROW(Solver<2>(B3).setValue(v1.cbegin(), v1.cend()));
	EXPECT_EQ(B1, B3);
}
TEST(Solver, dual_option)
{
	// clang-format off
	/*	start board				
	*	 _ _ _ _ _ _ _ _ _ _ _ _
	*	|     3 |      	| - 1,2 |	test for row
	*	| 4 5 6 |       |       |
	*	|_7_8_9_|_ _ _ _|_ _ _ _|
	*	|   	|       |   	|
	*	|	    |   all |       |
	*	|_ _ _ _|_ _ _ _|_ _ _ _|
	*	| 		|    	| 1 2 3 |
	*	|	    |	    | 4 5   |test for col
	*	|_ _ _ _|_ _ _ _|_7_8_ _|
	*	before: 0,8: contains 1,2,4,5,6,7,8,9 (all except 3)
	*	after:  0,8: contains only 4,5,7,8
	*	after:	8,0: contains all except 4,7,8
	*	after:  block 4 contains all options in all cells
	*/
	const std::vector<int> b1
	{
		0, 0, 3,	0, 0, 0,	0, 0, 0,
		4, 5, 6,	0, 0, 0,	0, 0, 0,
		7, 8, 9,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	1, 2, 3,
		0, 0, 0,	0, 0, 0,	4, 5, 0,
		0, 0, 0,	0, 0, 0,	7, 8, 0
	}; // clang-format on
	Board<Options<9>, 3> B1;
	Solver<3> Run1(B1);
	EXPECT_NO_THROW(Run1.setValue(b1.cbegin(), b1.cend()))
		<< "setValue failed in copying from vector";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option before 1";
	EXPECT_EQ(B1[0][8].count(), 8) << "dual_option before 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option before 3";
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option before 4";
	EXPECT_NO_THROW(Run1.dual_option(Location<3>(0))) << "dual_option failed 1";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option 1"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6) << "dual_option 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option 3"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option 4"; // unchanged
	EXPECT_NO_THROW(Run1.dual_option(Location<3>(1))) << "dual_option failed 3";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option 5"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6) << "dual_option 6"; // unchanged
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option 7"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option 8"; // unchanged
	EXPECT_NO_THROW(Run1.dual_option(Location<3>(80))) << "dual_option failed3";
	EXPECT_EQ(B1[0][0].count(), 2) << "dual_option 9"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 4) << "dual_option 10";
	EXPECT_EQ(B1[4][4].count(), 9) << "dual_option 11"; // unchanged
	EXPECT_EQ(B1[8][0].count(), 6) << "dual_option 12"; // unchanged
}
TEST(Solver, multi_option)
{
	// clang-format off
	/*	start board
	*	 _ _ _ _ _ _ _ _ _ _ _ _
	*	|       |      	|       | test for row
	*	| 4 5 6 |       |       |
	*	|_7_8_9_|_ _ _ _|_ _ _ _|
	*	|   	|       |   	|
	*	|	    |   all |       |
	*	|_ _ _ _|_ _ _ _|_ _ _ _|
	*	| 		|    	| 1 2   |
	*	|	    |	    | 4 5   | test for col
	*	|_ _ _ _|_ _ _ _|_7_8_ _|
	*	before: 0,8: contains all
	*	after:  0,8: contains only 4,5,7,8 (not 1,2,3,6,9)
	*	no change:  block 4 contains all options in all cells
	*/
	using list = std::vector<unsigned int>;
	const std::vector<int> b1
	{
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		4, 5, 6,	0, 0, 0,	0, 0, 0,
		7, 8, 9,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	1, 2, 0,
		0, 0, 0,	0, 0, 0,	4, 5, 0,
		0, 0, 0,	0, 0, 0,	7, 8, 0
	}; // clang-format on
	Board<Options<9>, 3> B1;
	Solver<3> Run1(B1);
	EXPECT_NO_THROW(Run1.setValue(b1.cbegin(), b1.cend()))
		<< "setValue failed in copying from vector";
	EXPECT_EQ(B1[0][0].count(), 3) << "before 1";
	EXPECT_EQ(B1[0][8].count(), 9) << "before 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "before 3";
	EXPECT_EQ(B1[8][8].count(), 3) << "before 4";
	// run for row
	EXPECT_NO_THROW(Run1.multi_option(Location<3>(2)))
		<< "multi_option failed 1";
	EXPECT_EQ(B1[0][0].count(), 3) << "after 1"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 6) << "after 2";
	EXPECT_EQ(B1[4][4].count(), 9) << "after 3"; // unchanged
	EXPECT_EQ(B1[0][8].available(), (list{4, 5, 6, 7, 8, 9}))
		<< "after 4";
	// run for col
	EXPECT_NO_THROW(Run1.multi_option(Location<3>{6, 8}))
		<< "multi_option failed 2";
	EXPECT_EQ(B1[0][0].count(), 3) << "after 21"; // unchanged
	EXPECT_EQ(B1[8][8].count(), 3) << "after 22"; // unchanged
	EXPECT_EQ(B1[0][8].count(), 4) << "after 23";
	EXPECT_EQ(B1[4][4].count(), 9) << "after 24"; // unchanged
	EXPECT_EQ(B1[0][8].available(), (list{4, 5, 7, 8}))
		<< "after 25";

	// run for block
	// clang-format off
	const std::vector<int> b2
	{									 //  _ _ _ _ _ _ _ _ _ _ _ _
		0, 0, 3,	0, 0, 0,	7, 0, 0, // |     3 |       | 7     | diagonal: 1,5,9
		4, 0, 6,	2, 7, 0,	0, 0, 0, // | 4   6 | 2 7   |       |
		0, 8, 0,	0, 0, 0,	0, 0, 0, // |_ _8_ _|_ _ _ _|_ _ _ _|
		2, 0, 0,	0, 0, 0,	0, 0, 0, // | 2     |       |       |
		0, 0, 7,	0, 0, 0,	0, 0, 0, // |     7 |       |       |
		0, 0, 0,	0, 0, 0,	0, 0, 0, // |_ _ _ _|_ _ _ _|_ _ _ _|
		0, 0, 2,	0, 0, 0,	0, 0, 0, // |     2 |       |       |
		0, 0, 0,	0, 0, 0,	0, 0, 0, // |       |       |       | test for col
		0, 0, 0,	0, 0, 0,	0, 0, 0	 // |_ _ _ _|_ _ _ _|_ _ _ _|
	}; // clang-format on
	Board<Options<9>, 3> B2;
	Solver<3> Run2(B2);
	EXPECT_NO_THROW(Run2.setValue(b2.cbegin(), b2.cend()))
		<< "setValue failed in copying from vector 4";
	EXPECT_EQ(B2[0][0].count(), 3) << "before 31";
	EXPECT_EQ(B2[1][1].count(), 3) << "before 32";
	EXPECT_EQ(B2[2][2].count(), 3) << "before 33";
	EXPECT_EQ(B2[0][1].count(), 4) << "before 34";
	EXPECT_EQ(B2[2][0].count(), 4) << "before 35";
	EXPECT_EQ(B2[0][0].available(), (list{1, 5, 9})) << "before 36";
	EXPECT_EQ(B2[2][0].available(), (list{1, 5, 7, 9}))
		<< "before 37";
	EXPECT_EQ(B2[8][8].count(), 9) << "before 38";
	EXPECT_EQ(B2[2][5].count(), 6) << "before 39";
	EXPECT_EQ(B2[2][5].available(), (list{1, 3, 4, 5, 6, 9}))
		<< "before 310";
	EXPECT_NO_THROW(Run2.multi_option(Location<3>(0)))
		<< "multi_option failed 3";
	EXPECT_EQ(B2[0][0].count(), 3) << "after 31";
	EXPECT_EQ(B2[1][1].count(), 3) << "after 32";
	EXPECT_EQ(B2[2][2].count(), 3) << "after 33";
	EXPECT_TRUE(B2[0][1].is_answer(2)) << "after 34";
	EXPECT_TRUE(B2[2][0].is_answer(7)) << "after 35";
	EXPECT_EQ(B2[0][0].available(), (list{1, 5, 9})) << "after 36";
	EXPECT_EQ(B2[8][8].count(), 9) << "after 38";
	EXPECT_EQ(B2[2][5].count(), 6) << "after 39";
	EXPECT_EQ(B2[2][5].available(), (list{1, 3, 4, 5, 6, 9}))
		<< "after 310";

	// clang-format off
	// 9*9 partials forming a set: 3 cels containing (123,12,13)
	const std::vector<int> b3
	{									 //  _ _ _ _ _ _ _ _ _ _ _ _
		0, 0, 0,	0, 0, 0,	0, 0, 0, // |       |       |       | row: 1,2; 1,2,3; 1,2,3
		4, 5, 6,	0, 0, 0,	0, 0, 0, // | 4 5 6 |       |       |
		7, 8, 9,	0, 0, 0,	0, 0, 0, // |_7_8_9_|_ _ _ _|_ _ _ _|
		0, 0, 0,	0, 2, 3,	0, 0, 0, // | 3     |   2 3 |       |
		3, 0, 0,	4, 0, 6,	0, 0, 0, // |       | 4   6 |       |
		0, 0, 0,	7, 8, 0,	0, 0, 0, // |_ _ _ _|_7_8_ _|_ _ _ _|
		0, 0, 0,	0, 9, 0,	1, 2, 0, // |       |   9   | 1 2   |		 3,6
		0, 0, 0,	0, 3, 0,	4, 5, 0, // |       |   3 1 | 4 5   |	col: 6,9
		0, 0, 0,	0, 0, 0,	7, 8, 0  // |_ _ _ _|_ _ _5_|_7_8_ _|		 3,6,9
	}; // clang-format on
	Board<Options<9>, 3> B3;
	Solver<3> Run3(B3);
	EXPECT_NO_THROW(Run3.setValue(b3.cbegin(), b3.cend()))
		<< "setValue failed in copying from vector 3";
	EXPECT_EQ(B3[0][0].count(), 2) << "before 41";
	EXPECT_EQ(B3[0][1].count(), 3) << "before 42";
	EXPECT_EQ(B3[0][2].count(), 3) << "before 43";
	EXPECT_EQ(B3[0][3].count(), 7) << "before 44";
	// EXPECT_EQ(B3[5][1].count(), 6) << "before 45";
	EXPECT_EQ(B3[0][1].available(), (list{1, 2, 3})) << "before 46";
	EXPECT_EQ(B3[6][8].count(), 2) << "before 47";
	EXPECT_EQ(B3[7][8].count(), 2) << "before 48";
	EXPECT_EQ(B3[8][8].count(), 3) << "before 49";
	EXPECT_EQ(B3[8][8].available(), (list{3, 6, 9}))
		<< "before 410";
	EXPECT_NO_THROW(Run3.multi_option(Location<3>(1)))
		<< "multi_option failed 4";
	// row:
	EXPECT_EQ(B3[0][0].count(), 2) << "after 41"; // unchanged
	EXPECT_EQ(B3[0][1].count(), 3) << "after 42"; // unchanged
	EXPECT_EQ(B3[0][2].count(), 3) << "after 43"; // unchanged
	EXPECT_EQ(B3[0][3].count(), 4) << "after 44";
	// EXPECT_EQ(B3[5][1].count(), 6) << "after 45"; // unchanged
	EXPECT_EQ(B3[0][0].available(), (list{1, 2})) << "after 46";
	EXPECT_EQ(B3[0][1].available(), (list{1, 2, 3})) << "after 47";
	EXPECT_EQ(B3[0][3].available(), (list{5, 6, 8, 9}))
		<< "after 48";

	// TODO invalid value for count, should return 0
	// count = 1
	// count > elem_size / 2
}

TEST(Solver, solve_board)
{
	// clang-format off
	/*	start board					answer board
	*	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
	*	|		| 		|   1 2 |	| 6 7 3	| 8 9 4	| 5 1 2 |
	*	|		|	3 5 |		|	| 9 1 2	| 7 3 5 | 4 8 6	|
	*	| _ _ _ | 6 _ _ | _ 7 _ |	|_8_4_5_|_6_1_2_|_9_7_3_|
	*	| 7		|		| 3		|	| 7 9 8	| 2 6 1	| 3 5 4 |
	*	|		| 4		| 8		|	| 5 2 6	| 4 7 3	| 8 9 1	|
	*	| 1 _ _ | _ _ _	| _ _ _ |	|_1_3_4_|_5_8_9_|_2_6_7_|
	*	| 		| 1 2	|		|	| 4 6 9	| 1 2 8 | 7 3 5	|
	*	|	8	|		|	4	|	| 2 8 7	| 3 5 6	| 1 4 9	|
	*	|_ _5_ _|_ _ _ _|_6_ _ _|	|_3_5_1_|_9_4_7_|_6_2_8_|
	*/
	const std::vector<int> b1
	{
		0, 0, 0,	0, 0, 0,	0, 1, 2,
		0, 0, 0,	0, 3, 5,	0, 0, 0,
		0, 0, 0,	6, 0, 0,	0, 7, 0,
		7, 0, 0,	0, 0, 0,	3, 0, 0,
		0, 0, 0,	4, 0, 0,	8, 0, 0,
		1, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	1, 2, 0,	0, 0, 0,
		0, 8, 0,	0, 0, 0,	0, 4, 0,
		0, 5, 0,	0, 0, 0,	6, 0, 0
	};
	const std::vector<int> b1a
	{
		6, 7, 3,	8, 9, 4,	5, 1, 2,
		9, 1, 2,	7, 3, 5,	4, 8, 6,
		8, 4, 5,	6, 1, 2,	9, 7, 3,
		7, 9, 8,	2, 6, 1,	3, 5, 4,
		5, 2, 6,	4, 7, 3,	8, 9, 1,
		1, 3, 4,	5, 8, 9,	2, 6, 7,
		4, 6, 9,	1, 2, 8,	7, 3, 5,
		2, 8, 7,	3, 5, 6,	1, 4, 9,
		3, 5, 1,	9, 4, 7,	6, 2, 8
	};
	// clang-format on
	Board<int, 3> start;
	std::copy(b1.cbegin(), b1.cend(), start.begin());
	Board<int, 3> answer;
	std::copy(b1a.cbegin(), b1a.cend(), answer.begin());
	Board<Options<9>, 3> options{};
	Solver<3>(options).setValue(b1.cbegin(), b1.cend());
	for (int i = 0; i < elem_size<3>; ++i)
	{
		Solver<3>(options).unique_in_section(options.row(i));
	}
}
TEST(Solver, deathtest)
{
	using L = Location<2>;
	Board<Options<4>, 2> B{};
	Solver<2> S{B};

	// SetValue(Itr, Itr)
	const std::vector<int> v1(10);
	const std::vector<int> v2(18);
	// input too short / too long
	EXPECT_DEBUG_DEATH(S.setValue(v1.cbegin(), v1.cend()), "Assertion failed:");
	EXPECT_DEBUG_DEATH(S.setValue(v2.cbegin(), v2.cend()), "Assertion failed:");

	// set_uniques
	{
		Board<Options<4>, 2> B1{};
		// deathtest: worker[0] has to be true
		EXPECT_DEBUG_DEATH(
			Solver<2>(B1).set_uniques(B1.row(0), std::bitset<5>{"10110"}),
			"Assertion failed: worker.test.0.");
		// deathtest: an unique in worker doesn't exist in the section
		// 1	24	324	24
		B1[0][0] = std::bitset<5>{"00010"}; // ans 1
		EXPECT_FALSE(B1[0][0].is_option(1));
		B1[0][1]    = std::bitset<5>{"10101"};
		B1[0][2]    = std::bitset<5>{"11101"};
		B1[0][3]    = std::bitset<5>{"10101"};
		auto worker = std::bitset<5>{"01011"};
		EXPECT_DEBUG_DEATH(
			Solver<2>(B1).set_uniques(B1.row(0), worker),
			"Assertion failed: false");
	}
	// single_option()
#ifdef _DEBUG // note: release triggers normal exception
	// when wrong value
	B[1][2] = std::bitset<5>{"00011"}; // 1, not answer
	EXPECT_DEBUG_DEATH(
		S.single_option(L(1, 2), 4), "Assertion failed: .*test.*");
#endif // _DEBUG
	// when more than 1 option available
	B[1][2] = std::bitset<5>{"10011"}; // 1, 4
	EXPECT_DEBUG_DEATH(
		S.single_option(L(1, 2), 1), "Assertion failed: .*count_all.*");

	// find_locations()
	{
		using set = std::bitset<5>;
		using loc = Location<2>;
		Board<Options<4>, 2> B5{};
		B5[0][0] = set{"00100"}; // ans 2
		B5[0][1] = set{"11011"};
		B5[0][2] = set{"10011"};
		B5[0][3] = set{"11011"};
		B5[1][0] = set{"01011"};
		B5[1][1] = set{"11001"};
		B5[2][0] = set{"11011"};
		B5[3][0] = set{"10011"};
		std::vector<loc> list{};
		// rep_count = 0
		EXPECT_DEBUG_DEATH(
			Solver<2>(B5).find_locations(B5.row(0), 0, 3),
			"Assertion failed: rep_count > 0");
		// incorrect rep_count: too high
		EXPECT_DEBUG_DEATH(
			Solver<2>(B5).find_locations(B5.row(0), 3, 3),
			"Assertion failed: ");
		// section -> rep_count > elem_size
		EXPECT_DEBUG_DEATH(
			Solver<2>(B5).find_locations(B5.row(0), 5, 3),
			"Assertion failed: .* <= elem_size");
		EXPECT_DEBUG_DEATH(
			Solver<2>(B5).find_locations(
				B5.row(0).cbegin(), B5.row(0).cend(), 5, 3),
			"Assertion failed: ");
		// itr -> rep_count > full_size
		EXPECT_DEBUG_DEATH(
			Solver<2>(B5).find_locations(
				B5.row(0).cbegin(), B5.row(0).cend(), 17, 3),
			"Assertion failed: .* <= full_size");
	}
	// set_section_locals()
	{
		using S   = Solver<2>;
		using set = std::bitset<5>;
		// worker empty
		Options<4> worker{set{"00000"}};
		ASSERT_TRUE(worker.is_empty());
		EXPECT_DEBUG_DEATH(S(B).set_section_locals(B.row(0), 2, worker), "");
		EXPECT_DEBUG_DEATH(S(B).set_section_locals(B.block(0), 2, worker), "");
		// worker all set
		worker = set{"11111"};
		EXPECT_DEBUG_DEATH(
			S(B).set_section_locals(B.col(2), 4, worker), "rep_count <= N");
		EXPECT_DEBUG_DEATH(
			S(B).set_section_locals(B.block(2), 4, worker), "rep_count <= N");
		// 1 value
		worker = set{"00101"};
		EXPECT_DEBUG_DEATH(
			S(B).set_section_locals(B.col(2), 1, worker), "rep_count > 1");
		EXPECT_DEBUG_DEATH(
			S(B).set_section_locals(B.block(3), 1, worker), "rep_count > 1");
		// TODO locations.size() == rep_count
	}
	// set_section_locals(section, rep_count, worker)
	{
		// worker.count_all() = 0
		Options<4> worker{0};
		EXPECT_DEBUG_DEATH(S.set_section_locals(B.row(0), 1, worker), "count");
		EXPECT_DEBUG_DEATH(
			S.set_section_locals(B.block(0), 1, worker), "count");
		worker = std::bitset<5>{"00001"};
		EXPECT_DEBUG_DEATH(S.set_section_locals(B.col(0), 1, worker), "count");
		EXPECT_DEBUG_DEATH(
			S.set_section_locals(B.block(0), 1, worker), "count");
	}
}

} // namespace SudokuTests::SolversTest
