#include <iostream>
#include "meta.hpp"

constexpr double mpi=0.13957039;
constexpr int n_m3pi_bins = 44;
constexpr double max_m2pi(int m2pi_bin) { return 0.92 + 0.02*m2pi_bin - mpi; }
// In this section units are MeV
constexpr double start_at = 1e+3*mpi * 2;
constexpr double stop_at = 1840.;

namespace omm {
    // 1-- table in MeV
    constexpr double bin_table[][2] {
        //   m(2pi) bin step
            { 280, 40 },
            { 620, 20 },
            { 770, 2 },
            { 792, 8 },
            { 800, 20 },
            { 920, 40 },
            {stop_at, 40 }  // effective end
    };
#include "ctb.hpp"
}

namespace zpp {
    // 0++ table in MeV
    constexpr double bin_table[][2] {
        //   m(2pi) bin step
            { 280, 40 },
            { 920, 10 },
            { 1080, 40 },
            { stop_at, 40 }  // effective end
    };
#include "ctb.hpp"
}

constexpr int max_n_m2pi_bins = omm::max_n_m2pi_bins > zpp::max_n_m2pi_bins ?
omm::max_n_m2pi_bins  : zpp::max_n_m2pi_bins;
constexpr
std::array<std::array<std::array<double, max_n_m2pi_bins>, n_m3pi_bins>, 2>
m2pi_table{
    zpp::make_m2pi_isobar_table<max_n_m2pi_bins>(),
    omm::make_m2pi_isobar_table<max_n_m2pi_bins>()
};
#include <iomanip>
#include <vector>
void print_table(const std::vector<double> &table)
{
    bool cr = false;
    for (auto i = 0; i < table.size(); i++) {
        cr = false;
        std::cout << std::setw(10) << table[i];
        if (07 == (i&07)) {
            std::cout << std::endl;
            cr = true;
        }
    }
    if (not cr)
        std::cout << std::endl;
}

#define a2v(x) (std::vector<double>(x.cbegin(), x.cend()))
#define a2vi(x) (std::vector<int>(x.cbegin(), x.cend()))
int main()
{
    for (auto w = 0; w < 2; w++) {
        for (auto i = 0; i < n_m3pi_bins; i++) {
            std::cout << "********* Wave # " << w << " *********" << std::endl;
            std::cout << "********** Bin # " << i << " *********" << std::endl;
            print_table(a2v(m2pi_table[w][i]));
        }
    }
    return 0;
}
