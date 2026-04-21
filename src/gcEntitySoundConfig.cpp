// gcEntitySoundConfig: sound config entity.
// 44-byte struct: parent, classdesc, cHandle, cName, int, u16.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned short);
    void End(void);
};

class cHandle {
public:
    int m_val;
    void Write(cWriteBlock &) const;
};

class cName {
public:
    int _data[6];
    void Write(cWriteBlock &) const;
};

void *cMemPool_GetPoolFromPtr(const void *);

extern char gcEntityGeomConfigvirtualtable[];
extern char cBaseclassdesc[];

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

void gcEntitySoundConfig_ctor(void *, cBase *);

class gcEntitySoundConfig {
public:
    cBase *mOwner;
    void *mClassDesc;
    cHandle mHandle;
    cName mName;
    cHandle mHandle2;
    unsigned short mField;

    gcEntitySoundConfig(cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

gcEntitySoundConfig *dcast(const cBase *);

// ── Constructor ──

gcEntitySoundConfig::gcEntitySoundConfig(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((int *)this)[1] = 0x389388;
    *(int *)((char *)this + 0x08) = 0;
    *(short *)((char *)this + 0x20) = 0;
    *(short *)((char *)this + 0x22) = 0;
    *(char *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x24) = 0;
    *(short *)((char *)this + 0x28) = 1;
}

// ── Write ──

void gcEntitySoundConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((cHandle *)((char *)this + 0x08))->Write(wb);
    ((cName *)((char *)this + 0x0C))->Write(wb);
    wb.Write(*(int *)((char *)this + 0x24));
    wb.Write(*(unsigned short *)((char *)this + 0x28));
    wb.End();
}

// ── AssignCopy ──

void gcEntitySoundConfig::AssignCopy(const cBase *base) {
    const gcEntitySoundConfig *src = dcast(base);
    mHandle = src->mHandle;
    mName = src->mName;
    mHandle2 = src->mHandle2;
    mField = src->mField;
}

// ── New ──

cBase *gcEntitySoundConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntitySoundConfig *result = 0;
    gcEntitySoundConfig *obj =
        (gcEntitySoundConfig *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcEntitySoundConfig_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── Destructor ──

extern "C" {

void gcEntitySoundConfig___dtor_gcEntitySoundConfig_void(gcEntitySoundConfig *self, int flags) {
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
