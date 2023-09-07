/*
    测试STL各个容器的迭代器类型
*/
#include <vector>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <iterator>

using namespace std;
namespace jz
{

void _display(input_iterator_tag) {
    cout << "input_iterator_tag" << endl;
}
void _display(output_iterator_tag) {
    cout << "output_iterator_tag" << endl;
}
void _display(forward_iterator_tag) {
    cout << "forward_iterator_tag" << endl;
}
void _display(bidirectional_iterator_tag) {
    cout << "bidirectional_iterator_tag" << endl;
}
void _display(random_access_iterator_tag) {
    cout << "random_access_iterator_tag" << endl;
}
template <typename T>
void display(T Itr) {
    typename iterator_traits<T>::iterator_category cagy;
    _display(cagy);
    cout << "typeid(Itr).name()=" << typeid(Itr).name() << endl;
}

void test() {
    display(array<int, 1>::iterator());
    display(vector<int>::iterator());
    display(forward_list<int>::iterator());
    display(list<int>::iterator());
    display(deque<int>::iterator());
    display(set<int>::iterator());
    display(map<int, int>::iterator());
    display(unordered_map<int, int>::iterator());
    display(unordered_set<int>::iterator());
    display(istream_iterator<int>());
    display(ostream_iterator<int>(cout, ""));
}

} // namespace jz