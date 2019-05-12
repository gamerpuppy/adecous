//
// Created by Keegan Millard on 2019-05-12.
//

#include "manage.h"
#include <thread>
#include <atomic>

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

uint64_t solve(uint64_t lb, uint64_t ub, int tc)
{
    const Solver solver;
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
