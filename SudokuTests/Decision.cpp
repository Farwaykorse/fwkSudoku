// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// http://www.viva64.com
//
//====---- SudokuTests/Decision.cpp                                   ----====//
//
// Unit tests for ...
//====--------------------------------------------------------------------====//
#include <gtest/gtest.h>

// Class under test
#include <Sudoku/Decision.h>
// helpers
#include <Sudoku/Value.h>
#include <Sudoku/exceptions.h>
// library
#include <array>
#include <bitset>
#include <vector>
#include <type_traits>
#include <cstddef> // std::size_t


namespace SudokuTests::workerBoard
{
constexpr int N = 3;
using typeT     = ::Sudoku::impl::worker_Board<N>;
using Board     = ::Sudoku::Board<::Sudoku::Options<::Sudoku::elem_size<N>>, N>;
using Board_ptr = std::unique_ptr<Board>;

namespace properties
{
	// Composite Type Categories
	static_assert(not std::is_fundamental_v<typeT>);
	static_assert(std::is_compound_v<typeT>);
	static_assert(std::is_object_v<typeT>);

	static_assert(not std::is_scalar_v<typeT>);
	static_assert(not std::is_array_v<typeT>);
	static_assert(not std::is_union_v<typeT>);
	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_reference_v<typeT>);
	static_assert(not std::is_function_v<typeT>);
	// Type Properties
	static_assert(not std::is_const_v<typeT>);
	static_assert(not std::is_volatile_v<typeT>);
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(not std::is_trivially_copyable_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);
	static_assert(std::is_standard_layout_v<typeT>);
	static_assert(not std::has_unique_object_representations_v<typeT>);
	static_assert(not std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_final_v<typeT>);
	static_assert(not std::is_abstract_v<typeT>);
	static_assert(std::is_aggregate_v<typeT>);
} // namespace properties

namespace constructors
{
	// default constructor: typeT()
	// NOTE: not, to prevent accidental processing of an empty board
	static_assert(std::is_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);

	// copy constructor: typeT(const typeT&)
	static_assert(not std::is_copy_constructible_v<typeT>);
	static_assert(not std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(not std::is_trivially_copy_constructible_v<typeT>);

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(not std::is_trivially_move_constructible_v<typeT>);

	//====----------------------------------------------------------------====//
	static_assert(not std::is_constructible_v<typeT, int>);
	static_assert(not std::is_constructible_v<typeT, Board_ptr>);
	static_assert(not std::is_constructible_v<typeT, int, Board_ptr>); // ??
	static_assert(not std::is_constructible_v<typeT, Board_ptr, int>);
} // namespace constructors

namespace destructors
{
	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(not std::is_trivially_destructible_v<typeT>);
	static_assert(not std::has_virtual_destructor_v<typeT>);
} // namespace destructors

namespace swapping
{
	static_assert(std::is_swappable_v<typeT>);         // C++17
	static_assert(std::is_nothrow_swappable_v<typeT>); // C++17
} // namespace swapping

namespace assignment
{
	// Copy assignment
	static_assert(not std::is_copy_assignable_v<typeT>);
	static_assert(not std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(not std::is_trivially_copy_assignable_v<typeT>);

	// Move assignment
	static_assert(std::is_move_assignable_v<typeT>);
	static_assert(std::is_nothrow_move_assignable_v<typeT>);
	static_assert(not std::is_trivially_move_assignable_v<typeT>);
} // namespace assignment
} // namespace SudokuTests::workerBoard

namespace SudokuTests::Type
{
using ::Sudoku::Board;
using ::Sudoku::Location;
using ::Sudoku::Location_Block;
using ::Sudoku::Multipass_Base;
using ::Sudoku::Options;
using ::Sudoku::Value;
using ::Sudoku::elem_size;

// Board_Section
using dataT        = int;
constexpr int size = 3;
using iBoard       = Board<dataT, size>;
using vBoard       = Board<Value, size>;
using const_iBoard = Board<dataT, size> const;
using oBoard       = Board<Options<elem_size<size>>, size>;
using Loc          = Location<size>;
using L_B          = Location_Block<size>;

using typeT = ::Sudoku::Multipass_Base<3>;

namespace type_properties
{ // http://howardhinnant.github.io/TypeHiearchy.pdf
	// Composite Type Categories
	static_assert(not std::is_fundamental_v<typeT>);
	static_assert(std::is_compound_v<typeT>);
	static_assert(std::is_object_v<typeT>);

	static_assert(not std::is_scalar_v<typeT>);
	static_assert(not std::is_array_v<typeT>);
	static_assert(not std::is_union_v<typeT>);
	static_assert(std::is_class_v<typeT>);
	static_assert(not std::is_reference_v<typeT>);
	static_assert(not std::is_function_v<typeT>);
	// Type Properties
	static_assert(not std::is_const_v<typeT>);
	static_assert(not std::is_volatile_v<typeT>);
	static_assert(not std::is_trivial_v<typeT>);
	static_assert(not std::is_trivially_copyable_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);
	static_assert(not std::has_unique_object_representations_v<typeT>);
	static_assert(not std::is_empty_v<typeT>);
	static_assert(not std::is_polymorphic_v<typeT>);
	static_assert(not std::is_aggregate_v<typeT>);
} // namespace type_properties

namespace constructors
{
	// default constructor: typeT()
	// NOTE: not, to prevent accidental processing of an empty board
	static_assert(std::is_default_constructible_v<typeT>);
	static_assert(std::is_nothrow_default_constructible_v<typeT>);
	static_assert(not std::is_trivially_default_constructible_v<typeT>);

	// copy constructor: typeT(const typeT&)
	static_assert(not std::is_copy_constructible_v<typeT>);
	static_assert(not std::is_nothrow_copy_constructible_v<typeT>);
	static_assert(not std::is_trivially_copy_constructible_v<typeT>);

	// move constructor: typeT(typeT&&)
	static_assert(std::is_move_constructible_v<typeT>);
	static_assert(std::is_nothrow_move_constructible_v<typeT>);
	static_assert(not std::is_trivially_move_constructible_v<typeT>);

	//====----------------------------------------------------------------====//
	// just a Board<int>
	static_assert(not std::is_constructible_v<typeT, iBoard>);
	static_assert(not std::is_nothrow_constructible_v<typeT, iBoard>);
	static_assert(not std::is_constructible_v<typeT, iBoard&>);
	static_assert(not std::is_nothrow_constructible_v<typeT, iBoard&>);
	// Board<Value>
	static_assert(std::is_constructible_v<typeT, vBoard>);
	static_assert(not std::is_nothrow_constructible_v<typeT, vBoard>);
	static_assert(std::is_constructible_v<typeT, vBoard&>);
	static_assert(not std::is_nothrow_constructible_v<typeT, vBoard&>);
	// Board<Options>
	static_assert(std::is_constructible_v<typeT, oBoard>);
	static_assert(std::is_nothrow_constructible_v<typeT, oBoard>);
	static_assert(std::is_constructible_v<typeT, oBoard&>);
	static_assert(std::is_nothrow_constructible_v<typeT, oBoard&>);

	//====----------------------------------------------------------------====//
	// Board<int>& and element number:
	static_assert(not std::is_constructible_v<typeT, iBoard&, int>);
	static_assert(not std::is_nothrow_constructible_v<typeT, iBoard&, int>);
	// Board<Options>& and element number:
	static_assert(not std::is_constructible_v<typeT, oBoard&, int>);
	static_assert(not std::is_nothrow_constructible_v<typeT, oBoard&, int>);

	// Board const&
	static_assert(std::is_constructible_v<typeT, vBoard const&>);
	static_assert(not std::is_nothrow_constructible_v<typeT, vBoard const&>);
	static_assert(std::is_constructible_v<typeT, oBoard const&>);
	static_assert(std::is_nothrow_constructible_v<typeT, oBoard const&>);
	// Board*
	static_assert(not std::is_constructible_v<typeT, iBoard*>);
	static_assert(not std::is_constructible_v<typeT, oBoard*>);
	// Board: Size errors
	static_assert(
		not std::is_constructible_v<typeT, Sudoku::Board<dataT, size + 1>&>);
	// Board: different data-type
	static_assert(
		not std::is_constructible_v<typeT, Sudoku::Board<float, size>&>);
	// from vector / array
	static_assert(
		not std::is_constructible_v<typeT, std::vector<int8_t> const&>);
	static_assert(not std::is_constructible_v<
				  typeT,
				  std::array<int8_t, ::Sudoku::full_size<size>> const&>);
	static_assert(
		not std::is_constructible_v<typeT, ::Sudoku::impl::worker_Board<size>>,
		"Prevent accidental construction when something else was intended.");
	// worker_Board objects are inputs for add_to_queue() and add_answer().

	TEST(Multipass, constructorValueBoard)
	{
		// clang-format off
		constexpr std::array<char, 16> v1
		{   // start	// after set_Value
			0,2, 0,0,	// 1	2	3	4
			4,0, 0,0,	// 4	3	1,2	1,2
			0,1, 4,0,	// 2,3	1	4	2,3
			0,0, 0,0	// 2,3	4	1,2	1,2,3
		}; // clang-format on
		Board<Value, 2> B1;
		std::transform(
			v1.cbegin(), v1.cend(), B1.begin(), ::Sudoku::to_Value<2, int>);

		Multipass_Base<2> base(B1);

		// error checking
		// clang-format off
		constexpr std::array<char, 16> v2
		{
			1,2, 3,4,
			4,3, 2,1,
			0,2, 0,0, // (2,1):2 error
			0,0, 0,0
		}; // clang-format on
		B1.clear();
		std::transform(
			v2.cbegin(), v2.cend(), B1.begin(), ::Sudoku::to_Value<2, int>);
		EXPECT_THROW(Multipass_Base<2>{B1}, ::Sudoku::error::invalid_Board);
	}
	TEST(Multipass, constructorOptionsBoard)
	{
		// clang-format off
		constexpr std::array<char, 16> v1
		{
			0,2, 0,0,
			4,0, 0,0,
			0,1, 4,0,
			0,0, 0,0
		}; // clang-format on
		Board<Options<4>, 2> B1{};
		::Sudoku::set_Value(B1, std::begin(v1), std::end(v1));

		Multipass_Base<2> base(B1);
	}
} // namespace constructors

namespace destructors
{
	static_assert(std::is_destructible_v<typeT>);
	static_assert(std::is_nothrow_destructible_v<typeT>);
	static_assert(not std::is_trivially_destructible_v<typeT>);
	static_assert(not std::has_virtual_destructor_v<typeT>);
} // namespace destructors

namespace swapping
{
	static_assert(std::is_swappable_v<typeT>);         // C++17
	static_assert(std::is_nothrow_swappable_v<typeT>); // C++17
} // namespace swapping

namespace assignment
{
	// Copy assignment
	static_assert(not std::is_copy_assignable_v<typeT>);
	static_assert(not std::is_nothrow_copy_assignable_v<typeT>);
	static_assert(not std::is_trivially_copy_assignable_v<typeT>);

	// Move assignment
	static_assert(std::is_move_assignable_v<typeT>);
	static_assert(std::is_nothrow_move_assignable_v<typeT>);
	static_assert(not std::is_trivially_move_assignable_v<typeT>);

	// wrong size
	using MB_2 = ::Sudoku::Multipass_Base<2>;
	using MB_3 = ::Sudoku::Multipass_Base<3>;
	static_assert(not std::is_assignable_v<MB_2, MB_3>);
	static_assert(not std::is_assignable_v<MB_3, MB_2>);

	// Other types
	static_assert(std::is_assignable_v<typeT, oBoard>);
	static_assert(std::is_nothrow_assignable_v<typeT, oBoard>);
	static_assert(std::is_assignable_v<typeT, vBoard>);
	static_assert(not std::is_nothrow_assignable_v<typeT, vBoard>);
	static_assert(not std::is_assignable_v<typeT, iBoard>);
	static_assert(
		not std::is_assignable_v<typeT, ::Sudoku::impl::worker_Board<size>>,
		"Prevent accidental resetting when something else was intended.");
	// worker_Board objects are inputs for add_to_queue() and add_answer().

	TEST(Multipass, assignmentOptionsBoard)
	{
		using ::Sudoku::impl::worker_Board;

		Multipass_Base<2> base;

		// clang-format off
		constexpr std::array<char, 16> v1
		{
			0,2, 0,0,
			4,0, 0,0,
			0,1, 4,0,
			0,0, 0,0
		}; // clang-format on
		Board<Options<4>, 2> B1{};
		::Sudoku::set_Value(B1, std::begin(v1), std::end(v1));

		EXPECT_TRUE(base.finished());
		base = B1;
		EXPECT_FALSE(base.finished());
		base.add_to_queue(worker_Board<2>{}); // queue size = 2
		worker_Board<2> tmp{};
		base.add_answer(tmp);
		EXPECT_EQ(base.answer_count(), 1);

		base = B1; // assign: queue size = 1; answers size = 0
		EXPECT_FALSE(base.finished());
		EXPECT_EQ(base.answer_count(), 0);
		[[maybe_unused]] auto dump = base.get_from_queue(); // queue size = 0
		EXPECT_TRUE(base.finished());
	}
	TEST(Multipass, assignmentValueBoard)
	{
		// clang-format off
		constexpr std::array<char, 16> v1
		{   // start	// after set_Value
			0,2, 0,0,	// 1	2	3	4
			4,0, 0,0,	// 4	3	1,2	1,2
			0,1, 4,0,	// 2,3	1	4	2,3
			0,0, 0,0	// 2,3	4	1,2	1,2,3
		}; // clang-format on
		Board<Value, 2> B1;
		std::transform(
			v1.cbegin(), v1.cend(), B1.begin(), ::Sudoku::to_Value<2, int>);

		Multipass_Base<2> base; // default constructed
		EXPECT_TRUE(base.finished());
		EXPECT_EQ(base.answer_count(), 0);
		::Sudoku::impl::worker_Board<2> tmp{};
		base.add_answer(tmp);
		EXPECT_EQ(base.answer_count(), 1);
		EXPECT_TRUE(base.finished());

		base = B1;
		EXPECT_FALSE(base.finished());
		EXPECT_EQ(base.answer_count(), 0);

		// error checking
		// clang-format off
		constexpr std::array<char, 16> v2
		{
			1,2, 3,4,
			4,3, 2,1,
			0,2, 0,0, // (2,1):2 error
			0,0, 0,0
		}; // clang-format on
		B1.clear();
		std::transform(
			v2.cbegin(), v2.cend(), B1.begin(), ::Sudoku::to_Value<2, int>);
		EXPECT_THROW(base = B1, ::Sudoku::error::invalid_Board);
	}
} // namespace assignment
} // namespace SudokuTests::Type

namespace SudokuTests::members
{
using ::Sudoku::Board;
using ::Sudoku::Options;
using ::Sudoku::Multipass_Base;

TEST(Multipass, mfFinished)
{
	// clang-format off
	constexpr std::array<char, 16> v1
	{
		0,2, 0,0,
		4,0, 0,0,
		0,1, 4,0,
		0,0, 0,0
	}; // clang-format on
	Board<Options<4>, 2> B1{};
	::Sudoku::set_Value(B1, std::begin(v1), std::end(v1));

	Multipass_Base<2> base(B1);

	static_assert(noexcept(base.finished()));
	// return type
	static_assert(std::is_same_v<bool, decltype(base.finished())>);
	EXPECT_FALSE(base.finished());

	// default constructed:
	Multipass_Base<2> base_def;
	EXPECT_TRUE(base_def.finished());

	// input: empty board
	Board<Options<4>, 2> empty{};
	Multipass_Base<2> empty_base(empty);
	EXPECT_FALSE(base.finished());

	// input: finished board
	// clang-format off
	constexpr std::array<char, 16> vfinished
	{
		1,2, 3,4,
		4,3, 2,1,
		2,1, 4,3,
		3,4, 1,2
	}; // clang-format on
	B1.clear();
	::Sudoku::set_Value(B1, std::begin(vfinished), std::end(vfinished));
	Multipass_Base<2> finished_base(B1);
	EXPECT_FALSE(base.finished());
}

TEST(Multipass, mfAnswerCount)
{
	// clang-format off
	constexpr std::array<char, 16> v1
	{
		1,2, 3,4,
		4,3, 2,1,
		2,1, 4,3,
		3,4, 1,2
	}; // clang-format on
	Board<Options<4>, 2> B1{};
	::Sudoku::set_Value(B1, std::begin(v1), std::end(v1));
	Multipass_Base<2> base(B1);

	static_assert(noexcept(base.answer_count()));
	// return type
	static_assert(std::is_same_v<int, decltype(base.answer_count())>);
	EXPECT_EQ(base.answer_count(), 0);
}
TEST(Multipass, mfAddAnswer)
{
	using ::Sudoku::impl::worker_Board;
	using ::Sudoku::config::MAX_ANSWERS;

	Board<Options<4>, 2> board{};
	board[0][0] = ::Sudoku::Value{2}; // not empty, prevents filling
	Multipass_Base<2> base{board};
	EXPECT_EQ(base.answer_count(), 0);

	worker_Board<2> item{12, std::make_unique<decltype(board)>(board)};
	static_assert(noexcept(base.add_answer(item)));

	base.add_answer(item);
	EXPECT_EQ(base.answer_count(), 1);
	base.add_answer(item);
	EXPECT_EQ(base.answer_count(), 2);
	// limited size of answer storage, adding (the same item)
	if constexpr (MAX_ANSWERS > 0 && MAX_ANSWERS < 80)
	{
		for (int i{}; i < MAX_ANSWERS; ++i)
		{
			base.add_answer(item);
		}
		EXPECT_EQ(base.answer_count(), MAX_ANSWERS);
	}
	EXPECT_EQ(base.finished(), false);
}
TEST(Multipass, mfAddToQueue)
{
	using ::Sudoku::impl::worker_Board;

	Board<Options<4>, 2> board{};
	board[0][0] = ::Sudoku::Value{1}; // not empty, to prevent filling
	Multipass_Base<2> base{board};
	EXPECT_FALSE(base.finished());
	worker_Board<2> item{5, std::make_unique<decltype(board)>(board)};

	static_assert(noexcept(base.add_to_queue(std::move(item))));

	base.add_to_queue(std::move(item));
	EXPECT_FALSE(base.finished());
}
TEST(Multipass, mfGetFromQueue)
{
	// depending on add_to_queue() and finished()
	using ::Sudoku::impl::worker_Board;

	Board<Options<4>, 2> board{};
	board[0][0] = ::Sudoku::Value{1}; // not empty, to prevent filling
	Multipass_Base<2> base{board};
	EXPECT_FALSE(base.finished());

	static_assert(noexcept(base.get_from_queue()));
	// return type
	static_assert(std::is_same_v<
				  std::optional<worker_Board<2>>,
				  decltype(base.get_from_queue())>);

	auto returned = base.get_from_queue();
	EXPECT_TRUE(returned.has_value());
	EXPECT_TRUE(base.finished());
	returned = base.get_from_queue();
	EXPECT_FALSE(returned.has_value());

	worker_Board<2> item{5, std::make_unique<decltype(board)>(board)};
	base.add_to_queue(std::move(item));
	EXPECT_FALSE(base.finished());
	returned = base.get_from_queue();
	EXPECT_TRUE(returned.has_value());
	EXPECT_TRUE(base.finished());
}
} // namespace SudokuTests::members

namespace SudokuTests::helpers
{
using ::Sudoku::Board;
using ::Sudoku::Options;
using ::Sudoku::Value;
using ::Sudoku::Location;
using ::Sudoku::elem_size;
using ::Sudoku::full_size;

TEST(Multipass, fillBeginEmptyBoard)
{
	using ::Sudoku::fill_begin_empty_board;

	Board<Options<4>, 2> vB_2{};
	Board<Options<9>, 3> vB_3{};
	Board<Options<16>, 4> vB_4{};

	static_assert(std::is_same_v<int, decltype(fill_begin_empty_board(vB_2))>);
	static_assert(not noexcept(fill_begin_empty_board(vB_2)));
	// should not throw, unless a programming error
	// always the same input & output, depending only on template<int N>

	Board<Options<4>, 2> not_empty{};
	not_empty.at(Location<2>{10}) = Value{1};
	[[maybe_unused]] int x{};
	EXPECT_DEBUG_DEATH(x = fill_begin_empty_board(not_empty), "count_options");
	not_empty.at(Location<2>{10}) = std::bitset<5>{"00001"};
	EXPECT_DEBUG_DEATH(x = fill_begin_empty_board(not_empty), "count_options");

	ASSERT_TRUE(vB_2.at(Location<2>{0}).all());
	constexpr int changes_2 = full_size<2>                        //
							  + (elem_size<2> - 2) * elem_size<2> //
							  + (2 - 1) * elem_size<2> * 2;
	constexpr int changes_3 = full_size<3>                        //
							  + (elem_size<3> - 3) * elem_size<3> //
							  + (3 - 1) * elem_size<3> * 3;
	constexpr int changes_4 = full_size<4>                        //
							  + (elem_size<4> - 4) * elem_size<4> //
							  + (4 - 1) * elem_size<4> * 4;
	// return value
	EXPECT_EQ(fill_begin_empty_board(vB_2), changes_2);
	EXPECT_EQ(fill_begin_empty_board(vB_3), changes_3);
	EXPECT_EQ(fill_begin_empty_board(vB_4), changes_4);

	EXPECT_TRUE(is_answer(vB_2.at(Location<2>{0}), Value{1}));
	EXPECT_TRUE(is_answer(vB_2.at(Location<2>{1}), Value{2}));
	EXPECT_EQ(vB_2.at(Location<2>{1, 0}).DebugString(), "11001");
	EXPECT_EQ(vB_2.at(Location<2>{1, 0}), vB_2.at(Location<2>{1, 1}));
	EXPECT_NE(vB_2.at(Location<2>{1, 0}), vB_2.at(Location<2>{1, 2}));
	EXPECT_EQ(vB_2.at(Location<2>{1, 2}), vB_2.at(Location<2>{1, 3}));
	EXPECT_EQ(vB_2.at(Location<2>{1, 2}).DebugString(), "00111");
	EXPECT_EQ(vB_2.at(Location<2>{3, 1}).DebugString(), "11011");
	EXPECT_EQ(vB_2.at(Location<2>{3, 2}).DebugString(), "10111");
}

TEST(Multipass, countOptions)
{
	using ::Sudoku::count_options;

	constexpr int N{2};
	Board<Options<elem_size<N>>, N> vB_2{};

	static_assert(noexcept(count_options(vB_2)));
	static_assert(std::is_same_v<int, decltype(count_options(vB_2))>);

	constexpr int all_options{elem_size<N> * full_size<N>};
	EXPECT_EQ(count_options(vB_2), all_options);
	vB_2.at(Location<N>{12}) = Value{3};
	EXPECT_EQ(count_options(vB_2), all_options - elem_size<N>);
	vB_2.clear();
	EXPECT_EQ(count_options(vB_2), all_options);
	if constexpr (N == 2)
	{ // wrong answer when:
		vB_2[1][3] = std::bitset<5>{"11110"};
		EXPECT_EQ(count_options(vB_2), all_options - elem_size<N>);
		vB_2[1][3] = std::bitset<5>{"00001"};
		EXPECT_EQ(count_options(vB_2), all_options - elem_size<N>);
	}
}

TEST(Multipass, execution)
{
	// clang-format off
	constexpr std::array<char, 16> v1
	{
		// start	// after set_Value
		0,2, 0,0,	// 1	2	3	4
		4,0, 0,0,	// 4	3	1,2	1,2
		0,1, 4,0,	// 2,3	1	4	2,3
		0,0, 0,0	// 2,3	4	1,2	1,2,3
	}; // clang-format on
	Board<Value, 2> B1;
	std::transform(
		v1.cbegin(), v1.cend(), B1.begin(), ::Sudoku::to_Value<2, int>);

	::Sudoku::Multipass_Base<2> base(B1);

	auto& answers = base();
	ASSERT_EQ(answers.size(), size_t{3}); // 3 possible solutions!
	// unchanged input values
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 1}), Value{2});
	EXPECT_EQ(answers.at(0)->at(Location<2>{1, 0}), Value{4});
	EXPECT_EQ(answers.at(0)->at(Location<2>{2, 1}), Value{1});
	EXPECT_EQ(answers.at(0)->at(Location<2>{2, 2}), Value{4});
	// Solved?
	EXPECT_EQ(::Sudoku::count_options(*answers.at(0)), 0);
	EXPECT_EQ(::Sudoku::count_options(*answers.at(1)), 0);
	EXPECT_EQ(::Sudoku::count_options(*answers.at(2)), 0);
	// Answers (set_Value)
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 0}), Value{1});
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 2}), Value{3});
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 3}), Value{4});
	EXPECT_EQ(answers.at(0)->at(Location<2>{1, 1}), Value{3});
	EXPECT_EQ(answers.at(0)->at(Location<2>{3, 1}), Value{4});
	// Solver results:
	// First branch:
	// Expect highest value of the available options in first unanswered
	// element (i.e. (1,2)), to be the last in the queue. Therefore:
	ASSERT_EQ(answers.at(0)->at(Location<2>{1, 2}), Value{2});
	EXPECT_EQ(answers.at(0)->at(Location<2>{1, 3}), Value{1});
	EXPECT_EQ(answers.at(0)->at(Location<2>{3, 2}), Value{1});
	// Second branch, last option:
	ASSERT_EQ(answers.at(0)->at(Location<2>{2, 0}), Value{3});
	EXPECT_EQ(answers.at(0)->at(Location<2>{2, 3}), Value{2});
	EXPECT_EQ(answers.at(0)->at(Location<2>{3, 0}), Value{2});
	// Second branch, other option:
	EXPECT_EQ(answers.at(1)->at(Location<2>{2, 0}), Value{2});
	// First branch:
	ASSERT_EQ(answers.at(2)->at(Location<2>{1, 2}), Value{1});
	EXPECT_EQ(answers.at(2)->at(Location<2>{1, 3}), Value{2});
	EXPECT_EQ(answers.at(2)->at(Location<2>{2, 3}), Value{3});
	EXPECT_EQ(answers.at(2)->at(Location<2>{3, 3}), Value{1});
}

TEST(Multipass, executionOptionsBoard)
{
	// clang-format off
	constexpr std::array<char, 16> v1
	{
		// start	// after set_Value
		0,2, 0,0,	// 1	2	3	4
		4,0, 0,0,	// 4	3	1,2	1,2
		0,1, 4,0,	// 2,3	1	4	2,3
		0,0, 0,0	// 2,3	4	1,2	1,2,3
	}; // clang-format on
	Board<Options<4>, 2> B1{};
	::Sudoku::set_Value(B1, std::begin(v1), std::end(v1));

	::Sudoku::Multipass_Base<2> base(B1);

	auto& answers = base();
	ASSERT_EQ(answers.size(), size_t{3}); // 3 possible solutions!
	// unchanged input values
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 1}), Value{2});
	EXPECT_EQ(answers.at(0)->at(Location<2>{1, 0}), Value{4});
	EXPECT_EQ(answers.at(0)->at(Location<2>{2, 1}), Value{1});
	EXPECT_EQ(answers.at(0)->at(Location<2>{2, 2}), Value{4});
	// Solved?
	EXPECT_EQ(::Sudoku::count_options(*answers.at(0)), 0);
	EXPECT_EQ(::Sudoku::count_options(*answers.at(1)), 0);
	EXPECT_EQ(::Sudoku::count_options(*answers.at(2)), 0);
	// Answers (set_Value)
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 0}), Value{1});
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 2}), Value{3});
	EXPECT_EQ(answers.at(0)->at(Location<2>{0, 3}), Value{4});
	EXPECT_EQ(answers.at(0)->at(Location<2>{1, 1}), Value{3});
	EXPECT_EQ(answers.at(0)->at(Location<2>{3, 1}), Value{4});
	// Solver results:
	// First branch:
	// Expect highest value of the available options in first unanswered
	// element (i.e. (1,2)), to be the last in the queue. Therefore:
	ASSERT_EQ(answers.at(0)->at(Location<2>{1, 2}), Value{2});
	EXPECT_EQ(answers.at(0)->at(Location<2>{1, 3}), Value{1});
	EXPECT_EQ(answers.at(0)->at(Location<2>{3, 2}), Value{1});
	// Second branch, last option:
	ASSERT_EQ(answers.at(0)->at(Location<2>{2, 0}), Value{3});
	EXPECT_EQ(answers.at(0)->at(Location<2>{2, 3}), Value{2});
	EXPECT_EQ(answers.at(0)->at(Location<2>{3, 0}), Value{2});
	// Second branch, other option:
	EXPECT_EQ(answers.at(1)->at(Location<2>{2, 0}), Value{2});
	// First branch:
	ASSERT_EQ(answers.at(2)->at(Location<2>{1, 2}), Value{1});
	EXPECT_EQ(answers.at(2)->at(Location<2>{1, 3}), Value{2});
	EXPECT_EQ(answers.at(2)->at(Location<2>{2, 3}), Value{3});
	EXPECT_EQ(answers.at(2)->at(Location<2>{3, 3}), Value{1});
}

TEST(Multipass, solve1)
{
	// clang-format off
	constexpr std::array<char, 81> v1
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
	constexpr std::array<char, 81> v1a	// requires unique
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

	Board<Value, 3> B1;
	Board<Value, 3> ans;
	std::transform(
		v1.cbegin(), v1.cend(), B1.begin(), ::Sudoku::to_Value<3, int>);
	std::transform(
		v1a.cbegin(), v1a.cend(), ans.begin(), ::Sudoku::to_Value<3, int>);

	::Sudoku::Multipass_Base<3> base(B1);

	auto& answers = base();
	EXPECT_EQ(answers.size(), size_t{1});
	for (int i{}; i < ::Sudoku::full_size<3>; ++i)
	{
		// EXPECT_TRUE(is_answer(answers.at(0)->at(Location<3>{i})));
		EXPECT_TRUE(is_answer(
			answers.at(0)->at(Location<3>{i}), ans.at(Location<3>{i})));
	}
	EXPECT_EQ(::Sudoku::count_options(*answers.at(0)), 0);
}

TEST(Multipass, solve2)
{
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
	// clang-format off
	constexpr std::array<char, 81> v1
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
	constexpr std::array<char, 81> v1a
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
	// clang-format on
	Board<Value, 3> B1;
	Board<Value, 3> ans;
	std::transform(
		v1.cbegin(), v1.cend(), B1.begin(), ::Sudoku::to_Value<3, int>);
	std::transform(
		v1a.cbegin(), v1a.cend(), ans.begin(), ::Sudoku::to_Value<3, int>);

	::Sudoku::Multipass_Base<3> base(B1);

	auto& answers = base();
	EXPECT_EQ(answers.size(), size_t{1});
	for (int i{}; i < ::Sudoku::full_size<3>; ++i)
	{
		EXPECT_TRUE(is_answer(answers.at(0)->at(Location<3>{i})));
		EXPECT_TRUE(is_answer(
			answers.at(0)->at(Location<3>{i}), ans.at(Location<3>{i})));
	}
	// recount options, verify
	EXPECT_EQ(::Sudoku::count_options(*answers.at(0)), 0);
}

TEST(Multipass, solve3)
{
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
	// clang-format off
	constexpr std::array<char, 81> v1
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
	constexpr std::array<char, 81> v1a	// can't solve
	{
		5, 3, 4,	6, 7, 8,	9, 1, 2,
		6, 7, 2,	1, 9, 5,	3, 4, 8,
		1, 9, 8,	3, 4, 2,	5, 6, 7,

		8, 5, 9,	7, 6, 1,	4, 2, 3,
		4, 2, 6,	8, 5, 3,	7, 9, 1,
		7, 1, 3,	9, 2, 4,	8, 5, 6,

		9, 6, 1,	5, 3, 7,	2, 8, 4,
		2, 8, 7,	4, 1, 9,	6, 3, 5,
		3, 4, 5,	2, 8, 6,	1, 7, 9
	};
	// clang-format on

	Board<Value, 3> B1;
	Board<Value, 3> ans;
	std::transform(
		v1.cbegin(), v1.cend(), B1.begin(), ::Sudoku::to_Value<3, int>);
	std::transform(
		v1a.cbegin(), v1a.cend(), ans.begin(), ::Sudoku::to_Value<3, int>);

	::Sudoku::Multipass_Base<3> base(B1);
	auto& answers = base();
	EXPECT_EQ(answers.size(), size_t{1});
	for (int i{}; i < ::Sudoku::full_size<3>; ++i)
	{
		EXPECT_TRUE(is_answer(answers.at(0)->at(Location<3>{i})));
		EXPECT_TRUE(is_answer(
			answers.at(0)->at(Location<3>{i}), ans.at(Location<3>{i})));
	}
	EXPECT_EQ(::Sudoku::count_options(*answers.at(0)), 0);
}

TEST(Multipass, emptyBase2)
{
	Board<Value, 2> empty{};
	::Sudoku::Multipass_Base<2> base{empty};
	auto& answers = base();
	EXPECT_EQ(answers.size(), 12U);
}

//! NOTE: way to much options to run.
// TEST(Multipass, empty_base_3)
//{
//	Board<Value, 3> empty{};
//	::Sudoku::Multipass_Base<3> base{empty};
//	auto& answers = base();
//	EXPECT_EQ(answers.size(), 12u);
//}

TEST(Multipass, almostEmpty)
{
	Board<Value, 2> one{};
	one.at(Location<2>{0}) = Value{1};
	::Sudoku::Multipass_Base<2> base{one};
	auto& answers = base();
	// EXPECT_EQ(answers.size(), 72u);
	EXPECT_EQ(answers.size(), size_t{::Sudoku::config::MAX_ANSWERS});
}

TEST(Multipass, pickLocation)
{
	using ::Sudoku::pick_location;

	// clang-format off
	constexpr std::array<char, 16> v1
	{
		1,2, 3,4,
		4,3, 2,1,
		2,1, 4,3,
		3,4, 1,2
	}; // clang-format on
	Board<Options<4>, 2> board{};
	::Sudoku::set_Value(board, v1.begin(), v1.end());

	static_assert(noexcept(pick_location(board)));
	// return type
	static_assert(std::is_same_v<Location<2>, decltype(pick_location(board))>);

	// already answered
#ifdef NDEBUG
	EXPECT_EQ(pick_location(board), Location<2>(0, 0));
#else
	EXPECT_DEBUG_DEATH(
		[[maybe_unused]] auto x = pick_location(board), "Assertion failed:");
#endif // NDEBUG
	// empty board
	board.clear();
	EXPECT_EQ(pick_location(board), Location<2>(0, 0));
	board[0][1] = std::bitset<5>{"10101"};
	EXPECT_EQ(pick_location(board), Location<2>(0, 1));
	// answered item
	board[1][1] = Value{1};
	EXPECT_EQ(pick_location(board), Location<2>(0, 1));
	// first item is answered
	board[0][0] = Value{2};
	EXPECT_EQ(pick_location(board), Location<2>(0, 1));
	board.clear();
	board[0][1] = Value{3};
	EXPECT_EQ(pick_location(board), Location<2>(0, 0));
}

TEST(Multipass, guess)
{
	using ::Sudoku::guess;
	using ::Sudoku::impl::worker_Board;

	::Sudoku::Multipass_Base<2> base;
	static_assert(noexcept(guess(base, worker_Board<2>())));
	ASSERT_TRUE(base.finished());
	// add_to_queue() & available() can result in an out of memory exception.
	// single_option should not throw, outside programming errors in available.

	// all possible options are valid
	Board<Options<4>, 2> B_empty;
	worker_Board<2> empty{0, std::make_unique<decltype(B_empty)>(B_empty)};
	guess(base, empty);
	int counter{};
	while (!base.finished())
	{
		++counter;
		auto item = base.get_from_queue();
		EXPECT_EQ(item.value().changes, 11); // reported changes
	}
	EXPECT_EQ(counter, 4); // number of boards added to the queue

	// all possible options are invalid -> no board added to queue
	// solved Board:
	// invalid Board:

	// incorrect values for the changes counter in the worker_Board
	// no influence on operation

	// Make tests independent of the strategy used in pick_location()
	//  (where possible)
	//
}

} // namespace SudokuTests::helpers
