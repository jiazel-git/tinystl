#pragma once
#include <memory>
#include <initializer_list>
#include <iostream>
#include "iterator.hpp"
using namespace std;
namespace jz
{
template <typename T, typename Alloc = std::allocator<T>>
class vector
{
public:
    using value_type       = T;
    using reference        = T&;
    using pointer          = T*;
    using size_type        = size_t;
    using difference_type  = ptrdiff_t;
    using const_pointer    = const T*;
    using const_reference  = const T&;
    using iterator         = T*;
    using const_iterator   = const T*;
    using allocator        = Alloc;
    using reverse_iterator = reverse_iterator<iterator>;

public:
    vector() : _start(nullptr), _end(nullptr), _capacity(nullptr) {}
    explicit vector(size_type n) { construct(n, value_type()); }
    vector(size_type n, const value_type& val) {
        construct(n, val);
    }
    vector(const vector& other) {
        size_t n = other._capacity - other._start;
        construct(n, other.cbegin(), other.cend());
    }
    template <typename InputIterator,
              typename Require = RequireInputIterator<InputIterator>>
    vector(InputIterator first, InputIterator last) {
        auto n = static_cast<difference_type>(last - first);
        construct(n, first, last);
    }
    vector(vector&& other) {
        swap(other);
    }
    vector& operator=(const vector& rh) {
        auto copy = rh;
        swap(copy);
        return *this;
    }
    vector& operator=(vector&& rh) {
        if (this != rh.data()) {
            swap(rh);
        }
        return *this;
    }
    vector(initializer_list<value_type> il) {
        auto n = il.size();
        construct(n, il.begin(), il.end());
    }
    friend ostream& operator<<(ostream& os, vector& v) {
        for (auto& it : v) {
            os << it << " ";
        }
        return os;
    }
    size_t capacity() const { return static_cast<size_t>(_capacity - _start); }
    size_t size() const { return static_cast<size_t>(_end - _start); }
    iterator begin() { return _start; }
    const_iterator cbegin() const { return const_cast<const_iterator>(_start); }
    reverse_iterator rbegin() const { return _end; }
    reverse_iterator rend() const { return _start; }
    iterator end() { return _end; }
    const_iterator cend() const { return const_cast<const_iterator>(_end); }
    pointer data() const { return const_cast<const_pointer>(_start); }
    pointer data() { return _start; }
    reference operator[](size_type index) const { return *(_start + index); }
    reference at(size_type index) const {
        if (index < 0 || index > size()) {
            throw out_of_range("out of range\n");
        }
        return operator[](index);
    }
    reference back() {
        if (empty()) {
            throw length_error("this vector is empty\n");
        }
        return _start[size() - 1];
    }
    const_reference back() const {
        if (empty()) {
            throw length_error("this vector is empty\n");
        }
        return const_cast<const_reference>(_start[size() - 1]);
    }
    void clear() noexcept {
        destruct(_start, _end);
        _end = _start;
    }
    reference front() {
        if (empty()) {
            throw length_error("this vector is empty\n");
        }
        return *begin();
    }
    const_reference front() const {
        if (empty()) {
            throw length_error("this vector is empty\n");
        }
        return const_cast<const_reference>(*begin());
    }
    void swap(vector& other) noexcept {
        using std::swap;
        swap(_start, other._start);
        swap(_end, other._end);
        swap(_capacity, other._capacity);
    }
    void push_back(const value_type& val) {
        if (check_capacity()) {
            expand();
        }
        _alloc.construct(_end++, val);
    }
    void push_back(value_type&& val) {
        if (check_capacity()) {
            expand();
        }
        new (_end++) value_type(std::move(val));
    }
    void pop_back() {
        if (empty()) {
            throw length_error("this vector is empty\n");
        }
        _alloc.destroy(_end--);
    }
    bool empty() const { return _start == _end; }
    // 改变容器大小，且创建对象
    void resize(size_type n) {
        if (n <= capacity()) {
            if (n < size()) {
                difference_type distance = size() - n;
                _end                     = _start + n;
                destruct(_end, _end + n);
            } else {
                difference_type distance = n - size();
                while (distance--) {
                    *(_end++) = value_type();
                }
            }
        } else {
            auto new_capacity = n;
            expand_enough(new_capacity);
            difference_type distance = n - size();
            while (distance--) {
                *(_end++) = value_type();
            }
        }
    }
    void resize(size_type n, const value_type& val) {
        if (n <= capacity()) {
            if (n < size()) {
                difference_type distance = size() - n;
                _end                     = _start + n;
                destruct(_end, _end + n);
            } else {
                difference_type distance = n - size();
                while (distance--) {
                    *(_end++) = val;
                }
            }
        } else {
            auto new_capacity = n;
            expand_enough(new_capacity);
            difference_type distance = n - size();
            while (distance--) {
                *(_end++) = val;
            }
        }
    }
    // 改变容器大小，不创建对象
    void reserve(size_type n) {
        if (n > capacity()) {
            expand_enough(n);
        }
    }
    template <typename InputIterator,
              typename Require = RequireInputIterator<InputIterator>>
    void assign(InputIterator first, InputIterator last) {
        auto n = last - first;
        if (n <= capacity()) {
            auto new_end = _start;
            while (n--) {
                *(new_end++) = *(first++);
            }
            _end = new_end;
        } else {
            if (_start) {
                destroy();
            }
            construct(size_type(n), first, last);
        }
    }
    void assign(size_type n, const value_type& val) {
        if (n <= capacity()) {
            auto new_end = _start;
            while (n--) {
                *(new_end++) = val;
            }
            _end = new_end;
        } else {
            if (_start) {
                destroy();
            }
            construct(n, val);
        }
    }
    void assign(initializer_list<value_type> il) {
        assign(il.size(), il.begin(), il.end());
    }
    iterator erase(const_iterator pos) {
        if (pos < cbegin() || pos >= cend()) {
            throw out_of_range("this posing is invalid\n");
        }
        auto p = const_cast<iterator>(pos);
        if (pos + 1 != cend()) {
            std::move(pos + 1, cend(), p);
        }
        _alloc.destroy(--_end);
        return p;
    }
    iterator erase(const_iterator first, const_iterator last) {
        if (!(first >= cbegin() && last <= cend())) {
            throw out_of_range("this range is invaild\n");
        }
        auto iter = std::move(const_cast<iterator>(last), _end, const_cast<iterator>(first));
        destruct(first, last);
        _end = iter;
        return to_non_const(first);
    }
    iterator insert(const_iterator pos, const value_type& val) {
        auto p = const_cast<iterator>(pos);
        if (_end == _capacity) {
            auto offset = pos - const_cast<const_iterator>(_start);
            expand();
            p = _start + offset;
        }
        size_t distance = static_cast<size_t>(_end - pos);
        auto tmp        = _end - 1;
        _end++;
        while (distance--) {
            *(tmp + 1) = std::move(*tmp);
            --tmp;
        }
        *p = val;
        return p;
    }
    iterator insert(const_iterator pos, size_type n, const value_type& val) {
        auto p        = const_cast<iterator>(pos);
        auto distance = _end - pos;
        if (_end + n > _capacity) {
            auto offset       = pos - const_cast<const_iterator>(_start);
            auto new_capacity = size() + n;
            expand_enough(new_capacity);
            p = _start + offset;
        }

        auto tmp = _end - 1;
        _end += n;
        while (distance--) {
            *(tmp + n) = std::move(*tmp);
            --tmp;
        }
        auto q = p;
        while (n--) {
            *(q++) = val;
        }
        return p;
    }
    template <typename InputIterator,
              typename Require = RequireInputIterator<InputIterator>>
    iterator insert(const_iterator pos, InputIterator first, InputIterator last) {
        auto p        = const_cast<iterator>(pos);
        auto distance = _end - pos;
        size_t n      = last - first;
        if (_end + n > _capacity) {
            auto offset       = pos - const_cast<const_iterator>(_start);
            auto new_capacity = size() + (last - first);
            expand_enough(new_capacity);
            p = _start + offset;
        }
        auto tmp = _end - 1;
        _end += n;
        while (distance--) {
            *(tmp + n) = std::move(*tmp);
            --tmp;
        }
        auto q = p;
        for (; first != last; ++first) {
            *(q++) = std::move(*first);
        }
        return p;
    }
    iterator insert(const_iterator pos, value_type&& val) {
        auto p = const_cast<iterator>(pos);
        if (_end == _capacity) {
            auto offset = pos - const_cast<const_iterator>(_start);
            expand();
            p = _start + offset;
        }
        size_t distance = static_cast<size_t>(_end - pos);
        auto tmp        = _end - 1;
        _end++;
        while (distance--) {
            *(tmp + 1) = std::move(*tmp);
            --tmp;
        }
        *p = val;
        return p;
    }
    iterator insert(const_iterator pos, initializer_list<value_type> il) {
        return insert(pos, il.begin(), il.end());
    }
    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (_end == _capacity) {
            expand();
        }
        _alloc.construct(_end++, std::forward<Args>(args)...);
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
    template <typename InputIterator>
    void construct(size_t n, InputIterator first, InputIterator last) {
        _start    = _alloc.allocate(n);
        _end      = _start;
        _capacity = _start + n;
        while (first != last) {
            *(_end++) = *(first++);
        }
    }
    void destroy() {
        size_t n = _capacity - _start;
        _alloc.deallocate(_start, n);
    }
    bool check_capacity() {
        return _end == _capacity;
    }
    void expand() {
        size_type new_capacity = empty() ? 1 : size() * 2;
        auto new_start         = _alloc.allocate(new_capacity);
        auto new_end           = new_start;
        auto new_end_of_stroge = new_start + new_capacity;
        auto tmp               = _start;
        size_type n            = size();
        while (n--) {
            *(new_end++) = std::move(*(tmp++));
        }
        destroy();
        _start    = new_start;
        _end      = new_end;
        _capacity = new_end_of_stroge;
    }
    void destruct(iterator first, iterator end) {
        for (auto it = first; it != end; ++it) {
            _alloc.destroy(it);
        }
    }
    void expand_enough(size_t n) {
        auto old_copy = *this;
        _start        = _alloc.allocate(n);
        _end          = _start;
        _capacity     = _start + n;
        for (auto it = old_copy.begin(); it != old_copy.end(); ++it) {
            *(_end++) = std::move(*it);
        }
    }

private:
    iterator _start;
    iterator _end;
    iterator _capacity;
    allocator _alloc;
};

} // namespace jz