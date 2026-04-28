// eSurface.cpp - decompiled from eAll_psp.obj.
//
// Functions:
//   0x00053774 eSurface::Write(cFile &) const  184B
//   0x00053bc0 eSurface::~eSurface(void)       124B

class cBase;
class cFile;
class cMemPool;

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
    void Write(cFile &) const;

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

#pragma control sched=1

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
