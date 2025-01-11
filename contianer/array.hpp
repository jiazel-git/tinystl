#pragma once
#include "iterator.hpp"
#include <algorithm>
#include <memory>

namespace jz
{
template < class _Ty, size_t _Size >
class _Array_const_iterator {
public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = _Ty;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const _Ty*;
    using reference         = const _Ty&;

public:
    _Array_const_iterator() noexcept : _Ptr( nullptr ) {}

    explicit _Array_const_iterator( pointer _Pointer, int _Off = 0 ) noexcept : _Ptr( _Pointer + _Off ) {}

    inline reference operator*() const noexcept {
        return *_Ptr;
    }

    inline pointer operator->() const noexcept {
        return _Ptr;
    }

    inline _Array_const_iterator& operator++() noexcept {
        ++_Ptr;
        return *this;
    }

    inline _Array_const_iterator operator++( int ) noexcept {
        auto _Tmp = *this;
        ++_Ptr;
        return _Tmp;
    }

    inline _Array_const_iterator& operator--() noexcept {
        --_Ptr;
        return *this;
    }

    inline _Array_const_iterator& operator--( int ) noexcept {
        auto _Tmp = *this;
        --_Ptr;
        return _Tmp;
    }

    inline _Array_const_iterator& operator+=( const difference_type _Off ) noexcept {
        _Ptr += _Off;
        return *this;
    }

    inline _Array_const_iterator operator+( const difference_type _Off ) noexcept {
        return operator+=( _Off );
    }

    inline _Array_const_iterator& operator-=( const difference_type _Off ) noexcept {
        return operator+=( -_Off );
    }

    inline _Array_const_iterator operator-( const difference_type _Off ) noexcept {
        return operator-=( _Off );
    }

    [[nodiscard]] inline difference_type operator-( const _Array_const_iterator& _Other ) noexcept {
        return _Ptr - _Other._Ptr;
    }

    [[nodiscard]] inline bool operator==( const _Array_const_iterator& _Other ) const noexcept {
        return _Ptr == _Other._Ptr;
    }

    [[nodiscard]] inline bool operator!=( const _Array_const_iterator& _Other ) const noexcept {
        return !( *this == _Other );
    }

    [[nodiscard]] inline reference operator[]( const difference_type _Off ) const noexcept {
        return *( _Ptr + _Off );
    }

    [[nodiscard]] inline bool operator>( const _Array_const_iterator& _Other ) const noexcept {
        return _Ptr > _Other._Ptr;
    }

    [[nodiscard]] inline bool operator<( const _Array_const_iterator& _Other ) const noexcept {
        return _Ptr < _Other._Ptr;
    }

    [[nodiscard]] inline bool operator>=( const _Array_const_iterator& _Other ) const noexcept {
        return !( _Ptr < _Other._Ptr );
    }

    [[nodiscard]] inline bool operator<=( const _Array_const_iterator& _Other ) const noexcept {
        return !( _Ptr > _Other._Ptr );
    }

private:
    pointer _Ptr;
};

template < class _Ty, size_t _Size >
class _Array_iterator : public _Array_const_iterator< _Ty, _Size > {
public:
    using _Mybase = _Array_const_iterator< _Ty, _Size >;

public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = _Ty;
    using difference_type   = ptrdiff_t;
    using reference         = _Ty&;
    using pointer           = _Ty*;

public:
    _Array_iterator() noexcept : _Mybase() {}
    explicit _Array_iterator( pointer _Pointer, int _Off = 0 ) noexcept : _Mybase( _Pointer, _Off ) {}

    inline reference operator*() const noexcept {
        return const_cast< reference >( _Mybase::operator*() );
    }

    inline pointer operator->() const noexcept {
        return const_cast< pointer >( _Mybase::operator->() );
    }

    inline _Array_iterator& operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    inline _Array_iterator operator++( int ) noexcept {
        auto _Tmp = *this;
        _Mybase::operator++();
        return _Tmp;
    }

    inline _Array_iterator operator+=( const difference_type _Off ) noexcept {
        _Mybase::operator+=( _Off );
        return *this;
    }

    inline _Array_iterator operator+( const difference_type _Off ) noexcept {
        _Mybase::operator+( _Off );
        return *this;
    }

    inline _Array_iterator operator-=( const difference_type _Off ) noexcept {
        _Mybase::operator-=( _Off );
        return *this;
    }

    inline _Array_iterator operator-( const difference_type _Off ) noexcept {
        _Mybase::operator-( _Off );
        return *this;
    }
    using _Mybase::operator-;

    [[nodiscard]] inline reference operator[]( const difference_type _Off ) noexcept {
        return *( operator+( _Off ) );
    }
};

template < class _Ty, size_t _Size >
class array {
public:
    using value_type             = _Ty;
    using size_type              = size_t;
    using reference              = _Ty&;
    using const_reference        = const _Ty&;
    using pointer                = _Ty*;
    using const_pointer          = const _Ty*;
    using difference_type        = ptrdiff_t;
    using iterator               = _Array_iterator< _Ty, _Size >;
    using const_iterator         = _Array_const_iterator< _Ty, _Size >;
    using reverse_iterator       = jz::reverse_iterator< iterator >;
    using const_reverse_iterator = jz::reverse_iterator< const_iterator >;

public:
    void fill( const value_type& _Val ) {
        std::fill_n( _Elems, _Size, _Val );
    }

    void swap( array& _Other ) noexcept {
        std::swap_ranges( _Elems, _Elems + _Size, _Other._Elems );
    }

    [[nodiscard]] inline iterator begin() noexcept {
        return iterator( _Elems, 0 );
    }

    [[nodiscard]] inline const_iterator begin() const noexcept {
        return const_iterator( _Elems, 0 );
    }

    [[nodiscard]] inline iterator end() noexcept {
        return iterator( _Elems, _Size );
    }

    [[nodiscard]] inline const_iterator end() const noexcept {
        return const_iterator( _Elems, _Size );
    }

    [[nodiscard]] inline const_iterator cbegin() const noexcept {
        return const_iterator( _Elems, 0 );
    }

    [[nodiscard]] inline const_iterator cend() const noexcept {
        return const_iterator( _Elems, _Size );
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

    [[nodiscard]] inline reverse_iterator rend() const noexcept {
        return const_reverse_iterator( cbegin() );
    }

    [[nodiscard]] inline const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator( cend() );
    }

    [[nodiscard]] inline const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator( cbegin() );
    }

    [[nodiscard]] constexpr inline size_type size() const noexcept {
        return _Size;
    }

    [[nodiscard]] constexpr inline size_type max_size() const noexcept {
        return size();
    }

    [[nodiscard]] constexpr inline bool empty() const noexcept {
        return false;
    }

    [[nodiscard]] inline reference at( const difference_type _Off ) {
        if ( _Off >= _Size ) {
            throw std::out_of_range( "_Off is bigger than array's size" );
        }
        return _Elems[ _Off ];
    }

    [[nodiscard]] inline const_reference at( const difference_type _Off ) const {
        if ( _Off >= _Size ) {
            throw std::out_of_range( "_Off is bigger than array's size" );
        }
        return _Elems[ _Off ];
    }

    [[nodiscard]] inline reference operator[]( const difference_type _Off ) noexcept {
        return _Elems[ _Off ];
    }

    [[nodiscard]] inline const_reference operator[]( const difference_type _Off ) const noexcept {
        return _Elems[ _Off ];
    }

    [[nodiscard]] inline reference front() noexcept {
        return _Elems[ 0 ];
    }

    [[nodiscard]] inline const_reference front() const noexcept {
        return _Elems[ 0 ];
    }

    [[nodiscard]] inline reference back() noexcept {
        return _Elems[ _Size - 1 ];
    }

    [[nodiscard]] inline const_reference back() const noexcept {
        return _Elems[ _Size - 1 ];
    }

    [[nodiscard]] inline pointer data() noexcept {
        return _Elems;
    }

    [[nodiscard]] inline const_pointer data() const noexcept {
        return _Elems;
    }

public:
    value_type _Elems[ _Size ];
};
}  // namespace jz