#include <iostream>
// #include "iterator.cpp"
// #include "alloc.cpp"
// #include "iterator_test.cpp"
// #include "iterator_category.cpp"
// #include "vector_test.cpp"
// #include "Vector_test.cpp"
#include "list.hpp"
#include <list>
struct comp
{
    bool operator()(const int& x) {
        return x % 2 == 0;
    }
};
int main(int argc, char** argv) {
    // jz::test();
    jz::list<int> l{1, 2, 3, 4, 5};
    l.insert(l.end(), 5, 6);
    l.pop_back();
    l.push_back(7);
    l.push_front(0);
    l.resize(12, 8);
    std::cout << l << std::endl;
    l.remove_if(comp());
    std::cout << std::endl;
    std::cout << l << std::endl;
    return 0;
}
