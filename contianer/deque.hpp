#pragma once
#include "iterator.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>

namespace jz
{
template < class _Mydeque >
class _Deque_const_iterator {
public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = _Mydeque::value_type;
    using reference         = _Mydeque::const_reference;
    using pointer           = _Mydeque::const_pointer;
    using size_type         = _Mydeque::size_type;
    using difference_type   = _Mydeque::difference_type;

public:
    _Deque_const_iterator() = default;
    _Deque_const_iterator( _Mydeque* _Ptr, size_t _Of ) noexcept : _Cont_Ptr( _Ptr ), _Off( _Of ) {}

public:
    [[nodiscard]] inline reference operator*() const noexcept {
        auto _Block    = _Cont_Ptr->_Get_block( _Off );
        auto _Off_elem = _Off % _Cont_Ptr->bsize();
        return _Cont_Ptr->data()[ _Block ][ _Off_elem ];
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
    _Mydeque* _Cont_Ptr{ nullptr };
    size_t    _Off{ 0 };
};

template < class _Mydeque >
class _Deque_iterator : public _Deque_const_iterator< _Mydeque > {
public:
    using _Mybase = _Deque_const_iterator< _Mydeque >;

public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = _Mydeque::value_type;
    using reference         = _Mydeque::reference;
    using pointer           = _Mydeque::pointer;
    using size_type         = _Mydeque::size_type;
    using difference_type   = _Mydeque::difference_type;

public:
    _Deque_iterator() = default;
    _Deque_iterator( _Mydeque* _Ptr, size_type _Of ) noexcept : _Mybase( _Ptr, _Of ) {}

public:
    [[nodiscard]] inline reference operator*() const noexcept {
        return const_cast< reference >( _Mybase::operator*() );
    }

    [[nodiscard]] inline pointer operator->() const noexcept {
        return const_cast< pointer >( _Mybase::operator->() );
    }

    inline _Deque_iterator& operator++() noexcept {
        _Mybase::operator++();
        return *this;
    }

    inline _Deque_iterator operator++( int ) noexcept {
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
    using value_type         = _Ty;
    using reference          = _Ty&;
    using const_reference    = const _Ty&;
    using pointer            = _Ty*;
    using const_pointer      = const _Ty*;
    using size_type          = size_t;
    using difference_type    = ptrdiff_t;
    using iterator           = _Deque_iterator< deque >;
    using const_iterator     = _Deque_const_iterator< deque >;
    using allocate_type      = _Alloc;
    using cont_allocate_type = std::allocator_traits< _Alloc >::template rebind_alloc< typename std::allocator_traits< _Alloc >::pointer >;

private:
    using _Contptr = pointer*;

public:
    [[nodiscard]] inline size_type _Get_block( const size_type _Of ) noexcept {
        auto _Block = ( _Of / _Bsize ) & ( _Blocks - 1 );
        return _Block;
    }

public:
    deque() = default;
    explicit deque( const _Alloc& _Al ) : _Aloc( _Al ) {}
    explicit deque( const size_type _Count, const _Alloc& _Al = _Alloc() ) : _Aloc( _Al ) {
        resize( _Count );
    }
    deque( const size_type _Count, const _Ty& _Val ) {
        _Construct_n( _Count, _Val );
    }
    deque( const size_type _Count, const _Ty& _Val, const _Alloc& _Al ) : _Aloc( _Al ) {
        _Construct_n( _Count, _Val );
    }
    deque( const deque& _Right ) {
        _Construct( _Right.begin(), _Right.end() );
    }

public:
    deque( const deque& _Right, const _Alloc& _Al ) : _Aloc( _Al ) {
        _Construct( _Right.begin(), _Right.end() );
    }
    template < class _Iter, std::enable_if_t< std::is_convertible_v< typename std::iterator_traits< _Iter >::iterator_category, std::input_iterator_tag >, int > = 0 >
    deque( _Iter _First, _Iter _Last ) {
        _Construct( _First, _Last );
    }
    template < class _Iter, std::enable_if_t< std::is_convertible_v< typename std::iterator_traits< _Iter >::iterator_category, std::input_iterator_tag >, int > = 0 >
    deque( _Iter _First, _Iter _Last, const _Alloc& _Al ) : _Aloc( _Al ) {
        _Construct( _First, _Last );
    }

public:
    deque& operator=( const deque& _Right ) {
        auto _Copy = _Right;
        std::swap( *this, _Copy );
        return *this;
    }

private:
    void _Construct_n( size_type _Count, const _Ty& _Val ) {
        for ( ; _Count > 0; --_Count ) {
            _Emplace_back_internal( _Val );
        }
    }
    void _Construct( iterator _First, iterator _Last ) {
        for ( ; _First != _Last; ++_First ) {
            _Emplace_back_internal( *_First );
        }
    }
    void _Construct( const_iterator _First, const_iterator _Last ) {
        for ( ; _First != _Last; ++_First ) {
            _Emplace_back_internal( *_First );
        }
    }

public:
    deque( deque&& _Right ) : _Aloc( std::move( _Right._Aloc ) ) {
        _Take_contents( _Right );
    }
    deque( deque&& _Right, const _Alloc& _Al ) : _Aloc( _Al ) {
        if ( !std::is_same_v< decltype( _Right._Aloc ), _Alloc > ) {
            _Construct( std::make_move_iterator( _Right.begin() ), std::make_move_iterator( _Right.end() ) );
            return;
        }
        _Take_contents( _Right );
    }

public:
    deque& operator=( deque&& _Right ) {
        if ( *this != _Right ) {
            _Tidy();
            if ( !std::is_same_v< decltype( _Right._Aloc ), _Alloc > ) {
                _Construct( std::make_move_iterator( _Right.begin() ), std::make_move_iterator( _Right.end() ) );
            } else {
                _Take_contents( _Right );
            }
        }
        return *this;
    }

private:
    void _Take_contents( deque& _Right ) {
        std::swap( _Cont, _Right._Cont );
        std::swap( _Blocks, _Right._Blocks );
        std::swap( _Size, _Right._Size );
        std::swap( _Off, _Right._Off );
    }

public:
    void push_front( const value_type& _Val ) {
        emplace_front( _Val );
    }
    void push_front( value_type&& _Val ) {
        emplace_front( std::move( _Val ) );
    }

    void push_back( const value_type& _Val ) {
        emplace_back( _Val );
    }
    void push_back( value_type&& _Val ) {
        emplace_back( std::move( _Val ) );
    }

    template < class... _Args >
    decltype( auto ) emplace_front( _Args&&... _Arg ) {
        if ( _Off % _Bsize == 0 && ( _Size + _Bsize ) / _Bsize >= _Blocks ) {
            _Growblock( 1 );
        }
        _Off &= _Blocks * _Bsize - 1;
        auto _Newoff = _Off != 0 ? _Off : _Blocks * _Bsize;
        auto _Block  = _Get_block( --_Newoff );
        if ( _Cont[ _Block ] == nullptr ) {
            _Cont[ _Block ] = _Aloc.allocate( _Bsize );
        }
        std::allocator_traits< _Alloc >::construct( _Aloc, _Cont[ _Block ] + _Newoff % _Bsize, std::forward< _Args >( _Arg )... );
        _Off = _Newoff;
        ++_Size;
    }
    template < class... _Args >
    decltype( auto ) emplace_back( _Args&&... _Arg ) {
        _Emplace_back_internal( std::forward< _Args >( _Arg )... );
    }

    void pop_back() noexcept {
        auto _Newoff = _Off + _Size - 1;
        auto _Block  = _Get_block( _Newoff );
        std::allocator_traits< _Alloc >::destroy( _Aloc, _Cont[ _Block ] + _Newoff % _Bsize );
        if ( --_Size == 0 ) {
            _Off = 0;
        }
    }
    void resize( const size_type _Newsize ) {
        while ( _Newsize < _Size ) {
            pop_back();
        }
        while ( _Newsize > _Size ) {
            emplace_back();
        }
    }
    void resize( const size_type _Newsize, const value_type& _Val ) {
        while ( _Newsize < _Size ) {
            pop_back();
        }
        while ( _Newsize > _Size ) {
            emplace_back( _Val );
        }
    }
    template < class... _Args >
    iterator emplace( iterator _Where, _Args&&... _Arg ) {}

public:
    [[nodiscard]] inline bool empty() const noexcept {
        return _Size == 0;
    }

    [[nodiscard]] inline iterator begin() noexcept {
        return iterator( this, _Off );
    }

    [[nodiscard]] inline const_iterator begin() const noexcept {
        return const_iterator( const_cast< deque< value_type >* >( this ), _Off );
    }

    [[nodiscard]] inline iterator end() noexcept {
        return iterator( this, _Off + _Size );
    }

    [[nodiscard]] inline const_iterator end() const noexcept {
        return const_iterator( const_cast< deque< value_type >* >( this ), _Off + _Size );
    }

    [[nodiscard]] inline _Contptr data() const noexcept {
        return _Cont;
    }

    [[nodiscard]] inline size_type bsize() const noexcept {
        return _Bsize;
    }

public:
    ~deque() {
        _Tidy();
    }

private:
    void _Growblock( size_type _Count ) {
        /*std::cout << "current info"
                  << " off= " << _Off << " size= " << _Size << " blocks= " << _Blocks << std::endl;*/
        size_type _New_blocks = _Blocks > 0 ? _Blocks : 1;
        if ( _New_blocks - _Blocks < _Count || _New_blocks < _Min_Blocks ) {
            _New_blocks *= 2;
        }
        cont_allocate_type _Calty( _Aloc );
        _Count              = _New_blocks - _Blocks;
        size_type _Boff     = _Off / _Bsize;
        auto      _New_cont = _Calty.allocate( _Blocks + _Count );
        auto      _Ptr      = _New_cont + _Boff;

        _Ptr = std::uninitialized_copy( _Cont + _Boff, _Cont + _Blocks, _Ptr );
        if ( _Boff <= _Count ) {
            _Ptr = std::uninitialized_copy( _Cont, _Cont + _Boff, _Ptr );
            std::uninitialized_value_construct_n( _Ptr, _Count - _Boff );
            std::uninitialized_value_construct_n( _New_cont, _Boff );
        } else {
            std::uninitialized_copy( _Cont, _Cont + _Count, _Ptr );
            _Ptr = std::uninitialized_copy( _Cont + _Count, _Cont + _Boff, _Ptr );
            std::uninitialized_value_construct_n( _Ptr, _Count );
        }
        // std::_Destroy_range( _Cont + _Boff, _Cont + _Blocks );
        _Calty.deallocate( _Cont, _Blocks );

        _Cont = _New_cont;
        _Blocks += _Count;
    }
    template < class... _Args >
    void _Emplace_back_internal( _Args&&... _Arg ) {
        // std::cout << "prepare emplace arg" << std::endl;
        if ( ( _Off + _Size ) % _Bsize == 0 && ( _Size + _Bsize ) / _Bsize >= _Blocks ) {
            std::cout << "prepare growblock cur bolcks=" << _Blocks << std::endl;
            _Growblock( 1 );
            std::cout << "growblock finish cur bolcks=" << _Blocks << std::endl;
        }

        _Off &= _Blocks * _Bsize - 1;
        auto _Newoff = _Off + _Size;
        auto _Block  = _Get_block( _Newoff );
        if ( _Cont[ _Block ] == nullptr ) {
            // std::cout << "prepare allocate block" << std::endl;
            _Cont[ _Block ] = _Aloc.allocate( _Bsize );
        }
        std::allocator_traits< _Alloc >::construct( _Aloc, _Cont[ _Block ] + _Newoff % _Bsize, std::forward< _Args >( _Arg )... );
        ++_Size;
    }
    void _Tidy() {
        while ( !empty() ) {
            pop_back();
        }
        cont_allocate_type _Calty( _Aloc );
        for ( size_type _Block = 0; _Block < _Blocks; ++_Block ) {
            if ( _Cont[ _Block ] ) {
                _Aloc.deallocate( _Cont[ _Block ], _Bsize );
                // std::_Destroy_in_place( _Cont[ _Block ] );
            }
        }
        if ( _Cont ) {
            _Calty.deallocate( _Cont, _Blocks );
        }
        _Cont   = nullptr;
        _Blocks = 0;
    }

private:
    _Contptr      _Cont{ nullptr };  // the pointer to blocks
    size_type     _Blocks{ 0 };      // the number of the bkicks
    size_type     _Off{ 0 };         // the offset of the elemt
    size_type     _Size{ 0 };        // the number of the elems
    allocate_type _Aloc{};
};
}  // namespace jz