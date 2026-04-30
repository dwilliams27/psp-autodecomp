// gcVoiceOver.cpp — decompiled from gcAll_psp.obj.
// Parent: eSoundData. Vtable at 0x388C40.
//
// Functions:
//   0x001211c4 gcVoiceOver::Write(cFile &) const                100B
//   0x00121414 gcVoiceOver::~gcVoiceOver(void)                  124B
//   0x00262594 gcVoiceOver::New(cMemPool *, cBase *) static     124B
//   (already matched)
//   gcVoiceOver::IsVoiceOver(void) const                          8B

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

class cObject {
public:
    cObject &operator=(const cObject &);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

static inline void copy_int_from_ptr(const int *src, int *dst) {
    *dst = *src;
}

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cMemPool_shim {
public:
    static cMemPool_shim *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class eSoundData {
public:
    char _pad[0x68];
    eSoundData(cBase *);
    ~eSoundData(void);
    void Write(cFile &) const;
};

class gcVoiceOver : public eSoundData {
public:
    cHandle mHandle;        // 0x68
    unsigned int mField6C;  // 0x6C

    gcVoiceOver(cBase *);
    ~gcVoiceOver(void);
    int IsVoiceOver(void) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool_shim *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcVoiceOvervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040F6C;
extern cType *D_0009F404;

// ── gcVoiceOver::IsVoiceOver(void) const ──
int gcVoiceOver::IsVoiceOver(void) const {
    return 1;
}

// ── gcVoiceOver::gcVoiceOver @ 0x001213c0 ──
gcVoiceOver::gcVoiceOver(cBase *parent) : eSoundData(parent) {
    *(void **)((char *)this + 4) = gcVoiceOvervirtualtable;
    mHandle.mIndex = 0;
    mField6C = 0;
    *(float *)((char *)this + 0x54) = 40.0f;
    *(float *)((char *)this + 0x58) = 10.0f;
}

void gcVoiceOver::AssignCopy(const cBase *src) {
    gcVoiceOver *other = dcast<gcVoiceOver>(src);
    ((cObject *)this)->operator=(*(cObject *)other);
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
    int v64 = *src64;
    int *dst64 = (int *)((char *)this + 0x64);
    int *src68 = (int *)((char *)other + 0x68);
    *dst64 = v64;
    int *dst68 = (int *)((char *)this + 0x68);
    int *src6C = (int *)((char *)other + 0x6C);
    *dst68 = *src68;
    __asm__ volatile("" ::: "memory");
    copy_int_from_ptr(src6C, (int *)((char *)this + 0x6C));
}

// ── gcVoiceOver::Write @ 0x001211c4 ──
void gcVoiceOver::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    eSoundData::Write(file);
    mHandle.Write(wb);
    wb.Write(mField6C);
    wb.End();
}

// ── gcVoiceOver::~gcVoiceOver @ 0x00121414 ──
gcVoiceOver::~gcVoiceOver(void) {
    *(void **)((char *)this + 4) = gcVoiceOvervirtualtable;
}

// ── gcVoiceOver::New @ 0x00262594 ──
cBase *gcVoiceOver::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcVoiceOver *result = 0;
    gcVoiceOver *obj = (gcVoiceOver *)rec->fn(base, 0x70, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcVoiceOver(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcVoiceOver::GetType @ 0x00262610 ──
const cType *gcVoiceOver::GetType(void) const {
    if (D_0009F404 == 0) {
        if (D_00040F6C == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                           (const char *)0x36D89C,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                       &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_00040F6C = cType::InitializeType(0, 0, 0x26, D_000385E4,
                                               0, (const char *)0x36D978,
                                               (const char *)0x36D980, 1);
        }
        D_0009F404 = cType::InitializeType(0, 0, 0x162, D_00040F6C,
                                           &gcVoiceOver::New,
                                           (const char *)0x36D984,
                                           (const char *)0x36D990, 1);
    }
    return D_0009F404;
}
