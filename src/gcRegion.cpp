#include "gcRegion.h"

inline void *operator new(unsigned int, void *p) { return p; }

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcExpressionList {
public:
    float Evaluate(void) const;
};

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

struct TypeDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

extern char gcRegionclassdesc[];   // at 0x387918
extern void *g_gcMap;              // at 0x37D7FC
extern int g_regionRemoveEnabled;  // at 0x37D7B0
extern void *g_expressionEvalStack; // at 0x37D7B4
extern const char gcRegion_base_name[];
extern const char gcRegion_base_desc[];
extern const char gcRegion_gettype_name[];
extern const char gcRegion_gettype_desc[];

void gcRegionBase_MemCardReplicate(void *, gcReplicationVisitor &);
void gcRegionBase_Write(const void *, cFile &);
void gcRegionBase_assignop(void *, const void *);
void gcEvent_assignop(void *, const void *);
void gcMap_OnRegionDeleted(void *, gcRegion *);
void cFactory_DeleteGroups(void *);
void *cMemPool_GetPoolFromPtr(const void *);
gcRegion *dcast(const cBase *);
void gcRegion_ctor(void *, cBase *);
int gcRegionBase_Read(void *, cFile &, cMemPool *);
int gcRegionBase_RemoveFromWorld(void *);
void cFile_SetCurrentPos(void *, unsigned int);

extern "C" {
    void gcEvent___dtor_gcEvent_void(void *, int);
    void gcRegionBase___dtor_gcRegionBase_void(void *, int);
    void *__vec_new(void *, int, int, void (*)(void *));
}

inline void operator delete(void *p) {
    void *pool = cMemPool_GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
    short off = rec->offset;
    rec->fn((char *)block + off, p);
}

static cType *type_cBase;
static cType *type_cNamed;
static cType *type_cObject;
static cType *type_cFactory;
static cType *type_gcRegionBase;
static cType *type_gcRegion;

int gcRegion::AllowParentCreate(void) const {
    return 1;
}

void gcRegion::MemCardReplicate(gcReplicationVisitor &v) {
    gcRegionBase_MemCardReplicate(this, v);
}

void gcRegion::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcRegionBase_Write(this, file);
    {
        char *typePtr = *(char **)((char *)this + 0xAC);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xA8;
        entry->fn(base + entry->offset, wb._data[0]);
    }
    {
        char *typePtr = *(char **)((char *)this + 0xC8);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xC4;
        entry->fn(base + entry->offset, wb._data[0]);
    }
    wb.End();
}

void gcRegion::AssignCopy(const cBase *base) {
    gcRegion *other = dcast(base);
    gcRegionBase_assignop(this, other);
    int i = 0;
    const int *src = (const int *)other;
    int *dst = (int *)this;
    do {
        i++;
        *(int *)((char *)dst + 0x78) = *(const int *)((const char *)src + 0x78);
        src = (const int *)((const char *)src + 4);
        dst = (int *)((char *)dst + 4);
    } while (i < 12);
    gcEvent_assignop((char *)this + 0xA8, (const char *)other + 0xA8);
    gcEvent_assignop((char *)this + 0xC4, (const char *)other + 0xC4);
}

cBase *gcRegion::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcRegion *result = 0;
    gcRegion *obj = (gcRegion *)entry->fn(base, 0xE0, 4, 0, 0);
    if (obj != 0) {
        gcRegion_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

int *gcRegion::GetGroups(int *count) {
    *count = 0xC;
    return (int *)((char *)this + 0x78);
}

gcRegion::~gcRegion(void) {
    *(char **)((char *)this + 4) = gcRegionclassdesc;
    gcMap_OnRegionDeleted(g_gcMap, this);
    cFactory_DeleteGroups(this);
    gcEvent___dtor_gcEvent_void((char *)this + 0xC4, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0xA8, 2);
    gcRegionBase___dtor_gcRegionBase_void(this, 0);
}

int gcRegion::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 4, true);
    if ((unsigned int)rb._data[3] >= 5U || (unsigned int)rb._data[3] < 3U)
        goto fail;
    if (!gcRegionBase_Read(this, file, pool))
        goto fail;
    {
        char *typePtr = *(char **)((char *)this + 0xAC);
        char *base0 = (char *)this + 0xA8;
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x30);
        short off = entry->offset;
        int pos = rb._data[0];
        void *target = base0 + off;
        ((void (*)(void *, int, void *))entry->fn)(target, pos, cMemPool_GetPoolFromPtr(base0));
    }
    if ((unsigned int)rb._data[3] >= 4U) {
        char *typePtr = *(char **)((char *)this + 0xC8);
        char *base1 = (char *)this + 0xC4;
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x30);
        short off = entry->offset;
        int pos = rb._data[0];
        void *target = base1 + off;
        ((void (*)(void *, int, void *))entry->fn)(target, pos, cMemPool_GetPoolFromPtr(base1));
    }
    return result;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
}

struct RegionRemoveScratch {
    int words0[4];
    int zero10;
    int zero14;
    unsigned char flag18;
    char pad19[3];
    int word1C;
    float floats20[10];
    int pad48[4];
    int word58;
    int word5C;
    int word60;
    int word64;
    int word68;
    int word6C;
    int word70;
    char vec74[16];
};

struct ExpressionEvalRecord {
    void *prev;
    int zero;
    void *list;
    void *scratchBase;
    void *exprData;
};

int gcRegion::RemoveFromWorld(void) {
    int blocked = 1;
    if (*(int *)((char *)this + 0x4C) & 1) {
        blocked = 0;
    }
    if (blocked != 0) {
        return 0;
    }

    RegionRemoveScratch scratch;
    scratch.zero10 = 0;
    scratch.zero14 = 0;
    scratch.flag18 = 1;
    scratch.words0[0] = 0;
    scratch.words0[1] = 0;
    scratch.words0[2] = 0;
    scratch.words0[3] = 0;
    scratch.word1C = 0;
    scratch.floats20[0] = 0.0f;
    scratch.word58 = 0;
    __vec_new(scratch.vec74, 2, 8, (void (*)(void *))0x2275F0);
    scratch.floats20[1] = 0.0f;
    scratch.floats20[2] = 0.0f;
    scratch.floats20[3] = 0.0f;
    scratch.floats20[4] = 0.0f;
    scratch.floats20[5] = 0.0f;
    scratch.floats20[6] = 0.0f;
    scratch.floats20[7] = 0.0f;
    scratch.floats20[8] = 0.0f;
    scratch.floats20[9] = 0.0f;
    scratch.word5C = 0;
    scratch.word60 = 0;
    scratch.word64 = 0;
    scratch.word68 = 0;
    scratch.word6C = 0;

    int empty = (*(int *)((char *)this + 0xCC) == 0) & 0xFF;
    scratch.word70 = 0;
    if (empty) {
        goto after_eval;
    }
    if ((g_regionRemoveEnabled == 1) & 0xFF) {
        ExpressionEvalRecord rec;
        rec.prev = g_expressionEvalStack;
        rec.zero = 0;
        rec.list = (char *)this + 0xC4;
        rec.scratchBase = &scratch;
        int *exprData = &scratch.word1C;
        rec.exprData = exprData;
        g_expressionEvalStack = &rec;
        *exprData = *exprData & -4;
        ((gcExpressionList *)((char *)this + 0xCC))->Evaluate();
        g_expressionEvalStack = rec.prev;
    }
after_eval:
    return gcRegionBase_RemoveFromWorld(this);
}

const void *gcRegion::GetType(void) const {
    if (!type_gcRegion) {
        if (!type_gcRegionBase) {
            if (!type_cFactory) {
                if (!type_cObject) {
                    if (!type_cNamed) {
                        if (!type_cBase) {
                            type_cBase = cType::InitializeType(gcRegion_base_name, gcRegion_base_desc, 1, 0, 0, 0, 0, 0);
                        }
                        type_cNamed = cType::InitializeType(0, 0, 2, type_cBase, (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                    }
                    type_cObject = cType::InitializeType(0, 0, 3, type_cNamed, 0, 0, 0, 0);
                }
                type_cFactory = cType::InitializeType(0, 0, 5, type_cObject, 0, 0, 0, 0);
            }
            type_gcRegionBase = cType::InitializeType(0, 0, 0xB3, type_cFactory, 0, 0, 0, 0);
        }
        type_gcRegion = cType::InitializeType(0, 0, 0x66, type_gcRegionBase, gcRegion::New, gcRegion_gettype_name, gcRegion_gettype_desc, 1);
    }
    return type_gcRegion;
}
