// ODR-WARNING: split-TU local redeclaration of cThread for isolated matching of
// cThread::WaitForFinish. Do not link with src/cThread.cpp.
#include "thread.h"

class cThread {
public:
    int m_threadId;
    unsigned int m_flags;
    void *m_vtable;

    void WaitForFinish(void);
    bool IsRunning(void) const { return (m_flags & 0x1000) != 0; }
};

#pragma control sched=1
void cThread::WaitForFinish(void) {
    int slot[3];
    __asm__ volatile("" ::: "memory");
    while (IsRunning()) {
        slot[1] = (0x6F);
        slot[2] = sceKernelGetThreadCurrentPriority();
        slot[0] = sceKernelGetThreadId();
        __asm__ volatile("" ::: "memory");
        sceKernelChangeThreadPriority(slot[0], 0x6F);
        sceKernelRotateThreadReadyQueue(slot[1]);
        sceKernelChangeThreadPriority(slot[0], slot[2]);
    }
}
