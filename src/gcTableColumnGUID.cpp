class cBase;
class cFile;
class cInStream;
class cMemPool;
class cWriteBlock;

struct cGUID {
    int mA;
    int mB;

    void Write(cWriteBlock &) const;
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

struct gcTableColumn {
    void *mOwner;
    void *mClassDesc;

    void Write(cFile &) const;
};

extern cType *D_000385DC;
extern cType *D_0009F478;
extern cType *D_0009F490;

struct gcTableColumnGUID : public gcTableColumn {
    cArrayBase_cGUID mValues;

    void AssignCopy(const cBase *other);
    float Get(int row) const;
    void Get(int row, wchar_t *buf, int bufsize) const;
    void Set(int row, const wchar_t *text, bool flag);
    static cBase *New(cMemPool *pool, cBase *parent);
    const cType *GetType(void) const;
    void Write(cFile &file) const;
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

// 0x0027354c, 220B
const cType *gcTableColumnGUID::GetType(void) const {
    if (D_0009F490 == 0) {
        if (D_0009F478 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F478 = cType::InitializeType(0, 0, 0x241, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F490 = cType::InitializeType(0, 0, 0x247, D_0009F478,
                                           &gcTableColumnGUID::New, 0, 0, 0);
    }
    return D_0009F490;
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

// 0x0012b750, 196B
void gcTableColumnGUID::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcTableColumn::Write(file);

    {
        int firstCount = (unsigned char *)mValues.mData
                             ? (((int *)(unsigned char *)mValues.mData)[-1] &
                                0x3FFFFFFF)
                             : 0;
        wb.Write(firstCount);
    }

    __asm__ volatile("" ::: "memory");
    int i = 0;
    void *data = (unsigned char *)mValues.mData;
    int count = 0;
    if (data != 0) {
        count = ((int *)data)[-1] & 0x3FFFFFFF;
    }
    if (i < count) {
        void *p = data;
        do {
            ((cGUID *)p)->Write(wb);
            i++;
            p = (char *)p + 8;
        } while (i < count);
    }

    wb.End();
}
