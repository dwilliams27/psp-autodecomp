#include "cBase.h"

class cFile;
class cMemPool;
class gcValMapLoading;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(bool);
    void End(void);
};

extern char gcValMapLoadingvirtualtable[];
extern char gcValMapLoadingDerivedvtable[];
extern char cBaseclassdesc[];                       // 0x37E6A8

extern const char gcValMapLoading_fmt[];            // 0x36E4F8
extern const char gcValGetText_text[];              // 0x36DAF0
extern const char gcValMapLoading_text_true[];      // 0x36F580
extern const char gcValMapLoading_text_false[];     // 0x36D944

template <class T> T *dcast(const cBase *);
void gcValue_Write(const gcValMapLoading *, cFile &);
void *cMemPool_GetPoolFromPtr(void *);
void cStrAppend(char *, const char *, ...);

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
    short pad;
    void (*fn)(void *, void *);
};

class gcValMapLoading {
public:
    int pad0;
    int pad4;
    int mField;
    bool mFlag;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void GetText(char *) const;
};

// -----------------------------------------------------------------------------
// gcValMapLoading::AssignCopy(const cBase *)  @ 0x0034fa00, 56B
// -----------------------------------------------------------------------------
void gcValMapLoading::AssignCopy(const cBase *base) {
    gcValMapLoading *src = dcast<gcValMapLoading>(base);
    mField = src->mField;
    mFlag = src->mFlag;
}

// -----------------------------------------------------------------------------
// gcValMapLoading::New(cMemPool *, cBase *) static  @ 0x0034fa38, 140B
// -----------------------------------------------------------------------------
cBase *gcValMapLoading::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValMapLoading *result = 0;
    gcValMapLoading *obj = (gcValMapLoading *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = (int)gcValMapLoadingvirtualtable;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)gcValMapLoadingDerivedvtable;
        ((int *)obj)[2] = 0;
        *((unsigned char *)obj + 0xC) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// gcValMapLoading::Write(cFile &) const  @ 0x0034fbdc, 100B
// -----------------------------------------------------------------------------
void gcValMapLoading::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue_Write(this, file);
    wb.Write(this->mField);
    wb.Write(this->mFlag);
    wb.End();
}

// -----------------------------------------------------------------------------
// gcValMapLoading::GetText(char *) const  @ 0x0034ff18, 84B
// -----------------------------------------------------------------------------
void gcValMapLoading::GetText(char *buf) const {
    cStrAppend(buf, gcValMapLoading_fmt, gcValGetText_text,
               mFlag ? gcValMapLoading_text_true : gcValMapLoading_text_false);
}

// -----------------------------------------------------------------------------
// gcValMapLoading::~gcValMapLoading(void)  @ 0x0034ff6c, 100B
// -----------------------------------------------------------------------------
extern "C" void gcValMapLoading___dtor_gcValMapLoading_void(void *self, int flags) {
    if (self != 0) {
        ((char **)self)[1] = cBaseclassdesc;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec =
                (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}
