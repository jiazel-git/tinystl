#pragma once
#include "iterator.hpp"
namespace jz
{
template < class _Ty, size_t _Bsize >
class _Deque_const_iterator {
public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = _Ty;
    using reference         = const _Ty&;
    using pointer           = const _Ty*;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;

public:
    _Deque_const_iterator() = default;
    _Deque_const_iterator( pointer _Ptr, size_t _Of ) noexcept : _Cont_Ptr( _Ptr ), _Off( _Of ) {}

public:
    [[nodiscard]] inline reference operator*() const noexcept {
        auto _Block    = _Cont_Ptr->_Get_block( _Off );
        auto _Off_elem = _Off % _Block_Size;
        return _Cont_Ptr[ _Block ][ _Off_elem ];
    }

    [[nodiscard]] inline pointer operator->() const noexcept {
        return this;
    }

    inline _Deque_const_iterator& operator++() noexcept {
        ++_Off;
        return *this;
    }

    inline _Deque_const_iterator operator++( int ) noexcept {
        auto _Copy = *this;
        ++_Off;
        return _Copy;
    }

    inline _Deque_const_iterator& operator--() noexcept {
        --_Off;
        return *this;
    }

    inline _Deque_const_iterator operator--( int ) noexcept {
        auto _Copy = *this;
        --_Off;
        return _Copy;
    }

    inline _Deque_const_iterator& operator+=( const difference_type _Of ) noexcept {
        _Off += _Of;
        return *this;
    }

    inline _Deque_const_iterator& operator-=( const difference_type _Of ) noexcept {
        _Off -= _Of;
        return *this;
    }

    [[nodiscard]] inline _Deque_const_iterator operator+( const difference_type _Of ) noexcept {
        _Off += _Of;
        return *this;
    }
    [[nodiscard]] inline _Deque_const_iterator operator-( const difference_type _Of ) noexcept {
        _Off -= _Of;
        return *this;
    }

    [[nodiscard]] inline difference_type operator-( const _Deque_const_iterator& _Other ) const noexcept {
        return _Off - _Other._Off;
    }

    [[nodiscard]] reference inline operator[]( const difference_type _Of ) noexcept {
        return *( *this + _Of );
    }

    [[nodiscard]] inline bool operator==( const _Deque_const_iterator& _Other ) const noexcept {
        return ( _Off == _Other._Off );
    }

    [[nodiscard]] inline bool operator!=( const _Deque_const_iterator& _Other ) const noexcept {
        return !( _Off == _Other._Off );
    }

    [[nodiscard]] inline bool operator>( const _Deque_const_iterator& _Other ) const noexcept {
        return ( _Off > _Other._Off );
    }

    [[nodiscard]] inline bool operator<( const _Deque_const_iterator& _Other ) const noexcept {
        return ( _Off < _Other._Off );
    }

    [[nodiscard]] inline bool operator>=( const _Deque_const_iterator& _Other ) const noexcept {
        return !( *this < _Other );
    }

    [[nodiscard]] inline bool operator<=( const _Deque_const_iterator& _Other ) const noexcept {
        return !( *this > _Other );
    }

protected:
    static constexpr size_type _Block_Size = _Bsize;

    pointer _Cont_Ptr{ nullptr };
    size_t  _Off{ 0 };
};

template < class _Ty, size_t _Bsize >
class _Deque_iterator : public _Deque_const_iterator< _Ty, _Bsize > {
public:
    using _Mybase = _Deque_const_iterator< _Ty, _Bsize >;

public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = _Ty;
    using reference         = _Ty&;
    using pointer           = _Ty*;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;

public:
    _Deque_iterator() = default;
    _Deque_iterator( pointer _Ptr, size_type _Of ) noexcept : _Mybase( _Ptr, _Of ) {}

public:
    [[nodiscard]] inline reference operator*() const noexcept {
        return const_cast< reference >( _Mybase::operator*() );
    }

    [[nodiscard]] inline pointer operator->() const noexcept {
        return const_cast< pointer >( _Mybase::operator->() );
    }

    [[nodiscard]] inline _Deque_iterator& operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    [[nodiscard]] inline _Deque_iterator operator++( int ) noexcept {
        auto _Copy = *this;
        _Mybase::operator++();
        return _Copy;
    }

    [[nodiscard]] inline _Deque_iterator& operator+=( const difference_type _Of ) noexcept {
        _Mybase::operator+=( _Of );
        return *this;
    }

    [[nodiscard]] inline _Deque_iterator& operator-=( const difference_type _Of ) noexcept {
        _Mybase::operator-=( _Of );
        return *this;
    }

    [[nodiscard]] inline _Deque_iterator operator+( const difference_type _Of ) noexcept {
        _Mybase::operator+( _Of );
        return *this;
    }

    using _Mybase::operator-;

    [[nodiscard]] inline _Deque_iterator operator-( const difference_type _Of ) noexcept {
        _Mybase::operator-( _Of );
        return *this;
    }

    [[nodiscard]] inline reference operator[]( const difference_type _Of ) const noexcept {
        return const_cast< reference >( _Mybase::operator[]( _Of ) );
    }
};
template < class _Ty, class _Alloc = std::allocator< _Ty > >
class deque {
private:
    static constexpr size_t _Min_Blocks = 8;
    static constexpr size_t _Bytes      = sizeof( _Ty );
    static constexpr size_t _Bsize      = _Bytes <= 1 ? 16 : _Bytes <= 2 ? 8 : _Bytes <= 4 ? 4 : _Bytes <= 8 ? 2 : 1;

public:
    using value_type      = _Ty;
    using reference       = _Ty&;
    using const_reference = const _Ty&;
    using pointer         = _Ty*;
    using const_pointer   = const _Ty*;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using iterator        = _Deque_iterator< _Ty, _Bsize >;
    using allocate_type   = _Alloc;

public:
    [[nodiscard]] inline size_type _Get_block( const size_type _Of ) const noexcept {
        auto _Block = ( _Of / _Bsize ) & ( _Blocks - 1 );
        return _Block;
    }

public:
    deque() = default;

private:
    void _Growblock() {}

private:
    pointer       _Cont{ nullptr };  // thep pointer to blocks
    size_type     _Blocks{ 0 };      // the number of the bkicks
    size_type     _Off{ 0 };         // the offset of the elemt
    size_type     _Size{ 0 };        // the number of the elems
    allocate_type _Aloc{};
};
}  // namespace jz