// eHeightmapMtl.cpp — decompiled from eAll_psp.obj
// eHeightmapMtl inherits from eGeomMtl. Same Write pattern as
// eWeatherEffectMtl / eGeomTrailMtl: a 4-byte int field at offset 0x68 is serialized.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
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
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class eGeomMtl {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eHeightmapMtl : public eGeomMtl {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

#pragma control sched=1

// ── eHeightmapMtl::Write(cFile &) const @ 0x0007acd8 ──
void eHeightmapMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeomMtl *)this)->Write(file);
    wb.Write(*(int *)((char *)this + 0x68));
    wb.End();
}

// ── eHeightmapMtl::Read(cFile &, cMemPool *) @ 0x0007ad30 ──
int eHeightmapMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((eGeomMtl *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" ::: "memory");
        cFileSystem::Read(h, (char *)this + 0x68, 4);
    }
    return result;
}

#pragma control sched=2
