#ifndef EFLUIDVOLUME_H
#define EFLUIDVOLUME_H

class cBase;
class cFile;
class cType;
class cReadBlock;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eFluidVolume {
public:
    cBase *mOwner;
    void *mClassDesc;
    cBase *mRef;
    float mField_C;
    float mField_10;

    eFluidVolume(cBase *);
    ~eFluidVolume();
    void Write(cFile &) const;
    void ReadOld(cReadBlock &);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    struct _DelRec { short offset; short pad; void (*fn)(void *, void *); };
    struct _PoolBlk { char pad[0x1C]; char *allocTable; };
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        _DelRec *rec = (_DelRec *)(((_PoolBlk *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

#endif
