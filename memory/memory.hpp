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

    bool incref_nz() noexcept {}
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
            decwref();
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
class ref_count_resource : ref_count_base {
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
    void alias_construct_from( const shard_ptr< _Ty2 >& other, element_type* px ) noexcept {
        other.incref();

        _ptr = px;
        _rep = other._rep;
    }

private:
    element_type*   _ptr{ nullptr };
    ref_count_base* _rep{ nullptr };

    template < class _Ty0 >
    friend class ptr_base;

    friend shard_ptr< _Ty >;
};

template < typename _Ty >
class shard_ptr : public ptr_base< _Ty > {};
}  // namespace jz
