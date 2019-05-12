//
// Created by millardk on 5/11/19.
//

#ifndef AUDECOUS_MEMO_H
#define AUDECOUS_MEMO_H

#include <cstdint>

struct Memo {
    uint64_t c;
    uint64_t d;
    static Memo makeMemo(int k, int num);
};


#endif //AUDECOUS_MEMO_H
