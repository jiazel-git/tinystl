/**
 * @description: algorithm.hpp
 * @Date       :2023/12/29 14:33:10
 * @Author     :lijiaze
 * @Version    :1.0
 */

namespace jz {
template < typename InputIterator, typename Func >
Func for_each( InputIterator first, InputIterator last, Func func ) {
    for ( ; first != last; ++first ) {
        func( *first );
    }
    return func;
}
}  // namespace jz