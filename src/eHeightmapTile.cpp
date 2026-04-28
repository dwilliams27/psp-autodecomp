// eHeightmapTile — eAll_psp.obj
//   0x00051a3c  Write(cWriteBlock &) const         (104B)
//   0x00051aa4  Read(cReadBlock &)                 (176B)
//
// 20-byte serialised tile record: two floats, two cHandles, and a u32.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    void Write(unsigned int);
    void Write(float);
};

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

class cMemPool_S {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eHeightmapTile {
public:
    float        mF0;
    float        mF4;
    cHandle      mH8;
    cHandle      mHC;
    unsigned int mU10;

    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

#pragma control sched=1

// ── 0x00051a3c — Write(cWriteBlock &) const, 104B ──
void eHeightmapTile::Write(cWriteBlock &wb) const {
    __asm__ volatile("" ::: "memory");
    wb.Write(mF0);
    __asm__ volatile("" ::: "memory");
    wb.Write(mF4);
    __asm__ volatile("" ::: "memory");
    mH8.Write(wb);
    __asm__ volatile("" ::: "memory");
    mHC.Write(wb);
    __asm__ volatile("" ::: "memory");
    wb.Write(mU10);
}

// ── 0x00051aa4 — Read(cReadBlock &), 176B ──
void eHeightmapTile::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], this, 4);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 4, 4);
    __asm__ volatile("" ::: "memory");
    mH8.mIndex = 0;
    mH8.Read(rb, cMemPool_S::GetPoolFromPtr(&mH8));
    __asm__ volatile("" ::: "memory");
    mHC.mIndex = 0;
    mHC.Read(rb, cMemPool_S::GetPoolFromPtr(&mHC));
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 16, 4);
}
