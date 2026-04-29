// eSoundDataMap.cpp — decompiled from eAll_psp.obj
// eSoundDataMap inherits from eSoundData. Vtable at 0x37FA60.

class cBase;
class cFile;
class cMemPool;
class cObject {
public:
    cObject &operator=(const cObject &);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern "C" void eSoundData_eSoundData(void *, cBase *);

class eSoundData {
public:
    char _pad[0x68];
    ~eSoundData();
    void Write(cFile &) const;
};

class eSoundDataMap : public eSoundData {
public:
    eSoundDataMap(cBase *);
    ~eSoundDataMap();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static eSoundDataMap *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char eSoundDataMapvirtualtable[];

template <class T> T *dcast(const cBase *);

// ── eSoundDataMap::AssignCopy(const cBase *) @ 0x001dfe8c ──
void eSoundDataMap::AssignCopy(const cBase *src) {
    eSoundDataMap *other = dcast<eSoundDataMap>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(int *)((char *)this + 0x44) = *(int *)((char *)other + 0x44);
    *(int *)((char *)this + 0x48) = *(int *)((char *)other + 0x48);
    *(float *)((char *)this + 0x4C) = *(float *)((char *)other + 0x4C);
    *(unsigned char *)((char *)this + 0x50) = *(unsigned char *)((char *)other + 0x50);
    *(unsigned char *)((char *)this + 0x51) = *(unsigned char *)((char *)other + 0x51);
    *(unsigned char *)((char *)this + 0x52) = *(unsigned char *)((char *)other + 0x52);
    *(float *)((char *)this + 0x54) = *(float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)other + 0x58);
    *(unsigned char *)((char *)this + 0x5C) = *(unsigned char *)((char *)other + 0x5C);
    *(int *)((char *)this + 0x60) = *(int *)((char *)other + 0x60);
    int *src64 = (int *)((char *)other + 0x64);
    int value64 = *src64;
    int *dst64 = (int *)((char *)this + 0x64);
    *dst64 = value64;
}

// ── eSoundDataMap::Write(cFile &) const @ 0x000210a8 ──
void eSoundDataMap::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eSoundData::Write(file);
    wb.End();
}

// ── eSoundDataMap::~eSoundDataMap(void) @ 0x001e012c ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the ~eSoundData() base-chain call (with flags=0), and the deleting-tail
// `if (flags & 1) operator delete(this)` epilogue.
eSoundDataMap::~eSoundDataMap() {
    *(void **)((char *)this + 4) = eSoundDataMapvirtualtable;
}

// ── eSoundDataMap::New(cMemPool *, cBase *) static @ 0x001dff2c ──
eSoundDataMap *eSoundDataMap::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSoundDataMap *result = 0;
    eSoundDataMap *obj = (eSoundDataMap *)e->fn(base, 0x68, 4, 0, 0);
    if (obj != 0) {
        eSoundData_eSoundData(obj, parent);
        *(void **)((char *)obj + 4) = eSoundDataMapvirtualtable;
        result = obj;
    }
    return result;
}
