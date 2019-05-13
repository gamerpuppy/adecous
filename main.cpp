
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

using namespace std;


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

void count(uint64_t lb, uint64_t ub, int tc)
{
    uint64_t adecousCount = solve(lb, ub, tc);
    cout << lb << " " << ub << '\n';
    cout << adecousCount << '\n';
}

void testmul(int n, uint64_t ub)
{
    Solver solver(21, ub);
    for (uint64_t mul = 0; mul < ub/n; ++mul)
    {
        int count = 0;
        for (uint64_t mod = 0; mod < n; mod++)
        {
            auto x = mul * n + mod;
            count += solver.isAdecous(x);
        }
        if (count != 0)
        {
            cout << mul << '\n';
        }
    }
}

int main(int argc, char**argv)
{

    if(argc < 2)
    {
        cout << "usage: command args\n";
        return 1;
    }

    const string command = string(argv[1]);
    if (command == "test") {
        test();

    } else if (command == "count") {
        uint64_t lb = atoll(argv[1]);
        uint64_t ub = atoll(argv[2]);
        int tc = atoi(argv[3]);
        count(lb, ub, tc);

    } else if (command == "picture") {
        const string filename = argv[2];
        uint64_t lb = atoll(argv[3]);
        uint64_t ub = atoll(argv[4]);
        uint64_t width = atoll(argv[5]);
        generatePicure(filename, lb, ub, width);

    } else if (command == "testmul") {
        int n = atoi(argv[2]);
        testmul(n, 100000);

    }

    return 0;
}













