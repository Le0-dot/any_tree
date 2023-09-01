#pragma once

#include <any>
#include <array>
#include <vector>
#include <type_traits>

#include "implementation.hpp"


namespace any_tree {

template<typename Payload, typename ChildrenContainer>
requires any_range<ChildrenContainer> || std::is_void_v<ChildrenContainer>
class node : public impl::node_payload<Payload>, public impl::node_children<ChildrenContainer> {
public:
    template<typename... Args>
    node(Args&&... args) : impl::node_payload<Payload>{std::forward<Args>(args)...} {}
};

template<typename Payload, typename ChildrenContainer>
requires any_range<ChildrenContainer> && std::ranges::random_access_range<ChildrenContainer>
class node<Payload, ChildrenContainer> : public impl::node_payload<Payload>, public impl::node_random_access<ChildrenContainer> {
public:
    template<typename... Args>
    node(Args&&... args) : impl::node_payload<Payload>{std::forward<Args>(args)...} {}
};


template<typename Paylaod, std::size_t N>
using static_node = node<Paylaod, std::array<std::any, N>>;

template<typename Payload>
using dynamic_node = node<Payload, std::vector<std::any>>;

template<typename Payload>
using leaf = node<Payload, void>;

}
