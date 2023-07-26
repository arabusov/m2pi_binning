
#include <iostream>
// needed for ctb.hpp
#include <array>
#include <cmath>
#include <algorithm>

constexpr double mpi=0.13957039;
constexpr int n_m3pi_bins = 44;
constexpr double max_m2pi(int m2pi_bin) { return 0.92 + 0.02*m2pi_bin - mpi; }
// In this section units are MeV
constexpr double start_at = 1e+3*mpi * 2;
constexpr double stop_at = 1840.;

#include "meta.hpp"
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
    std::cout << "Size of table: " << table.size() << std::endl;
}

void print_last(const std::vector<double> &edges,
        const std::vector<int> &last_inds)
{
    bool cr = false;
    for (auto i = 0; i < last_inds.size(); i++) {
        const auto m3pi = 0.92+i*0.02;
        const auto edge = edges[last_inds[i]];
        cr = false;
        std::cout << std::setw(8) << m3pi-mpi << ":" << std::setw(7) << edge
            << ",   ";
        if (03 == (i&03)) {
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
    std::cout << "0++ table:" << std::endl;
    print_table(a2v(zpp::edges_table));
    std::cout << "Last m2pis for 0++" << std::endl;
    print_last(a2v(zpp::edges_table), a2vi(zpp::last_m2pis));
    std::cout << "1-- table:" << std::endl;
    print_table(a2v(omm::edges_table));
    print_last(a2v(omm::edges_table), a2vi(omm::last_m2pis));
    const auto m2pi_bins3 = omm::make_m2pi_bins<0>();
    print_table(a2v(m2pi_bins3));
    const auto m2pi_bins = omm::make_m2pi_bins<2>();
    print_table(a2v(m2pi_bins));
    const auto m2pi_bins0 = omm::make_m2pi_bins<20>();
    print_table(a2v(m2pi_bins0));
    return 0;
}
