
#include <iostream>
#include <stdint-gcc.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <limits>
#include <vector>

#include "StepMemos.h"

StepMemos stepMemos;
uint64_t overflowLimit = 100000000;

const int cacheSize = 1000000;
std::vector<bool> cache;

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
    uint64_t cmax = std::numeric_limits<uint64_t>::max() / a;
    if(c > cmax) {
        return true;
    }

    uint64_t part = a * c;
    uint64_t partMax = std::numeric_limits<uint64_t>::max() - d;
    return part > partMax;
}

uint64_t getNextUnderThreshold(__int128 num) {
    do {
        __int128 bMask = stepMemos.bMasks[0];
        uint32_t steps = stepMemos.kVals[0];

        __int128 a = (num & ~bMask) >> steps;
        uint32_t b = num & bMask;

        Memo &memo = stepMemos.memos[0][b];
        num = a * memo.c + memo.d;

    } while (num >= overflowLimit);

    return num;
}

uint64_t getNextOverFlowCheck(int stepIdx, uint64_t num, uint64_t numorig)
{
    uint64_t bMask = stepMemos.bMasks[stepIdx];
    uint32_t steps = stepMemos.kVals[stepIdx];

    uint64_t a = (num & ~bMask) >> steps;
    uint32_t b = num & bMask;

    Memo &memo = stepMemos.memos[stepIdx][b];
    if(overFlows(a, memo.c, memo.d)){
        std::cout << numorig << " " << a << " " << memo.c << " " << memo.d << std::endl;
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
        if(cur < cacheSize)
            return cache[cur] ? 1 : 0;

        if (cur >> stepMemos.kVals[0] >= 10) {
            if (cur < overflowLimit) {
                cur = getNext(0, cur);
            } else {
                cur = getNextOverFlowCheck(0, cur, num);
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
//        if (x % 1000000000 == 0)
//            std::cout << x / 1000000000 << " B" << std::endl;

        count += isAdecous(x);
    }
    *counter += count;
}

uint64_t initCache() {
    cache.push_back(false);

    uint64_t count = 0;
    for(int i = 1; i < cacheSize; i++)
    {
        int result = isAdecousSingleStep(i);
        cache.push_back(result);
        count += result;
    }
    return count;
}


uint64_t getAudecousCountThreaded(int tc, uint64_t ub)
{
    std::thread *threads[tc];
    std::atomic_llong counter(initCache());

    for (int tn = 0; tn < tc; ++tn)
    {
        std::thread *threadP = new std::thread(
                adecousCountTask,
                &counter,
                cacheSize + tn,
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
    uint64_t count = initCache();
    for (uint64_t x = cacheSize; x < ub; ++x) {
        count += isAdecous(x);
    }
    return count;
}

using namespace std::chrono;

int main(int argc, char**argv)
{
    uint64_t n = 1000000000;

    auto start = high_resolution_clock::now();

    uint64_t adecousCount = getAudecousCountThreaded(16, n);

    auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start).count();

    std::cout << adecousCount << " adecous count under " << n << "\n";
    std::cout << (double) duration / 1000000 << "seconds  " << (double)duration * 1000 / n << "\n";

    return 0;
}