class cBase;
class cFile;
class cMemPool;
class cType;

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
    cBaseArray &operator=(const cBaseArray &);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcState {
public:
    char _pad0[0x20];
    unsigned int mField20;
    cHandle mField24;
    unsigned int mField28;
    cBaseArray mField2C;

    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void gcState__gcState_cBaseptr(void *self, cBase *parent);
    void *dcastdcast_gcStateptr__constcBaseptr(const cBase *);
}

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A3DC;

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
        gcState__gcState_cBaseptr(obj, parent);
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
