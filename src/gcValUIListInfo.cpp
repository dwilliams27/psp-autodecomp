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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileHandle;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class gcDesiredUIWidgetHelper {
public:
    void GetText(char *) const;
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
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
int gcValue_Read(gcValUIListInfo *, cFile &, cMemPool *);
void cFile_SetCurrentPos(void *, unsigned int);
void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class gcStringValue {
public:
    int Read(cFile &, cMemPool *);
};

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

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
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
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
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

class gcLobbyAccountStrings : public gcStringValue {
public:
    int Read(cFile &, cMemPool *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

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

// -----------------------------------------------------------------------------
// gcValUIListInfo::Read(cFile &, cMemPool *)  @ 0x00362b10, 232B
// -----------------------------------------------------------------------------
int gcValUIListInfo::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] == 2 && gcValue_Read(this, file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Read(*(cReadBlock *)rb);
    cFileSystem::Read((cFileHandle *)*(void **)rb[0], (char *)this + 0x14, 4);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// -----------------------------------------------------------------------------
// gcLobbyAccountStrings::Read(cFile &, cMemPool *)  @ 0x0027fd34, 240B
// -----------------------------------------------------------------------------
int gcLobbyAccountStrings::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] == 2 && this->gcStringValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    cFileSystem::Read((cFileHandle *)*(void **)rb[0], (char *)this + 8, 4);
    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    cFileSystem::Read((cFileHandle *)*(void **)rb[0], (char *)this + 0x10, 4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// -----------------------------------------------------------------------------
// gcValUIListInfo::GetText(char *) const  @ 0x00363080, 264B
// -----------------------------------------------------------------------------
void gcValUIListInfo::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 8))->GetText(local);
    cStrAppend(buf, (const char *)0x36DF34, local, (const char *)0x36DAF0);

    int flag = 0;
    int kind = mField;
    if ((kind == 4) || (kind == 5)) {
        flag = 1;
    }
    int doExtra = flag & 0xFF;
    if (doExtra != 0) {
        char valueText[256];
        valueText[0] = '\0';
        __asm__ volatile("" ::: "memory");
        int val = pad18;
        register char *valueBuf __asm__("$17") = valueText;
        int tagged = 0;
        if (val & 1) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }
        int check = val;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)val + slot->offset, valueBuf);
        } else {
            cStrCat(valueBuf, (const char *)0x36DB24);
        }
        cStrAppend(buf, (const char *)0x36DE40, valueBuf);
    }
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
