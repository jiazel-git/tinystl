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
    explicit list_node(const T& value)
        : _data(value), _prev(nullptr), _next(nullptr) {}
    list_node(const list_node& other)            = delete;
    list_node& operator=(const list_node& other) = delete;
    list_node(list_node&& other)                 = delete;
    list_node& operator=(list_node&& other)      = delete;
    T _data;
    node_raw_ptr _prev;
    node_ptr _next;
    using value_type      = T;
    using reference       = T&;
    using pointer         = T*;
    using const_reference = const T&;
    using const_pointer   = const T*;
};
template <class T>
class list_const_iterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename T::value_type;
    using pointer           = typename T::const_pointer;
    using reference         = typename T::const_reference;
    using difference_type   = ptrdiff_t;
    using node_raw_ptr      = T*;

public:
    list_const_iterator() = default;
    list_const_iterator(node_raw_ptr ptr) : _cur(ptr) {}
    list_const_iterator(const list_const_iterator& other) : _cur(other._cur) {}
    list_const_iterator(list_const_iterator&& other) : _cur(other._cur) {}
    list_const_iterator& operator=(const list_const_iterator& other) {
        auto copy = other;
        swap(copy);
        return *this;
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
    bool operator==(list_const_iterator other) { return _cur == other._cur; }
    bool operator!=(list_const_iterator other) {
        return !(this->operator==(other));
    }
    bool operator>(list_const_iterator other) {
        return _cur > other._cur;
    }
    bool operator<(list_const_iterator other) {
        return _cur < other._cur;
    }
    bool operator>=(list_const_iterator other) {
        return !(this->operator<(other));
    }
    bool operator<=(list_const_iterator other) {
        return !(this->operator>(other));
    }
    node_raw_ptr data() { return _cur; }

protected:
    node_raw_ptr _cur;
};
template <class T>
class list_iterator : public list_const_iterator<T>
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename T::value_type;
    using pointer           = typename T::pointer;
    using reference         = typename T::reference;
    using difference_type   = ptrdiff_t;
    using node_raw_ptr      = T*;
    ;

public:
    list_iterator() = default;
    list_iterator(node_raw_ptr ptr) : list_const_iterator<T>(ptr) {}
    list_iterator(const list_iterator& other) : list_const_iterator<T>(other) {}
    list_iterator(list_iterator&& other) : list_const_iterator<T>(std::move(other)) {}
    list_iterator& operator=(const list_iterator& other) {
        auto copy = other;
        swap(copy);
        return *this;
    }
    void swap(list_iterator& other) {
        using std::swap;
        swap(this->_cur, other._cur);
    }
    reference operator*() { return this->_cur->_data; }
    pointer operator->() { return &(operator*()); }
    list_iterator& operator++() {
        this->_cur = this->_cur->_next.get();
        return *this;
    }
    list_iterator operator++(int) {
        auto tmp   = *this;
        this->_cur = this->_cur->_next.get();
        return tmp;
    }
    list_iterator& operator--() {
        this->_cur = this->_cur->_prev;
        return *this;
    }
    list_iterator operator--(int) {
        auto tmp   = *this;
        this->_cur = this->_cur->_prev;
        return tmp;
    }
};
template <class T>
bool operator==(list_const_iterator<T>& left, list_const_iterator<T>& right) {
    return left.data() == right.data();
}
template <class T>
bool operator!=(list_const_iterator<T>& left, list_const_iterator<T>& right) {
    return !(left.data() == right.data());
}
template <class T>
bool operator<(list_const_iterator<T>& left, list_const_iterator<T>& right) {
    return left.data() < right.data();
}
template <class T>
bool operator<=(list_const_iterator<T>& left, list_const_iterator<T>& right) {
    return !(left.data() > right.data());
}
template <class T>
bool operator>(list_const_iterator<T>& left, list_const_iterator<T>& right) {
    return left.data() > right.data();
}
template <class T>
bool operator>=(list_const_iterator<T>& left, list_const_iterator<T>& right) {
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
    list() { init(); }
    list(size_type count, const T& value) {
        init();
        insert(cend(), count, value);
    }
    explicit list(size_type count) : list(count, value_type()) {}
    template <class InputIterator,
              class Require = RequireInputIterator<InputIterator>>
    list(InputIterator first, InputIterator last) {
        init();
        insert(cend(), first, last);
    }
    list(const list& other) {
        init();
        insert(cend(), other.cbegin(), other.cend());
    }
    list(list&& other) {
        init();
        swap(other);
    }
    list(std::initializer_list<T> il) {
        init();
        insert(cend(), il.begin(), il.end());
    }
    list& operator=(const list& other) {
        auto copy = other;
        swap(copy);
        return *this;
    }
    list& operator=(list&& other) {
        if (_head != other._head) {
            swap(other);
        }
        return *this;
    }

    list& operator=(std::initializer_list<T> il) {
        clear();
        insert(cend(), il.begin(), il.end());
        return *this;
    }
    void assign(size_type count, const T& value) {
        if (_size != 0) {
            clear();
        }
        insert(cend(), count, std::move(value));
    }
    template <class InputIterator,
              class Require = RequireInputIterator<InputIterator>>
    void assign(InputIterator first, InputIterator last) {
        if (_size != 0) {
            clear();
        }
        insert(cend(), first, last);
    }
    void assign(std::initializer_list<T> il) {
        if (_size != 0) {
            clear();
        }
        insert(cend(), il.begin(), il.end());
    }
    void swap(list& other) {
        using std::swap;
        swap(_head, other._head);
        swap(_tail, other._tail);
        swap(_size, other._size);
    }
    reference front() { return _head->_next->_data; }
    const_reference front() const { return const_cast<const_reference>(_head->_next->_data); }
    reference back() { return _tail->_prev->_data; }
    const_reference back() const { return const_cast<const_reference>(_tail->_prev->_data); }
    iterator begin() { return get_raw(_head->_next); }
    const_iterator cbegin() const { return get_raw(_head->_next); }
    iterator end() { return _tail; }
    const_iterator cend() const { return iterator{_tail}; }
    iterator rbegin() {}
    const_iterator crbegin() const {}
    iterator rend() {}
    const_iterator crend() const {}
    bool empty() { return get_raw(_head->_next) == _tail; }
    size_type size() { return _size; }
    void clear() noexcept {
        _head->_next = std::move(_tail->_prev->_next);
        _tail->_prev = get_raw(_head);
        _size        = 0;
    }
    template <class... Args>
    iterator emplace(const_iterator posion, Args&&... args) {
        auto prev     = posion.data()->_prev;
        auto cur      = std::move(prev->_next);
        auto new_node = std::make_unique<list_node<T>>(
            value_type(std::forward<Args>(args)...), prev, std::move(cur));
        new_node->_next->_prev = get_raw(new_node);
        new_node->_prev->_next = std::move(new_node);
        ++_size;
        return (--posion).data();
    }
    template <class... Args>
    void emplace_back(Args&&... args) {
        emplace(cend(), std::forward<Args>(args)...);
    }
    template <class... Args>
    void emplace_front(Args&&... args) {
        emplace(cbegin(), std::forward<Args>(args)...);
    }
    iterator insert(const_iterator posion, const value_type& value) {
        auto copy = value;
        return emplace(posion, std::move(copy));
    }
    iterator insert(const_iterator posion, size_type n, const value_type& value) {
        if (n == 0) {
            return posion.data();
        }
        auto firstinsert = insert(posion, value);
        --n;
        while (n--) {
            insert(posion, value);
        }
        return firstinsert.data();
    }
    template <class InputIterator,
              class Require = RequireInputIterator<InputIterator>>
    iterator insert(const_iterator posion, InputIterator first, InputIterator last) {
        if (first == last) {
            return posion.data();
        }
        auto firstinsert = insert(posion, *(first++));
        for (; first != last; ++first) {
            insert(posion, *first);
        }
        return firstinsert.data();
    }
    iterator insert(const_iterator posion, value_type&& value) {
        return emplace(posion, std::move(value));
    }
    iterator insert(const_iterator posion, std::initializer_list<T> il) {
        return insert(posion, il.begin(), il.end());
    }
    iterator erase(const_iterator posion) {
        auto prev   = posion.data()->_prev;
        auto ret    = std::move(posion.data()->_next);
        ret->_prev  = prev;
        prev->_next = std::move(ret);
        return get_raw(prev->_next);
    }
    iterator erase(const_iterator first, const_iterator last) {
        auto prev   = first.data()->_prev;
        last->_prev = prev;
        prev->_next = std::move(last);
        return get_raw(prev->_next);
    }
    void merge(list& x) {}
    void merge(list&& x) {}
    template <class Compare>
    void merge(list& x, Compare comp) {}
    template <class Compare>
    void merge(list&& x, Compare comp) {}
    void pop_back() {
        auto prev    = _tail->_prev->_prev;
        prev->_next  = std::move(prev->_next->_next);
        _tail->_prev = prev;
        --_size;
    }
    void pop_front() {
        _head->_next        = std::move(_head->_next->_next);
        _head->_next->_prev = get_raw(_head);
        --_size;
    }
    void push_back(const value_type& value) {
        insert(cend(), std::move(value));
    }
    void push_back(value_type&& value) {
        insert(cend(), std::move(value));
    }
    void push_front(const value_type& value) {
        insert(cbegin(), std::move(value));
    }
    void push_front(value_type&& value) {
        insert(cbegin(), std::move(value));
    }
    void remove(const value_type& value) {
        for (auto it = cbegin(); it != cend();) {
            if (*it == value) {
                it = erase(it);
            } else {
                ++it;
            }
        }
    }
    template <class Predicate>
    void remove_if(Predicate _Pred) {
        for (auto it = cbegin(); it != cend();) {
            if (_Pred(*it)) {
                it = erase(it);
            } else {
                ++it;
            }
        }
    }
    void resize(size_type n) {
        if (n < _size) {
            auto num = _size - n;
            while (num--) {
                pop_back();
            }
        } else {
            auto num = n - _size;
            while (num--) {
                push_back(value_type());
            }
        }
    }
    void resize(size_type n, const value_type& value) {
        if (n < _size) {
            auto num = _size - n;
            while (num--) {
                pop_back();
            }
        } else {
            auto num = n - _size;
            while (num--) {
                push_back(std::move(value));
            }
        }
    }
    void reverse() noexcept {}
    void sort() {}
    template <class Compare>
    void sort(Compare comp) {}
    void splice(const_iterator posion, list& x) {}
    void splice(const_iterator posion, list&& x) {}
    void splice(const_iterator position, list& x, const_iterator i) {}
    void splice(const_iterator position, list&& x, const_iterator i) {}
    void splice(const_iterator position, list& x, const_iterator first, const_iterator last) {}
    void splice(const_iterator position, list&& x, const_iterator first, const_iterator last) {}
    void unique() {}
    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred) {}
    ~list() = default;
    friend std::ostream& operator<<(std::ostream& os, list& l) {
        for (auto it = l.begin(); it != l.end(); ++it) {
            std::cout << *it << " ";
        }
        return os;
    }

private:
    static node_raw_ptr get_raw(const node_ptr& ptr) { return ptr.get(); }
    void init() {
        _head        = std::make_unique<list_node<T>>();
        _head->_next = std::make_unique<list_node<T>>(value_type(), get_raw(_head), nullptr);
        _tail        = get_raw(_head->_next);
        _size        = 0;
    }
    node_ptr _head;
    node_raw_ptr _tail;
    size_t _size;
};
} // namespace jz