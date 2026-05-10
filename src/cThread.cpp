#include "thread.h"

extern "C" {
    void free(void *);
}

extern char cThreadvirtualtable[];

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

static int s_primaryInit;
static int s_primaryThreadId;

struct VTableEntry {
    short adj;
    short pad;
    void (*fn)(void *, ...);
};

class cMemBlockAllocation;

class cThread {
public:
    int m_threadId;
    int m_flags;
    void *m_vtable;

    ~cThread(void);
    void Start(void);
    void WaitForFinish(void);
    static bool InPrimary(void);
    void OnCompleted(void);
    static int ThreadFunc(unsigned int argSize, void *argBlock);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            VTableEntry *rec = (VTableEntry *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->adj;
            ((void (*)(void *, void *))rec->fn)((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

class cMemAllocator {
public:
    ~cMemAllocator(void);
    void Reset(void);
    void LogAllocations(void) const;
    int BeginBlock(cMemBlockAllocation *);
    void EndBlock(cMemBlockAllocation *, cMemBlockAllocation *);
    void StopBlock(bool);
    static void operator delete(void *);
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
    __asm__ volatile("" ::: "memory");
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

cThread::~cThread(void) {
    m_vtable = cThreadvirtualtable;
    __asm__ volatile("" ::: "memory");
    int running = (((m_flags & 0x1000) != 0) & 0xFF);
    if (running != 0) {
        if (m_threadId > 0) {
            WaitForFinish();
            sceKernelTerminateDeleteThread(m_threadId);
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
