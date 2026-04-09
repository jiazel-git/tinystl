#include "array.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

void test_basic_operations() {
    jz::array< int, 5 > arr = { 1, 2, 3, 4, 5 };

    assert( arr.size() == 5 );
    assert( arr.max_size() == 5 );
    assert( !arr.empty() );
    assert( arr[ 0 ] == 1 );
    assert( arr[ 4 ] == 5 );
    assert( arr.front() == 1 );
    assert( arr.back() == 5 );
    assert( arr.at( 0 ) == 1 );
    assert( arr.at( 4 ) == 5 );
    assert( arr.data() != nullptr );
}

void test_empty_array() {
    jz::array< int, 0 > empty_arr;
    assert( empty_arr.size() == 0 );
    assert( empty_arr.max_size() == 0 );
    assert( empty_arr.empty() );
}

void test_bounds_checking() {
    jz::array< int, 3 > arr = { 1, 2, 3 };

    bool caught_positive = false;
    try {
        ( void )arr.at( 3 );
    } catch ( const std::out_of_range& ) {
        caught_positive = true;
    }
    assert( caught_positive );

    bool caught_negative = false;
    try {
        ( void )arr.at( -1 );
    } catch ( const std::out_of_range& ) {
        caught_negative = true;
    }
    assert( caught_negative );
}

void test_fill() {
    jz::array< int, 4 > arr;
    arr.fill( 42 );
    for ( auto& x : arr ) {
        assert( x == 42 );
    }
}

void test_swap() {
    jz::array< int, 3 > arr1 = { 1, 2, 3 };
    jz::array< int, 3 > arr2 = { 4, 5, 6 };

    arr1.swap( arr2 );

    assert( arr1[ 0 ] == 4 );
    assert( arr1[ 1 ] == 5 );
    assert( arr1[ 2 ] == 6 );
    assert( arr2[ 0 ] == 1 );
    assert( arr2[ 1 ] == 2 );
    assert( arr2[ 2 ] == 3 );
}

void test_iterators() {
    jz::array< int, 5 > arr = { 1, 2, 3, 4, 5 };

    int sum = 0;
    for ( auto& x : arr ) {
        sum += x;
    }
    assert( sum == 15 );

    sum = 0;
    for ( auto it = arr.begin(); it != arr.end(); ++it ) {
        sum += *it;
    }
    assert( sum == 15 );

    sum = 0;
    for ( auto it = arr.cbegin(); it != arr.cend(); ++it ) {
        sum += *it;
    }
    assert( sum == 15 );
}

void test_reverse_iterators() {
    jz::array< int, 5 > arr = { 1, 2, 3, 4, 5 };

    int sum = 0;
    for ( auto it = arr.rbegin(); it != arr.rend(); ++it ) {
        sum += *it;
    }
    assert( sum == 15 );

    sum = 0;
    for ( auto it = arr.crbegin(); it != arr.crend(); ++it ) {
        sum += *it;
    }
    assert( sum == 15 );
}

void test_iterator_arithmetic() {
    jz::array< int, 5 > arr = { 1, 2, 3, 4, 5 };

    auto it = arr.begin();
    assert( *( it + 2 ) == 3 );
    assert( *( it += 3 ) == 4 );

    auto it2 = arr.end();
    assert( *( it2 - 2 ) == 4 );

    assert( arr.end() - arr.begin() == 5 );
}

void test_comparison() {
    jz::array< int, 5 > arr = { 1, 2, 3, 4, 5 };

    assert( arr.begin() < arr.end() );
    assert( arr.end() > arr.begin() );
    assert( !( arr.begin() > arr.end() ) );
    assert( !( arr.end() < arr.begin() ) );
}

void test_ctad() {
    jz::array arr = { 1, 2, 3, 4, 5 };
    assert( arr.size() == 5 );

    jz::array arr2 = { 1.0, 2.0, 3.0 };
    assert( arr2.size() == 3 );
}

int main() {
    test_basic_operations();
    test_empty_array();
    test_bounds_checking();
    test_fill();
    test_swap();
    test_iterators();
    test_reverse_iterators();
    test_iterator_arithmetic();
    test_comparison();
    test_ctad();

    std::cout << "All tests passed!\n";
    return 0;
}
