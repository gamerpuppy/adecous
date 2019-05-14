//
// Created by Keegan Millard on 2019-05-12.
//

#include "manage.h"

#include <thread>
#include <atomic>
#include <fstream>
#include <cmath>

struct Task {
    const Solver &solver;
    const uint64_t start;
    const uint64_t ub;
    const int inc;
    std::atomic_llong &counter;

    Task(const Solver &solver, const uint64_t start, const uint64_t ub, const int inc, std::atomic_llong &counter)
            : solver(solver), start(start), ub(ub), inc(inc), counter(counter) {}

    void operator()() {
        uint64_t count = 0;
        for (uint64_t x = start; x < ub; x += inc) {
            count += solver.isAdecous(x);
        }
        counter += count;
    }
};

uint64_t solve(const Solver &solver, uint64_t lb, uint64_t ub, int tc)
{
    std::atomic_llong count(0);

    std::thread *threads[tc];
    for (int tn = 0; tn < tc; ++tn)
    {
        std::thread *threadP =
                new std::thread(Task(solver, lb+tn, ub, tc, count));
        threads[tn] = threadP;
    }

    for (auto thread : threads)
    {
        thread->join();
        delete thread;
    }
    return count;
}


void generatePicure(const std::string &filepath, uint64_t lb, uint64_t ub, uint64_t width) {
    uint64_t size = ub-lb;
    uint64_t height = ceil((double)size/width);
    const std::string file = filepath+"_"+std::to_string(width)+"_"+std::to_string(height)+".pgm";
    std::ofstream out(file);

    out << "P2 " << width << " " << height << " " << 255 << '\n';
    const Solver solver;
    for(uint64_t x = lb; x < ub; ++x)
    {
        int val = solver.isAdecous(x) ? 255 : 0;
        out << val << " ";
    }

    uint64_t rem = width*height - size;
    for(uint64_t i = 0; i < rem; i++)
        out << "0 ";

    out << "\n";

    out.close();
}





