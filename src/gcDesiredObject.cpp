// gcDesiredObject.cpp — decompiled from gcAll_psp.obj.
//
// Functions in this file:
//   0x0011b578 gcDesiredObject::Write(cFile &) const             100B
//   0x0025f28c gcDesiredObject::GetType(void) const              152B
//   0x0025f324 gcDesiredObject::GetName(char *) const            144B

class cBase;
class cFile;
class cType;

extern "C" int cStrAppend(char *, const char *, ...);
extern "C" void free(void *);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern cType *D_000385DC;   // shared cBase root type cache
extern cType *D_0009F3F4;   // gcDesiredObject type cache
extern char gcDesiredObjectvirtualtable[];
extern char cBaseclassdesc[];

struct gcDesiredObjectCastSlot {
    short mOffset;                              // +0
    short _pad;                                 // +2
    cBase *(*mCast)(cBase *);                   // +4
};

struct gcDesiredObjectGetNameSlot {
    short mOffset;                              // +0
    short _pad;                                 // +2
    void (*mGetName)(cBase *, char *);          // +4
};

struct gcDesiredObjectTypeInfo {
    char _pad[0x78];                            // +0x00
    gcDesiredObjectGetNameSlot mInner;          // +0x78 — parent GetName slot
    gcDesiredObjectCastSlot    mOwner;          // +0x80 — named-object cast slot
};

struct cBaseWithName {
    char _pad[12];                              // +0x00
    const char *mName;                          // +0x0C
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDesiredObject {
public:
    cBase *mParent;                             // +0x00
    gcDesiredObjectTypeInfo *mTypeInfo;         // +0x04
    int mField8;                                // +0x08

    ~gcDesiredObject(void);
    static void operator delete(void *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
    void GetName(char *) const;
};

inline void gcDesiredObject::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    } else {
        free(p);
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcDesiredObjectdtv, 0xf0\n");

// 0x0025f67c - gcDesiredObject::~gcDesiredObject(void)
gcDesiredObject::~gcDesiredObject(void) {
    *(char **)((char *)this + 4) = gcDesiredObjectvirtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}

// ── gcDesiredObject::Read @ 0x0011b5dc ──
int gcDesiredObject::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int val = mField8;
    int tag = val & 1;

    int flag1 = 0;
    if (tag != 0) flag1 = 1;

    cBase *outVal;
    if (flag1 != 0) {
        outVal = 0;
    } else {
        outVal = (cBase *)val;
    }
    __asm__ volatile("" : "+r"(outVal));
    cBase *sp14 = outVal;

    int flag2 = 0;
    if (tag != 0) flag2 = 1;

    int base;
    if (flag2 != 0) {
        base = val & ~1;
    } else {
        base = *(int *)val;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr(&mField8), (cBase *)base, sp14);

    int newVal;
    if (sp14 == 0) {
        newVal = base | 1;
    } else {
        newVal = (int)sp14;
    }
    mField8 = newVal;
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ── gcDesiredObject::Write @ 0x0011b578 ──
void gcDesiredObject::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    int val = mField8;
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

// ── gcDesiredObject::GetType @ 0x0025f28c ──
const cType *gcDesiredObject::GetType(void) const {
    if (D_0009F3F4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC, 0, 0, 0, 0);
    }
    return D_0009F3F4;
}

// ── gcDesiredObject::GetName @ 0x0025f324 ──
void gcDesiredObject::GetName(char *dst) const {
    gcDesiredObjectCastSlot *owner = &mTypeInfo->mOwner;
    const char *fmt = (const char *)0x36DCE4;
    cBase *obj = owner->mCast((cBase *)((char *)this + owner->mOffset));
    cStrAppend(dst, fmt, ((cBaseWithName *)obj)->mName);

    gcDesiredObjectGetNameSlot *inner = &mTypeInfo->mInner;
    inner->mGetName((cBase *)((char *)this + inner->mOffset), dst);

    cStrAppend(dst, (const char *)0x36DCEC);
}
