#pragma once

#include <any>
#include <typeindex>
#include <functional>
#include <unordered_map>

#include "traits.hpp"


namespace any_tree {

template<typename T>
using const_children_visitor = std::unordered_map<std::type_index, std::function<T(const std::any&)>>;

template<typename T>
using children_visitor = std::unordered_map<std::type_index, std::function<T(std::any&)>>;


template<typename F, typename T>
struct const_child_visitor { 
    using type = typename const_children_visitor<std::invoke_result_t<F, const T&>>::value_type; 
};

template<typename F>
struct const_child_visitor<F, void> { 
    using type = typename const_children_visitor<std::invoke_result_t<F>>::value_type; 
};

template<typename F, typename T>
using const_child_visitor_t = typename const_child_visitor<F, T>::type;


template<typename F, typename T>
struct child_visitor { 
    using type = typename children_visitor<std::invoke_result_t<F, T&>>::value_type; 
};

template<typename F>
struct child_visitor<F, void> { 
    using type = typename children_visitor<std::invoke_result_t<F>>::value_type; 
};

template<typename F, typename T>
using child_visitor_t = typename child_visitor<F, T>::type;


template<typename T>
auto const_visit_node(const const_children_visitor<T>& visitor, const std::any& node) -> T {
    const auto type = std::type_index(node.type());
    const auto func = visitor.at(type);
    return std::invoke(func, node);
}

template<typename T>
auto visit_node(const children_visitor<T>& visitor, std::any& node) -> T {
    const auto type = std::type_index(node.type());
    const auto func = visitor.at(type);
    return std::invoke(func, node);
}


template<typename T, typename F>
requires invocable_cref_v<F, T>
auto make_const_child_visitor(const F& f) -> const_child_visitor_t<F, T> {
    return {
	std::type_index(typeid(T)),
	[f] (const std::any& child) {
	    if constexpr(std::is_void_v<T>)
		return std::invoke(f);
	    else
		return std::invoke(f, std::any_cast<const T&>(child));
	}
    };
}

template<typename T, typename F>
requires invocable_ref_v<F, T>
auto make_child_visitor(const F& f) -> child_visitor_t<F, T> {
    return {
	std::type_index(typeid(T)),
	[f] (std::any& child) {
	    if constexpr(std::is_void_v<T>)
		return std::invoke(f);
	    else
		return std::invoke(f, std::any_cast<T&>(child));
	}
    };
}

}
