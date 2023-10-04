#ifndef __FORWARD_LIST__
#define __FORWARD_LIST__
#include "iterator.hpp"
namespace jz {
template < class T >
struct node_base {
    T          value_;
    node_base* next_;
    node_base() : value_(), next_( nullptr ) {}
    node_base( const T& value ) : value_( value ), next_( nullptr ) {}
    node_base( const node_base& )            = delete;
    node_base( node_base&& )                 = delete;
    node_base& operator=( const node_base& ) = delete;
    node_base& operator=( node_base&& )      = delete;
};
template < class Flist >
class Flist_const_iterator {
public:
    using iterator_category = forward_iterator_tag;
    using value_type        = typename Flist::value_type;
    using reference         = typename Flist::const_reference;
    using pointer           = typename Flist::const_pointer;
    using difference_type   = typename Flist::difference_type;
    using nodeptr_          = typename Flist::nodeptr_;

public:
    Flist_const_iterator() noexcept : ptr_() {}
    Flist_const_iterator( nodeptr_ node ) : ptr_( node ) {}
    Flist_const_iterator( const Flist_const_iterator& other ) : ptr_( other.ptr_ ) {}
    Flist_const_iterator( Flist_const_iterator&& other ) : ptr_( std::move( other.ptr_ ) ) {}
    reference operator*() {
        return ptr_->value_;
    }
    pointer operator->() {
        return ptr_;
    }
    Flist_const_iterator& operator++() {
        ptr_ = ptr_->next_;
        return *this;
    }
    Flist_const_iterator operator++( int ) {
        auto tmp = *this;
        ptr_     = ptr_->next_;
        return tmp;
    }

public:
    bool operator==( const Flist_const_iterator& other ) {
        return ptr_ == other.ptr_;
    }
    bool operator!=( const Flist_const_iterator& other ) {
        return !( operator==( other ) );
    }
    bool operator>( const Flist_const_iterator& other ) {
        return ptr_ > other.ptr_;
    }
    bool operator<( const Flist_const_iterator& other ) {
        return ptr_ < other.ptr_;
    }
    bool operator>=( const Flist_const_iterator& other ) {
        return !( operator<( other ) );
    }
    bool operator<=( const Flist_const_iterator& other ) {
        return !( operator>( other ) );
    }

protected:
    nodeptr_ ptr_;
};
template < class Flist >
class Flist_iterator : public Flist_const_iterator< Flist > {
public:
    using iterator_category = forward_iterator_tag;
    using value_type        = typename Flist::value_type;
    using reference         = typename Flist::reference;
    using pointer           = typename Flist::pointer;
    using difference_type   = typename Flist::difference_type;
    using nodeptr_          = typename Flist::nodeptr_;

public:
    Flist_iterator() : Flist_const_iterator< Flist >() {}
    Flist_iterator( nodeptr_ node ) : Flist_const_iterator< Flist >( node ) {}
    Flist_iterator( const Flist_iterator& other ) : Flist_const_iterator< Flist >( std::move( other ) ) {}
    Flist_iterator( Flist_iterator&& other ) : Flist_const_iterator< Flist >( other ) {}
    reference operator*() {
        return this->ptr_->value_;
    }
    pointer operator->() {
        return this->ptr_;
    }
    Flist_iterator& operator++() {
        this->ptr_ = this->ptr_->next_;
        return *this;
    }
    Flist_iterator operator++( int ) {
        auto tmp   = *this;
        this->ptr_ = this->ptr_->next_;
        return tmp;
    }
};

template < class T, class Alloc = std::allocator< T > >
class forward_list {
public:
    using nodeptr_        = node_base< T >*;
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using iterator        = Flist_iterator< nodeptr_ >;
    using const_iterator  = Flist_const_iterator< nodeptr_ >;
    using allocate_       = Alloc;

public:
    forward_list() {}
    explicit forward_list( const Alloc& alloc ) {}
    forward_list( size_type count, const T& value, const Alloc& alloc = std::allocator< T >() ) {}
    explicit forward_list( size_t count ) {}
    template < class InputIterator, class Require = RequireInputIterator< InputIterator > >
    forward_list( InputIterator first, InputIterator last ) {}
    forward_list( const forward_list& other ) {}
    forward_list( forward_list&& other ) {}
    forward_list( std::initializer_list< T > il ) {}
    forward_list& operator=( const forward_list& other ) {}
    forward_list& operator=( forward_list&& other ) noexcept {}
    forward_list& operator=( std::initializer_list< T > il ) {}
    ~forward_list() {}

private:
private:
    nodeptr_  head_;
    nodeptr_  tail_;
    allocate_ alloc_;
};

}  // namespace jz
#endif