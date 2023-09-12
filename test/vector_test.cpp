#include "vector.hpp"
#include "A.h"
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
    jz::vector<A> va;
    va.emplace_back(7.2925, "USD/CNY");
    cout << v1.size() << endl;
    cout << v1.capacity() << endl;
    v1.push_back(1);
    v1.emplace_back(5);
    for (int i = 0; i < va.size(); ++i) {
        cout << va[i] << " ";
    }
}
} // namespace jz