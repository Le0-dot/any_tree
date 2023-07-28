# any_tree
A small c++ header-only library for creating trees with any kinds of nodes

## Usage
To use library in your source code just clone repository and add these lines in your CMakeLists.txt
```cmake
add_subdirectory(any_tree)
target_link_library(${PROJECT_NAME} PUBLIC any_tree)
```

## Features
any_tree allows one to create trees with any number of children and holding any kind of values. Most importantly, in one tree nodes could hold different types of values and have different number of children.

To differentiate between node types visitor pattern is used. You can put different functions to be called with particular node types in an ```std::unordered_map``` (```children_visitor``` and ```const_children_visitor``` aliases are provided). You functions could return any kind of type (not only ```void```). Additionally, library supports const and non-const visitors, if you want to change you nodes while traversing a tree.

In node template instantiation, if you put either a ```Payload``` type (type of value that node holds) or a ```ChildrenContainer``` type (a c++20 range constainer) to void, then they would just not be created. Unlike ```std::array<T, 0>``` which still takes 1 byte, putting ```void``` in his stead would not waste any space.
