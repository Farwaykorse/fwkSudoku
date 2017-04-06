#pragma once

#include <string>
#include <sstream>
#include <iomanip>		// setw(), setfill()

#include "../Sudoku/Board.h"
#include "../Sudoku/Options.h"

namespace Sudoku
{
class Console
{
public:
	struct delimiter
	{
		char space;
		char empty;
		std::string row_block;
		std::string col_block;
		std::string newl;
		std::string block_cross;
	};
	const delimiter space
	{
		' ',	' ',	"",		"",		"",		""
	};
	const delimiter space2
	{
		' ',	' ',	"",		"",		"\n",	""
	};
	const delimiter display
	{
		' ',	' ',	"-",	"|",	"\n",	"o"
	};
	//static const Format::delimiter csv;
	//static const Format::delimiter xml;

	Console();
	Console(delimiter);
	~Console() = default;

	template<int N>
	std::stringstream print_row(const Board<int, N>&, int id) const;
	template<int N, int E>
	std::stringstream print_row(const Board<Options<E>, N>&, int id) const;
	template<int N>
	std::stringstream print_board(const Board<int, N>&) const;
	template<int N, int E>
	std::stringstream print_board(const Board<Options<E>, N>&) const;

	delimiter d;
private:
	int charsize(int value) const;
	int charsize(int, int counter) const;	// recursion
//	bool Format::find_option(const Board<std::set<int>>&, Location, int value);
	// format elem
	// format col-block section
	// format row-block = seperator line


};

inline
Console::Console() :
	d(display)
{
	// empty constructor
}

inline
Console::Console(delimiter del) :
	d(del)
{
	// empty constructor
}

inline
int Console::charsize(int in) const
{
	assert(in >= 0);
	if (in < 10) { return 1; }
	return charsize(in, 2);
}

inline
int Console::charsize(int in, int length) const
{
	if (in < pow(10, length))
	{
		return length;
	}
	++length;
	return charsize(in, length);
}

template<int N>
std::stringstream Console::print_row(const Board<int,N>& input, int row) const
{
	std::stringstream stream;
	const int chars = charsize(input.elem_size) + 1;

	stream << d.col_block << std::setfill(d.space);
	for (int i = 0; i < input.elem_size; ++i)
	{
		if (input[row][i] == 0)	// no value
		{
			stream << std::setw(chars) << d.space;
		}
		else
		{
			stream << std::setw(chars) << input[row][i];
		}
		if ((i + 1) % input.base_size == 0)
		{
			stream << std::setw(2) << d.col_block;
		}
	}
	return stream;
}

template<int N>
std::stringstream Console::print_board(const Board<int,N>& input) const
{
	std::stringstream stream;
	std::stringstream temp;
	const int chars = charsize(input.elem_size) + 1;

	// opening bar
	temp << d.block_cross;
	for (int j = 0; j < input.base_size; ++j)
	{
		temp << std::setfill(d.row_block[0]) << std::setw(chars * input.base_size + 2) << d.block_cross;
	}
	std::string bar;
	temp >> bar;
	stream << bar << '\n';

	// loop rows
	for (int i = 0; i < input.elem_size; ++i)
	{
		stream << print_row(input, i).str() << d.newl;
		if ((i + 1) % input.base_size == 0)
		{
			stream << bar << '\n';
		}
	}
	return stream;
}

template<int N, int E>
std::stringstream Console::print_board(const Board<Options<E>,N>& input) const
{
	static_assert(E == N*N, "");
	assert(input.elem_size == 9);	// no support for different sizes yet
	const int base_size = input.base_size;
	const int elem_size = input.elem_size;
	const int block_size = elem_size + base_size + 2;
	const int row_length = base_size * block_size;
	/*
	9   9   9
	o-----------------------------------------o
	| 123 123 123 | 123 123 123 | 123 123 123 |
	| 456 456 456 | 456 456 456 | 456 456 456 | 9
	| 789 789
	|
	| 123 123
	| ...
	o-----------------------------------------o
	|
	*/
	std::stringstream stream;
	std::stringstream n0;		// horizontal block seperator
	std::stringstream n4;		// vertical element seperator
	n0 << std::setfill(d.row_block[0]) << d.block_cross << std::setw(row_length) << d.block_cross << d.newl;
	n4 << std::setfill(' ') << std::setw(block_size) << d.col_block;
	//stream << std::endl;
	//for (size_t col = 0; col < elem_size; ++col)
	//{
	//	stream << std::setfill(' ') << std::setw(4) << col_prop.count_unknown(col);
	//}

	stream << std::setfill(d.empty) << std::setw(base_size);
	const std::string empty = stream.str();

	stream << '\n' << n0.str();


	for (int row{ 0 }; row < elem_size; ++row)
	{

		stream << print_row(input, row).str();

		if ((row + 1) % 3 == 0) { stream << n0.str(); }
		else { stream << '|' << n4.str() << n4.str() << n4.str() << std::endl; }
	}
	return stream;
}

template<int N, int E>
std::stringstream Console::print_row(const Board<Options<E>, N>& input, int row) const
{
	const int base_size = input.base_size;
	const int elem_size = input.elem_size;

	std::stringstream stream;

	int X{ 1 };
	for (int k{ 0 }; k < base_size; ++k)
	{
		stream << d.col_block << d.space;
		for (int col{ 0 }; col < elem_size; ++col)
		{
			for (int i{ X }; i < X + base_size; ++i)
			{
				if (input[row][col].test(i)) { stream << i; }
				else { stream << d.empty; }
			}
			if ((col + 1) % base_size == 0)
			{
				stream << std::setfill(d.space) << std::setw(2) << d.col_block << d.space;
			}
			else
			{
				stream << d.space;
			}
		}
		stream << d.newl;
		X += base_size;
	}
	return stream;
}
}	// namespace Sudoku
