//
// Created by Keegan Millard on 2019-05-12.
//

#ifndef AUDECOUS_SOLVER_H
#define AUDECOUS_SOLVER_H

#include "Memo.h"
#include <vector>


bool overFlows(uint64_t a, uint64_t c, uint64_t d);
int isAdecousSingleStep(uint64_t num);

class Solver {
private:
    static const uint64_t overflowLimit = 10000000000;
    static const int cacheSize = 4096*8*1000;
    static const int k = 21;
    static const uint64_t bMask = (1 << k) - 1;

    std::vector<Memo> memos;
    std::vector<bool> cache;

    void initMemos();
    void initCache();

    uint64_t getNextUnderThreshold(__int128 num) const;
    uint64_t getNextOverFlowCheck(uint64_t num) const;
    uint64_t getNext(uint64_t num) const;

public:
    Solver();
    int isAdecous(uint64_t num) const;
};


#endif //AUDECOUS_SOLVER_H
