//
// Created by Keegan Millard on 2019-05-12.
//

#include "Solver.h"

#include <limits>
#include <algorithm>
#include <thread>

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

bool MemoContainer::operator<(const MemoContainer &m2) const {
    return memo.operator<(m2.memo);
}

MemoContainer::MemoContainer(const Memo &memo, int count, int first) : memo(memo), count(count), first(first) {}

bool MemoContainer::operator==(const MemoContainer &m2) const {
    return memo == m2.memo;
}

void Solver::initModToCheck()
{

    for (int i = 0; i < memos.size(); i++)
    {
        MemoContainer cont(memos[i], 1, i);

        auto res = std::find(toCheck.begin(), toCheck.end(), cont);
        if (res == toCheck.end())
        {
            toCheck.push_back(cont);
        } else {
            MemoContainer &memo = const_cast<MemoContainer&>(*res);

            memo.count += 1;
        }
    }
}

Solver::Solver(int k_, int cacheSize_): k(k_), cacheSize(cacheSize_) {
    initCache();
    initMemos();
    initModToCheck();
}

Solver::Solver(): Solver(defaultK, defaultCacheSize) {}

uint64_t Solver::getCountForA(uint64_t a) const
{
    uint64_t count = 0;
    for (const MemoContainer& cont : toCheck)
    {
        uint64_t x = (a << k) | cont.first;
        bool res = isAdecous(x);
        if(res) {
            count += cont.count;
        }
    }
    return count;
}

Solver::Solver(int k_): Solver(k_, defaultCacheSize) {}

uint64_t Solver::getCount(uint64_t lb, uint64_t ub) const
{
    uint64_t aLow = lb >> k;
    if (lb & bMask != 0)
        aLow += 1;

    if (aLow < 10)
        aLow = 10;

    uint64_t aHigh = ub >> k;

    if(aHigh <= aLow)
    {
        return count(lb, ub);
    }

    uint64_t count = 0;
    for(auto a = aLow; a < aHigh; ++a)
    {
        count += getCountForA(a);
    }

    auto bound = aLow << k;
    for (int x = lb; x < bound; ++x)
    {
        count += isAdecous(x);
    }

    auto start = aHigh << k;
    for (int x = start; x < ub; ++x)
    {
        count += isAdecous(x);
    }

    return count;
}

struct Task2 {
    const Solver &solver;
    const uint64_t alb;
    const uint64_t aub;
    const int inc;
    std::atomic_llong &counter;

    Task2(const Solver &solver, const uint64_t start, const uint64_t ub, const int inc, std::atomic_llong &counter)
            : solver(solver), alb(start), aub(ub), inc(inc), counter(counter) {}

    void operator()() {
        uint64_t count = 0;
        for (uint64_t a = alb; a < aub; a += inc) {
            count += solver.getCountForA(a);
        }
        counter += count;
    }
};


uint64_t Solver::getCountThreaded(uint64_t lb, uint64_t ub, int tc) const
{
    uint64_t aLow = lb >> k;
    if (lb & bMask != 0)
        aLow += 1;

    if (aLow < 10)
        aLow = 10;

    uint64_t aHigh = ub >> k;

    if(aHigh <= aLow)
    {
        return count(lb, ub);
    }

    uint64_t count = 0;

    std::atomic_llong counter(0);
    std::thread *threads[tc];
    for (int tn = 0; tn < tc; ++tn)
    {
        std::thread *threadP =
                new std::thread(Task2(*this, aLow+tn, aHigh, tc, counter));
        threads[tn] = threadP;
    }

    auto bound = aLow << k;
    for (auto x = lb; x < bound; ++x)
    {
        count += isAdecous(x);
    }

    auto start = aHigh << k;
    for (auto x = start; x < ub; ++x)
    {
        count += isAdecous(x);
    }

    for (auto thread : threads)
    {
        thread->join();
        delete thread;
    }

    return counter + count;
}

uint64_t Solver::count(uint64_t lb, uint64_t ub) const
{
    uint64_t count = 0;
    for(auto x = lb; x < ub; ++x)
    {
        count += isAdecous(x);
    }
    return count;
}





