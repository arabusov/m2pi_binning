/*
 * Make uneven m2pi binning in compile-time
 *
 * Code is a C&P from different sources
 * always include these two before including ctb:
 * #include <array>
 * #include <cmath>
 */


constexpr int n_trans = sizeof(bin_table)/sizeof(bin_table[0][0])/2;

constexpr int n_el(double start, double end, double step)
{
    return std::ceil((end-start)/step);
}

template <int entry> constexpr auto make_primitive()
{
    constexpr double start = bin_table[entry][0],
    end = bin_table[entry+1][0],
    step = bin_table[entry][1];
    constexpr int N = n_el(start, end, step);
    std::array<double, N> res {};
    res[0] = start;
    for (auto i = 1; i < N; i++) {
        res[i] = res[i-1] + step;
    }
    if (res[0] == 280)
        res[0] = start_at;
    if (res[N-1] > stop_at)
        res[N-1] = stop_at;
    for (auto i = 0; i < N; i++) {
        res[i] *= 1e-3; // convert to GeV
    }
    return res;
}

namespace detail {
    template<std::size_t ... Size>
        struct num_tuple
        {

        };

    template<std::size_t Prepend, typename T>
        struct appender {};

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

template<typename T, std::size_t LL, std::size_t RL, std::size_t ... LLs,
    std::size_t ... RLs> constexpr
std::array<T, LL+RL> join(const std::array<T, LL> rhs,
        const std::array<T, RL> lhs, detail::num_tuple<LLs...>,
        detail::num_tuple<RLs...>)
{
	return {rhs[LLs]..., lhs[RLs]... };
};


template<typename T, std::size_t LL, std::size_t RL> constexpr
std::array<T, LL+RL> join(std::array<T, LL> rhs, std::array<T, RL> lhs)
{
    return join(rhs, lhs, typename detail::counter_tuple<LL>::type(),
            typename detail::counter_tuple<RL>::type());
}

template<int N>
struct concated {
    static constexpr int size { make_primitive<N-1>().size()
                              + concated<N-1>::size };
    static constexpr std::array<double, size> a { join(concated<N-1>::a,
                                                  make_primitive<N-1>()) };
};

template<>
struct concated<0> {
    static constexpr int size { 0 };
    static constexpr std::array<double, 0> a{};
};

constexpr auto edges_table {concated<n_trans-1>::a};
