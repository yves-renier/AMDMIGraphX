#ifndef MIGRAPHX_GUARD_RTGLIB_ARRAY_HPP
#define MIGRAPHX_GUARD_RTGLIB_ARRAY_HPP

#include <migraphx/config.hpp>
#include <migraphx/functional.hpp>
#include <migraphx/requires.hpp>
#include <type_traits>
#include <array>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {

namespace detail {

template <class R, class...>
struct array_type
{
    using type = R;
};
template <class... Ts>
struct array_type<void, Ts...> : std::common_type<Ts...>
{
};

template <class R, class... Ts>
using array_type_t = typename array_type<R, Ts...>::type;

template <class T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N> to_array_impl(T (&a)[N], seq<I...>)
{
    return {{a[I]...}};
}

} // namespace detail

template <class Result = void, class... Ts, MIGRAPHX_REQUIRES((sizeof...(Ts) > 0))>
constexpr std::array<detail::array_type_t<Result, Ts...>, sizeof...(Ts)> make_array(Ts&&... xs)
{
    return {static_cast<detail::array_type_t<Result, Ts...>>(std::forward<Ts>(xs))...};
}

constexpr std::array<int, 0> make_array() { return {}; }

template <class T, std::size_t N>
constexpr auto to_array(T (&a)[N])
{
    return detail::to_array_impl(a, detail::gens<N>{});
}

namespace detail {

template <std::size_t Offset = 0, class Array, std::size_t... I>
constexpr auto rearray_impl(Array a, seq<I...>)
{
    return make_array(a[I + Offset]...);
}

} // namespace detail

template <class T, std::size_t N>
constexpr auto pop_front(std::array<T, N> a)
{
    return detail::rearray_impl(a, detail::gens<N - 1>{});
}

template <class T, std::size_t N>
constexpr auto pop_back(std::array<T, N> a)
{
    return detail::rearray_impl<1>(a, detail::gens<N - 1>{});
}

} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx

#endif
