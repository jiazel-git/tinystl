#pragma once
#include <memory>
#include "iterator.hpp"
namespace jz
{
template <class T>
struct list_node
{
    using node_raw_ptr   = list_node*;
    using node_ptr       = std::unique_ptr<list_node>;
    explicit list_node() = default;
    explicit list_node(const T& value, node_raw_ptr prev, node_ptr&& next)
        : _data(value), _prev(prev), _next(std::move(next)) {}
    explicit list_node(T&& value, node_raw_ptr prev, node_ptr&& next)
        : _data(std::move(value)), _prev(prev), _next(std::move(next)) {}
    explicit list_node(const T& value = T{})
        : _data(value), _prev(nullptr), _next(nullptr) {}
    list_node(const list_node& other)            = delete;
    list_node& operator=(const list_node& other) = delete;
    list_node(list_node&& other)                 = delete;
    list_node& operator=(list_node&& other)      = delete;
    T _data;
    node_raw_ptr _prev;
    node_ptr _next;
};
template <class T>
class list_const_iterator
{
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using pointer           = const T*;
    using reference         = const T&;
    using difference_type   = ptrdiff_t;

public:
    list_const_iterator() : _cur(nullptr) {}
    list_const_iterator(const list_const_iterator& other) : _cur(other._cur) {}
    list_const_iterator& operator=(const list_const_iterator& other) {
        auto copy = other;
        swap(copy);
    }
    void swap(list_const_iterator& other) {
        using std::swap;
        swap(_cur, other._cur);
    }
    reference operator*() { return _cur->_data; }
    pointer operator->() { return &(operator*()); }
    list_const_iterator& operator++() {
        _cur = _cur->_next.get();
        return *this;
    }
    list_const_iterator operator++(int) {
        auto tmp = *this;
        _cur     = _cur->_next.get();
        return tmp;
    }
    list_const_iterator& operator--() {
        _cur = _cur->_prev;
        return *this;
    }
    list_const_iterator operator--(int) {
        auto tmp = *this;
        _cur     = _cur->_prev;
        return tmp;
    }
    pointer data() {
        return _cur;
    }

private:
    pointer _cur;
};
template <class T>
class list_iterator : public list_const_iterator<T>
{
public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;
    using difference_type   = ptrdiff_t;

public:
    list_iterator() : _cur(nullptr) {}
    list_iterator(const list_iterator& other) : _cur(other._cur) {}
    list_iterator& operator=(const list_iterator& other) {
        auto copy = other;
        swap(copy);
    }
    void swap(list_iterator& other) {
        using std::swap;
        swap(_cur, other._cur);
    }
    reference operator*() { return _cur->_data; }
    pointer operator->() { return &(operator*()); }
    list_iterator& operator++() {
        _cur = _cur->_next.get();
        return *this;
    }
    list_iterator operator++(int) {
        auto tmp = *this;
        _cur     = _cur->_next.get();
        return tmp;
    }
    list_iterator& operator--() {
        _cur = _cur->_prev;
        return *this;
    }
    list_iterator operator--(int) {
        auto tmp = *this;
        _cur     = _cur->_prev;
        return tmp;
    }
    pointer data() {
        return _cur;
    }

private:
    pointer _cur;
};
template <class T>
bool operator==(const list_iterator<T>& left, const list_iterator<T>& right) {
    return left.data() == right.data();
}
template <class T>
bool operator!=(const list_iterator<T>& left, const list_iterator<T>& right) {
    return !(left == right);
}
template <class T>
bool operator<(const list_iterator<T>& left, const list_iterator<T>& right) {
    return left.data() < right.data();
}
template <class T>
bool operator<=(const list_iterator<T>& left, const list_iterator<T>& right) {
    return !(left.data() > right.data());
}
template <class T>
bool operator>(const list_iterator<T>& left, const list_iterator<T>& right) {
    return left.data() > right.data();
}
template <class T>
bool operator>=(const list_iterator<T>& left, const list_iterator<T>& right) {
    return !(left.data() < right.data());
}
template <class T>
void swap(list_iterator<T>& left, list_iterator<T>& right) {
    using std::swap;
    swap(left.data(), right.data());
}
template <class T>
class list
{
public:
    using node_raw_ptr = list_node<T>*;
    using node_ptr     = std::unique_ptr<list_node<T>>;

public:
    using value_type       = T;
    using reference        = T&;
    using const_reference  = const T&;
    using pointer          = T*;
    using const_pointer    = const T*;
    using size_type        = size_t;
    using difference_type  = ptrdiff_t;
    using iterator         = list_iterator<list_node<T>>;
    using const_iterator   = list_const_iterator<list_node<T>>;
    using reverse_iterator = reverse_iterator<iterator>;

public:
    list() {}
    list(size_type count, const T& value) {}
    explicit list(size_type count) {}
    template <class InputIterator,
              class Require = RequireInputIterator<InputIterator>>
    list(InputIterator first, InputIterator last) {}
    list(const list& other) {}
    list(list&& other) {}
    list(std::initializer_list<T> il) {}
    list& operator=(const list& other) {}
    list& operator=(list&& other) {}
    list& operator=(std::initializer_list<T> il) {}
    void assign(size_type count, const T& value) {}
    template <class InputIterator,
              class Require = RequireInputIterator<InputIterator>>
    void assign(InputIterator first, InputIterator last) {}
    void assign(std::initializer_list<T> il) {}
    reference front() {}
    const_reference front() const {}
    reference back() {}
    const_reference back() const {}
    iterator begin() {}
    const_iterator cbegin() const {}
    iterator end() {}
    const_iterator cend() const {}
    iterator rbegin() {}
    const_iterator crbegin() const {}
    iterator rend() {}
    const_iterator crend() const {}
    bool empty() {}
    size_type size() {}
    void clear() {}

    ~list() = delete;

private:
    static node_raw_ptr get_raw(const node_ptr& ptr) { return ptr.get(); }
    void init() {}
    node_ptr _head;
    node_raw_ptr _tail;
    size_t _size;
};
} // namespace jz