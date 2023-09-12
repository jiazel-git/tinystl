#include <iostream>
// #include "iterator.cpp"
// #include "alloc.cpp"
// #include "iterator_test.cpp"
// #include "iterator_category.cpp"
#include "vector_test.cpp"
#include <vector>
int main(int argc, char** argv) {
    std::vector<int> v;
    v.push_back(1);
    v.emplace(v.begin(), 1, 2);
    v.emplace_back(1);
    std::allocator<int> alloc;
    alloc.construct((void*)0, 1);
    for (auto& it : v) {
        cout << it << " ";
    }
    return 0;
}
