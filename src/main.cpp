#include <iostream>
// #include "iterator.cpp"
// #include "alloc.cpp"
// #include "iterator_test.cpp"
// #include "iterator_category.cpp"
// #include "vector_test.cpp"
// #include "Vector_test.cpp"
#include "algorithm_test.cpp"
#include "list.hpp"
#include "stack.hpp"
#include "vector.hpp"
#include <algorithm>
#include <forward_list>

struct comp {
    bool operator()( const int& x ) {
        return x % 2 == 0;
    }
};
int main( int argc, char** argv ) {
    jz::test();
}
