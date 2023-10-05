#pragma once
#include <iostream>
namespace jz {
// 定义五种迭代器类型
struct output_iterator_tag {};
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template < class Iter >
struct iterator_traits {
    using iterator_category = typename Iter::iterator_category;
    using value_type        = typename Iter::value_type;
    using reference         = typename Iter::reference;
    using pointer           = typename Iter::pointer;
    using difference_type   = typename Iter::difference_type;
};
// 偏特化版本
template < typename T >
struct iterator_traits< T* > {
    using iterator_category = random_access_iterator_tag;
    using value_type        = T;
    using reference         = T&;
    using pointer           = T*;
    using difference_type   = std::ptrdiff_t;
};
template < typename T >
struct iterator_traits< const T* > {
    using iterator_category = random_access_iterator_tag;
    using value_type        = T;
    using reference         = const T&;
    using pointer           = const T*;
    using difference_type   = std::ptrdiff_t;
};
template < class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T& >
struct iterator {
    using iterator_category = Category;
    using value_type        = typename std::remove_const< T >::type;
    using difference_type   = Distance;
    using pointer           = Pointer;
    using reference         = Reference;
};
template < class Iter >
class reverse_iterator : public iterator< typename iterator_traits< Iter >::iterator_category,
                                          typename iterator_traits< Iter >::value_type,
                                          typename iterator_traits< Iter >::difference_type,
                                          typename iterator_traits< Iter >::pointer,
                                          typename iterator_traits< Iter >::reference > {

public:
    using iterator_type   = Iter;
    using difference_type = typename iterator_traits< Iter >::difference_type;
    using reference       = typename iterator_traits< Iter >::reference;
    using pointer         = typename iterator_traits< Iter >::pointer;

public:
    reverse_iterator() = default;
    explicit reverse_iterator( iterator_type iter ) : _cur( iter ) {}
    reverse_iterator( const reverse_iterator& other ) : _cur( other._cur ) {}
    template < typename T >
    reverse_iterator( const reverse_iterator< T >& other ) : _cur( other.base() ) {}
    iterator_type base() const {
        return _cur;
    }
    reverse_iterator operator-( difference_type n ) {
        _cur += n;
        return reverse_iterator( _cur );
    }
    reverse_iterator& operator--() {
        _cur++;
        return *this;
    }
    reverse_iterator operator--( int ) {
        auto tmp = *this;
        _cur++;
        return tmp;
    }
    reverse_iterator& operator-=( difference_type n ) {
        _cur += n;
        return *this;
    }
    pointer operator->() const {
        return &( operator*() );
    }
    reference operator[]( difference_type n ) const {
        return *( *this + n );
    }
    reference operator*() const {
        auto tmp = _cur;
        return *( --tmp );
    }
    reverse_iterator operator+( difference_type n ) {
        return reverse_iterator( _cur - n );
    }
    reverse_iterator& operator++() {
        _cur--;
        return *this;
    }
    reverse_iterator operator++( int ) {
        auto tmp = *this;
        _cur--;
        return tmp;
    }
    reverse_iterator& operator+=( difference_type n ) {
        _cur += n;
        return *this;
    }
    reverse_iterator& operator=( const reverse_iterator& other ) {
        _cur = other._cur;
        return *this;
    }

private:
    iterator_type _cur;
};
template < class Iter >
auto operator-( const reverse_iterator< Iter >& lhs, const reverse_iterator< Iter >& rhs ) -> decltype( rhs.base() - lhs.base() ) {
    return rhs.base() - lhs.base();
}
template < class Iter >
reverse_iterator< Iter > operator+( const reverse_iterator< Iter >& lhs, typename iterator_traits< Iter >::difference_type n ) {
    return reverse_iterator< Iter >( lhs.base() - n );
}
template < class Iter1, class Iter2 >
bool operator==( const reverse_iterator< Iter1 >& lhs, const reverse_iterator< Iter2 >& rhs ) {
    return lhs.base() == rhs.base();
}
template < class Iter1, class Iter2 >
bool operator!=( const reverse_iterator< Iter1 >& lhs, const reverse_iterator< Iter2 >& rhs ) {
    return !( lhs == rhs );
}
template < class Iter1, class Iter2 >
bool operator<( const reverse_iterator< Iter1 >& lhs, const reverse_iterator< Iter2 >& rhs ) {
    return lhs.base() < rhs.base();
}
template < class Iter1, class Iter2 >
bool operator>( const reverse_iterator< Iter1 >& lhs, const reverse_iterator< Iter2 >& rhs ) {
    return lhs.base() > rhs.base();
}
template < class Iter1, class Iter2 >
bool operator<=( const reverse_iterator< Iter1 >& lhs, const reverse_iterator< Iter2 >& rhs ) {
    return !( lhs > rhs );
}
template < class Iter1, class Iter2 >
bool operator>=( const reverse_iterator< Iter1 >& lhs, const reverse_iterator< Iter2 >& rhs ) {
    return !( lhs < rhs );
}

template < class Iter >
reverse_iterator< Iter > make_reverse_iterator( Iter iter ) {
    return reverse_iterator< Iter >( iter );
}
template < typename OutputIterator >
using RequireOutputIterator = typename std::enable_if< std::is_convertible< typename std::iterator_traits< OutputIterator >::iterator_category, std::output_iterator_tag >::value >::type;

template < typename InputIterator >
using RequireInputIterator = typename std::enable_if< std::is_convertible< typename std::iterator_traits< InputIterator >::iterator_category, std::input_iterator_tag >::value >::type;

template < typename ForwardIterator >
using RequireForwardIterator = typename std::enable_if< std::is_convertible< typename std::iterator_traits< ForwardIterator >::iterator_category, std::forward_iterator_tag >::value >::type;

template < typename BidirectionalIterator >
using RequireBidirectionalIterator =
    typename std::enable_if< std::is_convertible< typename std::iterator_traits< BidirectionalIterator >::iterator_category, std::bidirectional_iterator_tag >::value >::type;

template < typename RandomIterator >
using RequireRandomIterator = typename std::enable_if< std::is_convertible< typename std::iterator_traits< RandomIterator >::iterator_category, std::random_access_iterator_tag >::value >::type;
}  // namespace jz