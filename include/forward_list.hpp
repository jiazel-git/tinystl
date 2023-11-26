#ifndef __FORWARD_LIST__
#define __FORWARD_LIST__
#include "iterator.hpp"
#include <cstdlib>
#include <iostream>
namespace jz {
template < class T >
struct node_base {
    T          value_;
    node_base* next_;
    node_base() : value_(), next_( nullptr ) {}
    node_base( const T& value, node_base* node ) : value_( value ), next_( node ) {}
    node_base( const T& value ) : value_( value ), next_( nullptr ) {}
    node_base( const node_base& )            = default;
    node_base( node_base&& )                 = default;
    node_base& operator=( const node_base& ) = default;
    node_base& operator=( node_base&& )      = default;
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
    Flist_const_iterator( const Flist_const_iterator& other )      = default;
    Flist_const_iterator( Flist_const_iterator&& other )           = default;
    Flist_const_iterator& operator=( const Flist_const_iterator& ) = default;
    Flist_const_iterator& operator=( Flist_const_iterator&& )      = default;
    ~Flist_const_iterator()                                        = default;
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
    nodeptr_ data() {
        return ptr_;
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
    Flist_iterator( const Flist_iterator& other )      = default;
    Flist_iterator( Flist_iterator&& other )           = default;
    Flist_iterator& operator=( const Flist_iterator& ) = default;
    Flist_iterator& operator=( Flist_iterator&& )      = default;
    ~Flist_iterator()                                  = default;
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
    using node_           = node_base< T >;
    using nodeptr_        = node_*;
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;
    using pointer         = T*;
    using const_pointer   = const T*;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using iterator        = Flist_iterator< forward_list >;
    using const_iterator  = Flist_const_iterator< forward_list >;
    using allocate_       = Alloc;

public:
    forward_list() {
        init();
    }
    explicit forward_list( const Alloc& alloc ) {}
    forward_list( size_type count, const T& value, const Alloc& alloc = std::allocator< T >() ) {
        init();
        insert_after( before_begin(), count, value );
    }
    explicit forward_list( size_t count ) {
        init();
        insert_after( before_begin(), count, value_type() );
    }
    template < class InputIterator, class Require = RequireInputIterator< InputIterator > >
    forward_list( InputIterator first, InputIterator last ) {
        init();
        insert_after( before_begin(), first, last );
    }
    forward_list( const forward_list& other ) {
        init();
        insert_after( before_begin(), other.begin(), other.end() );
    }
    forward_list( forward_list&& other ) {
        init();
        insert_after( before_begin(), other.begin(), other.end() );
    }
    forward_list( std::initializer_list< T > il ) {
        init();
        insert_after( before_begin(), il.begin(), il.end() );
    }
    forward_list& operator=( const forward_list& other ) {
        auto copy = other;
        swap( copy );
        return *this;
    }
    forward_list& operator=( forward_list&& other ) noexcept {
        if ( head_ != other.head_ ) {
            swap( other );
        }
        return *this;
    }
    forward_list& operator=( std::initializer_list< T > il ) {
        clear();
        insert_after( before_begin(), il.begin(), il.end() );
        return *this;
    }
    iterator begin() {
        return head_->next_;
    }
    iterator end() {
        return iterator( 0 );
    }
    const_pointer cbegin() const {
        return begin();
    }
    const_iterator cend() const {
        return end();
    }
    iterator before_begin() {
        return head_;
    }
    const_iterator before_begin() const {
        return before_begin();
    }
    void push_front( const value_type& value ) {
        insert_after( head_, std::move( value ) );
    }
    void push_front( value_type&& value ) {
        insert_after( head_, std::move( value ) );
    }
    void pop_front() {
        nodeptr_ tmp_ = head_->next_;
        incr();
        delete tmp_;
    }
    void clear() noexcept {
        nodeptr_ tmp_ = head_->next_;
        nodeptr_ p_;
        while ( tmp_ ) {
            p_   = tmp_;
            tmp_ = tmp_->next_;
            delete p_;
            p_ = nullptr;
        }
    }
    bool empty() const noexcept {
        return head_->next_ == 0;
    }
    iterator insert_after( const_iterator pos, const value_type& value ) {
        nodeptr_ new_node_ = new node_( std::move( value ), pos.data()->next_ );
        pos.data()->next_  = new_node_;
        ++size_;
        return new_node_;
    }
    iterator insert_after( const_iterator pos, value_type&& value ) {
        nodeptr_ new_node_ = new node_( std::move( value ), pos.data()->next_ );
        pos.data()->next_  = new_node_;
        ++size_;
        return new_node_;
    }
    iterator insert_after( const_iterator pos, size_type count, const value_type& value ) {
        iterator p_ = pos.data();
        while ( count-- ) {
            pos = insert_after( pos, std::move( value ) );
        }
        return p_;
    }
    template < class InputIterator, class Require = RequireInputIterator< InputIterator > >
    iterator insert_after( const_iterator pos, InputIterator first, InputIterator last ) {
        auto p_ = pos;
        auto it = first;
        while ( it != last ) {
            insert_after( pos, *it );
            ++pos;
            ++it;
        }
        return p_.data();
    }
    iterator insert_after( const_iterator pos, std::initializer_list< T > il ) {
        return insert_after( pos, il.begin(), il.end() );
    }
    iterator erase_after( const_iterator pos ) {
        nodeptr_ tmp_     = pos.data()->next_;
        pos.data()->next_ = pos.data()->next_->next_;
        delete tmp_;
        tmp_ = nullptr;
        --size_;
        return pos.data()->next_;
    }
    iterator erase_after( const_iterator first, const_iterator last ) {
        auto it = first;
        while ( it != last ) {
            nodeptr_ p_ = it.data()->next_;
            ++it;
            delete p_;
            p_ = nullptr;
        }
        return ( ++first ).data();
    }
    template < class... Args >
    iterator emplace_after( const_iterator pos, Args&&... args ) {
        return insert_after( pos, value_type( std::forward< Args >( args )... ) );
    }
    void resize( size_type n ) {
        resize( n, value_type() );
    }
    void resize( size_type n, const value_type& value ) {
        size_t diff_ = std::abs( static_cast< int >( n - size_ ) );
        if ( n > size_ ) {
            auto it = end();
            while ( diff_-- ) {
                it = insert_after( it, std::move( value ) );
            }
            return;
        }
        if ( n < size_ ) {
            auto tmp_ = head_;
            while ( n-- ) {
                tmp_ = tmp_->next_;
            }
            auto p_ = tmp_;
            while ( tmp_ ) {
                p_   = tmp_;
                tmp_ = tmp_->next_;
                delete p_;
            }
            return;
        }
    }
    void remove( const value_type& value ) {
        remove_if( [ & ]( value_type val ) -> bool { return val == value; } );
    }
    template < class UnaryPredicate >
    void remove_if( UnaryPredicate pred_ ) {
        nodeptr_ tmp_ = head_;
        while ( tmp_->next_ != nullptr ) {
            if ( pred_( tmp_->next_->value_ ) ) {
                nodeptr_ p_ = tmp_->next_;
                tmp_->next_ = tmp_->next_->next_;
                delete p_;
                p_ = nullptr;
                continue;
            }
            tmp_ = tmp_->next_;
        }
    }
    void swap( forward_list& other ) {
        using std::swap;
        swap( head_->next_, other.head_->next_ );
        // swap( tail_, other.tail_ );
    }
    ~forward_list() {
        nodeptr_ tmp_ = head_;
        nodeptr_ p_;
        while ( tmp_ ) {
            p_   = tmp_;
            tmp_ = tmp_->next_;
            delete p_;
            p_ = nullptr;
        }
    }
    friend std::ostream& operator<<( std::ostream& os, const forward_list& fl ) {
        nodeptr_ tmp_ = fl.head_->next_;
        while ( tmp_ ) {
            os << tmp_->value_ << " ";
            tmp_ = tmp_->next_;
        }
        return os;
    }

private:
    void init() {
        head_ = new node_;
        // tail_ = head_;
        size_ = 0;
    }
    void incr() {
        head_->next_ = head_->next_->next_;
    }

private:
    nodeptr_ head_;
    // nodeptr_  tail_;
    size_type size_;
    // allocate_ alloc_;
};

}  // namespace jz
#endif