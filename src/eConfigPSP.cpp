class cBase;
class cFile;
class cMemPool;
class cType;

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
    void Write(bool);
    void Write(int);
    void Write(float);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class eConfigBase {
public:
    eConfigBase(cBase *);
    ~eConfigBase();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class eConfigPSP : public eConfigBase {
public:
    eConfigPSP(cBase *);
    ~eConfigPSP();

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern "C" {
    void eConfigPSP__eConfigPSP_cBaseptr(void *self, cBase *parent);
}

extern cType *D_000385DC;
extern cType *D_00040E78;
extern cType *D_00040E80;

// eConfigPSP::eConfigPSP(cBase *) @ 0x0001cdec
eConfigPSP::eConfigPSP(cBase *parent) : eConfigBase(parent) {
    *(void **)((char *)this + 4) = (void *)0x37F930;
    *(int *)((char *)this + 0x6C) = 0xE290;
    *(int *)((char *)this + 0x70) = 0;
    *(int *)((char *)this + 0x74) = 0;
    *(float *)((char *)this + 0x78) = 0.15f;
    *(float *)((char *)this + 0x7C) = 0.0018f;
    *(unsigned char *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x84) = 0xAC44;
    *(int *)((char *)this + 0x88) = 0x80;
    *(int *)((char *)this + 0x8C) = 0x8000;
    *(int *)((char *)this + 0x90) = 0x30000;
}

// eConfigPSP::New(cMemPool *, cBase *) static @ 0x001df84c
cBase *eConfigPSP::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eConfigPSP *result = 0;
    eConfigPSP *obj = (eConfigPSP *)rec->fn(base, 0x94, 4, 0, 0);
    if (obj != 0) {
        eConfigPSP__eConfigPSP_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// eConfigPSP::GetType(void) const @ 0x001df8c8
const cType *eConfigPSP::GetType(void) const {
    if (D_00040E80 == 0) {
        if (D_00040E78 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040E78 = cType::InitializeType(0, 0, 0x28B, D_000385DC,
                                               &eConfigBase::New, 0, 0, 0);
        }
        D_00040E80 = cType::InitializeType(0, 0, 0x295, D_00040E78,
                                           &eConfigPSP::New, 0, 0, 0);
    }
    return D_00040E80;
}

// eConfigPSP::Write(cFile &) const @ 0x0001cb34
void eConfigPSP::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    eConfigBase::Write(file);
    wb.Write(*(const int *)((const char *)this + 0x6C));
    wb.Write(*(const int *)((const char *)this + 0x70));
    wb.Write(*(const int *)((const char *)this + 0x74));
    wb.Write(*(const float *)((const char *)this + 0x78));
    wb.Write(*(const float *)((const char *)this + 0x7C));
    wb.Write(*(const bool *)((const char *)this + 0x80));
    wb.Write(*(const int *)((const char *)this + 0x84));
    wb.Write(*(const int *)((const char *)this + 0x88));
    wb.Write(*(const int *)((const char *)this + 0x8C));
    wb.Write(*(const int *)((const char *)this + 0x90));
    wb.End();
}

// eConfigPSP::~eConfigPSP(void) @ 0x001dfc00
eConfigPSP::~eConfigPSP() {
    *(void **)((char *)this + 4) = (void *)0x37F930;
}
