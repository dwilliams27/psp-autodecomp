// eSurface.cpp - decompiled from eAll_psp.obj.
//
// Functions:
//   0x00053774 eSurface::Write(cFile &) const  184B
//   0x00053bc0 eSurface::~eSurface(void)       124B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    ~cObject();
    void Write(cFile &) const;
};

class eSurface : public cObject {
public:
    unsigned int mField44;  // 0x44
    unsigned int mField48;  // 0x48
    cHandle mHandle4C;      // 0x4C
    unsigned int mField50;  // 0x50
    float mField54;         // 0x54
    float mField58;         // 0x58
    char _pad5C[4];
    float mField60;         // 0x60
    float mField64;         // 0x64
    unsigned int mField68;  // 0x68

    eSurface(cBase *);
    ~eSurface();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eSurfacevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046A18;

#pragma control sched=1

// -- eSurface::GetType(void) const @ 0x001F61D4 --
const cType *eSurface::GetType(void) const {
    if (D_00046A18 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType, factory,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000385E4;
        const char *kindName = (const char *)0x36D000;
        const char *kindDesc = (const char *)0x36D00C;
        __asm__ volatile("" : "+r"(parentType), "+r"(kindName), "+r"(kindDesc));
        D_00046A18 = cType::InitializeType(0, 0, 0x39, parentType, 0,
                                           kindName, kindDesc, 5);
    }
    return D_00046A18;
}

// -- eSurface::eSurface(cBase *) @ 0x00053B48 --
eSurface::eSurface(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eSurfacevirtualtable;
    *(int *)((char *)this + 0x44) = 1;
    *(int *)((char *)this + 0x48) = 0;
    *(volatile int *)((char *)this + 0x4C) = 0;
    *(int *)((char *)this + 0x50) = 0;
    *(float *)((char *)this + 0x54) = 1.0f;
    *(float *)((char *)this + 0x58) = 5.0f;
    float zero = 0.0f;
    *(float *)((char *)this + 0x60) = zero;
    *(float *)((char *)this + 0x64) = zero;
    *(int *)((char *)this + 0x68) = 0xFFA0A0A0;
}

// -- eSurface::Write @ 0x00053774 --
void eSurface::Write(cFile &file) const {
    cWriteBlock wb(file, 0xE);
    cObject::Write(file);
    wb.Write(mField44);
    wb.Write(mField48);
    wb.Write(mField68);
    mHandle4C.Write(wb);
    wb.Write(mField50);
    wb.Write(mField54);
    wb.Write(mField58);
    wb.Write(mField60);
    wb.Write(mField64);
    wb.End();
}

// -- eSurface::~eSurface @ 0x00053BC0 --
eSurface::~eSurface() {
    *(void **)((char *)this + 4) = eSurfacevirtualtable;
}
