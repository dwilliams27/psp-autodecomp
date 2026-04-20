class cFile;
class cMemPool;
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
    const cType *GetType(void) const;
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
