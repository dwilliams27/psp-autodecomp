class cBase;
class cFile;
class cMemPool;
class cType;
class eGeom;
class eDynamicGeom;
class gcUIGeom;

class cTimeValue {
public:
    int mTime;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

void cFile_SetCurrentPos(void *, unsigned int);

class eGeom {
public:
    int base;
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    ~eDynamicGeom();
    eDynamicGeom &operator=(const eDynamicGeom &);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcUI {
public:
    void Update(cTimeValue);
    void CloseAllDialogs(void);
    void DeleteSpawned(void);
    ~gcUI();
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcUIGeom : public eDynamicGeom {
public:
    gcUIGeom(cBase *);
    ~gcUIGeom();
    gcUIGeom &operator=(const gcUIGeom &);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Update(cTimeValue);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);

    static void operator delete(void *p);
};

extern "C" {
    void gcUIGeom__gcUIGeom_cBaseptr(void *self, cBase *parent);
    void gcUI__gcUI_cBaseptr_unsignedint(void *self, cBase *parent,
                                          unsigned int flags)
        asm("__0oEgcUIctP6FcBaseUi");
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct TypeVtableEntry {
    short offset;
    short _pad;
    const cType *(*fn)(const void *);
};

extern cType *D_000385DC;
extern cType *D_00040FF4;
extern cType *D_000469C0;
extern cType *D_0009F58C;

inline void gcUIGeom::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
}

// ── gcUIGeom::~gcUIGeom(void) @ 0x0013B3B4 ──
gcUIGeom::~gcUIGeom() {
    *(void **)((char *)this + 4) = (void *)0x38B300;
    gcUI *ui = (gcUI *)((char *)this + 0xF0);
    ui->CloseAllDialogs();
    ui->DeleteSpawned();
    ui->~gcUI();
}

// ── gcUIGeom::gcUIGeom(cBase *) @ 0x0013b370 ──
gcUIGeom::gcUIGeom(cBase *parent) : eDynamicGeom(parent) {
    *(void **)((char *)this + 4) = (void *)0x38B300;
    gcUI__gcUI_cBaseptr_unsignedint((char *)this + 0xF0, (cBase *)this, 1);
}

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

// ── gcUIGeom::Update(cTimeValue) @ 0x0013b8c0 ──
void gcUIGeom::Update(cTimeValue t) {
    ((gcUI *)((char *)this + 0xF0))->Update(t);
    if (*(unsigned char *)((char *)this + 0x8C) & 4) {
        int *vt = *(int **)((char *)this + 4);
        int *entry = (int *)((char *)vt + 0xB8);
        short adj = *(short *)entry;
        void (*fn)(void *) = (void (*)(void *))entry[1];
        fn((char *)this + adj);
    }
}

const cType *gcUIGeom::GetType(void) const {
    if (D_0009F58C == 0) {
        if (D_000469C0 == 0) {
            if (D_00040FF4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                               0, 0, 0, 0);
        }
        D_0009F58C =
            cType::InitializeType(0, 0, 0x82, D_000469C0, gcUIGeom::New,
                                  0, 0, 0);
    }
    return D_0009F58C;
}

// ── gcUIGeom::AssignCopy(const cBase *) @ 0x00290b1c ──
void gcUIGeom::AssignCopy(const cBase *base) {
    gcUIGeom *other = 0;
    if (base != 0) {
        if (D_0009F58C == 0) {
            if (D_000469C0 == 0) {
                if (D_00040FF4 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_00040FF4 = cType::InitializeType(0, 0, 0x16, D_000385DC,
                                                       0, 0, 0, 0);
                }
                D_000469C0 = cType::InitializeType(0, 0, 0x17, D_00040FF4,
                                                   0, 0, 0, 0);
            }
            D_0009F58C = cType::InitializeType(0, 0, 0x82, D_000469C0,
                                               gcUIGeom::New, 0, 0, 0);
        }
        const TypeVtableEntry *entry =
            (const TypeVtableEntry *)(*(const char **)((const char *)base + 4) + 8);
        const cType *target = D_0009F58C;
        const cType *type = entry->fn((const char *)base + entry->offset);
        int isGeom;
        if (target != 0 && type != 0) {
            do {
                if (type == target) {
                    isGeom = 1;
                    goto checked_type;
                }
                type = *(const cType *const *)((const char *)type + 0x1C);
            } while (type != 0);
            isGeom = 0;
        } else {
            isGeom = 0;
        }
    checked_type:
        if (isGeom != 0) {
            other = (gcUIGeom *)base;
        }
    }
    *this = *other;
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
