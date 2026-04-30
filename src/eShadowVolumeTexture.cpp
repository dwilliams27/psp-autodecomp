// eShadowVolumeTexture — sched=1 zone (eAll_psp.obj unity build)
//
// The matching constructor eShadowVolumeTexture::eShadowVolumeTexture(cBase *)
// lives in src/gcProfile.cpp (already matched). This file contains:
//   - Write(cFile &) const           @ 0x00086a98 — writes via parent + cWriteBlock
//   - ~eShadowVolumeTexture()        @ 0x00086bd4 — deleting destructor
//   - New(cMemPool *, cBase *) static @ 0x0021aed4 — placement-new from pool

class cBase;
class cFile;
class cMemPool;
class cType;
class eCamera;
class eRenderSurface;
class eWorld;

class cObject {
public:
    cObject &operator=(const cObject &);
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

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mIndex;
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

class cMemPool_shim {
public:
    static cMemPool_shim *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eVRAMMgrHelper {
public:
    static void SetTextureFromSurface(const eRenderSurface *, bool);
};

extern eRenderSurface D_00098338;

#pragma control sched=1

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
    ~eVirtualTexture();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eShadowVolumeTexture : public eVirtualTexture {
public:
    eShadowVolumeTexture(cBase *);
    ~eShadowVolumeTexture();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void Apply(int, const eCamera *, const eWorld *) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool_shim *pool = cMemPool_shim::GetPoolFromPtr(p);
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
extern cType *D_00046CA0;

extern "C" void eShadowVolumeTexture__eShadowVolumeTexture_cBaseptr(void *self, cBase *parent);

// ── eShadowVolumeTexture::Write(cFile &) const @ 0x00086a98 ──
void eShadowVolumeTexture::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eVirtualTexture *)this)->Write(file);
    wb.End();
}

// ── eShadowVolumeTexture::Read(cFile &, cMemPool *) @ 0x00086ae4 ──
int eShadowVolumeTexture::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eVirtualTexture::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eShadowVolumeTexture::~eShadowVolumeTexture(void) @ 0x00086bd4 ──
eShadowVolumeTexture::~eShadowVolumeTexture() {
    *(void **)((char *)this + 4) = (void *)0x385AA0;
}

// ── eShadowVolumeTexture::Apply(int, const eCamera *, const eWorld *) const @ 0x00086c50 ──
void eShadowVolumeTexture::Apply(int, const eCamera *, const eWorld *) const {
    eRenderSurface *surface = &D_00098338;
    eVRAMMgrHelper::SetTextureFromSurface(surface, false);
    *(unsigned short *)((char *)this + 0x48) = *(unsigned short *)((char *)surface + 0x0C);
    *(unsigned short *)((char *)this + 0x4A) = *(unsigned short *)((char *)surface + 0x0E);
}

// ── eShadowVolumeTexture::AssignCopy(const cBase *) @ 0x0021ae54 ──
void eShadowVolumeTexture::AssignCopy(const cBase *base) {
    eShadowVolumeTexture *other = dcast<eShadowVolumeTexture>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(signed char *)((char *)this + 0x44) = *(const signed char *)((char *)other + 0x44);
    *(signed char *)((char *)this + 0x45) = *(const signed char *)((char *)other + 0x45);
    *(signed char *)((char *)this + 0x46) = *(const signed char *)((char *)other + 0x46);
    *(unsigned char *)((char *)this + 0x47) = *(const unsigned char *)((char *)other + 0x47);
    *(short *)((char *)this + 0x48) = *(const short *)((char *)other + 0x48);
    *(short *)((char *)this + 0x4A) = *(const short *)((char *)other + 0x4A);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 0x4C) = *(const cHandle *)((char *)other + 0x4C);
}

// ── eShadowVolumeTexture::New(cMemPool *, cBase *) static @ 0x0021aed4 ──
// (Address 0x21aed4 is in .gnu.linkonce.t.*, but this static method was still
// compiled with sched=1 by the original — visible in the saves-clustered prologue.)
cBase *eShadowVolumeTexture::New(cMemPool *pool, cBase *parent) {
    eShadowVolumeTexture *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eShadowVolumeTexture *obj = (eShadowVolumeTexture *)rec->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        eShadowVolumeTexture__eShadowVolumeTexture_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eShadowVolumeTexture::GetType(void) const @ 0x0021af50 ──
const cType *eShadowVolumeTexture::GetType(void) const {
    if (D_00046CA0 == 0) {
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
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                __asm__ volatile("" : "+r"(parentType));
                __asm__ volatile("" ::: "memory");
                const char *kindName = (const char *)0x36CDA8;
                const char *kindDesc = (const char *)0x36CDB4;
                __asm__ volatile("" : "+r"(kindName), "+r"(kindDesc));
                D_00040FE8 = cType::InitializeType(
                    0, 0, 0xA, parentType, 0, kindName, kindDesc, 5);
            }
            D_00046B98 = cType::InitializeType(0, 0, 0x135, D_00040FE8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_00046B98;
        cBase *(*factory)(cMemPool *, cBase *) = &eShadowVolumeTexture::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046CA0 = cType::InitializeType(0, 0, 0x214, parentType, factory,
                                           0, 0, 0);
    }
    return D_00046CA0;
}
