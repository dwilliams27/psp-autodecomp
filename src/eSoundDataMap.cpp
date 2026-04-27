// eSoundDataMap.cpp — decompiled from eAll_psp.obj
// eSoundDataMap inherits from eSoundData. Vtable at 0x37FA60.

class cBase;
class cFile;
class cMemPool;

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
