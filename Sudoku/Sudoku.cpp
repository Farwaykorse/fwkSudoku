// Sudoku.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "SolverBase.h"

#include <algorithm>	// min
#include <iostream>
#include <chrono>

int main()
{
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

	Sudoku::Board<int, 3> start;
	std::copy(b1.cbegin(), b1.cend(), start.begin());
	Sudoku::Board<int, 3> answer;
	std::copy(b1a.cbegin(), b1a.cend(), answer.begin());

	// working object:
	Sudoku::SolverBase<3> other;

	const size_t repeat{ 1000 };
	Sudoku::SolverBase<3> test_board{};
	using time = std::chrono::time_point<std::chrono::steady_clock>;
	using duration = std::chrono::duration<long long, std::nano>;
	time t0{};
	time t1{};
	time t2{};
	duration tLoad{ duration::max() };
	duration tSolver{ duration::max() };
	duration tTotal{ duration::max() };

	// find uniques (should solve this board)
	for (size_t i{}; i < repeat; ++i)
	{
		t0 = std::chrono::steady_clock::now();
		test_board = start;
		t1 = std::chrono::steady_clock::now();
		test_board.solver_unique();
		t2 = std::chrono::steady_clock::now();

		tLoad = std::min(tLoad, t1 - t0);
		tTotal = std::min(tTotal, t2 - t0);
		tSolver = std::min(tSolver, tTotal - tLoad);
	}

	auto time_in_microsec = [](duration t_in)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(t_in).count();
	};
	std::cout << "Load:\t" << time_in_microsec(tLoad) << " us\n";
	std::cout << "Solve:\t" << time_in_microsec(tSolver) << " us\n";
	std::cout << "Total:\t" << time_in_microsec(tTotal) << " us\n";
	Sudoku::Board<int, 3> result = test_board.getResult();
	if (result == answer) { std::cout << " : ) Found the answer!\n"; }
	else { std::cout << ":{ bugs ...\n"; }
}
