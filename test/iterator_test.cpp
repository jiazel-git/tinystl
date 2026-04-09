#include "iterator.hpp"
#include <iostream>
#include <vector>
using namespace std;
namespace jz
{
void testIterator() {
    vector< int > v{ 1, 2, 3, 4, 5 };
    auto          rbegin = jz::make_reverse_iterator( v.end() );
    auto          rend   = jz::make_reverse_iterator( v.begin() );
    auto          tmpbegin( rbegin );
    auto          tmpend( rend );
    for ( ; rbegin != rend; ++rbegin ) {
        cout << *rbegin << " ";
    }
    cout << endl;
    for ( ; tmpbegin != tmpend; ++tmpbegin ) {
        cout << *( tmpbegin.operator->() ) << " ";
    }
    cout << endl;
    tmpbegin = jz::make_reverse_iterator( v.end() );
    cout << tmpbegin[ 3 ];
}

}  // namespace jz

int main() {
    jz::testIterator();
    return 0;
}
