// gcEntityAttackHelper — gcAll_psp.obj.
// Decompiled functions:
//   0x0010edb8  gcEntityAttackHelper::Write(cFile &) const                 (88B)
//   0x0010ee10  gcEntityAttackHelper::Read(cFile &, cMemPool *)            (200B)
//   0x0025b818  gcEntityAttackHelper::AssignCopy(const cBase *)            (112B)
//   0x0025b888  gcEntityAttackHelper::New(cMemPool *, cBase *) static      (164B)
//   0x0025b92c  gcEntityAttackHelper::GetType(void) const                  (160B)
//   0x0025b9cc  gcEntityAttackHelper::~gcEntityAttackHelper(void)          (100B)

inline void *operator new(unsigned int, void *p) { return p; }

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
    void Write(float);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class cName {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

template <class T> T *dcast(const cBase *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcEntityAttackHelper {
public:
    cBase *m_parent;          // 0x00
    void  *m_vtable;          // 0x04
    char   _name_pad[0x18];   // 0x08 .. 0x1F (cName)
    float  mField20;          // 0x20
    bool   mField24;          // 0x24

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    ~gcEntityAttackHelper();

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcEntityAttackHelper_cBase_vtable[];   // 0x37E6A8
extern char gcEntityAttackHelpervirtualtable[];    // 0x3885D8

extern cType *D_000385DC;
extern cType *D_00099A3F4;

// ============================================================
// 0x0010edb8 — Write(cFile &) const
// ============================================================
void gcEntityAttackHelper::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((cName *)((char *)this + 8))->Write(wb);
    wb.Write(*(const float *)((const char *)this + 0x20));
    wb.Write(*(const bool *)((const char *)this + 0x24));
    wb.End();
}

// ============================================================
// 0x0025b888 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcEntityAttackHelper::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAttackHelper *result = 0;
    gcEntityAttackHelper *obj =
        (gcEntityAttackHelper *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcEntityAttackHelper_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcEntityAttackHelpervirtualtable;
        *(short *)((char *)obj + 0x1C) = 0;
        *(short *)((char *)obj + 0x1E) = 0;
        *(char *)((char *)obj + 0x08) = 0;
        *(float *)((char *)obj + 0x20) = 0.1f;
        *(char *)((char *)obj + 0x24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0025b92c — GetType(void) const
// ============================================================
const cType *gcEntityAttackHelper::GetType(void) const {
    if (D_00099A3F4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099A3F4 = cType::InitializeType(0, 0, 0x145, D_000385DC,
                                            &gcEntityAttackHelper::New,
                                            0, 0, 0);
    }
    return D_00099A3F4;
}

// ============================================================
// 0x0025b9cc — ~gcEntityAttackHelper(void)  (deleting destructor)
// ============================================================
gcEntityAttackHelper::~gcEntityAttackHelper() {
    *(void **)((char *)this + 4) = gcEntityAttackHelper_cBase_vtable;
}

// ============================================================
// 0x0010ee10 — Read(cFile &, cMemPool *)
// ============================================================
int gcEntityAttackHelper::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    if (rb._data[3] != 2) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    ((cName *)((char *)this + 8))->Read(rb);
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, (char *)this + 0x20, 4);
    }
    char b;
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, &b, 1);
    }
    *(unsigned char *)((char *)this + 0x24) = (b != 0) ? 1 : 0;
    return result;
}

// ============================================================
// 0x0025b818 — AssignCopy(const cBase *)
// ============================================================
void gcEntityAttackHelper::AssignCopy(const cBase *base) {
    struct cNameData { int _w[6]; };
    gcEntityAttackHelper *other = dcast<gcEntityAttackHelper>(base);
    cNameData *src = (cNameData *)((char *)other + 8);
    cNameData *dst = (cNameData *)((char *)this + 8);
    *dst = *src;
    *(float *)((char *)this + 0x20) = *(const float *)((char *)other + 0x20);
    *(unsigned char *)((char *)this + 0x24) = *(const unsigned char *)((char *)other + 0x24);
}
