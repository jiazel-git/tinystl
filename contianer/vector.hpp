#pragma once
#include "iterator.hpp"
#include <initializer_list>
#include <iostream>
#include <memory>

using namespace std;
namespace jz
{
template < typename T, typename Alloc = std::allocator< T > >
class vector {
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
    using reverse_iterator = reverse_iterator< iterator >;

public:
    vector() : _start( nullptr ), _end( nullptr ), _capacity( nullptr ) {}
    explicit vector( size_type n ) {
        construct( n, value_type() );
    }
    vector( size_type n, const value_type& val ) {
        construct( n, val );
    }
    vector( const vector& other ) {
        size_t n = other._capacity - other._start;
        construct( n, other.cbegin(), other.cend() );
    }
    template < typename InputIterator, typename Require = RequireInputIterator< InputIterator > >
    vector( InputIterator first, InputIterator last ) {
        auto n = static_cast< difference_type >( last - first );
        construct( n, first, last );
    }
    vector( vector&& other ) {
        swap( other );
    }
    vector& operator=( const vector& rh ) {
        auto copy = rh;
        swap( copy );
        return *this;
    }
    vector& operator=( vector&& rh ) {
        if ( this != rh.data() ) {
            swap( rh );
        }
        return *this;
    }
    vector( initializer_list< value_type > il ) {
        auto n = il.size();
        construct( n, il.begin(), il.end() );
    }
    friend ostream& operator<<( ostream& os, vector& v ) {
        for ( auto& it : v ) {
            os << it << " ";
        }
        return os;
    }
    size_t capacity() const {
        return static_cast< size_t >( _capacity - _start );
    }
    size_t size() const {
        return static_cast< size_t >( _end - _start );
    }
    iterator begin() {
        return _start;
    }
    const_iterator cbegin() const {
        return _start;
    }
    reverse_iterator rbegin() const {
        return _end;
    }
    reverse_iterator rend() const {
        return _start;
    }
    iterator end() {
        return _end;
    }
    const_iterator cend() const {
        return _end;
    }
    const_pointer data() const {
        return data();
    }
    pointer data() {
        return const_cast< pointer >( _start );
    }
    reference operator[]( size_type index ) const {
        return *( _start + index );
    }
    reference at( size_type index ) const {
        if ( index < 0 || index > size() ) {
            throw out_of_range( "out of range\n" );
        }
        return operator[]( index );
    }
    reference back() {
        if ( empty() ) {
            throw length_error( "this vector is empty\n" );
        }
        return operator[]( size() - 1 );
    }
    const_reference back() const {
        return back();
    }
    void clear() noexcept {
        destruct( _start, _end );
        _end = _start;
    }
    reference front() {
        if ( empty() ) {
            throw length_error( "this vector is empty\n" );
        }
        return *begin();
    }
    const_reference front() const {
        return front();
    }
    void swap( vector& other ) noexcept {
        using std::swap;
        swap( _start, other._start );
        swap( _end, other._end );
        swap( _capacity, other._capacity );
    }
    void push_back( const value_type& val ) {
        push_back( std::move( val ) );
    }
    void push_back( value_type&& val ) {
        if ( check_capacity() ) {
            expand();
        }

        std::allocator_traits< allocator >::construct( _alloc, _end++, std::forward< value_type >( val ) );
    }
    void pop_back() {
        if ( empty() ) {
            throw length_error( "this vector is empty\n" );
        }
        std::allocator_traits< allocator >::destroy( _alloc, _end++ );
    }
    bool empty() const {
        return _start == _end;
    }
    // 改变容器大小，且创建对象
    void resize( size_type n ) {
        if ( n <= capacity() ) {
            if ( n < size() ) {
                difference_type distance = size() - n;
                _end                     = _start + n;
                destruct( _end, _end + n );
            } else {
                difference_type distance = n - size();
                while ( distance-- ) {

                    std::allocator_traits< allocator >::construct( _alloc, _end++, value_type() );
                }
            }
        } else {
            auto new_capacity = n;
            expand_enough( new_capacity );
            difference_type distance = n - size();
            while ( distance-- ) {

                std::allocator_traits< allocator >::construct( _alloc, _end++, value_type() );
            }
        }
    }
    void resize( size_type n, const value_type& val ) {
        if ( n <= capacity() ) {
            if ( n < size() ) {
                difference_type distance = size() - n;
                _end                     = _start + n;
                destruct( _end, _end + n );
            } else {
                difference_type distance = n - size();
                while ( distance-- ) {

                    std::allocator_traits< allocator >::construct( _alloc, _end++, std::move( val ) );
                }
            }
        } else {
            auto new_capacity = n;
            expand_enough( new_capacity );
            difference_type distance = n - size();
            while ( distance-- ) {

                std::allocator_traits< allocator >::construct( _alloc, _end++, std::move( val ) );
            }
        }
    }
    // 改变容器大小，不创建对象
    void reserve( size_type n ) {
        if ( n > capacity() ) {
            expand_enough( n );
        }
    }
    template < typename InputIterator, typename Require = RequireInputIterator< InputIterator > >
    void assign( InputIterator first, InputIterator last ) {
        auto n = last - first;
        if ( n <= capacity() ) {
            auto new_end = _start;
            while ( n-- ) {
                *( new_end++ ) = *( first++ );
            }
            _end = new_end;
        } else {
            if ( _start ) {
                destroy();
            }
            construct( size_type( n ), first, last );
        }
    }
    void assign( size_type n, const value_type& val ) {
        if ( n <= capacity() ) {
            auto new_end = _start;
            while ( n-- ) {
                *( new_end++ ) = val;
            }
            _end = new_end;
        } else {
            if ( _start ) {
                destroy();
            }
            construct( n, val );
        }
    }
    void assign( initializer_list< value_type > il ) {
        assign( il.begin(), il.end() );
    }
    iterator erase( const_iterator pos ) {
        if ( pos < cbegin() || pos >= cend() ) {
            throw out_of_range( "this posing is invalid\n" );
        }
        auto p = const_cast< iterator >( pos );
        std::move( pos + 1, cend(), p );
        std::allocator_traits< allocator >::destroy( _alloc, _end-- );
        return p;
    }
    iterator erase( const_iterator first, const_iterator last ) {
        if ( !( first >= cbegin() && last <= cend() ) ) {
            throw out_of_range( "this range is invaild\n" );
        }
        auto iter = std::move( const_cast< iterator >( last ), _end, const_cast< iterator >( first ) );
        destruct( first, last );
        _end = iter;
        return const_cast< iterator >( first );
    }
    iterator insert( const_iterator pos, value_type& val ) {
        auto offset = pos - _start;
        if ( check_capacity() ) {
            expand();
        }
        std::move( _start + offset, _end++, _start + offset + 1 );
        *( _start + offset ) = val;
        return _start + offset;
    }
    iterator insert( const_iterator pos, size_type n, const value_type& val ) {
        auto p        = const_cast< iterator >( pos );
        auto distance = _end - pos;
        if ( _end + n > _capacity ) {
            auto offset       = pos - const_cast< const_iterator >( _start );
            auto new_capacity = size() + n;
            expand_enough( new_capacity );
            p = _start + offset;
        }

        auto tmp = _end - 1;
        _end += n;
        while ( distance-- ) {
            *( tmp + n ) = std::move( *tmp );
            --tmp;
        }
        auto q = p;
        while ( n-- ) {
            *( q++ ) = val;
        }
        return p;
    }
    template < typename InputIterator, typename Require = RequireInputIterator< InputIterator > >
    iterator insert( const_iterator pos, InputIterator first, InputIterator last ) {
        auto   p        = const_cast< iterator >( pos );
        auto   distance = _end - pos;
        size_t n        = last - first;
        if ( _end + n > _capacity ) {
            auto offset       = pos - const_cast< const_iterator >( _start );
            auto new_capacity = size() + ( last - first );
            expand_enough( new_capacity );
            p = _start + offset;
        }
        auto tmp = _end - 1;
        _end += n;
        while ( distance-- ) {
            *( tmp + n ) = std::move( *tmp );
            --tmp;
        }
        auto q = p;
        for ( ; first != last; ++first ) {
            *( q++ ) = std::move( *first );
        }
        return p;
    }
    iterator insert( const_iterator pos, value_type&& val ) {
        auto offset = pos - _start;
        if ( check_capacity() ) {
            expand();
        }
        std::move( _start + offset, _end++, _start + offset + 1 );
        *( _start + offset ) = std::forward< value_type >( val );
        return _start + offset;
    }
    iterator insert( const_iterator pos, initializer_list< value_type > il ) {
        return insert( pos, il.begin(), il.end() );
    }
    template < typename... Args >
    void emplace_back( Args&&... args ) {
        if ( _end == _capacity ) {
            expand();
        }
        std::allocator_traits< allocator >::construct( _alloc, _end++, std::forward< Args >( args )... );
    }
    void shrink_to_fit() {
        if ( check_capacity() ) {
            // do nothing
        } else {
            reallocate_exaclty( static_cast< size_t >( _end - _start ) );
        }
    }
    ~vector() {
        destroy();
    }

private:
    void construct( size_t n, const value_type& val ) {
        _start    = _alloc.allocate( n );
        _end      = _start;
        _capacity = _start + n;
        while ( n-- ) {

            std::allocator_traits< allocator >::construct( _alloc, _end++, val );
        }
    }
    template < typename InputIterator >
    void construct( size_t n, InputIterator first, InputIterator last ) {
        _start    = _alloc.allocate( n );
        _end      = _start;
        _capacity = _start + n;
        while ( first != last ) {
            //*( _end++ ) = *( first++ ); //迭代器会失效

            std::allocator_traits< allocator >::construct( _alloc, _end++, *( first++ ) );
        }
    }
    void destroy() {
        size_t n = _capacity - _start;
        _alloc.deallocate( _start, n );
    }
    bool check_capacity() {
        return _end == _capacity;
    }
    void expand() {
        size_type new_capacity      = empty() ? 1 : size() * 2;
        auto      new_start         = _alloc.allocate( new_capacity );
        auto      new_end           = new_start;
        auto      new_end_of_stroge = new_start + new_capacity;
        auto      tmp               = _start;
        size_type n                 = size();
        while ( n-- ) {
            new ( new_end ) value_type( std::move( *tmp ) );
            ++new_end;
            ++tmp;
        }
        destroy();
        _start    = new_start;
        _end      = new_end;
        _capacity = new_end_of_stroge;
    }
    void destruct( const_iterator first, const_iterator end ) {
        for ( auto it = first; it != end; ++it ) {
            std::allocator_traits< allocator >::destroy( _alloc, it );
        }
    }
    void expand_enough( size_t n ) {
        auto old_copy = *this;
        _start        = _alloc.allocate( n );
        _end          = _start;
        _capacity     = _start + n;
        for ( auto it = old_copy.begin(); it != old_copy.end(); ++it ) {
            new ( _end ) value_type( std::move( *it ) );
            ++_end;
        }
    }
    void reallocate_exaclty( size_t _new_capacity ) {
        auto new_start = _alloc.allocate( _new_capacity );
        std::move( _start, _end, new_start );
        auto new_end      = new_start + _new_capacity;
        auto new_capacity = new_end;
        destroy();
        _start    = new_start;
        _end      = new_end;
        _capacity = new_capacity;
    }

private:
    iterator  _start;
    iterator  _end;
    iterator  _capacity;
    allocator _alloc;
};

template < class _Begin, class _End >
vector( _Begin, _End ) -> vector< typename iterator_traits< _Begin >::value_type >;

}  // namespace jz