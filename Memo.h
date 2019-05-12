//
// Created by millardk on 5/11/19.
//

#ifndef AUDECOUS_MEMO_H
#define AUDECOUS_MEMO_H

#include <cstdint>

class Memo {
private:
    Memo(uint64_t c_, uint64_t d_);

public:
    const uint64_t c;
    const uint64_t d;
    static Memo makeMemo(int k, int num);
};


#endif //AUDECOUS_MEMO_H
