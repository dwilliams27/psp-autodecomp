// gcRoomInstanceGroup — inherits cGroup. Decompiled functions:
//   0x000d2c20 gcRoomInstanceGroup::Write(cFile &) const
//   0x00238534 gcRoomInstanceGroup::IsManagedTypeExternal(void) const
//   0x00238550 gcRoomInstanceGroup::~gcRoomInstanceGroup(void)
// Pattern mirrors gcTableTemplateGroup.cpp / eTextureGroup.cpp.

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cGroup {
public:
    cBase *m_parent;       // 0x00
    void *m_classdesc;     // 0x04

    ~cGroup();
    void Write(cFile &) const;
};

class gcRoomInstanceGroup : public cGroup {
public:
    void Write(cFile &) const;
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
    ~gcRoomInstanceGroup();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char gcRoomInstanceGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998E0;

// ============================================================
// 0x0023839c — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcRoomInstanceGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcRoomInstanceGroup *result = 0;
    gcRoomInstanceGroup *obj =
        (gcRoomInstanceGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcRoomInstanceGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00238458 — GetType(void) const
// ============================================================
const cType *gcRoomInstanceGroup::GetType() const {
    if (D_000998E0 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998E0 = cType::InitializeType(0, 0, 0x221, D_00040C94,
                                           &gcRoomInstanceGroup::New,
                                           0, 0, 8);
    }
    return D_000998E0;
}

// ============================================================
// 0x000d2c20 — Write(cFile &) const
// ============================================================
void gcRoomInstanceGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ============================================================
// 0x00238534 — IsManagedTypeExternal(void) const
// ============================================================
bool gcRoomInstanceGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ============================================================
// 0x00238550 — ~gcRoomInstanceGroup(void)
// ============================================================
gcRoomInstanceGroup::~gcRoomInstanceGroup() {
    *(void **)((char *)this + 4) = (void *)0x386990;
}
