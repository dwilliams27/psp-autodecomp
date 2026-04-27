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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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
};

class eSpriteMtl : public eMaterial {
public:
    unsigned int mField5C;  // 0x5C
    unsigned int mField60;  // 0x60

    eSpriteMtl(cBase *);
    ~eSpriteMtl(void);
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char eSpriteMtlvirtualtable[];

// ── eSpriteMtl::Write @ 0x00031944 ──
void eSpriteMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eMaterial::Write(file);
    wb.Write(mField5C);
    wb.Write(mField60);
    wb.End();
}

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
