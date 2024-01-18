#include "algorithm.h"
#include "vector.hpp"
#include <functional>
#include <iostream>

namespace jz {
void test() {
    vector< int > vec{ 1, 2, 3, 4, 5, 3, 2, 2, 2 };
    // test for_each
    jz::for_each( vec.begin(), vec.end(), []( int val ) { std::cout << val << " "; } );
    std::cout << std::endl;
    // test count_if
    auto count = jz::count_if( vec.begin(), vec.end(), std::bind( std::equal_to<>{}, 2, std::placeholders::_1 ) );
    std::cout << "count=" << count << std::endl;
}
}  // namespace jz