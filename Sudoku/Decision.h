//====---- Sudoku/Decision.h                                          ----====//
//
// Automated solving.
//====--------------------------------------------------------------------====//
#pragma once

#include "Board.h"
#include "Options.h"
#include "Size.h"
#include "Solver.h"
#include "Solvers_set_option.h"
#include "Value.h"
#include "exceptions.h"

#include <gsl/gsl>
#include <optional>
#include <stack>
#include <vector>
#include <algorithm> // find_if_not
#include <memory>    // unique_ptr
#include <numeric>   // iota, reduce
#include <utility>   // forward, move
#include <cassert>


namespace Sudoku
{
namespace config
{
	// Maximum amount of possible answer boards.
	// == 0 : no answers retained; < 0 : unlimited answers
	static constexpr int MAX_ANSWERS = 50;
} // namespace config

namespace impl
{
	template<int N>
	struct worker_Board
	{
		using Options = ::Sudoku::Options<elem_size<N>>;
		using Board   = ::Sudoku::Board<Options, N>;

		int changes{};
		std::unique_ptr<Board> board;
	};
} // namespace impl

template<int N>
class Multipass_Base
{
	template<typename T>
	using Board_t      = ::Sudoku::Board<T, N>;
	using Options      = ::Sudoku::Options<elem_size<N>>;
	using Board_ptr    = std::unique_ptr<Board_t<Options>>;
	using worker_Board = impl::worker_Board<N>;
	using Stack        = std::stack<worker_Board, std::vector<worker_Board>>;

	Stack queue;
	std::vector<Board_ptr> answers;

public:
	Multipass_Base() = delete;
	explicit Multipass_Base(Board_t<Options> start)
	{
		int changes = all_options<N> - count_options(start);

		if (changes == 0)
		{
			changes = fill_begin_empty_board(start);
		}
		add_to_queue(worker_Board{
			changes, std::make_unique<decltype(start)>(std::move(start))});
	}
	explicit Multipass_Base(Board_t<Value> const& start)
	{
		int changes{};
		Board_t<Options> local{};
		changes = set_Value(local, start.cbegin(), start.cend());

		if (changes == 0)
		{
			changes = fill_begin_empty_board(local);
		}
		add_to_queue(worker_Board{
			changes, std::make_unique<decltype(local)>(std::move(local))});
	}

	bool finished() const noexcept { return queue.empty(); }
	int answer_count() const noexcept
	{
		return gsl::narrow_cast<int>(answers.size());
	}

	void add_answer(worker_Board& item) noexcept(config::MAX_ANSWERS >= 0)
	{
		if (config::MAX_ANSWERS && answer_count() < config::MAX_ANSWERS)
		{
			answers.push_back(std::move(item.board));
		}
	}
	void add_to_queue(worker_Board&& board)
	{
		queue.emplace(std::forward<worker_Board>(board));
	}
	std::optional<worker_Board> get_from_queue()
	{
		if (!finished())
		{
			auto item = std::move(queue.top());
			queue.pop();
			return std::move(item);
		}
		return {};
	}

	auto& operator()()
	{
		if (!finished())
		{
			scheduler(*this);
		}
		return answers;
	}
};

//====--------------------------------------------------------------------====//
// free-function declarations
template<int N>
[[nodiscard]] bool answered(Multipass_Base<N>&, impl::worker_Board<N>&);
template<int N>
void guess(Multipass_Base<N>&, impl::worker_Board<N> const&);
template<int N>
[[nodiscard]] Location<N>
	pick_location(Board<Options<elem_size<N>>, N> const&) noexcept;
template<int N>
auto choose_solver(impl::worker_Board<N>&);
template<int N>
[[nodiscard]] int test_unique(Board<Options<elem_size<N>>, N>&);
template<int N>
[[nodiscard]] int fill_begin_empty_board(Board<Options<elem_size<N>>, N>&);
template<int N>
[[nodiscard]] int
	count_options(Board<Options<elem_size<N>>, N> const&) noexcept;
template<int N>
void scheduler(Multipass_Base<N>&);

//====--------------------------------------------------------------------====//
// member-function definitions

//====--------------------------------------------------------------------====//
// free-function definitions

template<int N>
inline bool answered(Multipass_Base<N>& base, impl::worker_Board<N>& item)
{
	if (item.changes == all_options<N>)
	{
		base.add_answer(item);
		return true;
	}
	assert(item.changes < all_options<N>);
	return false;
}

// For an unanswered location, add new boards to the queue for each possibility
template<int N>
inline void guess(Multipass_Base<N>& base, impl::worker_Board<N> const& input)
{
	using Board = Board<Options<elem_size<N>>, N>;

	const Location<N> loc = pick_location(*(input.board));
	const auto options    = available(input.board->at(loc));

	for (auto const& value : options)
	{
		Board board = *(input.board);
		try
		{
			int changes = single_option(board, loc, value);
			changes += input.changes;
			base.add_to_queue(impl::worker_Board<N>{
				changes, std::make_unique<Board>(std::move(board))});
		}
		catch (error::invalid_Board const&)
		{ // continue to try next
		}
	}
}

// Pick a suitable location
template<int N>
Location<N> pick_location(Board<Options<elem_size<N>>, N> const& board) noexcept
{
	// first implementation, just return the first not answered location
	auto itr = std::find_if_not(
		board.cbegin(), board.cend(), [](Options<elem_size<N>> x) {
			return is_answer(x);
		});
	assert(itr != board.cend());
	return Location<N>{itr};

	// TODO improved: first, non answer, with least options
	// ...
}

template<int N>
auto choose_solver(impl::worker_Board<N>& input)
{
	assert(input.changes > 0);              // start guessing
	assert(input.changes < all_options<N>); // solved already

	int changes{};
	// choose what solver to run and run it
	while (true)
	{
		changes = 0;
		// unique in row
		{
			for (int i{}; i < elem_size<N>; ++i)
			{
				changes += unique_in_section(*input.board, input.board->row(i));
			}
			input.changes += changes;
		}
		if (input.changes == all_options<N>) return std::move(input);
		// unique in col
		{
			int changes_{};
			for (int i{}; i < elem_size<N>; ++i)
			{
				changes_ +=
					unique_in_section(*input.board, input.board->col(i));
			}
			input.changes += changes_;
			changes += changes_;
		}
		// if (changes > elem_size<N>) continue;
		if (input.changes == all_options<N>) return std::move(input);
		// unique in block
		{
			int changes_{};
			for (int i{}; i < elem_size<N>; ++i)
			{
				changes_ +=
					unique_in_section(*input.board, input.board->block(i));
			}
			input.changes += changes_;
			changes += changes_;
		}

		// int changes = test_unique<N>(*(input.board));
		// input.changes += changes;

		// finished
		if (input.changes == all_options<N>) return std::move(input);
		// can't find the answer
		if (changes == 0) return std::move(input);
	}
	// how to recognize guessing might be the only option?
	// how many changes before guessing becomes a reasonable option?
}

// Temporary solver for use during design
template<int N>
int test_unique(Board<Options<elem_size<N>>, N>& board)
{
	int changes{};
	for (int i = 0; i < elem_size<N>; ++i)
	{
		changes += unique_in_section(board, board.row(i));
	}
	for (int i = 0; i < elem_size<N>; ++i)
	{
		changes += unique_in_section(board, board.col(i));
	}
	for (int i = 0; i < elem_size<N>; ++i)
	{
		changes += unique_in_section(board, board.block(i));
	}
	return changes;
}

// Set the first row to answers: [1, elem_size<N>], ordered
template<int N>
int fill_begin_empty_board(Board<Options<elem_size<N>>, N>& board)
{
	assert(count_options(board) == full_size<N> * elem_size<N>);
	std::iota(board.row(0).begin(), board.row(0).end(), Value{1});
	// No unique solutions are lost.
	// The answers only differ by the symbol choice for each option.
	// Therefore (N^2)! less answers need to be found.
	// N = 2 -> 4! = 24; N = 3 -> 9! = 362 880

	int changes{full_size<N>}; // changes in row(0)
	for (int i{}; i < elem_size<N>; ++i)
	{
		Location<N> loc{i};
		Value value{static_cast<size_t>(i + 1)};
		changes += remove_option_section(board, board.col(loc), loc, value);
		changes += remove_option_section(board, board.block(loc), loc, value);
	}
	return changes;
}

template<int N>
inline int count_options(Board<Options<elem_size<N>>, N> const& board) noexcept
{
	using Options   = Options<elem_size<N>>;
	const auto size = [](size_t x, Options y) noexcept
	{
		return x + y.count();
	};
	auto count = gsl::narrow_cast<int>(
		std::reduce(std::begin(board), std::end(board), size_t{0}, size));
	return count;
}

template<int N>
void scheduler(Multipass_Base<N>& base)
{
	impl::worker_Board<N> worker;

	// if anything in queue
	while (std::optional<impl::worker_Board<N>> item{base.get_from_queue()})
	{
		if (answered(base, item.value())) continue;

		try
		{
			worker = choose_solver(item.value());
		}
		catch (error::invalid_Board const&)
		{
			continue;
		}

		if (!answered(base, worker))
		{
			guess(base, worker);
		}
	}
	// finished
}

} // namespace Sudoku
