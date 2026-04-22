class cBase;
class cFile;
class cMemPool;

extern char eCameraEffectGeomvirtualtable[];
extern char eGeomvirtualtable[];

extern "C" void eGeom___dtor_eGeom_void(void *, int);
extern "C" void eGeom_eGeom(void *, cBase *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);
void *cMemPool_GetPoolFromPtr(const void *);

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                  const cType *, cBase *(*)(cMemPool *, cBase *),
                                  const char *, const char *, unsigned int);
};

extern const char eCameraEffectGeom_base_name[];
extern const char eCameraEffectGeom_base_desc[];

static cType *type_eCameraEffectGeom_base_base;  // 0x385DC
static cType *type_eCameraEffectGeom_base;       // 0x40FF4
static cType *type_eCameraEffectGeom;            // 0x41004

class eGeom {
public:
    eGeom(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eCameraEffectGeom : public eGeom {
public:
    eCameraEffectGeom(cBase *parent);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static eCameraEffectGeom *New(cMemPool *pool, cBase *parent);
};

// ── eCameraEffectGeom::Write ── 0x0002d148
void eCameraEffectGeom::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eGeom::Write(file);
    wb.End();
}

// ── eCameraEffectGeom::Read ── 0x0002d194
int eCameraEffectGeom::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && eGeom::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eCameraEffectGeom::New ── 0x001e2ef4
eCameraEffectGeom *eCameraEffectGeom::New(cMemPool *pool, cBase *parent) {
    eCameraEffectGeom *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eCameraEffectGeom *obj = (eCameraEffectGeom *)entry->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        eGeom_eGeom(obj, parent);
        *(void **)((char *)obj + 4) = eCameraEffectGeomvirtualtable;
        result = obj;
    }
    return result;
}

// ── eCameraEffectGeom::GetType ── 0x001e2f7c
const cType *eCameraEffectGeom::GetType(void) const {
    if (!type_eCameraEffectGeom) {
        if (!type_eCameraEffectGeom_base) {
            if (!type_eCameraEffectGeom_base_base) {
                type_eCameraEffectGeom_base_base = cType::InitializeType(
                    eCameraEffectGeom_base_name, eCameraEffectGeom_base_desc, 1, 0, 0, 0, 0, 0);
            }
            type_eCameraEffectGeom_base = cType::InitializeType(
                0, 0, 0x16, type_eCameraEffectGeom_base_base, 0, 0, 0, 0);
        }
        type_eCameraEffectGeom = cType::InitializeType(
            0, 0, 0x1B9, type_eCameraEffectGeom_base,
            (cBase *(*)(cMemPool *, cBase *))&eCameraEffectGeom::New, 0, 0, 0);
    }
    return type_eCameraEffectGeom;
}

// ── eCameraEffectGeom::~eCameraEffectGeom ── 0x001e3058
extern "C" void eCameraEffectGeom___dtor_eCameraEffectGeom_void(eCameraEffectGeom *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eCameraEffectGeomvirtualtable;
        eGeom___dtor_eGeom_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            void *base = (char *)block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, self);
        }
    }
}
