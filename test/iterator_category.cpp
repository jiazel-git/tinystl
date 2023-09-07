#include "iterator.hpp"
#include <iostream>

namespace jz
{

void _display(input_iterator_tag) {
    std::cout << "input_iterator_tag" << std::endl;
}
void _display(output_iterator_tag) {
    std::cout << "output_iterator_tag" << std::endl;
}
void _display(forward_iterator_tag) {
    std::cout << "forward_iterator_tag" << std::endl;
}
void _display(bidirectional_iterator_tag) {
    std::cout << "bidirectional_iterator_tag" << std::endl;
}
void _display(random_access_iterator_tag) {
    std::cout << "random_access_iterator_tag" << std::endl;
}
template <class Category>
void display(Category cagy) {
    _display(Category());
}
void test() {
    display(jz::input_iterator_tag());
    display(jz::output_iterator_tag());
    display(jz::forward_iterator_tag());
    display(jz::bidirectional_iterator_tag());
    display(jz::random_access_iterator_tag());
}

} // namespace jz