#include "cFile.h"

cListSubscriber::cListSubscriber(cBase *owner) {
    mOwner = owner;
    mTypeInfo = (void *)0x0037E718;
    mNext = 0;
    mPrev = 0;
    mAttached = 0;
    mFirst = -1;
    mLast = -1;
    mCount = 0;
    mNumVisible = 0;
}

void cListSubscriber::OnCleared(void) {
    mLast = -1;
    mFirst = -1;
    mCount = 0;
}

void cListSubscriber::Attach(void) {
    if (!mAttached) {
        mAttached = 1;
    }
}

void cListSubscriber::Detach(void) {
    if (mAttached) {
        mLast = -1;
        mFirst = -1;
        mCount = 0;
        mAttached = 0;
    }
}

void cListSubscriber::OnAdded(void *item) {
    mCount++;
    if (mFirst < 0) {
        mLast = 0;
        mFirst = 0;
    }
}

void cListSubscriber::SetNumVisible(int numVisible) {
    mNumVisible = numVisible;
}
