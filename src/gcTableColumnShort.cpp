class cBase;
class cFile;
class cInStream;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cOutStream;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
};

class cArrayShort {
public:
    short *mData;
    cArrayShort &operator=(const cArrayShort &);
    void Read(cInStream &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const short *);
    void End(void);
};

class cOutStream {
public:
    void Write(int, int, bool);
};

struct gcTableColumnShort;
int cAtoI(const wchar_t *);
int cStrFormat(wchar_t *, const wchar_t *, ...);
gcTableColumnShort *dcast(const cBase *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void voidcArray_short___Read_cReadBlockref__001D9B0C(cArrayShort *,
                                                                cReadBlock *);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct gcTableColumn {
    void *mOwner;
    void *mClassDesc;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern cType *D_000385DC;
extern cType *D_0009F478;
extern cType *D_0009F480;

struct gcTableColumnShort : public gcTableColumn {
    cArrayShort mValues;

    ~gcTableColumnShort(void);
    static void operator delete(void *);
    void Read(cInStream &stream);
    void AssignCopy(const cBase *other);
    void Get(int row, wchar_t *buf, int bufsize) const;
    void Set(int row, const wchar_t *text, bool flag);
    int Compare(int row1, int row2) const;
    static cBase *New(cMemPool *pool, cBase *parent);
    const cType *GetType(void) const;
    void Write(cFile &file) const;
    void Write(cOutStream &os) const;
    int Read(cFile &file, cMemPool *pool);
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

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void gcTableColumnShort::operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSgcTableColumnShortdtv, 0x120\n");

// 0x002719f0, 288B
gcTableColumnShort::~gcTableColumnShort(void) {
    *(char **)((char *)this + 4) = (char *)0x389B50;
    char *slot = (char *)this + 8;
    if (slot != 0) {
        char *data = *(char **)((char *)this + 8);
        int count = 0;
        if (data != 0) {
            count = ((int *)data)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (data != 0) {
            data -= 4;
            if (data != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(data);
                char *block = ((char **)pool)[9];
                DtorDeleteRecord *rec =
                    (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
                short off = rec->offset;
                void (*fn)(void *, void *) = rec->fn;
                fn(block + off, data);
            }
            *(int *)((char *)this + 8) = 0;
        }
    }
    if (this != 0) {
        *(char **)((char *)this + 4) = (char *)0x37E6A8;
    }
}

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

// 0x002715c0, 220B
const cType *gcTableColumnShort::GetType(void) const {
    if (D_0009F480 == 0) {
        if (D_0009F478 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F478 = cType::InitializeType(0, 0, 0x241, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F480 = cType::InitializeType(0, 0, 0x243, D_0009F478,
                                           &gcTableColumnShort::New, 0, 0, 0);
    }
    return D_0009F480;
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

// 0x0012ad9c, 152B
void gcTableColumnShort::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcTableColumn::Write(file);
    wb.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0);
    wb.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0,
             mValues.mData);
    wb.End();
}

// 0x0012ae34, 200B
int gcTableColumnShort::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1U || gcTableColumn::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    voidcArray_short___Read_cReadBlockref__001D9B0C(&mValues, &rb);
    return 1;
}

// 0x00271910, 196B
void gcTableColumnShort::Write(cOutStream &os) const {
    os.Write(mValues.mData ? (((int *)mValues.mData)[-1] & 0x3FFFFFFF) : 0,
             0x20, 1);
    int i = 0;
    short *data = mValues.mData;
    __asm__ volatile("" : : "r"(data));
    int offset = 0;
loop:
    int count = 0;
    if (mValues.mData != 0) {
        count = ((int *)mValues.mData)[-1] & 0x3FFFFFFF;
    }
    if (i < count) {
        bool bitOrder = true;
        int value = *(short *)((char *)mValues.mData + offset);
        value = (short)value;
        os.Write(value, 0x10, bitOrder);
        i++;
        offset += 2;
        goto loop;
    }
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
