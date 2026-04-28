// gcValLobbyScoreboardRowInfo -- decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;

extern char cBaseclassdesc[];
extern char gcValLobbyScoreboardRowInfovirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

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

class gcValue {
public:
    cBase *mParent;
    void *mVtable;

    void Write(cFile &) const;
};

class gcValLobbyScoreboardRowInfo : public gcValue {
public:
    int mField8;
    int mFieldC;
    int mDesired10;
    int mDesired14;
    int mBase18;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

// 0x0034d2c4 -- gcValLobbyScoreboardRowInfo::New(cMemPool *, cBase *) static
cBase *gcValLobbyScoreboardRowInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyScoreboardRowInfo *result = 0;
    gcValLobbyScoreboardRowInfo *obj =
        (gcValLobbyScoreboardRowInfo *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValLobbyScoreboardRowInfovirtualtable;
        ((int *)obj)[2] = 0;
        int tagged = (int)obj | 1;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = tagged;
        ((int *)obj)[5] = tagged;
        ((int *)obj)[6] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0034d478 -- gcValLobbyScoreboardRowInfo::Write(cFile &) const
void gcValLobbyScoreboardRowInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mFieldC);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    int val = this->mBase18;
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
