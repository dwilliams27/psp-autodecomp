// gcPartialBodyControllerTemplate — derives from gcPartialEntityControllerTemplate.
// Decompiled functions:
//   0x0013fd40 gcPartialBodyControllerTemplate::gcPartialBodyControllerTemplate(cBase *)
//   0x0013fbe8 gcPartialBodyControllerTemplate::Write(cFile &) const
//   0x0013ff5c gcPartialBodyControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const
//   0x001400d4 gcPartialBodyControllerTemplate::FindAttackSet(cHandleT<gcEnumeration>) const
//   0x002a5580 gcPartialBodyControllerTemplate::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class gcEnumeration;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(short);
    void End(void);
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    void Write(cWriteBlock &) const;
    cBaseArray &operator=(const cBaseArray &);
};

template <class T>
struct cHandleT {
    int mIndex;
    bool operator==(const cHandleT &other) const { return mIndex == other.mIndex; }
};

class cHandle {
public:
    int mIndex;
};

class gcPartialEntityControllerTemplate {
public:
    cBase *mOwner;          // 0x00
    void *mClassDesc;       // 0x04
    cHandle mHandle1;       // 0x08
    unsigned int mIndex;    // 0x0C
    cHandle mHandle2;       // 0x10

    gcPartialEntityControllerTemplate(cBase *);
    void Write(cFile &) const;
};

class gcPartialBodyControllerTemplate : public gcPartialEntityControllerTemplate {
public:
    cBaseArray mArr1;       // 0x14
    short mField1C;         // 0x1C
    short _pad1E;           // 0x1E
    cBaseArray mArr2;       // 0x20

    gcPartialBodyControllerTemplate(cBase *);
    void AssignCopy(const cBase *);
    int FindAnimationSet(cHandleT<gcEnumeration>) const;
    void Write(cFile &) const;
    int FindAttackSet(cHandleT<gcEnumeration>) const;
    static cBase *New(cMemPool *, cBase *);
};

// Animation set / attack set entry: cHandleT<gcEnumeration> at +8.
class gcPartialBodyControllerSet {
public:
    char _pad[8];
    cHandleT<gcEnumeration> mEnumeration;   // +8
    const cHandleT<gcEnumeration> &GetEnumeration(void) const;
};

inline const cHandleT<gcEnumeration> &
gcPartialBodyControllerSet::GetEnumeration(void) const {
    return mEnumeration;
}

extern char gcPartialBodyControllerTemplatevtable[];   // 0x38BA50

void gcPartialBodyControllerTemplate_ctor(gcPartialBodyControllerTemplate *, cBase *);

template <class T> T *dcast(const cBase *);

// Pool allocation infrastructure (matches gcEntityAttackSet::New pattern).
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// =====================================================================
// 0x0013fd40 — constructor
// =====================================================================
gcPartialBodyControllerTemplate::gcPartialBodyControllerTemplate(cBase *owner)
    : gcPartialEntityControllerTemplate(owner)
{
    mClassDesc = gcPartialBodyControllerTemplatevtable;
    mArr1.mData = 0;
    mArr1.mOwner = (void *)this;
    mField1C = 0;
    mArr2.mData = 0;
    mArr2.mOwner = (void *)this;
}

// =====================================================================
// 0x0013fbe8 — Write(cFile &) const
// =====================================================================
void gcPartialBodyControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcPartialEntityControllerTemplate::Write(file);
    mArr1.Write(wb);
    wb.Write(mField1C);
    mArr2.Write(wb);
    wb.End();
}

// =====================================================================
// 0x002a54fc — AssignCopy(const cBase *)
// =====================================================================
void gcPartialBodyControllerTemplate::AssignCopy(const cBase *base) {
    gcPartialBodyControllerTemplate *other =
        dcast<gcPartialBodyControllerTemplate>(base);
    unsigned int *dstIndex = &mIndex;
    mHandle1 = other->mHandle1;
    const unsigned int *srcIndex = &other->mIndex;
    *dstIndex = *srcIndex;
    mHandle2 = other->mHandle2;
    mArr1.operator=(other->mArr1);
    mField1C = other->mField1C;
    mArr2.operator=(other->mArr2);
}

// =====================================================================
// 0x0013ff5c — FindAnimationSet(cHandleT<gcEnumeration>) const
// =====================================================================
int gcPartialBodyControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration> h) const {
    int i = 0;
    void *data = mArr1.mData;
    int offset = 0;
    while (1) {
        int size = 0;
        if (data != 0) {
            size = ((int *)data)[-1];
        }
        if (i >= size) {
            return -1;
        }
        gcPartialBodyControllerSet *e =
            *(gcPartialBodyControllerSet **)((char *)data + offset);
        const cHandleT<gcEnumeration> *eh =
            (const cHandleT<gcEnumeration> *)((const char *)e + 8);
        if (*eh == h) {
            return i;
        }
        i++;
        offset += 4;
    }
}

// =====================================================================
// 0x001400d4 — FindAttackSet(cHandleT<gcEnumeration>) const
// =====================================================================
int gcPartialBodyControllerTemplate::FindAttackSet(cHandleT<gcEnumeration> h) const {
    int i = 0;
    void *data = mArr2.mData;
    int offset = 0;
    while (1) {
        int size = 0;
        if (data != 0) {
            size = ((int *)data)[-1];
        }
        if (i >= size) {
            return -1;
        }
        gcPartialBodyControllerSet *e =
            *(gcPartialBodyControllerSet **)((char *)data + offset);
        const cHandleT<gcEnumeration> *eh =
            (const cHandleT<gcEnumeration> *)((const char *)e + 8);
        if (*eh == h) {
            return i;
        }
        i++;
        offset += 4;
    }
}

// =====================================================================
// 0x002a5580 — New(cMemPool *, cBase *) static
// =====================================================================
cBase *gcPartialBodyControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcPartialBodyControllerTemplate *result = 0;
    gcPartialBodyControllerTemplate *obj =
        (gcPartialBodyControllerTemplate *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        gcPartialBodyControllerTemplate_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
