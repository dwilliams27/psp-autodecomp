class cBase;
class cFile;
class cMemPool;
class cType;

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

class nwConfigBase {
public:
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern "C" {
    void nwConfigBase__nwConfigBase_cBaseptr(void *self, cBase *parent);
}

extern cType *D_000385DC;
extern cType *D_0009F928;
extern const char D_00371158[];
extern const char D_00371160[];

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

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
        nwConfigBase__nwConfigBase_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
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
