#ifndef __STACK
#define __STACK
#include "vector.hpp"
#include <vector>
namespace jz {
template < class T, class Container = jz::vector< T > >
class stack
{
public:
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;
    using container_type  = Container;
    using size_type       = size_t;

public:
    stack() : _container() {}
    explicit stack( const Container& container ) : _container( container ) {}
    explicit stack( Container&& container ) : _container( std::move( container ) ) {}
    stack( const stack& other ) {
        stack( std::move( other ) );
    }
    stack( stack&& other ) {
        swap( other );
    }
    template < class InputIterator, class Require = RequireInputIterator< InputIterator > >
    stack( InputIterator first, InputIterator last ) {
        _container( first, last );
    }
    template < class Alloc >
    explicit stack( const Alloc& alloc ) {}
    template < class Alloc >
    stack( const Container& container, const Alloc&& alloc ) {}
    template < class Alloc >
    stack( Container&& container, const Alloc&& alloc ) {}
    template < class Alloc >
    stack( const stack& other, const Alloc& alloc ) {}
    template < class Alloc >
    stack( stack&& other, const Alloc& alloc ) {}
    template < class InputIt, class Alloc >
    stack( InputIt first, InputIt last, const Alloc& alloc ) {}
    stack& operator=( const stack& other ) {
        _container = other._container;
        return *this;
    }
    stack& operator=( stack&& other ) {
        _container = std::move( other._container );
        return *this;
    }
    ~stack() = default;
    void swap( stack& other ) {
        _container.swap( other._container );
    }
    reference top() {
        return _container.back();
    }
    const_reference top() const {
        return _container.back();
    }
    bool empty() {
        return _container.empty();
    }
    size_t size() {
        return _container.size();
    }
    void push( const value_type& value ) {
        _container.push_back( value );
    }
    void push( value_type&& value ) {
        _container.push_back( std::move( value ) );
    }
    template < class... Args >
    void emplace( Args&&... args ) {
        _container.emplacebac( std::forward< Args >( args )... );
    }
    void pop() {
        _container.pop_back();
    }
    T* data() {
        return _container.data();
    }

private:
    container_type _container;
};
}  // namespace jz
#endif