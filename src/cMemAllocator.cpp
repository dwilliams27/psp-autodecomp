#include "cThread.h"
#include "cFastMemAllocator.h"
#include "cFile.h"

class cDynamicMemAllocator {
public:
    int GetUsedSize(void) const;
};

class cNamed {
public:
    void OnNameChanged(void);
};

void cMemAllocator::StopBlock(bool) {
}

void cFastMemAllocator::Free(void *) {
}

int cDynamicMemAllocator::GetUsedSize(void) const {
    return *(int *)((char *)this + 0x1AC);
}

bool cListSubscriber::IsModifiable(void) const {
    return false;
}

void cNamed::OnNameChanged(void) {
}
