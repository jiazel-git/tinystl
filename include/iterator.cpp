#include <iostream>
#include <vector>
#include "A.h"
using namespace std;
namespace jz
{

void test() {
    A a1(7.2606, "USD/CNY");
    A a2(146.2300, "USD/JPY");
    A a3(0.9279, "USD/EUR");
    A a4(1.0773, "EUR/USD");
    vector<A> v{a1, a2, a3, a4};
    // 不使用迭代器访问A
    cout << "不使用迭代器访问A----------" << endl;
    for (auto& it : v) {
        cout << it;
    }
    // 使用迭代器访问A
    cout << "使用迭代器访问A------------" << endl;
    for (auto it = v.begin(); it != v.end(); ++it) {
        auto ret = it.operator->();
        // cout << it->get_currency() << endl;
        cout << *ret;
        cout << ret;
    }
    cout << "----------------------------" << endl;
    for (auto it = v.begin(); it != v.end(); ++it) {
        auto ret = *it;
        cout << &ret;
        cout << ret;
    }
    cout << "以下是对迭代器的一些其他操作" << endl;
    vector<A>::iterator itBegin = v.begin();     // 取容器第一个元素的迭代器
    vector<A>::iterator itEnd   = v.end();       // 取容器的尾后迭代器
    vector<A>::value_type aa(0.8559, "EUR/GBP"); // A
    vector<A>::reference ra = aa;                // A&
    vector<A>::pointer pa   = &aa;               // A*
    vector<A>::difference_type dt;               // ptrdiff_t
    vector<A>::size_type st;                     // size_t
    cout << *(itBegin++);                        // 对迭代器进行后++操作
    cout << *(++itBegin);                        // 对迭代器进行前++操作
    cout << (itBegin == itEnd) << endl;          // 判断两个迭代器是否相等
    cout << (itBegin < itEnd) << endl;           // 判断两个迭代器的大小
    cout << (itEnd - itBegin) << endl;           // 计算两个迭代器之间的距离
}

} // namespace jz