class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    int mValue0;
    int mValue1;

    void Write(cFile &) const;
};

class gcValPointValue : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
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

struct cTypeMethod {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern char cBaseclassdesc[];
extern char D_000005B8[];
extern char D_00009A80[];

void gcDesiredObject_gcDesiredObject(void *, cBase *);

cBase *gcValPointValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValPointValue *result = 0;
    gcValPointValue *obj = (gcValPointValue *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = D_00009A80;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x10, (cBase *)obj);
        ((void **)obj)[5] = D_000005B8;
        ((int *)obj)[7] = 7;
        ((int *)obj)[8] = 0;
        ((void **)obj)[5] = (void *)0x389760;
        result = obj;
    }
    return (cBase *)result;
}

void gcValPointValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(((const int *)this)[2]);
    wb.Write(((const int *)this)[3]);

    const cTypeMethod *method =
        (const cTypeMethod *)((char *)((const void **)this)[5] + 0x28);
    char *base = (char *)this + 0x10;
    method->fn(base + method->offset, wb._file);
    wb.End();
}
