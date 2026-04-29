// Batch of unrelated functions grouped by the orchestrator into one TU.
// Sections are independent.

class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

// ── gcValEntityIsLocallyControlled section ──
class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];         // to 0x78
    cTypeMethod text_m;     // 0x78
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityIsLocallyControlled : public gcValue {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
};

extern "C" void cStrCat(char *, const char *);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern const char gcValEntityIsLocallyControlled_text[];   // @ 0x36F334

extern char cBaseclassdesc[];
extern char gcValEntityIsLocallyControlled_vtbl[] __asm__(".L00007728");
extern char D_00000338[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcValEntityIsLocallyControlled::AssignCopy(const cBase *) @ 0x00334A64 ──
void gcValEntityIsLocallyControlled::AssignCopy(const cBase *base) {
    gcValEntityIsLocallyControlled *other = dcast<gcValEntityIsLocallyControlled>(base);
    gcDesiredEntity &srcDesired = *(gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(srcDesired);
}

// ── gcValEntityIsLocallyControlled::New(cMemPool *, cBase *) static @ 0x00334A98 ──
cBase *gcValEntityIsLocallyControlled::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityIsLocallyControlled *result = 0;
    gcValEntityIsLocallyControlled *obj =
        (gcValEntityIsLocallyControlled *)entry->fn(base, 0x34, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = gcValEntityIsLocallyControlled_vtbl;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[3] = D_00000338;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((void **)obj)[3] = (void *)0x388A48;
        ((char **)obj)[8] = cBaseclassdesc;
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)sub | 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValEntityIsLocallyControlled::Write(cFile &) const @ 0x00334CB0 (108B) ──
void gcValEntityIsLocallyControlled::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

// ── gcValEntityIsLocallyControlled::GetText(char *) const @ 0x00334E58 (80B) ──
void gcValEntityIsLocallyControlled::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrCat(buf, gcValEntityIsLocallyControlled_text);
}

// ── gcValStringIndex::AssignCopy(const cBase *) @ 0x0035E880 ──
class gcDesiredString {
public:
    gcDesiredString &operator=(const gcDesiredString &);
};

class gcValStringIndex {
public:
    void AssignCopy(const cBase *);
};

void gcValStringIndex::AssignCopy(const cBase *base) {
    gcValStringIndex *other = dcast<gcValStringIndex>(base);
    gcDesiredString &srcDesired = *(gcDesiredString *)((char *)other + 8);
    ((gcDesiredString *)((char *)this + 8))->operator=(srcDesired);
}

// ── eStaticMesh::PlatformFree(void) @ 0x0004497C ──
class eStaticMeshVisData;

extern "C" void eStaticMeshVisData___dtor_eStaticMeshVisData_void(void *, int);

class eStaticMesh {
public:
    void PlatformFree(void);
};

void eStaticMesh::PlatformFree(void) {
    void *data = *(void **)((char *)this + 0x60);
    if (data != 0) {
        eStaticMeshVisData___dtor_eStaticMeshVisData_void(data, 3);
        *(void **)((char *)this + 0x60) = 0;
    }
}

// ── eWeatherEffect::eWeatherEffect(cBase *) @ 0x000607D8 ──
class eGeom {
public:
    eGeom(cBase *);
};

class eWeatherEffect : public eGeom {
public:
    eWeatherEffect(cBase *);
};

extern char eWeatherEffectvirtualtable[];

eWeatherEffect::eWeatherEffect(cBase *parent) : eGeom(parent) {
    *(void **)((char *)this + 4) = eWeatherEffectvirtualtable;
    *(int *)((char *)this + 0x90) = 0;
}

// ── eFontGroup::AssignCopy(const cBase *) @ 0x001DD23C ──
class eFontGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void eFontGroup::AssignCopy(const cBase *base) {
    eFontGroup *src = dcast<eFontGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}
