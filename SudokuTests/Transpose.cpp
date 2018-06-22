//====---- SudokuTests/Transpose.cpp                                  ----====//
//
// Unit tests for the transpose functions.
//====--------------------------------------------------------------------====//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Transpose.h>
// Helpers

// additional
#include <type_traits>


namespace SudokuTests::Transpose
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Location_Block;

using ::Sudoku::transpose_row_col;
using ::Sudoku::transpose_row_block;

static_assert(std::is_same_v<
			  Board<int, 2>,
			  decltype(transpose_row_col(Board<int, 2>()))>);
static_assert(std::is_same_v<
			  Board<int, 2>,
			  decltype(transpose_row_block(Board<int, 2>()))>);

TEST(Transpose, Instantiate)
{ // encapsulate the variable (Clang warning: missing-variable-declarations)
	const Board<int, 2> board_test{};
	static_assert(noexcept(transpose_row_col(Board<int, 2>())));
	static_assert(noexcept(transpose_row_col(board_test)));
	static_assert(noexcept(transpose_row_block(Board<int, 2>())));
	static_assert(noexcept(transpose_row_block(board_test)));
}

TEST(Transpose, row_col)
{
	using L = Location<2>;

	const Board<int, 2> row{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	static_assert(noexcept(transpose_row_col(row)));
	Board<int, 2> col = transpose_row_col(row);
	EXPECT_EQ(row[L{0}], col[L{0}]);
	EXPECT_EQ(row[L{1}], (col[L{1, 0}]));
	EXPECT_EQ(row[L{2}], (col[L{2, 0}]));
	EXPECT_EQ(row[L{3}], (col[L{3, 0}]));
	EXPECT_EQ((row[L{1, 0}]), (col[L{0, 1}]));
	EXPECT_EQ((row[L{1, 1}]), (col[L{1, 1}]));
	EXPECT_EQ((row[L{1, 2}]), (col[L{2, 1}]));
	EXPECT_EQ((row[L{1, 3}]), (col[L{3, 1}]));
	EXPECT_EQ((row[L{2, 0}]), (col[L{0, 2}]));
	EXPECT_EQ((row[L{2, 1}]), (col[L{1, 2}]));
	EXPECT_EQ((row[L{2, 2}]), (col[L{2, 2}]));
	EXPECT_EQ((row[L{2, 3}]), (col[L{3, 2}]));

	Board<int, 2> undo = transpose_row_col(col);
	EXPECT_EQ(undo, row);
}

TEST(Transpose, row_col3)
{
	using L = Location<3>;

	// for (int i{}; i < 500; ++i)
	{
		const Board<int, 3> row{
			0,  1,  2,  3,  4,  5,  6,  7,  8,  //
			9,  10, 11, 12, 13, 14, 15, 16, 17, //
			18, 19, 20, 21, 22, 23, 24, 25, 26, //
			27, 28, 29, 30, 31, 32, 33, 34, 35, //
			36, 37, 38, 39, 40, 41, 42, 43, 44, //
			45, 46, 47, 48, 49, 50, 51, 52, 53, //
			54, 55, 56, 57, 58, 59, 60, 61, 62, //
			63, 64, 65, 66, 67, 68, 69, 70, 71, //
			72, 73, 74, 75, 76, 77, 78, 79, 80  //
		};
		Board<int, 3> col = transpose_row_col(row);
		EXPECT_EQ(row[L{0}], col[L{0}]);
		EXPECT_EQ(row[L{1}], (col[L{1, 0}]));
		EXPECT_EQ(row[L{2}], (col[L{2, 0}]));
		EXPECT_EQ(row[L{3}], (col[L{3, 0}]));
		EXPECT_EQ((row[L{1, 0}]), (col[L{0, 1}]));
		EXPECT_EQ((row[L{1, 1}]), (col[L{1, 1}]));
		EXPECT_EQ((row[L{1, 2}]), (col[L{2, 1}]));
		EXPECT_EQ((row[L{1, 3}]), (col[L{3, 1}]));
		EXPECT_EQ((row[L{2, 0}]), (col[L{0, 2}]));
		EXPECT_EQ((row[L{2, 1}]), (col[L{1, 2}]));
		EXPECT_EQ((row[L{2, 2}]), (col[L{2, 2}]));
		EXPECT_EQ((row[L{2, 3}]), (col[L{3, 2}]));

		Board<int, 3> undo = transpose_row_col(col);
		EXPECT_EQ(undo, row);
	}
}

TEST(Transpose, row_block2)
{
	using L = Location<2>;
	using B = Location_Block<2>;

	const Board<int, 2> row{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	Board<int, 2> block = transpose_row_block(row);
	EXPECT_EQ(row[L{0}], (block[B{0, 0}]));
	EXPECT_EQ(row[L{1}], (block[B{0, 1}]));
	EXPECT_EQ(row[L{2}], (block[B{0, 2}]));
	EXPECT_EQ(row[L{3}], (block[B{0, 3}]));
	EXPECT_EQ((row[L{1, 0}]), (block[B{1, 0}]));
	EXPECT_EQ((row[L{1, 1}]), (block[B{1, 1}]));
	EXPECT_EQ((row[L{1, 2}]), (block[B{1, 2}]));
	EXPECT_EQ((row[L{1, 3}]), (block[B{1, 3}]));
	EXPECT_EQ((row[L{2, 0}]), (block[B{2, 0}]));
	EXPECT_EQ((row[L{2, 1}]), (block[B{2, 1}]));
	EXPECT_EQ((row[L{2, 2}]), (block[B{2, 2}]));
	EXPECT_EQ((row[L{2, 3}]), (block[B{2, 3}]));

	Board<int, 2> undo = transpose_row_block(block);
	EXPECT_EQ(undo, row);
}

TEST(Transpose, row_block3)
{
	using L = Location<3>;
	using B = Location_Block<3>;

	// for (int i{}; i < 500; ++i)
	{
		const Board<int, 3> row{
			0,  1,  2,  3,  4,  5,  6,  7,  8,  //
			9,  10, 11, 12, 13, 14, 15, 16, 17, //
			18, 19, 20, 21, 22, 23, 24, 25, 26, //
			27, 28, 29, 30, 31, 32, 33, 34, 35, //
			36, 37, 38, 39, 40, 41, 42, 43, 44, //
			45, 46, 47, 48, 49, 50, 51, 52, 53, //
			54, 55, 56, 57, 58, 59, 60, 61, 62, //
			63, 64, 65, 66, 67, 68, 69, 70, 71, //
			72, 73, 74, 75, 76, 77, 78, 79, 80  //
		};
		Board<int, 3> block = transpose_row_block(row);
		EXPECT_EQ(row[L{0}], (block[B{0, 0}]));
		EXPECT_EQ(row[L{1}], (block[B{0, 1}]));
		EXPECT_EQ(row[L{2}], (block[B{0, 2}]));
		EXPECT_EQ(row[L{3}], (block[B{0, 3}]));
		EXPECT_EQ(row[L{8}], (block[B{0, 2, 2}]));
		EXPECT_EQ((row[L{1, 0}]), (block[B{1, 0}]));
		EXPECT_EQ((row[L{1, 1}]), (block[B{1, 1}]));
		EXPECT_EQ((row[L{1, 2}]), (block[B{1, 2}]));
		EXPECT_EQ((row[L{1, 3}]), (block[B{1, 3}]));
		EXPECT_EQ((row[L{2, 0}]), (block[B{2, 0}]));
		EXPECT_EQ((row[L{2, 1}]), (block[B{2, 1}]));
		EXPECT_EQ((row[L{2, 2}]), (block[B{2, 2}]));
		EXPECT_EQ((row[L{2, 3}]), (block[B{2, 3}]));

		Board<int, 3> undo = transpose_row_block(block);
		EXPECT_EQ(undo, row);
	}
}

TEST(Transpose, wrong_order)
{
	using L = Location<3>;
	using B = Location_Block<3>;

	const Board<int, 3> row{
		0,  1,  2,  3,  4,  5,  6,  7,  8,  //
		9,  10, 11, 12, 13, 14, 15, 16, 17, //
		18, 19, 20, 21, 22, 23, 24, 25, 26, //
		27, 28, 29, 30, 31, 32, 33, 34, 35, //
		36, 37, 38, 39, 40, 41, 42, 43, 44, //
		45, 46, 47, 48, 49, 50, 51, 52, 53, //
		54, 55, 56, 57, 58, 59, 60, 61, 62, //
		63, 64, 65, 66, 67, 68, 69, 70, 71, //
		72, 73, 74, 75, 76, 77, 78, 79, 80  //
	};
	// first
	Board<int, 3> col = transpose_row_col(row);
	EXPECT_EQ((row[L{1, 3}]), (col[L{3, 1}]));

	// second
	Board<int, 3> block_col = transpose_row_block(col);
	EXPECT_EQ((col[L{1, 3}]), (block_col[B{1, 3}]));

	// Trying to undo first, while still in second configuration.
	Board<int, 3> block_row = transpose_row_col(block_col);
	// Trying to undo second, while in a new unique situation.
	Board<int, 3> row2 = transpose_row_block(block_row);
	// NOT EQUAL to starting situation.
	EXPECT_NE(row, row2);

	// Undo all to get back to start.
	Board<int, 3> row3 = transpose_row_block(
		transpose_row_col(transpose_row_block(transpose_row_col(row2))));
	EXPECT_EQ(row, row3);
}

TEST(Transpose, swap_row_col)
{
	using L = Location<3>;

	// for (int i{}; i < 500; ++i)
	{
		Board<int, 3> board{
			0,  1,  2,  3,  4,  5,  6,  7,  8,  //
			9,  10, 11, 12, 13, 14, 15, 16, 17, //
			18, 19, 20, 21, 22, 23, 24, 25, 26, //
			27, 28, 29, 30, 31, 32, 33, 34, 35, //
			36, 37, 38, 39, 40, 41, 42, 43, 44, //
			45, 46, 47, 48, 49, 50, 51, 52, 53, //
			54, 55, 56, 57, 58, 59, 60, 61, 62, //
			63, 64, 65, 66, 67, 68, 69, 70, 71, //
			72, 73, 74, 75, 76, 77, 78, 79, 80  //
		};
		board = transpose_row_col(std::move(board));
		EXPECT_EQ((board[L{1, 1}]), 10);
		EXPECT_EQ((board[L{1, 2}]), 19);
		EXPECT_EQ((board[L{1, 3}]), 28);
		EXPECT_EQ((board[L{7, 8}]), 79);
		EXPECT_EQ((board[L{7, 7}]), 70);
		board = transpose_row_col(std::move(board));
		EXPECT_EQ((board[L{1, 1}]), 10);
		EXPECT_EQ((board[L{2, 1}]), 19);
		EXPECT_EQ((board[L{3, 1}]), 28);
		EXPECT_EQ((board[L{8, 7}]), 79);
		EXPECT_EQ((board[L{7, 7}]), 70);
	}
}

TEST(Transpose, swap_row_block)
{
	using L = Location<3>;

	// for (int i{}; i < 500; ++i)
	{
		Board<int, 3> board{
			0,  1,  2,  3,  4,  5,  6,  7,  8,  //
			9,  10, 11, 12, 13, 14, 15, 16, 17, //
			18, 19, 20, 21, 22, 23, 24, 25, 26, //
			27, 28, 29, 30, 31, 32, 33, 34, 35, //
			36, 37, 38, 39, 40, 41, 42, 43, 44, //
			45, 46, 47, 48, 49, 50, 51, 52, 53, //
			54, 55, 56, 57, 58, 59, 60, 61, 62, //
			63, 64, 65, 66, 67, 68, 69, 70, 71, //
			72, 73, 74, 75, 76, 77, 78, 79, 80  //
		};
		board = transpose_row_block(std::move(board));
		EXPECT_EQ((board[L{0, 1}]), 1);
		EXPECT_EQ((board[L{1, 1}]), 4);
		EXPECT_EQ((board[L{2, 2}]), 8);
		EXPECT_EQ((board[L{4, 8}]), 50);
		EXPECT_EQ((board[L{5, 7}]), 52);
		EXPECT_EQ((board[L{8, 8}]), 80);
		board = transpose_row_block(std::move(board));
		EXPECT_EQ((board[L{0, 1}]), 1);
		EXPECT_EQ((board[L{1, 1}]), 10);
		EXPECT_EQ((board[L{2, 2}]), 20);
		EXPECT_EQ((board[L{4, 8}]), 44);
		EXPECT_EQ((board[L{5, 7}]), 52);
		EXPECT_EQ((board[L{8, 8}]), 80);
	}
}

namespace test_class
{
	class nomove
	{
		int a{};

	public:
		nomove()              = default;
		nomove(const nomove&) = default;
		nomove(nomove&& x) noexcept(false) { a = std::move(x.a); }
		nomove& operator=(const nomove& x) noexcept
		{
			a = x.a;
			return *this;
		}
		bool operator==(const nomove& x) const noexcept { return a == x.a; }
	};
	static_assert(std::is_copy_constructible_v<nomove>);
	static_assert(std::is_nothrow_copy_constructible_v<nomove>);
	static_assert(std::is_copy_assignable_v<nomove>);
	static_assert(std::is_move_constructible_v<nomove>);
	static_assert(!std::is_nothrow_move_constructible_v<nomove>);
	static_assert(std::is_move_assignable_v<nomove>);
	static_assert(std::is_swappable_v<nomove>);
	static_assert(!std::is_nothrow_swappable_v<nomove>);
} // namespace test_class

TEST(Transpose, throwing_move)
{
	using L = Location<3>;


	Board<test_class::nomove, 3> board{};
	EXPECT_EQ(board[L{8}], test_class::nomove());
	Board<test_class::nomove, 3> col = transpose_row_col(board);
	EXPECT_EQ(col[L{1}], test_class::nomove());
	Board<test_class::nomove, 3> block = transpose_row_block(board);
	EXPECT_EQ(block[L{3}], test_class::nomove());
}

} // namespace SudokuTests::Transpose
