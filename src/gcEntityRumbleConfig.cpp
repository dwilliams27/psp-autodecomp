// gcEntityRumbleConfig: rumble configuration entity.
// 28-byte struct: parent, classdesc, cHandle, cHandle, 3 floats.
//
// Functions:
//   gcEntityRumbleConfig::Write(cFile &) const           @ 0x00125e58
//   gcEntityRumbleConfig::Read(cFile &, cMemPool *)      @ 0x00125ec8
//   gcEntityRumbleConfig::gcEntityRumbleConfig(cBase *)  @ 0x00125fb0
//   gcEntityRumbleConfig::AssignCopy(const cBase *)      @ 0x00267f4c
//   gcEntityRumbleConfig::New(cMemPool *, cBase *)       @ 0x00267fac
//   gcEntityRumbleConfig::GetType(void) const            @ 0x00268028
//   gcEntityRumbleConfig::~gcEntityRumbleConfig(void)    @ 0x002680c8

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;
struct cFileHandle;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
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
    static int Read(cFileHandle *, void *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcEntityRumbleConfigPoolNS {
public:
    static gcEntityRumbleConfigPoolNS *GetPoolFromPtr(const void *);
};

extern char cBaseclassdesc[];
extern const char gcEntityRumbleConfig_type_name[];
extern const char gcEntityRumbleConfig_type_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct cHandle {
    int mIndex;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcEntityRumbleConfig {
public:
    gcEntityRumbleConfig(cBase *);
    ~gcEntityRumbleConfig();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcEntityRumbleConfigPoolNS *pool =
            gcEntityRumbleConfigPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

gcEntityRumbleConfig *dcast(const cBase *);

// ── Constructor ──  @ 0x00125fb0, 64B

gcEntityRumbleConfig::gcEntityRumbleConfig(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((int *)this)[1] = 0x3893F8;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x0C) = 0;
    *(float *)((char *)this + 0x10) = 10.0f;
    *(float *)((char *)this + 0x14) = 1.0f;
    *(float *)((char *)this + 0x18) = 0.0f;
}

// ── Write ──  @ 0x00125e58, 112B

void gcEntityRumbleConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.Write(*(int *)((char *)this + 0x08));
    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(float *)((char *)this + 0x10));
    wb.Write(*(float *)((char *)this + 0x14));
    wb.Write(*(float *)((char *)this + 0x18));
    wb.End();
}

// ── Read ──  @ 0x00125ec8, 232B

int gcEntityRumbleConfig::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x08, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x0C, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x10, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x14, 4);
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x18, 4);
    return result;
}

// ── AssignCopy ──  @ 0x00267f4c, 96B

void gcEntityRumbleConfig::AssignCopy(const cBase *base) {
    gcEntityRumbleConfig *other = dcast(base);
    {
        cHandle *s = (cHandle *)((char *)other + 0x08);
        cHandle *d = (cHandle *)((char *)this + 0x08);
        *d = *s;
    }
    {
        cHandle *s = (cHandle *)((char *)other + 0x0C);
        cHandle *d = (cHandle *)((char *)this + 0x0C);
        *d = *s;
    }
    *(float *)((char *)this + 0x10) = *(float *)((char *)other + 0x10);
    *(float *)((char *)this + 0x14) = *(float *)((char *)other + 0x14);
    *(float *)((char *)this + 0x18) = *(float *)((char *)other + 0x18);
}

// ── New ──  @ 0x00267fac, 124B

cBase *gcEntityRumbleConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityRumbleConfig *result = 0;
    gcEntityRumbleConfig *obj =
        (gcEntityRumbleConfig *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEntityRumbleConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ──  @ 0x00268028, 160B

static cType *type_base;
static cType *type_gcEntityRumbleConfig;

const cType *gcEntityRumbleConfig::GetType(void) const {
    if (!type_gcEntityRumbleConfig) {
        if (!type_base) {
            type_base = cType::InitializeType(
                gcEntityRumbleConfig_type_name,
                gcEntityRumbleConfig_type_desc, 1,
                0, 0, 0, 0, 0);
        }
        type_gcEntityRumbleConfig = cType::InitializeType(
            0, 0, 0xA2, type_base, &gcEntityRumbleConfig::New,
            0, 0, 0);
    }
    return type_gcEntityRumbleConfig;
}

// ── Destructor ──  @ 0x002680c8, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain (cBase has none), and the deleting-
// tail dispatch through operator delete on (flag & 1). Body just resets the
// classdesc pointer at offset 4 to the parent (cBase) classdesc.
gcEntityRumbleConfig::~gcEntityRumbleConfig() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
