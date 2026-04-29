// gcDesiredEventParams.cpp - decompiled from gcAll_psp.obj.
//
// Functions:
//   0x00128e14 gcDesiredEventParams::Write(cFile &) const
//   0x0026a3a0 gcDesiredEventParams::New(cMemPool *, cBase *) static

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
    void WriteBase(const cBase *);
    void End(void);
};

class cBaseArray {
public:
    int _data[2];
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

inline void *operator new(unsigned int, void *p) { return p; }

class gcDesiredEventParams {
public:
    char _pad[8];
    cBaseArray mParams0;
    cBaseArray mParams1;
    int mEventObject;

    gcDesiredEventParams(cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F45C;

// gcDesiredEventParams::Write(cFile &) const @ 0x00128e14
void gcDesiredEventParams::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    mParams0.Write(wb);
    mParams1.Write(wb);

    int val = mEventObject;
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

// gcDesiredEventParams::New(cMemPool *, cBase *) static @ 0x0026a3a0
cBase *gcDesiredEventParams::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDesiredEventParams *result = 0;
    gcDesiredEventParams *obj = (gcDesiredEventParams *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDesiredEventParams(parent);
        result = obj;
    }
    return (cBase *)result;
}

// gcDesiredEventParams::GetType(void) const @ 0x0026a41c
const cType *gcDesiredEventParams::GetType(void) const {
    if (D_0009F45C == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F45C = cType::InitializeType(0, 0, 0x153, D_000385DC,
                                           &gcDesiredEventParams::New, 0, 0, 0);
    }
    return D_0009F45C;
}
