#include "cThread.h"

void cThread::OnCompleted(void) {
}

void cMemAllocator::Reset(void) {
}

void cMemAllocator::LogAllocations(void) const {
}

int cMemAllocator::BeginBlock(cMemBlockAllocation *) {
    return 0;
}

void cMemAllocator::EndBlock(cMemBlockAllocation *, cMemBlockAllocation *) {
}
