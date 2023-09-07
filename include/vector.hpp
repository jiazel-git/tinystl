#pragma once
#include <memory>
#include <initializer_list>
#include <iostream>
using namespace std;
namespace jz
{
template <typename T, typename Alloc = std::allocator<T>>
class vector
{
public:
    using value_type             = T;
    using reference              = T&;
    using pointer                = T*;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using iterator               = T*;
    using const_iterator         = const T*;
    using const_pointer          = const T*;
    using const_reference        = const T&;
    using allocator              = Alloc;
    using reverse_iterator       = T*;
    using const_reverse_iterator = const T*;

public:
    vector() : _start(nullptr), _end(nullptr), _capacity(nullptr) {}
    explicit vector(size_type n) { construct(n, value_type()); }
    vector(size_type n, const value_type& val) {
        construct(n, val);
    }
    vector(const vector& orher) {
        size_t n = orher._capacity - orher._start;
    }
    vector(vector&&) {}
    vector& operator=(const vector& rh) {}
    vector& operator=(vector&& rh) {}
    vector(initializer_list<value_type> il) {}
    friend ostream& operator<<(ostream& os, vector& v) {
        iterator it = v._start;
        for (; it != v._end; ++it) {
            os << *it << " ";
        }
        return os;
    }
    ~vector() {
        destroy();
    }

private:
    void construct(size_t n, const value_type& val) {
        _start    = _alloc.allocate(n);
        _end      = _start;
        _capacity = _start + n;
        while (n--) {
            _alloc.construct(_end++, val);
        }
    }
    void destroy() {
        size_t n = _capacity - _start;
        _alloc.deallocate(_start, n);
    }

private:
    iterator _start;
    iterator _end;
    iterator _capacity;
    allocator _alloc;
};

} // namespace jz