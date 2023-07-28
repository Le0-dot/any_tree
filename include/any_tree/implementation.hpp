#pragma once

#include <algorithm>
#include <type_traits>

#include "traits.hpp"


namespace any_tree::impl {

template<typename Payload>
class node_payload {
private:
    Payload _payload{};

public:
    [[nodiscard]] const Payload& payload() const noexcept { return _payload; }
    [[nodiscard]]       Payload& payload()       noexcept { return _payload; }
};

template<>
class node_payload<void> {};


template<typename ChildrenContainer>
requires any_range<ChildrenContainer> || std::is_void_v<ChildrenContainer>
class node_children {
private:
    ChildrenContainer _children{};

public:
    [[nodiscard]] const auto& children() const noexcept { return _children; }
    [[nodiscard]]       auto& children()       noexcept { return _children; }

    [[nodiscard]] const auto children_begin() const noexcept { return _children.cbegin(); }
    [[nodiscard]]       auto children_begin()       noexcept { return _children.begin();  }

    [[nodiscard]] const auto children_end() const noexcept { return _children.cend(); }
    [[nodiscard]]       auto children_end()       noexcept { return _children.end();  }

    template<typename F>
    requires std::invocable<F, const std::any&>
    void for_each_child(const F& func) const {
	std::for_each(children_begin(), children_end(), func);
    }

    template<typename F>
    requires std::invocable<F, std::any&>
    void for_each_child(const F& func) {
	std::for_each(children_begin(), children_end(), func);
    }
};

template<>
class node_children<void> {};


template<typename ChildrenContainer>
requires std::ranges::random_access_range<ChildrenContainer>
class node_random_access : public node_children<ChildrenContainer> {
public:
    [[nodiscard]] constexpr std::size_t children_size() const noexcept { return this->children().size(); }

    [[nodiscard]] constexpr const std::any& child_at(std::size_t n) const { return this->children().at(n); }
    [[nodiscard]] constexpr       std::any& child_at(std::size_t n)       { return this->children().at(n); }

    template<typename T, typename... Args>
    constexpr auto child_emplace_at(typename ChildrenContainer::const_iterator pos, Args&&... args) {
	auto iter = std::ranges::next(this->children_begin(), children_size(), pos);

	if(iter == this->children_end())
	    throw std::out_of_range{"node_random_access: emplace_at: recieved iterator outside the range"};

	iter->template emplace<T>(std::forward<Args>(args)...);
	return iter;
    }
};

}
