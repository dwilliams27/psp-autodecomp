// eStaticGeomTemplate.cpp — decompiled from eAll_psp.obj
// Parent: eGeomTemplate (which inherits from cObject).

class cBase;
class cFile;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class eGeomTemplate : public cObject {
public:
    eGeomTemplate(cBase *);
    ~eGeomTemplate(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eStaticGeomTemplate : public eGeomTemplate {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

// ── eStaticGeomTemplate::Write(cFile &) const @ 0x00041050 ──
void eStaticGeomTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eGeomTemplate *)this)->Write(file);
    wb.End();
}

// ── eStaticGeomTemplate::Read(cFile &, cMemPool *) @ 0x0004109c ──
int eStaticGeomTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((eGeomTemplate *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
