// gcValGameSetting - gcLValue subclass with a desired value and enum entry.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
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

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;
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

class gcValGameSetting : public gcLValue {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

extern char cBaseclassdesc[];
extern char gcLValuevirtualtable[];
extern char gcDesiredEnumerationEntryvirtualtable[];

typedef void (*WriteFn)(void *, cFile *);

// 0x00344838 - gcValGameSetting::New(cMemPool *, cBase *) static
cBase *gcValGameSetting::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValGameSetting *result = 0;
    gcValGameSetting *obj = (gcValGameSetting *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((int *)obj)[2] = 0;
        int tagged = (int)obj | 1;
        ((void **)obj)[5] = cBaseclassdesc;
        ((int *)obj)[3] = tagged;
        ((gcValGameSetting **)obj)[4] = obj;
        ((void **)obj)[5] = gcDesiredEnumerationEntryvirtualtable;
        ((unsigned char *)obj)[0x18] = 1;
        ((unsigned char *)obj)[0x19] = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = 0;
        ((int *)obj)[9] = tagged;
        ((int *)obj)[10] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00344a4c - gcValGameSetting::Write(cFile &) const
void gcValGameSetting::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcLValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 0x0C))->Write(wb);
    wb.Write(*(int *)((const char *)this + 8));
    const cTypeMethod *entry = &((const cType *)*(void *const *)((const char *)this + 0x14))->write_m;
    char *base = (char *)this + 0x10;
    ((WriteFn)entry->fn)(base + entry->offset, *(cFile **)&wb._data[0]);
    wb.Write(*(int *)((const char *)this + 0x28));
    wb.End();
}
