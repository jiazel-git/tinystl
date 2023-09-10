#include <iostream>
// #include "iterator.cpp"
// #include "alloc.cpp"
// #include "iterator_test.cpp"
// #include "iterator_category.cpp"
#include <vector>
#include "vector.hpp"
int main(int argc, char** argv) {
    // jz::test();
    jz::vector<int> v{1, 2};
    cout << v.capacity() << endl;
    v.insert(v.begin(), 5);
    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    return 0;
}
