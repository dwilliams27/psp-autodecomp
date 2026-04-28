// gcDesiredCamera.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredCamera {
public:
    gcDesiredCamera(cBase *);

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

extern cType *D_000385DC;
extern cType *D_0009F408;

// 0x00121490 - gcDesiredCamera::Write(cFile &) const
void gcDesiredCamera::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.Write(*(int *)((const char *)this + 8));

    char *typeInfo = *(char **)((const char *)this + 16);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (char *)this + 12;
    rec->fn((char *)base + off, *(cFile **)&wb);

    wb.End();
}

// 0x00262AC4 - gcDesiredCamera::New(cMemPool *, cBase *) static
cBase *gcDesiredCamera::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDesiredCamera *result = 0;
    gcDesiredCamera *obj = (gcDesiredCamera *)rec->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDesiredCamera(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x00262B40 - gcDesiredCamera::GetType(void) const
const cType *gcDesiredCamera::GetType(void) const {
    if (D_0009F408 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F408 = cType::InitializeType(0, 0, 0xA6, D_000385DC,
                                           &gcDesiredCamera::New, 0, 0, 0);
    }
    return D_0009F408;
}
