class cBase;
class cInStream;
class cMemPool;

class cArrayShort {
public:
    short *mData;
    cArrayShort &operator=(const cArrayShort &);
    void Read(cInStream &);
};

struct gcTableColumnShort;
int cAtoI(const wchar_t *);
int cStrFormat(wchar_t *, const wchar_t *, ...);
gcTableColumnShort *dcast(const cBase *);

struct gcTableColumnShort {
    char _pad[0x08];
    cArrayShort mValues;

    void Read(cInStream &stream);
    void AssignCopy(const cBase *other);
    void Get(int row, wchar_t *buf, int bufsize) const;
    void Set(int row, const wchar_t *text, bool flag);
    int Compare(int row1, int row2) const;
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

// 0x002719d4, 28B
void gcTableColumnShort::Read(cInStream &stream) {
    mValues.Read(stream);
}

// 0x00271504, 52B
void gcTableColumnShort::AssignCopy(const cBase *other) {
    gcTableColumnShort *src = dcast(other);
    cArrayShort &srcArr = *(cArrayShort *)((char *)src + 8);
    ((cArrayShort *)((char *)this + 8))->operator=(srcArr);
}

// 0x002718a4, 56B
void gcTableColumnShort::Get(int row, wchar_t *buf, int bufsize) const {
    cStrFormat(buf, L"%d", mValues.mData[row]);
}

// 0x00271828, 68B
void gcTableColumnShort::Set(int row, const wchar_t *text, bool flag) {
    mValues.mData[row] = (short)cAtoI(text);
}

// 0x002718dc, 52B
int gcTableColumnShort::Compare(int row1, int row2) const {
    short *data = mValues.mData;
    int result = -1;
    if (data[row1] >= data[row2]) {
        result = data[row2] < data[row1];
    }
    return result;
}

// 0x00271538, 136B — static, ctor fully inlined (no jal to constructor).
cBase *gcTableColumnShort::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableColumnShort *result = 0;
    gcTableColumnShort *obj = (gcTableColumnShort *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37e6a8;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = 0x389b50;
        ((int *)obj)[2] = 0;
        result = obj;
    }
    return (cBase *)result;
}
