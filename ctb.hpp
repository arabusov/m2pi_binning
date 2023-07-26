/*
 * Make uneven m2pi binning in compile-time
 *
 * Code is a C&P from different sources
 * always include these two before including ctb:
 * #include "meta.hpp"
 */

constexpr int n_trans = sizeof(bin_table)/sizeof(bin_table[0][0])/2;

static_assert(n_trans >= 2);
static_assert(bin_table[n_trans-1][0] > bin_table[n_trans-2][0]);

template <int entry> constexpr
auto make_primitive()
{
    constexpr double start = bin_table[entry][0],
    end = bin_table[entry+1][0],
    step = bin_table[entry][1];
    static_assert(end >= (start + step));
    static_assert(step > 0.);
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

constexpr auto edges_table { concated<n_trans-1>::a };

constexpr auto last_m2pis { []() {
    std::array<int, n_m3pi_bins> res{};
    for_<n_m3pi_bins>([&](auto i) {
            constexpr double new_edge =  max_m2pi(i.value);
            constexpr int k = count_if(edges_table, [&](const auto x) {
                    return x < new_edge; }) - 1;
            constexpr double dprev = edges_table[k] - edges_table[k-1];
            constexpr double dlast = new_edge - edges_table[k];
            static_assert(dlast > 0);
            if (dlast < dprev/2.)
                res[i.value] = k - 1;
            else
                res[i.value] = k;
            });
    return res;
}() };

template<int m3pi_bin>
constexpr auto make_m2pi_bins()
{
    constexpr int n_m2pi_bins = last_m2pis[m3pi_bin]+2;
    std::array<double, n_m2pi_bins> res{};
    for_<n_m2pi_bins-1>([&](auto i) {
            res[i.value] = edges_table[i.value];
            });
    res[n_m2pi_bins-1] = max_m2pi(m3pi_bin);
    return res;
}
