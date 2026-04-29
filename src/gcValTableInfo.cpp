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
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

typedef void (*DesiredWriteFn)(cBase *, cFile *);

struct DesiredWriteSlot {
    short mOffset;
    short _pad;
    DesiredWriteFn mFn;
};

struct DesiredTypeInfoWrite {
    char _pad[0x28];
    DesiredWriteSlot mSlot;
};

struct PoolBlock {
    char _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValTableInfo : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern char gcValTableInfovirtualtable[];
extern char D_00000838[];

cBase *gcValTableInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValTableInfo *result = 0;
    gcValTableInfo *obj =
        (gcValTableInfo *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValTableInfovirtualtable;
        gcDesiredObject_gcDesiredObject((char *)obj + 8, (cBase *)obj);
        ((void **)obj)[3] = D_00000838;
        ((int *)obj)[5] = 1;
        ((int *)obj)[6] = 0;
        ((int *)obj)[3] = 0x38A000;
        ((int *)obj)[7] = 2;
        ((int *)obj)[8] = (int)((unsigned int)obj | 1);
        result = obj;
    }
    return (cBase *)result;
}

void gcValTableInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    DesiredTypeInfoWrite *ti = *(DesiredTypeInfoWrite **)((char *)this + 0x0C);
    DesiredWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x08);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.Write(*(int *)((char *)this + 0x1C));
    ((const gcDesiredValue *)((const char *)this + 0x20))->Write(wb);
    wb.End();
}
