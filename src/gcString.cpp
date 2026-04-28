// gcString — localized wide-string object.

inline void *operator new(unsigned int, void *p) { return p; }

class cBase {
public:
    cBase *mOwner;
    void *mClassDesc;
};

class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

template <class T>
class cArrayBase {
public:
    int mData;
    cArrayBase<T> &operator=(const cArrayBase<T> &);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct IntPair {
    int a;
    int b;
};

class gcString : public cBase {
public:
    int mField08;
    int mField0C;
    IntPair mField10;
    cArrayBase<wchar_t> mText;
    cArrayBase<wchar_t> mName;
    int mField20;

    gcString(cBase *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

template <class T> T *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_00099900;

static inline int *hold_ptr(int *p) {
    __asm__ volatile("" : "+r"(p));
    return p;
}

// ── gcString::AssignCopy(const cBase *) @ 0x0023a890 ──
void gcString::AssignCopy(const cBase *base) {
    gcString *src = dcast<gcString>(base);
    int v08 = *hold_ptr(&src->mField08);
    *hold_ptr(&mField08) = v08;
    int v0C = *hold_ptr(&src->mField0C);
    *hold_ptr(&mField0C) = v0C;
    int *srcp = hold_ptr(&src->mField10.a);
    int v10 = *srcp;
    int *dstp = hold_ptr(&mField10.a);
    srcp = hold_ptr(&src->mField10.b);
    *dstp = v10;
    int v14 = *srcp;
    *hold_ptr(&mField10.b) = v14;
    mText = src->mText;
    mName = src->mName;
    int v20 = *hold_ptr(&src->mField20);
    *hold_ptr(&mField20) = v20;
}

// ── gcString::New(cMemPool *, cBase *) static @ 0x0023a92c ──
cBase *gcString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcString *result = 0;
    gcString *obj = (gcString *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcString(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcString::GetType(void) const @ 0x0023a9a8 ──
const cType *gcString::GetType(void) const {
    if (!D_00099900) {
        if (!D_000385DC) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099900 = cType::InitializeType(0, 0, 0x87, D_000385DC,
                                           gcString::New, 0, 0, 0);
    }
    return D_00099900;
}
