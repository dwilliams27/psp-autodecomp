// eGeomWake methods. Inherits eDynamicGeom : eGeom : cObject. In sched=1 zone.
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
    int base;
};

class eDynamicGeom : public eGeom {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eGeomWake : public eDynamicGeom {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void eGeomWake__eGeomWake_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── eGeomWake::Write(cFile &) const @ 0x00079ef0 ──
void eGeomWake::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eDynamicGeom::Write(file);
    wb.End();
}

// ── eGeomWake::Read(cFile &, cMemPool *) @ 0x00079f3c ──
int eGeomWake::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && this->eDynamicGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eGeomWake::New(cMemPool *, cBase *) static @ 0x00213134 ──
cBase *eGeomWake::New(cMemPool *pool, cBase *parent) {
    eGeomWake *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eGeomWake *obj = (eGeomWake *)rec->fn(base, 0x120, 0x10, 0, 0);
    if (obj != 0) {
        eGeomWake__eGeomWake_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
