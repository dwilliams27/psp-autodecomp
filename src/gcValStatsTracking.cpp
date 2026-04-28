class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
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

extern char cBaseclassdesc[];
extern char gcValStatsTrackingvirtualtable[];

class gcValStatsTracking : public gcLValue {
public:
    int mField8;
    gcDesiredValue mDesiredC;
    int mField10;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

cBase *gcValStatsTracking::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValStatsTracking *result = 0;
    gcValStatsTracking *obj = (gcValStatsTracking *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValStatsTrackingvirtualtable;
        *(int *)((char *)obj + 8) = 0;
        int tagged = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = tagged;
        *(int *)((char *)obj + 0x10) = tagged;
        result = obj;
    }
    return (cBase *)result;
}

void gcValStatsTracking::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    wb.Write(mField8);
    mDesiredC.Write(wb);
    int val = mField10;
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}
