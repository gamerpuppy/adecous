//
// Created by Keegan Millard on 2019-05-12.
//

#ifndef AUDECOUS_SOLVER_H
#define AUDECOUS_SOLVER_H

#include "Memo.h"
#include <vector>
#include <set>

bool overFlows(uint64_t a, uint64_t c, uint64_t d);
int isAdecousSingleStep(uint64_t num);

struct MemoContainer {
    const Memo memo;
    int count;
    int first;

    MemoContainer(const Memo &memo, int count, int first);
    MemoContainer(const MemoContainer&) = default;

    bool operator<(const MemoContainer &m2) const;
    bool operator==(const MemoContainer &m2) const;
};

class Solver {
public:
    static const int defaultCacheSize = 4096*8*200;
    static const int defaultK = 21;

    static const uint64_t overflowLimit = 10000000000;

    const int cacheSize;
    const int k = 15;
    const uint64_t bMask = (1 << k) - 1;

    std::vector<Memo> memos;
    std::vector<bool> cache;

    std::vector<MemoContainer> toCheck;

    void initMemos();
    void initCache();
    void initModToCheck();

    uint64_t getNextUnderThreshold(__int128 num) const;
    uint64_t getNextOverFlowCheck(uint64_t num) const;
    uint64_t getNext(uint64_t num) const;

    Solver();
    Solver(int k);
    Solver(int k, int cacheSize);
    int isAdecous(uint64_t num) const;

    uint64_t getCountForA(uint64_t a) const;
    uint64_t getCount(uint64_t lb, uint64_t ub) const;

    uint64_t getCountThreaded(uint64_t lb, uint64_t ub, int tc) const;


    uint64_t count(uint64_t lb, uint64_t ub) const;

};


#endif //AUDECOUS_SOLVER_H
