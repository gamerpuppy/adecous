
#include <iostream>
#include <stdint-gcc.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <limits>

#include "StepMemos.h"

StepMemos stepMemos;

int isAdecousSingleStep(uint64_t num)
{
    while(num != 1)
    {
        if (num == 10)
            return 0;
        else if (num % 2 == 0)
            num /= 2;
        else
            num = num * 3 + 1;
    }
    return 1;
}

//bool overFlows(uint64_t a, uint64_t c, uint64_t d)
//{
//    uint64_t cmax = std::numeric_limits<uint64_t>().max() / a;
//    if(c > cmax) {
//        return true;
//    }
//
//    uint64_t part = a * c;
//    uint64_t partMax = std::numeric_limits<uint64_t>().max() - d;
//    if(part > partMax)
//        return true;
//
//    return false;
//}


uint64_t getNext(int stepIdx, uint64_t num)
{
    uint64_t bMask = stepMemos.bMasks[stepIdx];
    uint32_t steps = stepMemos.kVals[stepIdx];

    uint64_t a = (num & ~bMask) >> steps;
    uint32_t b = num & bMask;

    Memo &memo = stepMemos.memos[stepIdx][b];
//    if(overFlows(a, memo.c, memo.d)){
//        return 1;
//    }

    return a * memo.c + memo.d;
}

int isAdecous(uint64_t num)
{
    while (true)
    {
        bool stepped = false;
        for (int i = 0; i < 3; i++) {
            long cmp = num >> stepMemos.kVals[i];

            if (cmp >= 10) {
                num = getNext(i, num);
                stepped = true;
                break;
            }
        }
        if(!stepped) {
            return isAdecousSingleStep(num);
        }
    }
}

uint64_t getAudecousCountSingleThread(uint64_t ub)
{
    uint64_t count = 0;
    for (uint64_t x = 1; x < ub; ++x) {
        count += isAdecousSingleStep(x);
    }
    return count;
}

uint64_t getAudecousCountCmp(uint64_t ub)
{
    uint64_t count = 0;
    for (uint64_t x = 1; x < ub; ++x) {
        uint64_t val1 = isAdecousSingleStep(x);
        uint64_t val2 = isAdecous(x);

        if(val1 != val2) {
            std::cout << x << " " << val1 << '\n';
        }

        count += val2;
    }
    return count;
}

uint64_t getAudecousCount(uint64_t ub)
{
    uint64_t count = 0;
    for (uint64_t x = 1; x < ub; ++x) {
        count += isAdecous(x);
    }
    return count;
}

void adecousCountTask(std::atomic_llong *counter, uint64_t start, uint64_t inc, uint64_t ub)
{
    uint64_t count = 0;
    for (uint64_t x = start; x < ub; x += inc) {
        count += isAdecous(x);
    }
    *counter += count;
}

uint64_t getAudecousCountThreaded(int tc, uint64_t ub)
{
    std::thread *threads[tc];
    std::atomic_llong counter(0);

    for (int tn = 0; tn < tc; ++tn)
    {
        std::thread *threadP = new std::thread(
                adecousCountTask,
                &counter,
                1 + tn,
                tc,
                ub);

        threads[tn] = threadP;
    }

    for (auto thread : threads)
    {
        thread->join();
        delete thread;
    }
    return counter;
}


using namespace std::chrono;

int main(int argc, char**argv)
{
    uint64_t n = 1000000000;

    auto start = high_resolution_clock::now();

//    uint64_t adecousCount = getAudecousCountCmp(n);

    uint64_t adecousCount = getAudecousCount(n);
//    uint64_t adecousCount = getAudecousCountThreaded(16, n);

    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start).count();

    std::cout << adecousCount << " adecous count in " << n << '\n';
    std::cout << duration << " " << (double)duration / n << "\n";

    return 0;
}