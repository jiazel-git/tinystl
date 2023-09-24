#ifndef __QUEUE
#define __QUEUE
#include "list.hpp"
namespace jz {
template < class T, class Container = jz::list< T > >
class queue
{
public:
    using value_type      = T;
    using size_type       = size_t;
    using reference       = T&;
    using const_reference = const T&;

public:
    queue() : _container() {}
    explicit queue( const Container& container ) : _container( container ) {}
    explicit queue( Container&& container ) : _container( std::move( container ) ) {}
    queue( const queue& other ) {
        _container( other._container );
    }
    queue( queue&& other ) {
        _container( std::move( other._container ) );
    }
    template < class InputIterator, class Require = RequireInputIterator< InputIterator > >
    queue( InputIterator first, InputIterator last ) {
        _container( first, last );
    }
    template < class Alloc >
    explicit queue( const Alloc& alloc );
    template < class Alloc >
    queue( const Container& cont, const Alloc& alloc );
    template < class Alloc >
    queue( Container&& cont, const Alloc& alloc );
    template < class Alloc >
    queue( const queue& other, const Alloc& alloc );
    template < class Alloc >
    queue( queue&& other, const Alloc& alloc );
    template < class InputIt, class Alloc >
    queue( InputIt first, InputIt last, const Alloc& alloc );
    queue& operator=( const queue& other ) {
        _container = other._container;
    }
    queue& operator=( queue&& other ) {
        _container = std::move( other._container );
    }
    reference front() {
        return _container.front();
    }
    const_reference front() const {
        return _container.front();
    }
    reference back() {
        return _container.back();
    }
    const_reference back() const {
        return _container.back;
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
        _container.emplace_back( std::forward< Args >( args )... );
    }
    void pop() {
        _container.pop_back();
    }
    void swap( queue& other ) {
        _container.swap( other._container );
    }
    ~queue() = default;

private:
    Container _container;
};
}  // namespace jz
#endif