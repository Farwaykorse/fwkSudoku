//====---- Sudoku/traits.h                                            ----====//
//
// Type traits.
//====--------------------------------------------------------------------====//
#pragma once

#include <iterator>
#include <type_traits>


namespace Sudoku
{
inline namespace traits
{
	template<typename T>
	struct is_int : std::integral_constant<bool, std::is_integral_v<T>>
	{
	};
	template<>
	struct is_int<bool> : std::false_type
	{
	};
	template<typename T>
	inline static constexpr bool is_int_v = is_int<T>::value;

	//====----------------------------------------------------------------====//
	namespace impl
	{ // Iterator, helper functions to check properties
		template<typename Itr>
		using Category_ = typename std::iterator_traits<Itr>::iterator_category;
		template<typename Itr>
		using Target_ = typename std::iterator_traits<Itr>::value_type;
	} // namespace impl

	// random iterator
	template<typename Itr>
	inline constexpr bool is_random =
		std::is_same_v<impl::Category_<Itr>, std::random_access_iterator_tag>;
	// bidirectional iterator or better
	template<typename Itr>
	inline constexpr bool is_bidir =
		std::is_same_v<impl::Category_<Itr>, std::bidirectional_iterator_tag> ||
		is_random<Itr>;
	// forward iterator or better
	template<typename Itr>
	inline constexpr bool is_forward =
		std::is_same_v<impl::Category_<Itr>, std::forward_iterator_tag> ||
		is_bidir<Itr>;
	// input iterator or better
	template<typename Itr>
	inline constexpr bool is_input =
		std::is_same_v<impl::Category_<Itr>, std::input_iterator_tag> ||
		is_forward<Itr>;

	// iterator pointing to type
	template<typename ItrT, typename TargetT>
	inline constexpr bool iterator_to =
		std::is_same_v<impl::Target_<ItrT>, TargetT>;

	// iterator pointing to type implicitly convertible to type
	template<typename ItrT, typename TargetT>
	inline constexpr bool iterator_implicit_to =
		std::is_convertible_v<impl::Target_<ItrT>, TargetT>;

	// iterator pointing to type explicitly convertible to type
	template<typename ItrT, typename TargetT>
	inline constexpr bool iterator_explicit_to =
		std::is_constructible_v<TargetT, impl::Target_<ItrT>>;

} // namespace traits
} // namespace Sudoku
