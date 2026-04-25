// gcFloatSet — inherits gcNamedSet and holds a cArrayBase<float>.
// Decompiled functions:
//   0x0024967c gcFloatSet::GetSize(void) const
//   0x000fe9a4 gcFloatSet::Write(cFile &) const
//   0x00249370 gcFloatSet::New(cMemPool *, cBase *) static
//   0x00249408 gcFloatSet::GetType(void) const
//   0x000fea3c gcFloatSet::Read(cFile &, cMemPool *)

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
    void Write(int, const float *);
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
    unsigned int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcFloatSet : public gcNamedSet {
public:
    cArray<float> mArray; // 0x0C

    int GetSize(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

extern char gcFloatSet_cBase_vtable[];        // 0x37E6A8
extern char gcFloatSet_gcNamedSet_vtable[];   // 0x386C58
extern char gcFloatSetvirtualtable[];         // 0x387E98

extern cType *D_000385DC;
extern cType *D_000998F4;
extern cType *D_0009A2EC;

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
// 0x0024967c — GetSize(void) const
// ============================================================
int gcFloatSet::GetSize(void) const {
    int *p = (int *)mArray.mData;
    int size = 0;
    if (p != 0) {
        size = p[-1] & 0x3FFFFFFF;
    }
    return size;
}

// ============================================================
// 0x000fe9a4 — Write(cFile &) const
// ============================================================
void gcFloatSet::Write(cFile &file) const {
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
// 0x00249370 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcFloatSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcFloatSet *result = 0;
    gcFloatSet *obj = (gcFloatSet *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcFloatSet_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcFloatSet_gcNamedSet_vtable;
        ((int *)obj)[2] = 0;
        ((void **)obj)[1] = gcFloatSetvirtualtable;
        ((int *)obj)[3] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00249408 — GetType(void) const
// ============================================================
const cType *gcFloatSet::GetType(void) const {
    if (D_0009A2EC == 0) {
        if (D_000998F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000998F4 = cType::InitializeType(0, 0, 0xAD, D_000385DC, 0, 0, 0, 0);
        }
        D_0009A2EC = cType::InitializeType(0, 0, 0xAE, D_000998F4,
                                           gcFloatSet::New, 0, 0, 0);
    }
    return D_0009A2EC;
}

// ============================================================
// 0x000fea3c — Read(cFile &, cMemPool *)
// ============================================================
int gcFloatSet::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile ("ori %0,$0,1" : "=r"(result));
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
