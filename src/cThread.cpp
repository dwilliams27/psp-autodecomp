#include "cThread.h"
#include "thread.h"

extern "C" {
    void *cMemPool_GetPoolFromPtr(void *);
    void free(void *);
}

extern char cThreadvirtualtable[];

static int s_primaryInit;
static int s_primaryThreadId;

struct VTableEntry {
    short adj;
    short pad;
    void (*fn)(void *, ...);
};

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

void cThread::Start(void) {
    if (m_threadId > 0) {
        int *p = &m_flags;
        *p |= 0x1000;
        sceKernelWakeupThread(m_threadId);
    }
}

void cThread::WaitForFinish(void) {
    while ((bool)(m_flags & 0x1000)) {
        int prio = sceKernelGetThreadCurrentPriority();
        int tid = sceKernelGetThreadId();
        sceKernelChangeThreadPriority(tid, 0x6F);
        sceKernelRotateThreadReadyQueue(0x6F);
        sceKernelChangeThreadPriority(tid, prio);
    }
}

bool cThread::InPrimary(void) {
    if (!s_primaryInit) {
        s_primaryInit = 1;
        s_primaryThreadId = sceKernelGetThreadId();
    }
    return sceKernelGetThreadId() == s_primaryThreadId;
}

extern "C" void cThread___dtor_cThread_void(cThread *self, int flags) {
    if (self != 0) {
        self->m_vtable = cThreadvirtualtable;
        bool running = (bool)(self->m_flags & 0x1000);
        if (running) {
            if (self->m_threadId > 0) {
                self->WaitForFinish();
                sceKernelTerminateDeleteThread(self->m_threadId);
            }
        }
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                VTableEntry *rec = (VTableEntry *)(*(char **)((char *)block + 0x1C) + 0x30);
                short adj = rec->adj;
                ((void (*)(void *, void *))rec->fn)((char *)block + adj, self);
            } else {
                free(self);
            }
        }
    }
}

int cThread::ThreadFunc(unsigned int argSize, void *argBlock) {
    cThread *self = *(cThread **)argBlock;
    int mask = ~0x1000;
    int *pFlags = &self->m_flags;
    do {
        sceKernelSleepThread();
        {
            VTableEntry *e = (VTableEntry *)((char *)self->m_vtable + 16);
            short adj = e->adj;
            ((void (*)(void *))e->fn)((char *)self + adj);
        }
        *pFlags &= mask;
        {
            VTableEntry *e = (VTableEntry *)((char *)self->m_vtable + 24);
            short adj = e->adj;
            ((void (*)(void *))e->fn)((char *)self + adj);
        }
    } while ((self->m_flags & 1) == 0);

    if (self != 0) {
        VTableEntry *e = (VTableEntry *)((char *)self->m_vtable + 32);
        short adj = e->adj;
        ((void (*)(void *, int))e->fn)((char *)self + adj, 3);
    }
    sceKernelExitDeleteThread(0);
    return 0;
}
