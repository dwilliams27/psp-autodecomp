// eGeomMtlSet.cpp — decompiled from eAll_psp.obj
// eGeomMtlSet inherits from eMaterialSet.

class cFile;
class cMemPool;

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

class eMaterialSet {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eGeomMtlSet : public eMaterialSet {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

// ── eGeomMtlSet::Write(cFile &) const @ 0x0002CEE4 ──
void eGeomMtlSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eMaterialSet *)this)->Write(file);
    wb.End();
}

// ── eGeomMtlSet::Read(cFile &, cMemPool *) @ 0x0002CF30 ──
int eGeomMtlSet::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((eMaterialSet *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
