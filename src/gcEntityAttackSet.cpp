// gcEntityAttackSet — inherits gcNamedSet and holds a cBaseArray.
// Decompiled functions:
//   0x0025c864 gcEntityAttackSet::SetSize(int)
//   0x0025c6a8 gcEntityAttackSet::AssignCopy(const cBase *)
//   0x0010fa78 gcEntityAttackSet::Write(cFile &) const
//   0x0025cb14 gcEntityAttackSet::~gcEntityAttackSet(void)
//   0x0025c6ec gcEntityAttackSet::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void SetSize(int);
    void Write(cWriteBlock &) const;
    void RemoveAll(void);
    cBaseArray &operator=(const cBaseArray &);
};

class gcNamedSetName {
public:
    int mIndex;
};

class gcNamedSet {
public:
    cBase *m_parent;     // 0x00
    void *m_vtable;      // 0x04
    gcNamedSetName mName;// 0x08
    void Write(cFile &) const;
};

class gcEntityAttackSet : public gcNamedSet {
public:
    cBaseArray mArray;   // 0x0C (data ptr) + 0x10 (self ptr) — 8 bytes

    void SetSize(int);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

gcEntityAttackSet *dcast(const cBase *);

extern char gcEntityAttackSet_cBase_vtable[];       // 0x37E6A8
extern char gcEntityAttackSet_gcNamedSet_vtable[];  // 0x386C58
extern char gcEntityAttackSetvirtualtable[];        // 0x3886B8
extern char cBaseclassdesc[];                       // 0x37E6A8

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

void *cMemPool_GetPoolFromPtr(void *);

// ============================================================
// 0x0025c864 — SetSize(int): forward to cBaseArray::SetSize on member at +12
// ============================================================
void gcEntityAttackSet::SetSize(int size) {
    ((cBaseArray *)((char *)this + 12))->SetSize(size);
}

// ============================================================
// 0x0025c6a8 — AssignCopy(const cBase *)
// ============================================================
void gcEntityAttackSet::AssignCopy(const cBase *base) {
    gcEntityAttackSet *src = dcast(base);
    ((gcNamedSet *)this)->mName = ((gcNamedSet *)src)->mName;
    __asm__ volatile("" ::: "memory");
    this->mArray = src->mArray;
}

// ============================================================
// 0x0010fa78 — Write(cFile &) const
// ============================================================
void gcEntityAttackSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcNamedSet::Write(file);
    ((cBaseArray *)((char *)this + 12))->Write(wb);
    wb.End();
}

// ============================================================
// 0x0025c6ec — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityAttackSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAttackSet *result = 0;
    gcEntityAttackSet *obj =
        (gcEntityAttackSet *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcEntityAttackSet_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcEntityAttackSet_gcNamedSet_vtable;
        ((int *)obj)[2] = 0;
        ((void **)obj)[1] = gcEntityAttackSetvirtualtable;
        ((int *)obj)[3] = 0;
        ((int **)obj)[4] = (int *)obj;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0025cb14 — destructor
// ============================================================
extern "C" {

void gcEntityAttackSet___dtor_gcEntityAttackSet_void(
        gcEntityAttackSet *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcEntityAttackSetvirtualtable;
        void *arr = (char *)self + 12;
        if (arr != 0) {
            ((cBaseArray *)arr)->RemoveAll();
        }
        ((void **)self)[1] = cBaseclassdesc;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
