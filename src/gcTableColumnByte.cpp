// gcTableColumnByte — byte-typed table column.
//
// Functions matched here:
//   gcTableColumnByte::Get(int) const                     @ 0x002712a0  (gcAll_psp.obj)
//   gcTableColumnByte::Get(int, wchar_t *, int) const     @ 0x002712b8  (gcAll_psp.obj)
//   gcTableColumnByte::Compare(int, int) const            @ 0x002712ec  (gcAll_psp.obj)
//   gcTableColumnByte::Read(cInStream &)                  @ 0x00271318? (gcAll_psp.obj)
//   gcTableColumnByte::Set(int, const wchar_t *, bool)    @ 0x00271248  (gcAll_psp.obj)
//   gcTableColumnByte::Set(int, float)                    @ 0x00271288  (gcAll_psp.obj)
//   gcTableColumnByte::New(cMemPool *, cBase *) static    @ 0x00270f64  (gcAll_psp.obj)
//   gcTableColumnByte::Write(cFile &) const               @ 0x0012ac3c  (gcAll_psp.obj)
//   gcTableColumnByte::Write(cOutStream &) const          @ 0x00271318  (gcAll_psp.obj)
//   gcTableColumnByte::Read(cFile &, cMemPool *)          @ 0x0012acd4  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cInStream;

class cArrayByte {
public:
    unsigned char *mData;
    void Read(cInStream &);
};

extern "C" int cAtoI(const wchar_t *);
extern "C" int cStrFormat(wchar_t *, const wchar_t *, ...);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void cReadBlock_Read_cArrayByte(void *rb, cArrayByte *out);

extern char gcTableColumnclassdesc[];      // 0x37e6a8
extern char gcTableColumnByteclassdesc[];  // 0x389a80

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const unsigned char *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
};

class cOutStream {
public:
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
};

struct gcTableColumn {
    void *mOwner;
    void *mClassDesc;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct gcTableColumnByte : public gcTableColumn {
    cArrayByte mValues;

    gcTableColumnByte(cBase *parent) {
        mClassDesc = gcTableColumnclassdesc;
        mOwner = parent;
        mClassDesc = gcTableColumnByteclassdesc;
        mValues.mData = 0;
    }

    float Get(int row) const;
    int Compare(int row1, int row2) const;
    void Read(cInStream &stream);
    void Get(int row, wchar_t *buf, int bufsize) const;
    void Set(int row, const wchar_t *text, bool flag);
    void Set(int row, float value);
    static cBase *New(cMemPool *pool, cBase *parent);
    void Write(cFile &file) const;
    void Write(cOutStream &os) const;
    int Read(cFile &file, cMemPool *pool);
};

float gcTableColumnByte::Get(int row) const {
    return (float)mValues.mData[row];
}

int gcTableColumnByte::Compare(int row1, int row2) const {
    int result = -1;
    int a = mValues.mData[row1];
    int b = mValues.mData[row2];
    if (a >= b) {
        result = b < a;
    }
    return result;
}

void gcTableColumnByte::Read(cInStream &stream) {
    mValues.Read(stream);
}

void gcTableColumnByte::Get(int row, wchar_t *buf, int bufsize) const {
    cStrFormat(buf, L"%d", mValues.mData[row]);
}

void gcTableColumnByte::Set(int row, const wchar_t *text, bool flag) {
    mValues.mData[row] = (unsigned char)cAtoI(text);
}

// 0x00271288 — store truncated float as a byte at mValues.mData[row].
void gcTableColumnByte::Set(int row, float value) {
    mValues.mData[row] = (unsigned char)(int)value;
}

// 0x00270f64 — pool-allocate a new gcTableColumnByte and inline-construct it.
cBase *gcTableColumnByte::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableColumnByte *result = 0;
    gcTableColumnByte *obj = (gcTableColumnByte *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcTableColumnByte(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x0012ac3c — Write to cFile via cWriteBlock: header + count-prefixed bytes.
void gcTableColumnByte::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcTableColumn::Write(file);
    wb.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0);
    wb.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0,
             mValues.mData);
    wb.End();
}

// 0x00271318 — Write to cOutStream as 32-bit count followed by 8-bit bytes.
void gcTableColumnByte::Write(cOutStream &os) const {
    os.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0,
             0x20, 1);
    int i = 0;
    while (i < (mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0)) {
        unsigned int value = mValues.mData[i] & 0xFF;
        bool bitOrder = true;
        os.Write(value, 8, bitOrder);
        i++;
    }
}

// 0x0012acd4 — Read from cFile via cReadBlock; delegates byte-array read to template.
int gcTableColumnByte::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || gcTableColumn::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    cReadBlock_Read_cArrayByte(&rb, &mValues);
    return 1;
}
