
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

uint64_t count(uint64_t lb, uint64_t ub)
{
    uint64_t count = 0;
    for(auto x = lb; x < ub; ++x)
    {
        count += isAdecousSingleStep(x);
    }
    return count;
}

using namespace std::chrono;

int main(int argc, char**argv)
{
    Solver solver(18);

    uint64_t count = 0;
    for (int i = 1; i < 42; ++i)
    {
        uint64_t lb = 1ull << (i-1);
        uint64_t ub = 1ull << i;
        auto res = solver.getCountThreaded(lb, ub, 12);
        count += res;
        double percent = (double)count*100/ub;
        cout << "2^" << i << " " << count << " (" << percent << "%)" << endl;
    }

//    uint64_t lb = 0;
//    uint64_t ub = 1ull << 33;
//    int tc = 8;

//    Solver solver(16);
//
//    auto start = high_resolution_clock::now();
//    auto res = solver.getCountThreaded(lb,ub,tc);
//    auto end = high_resolution_clock::now();
//
//    cout << lb << " " << ub << " " << res << '\n';
//    cout << duration_cast<milliseconds>(end-start).count() << '\n';

//    start = high_resolution_clock::now();
//    res = solve(solver, lb, ub, tc);
//    end = high_resolution_clock::now();
//
//    cout << lb << " " << ub << " " << res << '\n';
//    cout << duration_cast<milliseconds>(end-start).count() << '\n';

//    start = high_resolution_clock::now();
//    res = solver.getCount(lb, ub);
//    end = high_resolution_clock::now();
//
//    cout << lb << " " << ub << " " << res << '\n';
//    cout << duration_cast<milliseconds>(end-start).count() << '\n';

//    start = high_resolution_clock::now();
//    res = count(lb, ub);
//    end = high_resolution_clock::now();
//
//    cout << lb << " " << ub << " " << res << '\n';
//    cout << duration_cast<milliseconds>(end-start).count() << '\n';
//

//    if(argc < 2)
//    {
//        cout << "usage: command args\n";
//        return 1;
//    }
//
//    const string command = string(argv[1]);
//    if (command == "count") {
//        uint64_t lb = atoll(argv[2]);
//        uint64_t ub = atoll(argv[3]);
//        int tc = atoi(argv[4]);
//        auto res = solve(lb, ub, tc);
//        cout << lb << " " << ub << " " << res << '\n';
//
//    } else if (command == "countfast") {
//        uint64_t lb = atoll(argv[2]);
//        uint64_t ub = atoll(argv[3]);
//        int tc = atoi(argv[4]);
//        Solver solver(16);
//        auto res = solver.getCountThreaded(lb,ub,tc);
//        cout << lb << " " << ub << " " << res << '\n';
//
//    }

    return 0;
}













