// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//===----------------------------------------------------------------------===//
// Sudoku.cpp : Defines the entry point for the console application.
//

#include "Console.h"

#include <Strategic/Solver.h>
#include <Sudoku/Size.h>
#include <Sudoku/Value.h>

#include <array>

#include <algorithm> // min
#include <chrono>
#include <iostream>


template<int N = 3, typename T = std::array<char, Sudoku::full_size<N>>>
static void test(const T& B_in, const T& A_in)
{
	using Value              = Sudoku::Value;
	constexpr auto elem_size = Sudoku::elem_size<N>;

	Sudoku::Board<Value, N> start;
	Sudoku::Board<Value, N> answer;
	std::transform(
		B_in.cbegin(), B_in.cend(), start.begin(), Sudoku::to_Value<N, char>);
	std::transform(
		A_in.cbegin(), A_in.cend(), answer.begin(), Sudoku::to_Value<N, char>);

#ifdef _DEBUG
	constexpr int repeat{1}; // runs (only keep fastest)
	constexpr int reruns{1}; // reruns (fastest are summed)
#else
	constexpr int repeat{1000}; // runs (only keep fastest)
	constexpr int reruns{10};   // reruns (fastest are summed)
#endif // _DEBUG
	using time     = std::chrono::time_point<std::chrono::steady_clock>;
	using duration = std::chrono::duration<int64_t, std::nano>;

	Sudoku::Board<Sudoku::Options<elem_size>, N> options{};
	{
		time t0{};
		time t1{};
		time t2{};
		duration tLoad{duration::max()};
		duration tSolver{duration::max()};
		duration tTotal{duration::max()};

		static int id{0};
		static duration tLoad_total{0};
		static duration tSolver_total{0};
		static duration tTotal_total{0};
		static duration tdiff{0};
		static bool last{};

		for (auto j{0}; j < reruns; ++j) //-V1008
		{
			for (auto i{0}; i < repeat; ++i) //-V1008
			{
				t0 = std::chrono::steady_clock::now();
				Sudoku::Board<Sudoku::Options<elem_size>, N> local{};
				set_Value(local, start.cbegin(), start.cend());
				t1 = std::chrono::steady_clock::now();
				Sudoku::test_solver_unique(local);
				t2 = std::chrono::steady_clock::now();

				// save fastest solving time
				tdiff   = t1 - t0;
				tLoad   = std::min(tLoad, tdiff);
				tdiff   = t2 - t0;
				last    = tTotal > tdiff;
				tTotal  = std::min(tTotal, tdiff);
				tSolver = std::min(tSolver, tTotal - tLoad);

				options = local;
			}
			if (last)
			{
				std::cout << "Last run was fastest " << j << ": true\n";
			}
			// sum fastest times
			tLoad_total += tLoad;
			tSolver_total += tSolver;
			tTotal_total += tTotal;
		}
		const auto time_in_microsec = [](duration t_in)
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(t_in)
				.count();
		};
		std::cout << "ID: " << id << "\n"
				  << "Load:\t" << time_in_microsec(tLoad) << " us"
				  << " (" << time_in_microsec(tLoad_total) << " us)\n";
		std::cout << "Solve:\t" << time_in_microsec(tSolver) << " us"
				  << " (" << time_in_microsec(tSolver_total) << " us)\n";
		std::cout << "Total:\t" << time_in_microsec(tTotal) << " us"
				  << " (" << time_in_microsec(tTotal_total) << " us)\n";

		const auto result = Sudoku::getResult(options);
		if (result == answer)
		{
			std::cout << " : ) Found the answer!\n";
		}
		else
		{
			std::cout << "Different from expected \n";
			std::cout << Sudoku::Console().print_board(options).str();
		}
		++id;
	}
	{
		time t0{};
		time t1{};
		time t2{};
		duration tLoad{duration::max()};
		duration tSolver{duration::max()};
		duration tTotal{duration::max()};

		static int id{0};
		static duration tLoad_total{0};
		static duration tSolver_total{0};
		static duration tTotal_total{0};
		static duration tdiff{0};
		static bool last{};

		for (auto j{0}; j < reruns; ++j) //-V1008
		{
			for (auto i{0}; i < repeat; ++i) //-V1008
			{
				t0 = std::chrono::steady_clock::now();
				Sudoku::Board<Sudoku::Options<elem_size>, N> local{};
				set_Value(local, start.cbegin(), start.cend());
				t1 = std::chrono::steady_clock::now();
				Sudoku::test_solver_exclusive(local);
				t2 = std::chrono::steady_clock::now();

				// save fastest solving time
				tdiff   = t1 - t0;
				tLoad   = std::min(tLoad, tdiff);
				tdiff   = t2 - t0;
				last    = tTotal > tdiff;
				tTotal  = std::min(tTotal, tdiff);
				tSolver = std::min(tSolver, tTotal - tLoad);

				options = local;
			}
			if (last)
			{
				std::cout << "Last run was fastest " << j << ": true\n";
			}
			// sum fastest times
			tLoad_total += tLoad;
			tSolver_total += tSolver;
			tTotal_total += tTotal;
		}
		const auto time_in_microsec = [](duration t_in)
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(t_in)
				.count();
		};
		std::cout << "ID: " << id << "\n"
				  << "Load:\t" << time_in_microsec(tLoad) << " us"
				  << " (" << time_in_microsec(tLoad_total) << " us)\n";
		std::cout << "Solve:\t" << time_in_microsec(tSolver) << " us"
				  << " (" << time_in_microsec(tSolver_total) << " us)\n";
		std::cout << "Total:\t" << time_in_microsec(tTotal) << " us"
				  << " (" << time_in_microsec(tTotal_total) << " us)\n";

		const auto result = Sudoku::getResult(options);
		if (result == answer)
		{
			std::cout << " : ) Found the answer!\n";
		}
		else
		{
			std::cout << "Different from expected \n";
			std::cout << Sudoku::Console().print_board(options).str();
		}
		++id;
	}
}


int main() // NOLINT(bugprone-exception-escape) ignore potential for exceptions
{
	// clang-format off
	constexpr std::array<char, 81> b1
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
	constexpr std::array<char, 81> b1a	// requires unique
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
	test(b1, b1a);


	//	start board					answer board
	//	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
	//	|	  5	| 3		|       |	|     5	| 3    	|       |
	//	| 8		|       |	2	|	| 8    	|   5   |   2  	|
	//	|_ _7_ _|_ _1_ _|_5_ _ _|	|_ _7_ _|_ _1_ _|_5_ _ _|
	//	| 4		|     5	| 3     |	| 4    	|     5	| 3     |
	//	|	1	|   7 	|  	  6	|	|   1  	|   7 3	|     6	|
	//	|_ _ _3_|_2_ _ _|_ _8_ _|	|_ _ _3_|_2_ _ _|_ _8_ _|
	//	| 	6	| 5     |	  9	|	|   6  	| 5     |     9	|
	//	|     4	|		|   3	|	|     4	|      	|   3  	|
	//	|_ _ _ _|_ _ _9_|_7_ _ _|	|_ _ _ _|_ _ _9_|_7_ _ _|
	//
	constexpr std::array<char, 81> b2
	{
		0, 0, 5,	3, 0, 0,	0, 0, 0,
		8, 0, 0,	0, 0, 0,	0, 2, 0,
		0, 7, 0,	0, 1, 0,	5, 0, 0,

		4, 0, 0,	0, 0, 5,	3, 0, 0,
		0, 1, 0,	0, 7, 0,	0, 0, 6,
		0, 0, 3,	2, 0, 0,	0, 8, 0,

		0, 6, 0,	5, 0, 0,	0, 0, 9,
		0, 0, 4,	0, 0, 0,	0, 3, 0,
		0, 0, 0,	0, 0, 9,	7, 0, 0
	};
	constexpr std::array<char, 81> b2a // can't solve without guessing
	{
		1, 4, 5,	3, 2, 7,	6, 9, 8,
		8, 3, 9,	6, 5, 4,	1, 2, 7,
		6, 7, 2,	9, 1, 8,	5, 4, 3,

		4, 9, 6,	1, 8, 5,	3, 7, 2,
		2, 1, 8,	4, 7, 3,	9, 5, 6,
		7, 5, 3,	2, 9, 6,	4, 8, 1,

		3, 6, 7,	5, 4, 2,	8, 1, 9,
		9, 8, 4,	7, 6, 1,	2, 3, 5,
		5, 2, 1,	8, 3, 9,	7, 6, 4
	};
	test(b2, b2a);

	// source: en.wikipedia.org
	//	start board					answer board
	//	 _ _ _ _ _ _ _ _ _ _ _ _	 _ _ _ _ _ _ _ _ _ _ _ _
	//	| 5 3   |   7   |       |	| 5 3 4 | 6 7 8	| 9 1 2 |
	//	| 6     | 1 9 5 |       |	| 6 7 2 | 1 9 5 | 3 4 8	|
	//	|_ _9_8_|_ _ _ _|_ _6_ _|	|_1_9_8_|_3_4_2_|_5_6_7_|
	//	| 8     |   6   |     3 |	| 8 5 9	| 7 6 1	| 4 2 3 |
	//	| 4     | 8   3 |     1 |	| 4 2 6	| 8 5 3	| 7 9 1	|
	//	|_7_ _ _|_ _2_ _|_ _ _6_|	|_7_1_3_|_9_2_4_|_8_5_6_|
	//	|   6   |       | 2 8   |	| 9 6 1	| 5 3 7 | 2 8 4	|
	//	|       | 4 1 9 |     5 |	| 2 8 7	| 4 1 9	| 6 3 5	|
	//	|_ _ _ _|_ _8_ _|_ _7_9_|	|_3_4_5_|_2_8_6_|_1_7_9_|
	//
	constexpr std::array<char, 81> b3
	{
		5, 3, 0,	0, 7, 0,	0, 0, 0,
		6, 0, 0,	1, 9, 5,	0, 0, 0,
		0, 9, 8,	0, 0, 0,	0, 6, 0,

		8, 0, 0,	0, 6, 0,	0, 0, 3,
		4, 0, 0,	8, 0, 3,	0, 0, 1,
		7, 0, 0,	0, 2, 0,	0, 0, 6,

		0, 6, 0,	0, 0, 0,	2, 8, 0,
		0, 0, 0,	4, 1, 9,	0, 0, 5,
		0, 0, 0,	0, 8, 0,	0, 7, 9
	};

	constexpr std::array<char, 81> b3a			// no unique
		// source: https://en.wikipedia.org/wiki/Sudoku
	{
		5, 3, 4,	6, 7, 8,	9, 1, 2,	// 0,8=1 random to trigger output
		6, 7, 2,	1, 9, 5,	3, 4, 8,
		1, 9, 8,	3, 4, 2,	5, 6, 7,

		8, 5, 9,	7, 6, 1,	4, 2, 3,
		4, 2, 6,	8, 5, 3,	7, 9, 1,
		7, 1, 3,	9, 2, 4,	8, 5, 6,

		9, 6, 1,	5, 3, 7,	2, 8, 4,
		2, 8, 7,	4, 1, 9,	6, 3, 5,
		3, 4, 5,	2, 8, 6,	1, 7, 9
	};
	test(b3, b3a);

	constexpr std::array<char, 81> b4
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
	constexpr std::array<char, 81> b4a			// no unique
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
	test(b4, b4a);

	constexpr std::array<char, 81> b5
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
	constexpr std::array<char, 81> b5a	// requires double_option, not unique
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
	test(b5, b5a);

	constexpr std::array<char, 81> b6
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
	constexpr std::array<char, 81> b6a	// requires multi_option (3), not unique
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
	test(b6, b6a);

	constexpr std::array<char, 81> b7
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
	constexpr std::array<char, 81> b7a	// incomplete, require unique
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
	test(b7, b7a);

#if false // unused boards
	constexpr std::array<char, 81> easiest
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
	constexpr std::array<char, 81> gentle
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
	constexpr std::array<char, 81> moderate
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
	constexpr std::array<char, 81> tough
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

	constexpr std::array<char, 81> diagonal
		// 17 clues and diagonal symmetry
		// source: https://en.wikipedia.org/wiki/Sudoku_solving_algorithms
	{
		0, 0, 0,	0, 0, 0,	0, 0, 1,
		0, 0, 0,	0, 0, 0,	0, 2, 3,
		0, 0, 4,	0, 0, 5,	0, 0, 0,

		0, 0, 0,	1, 0, 0,	0, 0, 0,
		0, 0, 0,	0, 3, 0,	6, 0, 0,
		0, 0, 7,	0, 0, 0,	5, 8, 0,

		0, 0, 0,	0, 6, 7,	0, 0, 0,
		0, 1, 0,	0, 0, 4,	0, 0, 0,
		5, 2, 0,	0, 0, 0,	0, 0, 0
	};

	constexpr std::array<char, 81> automorphic
		// 18 clues and two-way diagonal symmetry
		// source: https://en.wikipedia.org/wiki/Mathematics_of_Sudoku#Automorphic_Sudokus
	{
		0, 0, 0,	2, 1, 0,	0, 0, 0,
		0, 0, 7,	3, 0, 0,	0, 0, 0,
		0, 5, 8,	0, 0, 0,	0, 0, 0,

		4, 3, 0,	0, 0, 0,	0, 0, 0,
		2, 0, 0,	0, 0, 0,	0, 0, 8,
		0, 0, 0,	0, 0, 0,	0, 7, 6,

		0, 0, 0,	0, 0, 0,	2, 5, 0,
		0, 0, 0,	0, 0, 7,	3, 0, 0,
		0, 0, 0,	0, 9, 8,	0, 0, 0
	};

	constexpr std::array<char, 81> automorphic2
		// source: https://en.wikipedia.org/wiki/Mathematics_of_Sudoku#Automorphic_Sudokus
	{
		0, 0, 0,	5, 6, 0,	0, 3, 4,
		0, 0, 0,	7, 8, 0,	0, 5, 6,
		0, 0, 0,	0, 0, 0,	0, 0, 0,

		1, 2, 0,	0, 0, 0,	0, 0, 0,
		3, 4, 0,	0, 0, 0,	0, 6, 7,
		0, 0, 0,	0, 0, 0,	0, 8, 9,

		0, 0, 0,	0, 0, 0,	0, 0, 0,
		4, 5, 0,	0, 2, 3,	0, 0, 0,
		6, 7, 0,	0, 4, 5,	0, 0, 0
	};

	constexpr std::array<char, 81> empty
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
#endif // unused boards
// clang-format off
} // NOLINT(readability/fn_size)
