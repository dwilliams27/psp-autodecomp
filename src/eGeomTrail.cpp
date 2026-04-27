// eGeomTrail::eGeomTrail(cBase *) @ 0x00079460
// Inherits eDynamicGeom : eGeom : cObject. In sched=1 zone.
#pragma control sched=1

class cBase;
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

class eGeom {
public:
    eGeom(cBase *);
    char _eGeomPad[0x90];
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    char _dynPad[0x58];
};

class eGeomTrail : public eDynamicGeom {
public:
    eGeomTrail(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    char _trailPad[0x30];
};

extern char eGeomTrailvirtualtable[];

extern "C" {
    void eGeomTrail__eGeomTrail_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

eGeomTrail::eGeomTrail(cBase *base) : eDynamicGeom(base) {
    *(void **)((char *)this + 4) = eGeomTrailvirtualtable;
    *(int *)((char *)this + 0xF0) = 0;
    *(int *)((char *)this + 0xF8) = 0;
    *(int *)((char *)this + 0xFC) = 0;
}

// ── eGeomTrail::Write(cFile &) const @ 0x00079358 ──
void eGeomTrail::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eDynamicGeom::Write(file);
    wb.End();
}

// ── eGeomTrail::Read(cFile &, cMemPool *) @ 0x000793A4 ──
int eGeomTrail::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eGeomTrail::New(cMemPool *, cBase *) static @ 0x00211CB4 ──
cBase *eGeomTrail::New(cMemPool *pool, cBase *parent) {
    eGeomTrail *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eGeomTrail *obj = (eGeomTrail *)rec->fn(base, 0x100, 0x10, 0, 0);
    if (obj != 0) {
        eGeomTrail__eGeomTrail_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
