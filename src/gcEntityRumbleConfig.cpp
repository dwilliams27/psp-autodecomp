// gcEntityRumbleConfig: rumble configuration entity.
// 28-byte struct: parent, classdesc, cHandle, cHandle, 3 floats.

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

void *cMemPool_GetPoolFromPtr(const void *);

extern char cBaseclassdesc[];

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

class gcEntityRumbleConfig {
public:
    gcEntityRumbleConfig(cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

gcEntityRumbleConfig *dcast(const cBase *);

// ── Constructor ──

gcEntityRumbleConfig::gcEntityRumbleConfig(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((int *)this)[1] = 0x3893F8;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x0C) = 0;
    *(float *)((char *)this + 0x10) = 10.0f;
    *(float *)((char *)this + 0x14) = 1.0f;
    *(float *)((char *)this + 0x18) = 0.0f;
}

// ── Write ──

void gcEntityRumbleConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.Write(*(int *)((char *)this + 0x08));
    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(float *)((char *)this + 0x10));
    wb.Write(*(float *)((char *)this + 0x14));
    wb.Write(*(float *)((char *)this + 0x18));
    wb.End();
}

// ── AssignCopy ──

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

// ── New ──

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

// ── Destructor ──

extern "C" {

void gcEntityRumbleConfig___dtor_gcEntityRumbleConfig_void(gcEntityRumbleConfig *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = cBaseclassdesc;
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
