#include "cBase.h"

class cFile;
class cType;
class gcValUIListInfo;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredUIWidgetHelper {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char gcValUIListInfovirtualtable[];
extern char gcValUIListInfoDerivedvtable[];
extern char cBaseclassdesc[];

void gcValue_Write(const gcValUIListInfo *, cFile &);
void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValUIListInfo {
public:
    int pad0;
    int pad4;
    char widgetHelper[0xC];
    int mField;
    int pad18;

    ~gcValUIListInfo();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValUIListInfo;

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oPgcValUIListInfodtv, 0xd4\n");

// -----------------------------------------------------------------------------
// gcValUIListInfo::~gcValUIListInfo(void)  @ 0x00363458, 212B
// -----------------------------------------------------------------------------
gcValUIListInfo::~gcValUIListInfo() {
    *(char **)((char *)this + 4) = gcValUIListInfovirtualtable;
    char *slot = (char *)this + 0x18;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x18) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}

// -----------------------------------------------------------------------------
// gcValUIListInfo::New(cMemPool *, cBase *) static  @ 0x003628e0, 168B
// -----------------------------------------------------------------------------
cBase *gcValUIListInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValUIListInfo *result = 0;
    gcValUIListInfo *obj = (gcValUIListInfo *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = (int)gcValUIListInfovirtualtable;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)gcValUIListInfoDerivedvtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 8, 1);
        ((int *)obj)[5] = 1;
        ((int *)obj)[6] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// gcValUIListInfo::Write(cFile &) const  @ 0x00362aa0, 112B
// -----------------------------------------------------------------------------
void gcValUIListInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue_Write(this, file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 8))->Write(wb);
    wb.Write(mField);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    wb.End();
}

const cType *gcValUIListInfo::GetType(void) const {
    if (!type_gcValUIListInfo) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValUIListInfo = cType::InitializeType(0, 0, 0x1F1, type_value,
                                                     gcValUIListInfo::New,
                                                     0, 0, 0);
    }
    return type_gcValUIListInfo;
}
