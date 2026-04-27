// eModelMtl.cpp — decompiled from eAll_psp.obj.
// Parent: eGeomMtl.
//
// Functions:
//   0x0005DEBC eModelMtl::Write(cFile &) const            100B
//   0x0005DF20 eModelMtl::Read(cFile &, cMemPool *)       228B

class cBase;
class cFile;
class cMemPool;
struct cFileHandle;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
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
    static int Read(cFileHandle *, void *, unsigned int);
};

class eGeomMtl {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eModelMtl : public eGeomMtl {
public:
    char _pad[0x68 - 0x00];
    unsigned int mField68;  // 0x68
    unsigned int mField6C;  // 0x6C

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

#pragma control sched=1

// ── eModelMtl::Write @ 0x0005DEBC ──
void eModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eGeomMtl::Write(file);
    wb.Write(mField68);
    wb.Write(mField6C);
    wb.End();
}

// ── eModelMtl::Read @ 0x0005DF20 ──
int eModelMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && eGeomMtl::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" ::: "memory");
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x68, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" ::: "memory");
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x6C, 4);
    }
    return result;
}
