
#include <iostream>
#include <chrono>
#include <cstdint>

#include "manage.h"

constexpr uint64_t pow10(int pow)
{
    uint64_t ret = 1;
    for(int i = 0; i < pow; ++i)
    {
        ret *= 10;
    }
    return ret;
}

using namespace std::chrono;

int main(int argc, char**argv)
{
    uint64_t n = pow10(8);
    auto start = high_resolution_clock::now();

    long adecousCount = solve(0, n, 8);

    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
    std::cout << adecousCount << " adecous count under " << n << "\n";
    std::cout << (double) duration / 1000000 << " seconds  " << (double)duration * 1000 / n << "\n";

    return 0;
}