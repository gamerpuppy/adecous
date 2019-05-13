//
// Created by Keegan Millard on 2019-05-12.
//

#include "Solver.h"

#include <limits>

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

uint64_t Solver::getNextUnderThreshold(__int128 num) const
{
    const __int128 wideBMask = bMask;
    do {
        __int128 a = (num & ~wideBMask) >> k;
        uint32_t b = num & bMask;
        const Memo &memo = memos[b];
        num = a * memo.c + memo.d;

    } while (num >= overflowLimit);

    return num;
}

uint64_t Solver::getNextOverFlowCheck(uint64_t num) const
{
    uint64_t a = (num & ~bMask) >> k;
    uint32_t b = num & bMask;

    const Memo &memo = memos[b];
    if(overFlows(a, memo.c, memo.d)){
        __int128 next = (__int128) a * memo.c + memo.d;
        return getNextUnderThreshold(next);
    }

    return  a * memo.c + memo.d;
}

uint64_t Solver::getNext(uint64_t num) const
{
    uint64_t a = (num & ~bMask) >> k;
    uint32_t b = num & bMask;
    const Memo &memo = memos[b];
    return  a * memo.c + memo.d;
}

int Solver::isAdecous(uint64_t num) const
{
    while (true) {
        if (num >> k >= 10) {
            if (num < overflowLimit) {
                num = getNext(num);
            } else {
                num = getNextOverFlowCheck(num);
            }

        } else if (num < cacheSize) {
            return cache[num] ? 1 : 0;

        } else {
            return isAdecousSingleStep(num);
        }
    }
}

void Solver::initCache() {
    cache.push_back(false);
    for(int i = 1; i < cacheSize; i++)
    {
        cache.push_back(isAdecousSingleStep(i));
    }
}

void Solver::initMemos()
{
    int count = 1 << k;
    for (int i = 0; i < count; ++i)
    {
        memos.push_back(Memo::makeMemo(k, i));
    }
}

Solver::Solver(int k_, int cacheSize_): k(k_), cacheSize(cacheSize_) {
    initCache();
    initMemos();
}

Solver::Solver(): Solver(defaultK, defaultCacheSize) {}



