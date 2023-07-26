#ifndef __META_HPP__
#define __META_HPP__
#include <array>
#include <cmath>
#include <algorithm>

constexpr int n_el(double start, double end, double step)
{
    return std::ceil((end-start)/step);
}

namespace detail {
    template<std::size_t ... Size> struct num_tuple{};

    template<std::size_t Prepend, typename T> struct appender {};

    template<std::size_t Prepend, std::size_t ... Sizes>
        struct appender<Prepend, num_tuple<Sizes...>>
        {
            using type = num_tuple<Prepend, Sizes...>;
        };

    template<std::size_t Size, std::size_t Counter = 0>
        struct counter_tuple
        {
            using type =
                typename appender<Counter,
                         typename counter_tuple<Size, Counter+1>::type>::type;
        };


    template<std::size_t Size>
        struct counter_tuple<Size, Size>
        {
            using type = num_tuple<>;
        };

}

template<typename T, std::size_t LL, std::size_t RL,
    std::size_t ... LLs, std::size_t ... RLs>
    constexpr std::array<T, LL+RL>
join(const std::array<T, LL> rhs, const std::array<T, RL> lhs,
        detail::num_tuple<LLs...>, detail::num_tuple<RLs...>)
{
	return {rhs[LLs]..., lhs[RLs]... };
};


template<typename T, std::size_t LL, std::size_t RL>
    constexpr std::array<T, LL+RL>
join(std::array<T, LL> rhs, std::array<T, RL> lhs)
{
    return join(rhs, lhs, typename detail::counter_tuple<LL>::type(),
            typename detail::counter_tuple<RL>::type());
}

// C&P from stackoverflow
template<typename T, size_t N, typename Pred, size_t... Is>
constexpr size_t count_if_helper(std::array<T, N> const& arr, Pred&& pred,
        std::index_sequence<Is...>*){
    return ((size_t)(bool)pred(arr[Is]) + ...);
}

template<typename T, size_t N, typename Pred>
constexpr size_t count_if(std::array<T, N> const& arr, Pred&& pred){
    return count_if_helper(arr, std::forward<Pred>(pred),
            (std::make_index_sequence<N>*)nullptr);
}

template<std::size_t N>
struct num { static const constexpr int value = N; };

template <class F, std::size_t... Is> constexpr
void for_(F func, std::index_sequence<Is...>)
{
    (func(num<Is>{}), ...);
}

template <std::size_t N, typename F> constexpr
void for_(F func)
{
    for_(func, std::make_index_sequence<N>());
}

#endif /* __META_HPP__ */
