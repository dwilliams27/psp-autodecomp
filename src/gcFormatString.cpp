// Days of Thunder decompilation
//   gcFormatString::AssignCopy(const cBase *)     @ 0x0027c258
//   gcFormatString::Write(cFile &) const          @ 0x0027c43c
//   gcFormatString::GetName(char *) const         @ 0x0027c94c
//   gcValCameraVariable::GetText(char *) const    @ 0x00322cf8

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};
class gcStringTable;
class gcString;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
    const char *GetName(char *, bool, char *) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    cBaseArray &operator=(const cBaseArray &);
    void RemoveAll(void);
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

template <class T> T *dcast(const cBase *);

extern "C" {
    void cStrCat(char *, const char *);
}

void cStrAppend(char *, const char *, ...);

class gcStringValue {
public:
    int _v0;
    int _v4;
    void Write(cFile &) const;
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

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

// ─────────────────────────────────────────────────────────────────────────
// gcFormatString @ 0x0027c258 / 0x0027c43c / 0x0027c94c / 0x0027d058
// ─────────────────────────────────────────────────────────────────────────

class gcFormatString : public gcStringValue {
public:
    cHandlePairT<gcStringTable, cSubHandleT<gcString> > mPair;  // 0x08
    cBaseArray mArray0;                                          // 0x10
    cBaseArray mArray1;                                          // 0x18

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void GetName(char *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    ~gcFormatString();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcFormatStringvirtualtable[];
extern char cBaseclassdesc[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4C4;

void gcFormatString::AssignCopy(const cBase *src) {
    gcFormatString *o = dcast<gcFormatString>(src);
    this->mPair.mHandle = o->mPair.mHandle;
    __asm__ volatile("" ::: "memory");
    cSubHandleT<gcString> subHandle = o->mPair.mSubHandle;
    this->mPair.mSubHandle = subHandle;
    this->mArray0 = o->mArray0;
    this->mArray1 = o->mArray1;
}

cBase *gcFormatString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcFormatString *result = 0;
    gcFormatString *obj = (gcFormatString *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcFormatStringvirtualtable;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((gcFormatString **)obj)[5] = obj;
        ((int *)obj)[6] = 0;
        ((gcFormatString **)obj)[7] = obj;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcFormatString::GetType(void) const {
    if (D_0009F4C4 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC, 0, 0, 0, 0);
        }
        D_0009F4C4 = cType::InitializeType(0, 0, 0xEA, D_0009F454,
                                           gcFormatString::New, 0, 0, 0);
    }
    return D_0009F4C4;
}

void gcFormatString::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((cHandle *)&this->mPair.mHandle)->Write(wb);
    wb.Write((unsigned int)this->mPair.mSubHandle.mIndex);
    this->mArray0.Write(wb);
    this->mArray1.Write(wb);
    wb.End();
}

void gcFormatString::GetName(char *buf) const {
    char local[4096];
    this->mPair.GetName(local, false, (char *)1);
    cStrCat(buf, local);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcFormatString::~gcFormatString() {
    *(void **)((char *)this + 4) = gcFormatStringvirtualtable;
    void *arr1 = (char *)this + 0x18;
    void *arr0 = (char *)this + 0x10;
    if (arr1 != 0) {
        ((cBaseArray *)arr1)->RemoveAll();
    }
    if (arr0 != 0) {
        ((cBaseArray *)arr0)->RemoveAll();
    }
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValCameraVariable::GetText(char *) const @ 0x00322cf8
// ─────────────────────────────────────────────────────────────────────────

class gcDesiredCamera {
public:
    void GetText(char *) const;
};

extern const char gcValCameraVariable_fmt[];   // @ 0x36DCB8
extern const char gcValGetText_text[];         // @ 0x36DAF0

class gcValCameraVariable {
public:
    int _b0;
    int _b4;
    gcDesiredCamera mDesired;  // 0x08
    void GetText(char *) const;
};

void gcValCameraVariable::GetText(char *buf) const {
    this->mDesired.GetText(buf);
    cStrAppend(buf, gcValCameraVariable_fmt, gcValGetText_text);
}
