#ifndef CTHREAD_H
#define CTHREAD_H

class cMemBlockAllocation;

class cThread {
public:
    void OnCompleted(void);
};

class cMemAllocator {
public:
    void Reset(void);
    void LogAllocations(void) const;
    int BeginBlock(cMemBlockAllocation *);
    void EndBlock(cMemBlockAllocation *, cMemBlockAllocation *);
};

#endif
