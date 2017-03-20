//	Test boards for unit-test usage
//
#pragma once
#include "stdafx.h"
#include "Board.h"
#include "Location.h"

#include <vector>

namespace Sudoku_Test::Test_Boards
{

// Basic value boards, not proper Sudoku's
// No use of iterators
namespace bN_fill_1
{
// Requirements: basic class Board<int,N>
template<int N>
void set_ElemNr(Sudoku::Board<int, N>& target)
{
	constexpr int elem{ N*N };
	for (int i = 0; i < target.full_size; ++i)
	{
		target.at(i) = i;// % elem;
	}
}

// Requirements: basic class Board<int,N>
template<int N>
void set_RowNr_X(Sudoku::Board<int, N>& target, int X)
{
	for (int row = 0; row < target.elem_size; ++row)
	{
		for (int col = 0; col < target.elem_size; ++col)
		{
			target.at(row, col) = row + X;
		}
	}
}
template<int N>
void set_RowNr(Sudoku::Board<int, N>& target) { set_RowNr_X(target, 0); }

// Requirements: class Board<int,N>::Inbetween
template<int N>
void set_ColNr_X(Sudoku::Board<int, N>& target, int X)
{
	for (int row = 0; row < target.elem_size; ++row)
	{
		for (int col = 0; col < target.elem_size; ++col)
		{
			target[row][col] = col + X;
		}
	}
}
template<int N>
void set_ColNr(Sudoku::Board<int, N>& target) { set_ColNr_X(target, 0); }

template<int N>
void set_BlockNr_X(Sudoku::Board<int, N>& target)
{
	for (int i = 0; i < target.full_size; ++i)
	{
		target.at(i) = Sudoku::Location<N>(i).block_elem();
	}
}

}	// namespace sN_fill

// 4x4 valid boards
namespace b2_valid
{
std::vector<int> full_1
{
	1,2,3,4,
	3,4,1,2,
	2,1,4,3,
	4,3,2,1
};

std::vector<int> two_by_two_1{ 0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
}	// namespace b2_valid

namespace b3_valid
{
	// requires unique
	[[maybe_unused]]
	static const std::vector<int> b1
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
	[[maybe_unused]]
	static const std::vector<int> b1a
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

	// can't solve
	[[maybe_unused]]
	const std::vector<int> b2
	{										//	 _ _ _ _ _ _ _ _ _ _ _ _
		0, 0, 5,	3, 0, 0,	0, 0, 0,	//	|	  5	| 3		|       |
		8, 0, 0,	0, 0, 0,	0, 2, 0,	//	| 8		|       |	2	|
		0, 7, 0,	0, 1, 0,	5, 0, 0,	//	|_ _7_ _|_ _1_ _|_5_ _ _|

		4, 0, 0,	0, 0, 5,	3, 0, 0,	//	| 4		|     5	| 3     |
		0, 1, 0,	0, 7, 0,	0, 0, 6,	//	|	1	|   7 	|  	  6	|
		0, 0, 3,	2, 0, 0,	0, 8, 0,	//	|_ _ _3_|_2_ _ _|_ _8_ _|

		0, 6, 0,	5, 0, 0,	0, 0, 9,	//	| 	6	| 5     |	  9		
		0, 0, 4,	0, 0, 0,	0, 3, 0,	//	|     4	|		|   3	|
		0, 0, 0,	0, 0, 9,	7, 0, 0		//	|_ _ _ _|_ _ _9_|_7_ _ _|
	};
	[[maybe_unused]]
	const std::vector<int> b2a
	{										//	 _ _ _ _ _ _ _ _ _ _ _ _
		0, 0, 5,	3, 0, 0,	0, 0, 1,	//	|     5	| 3    	|       | 0,8=1 random to trigger output
		8, 0, 0,	0, 5, 0,	0, 2, 0,	//	| 8    	|   5   |   2  	|
		0, 7, 0,	0, 1, 0,	5, 0, 0,	//	|_ _7_ _|_ _1_ _|_5_ _ _|

		4, 0, 0,	0, 0, 5,	3, 0, 0,	//	| 4    	|     5	| 3     |
		0, 1, 0,	0, 7, 3,	0, 0, 6,	//	|   1  	|   7 3	|     6	|
		0, 0, 3,	2, 0, 0,	0, 8, 0,	//	|_ _ _3_|_2_ _ _|_ _8_ _|

		0, 6, 0,	5, 0, 0,	0, 0, 9,	//	|   6  	| 5     |     9	|
		0, 0, 4,	0, 0, 0,	0, 3, 0,	//	|     4	|      	|   3  	|
		0, 0, 0,	0, 0, 9,	7, 0, 0		//	|_ _ _ _|_ _ _9_|_7_ _ _|
	};

	// source: en.wikipedia.org
	[[maybe_unused]]
	const std::vector<int> b3				
	{										//	 _ _ _ _ _ _ _ _ _ _ _ _
		5, 3, 0,	0, 7, 0,	0, 0, 0,	//	| 5 3   |   7   |       |
		6, 0, 0,	1, 9, 5,	0, 0, 0,	//	| 6     | 1 9 5 |       |
		0, 9, 8,	0, 0, 0,	0, 6, 0,	//	|_ _9_8_|_ _ _ _|_ _6_ _|

		8, 0, 0,	0, 6, 0,	0, 0, 3,	//	| 8     |   6   |     3 |
		4, 0, 0,	8, 0, 3,	0, 0, 1,	//	| 4     | 8   3 |     1 |
		7, 0, 0,	0, 2, 0,	0, 0, 6,	//	|_7_ _ _|_ _2_ _|_ _ _6_|

		0, 6, 0,	0, 0, 0,	2, 8, 0,	//	|   6   |       | 2 8   |
		0, 0, 0,	4, 1, 9,	0, 0, 5,	//	|       | 4 1 9 |     5 |
		0, 0, 0,	0, 8, 0,	0, 7, 9		//	|_ _ _ _|_ _8_ _|_ _7_9_|
	};
	[[maybe_unused]]
	const std::vector<int> b3a				// no unique
	{										//	 _ _ _ _ _ _ _ _ _ _ _ _
		5, 3, 4,	6, 7, 8,	9, 1, 2,	//	| 5 3 4 | 6 7 8	| 9 1 2 |
		6, 7, 2,	1, 9, 5,	3, 4, 8,	//	| 6 7 2 | 1 9 5 | 3 4 8	|
		1, 9, 8,	3, 4, 2,	5, 6, 7,	//	|_1_9_8_|_3_4_2_|_5_6_7_|

		8, 5, 9,	7, 6, 1,	4, 2, 3,	//	| 8 5 9	| 7 6 1	| 4 2 3 |
		4, 2, 6,	8, 5, 3,	7, 9, 1,	//	| 4 2 6	| 8 5 3	| 7 9 1	|
		7, 1, 3,	9, 2, 4,	8, 5, 6,	//	|_7_1_3_|_9_2_4_|_8_5_6_|

		9, 6, 1,	5, 3, 7,	2, 8, 4,	//	| 9 6 1	| 5 3 7 | 2 8 4	|
		2, 8, 7,	4, 1, 9,	6, 3, 5,	//	| 2 8 7	| 4 1 9	| 6 3 5	|
		3, 4, 5,	2, 8, 6,	1, 7, 9		//	|_3_4_5_|_2_8_6_|_1_7_9_|
	};

	[[maybe_unused]]
	const std::vector<int> b4
	{
		2, 0, 0,	0, 7, 0,	0, 3, 8,
		0, 0, 0,	0, 0, 6,	0, 7, 0,
		3, 0, 0,	0, 4, 0,	6, 0, 0,

		0, 0, 8,	0, 2, 0,	7, 0, 0,
		1, 0, 0,	0, 0, 0,	0, 0, 6,
		0, 0, 7,	0, 3, 0,	4, 0, 0,

		0, 0, 4,	0, 8, 0,	0, 0, 9,
		0, 6, 0,	4, 0, 0,	0, 0, 0,
		9, 1, 0,	0, 6, 0,	0, 0, 2
	};
	[[maybe_unused]]
	const std::vector<int> b4a			// no unique
	{
		2, 4, 6,	9, 7, 5,	1, 3, 8,
		5, 8, 9,	3, 1, 6,	2, 7, 4,
		3, 7, 1,	2, 4, 8,	6, 9, 5,

		4, 9, 8,	6, 2, 1,	7, 5, 3,
		1, 3, 2,	7, 5, 4,	9, 8, 6,
		6, 5, 7,	8, 3, 9,	4, 2, 1,

		7, 2, 4,	1, 8, 3,	5, 6, 9,
		8, 6, 5,	4, 9, 2,	3, 1, 7,
		9, 1, 3,	5, 6, 7,	8, 4, 2
	};

	// requires double_option, not unique
	[[maybe_unused]]
	const std::vector<int> b5
	{
		4, 0, 0,	0, 0, 0,	0, 3, 8,
		0, 0, 2,	0, 0, 4,	1, 0, 0,
		0, 0, 5,	3, 0, 0,	2, 4, 0,

		0, 7, 0,	6, 0, 9,	0, 0, 4,
		0, 2, 0,	0, 0, 0,	0, 7, 0,
		6, 0, 0,	7, 0, 3,	0, 9, 0,

		0, 5, 7,	0, 0, 8,	3, 0, 0,
		0, 0, 3,	9, 0, 0,	4, 0, 0,
		2, 4, 0,	0, 0, 0,	0, 0, 9
	};
	[[maybe_unused]]
	const std::vector<int> b5a
	{
		4, 6, 1,	5, 7, 2,	9, 3, 8,
		7, 3, 2,	8, 9, 4,	1, 5, 6,
		8, 9, 5,	3, 1, 6,	2, 4, 7,

		3, 7, 8,	6, 2, 9,	5, 1, 4,
		5, 2, 9,	4, 8, 1,	6, 7, 3,
		6, 1, 4,	7, 5, 3,	8, 9, 2,

		9, 5, 7,	2, 4, 8,	3, 6, 1,
		1, 8, 3,	9, 6, 7,	4, 2, 5,
		2, 4, 6,	1, 3, 5,	7, 8, 9
	};

	// requires multi_option (3), not unique
	[[maybe_unused]]
	const std::vector<int> b6
	{
		0, 7, 0,	0, 0, 8,	0, 2, 9,
		0, 0, 2,	0, 0, 0,	0, 0, 4,
		8, 5, 4,	0, 2, 0,	0, 0, 0,

		0, 0, 8,	3, 7, 4,	2, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 3,	2, 6, 1,	7, 0, 0,

		0, 0, 0,	0, 9, 0,	6, 1, 2,
		2, 0, 0,	0, 0, 0,	4, 0, 0,
		1, 3, 0,	6, 0, 0,	0, 7, 0
	};
	[[maybe_unused]]
	const std::vector<int> b6a
	{
		6, 7, 1,	4, 3, 8,	5, 2, 9,
		3, 9, 2,	7, 1, 5,	8, 6, 4,
		8, 5, 4,	9, 2, 6,	1, 3, 7,

		5, 1, 8,	3, 7, 4,	2, 9, 6,
		7, 2, 6,	8, 5, 9,	3, 4, 1,
		9, 4, 3,	2, 6, 1,	7, 8, 5,

		4, 8, 7,	5, 9, 3,	6, 1, 2,
		2, 6, 9,	1, 8, 7,	4, 5, 3,	// (7,9)3 double_option
		1, 3, 5,	6, 4, 2,	9, 7, 8
	};

	// incomplete, require unique
	[[maybe_unused]]
	const std::vector<int> b7
	{
		0, 8, 0,	0, 9, 0,	0, 3, 0,
		0, 3, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 2,	0, 6, 0,	1, 0, 8,

		0, 2, 0,	8, 0, 0,	5, 0, 0,
		8, 0, 0,	9, 0, 0,	0, 0, 6,
		0, 0, 4,	0, 0, 0,	0, 7, 0,

		5, 0, 3,	0, 4, 0,	9, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 1, 0,
		0, 1, 0,	0, 5, 0,	0, 2, 0
	};
	[[maybe_unused]]
	const std::vector<int> b7a
	{
		0, 8, 0,	0, 9, 0,	0, 3, 0,
		0, 3, 0,	0, 0, 0,	0, 6, 9,
		0, 0, 2,	0, 6, 0,	1, 5, 8,

		0, 2, 0,	8, 0, 4,	5, 9, 0,
		8, 0, 0,	9, 0, 0,	0, 4, 6,
		0, 0, 4,	0, 0, 0,	8, 7, 0,

		5, 6, 3,	0, 4, 0,	9, 8, 7,
		2, 0, 0,	0, 0, 0,	0, 1, 5,
		0, 1, 0,	0, 5, 0,	0, 2, 0
	};

	[[maybe_unused]]
	const std::vector<int> easiest
	{
		0, 0, 0,	1, 0, 5,	0, 0, 0,
		1, 4, 0,	0, 0, 0,	6, 7, 0,
		0, 8, 0,	0, 0, 2,	4, 0, 0,

		0, 6, 3,	0, 7, 0,	0, 1, 0,
		9, 0, 0,	0, 0, 0,	0, 0, 3,
		0, 1, 0,	0, 9, 0,	5, 2, 0,

		0, 0, 7,	2, 0, 0,	0, 8, 0,
		0, 2, 6,	0, 0, 0,	0, 3, 5,
		0, 0, 0,	4, 0, 9,	0, 0, 0
	};
	[[maybe_unused]]
	const std::vector<int> gentle
	{
		0, 0, 0,	0, 0, 4,	0, 2, 8,
		4, 0, 6,	0, 0, 0,	0, 0, 5,
		1, 0, 0,	0, 3, 0,	6, 0, 0,

		0, 0, 0,	3, 0, 1,	0, 0, 0,
		0, 8, 7,	0, 0, 0,	1, 4, 0,
		0, 0, 0,	7, 0, 9,	0, 0, 0,

		0, 0, 2,	0, 1, 0,	0, 0, 3,
		9, 0, 0,	0, 0, 0,	5, 0, 7,
		6, 7, 0,	4, 0, 0,	0, 0, 0
	};
	[[maybe_unused]]
	const std::vector<int> moderate
	{
		4, 0, 0,	0, 1, 0,	0, 0, 0,
		0, 0, 0,	3, 0, 9,	0, 4, 0,
		0, 7, 0,	0, 0, 5,	0, 0, 9,

		0, 0, 0,	0, 6, 0,	0, 2, 1,
		0, 0, 4,	0, 7, 0,	6, 0, 0,
		1, 9, 0,	0, 5, 0,	0, 0, 0,

		9, 0, 0,	4, 0, 0,	0, 7, 0,
		0, 3, 0,	6, 0, 8,	0, 0, 0,
		0, 0, 0,	0, 3, 0,	0, 0, 6
	};
	[[maybe_unused]]
	const std::vector<int> tough
	{
		3, 0, 9,	0, 0, 0,	4, 0, 0,
		2, 0, 0,	7, 0, 9,	0, 0, 0,
		0, 2, 7,	0, 0, 0,	0, 0, 0,

		7, 5, 0,	0, 6, 0,	2, 3, 0,
		6, 0, 0,	9, 0, 4,	0, 0, 8,
		0, 2, 8,	0, 5, 0,	0, 4, 1,

		0, 0, 0,	0, 0, 0,	5, 9, 0,
		0, 0, 0,	1, 0, 6,	0, 0, 7,
		0, 0, 6,	0, 0, 0,	1, 0, 4
	};

	[[maybe_unused]]
	const std::vector<int> empty
	{
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,

		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,

		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 0, 0,	0, 0, 0
	};

}


}	// namespace Sudoku_Test::Test_Boards