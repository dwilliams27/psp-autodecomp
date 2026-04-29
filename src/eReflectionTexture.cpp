// eReflectionTexture — sched=1 zone (eAll_psp.obj unity build)
//
// Functions:
//   0x00084eb8  eReflectionTexture::~eReflectionTexture(void)
//   0x00219da8  eReflectionTexture::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class cType;

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

template <class T> T *dcast(const cBase *);

typedef int v4sf_t __attribute__((mode(V4SF)));

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

#pragma control sched=1

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
    ~eVirtualTexture();
    void Write(cFile &) const;
};

class eReflectionTexture : public eVirtualTexture {
public:
    eReflectionTexture(cBase *);
    ~eReflectionTexture();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FE8;
extern cType *D_00046B98;
extern cType *D_00046C90;

// ── eReflectionTexture::eReflectionTexture(cBase *) @ 0x00084e50 ──
eReflectionTexture::eReflectionTexture(cBase *parent) : eVirtualTexture(parent) {
    *(void **)((char *)this + 4) = (void *)0x3857C8;
    __asm__ volatile(
        "mtc1 $zero, $f12\n"
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "lui $a2, 0x3f80\n"
        "mtc1 $a2, $f13\n"
        "mfc1 $a2, $f13\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0x50($s0)\n"
        "swc1 $f12, 0x60($s0)"
        ::: "a0", "a1", "a2", "memory");
    *(int *)((char *)this + 0x64) = 0xFF000000;
}

// ── eReflectionTexture::Write(cFile &) const @ 0x00084cc4 ──
void eReflectionTexture::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eVirtualTexture::Write(file);
    wb.Write(3, (const float *)((const char *)this + 0x50));
    wb.Write(*(const float *)((const char *)this + 0x60));
    wb.Write(*(const unsigned int *)((const char *)this + 0x64));
    wb.End();
}

// ── eReflectionTexture::~eReflectionTexture(void) @ 0x00084eb8 ──
eReflectionTexture::~eReflectionTexture() {
    *(void **)((char *)this + 4) = (void *)0x3857C8;
}

// ── eReflectionTexture::AssignCopy(const cBase *) @ 0x00219d08 ──
void eReflectionTexture::AssignCopy(const cBase *src) {
    eReflectionTexture *other = dcast<eReflectionTexture>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(signed char *)((char *)this + 0x44) = *(const signed char *)((char *)other + 0x44);
    *(signed char *)((char *)this + 0x45) = *(const signed char *)((char *)other + 0x45);
    *(signed char *)((char *)this + 0x46) = *(const signed char *)((char *)other + 0x46);
    *(unsigned char *)((char *)this + 0x47) = *(const unsigned char *)((char *)other + 0x47);
    *(short *)((char *)this + 0x48) = *(const short *)((char *)other + 0x48);
    *(short *)((char *)this + 0x4A) = *(const short *)((char *)other + 0x4A);
    __asm__ volatile("" ::: "memory");
    int *dst4C = (int *)((char *)this + 0x4C);
    int *src4C = (int *)((char *)other + 0x4C);
    *dst4C = *src4C;
    *(v4sf_t *)((char *)this + 0x50) = *(const v4sf_t *)((char *)other + 0x50);
    *(float *)((char *)this + 0x60) = *(const float *)((char *)other + 0x60);
    int *dst64 = (int *)((char *)this + 0x64);
    int *src64 = (int *)((char *)other + 0x64);
    *dst64 = *src64;
}

// ── eReflectionTexture::GetType(void) const @ 0x00219e24 ──
const cType *eReflectionTexture::GetType(void) const {
    if (D_00046C90 == 0) {
        if (D_00046B98 == 0) {
            if (D_00040FE8 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(
                                name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                       0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                const char *kindName = (const char *)0x36CDA8;
                const char *kindDesc = (const char *)0x36CDB4;
                __asm__ volatile("" : "+r"(parentType), "+r"(kindName),
                                 "+r"(kindDesc));
                D_00040FE8 = cType::InitializeType(0, 0, 0xA, parentType,
                                                   0, kindName, kindDesc, 5);
            }
            D_00046B98 = cType::InitializeType(0, 0, 0x135, D_00040FE8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00046B98;
        cBase *(*factory)(cMemPool *, cBase *) = &eReflectionTexture::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046C90 = cType::InitializeType(0, 0, 0x120, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046C90;
}

// ── eReflectionTexture::New(cMemPool *, cBase *) static @ 0x00219da8 ──
cBase *eReflectionTexture::New(cMemPool *pool, cBase *parent) {
    eReflectionTexture *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eReflectionTexture *obj = (eReflectionTexture *)rec->fn(base, 0x70, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) eReflectionTexture(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2
