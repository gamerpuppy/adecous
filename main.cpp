
#include <iostream>
#include <chrono>
#include <cstdint>
#include <set>

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

uint64_t count(uint64_t lb, uint64_t ub, const Solver& solver)
{
    uint64_t count = 0;
    for(auto x = lb; x < ub; ++x)
    {
        count += solver.isAdecous(x);
    }
    return count;
}


using namespace std::chrono;

int main(int argc, char**argv)
{
    if(argc < 2)
    {
        cout << "usage: command args\n";
        return 1;
    }

    const string command = string(argv[1]);
    if (command == "count") {
        uint64_t lb = atoll(argv[2]);
        uint64_t ub = atoll(argv[3]);
        int tc = atoi(argv[4]);
        auto res = count(lb, ub, tc);
        cout << lb << " " << ub << " " << res << '\n';

    } else if (command == "countfast") {
        uint64_t lb = atoll(argv[2]);
        uint64_t ub = atoll(argv[3]);
        int tc = atoi(argv[4]);
        Solver solver(16);
        auto res = solver.getCountThreaded(lb,ub,tc);
        cout << lb << " " << ub << " " << res << '\n';

    }

    return 0;
}













