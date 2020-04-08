// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/Board_Section_traits.cpp                       ----====//
//
// Unit tests for the traits defined for the Board_Section class.
//====--------------------------------------------------------------------====//

#include <Sudoku/Board_Section_traits.h> // Class under test


namespace SudokuTests::Section_traits
{
using dataT        = int;
constexpr int size = 3;

using Row         = typename ::Sudoku::Board_Section::Row<dataT, size>;
using Col         = typename ::Sudoku::Board_Section::Col<dataT, size>;
using Block       = typename ::Sudoku::Board_Section::Block<dataT, size>;
using const_Row   = typename ::Sudoku::Board_Section::const_Row<dataT, size>;
using const_Col   = typename ::Sudoku::Board_Section::const_Col<dataT, size>;
using const_Block = typename ::Sudoku::Board_Section::const_Block<dataT, size>;

// inline namespace traits
static_assert(std::is_same_v<
			  ::Sudoku::Board_Section::is_Section<int>,
			  ::Sudoku::Board_Section::traits::is_Section<int>>);

using ::Sudoku::Board_Section::traits::is_Section_v;
static_assert(not is_Section_v<int>);
static_assert(is_Section_v<Row>);
static_assert(is_Section_v<Col>);
static_assert(is_Section_v<Block>);
static_assert(is_Section_v<const_Row>);
static_assert(is_Section_v<const_Col>);
static_assert(is_Section_v<const_Block>);

using ::Sudoku::Board_Section::traits::is_const_Section_v;
static_assert(not is_const_Section_v<int>);
static_assert(not is_const_Section_v<Row>);
static_assert(not is_const_Section_v<Col>);
static_assert(not is_const_Section_v<Block>);
static_assert(is_const_Section_v<const_Row>);
static_assert(is_const_Section_v<const_Col>);
static_assert(is_const_Section_v<const_Block>);

static_assert(::Sudoku::Board_Section::traits::is_Row<Row>::value);
using ::Sudoku::Board_Section::traits::is_Row_v;
static_assert(not is_Row_v<int>);
static_assert(is_Row_v<Row>);
static_assert(is_Row_v<const_Row>);
static_assert(not is_Row_v<Col>);
static_assert(not is_Row_v<const_Col>);
static_assert(not is_Row_v<Block>);
static_assert(not is_Row_v<const_Block>);


static_assert(::Sudoku::Board_Section::traits::is_Col<Col>::value);
using ::Sudoku::Board_Section::traits::is_Col_v;
static_assert(not is_Col_v<int>);
static_assert(is_Col_v<Col>);
static_assert(is_Col_v<const_Col>);
static_assert(not is_Col_v<Row>);
static_assert(not is_Col_v<const_Row>);
static_assert(not is_Col_v<Block>);
static_assert(not is_Col_v<const_Block>);


static_assert(::Sudoku::Board_Section::traits::is_Block<Block>::value);
using ::Sudoku::Board_Section::traits::is_Block_v;
static_assert(not is_Block_v<int>);
static_assert(is_Block_v<Block>);
static_assert(is_Block_v<const_Block>);
static_assert(not is_Block_v<Row>);
static_assert(not is_Block_v<const_Row>);
static_assert(not is_Block_v<Col>);
static_assert(not is_Block_v<const_Col>);

// For references to Section:
static_assert(not is_Section_v<Row&>); //? Should this work?
} // namespace SudokuTests::Section_traits
