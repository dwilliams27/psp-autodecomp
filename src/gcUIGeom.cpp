class cBase;
class cFile;
class cMemPool;
class eGeom;
class eDynamicGeom;
class gcUIGeom;

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

class gcUIGeom : public eDynamicGeom {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern "C" {
    void gcUIGeom__gcUIGeom_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcUIGeom::Write(cFile &) const @ 0x0013b268 ──
void gcUIGeom::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->eDynamicGeom::Write(file);
    wb.End();
}

// ── gcUIGeom::Read(cFile &, cMemPool *) @ 0x0013b2b4 ──
// STATUS: NEAR-MATCH (20/188 bytes differ)
// Same prologue scheduling diff as eDynamicGeom::Read (also 20/188).
// SNC sched=2 does not interleave saves/moves the way the original compiler did
// here: expected has {sw s0, sw s2, move s0, move s2, sw s1, sw s3, move s1, ...}
// while ours has {sw s0, sw s1, sw s2, move s0, move s1, move s2, sw s3, ...}.
// Trailing 0x30..0x88 (eDynamicGeom::Read call + epilogues) match perfectly.
int gcUIGeom::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || !this->eDynamicGeom::Read(file, pool)) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return result;
}

// ── gcUIGeom::New(cMemPool *, cBase *) static @ 0x00290cc0 ──
cBase *gcUIGeom::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIGeom *result = 0;
    gcUIGeom *obj = (gcUIGeom *)rec->fn(base, 0x280, 0x10, 0, 0);
    if (obj != 0) {
        gcUIGeom__gcUIGeom_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
