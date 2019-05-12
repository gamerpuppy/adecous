//
// Created by millardk on 5/11/19.
//

#ifndef AUDECOUS_STEPMEMOS_H
#define AUDECOUS_STEPMEMOS_H

#include <stdint-gcc.h>
#include "Memo.h"

struct StepMemos {
    int kVals[3];
    uint64_t bMasks[3];
    Memo *memos[3];

    StepMemos();
    ~StepMemos();
};


#endif //AUDECOUS_STEPMEMOS_H
