
#include <iostream>
#include <stdint-gcc.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <limits>

#include "StepMemos.h"

StepMemos stepMemos;
std::atomic_llong overflows(0);
uint64_t overflowLimit = 100000000;

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

bool overFlows(uint64_t a, uint64_t c, uint64_t d)
{
    uint64_t cmax = std::numeric_limits<uint64_t>().max() / a;
    if(c > cmax) {
        return true;
    }

    uint64_t part = a * c;
    uint64_t partMax = std::numeric_limits<uint64_t>().max() - d;
    return part > partMax;
}

uint64_t getNextUnderThreshold(__int128 num) {
    do {
        uint64_t bMask = stepMemos.bMasks[0];
        uint32_t steps = stepMemos.kVals[0];

        uint64_t a = (num & ~bMask) >> steps;
        uint32_t b = num & bMask;

        Memo &memo = stepMemos.memos[0][b];
        num = a * memo.c + memo.d;

    } while (num >= overflowLimit);

    return num;
}

uint64_t getNextOverFlowCheck(int stepIdx, uint64_t num)
{
    uint64_t bMask = stepMemos.bMasks[stepIdx];
    uint32_t steps = stepMemos.kVals[stepIdx];

    uint64_t a = (num & ~bMask) >> steps;
    uint32_t b = num & bMask;

    Memo &memo = stepMemos.memos[stepIdx][b];
    if(overFlows(a, memo.c, memo.d)){
        overflows++;
        __int128 next = (__int128) a * memo.c + memo.d;
        return getNextUnderThreshold(next);
    }

    return  a * memo.c + memo.d;
}

uint64_t getNext(int stepIdx, uint64_t num)
{
    uint64_t bMask = stepMemos.bMasks[stepIdx];
    uint32_t steps = stepMemos.kVals[stepIdx];

    uint64_t a = (num & ~bMask) >> steps;
    uint32_t b = num & bMask;

    Memo &memo = stepMemos.memos[stepIdx][b];
    return  a * memo.c + memo.d;
}

int isAdecous(uint64_t num)
{
    uint64_t cur = num;
    while (true)
    {
        if (cur >> stepMemos.kVals[0] >= 10) {
            if (cur < overflowLimit) {
                cur = getNext(0, cur);
            } else {
                cur = getNextOverFlowCheck(0, cur);
            }

        } else if (cur >> stepMemos.kVals[1] >= 10) {
            cur = getNext(1, cur);

        } else if (cur >> stepMemos.kVals[2] >= 10) {
            cur = getNext(2, cur);

        } else {
            return isAdecousSingleStep(cur);

        }
    }
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

uint64_t getAudecousCount(uint64_t ub)
{
    uint64_t count = 0;
    for (uint64_t x = 1; x < ub; ++x) {
        count += isAdecous(x);
    }
    return count;
}

using namespace std::chrono;

int main(int argc, char**argv)
{
    uint64_t n = 1000000000;

    auto start = high_resolution_clock::now();

//    uint64_t adecousCount = getAudecousCountCmp(n);

//    uint64_t adecousCount = getAudecousCount(n);
    uint64_t adecousCount = getAudecousCountThreaded(16, n);

    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start).count();

    std::cout << adecousCount << " adecous count under " << n << "\n";

    double msPerMillion = duration / 1000 / (n / 1000000);

    std::cout << (double) duration / 1000000 << " seconds " << msPerMillion << " ms per million \n";
    std::cout << overflows << " overflows" << '\n';

    return 0;
}