#ifndef _JZALLOC_
#define _JZALLOC_

#include <new>      // for placement new
#include <cstddef>  // for pridiff_t, size_t
#include <cstdlib>  // for exit()
#include <climits>  // for UINT_MAX
#include <iostream> // for cerr

namespace jz
{

// 分配内存
template <typename T>
inline T* _allocate(ptrdiff_t size, T*) {
    std::set_new_handler(0);
    T* ret = (T*)::operator new((size_t)(size * sizeof(T)));
    if (ret == 0) {
        std::cout << "out of memory" << std::endl;
        exit(1);
    }
    return ret;
}
// 释放内存
template <typename T>
inline void _deallocate(T* ptr) {
    ::operator delete(ptr);
}

// 构造
template <typename T1, typename T2>
inline void _construct(T1* p, const T2& value) {
    new (p) T1(value);
}

// 销毁
template <typename T>
inline void _destory(T* ptr) {
    ptr->~T();
}

// 定义allocator
template <typename T>
class allocator
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    pointer allocate(size_type n, const void* hint = 0) {
        return _allocate((difference_type)n, (pointer)0);
    }
    void deallocate(pointer p, size_type n) { _deallocate(p); }
    void construct(pointer p, value_type value) { _construct(p, value); }
    void destory(pointer p) { _destory(p); }
    pointer address(reference x) { return (pointer)&x; }
    const_pointer const_address(const_reference x) { return (const_pointer)&x; }
    size_type max_size() const {
        return size_type(UINTMAX_MAX / sizeof(T));
    }
};
}; // namespace jz

#endif