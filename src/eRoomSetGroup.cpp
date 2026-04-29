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
    void End();
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

class eRoomSetGroup : public cGroup {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    ~eRoomSetGroup();
    static bool IsManagedTypeExternalStatic();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char eRoomSetGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E6C;

// ── eRoomSetGroup::Write(cFile &) const @ 0x0001AFC0 ──
void eRoomSetGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eRoomSetGroup::New(cMemPool *, cBase *) static @ 0x001DEA84 ──
cBase *eRoomSetGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eRoomSetGroup *result = 0;
    eRoomSetGroup *obj = (eRoomSetGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eRoomSetGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eRoomSetGroup::GetType(void) const @ 0x001DEB40 ──
const cType *eRoomSetGroup::GetType(void) const {
    if (D_00040E6C == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E6C = cType::InitializeType(0, 0, 0x223, D_00040C94,
                                           &eRoomSetGroup::New,
                                           0, 0, 8);
    }
    return D_00040E6C;
}

// ── eRoomSetGroup::~eRoomSetGroup(void) @ 0x001DEC38 ──
// SNC auto-generates the deleting destructor (D0) from this canonical body
// plus inheritance from cGroup and the inline `operator delete` declared
// above: vtable write, chain to ~cGroup(), delete-flag check, pool dispatch.
eRoomSetGroup::~eRoomSetGroup() {
    ((void **)this)[1] = eRoomSetGroupvirtualtable;
}
