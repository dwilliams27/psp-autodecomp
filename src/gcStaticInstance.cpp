class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

class cObject {
public:
    cObject(cBase *);
    ~cObject();
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct TypeDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

class gcStaticInstance : public cObject {
public:
    char _pad8[0x44];
    cHandle mField44;
    cBase *mField48;
    float mField4C;
    bool mField50;

    gcStaticInstance(cBase *);
    ~gcStaticInstance();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcStaticInstanceclassdesc[];

gcStaticInstance::gcStaticInstance(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = gcStaticInstanceclassdesc;
    mField44.mId = 0;
    mField48 = 0;
    mField4C = 1.0f;
    mField50 = false;
}

gcStaticInstance::~gcStaticInstance() {
    *(void **)((char *)this + 4) = gcStaticInstanceclassdesc;
    if (mField48 != 0) {
        char *classdesc = *(char **)((char *)mField48 + 4);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(classdesc + 0x50);
        entry->fn((char *)mField48 + entry->offset, 3);
        mField48 = 0;
    }
    return;
}

void gcStaticInstance::Write(cFile &file) const {
    cWriteBlock wb(file, 0xA);
    cObject::Write(file);
    mField44.Write(wb);
    wb.WriteBase(mField48);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.End();
}

cBase *gcStaticInstance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStaticInstance *result = 0;
    gcStaticInstance *obj = (gcStaticInstance *)rec->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcStaticInstance(parent);
        result = obj;
    }
    return (cBase *)result;
}
