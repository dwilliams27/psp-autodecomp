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

// ── eVirtualTexture::~eVirtualTexture @ 0x00060654 ──
// SNC auto-generates: (this != 0) guard, ~eTexture(0) base call, and the
// `if (flags & 1) operator delete(this)` deleting tail (inlined).
eVirtualTexture::~eVirtualTexture(void) {
    *(void **)((char *)this + 4) = eVirtualTexturevirtualtable;
}

#pragma control sched=2
