//
// Created by millardk on 5/11/19.
//

#include "StepMemos.h"

Memo* createMemoArray(int steps)
{
    int count = 1 << steps;
    auto memos = new Memo[count];

    for (int i = 0; i < count; ++i)
    {
        memos[i] = Memo::makeMemo(steps, i);
    }
    return memos;
}

StepMemos::StepMemos()
{
    kVals[0] = 20;
    kVals[1] = 17;
    kVals[2] = 7;

    for (int i = 0; i < 3; i++)
    {
        memos[i] = createMemoArray(kVals[i]);
        bMasks[i] = (1 << kVals[i]) - 1;
    }
}

StepMemos::~StepMemos()
{
    for(int i = 0; i < 3; i++)
    {
        delete memos[i];
    }
}