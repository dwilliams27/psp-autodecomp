// eParticleSystemMtl.cpp — decompiled from eAll_psp.obj
// eParticleSystemMtl inherits from eGeomMtl.

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

class eParticleSystemMtl : public eGeomMtl {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

// ── eParticleSystemMtl::Write(cFile &) const @ 0x00034008 ──
void eParticleSystemMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeomMtl *)this)->Write(file);
    wb.Write(*(int *)((char *)this + 0x68));
    wb.End();
}

// ── eParticleSystemMtl::Read(cFile &, cMemPool *) @ 0x00034060 ──
int eParticleSystemMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((eGeomMtl *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x68, 4);
    return result;
}
