class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cNamed {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
    void RemoveAll(void);
    cBaseArray &operator=(const cBaseArray &);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char _pad[0x1C];
    char *allocTable;
};

struct HandleEntry {
    char _pad[0x30];
    int mId;
};

class gcState {
public:
    char _pad0[0x20];
    unsigned int mField20;
    cHandle mField24;
    unsigned int mField28;
    cBaseArray mField2C;

    gcState(cBase *);
    ~gcState();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        rec->fn(block + off, p);
    }
};

extern "C" {
    void *dcastdcast_gcStateptr__constcBaseptr(const cBase *);
}

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A3DC;
extern void *D_00038890[];

// ── gcState::gcState(cBase *) @ 0x0010ae68 ──
gcState::gcState(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(short *)((char *)this + 0x1C) = 0;
    *(short *)((char *)this + 0x1E) = 0;
    *(char *)((char *)this + 8) = 0;
    *(void **)((char *)this + 4) = (void *)0x3883D0;
    *(int *)((char *)this + 0x20) = 0;

    int parentHandle = *(int *)((char *)parent + 0x44);
    HandleEntry *entry;
    if (parentHandle == 0) {
        entry = 0;
    } else {
        HandleEntry *lookup = (HandleEntry *)D_00038890[parentHandle & 0xFFFF];
        entry = 0;
        if (lookup != 0) {
            if (lookup->mId == parentHandle) {
                entry = lookup;
            }
        }
    }

    __asm__ volatile("" ::: "memory");
    volatile int defaultIndex = 0;
    int *defaultIndexPtr = (int *)&defaultIndex;
    int handle = 0;
    if (entry != 0) {
        handle = entry->mId;
    }
    *(int *)((char *)this + 0x24) = handle;
    int index = *defaultIndexPtr;
    *(int *)((char *)this + 0x2C) = 0;
    *(int *)((char *)this + 0x28) = index;
    __asm__ volatile("" ::: "memory");
    *(gcState **)((char *)this + 0x30) = this;
}

// ── gcState::~gcState(void) @ 0x00259728 ──
gcState::~gcState() {
    *(void **)((char *)this + 4) = (void *)0x3883D0;
    void *array = (char *)this + 0x2C;
    if (array != 0) {
        ((cBaseArray *)array)->RemoveAll();
    }
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// ── gcState::Write(cFile &) const @ 0x0010ac68 ──
void gcState::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cNamed *)this)->Write(file);
    wb.Write(mField20);
    mField24.Write(wb);
    wb.Write(mField28);
    mField2C.Write(wb);
    wb.End();
}

// ── gcState::New(cMemPool *, cBase *) static @ 0x0025908c ──
cBase *gcState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcState *result = 0;
    gcState *obj = (gcState *)rec->fn(base, 0x34, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcState(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcState::AssignCopy(const cBase *) @ 0x00258ff0 ──
void gcState::AssignCopy(const cBase *src) {
    gcState *other = (gcState *)dcastdcast_gcStateptr__constcBaseptr(src);

    int *srcp = (int *)((char *)other + 8);
    int *dst = (int *)((char *)this + 8);
    int x0 = srcp[0];
    int x1 = srcp[1];
    int x2 = srcp[2];
    dst[0] = x0;
    int x3 = srcp[3];
    dst[1] = x1;
    int x4 = srcp[4];
    dst[2] = x2;
    int x5 = srcp[5];
    dst[3] = x3;
    dst[4] = x4;
    dst[5] = x5;

    int *src20 = (int *)((char *)other + 0x20);
    int x20 = *src20;
    int *dst20 = (int *)((char *)this + 0x20);
    int *src24 = (int *)((char *)other + 0x24);
    *dst20 = x20;
    cHandle *dst24 = (cHandle *)((char *)this + 0x24);
    cHandle *srcHandle = (cHandle *)src24;
    int *src28 = (int *)((char *)other + 0x28);
    *dst24 = *srcHandle;
    int *dst28 = (int *)((char *)this + 0x28);
    *dst28 = *src28;
    __asm__ volatile("" ::: "memory");

    ((cBaseArray *)((char *)this + 0x2C))->operator=(
        *(const cBaseArray *)((char *)other + 0x2C));
}

// ── gcState::GetType(void) const @ 0x00259108 ──
const cType *gcState::GetType(void) const {
    if (D_0009A3DC == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_0009A3DC = cType::InitializeType(0, 0, 0xB5, D_000385E0,
                                           &gcState::New, 0, 0, 0);
    }
    return D_0009A3DC;
}
