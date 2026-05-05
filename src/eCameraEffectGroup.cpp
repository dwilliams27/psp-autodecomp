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

class eCameraEffectGroup : public cGroup {
public:
    eCameraEffectGroup(cBase *);
    ~eCameraEffectGroup();
    void Write(cFile &) const;
    const cType *GetType(void) const;
    const cType *GetManagedType(void) const;
    const char *GetDataDirectory(void) const;
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

extern char eCameraEffectGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eCameraEffect {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C94;
extern cType *D_00040E5C;
extern cType *D_00041008;

// ── eCameraEffectGroup::Write(cFile &) const @ 0x000198B0 ──
void eCameraEffectGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eCameraEffectGroup::New(cMemPool *, cBase *) static @ 0x001DE0E4 ──
cBase *eCameraEffectGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eCameraEffectGroup *result = 0;
    eCameraEffectGroup *obj = (eCameraEffectGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eCameraEffectGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eCameraEffectGroup::GetType(void) const @ 0x001DE1A0 ──
const cType *eCameraEffectGroup::GetType(void) const {
    if (D_00040E5C == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_00040E5C = cType::InitializeType(0, 0, 0x1B1, D_00040C94,
                                           &eCameraEffectGroup::New,
                                           0, 0, 8);
    }
    return D_00040E5C;
}

// ── eCameraEffectGroup::GetManagedType(void) const @ 0x000199B8 ──
const cType *eCameraEffectGroup::GetManagedType(void) const {
    if (D_00041008 == 0) {
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
        D_00041008 = cType::InitializeType(
            0, 0, 0x61, D_000385E4, &eCameraEffect::New,
            (const char *)0x36CD94, (const char *)0x36CDA4, 5);
    }
    return D_00041008;
}

// ── eCameraEffectGroup::GetDataDirectory(void) const @ 0x00019AE0 ──
const char *eCameraEffectGroup::GetDataDirectory(void) const {
    if (D_00041008 == 0) {
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
        D_00041008 = cType::InitializeType(
            0, 0, 0x61, D_000385E4, &eCameraEffect::New,
            (const char *)0x36CD94, (const char *)0x36CDA4, 5);
    }
    return (const char *)((int *)D_00041008)[5];
}

// ── eCameraEffectGroup::~eCameraEffectGroup(void) @ 0x001DE298 ──
eCameraEffectGroup::~eCameraEffectGroup() {
    ((void **)this)[1] = eCameraEffectGroupvirtualtable;
}
