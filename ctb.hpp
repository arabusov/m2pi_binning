/*
 * Make uneven m2pi binning in compile-time
 *
 * Code is a C&P from different sources
 * always include these two before including ctb:
 * #include <array>
 * #include <cmath>
 * #include <algorithm>
 */


constexpr int n_trans = sizeof(bin_table)/sizeof(bin_table[0][0])/2;

constexpr int n_el(double start, double end, double step)
{
    return std::ceil((end-start)/step);
}
template <int entry> constexpr
auto make_primitive()
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


constexpr int n_m3pi_bins = 44;
constexpr auto last_m2pis{[]()
{
    std::array<double, n_m3pi_bins>res{};
    for_<n_m3pi_bins>([&](auto i) {
            constexpr double new_edge =  0.92 + 0.02*i.value - mpi;
            constexpr int k = count_if(edges_table, [&](const auto x) {
                    return x < new_edge;
                    }) - 1;
            constexpr double dprev = edges_table[k] - edges_table[k-1];
            constexpr double dlast = new_edge - edges_table[k];
            static_assert(dlast > 0);
            if (dlast < dprev/2.)
                res[i.value] = k - 1;
            else
                res[i.value] = k;
    });
    return res;
}()
};
