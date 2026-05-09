// gcStreamedCinematicConfig: config for a streamed cinematic.
// 32-byte struct: parent, classdesc, two cGUIDs, a cHandle, two bools.

class cBase;
class cFile;
class cFileHandle;
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
    static void Read(cFileHandle *, void *, unsigned int);
};

class cGUID {
public:
    int m0;
    int m4;
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

template <class T>
class cGUIDT {
public:
    int m0;
    int m4;
};

class cHandle {
public:
    int m_val;
    void Read(cReadBlock &, cMemPool *);
    void Write(cWriteBlock &) const;
};

void *cMemPool_GetPoolFromPtr(void *);
void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class gcStreamedCinematic {
public:
    static void Delete(cBase *);
};

extern char gcStreamedCinematicConfigclassdesc[];          // at 0x387750
extern char cBaseclassdesc[];                              // at 0x37E6A8

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

void gcStreamedCinematicConfig_ctor(void *, cBase *);

class gcStreamedCinematicConfig {
public:
    gcStreamedCinematicConfig(cBase *);
    ~gcStreamedCinematicConfig();
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    }
};

gcStreamedCinematicConfig *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_00099AD8;

// ── Constructor ──

gcStreamedCinematicConfig::gcStreamedCinematicConfig(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((void **)this)[1] = gcStreamedCinematicConfigclassdesc;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x10) = 0;
    *(int *)((char *)this + 0x14) = 0;
    *(int *)((char *)this + 0x18) = 0;
    *(char *)((char *)this + 0x1C) = 0;
    *(char *)((char *)this + 0x1D) = 0;
}

// ── Write ──

void gcStreamedCinematicConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    ((cGUID *)((char *)this + 0x08))->Write(wb);
    ((cHandle *)((char *)this + 0x18))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x1C));
    ((cGUID *)((char *)this + 0x10))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x1D));
    wb.End();
}

// ── Read ──

int gcStreamedCinematicConfig::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    char enabled;
    char loop;

    if (rb[3] != 3) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    ((cGUID *)((char *)this + 0x08))->Read(*(cReadBlock *)rb);
    *(int *)((char *)this + 0x18) = 0;
    {
        cHandle *handle = (cHandle *)((char *)this + 0x18);
        handle->Read(*(cReadBlock *)rb, (cMemPool *)cMemPool_GetPoolFromPtr(handle));
    }
    cFileSystem::Read(*(cFileHandle **)rb[0], &enabled, 1);
    *(unsigned char *)((char *)this + 0x1C) = (enabled != 0);
    ((cGUID *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], &loop, 1);
    *(unsigned char *)((char *)this + 0x1D) = (loop != 0);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ── AssignCopy ──

void gcStreamedCinematicConfig::AssignCopy(const cBase *base) {
    const gcStreamedCinematicConfig *src = dcast(base);
    const cGUID *srcGuid0 = (const cGUID *)((const char *)src + 0x08);
    cGUID *dstGuid0 = (cGUID *)((char *)this + 0x08);
    int value8 = srcGuid0->m0;
    int valueC = srcGuid0->m4;
    dstGuid0->m0 = value8;
    dstGuid0->m4 = valueC;
    *(cGUID *)((char *)this + 0x10) = *(const cGUID *)((const char *)src + 0x10);
    *(cHandle *)((char *)this + 0x18) = *(const cHandle *)((const char *)src + 0x18);
    *(unsigned char *)((char *)this + 0x1C) = *(const unsigned char *)((const char *)src + 0x1C);
    *(unsigned char *)((char *)this + 0x1D) = *(const unsigned char *)((const char *)src + 0x1D);
}

// ── New ──

cBase *gcStreamedCinematicConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStreamedCinematicConfig *result = 0;
    gcStreamedCinematicConfig *obj =
        (gcStreamedCinematicConfig *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcStreamedCinematicConfig_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ──

const cType *gcStreamedCinematicConfig::GetType(void) const {
    if (D_00099AD8 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AD8 = cType::InitializeType(0, 0, 0x1BE, D_000385DC,
                                           &gcStreamedCinematicConfig::New, 0, 0, 0);
    }
    return D_00099AD8;
}

// ── Destructor ──

gcStreamedCinematicConfig::~gcStreamedCinematicConfig() {
    *(void **)((char *)this + 4) = gcStreamedCinematicConfigclassdesc;
    gcStreamedCinematic::Delete((cBase *)this);
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
