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
extern char D_00389508[];

extern "C" void cObject_cObject(void *, cBase *);
void gcDesiredObject_gcDesiredObject(void *, cBase *);
void gcValue_Write(const void *, cFile &);

class gcValSaveGame {
public:
    int pad0;
    int pad4;
    int pad8;
    int padC;
    int pad10;
    int pad14;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

class gcDesiredEntityTemplate {
public:
    static cBase *New(cMemPool *, cBase *);
};

cBase *gcValSaveGame::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValSaveGame *result = 0;
    gcValSaveGame *obj = (gcValSaveGame *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = (int)cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)cObject_cObject;
        register unsigned int tagged __asm__("$5");
        __asm__ volatile("ori %0, %1, 1" : "=r"(tagged) : "r"(obj));
        ((int *)obj)[2] = 0;
        ((unsigned int *)obj)[3] = tagged;
        ((unsigned int *)obj)[4] = tagged;
        ((unsigned int *)obj)[5] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

void gcValSaveGame::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcValue_Write(this, file);
    wb.Write(this->pad8);
    ((gcDesiredValue *)((char *)this + 0xC))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    int val = this->pad14;
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

cBase *gcDesiredEntityTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredEntityTemplate *result = 0;
    gcDesiredEntityTemplate *obj =
        (gcDesiredEntityTemplate *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] = D_00389508;
        *(int *)((char *)obj + 0xC) = 7;
        *(int *)((char *)obj + 0x10) = 0;
        *(int *)((char *)obj + 0x14) = (int)parent | 1;
        result = obj;
    }
    return (cBase *)result;
}
