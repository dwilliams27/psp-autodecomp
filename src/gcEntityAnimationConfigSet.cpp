// gcEntityAnimationConfigSet — inherits gcNamedSet and holds a cBaseArray.
// Decompiled functions:
//   0x0025aa80 gcEntityAnimationConfigSet::SetSize(int)
//   0x0025a8c4 gcEntityAnimationConfigSet::AssignCopy(const cBase *)
//   0x0010e270 gcEntityAnimationConfigSet::Write(cFile &) const
//   0x0025ad30 gcEntityAnimationConfigSet::~gcEntityAnimationConfigSet(void)
//   0x0025a908 gcEntityAnimationConfigSet::New(cMemPool *, cBase *) static

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

class gcEntityAnimationConfigSet : public gcNamedSet {
public:
    cBaseArray mArray;   // 0x0C (data ptr) + 0x10 (self ptr) — 8 bytes

    void SetSize(int);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

gcEntityAnimationConfigSet *dcast(const cBase *);

extern char gcEntityAnimationConfigSet_cBase_vtable[];       // 0x37E6A8
extern char gcEntityAnimationConfigSet_gcNamedSet_vtable[];  // 0x386C58
extern char gcEntityAnimationConfigSetvirtualtable[];        // 0x3884E8
extern char cBaseclassdesc[];                                // 0x37E6A8

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
// 0x0025aa80 — SetSize(int): forward to cBaseArray::SetSize on member at +12
// ============================================================
void gcEntityAnimationConfigSet::SetSize(int size) {
    ((cBaseArray *)((char *)this + 12))->SetSize(size);
}

// ============================================================
// 0x0025a8c4 — AssignCopy(const cBase *)
// ============================================================
void gcEntityAnimationConfigSet::AssignCopy(const cBase *base) {
    gcEntityAnimationConfigSet *src = dcast(base);
    ((gcNamedSet *)this)->mName = ((gcNamedSet *)src)->mName;
    __asm__ volatile("" ::: "memory");
    this->mArray = src->mArray;
}

// ============================================================
// 0x0010e270 — Write(cFile &) const
// ============================================================
void gcEntityAnimationConfigSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcNamedSet::Write(file);
    ((cBaseArray *)((char *)this + 12))->Write(wb);
    wb.End();
}

// ============================================================
// 0x0025a908 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityAnimationConfigSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAnimationConfigSet *result = 0;
    gcEntityAnimationConfigSet *obj =
        (gcEntityAnimationConfigSet *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcEntityAnimationConfigSet_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcEntityAnimationConfigSet_gcNamedSet_vtable;
        ((int *)obj)[2] = 0;
        ((void **)obj)[1] = gcEntityAnimationConfigSetvirtualtable;
        ((int *)obj)[3] = 0;
        ((int **)obj)[4] = (int *)obj;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0025ad30 — destructor
// ============================================================
extern "C" {

void gcEntityAnimationConfigSet___dtor_gcEntityAnimationConfigSet_void(
        gcEntityAnimationConfigSet *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcEntityAnimationConfigSetvirtualtable;
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
