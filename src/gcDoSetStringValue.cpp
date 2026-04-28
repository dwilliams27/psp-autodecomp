class cBase;
class cFile;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    gcAction(cBase *);
    void Write(cFile &) const;
};

class gcDoSetStringValue : public gcAction {
public:
    int mValue;
    int mString;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcStatsTrackingStrings {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcDoSetStringValuevirtualtable[];
extern char gcStatsTrackingStringsvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcDoSetStringValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetStringValue *result = 0;
    gcDoSetStringValue *obj =
        (gcDoSetStringValue *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcAction(parent);
        ((void **)obj)[1] = gcDoSetStringValuevirtualtable;
        int self = (int)obj | 1;
        ((int *)obj)[3] = self;
        ((int *)obj)[4] = self;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoSetStringValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    int valC = ((const int *)this)[3];
    int flagC = 0;
    if (valC & 1) {
        flagC = 1;
    }
    cBase *ptrC;
    if (flagC != 0) {
        ptrC = 0;
    } else {
        ptrC = (cBase *)valC;
    }
    wb.WriteBase(ptrC);

    int val10 = ((const int *)this)[4];
    int flag10 = 0;
    if (val10 & 1) {
        flag10 = 1;
    }
    cBase *ptr10;
    if (flag10 != 0) {
        ptr10 = 0;
    } else {
        ptr10 = (cBase *)val10;
    }
    wb.WriteBase(ptr10);

    wb.End();
}

cBase *gcStatsTrackingStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStatsTrackingStrings *result = 0;
    gcStatsTrackingStrings *obj =
        (gcStatsTrackingStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcStatsTrackingStringsvirtualtable;
        *(int *)((char *)obj + 8) = 0;
        int self = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = self;
        *(int *)((char *)obj + 0x10) = self;
        result = obj;
    }
    return (cBase *)result;
}
