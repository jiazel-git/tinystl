/**
 * @description: memory.hpp
 * @Date       :2024/02/05 11:10:39
 * @Author     :lijiaze
 * @Version    :1.0
 */
#pragma once
#include <iostream>
#include <mutex>

namespace jz {

template < typename _Ty >
class shared_ptr;

// 引用计数基类，用来控制指针引用情况
class ref_count_base {

private:
    virtual void destroy() noexcept     = 0;
    virtual void delete_this() noexcept = 0;

protected:
    unsigned long _uses  = 1;
    unsigned long _weaks = 1;

    constexpr ref_count_base() noexcept = default;

private:
    std::mutex _mutex{};

public:
    ref_count_base( const ref_count_base& )            = delete;
    ref_count_base& operator=( const ref_count_base& ) = delete;
    virtual ~ref_count_base() noexcept {}

    bool incref_nz() noexcept {
        return true;
    }
    void incwref() noexcept {
        lock_and_incwref();
    }
    void incref() {
        lock_and_incref();
    }
    void decwref() {
        lock_and_decwref();
    }
    void decref() {
        lock_and_decref();
    }
    long use_count() const noexcept {
        return static_cast< long >( _uses );
    }
    virtual void* get_deleter( const type_info& ) const noexcept {
        return nullptr;
    }

private:
    void lock_and_incwref() {
        std::lock_guard< std::mutex > lg{ _mutex };
        ++_weaks;
    }
    void lock_and_incref() {
        std::lock_guard< std::mutex > lg{ _mutex };
        ++_uses;
    }
    void lock_and_decwref() {
        std::lock_guard< std::mutex > lg{ _mutex };
        if ( --_weaks == 0 ) {
            delete_this();
        }
    }
    void lock_and_decref() {
        std::lock_guard< std::mutex > lg{ _mutex };
        if ( --_uses == 0 ) {
            destroy();
            if ( --_weaks == 0 ) {
                delete_this();
            }
        }
    }
};
// 引用计数类，管理引用计数资源
template < typename _Ty >
class ref_count : public ref_count_base {
public:
    explicit ref_count( _Ty* ptr ) : ref_count_base(), _ptr( ptr ) {}

private:
    virtual void destroy() noexcept override {
        delete _ptr;
    }
    virtual void delete_this() noexcept override {
        delete this;
    }

private:
    _Ty* _ptr;
};

// 真正的引用计数资源
template < typename _Resource, typename _Dx >
class ref_count_resource : public ref_count_base {
public:
    ref_count_resource( _Resource resource, _Dx deleter ) : ref_count_base(), _resource( resource ), _deleter( std::move( deleter ) ) {}
    virtual ~ref_count_resource() noexcept override = default;
    virtual void* get_deleter( const type_info& typeID ) const noexcept override {
        if ( typeID == typeid( _Dx ) ) {
            return const_cast< _Dx* >( &_deleter );
        }
        return nullptr;
    }

private:
    virtual void destroy() noexcept override {
        _deleter( _resource );
    }
    virtual void delete_this() noexcept override {
        delete this;
    }

private:
    _Resource _resource;
    _Dx       _deleter;
};

template < typename _Ty >
class ptr_base {
public:
    using element_type = std::remove_extent_t< _Ty >;
    long use_count() const noexcept {
        return _rep ? _rep->use_count() : 0;
    }
    template < typename _Ty2 >
    bool owner_before( const ptr_base< _Ty2 >& right ) const noexcept {
        return _rep < right._rep;
    }
    ptr_base( const ptr_base& )            = delete;
    ptr_base& operator=( const ptr_base& ) = delete;

protected:
    element_type* get() const noexcept {
        return _ptr;
    }
    constexpr ptr_base() noexcept = default;
    ~ptr_base()                   = default;

    template < typename _Ty2 >
    void move_construct_from( ptr_base< _Ty2 >&& right ) noexcept {
        _ptr = right._ptr;
        _rep = right._rep;

        right._ptr = nullptr;
        right._rep = nullptr;
    }

    template < typename _Ty2 >
    void copy_construct_from( const ptr_base< _Ty2 >& other ) noexcept {
        other.incref();

        _ptr = other._ptr;
        _rep = other._rep;
    }

    template < typename _Ty2 >
    void alias_construct_from( const shared_ptr< _Ty2 >& other, element_type* px ) noexcept {
        other.incref();

        _ptr = px;
        _rep = other._rep;
    }

    template < typename _Ty2 >
    void alias_move_construct_from( shared_ptr< _Ty2 >&& other, element_type* px ) noexcept {
        _ptr = px;
        _rep = other._rep;

        other.ptr  = nullptr;
        other._rep = nullptr;
    }

    void incref() const noexcept {
        if ( _rep ) {
            _rep->incref();
        }
    }

    void decref() const noexcept {
        if ( _rep ) {
            _rep->decref();
        }
    }

    void swap( ptr_base& right ) {
        using std::swap;
        swap( _ptr, right._ptr );
        swap( _rep, right._rep );
    }

private:
    element_type*   _ptr{ nullptr };
    ref_count_base* _rep{ nullptr };

    template < class _Ty0 >
    friend class ptr_base;

    friend shared_ptr< _Ty >;
};

template < typename _Ty >
class shared_ptr : public ptr_base< _Ty > {
private:
    using _mybase = ptr_base< _Ty >;

public:
    using typename _mybase::element_type;

    constexpr shared_ptr() noexcept = default;

    constexpr shared_ptr( nullptr_t ) noexcept {}

    /*template < typename _Ux,
               std::enable_if_t< std::conjunction_v< std::conditional_t< std::is_array_v< _Ty >, std::_Can_array_delete< _Ux >, std::_Can_scalar_delete< _Ux > >, std::_SP_convertible< _Ux, _Ty > >,
                                 int > = 0 >*/
    // template < typename _Ux, typename std::enable_if< std::is_convertible< _Ty, _Ux >::value, int >::type >
    explicit shared_ptr( element_type* px ) {
        if ( std::is_array< _Ty >::value ) {
            setpd( px, std::default_delete< element_type[] >{} );
        }
        else {
            set_ptr_rep_and_enable_shared( px, new ref_count< element_type >( px ) );
        }
    }

    /*template < typename _Ux,
               typename _Dx,
               std::enable_if_t< std::conjunction_v< std::conditional_t< std::is_array_v< _Ty >, std::_Can_array_delete< _Ux >, std::_Can_scalar_delete< _Ux > >, std::_SP_convertible< _Ux, _Ty > >,
                                 int > = 0 >*/
    template < typename _Dx >
    explicit shared_ptr( element_type* px, _Dx dx ) {
        setpd( px, std::move( dx ) );
    }

    /*template < typename _Dx,
               std::enable_if_t< std::conjunction_v< std::conditional_t< std::is_array_v< _Ty >, std::_Can_array_delete< _Dx >, std::_Can_scalar_delete< _Dx > >, std::_SP_convertible< _Dx, _Ty > >,
                                 int > = 0 >*/
    template < typename _Dx >
    explicit shared_ptr( nullptr_t, _Dx dx ) {
        setpd( nullptr, std::move( dx ) );
    }

    template < typename _Ty2 >
    shared_ptr( const shared_ptr< _Ty2 >& right, element_type* px ) noexcept {
        this->alias_construct_from( right, px );
    }

    template < typename _Ty2 >
    shared_ptr( shared_ptr< _Ty2 >&& right, element_type* px ) noexcept {
        this->alias_move_construct_from( std::move( right ), px );
    }

    shared_ptr( const shared_ptr& other ) noexcept {
        this->copy_construct_from( other );
    }

    shared_ptr( shared_ptr&& other ) noexcept {
        this->move_construct_from( std::move( other ) );
    }

    // template < class _Ty2, std::enable_if_t< std::_SP_pointer_compatible< _Ty2, _Ty >::value, int > = 0 >
    template < class _Ty2 >
    shared_ptr( shared_ptr< _Ty2 >&& right ) noexcept {  // construct shared_ptr object that takes resource from _Right
        this->move_construct_from( std::move( right ) );
    }
    ~shared_ptr() noexcept {
        this->decref();
    }

    shared_ptr& operator=( const shared_ptr& right ) noexcept {
        shared_ptr( right ).swap( *this );
        return *this;
    }

    template < typename _Ty2 >
    shared_ptr& operator=( const shared_ptr< _Ty2 >& right ) noexcept {
        shared_ptr( right ).swap( *this );
        return *this;
    }

    shared_ptr& operator=( shared_ptr&& right ) noexcept {
        shared_ptr( std::move( right ) ).swap( *this );
        return *this;
    }

    template < typename _Ty2 >
    shared_ptr& operator=( shared_ptr< _Ty2 >&& right ) noexcept {
        shared_ptr( std::move( right ) ).swap( *this );
        return *this;
    }
    void swap( shared_ptr& other ) noexcept {
        this->swap( other );
    }

    void reset() noexcept {
        shared_ptr().swap( *this );
    }

    template < typename _Ux >
    void reset( _Ux px ) {
        shared_ptr( px ).swap( *this );
    }

    template < typename _Ux, typename _Dx >
    void reset( _Ux* px, _Dx dx ) {
        shared_ptr( px, dx ).swap( *this );
    }

    using _mybase::get;
    // template < typename _Ty2 = _Ty, std::enable_if_t< !std::disjunction_v< std::is_array< _Ty2 >, std::is_void< _Ty2 > >, int > >
    template < typename _Ty2 = _Ty >
    _Ty2& operator*() const noexcept {
        return *get();
    }

    // template < typename _Ty2 = _Ty, std::enable_if_t< !std::is_array_v< _Ty2 >, int > = 0 >
    template < typename _Ty2 = _Ty >
    _Ty2* operator->() const noexcept {
        return get();
    }

    // template < typename _Ty2 = _Ty, typename _Elem = element_type, std::enable_if_t< std::is_array_v< _Ty2 > > = 0 >
    template < typename _Ty2 = _Ty, typename _Elem = element_type, std::enable_if_t< std::is_array< _Ty2 >::value > = 0 >
    _Elem& operator[]( std::ptrdiff_t idx ) const noexcept {
        return get()[ idx ];
    }

    explicit operator bool() const noexcept {
        return get() != nullptr;
    }

private:
    template < typename _UxptrOrNullptr, typename _Dx >
    void setpd( const _UxptrOrNullptr px, _Dx dx ) {
        set_ptr_rep_and_enable_shared( px, new ref_count_resource< _UxptrOrNullptr, _Dx >( px, std::move( dx ) ) );
    }

    template < typename _Ux >
    void set_ptr_rep_and_enable_shared( _Ux* const px, ref_count_base* const rx ) noexcept {
        this->_ptr = px;
        this->_rep = rx;
    }

    void set_ptr_rep_and_enable_shared( nullptr_t, ref_count_base* const rx ) noexcept {
        this->_ptr = nullptr;
        this->_rep = rx;
    }
};
}  // namespace jz
