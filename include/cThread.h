#ifndef CTHREAD_H
#define CTHREAD_H

class cMemBlockAllocation;

class cThread {
public:
    int m_threadId;    // offset 0
    int m_flags;       // offset 4
    void *m_vtable;    // offset 8

    void Start(void);
    void WaitForFinish(void);
    static bool InPrimary(void);
    void OnCompleted(void);
    static int ThreadFunc(unsigned int argSize, void *argBlock);
};

class cMemAllocator {
public:
    void Reset(void);
    void LogAllocations(void) const;
    int BeginBlock(cMemBlockAllocation *);
    void EndBlock(cMemBlockAllocation *, cMemBlockAllocation *);
    void StopBlock(bool);
};

#endif
