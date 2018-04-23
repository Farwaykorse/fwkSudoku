//====---- SudokuTests/Board_Section.cpp                              ----====//
//
// Unit tests for the template class Sudoku::Board_Section
//====--------------------------------------------------------------------====//

// Class under test
#include <Sudoku/Board_Section_traits.h>


namespace SudokuTests
{
using namespace ::Sudoku;
using namespace ::Sudoku::Board_Sections;

using dataT        = int;
constexpr int size = 3;

using Row         = typename Board_Sections::Row<dataT, size>;
using Col         = typename Board_Sections::Col<dataT, size>;
using Block       = typename Board_Sections::Block<dataT, size>;
using const_Row   = typename Board_Sections::const_Row<dataT, size>;
using const_Col   = typename Board_Sections::const_Col<dataT, size>;
using const_Block = typename Board_Sections::const_Block<dataT, size>;

namespace Section_traits
{
	// inline namespace traits
	static_assert(std::is_same_v<
				  Board_Sections::is_Section<int>,
				  Board_Sections::traits::is_Section<int>>);
	using namespace ::Sudoku::Board_Sections::traits;

	static_assert(not is_Section_v<int>);
	static_assert(is_Section_v<Row>);
	static_assert(is_Section_v<Col>);
	static_assert(is_Section_v<Block>);
	static_assert(is_Section_v<const_Row>);
	static_assert(is_Section_v<const_Col>);
	static_assert(is_Section_v<const_Block>);

	static_assert(not is_const_Section<int>::value);
	static_assert(not is_const_Section_v<int>);
	static_assert(not is_const_Section_v<Row>);
	static_assert(not is_const_Section_v<Col>);
	static_assert(not is_const_Section_v<Block>);
	static_assert(is_const_Section_v<const_Row>);
	static_assert(is_const_Section_v<const_Col>);
	static_assert(is_const_Section_v<const_Block>);

	static_assert(not is_Row<int>::value);
	static_assert(not is_Row_v<int>);
	static_assert(is_Row<Row>::value);
	static_assert(is_Row_v<Row>);
	static_assert(is_Row_v<const_Row>);
	static_assert(not is_Row_v<Col>);
	static_assert(not is_Row_v<const_Col>);
	static_assert(not is_Row_v<Block>);
	static_assert(not is_Row_v<const_Block>);

	static_assert(not is_Col<int>::value);
	static_assert(not is_Col_v<int>);
	static_assert(is_Col<Col>::value);
	static_assert(is_Col_v<Col>);
	static_assert(is_Col_v<const_Col>);
	static_assert(not is_Col_v<Row>);
	static_assert(not is_Col_v<const_Row>);
	static_assert(not is_Col_v<Block>);
	static_assert(not is_Col_v<const_Block>);

	static_assert(not is_Block<int>::value);
	static_assert(not is_Block_v<int>);
	static_assert(is_Block<Block>::value);
	static_assert(is_Block_v<Block>);
	static_assert(is_Block_v<const_Block>);
	static_assert(not is_Block_v<Row>);
	static_assert(not is_Block_v<const_Row>);
	static_assert(not is_Block_v<Col>);
	static_assert(not is_Block_v<const_Col>);

	// For references to Section:
	static_assert(not is_Section_v<Row&>); //? Should this work?
} // namespace traits


} // namespace SudokuTests
