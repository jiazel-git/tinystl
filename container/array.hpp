#pragma once
#include "iterator.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

namespace jz
{
template < class Ty, size_t Size >
class ArrayConstIterator {
public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = Ty;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const Ty*;
    using reference         = const Ty&;
    using const_reference   = const Ty&;

public:
    ArrayConstIterator() noexcept : ptr_( nullptr ) {}

    explicit ArrayConstIterator( pointer pointer, difference_type off = 0 ) noexcept : ptr_( pointer + off ) {}

    inline reference operator*() const noexcept {
        return *ptr_;
    }

    inline pointer operator->() const noexcept {
        return ptr_;
    }

    inline ArrayConstIterator& operator++() noexcept {
        ++ptr_;
        return *this;
    }

    inline ArrayConstIterator operator++( int ) noexcept {
        auto tmp = *this;
        ++ptr_;
        return tmp;
    }

    inline ArrayConstIterator& operator--() noexcept {
        --ptr_;
        return *this;
    }

    inline ArrayConstIterator operator--( int ) noexcept {
        auto tmp = *this;
        --ptr_;
        return tmp;
    }

    inline ArrayConstIterator& operator+=( const difference_type off ) noexcept {
        ptr_ += off;
        return *this;
    }

    inline ArrayConstIterator operator+( const difference_type off ) const noexcept {
        return ArrayConstIterator( ptr_ + off );
    }

    inline ArrayConstIterator& operator-=( const difference_type off ) noexcept {
        return operator+=( -off );
    }

    inline ArrayConstIterator operator-( const difference_type off ) const noexcept {
        return operator+( -off );
    }

    [[nodiscard]] inline difference_type operator-( const ArrayConstIterator& other ) const noexcept {
        return ptr_ - other.ptr_;
    }

    [[nodiscard]] inline bool operator==( const ArrayConstIterator& other ) const noexcept {
        return ptr_ == other.ptr_;
    }

    [[nodiscard]] inline bool operator!=( const ArrayConstIterator& other ) const noexcept {
        return !( *this == other );
    }

    [[nodiscard]] inline reference operator[]( const difference_type off ) const noexcept {
        return *( ptr_ + off );
    }

    [[nodiscard]] inline bool operator>( const ArrayConstIterator& other ) const noexcept {
        return ptr_ > other.ptr_;
    }

    [[nodiscard]] inline bool operator<( const ArrayConstIterator& other ) const noexcept {
        return ptr_ < other.ptr_;
    }

    [[nodiscard]] inline bool operator>=( const ArrayConstIterator& other ) const noexcept {
        return !( ptr_ < other.ptr_ );
    }

    [[nodiscard]] inline bool operator<=( const ArrayConstIterator& other ) const noexcept {
        return !( ptr_ > other.ptr_ );
    }

protected:
    pointer ptr_;
};

template < class Ty, size_t Size >
class ArrayIterator : public ArrayConstIterator< Ty, Size > {
public:
    using MyBase = ArrayConstIterator< Ty, Size >;
    using typename MyBase::iterator_category;

    using value_type      = Ty;
    using reference       = Ty&;
    using pointer         = Ty*;
    using difference_type = std::ptrdiff_t;

public:
    ArrayIterator() noexcept : MyBase() {}
    explicit ArrayIterator( pointer pointer, difference_type off = 0 ) noexcept : MyBase( pointer, off ) {}

    inline reference operator*() const noexcept {
        return const_cast< reference >( MyBase::operator*() );
    }

    inline pointer operator->() const noexcept {
        return const_cast< pointer >( MyBase::operator->() );
    }

    inline ArrayIterator& operator++() noexcept {
        MyBase::operator++();
        return *this;
    }

    inline ArrayIterator operator++( int ) noexcept {
        auto tmp = *this;
        MyBase::operator++();
        return tmp;
    }

    inline ArrayIterator operator+=( const difference_type off ) noexcept {
        MyBase::operator+=( off );
        return *this;
    }

    inline ArrayIterator operator+( const difference_type off ) const noexcept {
        return ArrayIterator( const_cast< pointer >( MyBase::ptr_ ) + off );
    }

    inline ArrayIterator operator-=( const difference_type off ) noexcept {
        MyBase::operator-=( off );
        return *this;
    }

    inline ArrayIterator operator-( const difference_type off ) const noexcept {
        return operator+( -off );
    }
    using MyBase::operator-;

    [[nodiscard]] inline reference operator[]( const difference_type off ) noexcept {
        return *( operator+( off ) );
    }
};

template < class Ty, size_t Size >
class array {
public:
    using value_type             = Ty;
    using size_type              = size_t;
    using reference              = Ty&;
    using const_reference        = const Ty&;
    using pointer                = Ty*;
    using const_pointer          = const Ty*;
    using difference_type        = std::ptrdiff_t;
    using iterator               = ArrayIterator< Ty, Size >;
    using const_iterator         = ArrayConstIterator< Ty, Size >;
    using reverse_iterator       = jz::reverse_iterator< iterator >;
    using const_reverse_iterator = jz::reverse_iterator< const_iterator >;

public:
    void fill( const value_type& val ) {
        std::fill_n( elems_, Size, val );
    }

    void swap( array& other ) noexcept {
        std::swap_ranges( elems_, elems_ + Size, other.elems_ );
    }

    [[nodiscard]] inline iterator begin() noexcept {
        return iterator( elems_, 0 );
    }

    [[nodiscard]] inline const_iterator begin() const noexcept {
        return const_iterator( elems_, 0 );
    }

    [[nodiscard]] inline iterator end() noexcept {
        return iterator( elems_, Size );
    }

    [[nodiscard]] inline const_iterator end() const noexcept {
        return const_iterator( elems_, Size );
    }

    [[nodiscard]] inline const_iterator cbegin() const noexcept {
        return const_iterator( elems_, 0 );
    }

    [[nodiscard]] inline const_iterator cend() const noexcept {
        return const_iterator( elems_, Size );
    }

    [[nodiscard]] inline reverse_iterator rbegin() noexcept {
        return reverse_iterator( end() );
    }

    [[nodiscard]] inline const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator( cend() );
    }

    [[nodiscard]] inline reverse_iterator rend() noexcept {
        return reverse_iterator( begin() );
    }

    [[nodiscard]] inline const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator( cbegin() );
    }

    [[nodiscard]] inline const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator( cend() );
    }

    [[nodiscard]] inline const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator( cbegin() );
    }

    [[nodiscard]] constexpr inline size_type size() const noexcept {
        return Size;
    }

    [[nodiscard]] constexpr inline size_type max_size() const noexcept {
        return size();
    }

    [[nodiscard]] constexpr inline bool empty() const noexcept {
        return Size == 0;
    }

    [[nodiscard]] inline reference at( const difference_type off ) {
        if ( off < 0 || static_cast< size_type >( off ) >= Size ) {
            throw std::out_of_range( "off is out of range" );
        }
        return elems_[ off ];
    }

    [[nodiscard]] inline const_reference at( const difference_type off ) const {
        if ( off < 0 || static_cast< size_type >( off ) >= Size ) {
            throw std::out_of_range( "off is out of range" );
        }
        return elems_[ off ];
    }

    [[nodiscard]] inline reference operator[]( const size_type off ) noexcept {
        return elems_[ off ];
    }

    [[nodiscard]] inline const_reference operator[]( const size_type off ) const noexcept {
        return elems_[ off ];
    }

    [[nodiscard]] inline reference front() noexcept {
        return elems_[ 0 ];
    }

    [[nodiscard]] inline const_reference front() const noexcept {
        return elems_[ 0 ];
    }

    [[nodiscard]] inline reference back() noexcept {
        return elems_[ Size - 1 ];
    }

    [[nodiscard]] inline const_reference back() const noexcept {
        return elems_[ Size - 1 ];
    }

    [[nodiscard]] inline pointer data() noexcept {
        return elems_;
    }

    [[nodiscard]] inline const_pointer data() const noexcept {
        return elems_;
    }

public:
    value_type elems_[ Size ];
};

template < class First, class... Rest >
struct ElemsAllSame
{
    static_assert( std::conjunction_v< std::is_same< First, Rest >... >, "all elements should be same type" );
    using type = First;
};

template < class First, class... Rest >
array( First, Rest... ) -> array< typename ElemsAllSame< First, Rest... >::type, sizeof...( Rest ) + 1 >;
}  // namespace jz
