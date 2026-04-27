// eGeomTrailMtl.cpp — decompiled from eAll_psp.obj
// eGeomTrailMtl inherits from eGeomMtl. Same Write/Read pattern as
// eWeatherEffectMtl: a 4-byte int field at offset 0x68 is serialized.

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

class eGeomTrailMtl : public eGeomMtl {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

#pragma control sched=1

// ── eGeomTrailMtl::Write(cFile &) const @ 0x00078e24 ──
void eGeomTrailMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeomMtl *)this)->Write(file);
    wb.Write(*(int *)((char *)this + 0x68));
    wb.End();
}

// ── eGeomTrailMtl::Read(cFile &, cMemPool *) @ 0x00078e7c ──
int eGeomTrailMtl::Read(cFile &file, cMemPool *pool) {
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
