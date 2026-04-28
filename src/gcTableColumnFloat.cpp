class cBase;
class cMemPool;
class cInStream;

inline void *operator new(unsigned int, void *p) { return p; }

class cArrayFloat {
public:
    float *mData;
    cArrayFloat &operator=(const cArrayFloat &);
    void Read(cInStream &);
};

extern char gcTableColumnclassdesc[];
extern char gcTableColumnFloatclassdesc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct gcTableColumnFloat;
float cAtoF(const wchar_t *);
gcTableColumnFloat *dcast(const cBase *);

struct gcTableColumnFloat {
    void *mOwner;
    void *mClassDesc;
    cArrayFloat mValues;

    gcTableColumnFloat(cBase *parent) {
        mClassDesc = gcTableColumnclassdesc;
        mOwner = parent;
        mClassDesc = gcTableColumnFloatclassdesc;
        mValues.mData = 0;
    }

    void Set(int row, float value);
    float Get(int row) const;
    void Read(cInStream &stream);
    void AssignCopy(const cBase *other);
    void Set(int row, const wchar_t *text, bool flag);
    int Compare(int row1, int row2) const;
    static cBase *New(cMemPool *pool, cBase *parent);
};

cBase *gcTableColumnFloat::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableColumnFloat *result = 0;
    gcTableColumnFloat *obj = (gcTableColumnFloat *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcTableColumnFloat(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcTableColumnFloat::Set(int row, float value) {
    mValues.mData[row] = value;
}

float gcTableColumnFloat::Get(int row) const {
    return mValues.mData[row];
}

void gcTableColumnFloat::Read(cInStream &stream) {
    mValues.Read(stream);
}

void gcTableColumnFloat::AssignCopy(const cBase *other) {
    gcTableColumnFloat *src = dcast(other);
    cArrayFloat &srcArr = *(cArrayFloat *)((char *)src + 8);
    ((cArrayFloat *)((char *)this + 8))->operator=(srcArr);
}

void gcTableColumnFloat::Set(int row, const wchar_t *text, bool flag) {
    float val = cAtoF(text);
    mValues.mData[row] = val;
}

int gcTableColumnFloat::Compare(int row1, int row2) const {
    float a = mValues.mData[row1];
    float b = mValues.mData[row2];
    int v = -1;
    if (!(a < b)) {
        v = (a <= b) ? 0 : 1;
    }
    return v;
}
