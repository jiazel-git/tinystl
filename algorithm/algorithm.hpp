/**
 * @description: algorithm.hpp
 * @Date       :2023/12/29 14:33:10
 * @Author     :lijiaze
 * @Version    :1.0
 */
#include "iostream"
namespace jz {
template < typename InputIterator, typename Func >
Func for_each( InputIterator first, InputIterator last, Func func ) {
    for ( ; first != last; ++first ) {
        func( *first );
    }
    return func;
}
template < typename InputIterator, typename Predicate >
InputIterator find_if_not( InputIterator first, InputIterator last, Predicate pred ) {
    for ( ; first != last; ++first ) {
        if ( !pred( *first ) ) {
            break;
        }
    }
    return first;
}
template < typename InputIterator, typename Predicate >
InputIterator find_if( InputIterator first, InputIterator last, Predicate pred ) {
    for ( ; first != last; ++first ) {
        if ( pred( first ) ) {
            break;
        }
    }
    return first;
}
template < typename InputIterator, typename Predicate >
size_t count_if( InputIterator first, InputIterator last, Predicate pred ) {
    size_t count = 0;
    for ( ; first != last; ++first ) {
        if ( pred( *first ) ) {
            ++count;
        }
    }
    return count;
}
template < typename InputIterator, typename Predicate >
bool all_of( InputIterator first, InputIterator last, Predicate pred ) {
    for ( ; first != last; ++first ) {
        if ( !pred( *first ) ) {
            return false;
        }
    }
    return true;
}
template < typename InputIterator, typename Predicate >
bool any_of( InputIterator first, InputIterator last, Predicate pred ) {
    for ( ; first != last; ++first ) {
        if ( pred( *first ) ) {
            return true;
        }
    }
    return false;
}
template < typename InputIterator, typename Predicate >
bool none_of( InputIterator first, InputIterator last, Predicate pred ) {
    for ( ; first != last; ++first ) {
        if ( pred( *first ) ) {
            return false;
        }
    }
    return true;
}
template < typename InputIterator, typename OutputIterator, typename Predicate >
OutputIterator copy_if( InputIterator first, InputIterator last, OutputIterator dest, Predicate pred ) {
    for ( ; first != last; ++first ) {
        if ( pred( *first ) ) {
            *dest = *first;
            ++dest;
        }
    }
    return dest;
}
template < typename ForwardIterator1, typename ForwardIterator2, typename Predicate >
ForwardIterator1 find_first_of( ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Predicate pred ) {
    for ( ; first1 != last1; ++first1 ) {
        for ( ; first2 != last2; ++first2 ) {
            if ( pred( *first1, *first2 ) ) {
                return first1;
            }
        }
    }
    return first1;
}
template < typename ForwardIterator1, typename ForwardIterator2 >
ForwardIterator1 find_first_of( ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2 ) {
    return find_first_of( first1, last1, first2, last2, std::equal_to<>{} );
}
template < typename InputIterator, typename OutputIterator, typename Func >
OutputIterator transform( InputIterator first, InputIterator last, OutputIterator dest, Func func ) {
    for ( ; first != last; ++first ) {
        *dest = func( *first );
        ++dest;
    }
    return dest;
}
template < typename InputIterator, typename OutputIterator, typename Func >
OutputIterator transform( InputIterator first1, InputIterator last1, InputIterator first2, OutputIterator dest, Func func ) {
    for ( ; first1 != last1; ++first1, ++first2 ) {
        *dest = func( *first1, *first2 );
        ++dest;
    }
    return dest;
}
template < typename ForwardIterator, typename value_type >
void replace( ForwardIterator first, ForwardIterator last, const value_type& old_val, const value_type& new_value ) {
    for ( ; first != last; ++first ) {
        if ( *first == old_val ) {
            *first = new_value;
        }
    }
}
template < typename ForwardIterator, typename Predicate, typename value_type >
void replace_if( ForwardIterator first, ForwardIterator last, Predicate pred, const value_type& val ) {
    for ( ; first != last; ++first ) {
        if ( pred( *first ) ) {
            *first = val;
        }
    }
}
template < typename InputIterator, typename OutputIterator, typename value_type >
OutputIterator replace_copy( InputIterator first, InputIterator last, OutputIterator dest, const value_type& old_val, const value_type& new_value ) {
    for ( ; first != last; ++first, ++dest ) {
        if ( *first == old_val ) {
            *dest = new_value;
        }
        else {
            *dest = *first;
        }
    }
    return dest;
}
template < typename InputIterator, typename OutputIterator, typename Predicate, typename value_type >
OutputIterator replace_copy_if( InputIterator first, InputIterator last, OutputIterator dest, Predicate pred, const value_type& val ) {
    for ( ; first != last; ++first, ++dest ) {
        if ( pred( *first ) ) {
            *dest = val;
        }
        else {
            *dest = *first;
        }
    }
    return dest;
}
template < typename InputIterator, typename OutputIterator, typename val_type >
OutputIterator remove_copy( InputIterator first, InputIterator last, OutputIterator dest, const val_type& val ) {
    for ( ; first != last; ++first ) {
        if ( *first != val ) {
            *dest = *first;
            ++dest;
        }
    }
    return dest;
}
template < typename InputIterator, typename OutputIterator, typename Predicate >
OutputIterator remove_copy_if( InputIterator first, InputIterator last, OutputIterator dest, Predicate pred ) {
    for ( ; first != last; ++first ) {
        if ( !pred( *first ) ) {
            *dest = *first;
            ++dest;
        }
    }
    return dest;
}
template < typename BidirectionalIterator, typename Predicate >
BidirectionalIterator insert_sort( const BidirectionalIterator first, const BidirectionalIterator last, Predicate pred ) {
    if ( first != last ) {
        for ( auto mid = first; ++mid != last; ) {
            auto hole = mid;
            auto val  = std::move( *mid );
            if ( pred( val, *first ) ) {
                std::move_backward( first, mid, ++hole );
                *first = std::move( val );
            }
            else {
                for ( auto prev = hole; pred( val, *--prev ); hole = prev ) {
                    *hole = std::move( *prev );
                }
                *hole = std::move( val );
            }
        }
    }

    return last;
}
}  // namespace jz