#pragma once

#include <any>
#include <ranges>
#include <concepts>


namespace any_tree {

template<typename R>
concept any_range = std::ranges::range<R> && std::same_as<std::any, std::ranges::range_value_t<R>>;


template<typename F, typename T>
struct invocable_ref { constexpr static bool value = std::invocable<F, T&>; };

template<typename F>
struct invocable_ref<F, void> { constexpr static bool value = std::invocable<F>; };

template<typename F, typename T>
constexpr bool invocable_ref_v = invocable_ref<F, T>::value;


template<typename F, typename T>
struct invocable_cref { constexpr static bool value = std::invocable<F, const T&>; };

template<typename F>
struct invocable_cref<F, void> { constexpr static bool value = std::invocable<F>; };

template<typename F, typename T>
constexpr bool invocable_cref_v = invocable_cref<F, T>::value;

}
