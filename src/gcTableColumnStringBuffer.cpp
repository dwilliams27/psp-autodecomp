class cBase;
class cMemPool;

class cArrayBase_cArray_wchar_t {
public:
    wchar_t **mData;
    cArrayBase_cArray_wchar_t &operator=(const cArrayBase_cArray_wchar_t &);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                  cBase *(*)(cMemPool *, cBase *),
                                  const char *, const char *, unsigned int);
};

struct gcTableColumnStringBuffer;
int cStrCompare(const wchar_t *, const wchar_t *, bool);
int cStrCopy(wchar_t *, const wchar_t *, int);
gcTableColumnStringBuffer *dcast(const cBase *);

struct gcTableColumnStringBuffer {
    char _pad[0x08];
    int mMaxLen;
    cArrayBase_cArray_wchar_t mValues;

    void Set(int, float);
    void AssignCopy(const cBase *other);
    int Compare(int row1, int row2) const;
    void Get(int row, wchar_t *buf, int bufsize) const;
    static cBase *New(cMemPool *pool, cBase *parent);
    const cType *GetType(void) const;
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

extern const char gcTableColumnStringBuffer_base_name[];
extern const char gcTableColumnStringBuffer_base_desc[];

static cType *type_base;
static cType *type_intermediate;
static cType *type_gcTableColumnStringBuffer;

// 0x00272dc8, 8B
void gcTableColumnStringBuffer::Set(int, float) {
}

// 0x00272bf0, 60B
void gcTableColumnStringBuffer::AssignCopy(const cBase *other) {
    gcTableColumnStringBuffer *src = dcast(other);
    mMaxLen = src->mMaxLen;
    mValues = src->mValues;
}

// 0x00272e1c, 64B
int gcTableColumnStringBuffer::Compare(int row1, int row2) const {
    wchar_t **data = mValues.mData;
    return cStrCompare(data[row1], data[row2], true);
}

// 0x00272dd0, 76B
void gcTableColumnStringBuffer::Get(int row, wchar_t *buf, int bufsize) const {
    wchar_t *src = mValues.mData[row];
    int len = mMaxLen;
    if (bufsize < len) len = bufsize;
    cStrCopy(buf, src, len);
}

// 0x00272c2c, 140B
cBase *gcTableColumnStringBuffer::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableColumnStringBuffer *result = 0;
    gcTableColumnStringBuffer *obj = (gcTableColumnStringBuffer *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37e6a8;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = 0x389dc0;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00272cb8, 220B
const cType *gcTableColumnStringBuffer::GetType(void) const {
    if (!type_gcTableColumnStringBuffer) {
        if (!type_intermediate) {
            if (!type_base) {
                type_base = cType::InitializeType(gcTableColumnStringBuffer_base_name,
                                                   gcTableColumnStringBuffer_base_desc,
                                                   1, 0, 0, 0, 0, 0);
            }
            type_intermediate = cType::InitializeType(0, 0, 0x241, type_base, 0, 0, 0, 0);
        }
        type_gcTableColumnStringBuffer = cType::InitializeType(0, 0, 0x246, type_intermediate,
                                                                gcTableColumnStringBuffer::New,
                                                                0, 0, 0);
    }
    return type_gcTableColumnStringBuffer;
}
