class cBase;
class cInStream;
class cMemPool;

struct cGUID {
    int mA;
    int mB;
};

class cArrayBase_cGUID {
public:
    cGUID *mData;
    cArrayBase_cGUID &operator=(const cArrayBase_cGUID &);
};

struct gcTableColumnGUID;
int cStrFormat(char *, const char *, ...);
int cStrCopy(wchar_t *, const char *);
int cStrCopy(char *, const wchar_t *);
int sscanf(const char *, const char *, ...);
gcTableColumnGUID *dcast(const cBase *);

struct gcTableColumnGUID {
    char _pad[0x08];
    cArrayBase_cGUID mValues;

    void AssignCopy(const cBase *other);
    float Get(int row) const;
    void Get(int row, wchar_t *buf, int bufsize) const;
    void Set(int row, const wchar_t *text, bool flag);
    static cBase *New(cMemPool *pool, cBase *parent);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// 0x00273490, 52B
void gcTableColumnGUID::AssignCopy(const cBase *other) {
    gcTableColumnGUID *src = dcast(other);
    cArrayBase_cGUID &srcArr = *(cArrayBase_cGUID *)((char *)src + 8);
    ((cArrayBase_cGUID *)((char *)this + 8))->operator=(srcArr);
}

// 0x00273828, 72B
float gcTableColumnGUID::Get(int row) const {
    cGUID *p = &mValues.mData[row];
    bool b = (p->mA != 0) || (p->mB != 0);
    return b ? 1.0f : 0.0f;
}

// 0x00273870, 80B
void gcTableColumnGUID::Get(int row, wchar_t *buf, int bufsize) const {
    cGUID *p = &mValues.mData[row];
    char tmp[32];
    cStrFormat(tmp, "%08X-%08X", p->mA, p->mB);
    cStrCopy(buf, tmp);
}

// 0x002737c8, 88B
void gcTableColumnGUID::Set(int row, const wchar_t *text, bool flag) {
    char tmp[32];
    cStrCopy(tmp, text);
    cGUID *p = &mValues.mData[row];
    sscanf(tmp, "%08X-%08X", &p->mA, &p->mB);
}

// 0x002734c4, 136B — static, ctor fully inlined (no jal to constructor).
cBase *gcTableColumnGUID::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableColumnGUID *result = 0;
    gcTableColumnGUID *obj = (gcTableColumnGUID *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37e6a8;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = 0x389e90;
        ((int *)obj)[2] = 0;
        result = obj;
    }
    return (cBase *)result;
}
