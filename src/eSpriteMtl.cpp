// eSpriteMtl.cpp — decompiled from eAll_psp.obj.
// Parent: eMaterial. Vtable at 0x380358 (eSpriteMtlvirtualtable).
//
// Functions:
//   0x00031944 eSpriteMtl::Write(cFile &) const             100B
//   0x00031A8C eSpriteMtl::eSpriteMtl(cBase *)               68B
//   0x001E3E6C eSpriteMtl::~eSpriteMtl(void)                124B  (deleting destructor auto-gen)

class cBase;
class cFile;
class cMemPool;
class cType;
struct cFileHandle;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eMaterial {
public:
    char _pad[0x5C];
    eMaterial(cBase *);
    ~eMaterial(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eSpriteMtl : public eMaterial {
public:
    unsigned int mField5C;  // 0x5C
    unsigned int mField60;  // 0x60

    eSpriteMtl(cBase *);
    ~eSpriteMtl(void);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char eSpriteMtlvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;
extern cType *D_00041038;

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// ── eSpriteMtl::Write @ 0x00031944 ──
void eSpriteMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eMaterial::Write(file);
    wb.Write(mField5C);
    wb.Write(mField60);
    wb.End();
}

#pragma control sched=1

// ── eSpriteMtl::Read @ 0x000319A8 ──
int eSpriteMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    int rb[5];
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] == 2 && eMaterial::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x5C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x60, 4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

#pragma control sched=2

// ── eSpriteMtl::eSpriteMtl @ 0x00031A8C ──
eSpriteMtl::eSpriteMtl(cBase *parent) : eMaterial(parent) {
    *(void **)((char *)this + 4) = eSpriteMtlvirtualtable;
    mField5C = 0x18;
    mField60 = (unsigned int)-1;
}

// ── eSpriteMtl::~eSpriteMtl @ 0x001E3E6C ──
// SNC auto-generates: (this != 0) guard, ~eMaterial(0) base call, and
// `if (flags & 1) operator delete(this)` deleting tail (inlined).
eSpriteMtl::~eSpriteMtl(void) {
    *(void **)((char *)this + 4) = eSpriteMtlvirtualtable;
}

// ── eSpriteMtl::GetType @ 0x001E3D0C ──
const cType *eSpriteMtl::GetType(void) const {
    if (D_00041038 == 0) {
        if (D_00040FEC == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36CD74, (const char *)0x36CD7C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        &cNamed::New,
                        0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_00040FEC = cType::InitializeType(
                0, 0, 0x10, D_000385E4, 0,
                (const char *)0x36CDCC, (const char *)0x36CDD8, 5);
        }
        D_00041038 = cType::InitializeType(0, 0, 0x3F, D_00040FEC,
                                           0, 0, 0, 0);
    }
    return D_00041038;
}
