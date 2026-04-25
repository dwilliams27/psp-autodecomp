// gcBoolSet — inherits gcNamedSet and holds a cArrayBase<bool>.
// Decompiled functions:
//   0x0023a0c0 gcBoolSet::GetSize(void) const
//   0x000d4f2c gcBoolSet::Write(cFile &) const
//   0x00239dcc gcBoolSet::New(cMemPool *, cBase *) static
//   0x00239e64 gcBoolSet::GetType(void) const
//   0x000d4fc4 gcBoolSet::Read(cFile &, cMemPool *)

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const bool *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T>
class cArray : public cArrayBase<T> {
public:
    void Read(cReadBlock &);
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
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcBoolSet : public gcNamedSet {
public:
    cArray<bool> mArray; // 0x0C

    int GetSize(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

extern char gcBoolSet_cBase_vtable[];        // 0x37E6A8
extern char gcBoolSet_gcNamedSet_vtable[];   // 0x386C58
extern char gcBoolSetvirtualtable[];         // 0x386CD8

extern cType *D_000385DC;
extern cType *D_000998F4;
extern cType *D_000998F8;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct VTableSlotVoid {
    short offset;
    short pad;
    void (*fn)(void *, cMemPool *, int);
};

// ============================================================
// 0x0023a0c0 — GetSize(void) const
// ============================================================
int gcBoolSet::GetSize(void) const {
    int *p = (int *)mArray.mData;
    int size = 0;
    if (p != 0) {
        size = p[-1] & 0x3FFFFFFF;
    }
    return size;
}

// ============================================================
// 0x000d4f2c — Write(cFile &) const
// ============================================================
void gcBoolSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcNamedSet::Write(file);

    int s1 = 0;
    if (mArray.mData != 0) {
        s1 = ((int *)mArray.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(s1);

    int s2 = 0;
    if (mArray.mData != 0) {
        s2 = ((int *)mArray.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(s2, mArray.mData);

    wb.End();
}

// ============================================================
// 0x00239dcc — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcBoolSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcBoolSet *result = 0;
    gcBoolSet *obj = (gcBoolSet *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcBoolSet_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcBoolSet_gcNamedSet_vtable;
        ((int *)obj)[2] = 0;
        ((void **)obj)[1] = gcBoolSetvirtualtable;
        ((int *)obj)[3] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00239e64 — GetType(void) const
// ============================================================
const cType *gcBoolSet::GetType(void) const {
    if (D_000998F8 == 0) {
        if (D_000998F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000998F4 = cType::InitializeType(0, 0, 0xAD, D_000385DC, 0, 0, 0, 0);
        }
        D_000998F8 = cType::InitializeType(0, 0, 0xAF, D_000998F4,
                                           gcBoolSet::New, 0, 0, 0);
    }
    return D_000998F8;
}

// ============================================================
// 0x000d4fc4 — Read(cFile &, cMemPool *)
// ============================================================
int gcBoolSet::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__("li %0,1" : "=r"(result));
    if (rb._data[3] != 1 || gcNamedSet::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    mArray.Read(rb);
    void *vt = m_vtable;
    VTableSlotVoid *slot = (VTableSlotVoid *)((char *)vt + 0x38);
    slot->fn((char *)this + slot->offset, pool, 0);
    return result;
}
