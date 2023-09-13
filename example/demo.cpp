#include <any>
#include <iostream>
#include <stdexcept>
#include <functional>

#include <any_tree.hpp>

/////////////////////////////////////////
///               USAGE               ///
/////////////////////////////////////////

using namespace any_tree;

struct data {
    int a, b;
};

using root_node = dynamic_node<void>; // based on std::vector, no payload
using child1_node = static_node<data, 2>; // base on std::array of size 2
using child2_node = leaf<data>; // node with no children, i.e. leaf
using child3_node = leaf<void>; // node with no children and no payload, i.e. class {};

int main() {
    std::any n{root_node()}; // root node of the tree

    std::any_cast<root_node&>(n).children().emplace_back(child1_node(1, 2)); // access vector and push back some elements
    std::any_cast<root_node&>(n).children().emplace_back(child2_node(1)); // pass 1 to constructor of data
    std::any_cast<root_node&>(n).children().emplace_back(child3_node());

    std::cout << std::any_cast<child1_node&>(std::any_cast<root_node&>(n).children().front()).children_size() << '\n'; // make sure that everything is alright

    // const visitors that return void
    const_children_visitor<void> cv {
	make_const_child_visitor<root_node>([&cv] (const root_node& n) {
		std::cout << "const dynamic_node<void>&\n"; 
		n.for_each_child([&cv] (const std::any& n) { visit_node<void>(cv, n); });
	}),
	make_const_child_visitor<child1_node>([&cv] (const child1_node& n) { 
	    std::cout << "const static_node<data, 2>&\n"; 
	    n.for_each_child([&cv] (const std::any& n) { visit_node<void>(cv, n); });
	}),
	make_const_child_visitor<child2_node>([&cv] (const child2_node&) { std::cout << "const leaf<data>&\n"; }),
	make_const_child_visitor<child3_node>([&cv] (const child3_node&) { std::cout << "const leaf<void>&\n"; }),
	make_const_child_visitor<void>([] () { std::cout << "empty node\n"; }),
    };

    // apply const visitors
    visit_node(cv, n);


    // visitors that return int
    children_visitor<int> v {
	make_child_visitor<root_node>([] (root_node&) { std::cout << "root_node&\n"; return 1; }),
	make_child_visitor<child1_node>([] (child1_node& n) { std::cout << "child1_node&\n"; return 2; }),
	make_child_visitor<child2_node>([] (child2_node&) { std::cout << "child2_node&\n"; return 3; }),
	make_child_visitor<child3_node>([] (child3_node&) { std::cout << "child3_node&\n"; return 3; }),
    };

    // apply visitors
    std::cout << visit_node(v, n) << '\n';

    for(auto& c: std::any_cast<root_node&>(n).children())
	std::cout << visit_node(v, c) << '\n';

    // will throw
    try {
	auto c = std::any_cast<root_node>(n).child_at(3);
    } catch (const std::out_of_range& e) {
	std::cout << e.what() << '\n';
    }

    auto iter = std::any_cast<const root_node&>(n).children_begin();
    std::any_cast<root_node&>(n).child_emplace_at<leaf<data>>(iter, leaf<data>{});

    try {
	auto iter2 = std::any_cast<const root_node&>(n).children_end();
	std::any_cast<root_node&>(n).child_emplace_at<leaf<int>>(iter2, leaf<int>());
    } catch (const std::out_of_range& e) {
	std::cout << e.what() << '\n';
    }

    // apply const visitors again to see changes
    visit_node(cv, n);

    return 0;
}
