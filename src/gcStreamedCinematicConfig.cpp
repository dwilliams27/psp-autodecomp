// gcStreamedCinematicConfig: config for a streamed cinematic.
// 32-byte struct: parent, classdesc, two cGUIDs, a cHandle, two bools.

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
    void End(void);
};

class cGUID {
public:
    int m0;
    int m4;
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
    void Write(cWriteBlock &) const;
};

void *cMemPool_GetPoolFromPtr(void *);

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
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
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

// ── AssignCopy ──

void gcStreamedCinematicConfig::AssignCopy(const cBase *base) {
    const gcStreamedCinematicConfig *src = dcast(base);
    *(cGUID *)((char *)this + 0x08) = *(const cGUID *)((const char *)src + 0x08);
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

extern "C" {

void gcStreamedCinematicConfig___dtor_gcStreamedCinematicConfig_void(
    gcStreamedCinematicConfig *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcStreamedCinematicConfigclassdesc;
        gcStreamedCinematic::Delete((cBase *)self);
        *(void **)((char *)self + 4) = cBaseclassdesc;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
