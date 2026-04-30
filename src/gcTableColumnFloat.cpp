class cBase;
class cMemPool;
class cInStream;
class cFile;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

inline void *operator new(unsigned int, void *p) { return p; }

class cArrayFloat {
public:
    float *mData;
    cArrayFloat &operator=(const cArrayFloat &);
    void Read(cInStream &);
    void Read(class cReadBlock &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const float *);
    void End(void);
};

class cOutStream {
public:
    void Write(int, int, bool);
    void Write(float, bool);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
};

extern char gcTableColumnclassdesc[];
extern char gcTableColumnFloatclassdesc[];
extern cType *D_000385DC;
extern cType *D_0009F478;
extern cType *D_0009F484;

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
extern "C" int cStrFormat(wchar_t *, const wchar_t *, ...);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
gcTableColumnFloat *dcast(const cBase *);

struct gcTableColumn {
    void *mOwner;
    void *mClassDesc;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct gcTableColumnFloat : public gcTableColumn {
    cArrayFloat mValues;

    gcTableColumnFloat(cBase *parent) {
        mClassDesc = gcTableColumnclassdesc;
        mOwner = parent;
        mClassDesc = gcTableColumnFloatclassdesc;
        mValues.mData = 0;
    }

    void Set(int row, float value);
    float Get(int row) const;
    void Get(int row, wchar_t *buf, int bufsize) const;
    void Read(cInStream &stream);
    void AssignCopy(const cBase *other);
    void Set(int row, const wchar_t *text, bool flag);
    int Compare(int row1, int row2) const;
    static cBase *New(cMemPool *pool, cBase *parent);
    const cType *GetType(void) const;
    void Write(cFile &file) const;
    void Write(cOutStream &os) const;
    int Read(cFile &file, cMemPool *pool);
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

const cType *gcTableColumnFloat::GetType(void) const {
    if (D_0009F484 == 0) {
        if (D_0009F478 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F478 = cType::InitializeType(0, 0, 0x241, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F484 = cType::InitializeType(0, 0, 0x244, D_0009F478,
                                           &gcTableColumnFloat::New, 0, 0, 0);
    }
    return D_0009F484;
}

void gcTableColumnFloat::Set(int row, float value) {
    mValues.mData[row] = value;
}

float gcTableColumnFloat::Get(int row) const {
    return mValues.mData[row];
}

void gcTableColumnFloat::Get(int row, wchar_t *buf, int bufsize) const {
    cStrFormat(buf, (const wchar_t *)0x36DF74, mValues.mData[row]);
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

void gcTableColumnFloat::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcTableColumn::Write(file);
    wb.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0);
    wb.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0,
             mValues.mData);
    wb.End();
}

void gcTableColumnFloat::Write(cOutStream &os) const {
    os.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0,
             0x20, true);
    int i = 0;
    while (i < (mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0)) {
        os.Write(mValues.mData[i], true);
        i++;
    }
}

int gcTableColumnFloat::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || gcTableColumn::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    mValues.Read(rb);
    return 1;
}
