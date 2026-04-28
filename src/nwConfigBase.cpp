class cBase;
class cFile;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned short);
    void Write(int);
    void Write(int, const char *);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class nwConfigBase {
public:
    nwConfigBase(cBase *);
    ~nwConfigBase();

    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

void cStrCopy(char *, const char *, int);

extern "C" nwConfigBase *dcastdcast_nwConfigBaseptr__constcBaseptr__0036B228(const cBase *);

extern cType *D_000385DC;
extern cType *D_0009F928;
extern const char D_00371158[];
extern const char D_00371160[];

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// nwConfigBase::nwConfigBase(cBase *) @ 0x001a4eec
nwConfigBase::nwConfigBase(cBase *parent) {
    *(cBase **)this = parent;
    *(void **)((char *)this + 4) = (void *)0x38D8D8;
    *(int *)((char *)this + 8) = 0x65;
    *(unsigned short *)((char *)this + 0xC) = 0x2694;
    *(int *)((char *)this + 0x10) = 0x4000;
    *(char *)((char *)this + 0x14) = 0;
    *(int *)((char *)this + 0x18) = 0;
    *(int *)((char *)this + 0x1C) = 0x29000;
    *(nwConfigBase **)0x37D94C = this;
    cStrCopy((char *)this + 0x20, (const char *)0x371168, 0x20);
    cStrCopy((char *)this + 0x40, (const char *)0x371170, 0x20);
}

// nwConfigBase::~nwConfigBase(void) @ 0x001a4f7c
nwConfigBase::~nwConfigBase() {
    *(void **)((char *)this + 4) = (void *)0x38D8D8;
    *(nwConfigBase **)0x37D94C = 0;
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// nwConfigBase::Write(cFile &) const @ 0x001a4d00
void nwConfigBase::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    wb.Write(*(const int *)((const char *)this + 8));
    wb.Write(*(const unsigned short *)((const char *)this + 0xC));
    wb.Write(*(const int *)((const char *)this + 0x10));
    wb.Write(*(const bool *)((const char *)this + 0x14));
    wb.Write(*(const int *)((const char *)this + 0x18));
    wb.Write(*(const int *)((const char *)this + 0x1C));
    wb.Write(0x20, (const char *)((const char *)this + 0x20));
    wb.Write(0x20, (const char *)((const char *)this + 0x40));
    wb.End();
}

// nwConfigBase::New(cMemPool *, cBase *) static @ 0x00243f14
cBase *nwConfigBase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    nwConfigBase *result = 0;
    nwConfigBase *obj = (nwConfigBase *)rec->fn(base, 0x60, 4, 0, 0);
    if (obj != 0) {
        new (obj) nwConfigBase(parent);
        result = obj;
    }
    return (cBase *)result;
}

// nwConfigBase::AssignCopy(const cBase *) @ 0x0036b340
void nwConfigBase::AssignCopy(const cBase *src) {
    nwConfigBase *other =
        dcastdcast_nwConfigBaseptr__constcBaseptr__0036B228(src);

    int *self_i = (int *)this;
    int *other_i = (int *)other;

    self_i[2] = other_i[2];
    *(unsigned short *)((char *)this + 0xC) =
        *(unsigned short *)((char *)other + 0xC);
    self_i[4] = other_i[4];
    *(unsigned char *)((char *)this + 0x14) =
        *(unsigned char *)((char *)other + 0x14);
    self_i[6] = other_i[6];
    self_i[7] = other_i[7];

    int i = 0;
    do {
        *((char *)this + (0x20 + i)) = *((char *)other + 0x20 + i);
        i += 1;
    } while (i < 0x20);

    i = 0;
    do {
        *((char *)this + (0x40 + i)) = *((char *)other + 0x40 + i);
        i += 1;
    } while (i < 0x20);
}

// nwConfigBase::GetType(void) const @ 0x0036b3e4
const cType *nwConfigBase::GetType(void) const {
    if (D_0009F928 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType(D_00371158, D_00371160, 1,
                                               0, 0, 0, 0, 0);
        }
        D_0009F928 = cType::InitializeType(0, 0, 0x28C, D_000385DC,
                                           &nwConfigBase::New, 0, 0, 0);
    }
    return D_0009F928;
}
