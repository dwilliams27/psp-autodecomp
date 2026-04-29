// eMesh.cpp — decompiled from eAll_psp.obj.
// Parent: cObject. Vtable at 0x380E38 (eMeshvirtualtable).
//
// Functions:
//   0x00043d70 eMesh::Write(cFile &) const  148B
//   0x00043f74 eMesh::~eMesh(void)          124B

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
    void Write(bool);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eMesh : public cObject {
public:
    int mField44;            // 0x44
    unsigned int mField48;   // 0x48
    float mField4C;          // 0x4C
    float mField50;          // 0x50
    float mField54;          // 0x54
    bool mField58;           // 0x58

    eMesh(cBase *);
    ~eMesh();
    const cType *GetType(void) const;
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char eMeshvirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469B8;

// ── eMesh::Write @ 0x00043d70 ──
void eMesh::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    cObject::Write(file);
    wb.Write(mField44);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.Write(mField54);
    wb.Write(mField58);
    wb.Write(mField48);
    wb.End();
}

// ── eMesh::~eMesh @ 0x00043f74 ──
// SNC auto-generates: (this != 0) guard, ~cObject(0) base call, and the
// `if (flags & 1) operator delete(this)` deleting tail (inlined).
eMesh::~eMesh() {
    *(void **)((char *)this + 4) = eMeshvirtualtable;
}

// ── eMesh::GetType(void) const @ 0x001ED500 ──
const cType *eMesh::GetType(void) const {
    if (D_000469B8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))&cNamed::New,
                    0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000469B8 = cType::InitializeType(0, 0, 0xE, D_000385E4, 0,
                                           (const char *)0x36CE40,
                                           (const char *)0x36CE48, 1);
    }
    return D_000469B8;
}
