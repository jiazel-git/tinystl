#include <iostream>
// #include "iterator.cpp"
// #include "alloc.cpp"
// #include "iterator_test.cpp"
// #include "iterator_category.cpp"
// #include "vector_test.cpp"
// #include "Vector_test.cpp"
#include "list.hpp"
#include "stack.hpp"
#include "vector.hpp"
#include <forward_list>

struct comp {
    bool operator()( const int& x ) {
        return x % 2 == 0;
    }
};
int main( int argc, char** argv ) {
    // jz::test();
    jz::list< int >     l{ 1, 2, 3, 4, 5 };
    auto                it = l.begin();
    forward_list< int > fl{ 1, 2, 3, 4, 5 };
    fl.push_front( 6 );
    for ( auto& it : fl ) {
        cout << it << " ";
    }
    return 0;
}
