
#include <iostream>
// needed for ctb.hpp
#include <array>
#include <cmath>

// In this section units are MeV
constexpr double start_at = 279.;
constexpr double stop_at = 1800.;

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
            {1840, 40 }  // effective end
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
            { 1840, 40 }  // effective end
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

int main()
{
    print_table(std::vector<double>(omm::edges_table.cbegin(), omm::edges_table.cend()));
    print_table(std::vector<double>(zpp::edges_table.cbegin(), zpp::edges_table.cend()));
    return 0;
}
