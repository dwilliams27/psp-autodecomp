#include "cBase.h"

class cFile;
class cMemPool;
class gcValUIListInfo;

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

extern char gcValUIListInfovirtualtable[];
extern char gcValUIListInfoDerivedvtable[];

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

class gcValUIListInfo {
public:
    int pad0;
    int pad4;
    char widgetHelper[0xC];
    int mField;
    int pad18;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

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
