// gcValRandomNumber -- decompiled from gcAll_psp.obj

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class gcValue {
public:
    cBase *mParent;
    void *mClassDesc;

    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    int mValue;

    void Write(cWriteBlock &) const;
};

extern char cBaseclassdesc[];
extern char gcValRandomNumbervirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValRandomNumber : public gcValue {
public:
    int mField8;
    gcDesiredValue mDesiredC;
    gcDesiredValue mDesired10;
    bool mField14;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

// 0x00357d48 -- gcValRandomNumber::New(cMemPool *, cBase *) static
cBase *gcValRandomNumber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValRandomNumber *result = 0;
    gcValRandomNumber *obj = (gcValRandomNumber *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValRandomNumbervirtualtable;
        int one = 1;
        int tagged = (int)obj | 1;
        ((int *)obj)[2] = one;
        ((int *)obj)[3] = tagged;
        ((int *)obj)[4] = tagged;
        ((unsigned char *)obj)[0x14] = 0;
        result = obj;
    }
    return (cBase *)result;
}

void gcValRandomNumber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(mField8);
    wb.Write(mField14);
    mDesiredC.Write(wb);
    mDesired10.Write(wb);
    wb.End();
}
