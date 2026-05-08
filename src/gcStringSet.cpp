class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class gcStringTable;
class gcString;

class cBase {
public:
    void *_parent;   // offset 0
    void *_vtable;   // offset 4
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

template <class T>
class cArray : public cArrayBase<T> {
public:
    void Read(cReadBlock &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int _data[2];
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                  cBase *(*)(cMemPool *, cBase *),
                                  const char *, const char *, unsigned int);
};

class gcNamedSetName {
public:
    int mIndex;
};

class gcNamedSet : public cBase {
public:
    gcNamedSetName mName;   // offset 8
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcStringSet : public gcNamedSet {
public:
    cArrayBase<cHandlePairT<gcStringTable, cSubHandleT<gcString> > > mArray;  // offset 12
    int GetSize(void) const {
        int size = 0;
        int *arr = (int *)mArray.mData;
        if (arr != 0) {
            size = arr[-1] & 0x3FFFFFFF;
        }
        return size;
    }
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

class gcValue {
public:
    int Read(cFile &, cMemPool *);
};

// ODR-WARNING: split-TU method addition for gcValPointValue. Keep this
// local redeclaration minimal so existing matched siblings do not drift.
class gcValPointValue : public gcValue {
public:
    int Read(cFile &, cMemPool *);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

gcStringSet *dcast(const cBase *);

extern char gcStringSet_cBase_vtable[];        // 0x37E6A8
extern char gcStringSet_gcNamedSet_vtable[];   // 0x386C58
extern char gcStringSetvirtualtable[];         // 0x387F18

extern const char gcStringSet_base_name[];
extern const char gcStringSet_base_desc[];

static cType *type_base;
static cType *type_named;
static cType *type_gcStringSet;

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

extern "C" void cFileSystem_Read(void *, void *, unsigned int);

// ============================================================
// gcStringSet::AssignCopy(const cBase *)
// ============================================================
void gcStringSet::AssignCopy(const cBase *base) {
    gcStringSet *src = dcast(base);
    ((gcNamedSet *)this)->mName = ((gcNamedSet *)src)->mName;
    __asm__ volatile("" ::: "memory");
    this->mArray = src->mArray;
}

// ============================================================
// gcStringSet::New(cMemPool *, cBase *) static
// ============================================================
cBase *gcStringSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStringSet *result = 0;
    gcStringSet *obj = (gcStringSet *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcStringSet_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcStringSet_gcNamedSet_vtable;
        ((int *)obj)[2] = 0;
        ((void **)obj)[1] = gcStringSetvirtualtable;
        ((int *)obj)[3] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// gcStringSet::Write(cFile &) const
// ============================================================
void gcStringSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcNamedSet::Write(file);
    wb.Write(GetSize());
    int i = 0;
    int size = GetSize();
    if (i < size) {
        int offset = 0;
        cHandle *p = (cHandle *)((char *)mArray.mData + offset);
        do {
            p->Write(wb);
            wb.Write((unsigned int)((int *)p)[1]);
            i++;
            p = (cHandle *)((char *)p + 8);
        } while (i < size);
    }
    wb.End();
}

// ============================================================
// gcStringSet::GetType(void) const
// ============================================================
const cType *gcStringSet::GetType(void) const {
    if (!type_gcStringSet) {
        if (!type_named) {
            if (!type_base) {
                type_base = cType::InitializeType(gcStringSet_base_name, gcStringSet_base_desc, 1, 0, 0, 0, 0, 0);
            }
            type_named = cType::InitializeType(0, 0, 0xAD, type_base, 0, 0, 0, 0);
        }
        type_gcStringSet = cType::InitializeType(0, 0, 0x1ED, type_named, gcStringSet::New, 0, 0, 0);
    }
    return type_gcStringSet;
}

// ============================================================
// gcStringSet::Read(cFile &, cMemPool *)
// ============================================================
int gcStringSet::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || gcNamedSet::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    ((cArray<cHandlePairT<gcStringTable, cSubHandleT<gcString> > > *)&mArray)->Read(rb);
    void *vt = _vtable;
    VTableSlotVoid *slot = (VTableSlotVoid *)((char *)vt + 0x38);
    slot->fn((char *)this + slot->offset, pool, 0);
    return result;
}

// ============================================================
// gcValPointValue::Read(cFile &, cMemPool *)
// ============================================================
int gcValPointValue::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || gcValue::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    cFileSystem_Read(*(void **)rb._data[0], (char *)this + 8, 4);
    cFileSystem_Read(*(void **)rb._data[0], (char *)this + 12, 4);
    char *base = (char *)this + 16;
    char *mType = *(char **)((char *)this + 20);
    cTypeMethod *slot = (cTypeMethod *)(mType + 0x30);
    cFile *f = (cFile *)rb._data[0];
    typedef void (*ReadFn)(void *, cFile *, cMemPool *);
    ((ReadFn)slot->fn)(base + slot->offset, f, cMemPool::GetPoolFromPtr(base));
    return result;
}
