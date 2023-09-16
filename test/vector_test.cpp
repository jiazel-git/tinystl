#include "A.h"
#include "vector.hpp"
namespace jz
{
void test() {
    // 测试构造函数
    jz::vector<int> v;                                   // 默认构造函数
    jz::vector<int> v1(5);                               // 构造一个大小为5的空vector
    jz::vector<int> v2(5, 1);                            // 构造5个值为1的vector
    jz::vector<int> v3(v2);                              // 拷贝构造函数
    jz::vector<int> v4(v3.begin(), v3.begin() + 2);      // 范围拷贝构造函数
    jz::vector<int> v5{1, 2, 3, 4, 5};                   // 初始化列表构造函数
    jz::vector<int> v6(jz::vector<int>{6, 7, 8, 9, 10}); // 移动构造函数
    v5 = v6;                                             // 拷贝赋值
    jz::vector<A> va;
    for (int i = 0; i < v6.size(); i++) {
        std::cout << v6[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < v6.size(); i++) {
        std::cout << v6.at(i) << " ";
    }
    std::cout << std::endl;
    try {
        std::cout << v6.at(6) << std::endl;
    } catch (std::exception& e) {
        std::cout << e.what();
    }
    for (auto it = v5.begin(); it != v5.end(); ++it) {
        cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "v5.size()---------->" << v5.size() << std::endl;
    std::cout << "v5.capacity()---------->" << v5.capacity() << std::endl;
    v5.push_back(11);
    std::cout << "v5.size()---------->" << v5.size() << std::endl;
    std::cout << "v5.capacity()---------->" << v5.capacity() << std::endl;
    v5.resize(7);
    std::cout << v5 << std::endl;
    std::cout << "v5.size()---------->" << v5.size() << std::endl;
    std::cout << "v5.capacity()---------->" << v5.capacity() << std::endl;
    jz::vector<int> v7;
    v7.assign(5, 8);
    std::cout << v7 << std::endl;
    std::cout << "v7.front()--------->" << v7.front() << std::endl;
    std::cout << "v7.back()--------->" << v7.back() << std::endl;
    std::cout << "v7.empty()---------->" << v7.empty() << std::endl;
    v7.clear();
    std::cout << "v7.clear()---------" << std::endl;
    std::cout << "v7.empty()---------->" << v7.empty() << std::endl;
    std::cout << "v7.size()---------->" << v7.size() << std::endl;
    std::cout << "v7.capacity()---------->" << v7.capacity() << std::endl;
    v7.resize(10, 5);
    std::cout << "v7.capacity()---------->" << v7.capacity() << std::endl;
    v7.insert(v7.end(), {6, 7, 8, 9, 10});
    std::cout << v7 << endl;
}
} // namespace jz