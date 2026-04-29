// eVirtualTexture.cpp — decompiled from eAll_psp.obj
// Parent: eTexture. Vtable at 0x382958 (eVirtualTexturevirtualtable).
//
// Three functions (transition-zone, all sched=1 per prologue clustering):
//   - Write(cFile &) const          @ 0x000604dc — writes via parent + cWriteBlock + cHandle
//   - eVirtualTexture(cBase *)      @ 0x00060610 — constructor, sets vtable + flags
//   - ~eVirtualTexture(void)        @ 0x00060654 — deleting destructor

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

#pragma control sched=1

class eTexture {
public:
    char _pad[0x4C];
    eTexture(cBase *);
    ~eTexture();
    void Write(cFile &) const;
};

class eVirtualTexture : public eTexture {
public:
    int mHandle4C;     // cHandle at offset 0x4C

    eVirtualTexture(cBase *);
    ~eVirtualTexture();
    const cType *GetType(void) const;
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eVirtualTexturevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FE8;
extern cType *D_00046B98;

// ── eVirtualTexture::Write @ 0x000604dc ──
void eVirtualTexture::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eTexture::Write(file);
    ((const cHandle *)&mHandle4C)->Write(wb);
    wb.End();
}

// ── eVirtualTexture::eVirtualTexture @ 0x00060610 ──
eVirtualTexture::eVirtualTexture(cBase *parent) : eTexture(parent) {
    *(void **)((char *)this + 4) = eVirtualTexturevirtualtable;
    mHandle4C = 0;
    __asm__ volatile("" ::: "memory");
    unsigned char f = *((unsigned char *)this + 0x47);
    *((unsigned char *)this + 0x47) = (unsigned char)(f | 1);
}

// ── eVirtualTexture::GetType @ 0x00206cc8 ──
const cType *eVirtualTexture::GetType(void) const {
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
            __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
            D_00040FE8 = cType::InitializeType(0, 0, 0xA, parentType,
                                               0, kindName, kindDesc, 5);
        }
        D_00046B98 = cType::InitializeType(0, 0, 0x135, D_00040FE8,
                                           0, 0, 0, 0);
    }
    return D_00046B98;
}

// ── eVirtualTexture::~eVirtualTexture @ 0x00060654 ──
// SNC auto-generates: (this != 0) guard, ~eTexture(0) base call, and the
// `if (flags & 1) operator delete(this)` deleting tail (inlined).
eVirtualTexture::~eVirtualTexture(void) {
    *(void **)((char *)this + 4) = eVirtualTexturevirtualtable;
}

#pragma control sched=2
