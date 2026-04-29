// eSurfaceGroup methods: Write, ~eSurfaceGroup, New
//
// Patterned after eMaterialGroup.cpp (same class hierarchy: cBase -> cGroup ->
// eXxxGroup) with the canonical-dtor pattern from eSphereShape_dtor.cpp so SNC
// auto-generates the (this != 0) guard, the chain call to ~cGroup(), and the
// deleting-tail dispatch through operator delete.

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cGroup {
public:
    cBase *m_parent;        // 0x00
    void *m_vtbl;           // 0x04
    unsigned char mFlag;    // 0x08
    char _pad[3];
    int mField;             // 0x0C
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
};

class eSurfaceGroup : public cGroup {
public:
    eSurfaceGroup(cBase *);
    ~eSurfaceGroup();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char eSurfaceGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E38;

// ── eSurfaceGroup::Write(cFile &) const @ 0x000164EC ──
void eSurfaceGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eSurfaceGroup::~eSurfaceGroup(void) @ 0x001DCCF0 ──
eSurfaceGroup::~eSurfaceGroup() {
    ((void **)this)[1] = eSurfaceGroupvirtualtable;
}

// ── eSurfaceGroup::New(cMemPool *, cBase *) static @ 0x001DCB3C ──
cBase *eSurfaceGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSurfaceGroup *result = 0;
    eSurfaceGroup *obj = (eSurfaceGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eSurfaceGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSurfaceGroup::GetType(void) const @ 0x001DCBF8 ──
const cType *eSurfaceGroup::GetType(void) const {
    if (D_00040E38 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E38 = cType::InitializeType(0, 0, 0x3A, D_00040C94,
                                           &eSurfaceGroup::New,
                                           0, 0, 8);
    }
    return D_00040E38;
}
