// eMaterial.cpp — decompiled from eAll_psp.obj
// Parent: cObject. Vtable at 0x37FDF8 (eMaterialvirtualtable).

class cBase;
class cFile;
class cMemPool;

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
    void Write(cFile &) const;
    void PlatformFree(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

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
