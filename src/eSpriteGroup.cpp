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

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End();
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock();
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
    int Read(cFile &, cMemPool *);
};

class eSpriteGroup : public cGroup {
public:
    eSpriteGroup(cBase *);
    ~eSpriteGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    const cType *GetManagedType(void) const;
    const char *GetDataDirectory(void) const;
    const char *GetFileExtension(void) const;
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

extern char eSpriteGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C94;
extern cType *D_00040E40;
extern cType *D_00041110;

class eSprite {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

// ── eSpriteGroup::Write(cFile &) const @ 0x00017064 ──
void eSpriteGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eSpriteGroup::Read(cFile &, cMemPool *) @ 0x000170B0 ──
int eSpriteGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1) goto fail;
    if (cGroup::Read(file, pool)) goto succ;
fail:
    ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
    return 0;
succ:
    return result;
}

// ── eSpriteGroup::~eSpriteGroup(void) @ 0x001DD1C0 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup(), and the deleting-tail dispatch through
// operator delete (which itself does pool/block lookup and calls the slot fn).
eSpriteGroup::~eSpriteGroup() {
    ((void **)this)[1] = eSpriteGroupvirtualtable;
}

// ── eSpriteGroup::New(cMemPool *, cBase *) static @ 0x001DD00C ──
cBase *eSpriteGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eSpriteGroup *result = 0;
    eSpriteGroup *obj = (eSpriteGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eSpriteGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eSpriteGroup::GetType(void) const @ 0x001DD0C8 ──
const cType *eSpriteGroup::GetType(void) const {
    if (D_00040E40 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E40 = cType::InitializeType(0, 0, 0x3E, D_00040C94,
                                           &eSpriteGroup::New,
                                           0, 0, 8);
    }
    return D_00040E40;
}

// ── eSpriteGroup::GetManagedType(void) const @ 0x0001716c ──
const cType *eSpriteGroup::GetManagedType(void) const {
    if (D_00041110 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_00041110 = cType::InitializeType(
            0, 0, 0x3D, D_000385E4, &eSprite::New,
            (const char *)0x36CDDC, (const char *)0x36CDE4, 5);
    }
    return D_00041110;
}

// ── eSpriteGroup::GetDataDirectory(void) const @ 0x00017294 ──
const char *eSpriteGroup::GetDataDirectory(void) const {
    if (D_00041110 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_00041110 = cType::InitializeType(
            0, 0, 0x3D, D_000385E4, &eSprite::New,
            (const char *)0x36CDDC, (const char *)0x36CDE4, 5);
    }
    return (const char *)((int *)D_00041110)[5];
}

// ── eSpriteGroup::GetFileExtension(void) const @ 0x000173C0 ──
const char *eSpriteGroup::GetFileExtension(void) const {
    if (D_00041110 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_00041110 = cType::InitializeType(
            0, 0, 0x3D, D_000385E4, &eSprite::New,
            (const char *)0x36CDDC, (const char *)0x36CDE4, 5);
    }
    return (const char *)((int *)D_00041110)[6];
}

// ── eSpriteGroup::IsManagedTypeExternalStatic(void) static @ 0x000174EC ──
bool eSpriteGroup::IsManagedTypeExternalStatic() {
    if (D_00041110 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36CD74, (const char *)0x36CD7C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_00041110 = cType::InitializeType(
            0, 0, 0x3D, D_000385E4, &eSprite::New,
            (const char *)0x36CDDC, (const char *)0x36CDE4, 5);
    }
    int flags = *(int *)D_00041110;
    bool result = false;
    if (flags & 1) result = true;
    return result;
}
