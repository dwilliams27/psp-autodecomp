// eMaterial.cpp — decompiled from eAll_psp.obj
// Parent: cObject. Vtable at 0x37FDF8 (eMaterialvirtualtable).

class cBase;
class cFile;
class cMemPool;
class cType;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
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

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
extern "C" void cHandleT_eTexture_ctor(void *);  // 0x001E128C: cHandleT<eTexture>::cHandleT(void)

extern char eMaterialvirtualtable[];

class eMaterial : public cObject {
public:
    char _padA[0x44];      // cObject base + padding through offset 0x43
    int mField44;          // 0x44
    int mTextures[2];      // 0x48 (cHandleT<eTexture>[2])
    int mField50;          // 0x50
    int mField54;          // 0x54
    int mField58;          // 0x58

    eMaterial(cBase *);
    ~eMaterial(void);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void PlatformFree(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040FEC;

// ── eMaterial::Write @ 0x0002BF4C ──
void eMaterial::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.Write(mField44);
    wb.End();
}

// ── eMaterial::eMaterial @ 0x0002C074 ──
eMaterial::eMaterial(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eMaterialvirtualtable;
    mField44 = 0;
    __vec_new(mTextures, 2, 4, cHandleT_eTexture_ctor);
    mField50 = 0;
    mField54 = 0;
    mField58 = 0;
}

// ── eMaterial::~eMaterial @ 0x0002C0D0 ──
// SNC auto-generates: (this != 0) guard, ~cObject(0) base call, and the
// `if (flags & 1) operator delete(this)` deleting tail (inlined).
eMaterial::~eMaterial(void) {
    *(void **)((char *)this + 4) = eMaterialvirtualtable;
    PlatformFree();
}

const cType *eMaterial::GetType(void) const {
    if (D_00040FEC == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))&cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00040FEC = cType::InitializeType(0, 0, 0x10, D_000385E4,
                                           0, (const char *)0x36CDCC,
                                           (const char *)0x36CDD8, 5);
    }
    return D_00040FEC;
}
