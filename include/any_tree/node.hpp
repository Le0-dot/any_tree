#pragma once

#include <any>
#include <array>
#include <vector>
#include <type_traits>

#include "implementation.hpp"


namespace any_tree {

template<typename Payload, typename ChildrenContainer, std::size_t ID = 0>
requires any_range<ChildrenContainer> || std::is_void_v<ChildrenContainer>
class node : public impl::node_payload<Payload>, public impl::node_children<ChildrenContainer> {
public:
    template<typename... Args>
    node(Args&&... args) : impl::node_payload<Payload>{std::forward<Args>(args)...} {}
};

template<typename Payload, typename ChildrenContainer, std::size_t ID>
requires any_range<ChildrenContainer> && std::ranges::random_access_range<ChildrenContainer>
class node<Payload, ChildrenContainer, ID> : public impl::node_payload<Payload>, public impl::node_random_access<ChildrenContainer> {
public:
    template<typename... Args>
    node(Args&&... args) : impl::node_payload<Payload>{std::forward<Args>(args)...} {}
};


template<typename Paylaod, std::size_t N, std::size_t ID = 0>
using static_node = node<Paylaod, std::array<std::any, N>, ID>;

template<typename Payload, std::size_t ID = 0>
using dynamic_node = node<Payload, std::vector<std::any>, ID>;

template<typename Payload, std::size_t ID = 0>
using leaf = node<Payload, void, ID>;

}
