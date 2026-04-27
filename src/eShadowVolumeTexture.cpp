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

#pragma control sched=1

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
    ~eVirtualTexture();
    void Write(cFile &) const;
};

class eShadowVolumeTexture : public eVirtualTexture {
public:
    eShadowVolumeTexture(cBase *);
    ~eShadowVolumeTexture();
    void Write(cFile &) const;
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

extern "C" void eShadowVolumeTexture__eShadowVolumeTexture_cBaseptr(void *self, cBase *parent);

// ── eShadowVolumeTexture::Write(cFile &) const @ 0x00086a98 ──
void eShadowVolumeTexture::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eVirtualTexture *)this)->Write(file);
    wb.End();
}

// ── eShadowVolumeTexture::~eShadowVolumeTexture(void) @ 0x00086bd4 ──
eShadowVolumeTexture::~eShadowVolumeTexture() {
    *(void **)((char *)this + 4) = (void *)0x385AA0;
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
