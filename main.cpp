
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

void test()
{
    using namespace std::chrono;

    uint64_t n = pow10(8);
    auto start = high_resolution_clock::now();

    long adecousCount = solve(0, n, 8);

    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
    std::cout << adecousCount << " adecous count under " << n << "\n";
    std::cout << (double) duration / 1000000 << " seconds  " << (double)duration * 1000 / n << "\n";
}

int main(int argc, char**argv)
{
    using namespace std;

    if (argc == 2) {
        string arg2(argv[0]);
        if (arg2 == "test") {
            test();
            return 0;
        }

    } else if (argc < 4) {
        cout << "usage: lowerBound, upperBound, threads\n";
        return 1;

    }

    uint64_t lb = atoll(argv[1]);
    uint64_t ub = atoll(argv[2]);
    int tc = atoi(argv[3]);

    uint64_t adecousCount = solve(lb, ub, tc);

    cout << lb << " " << ub << '\n';
    cout << adecousCount << '\n';

    return 0;
}













