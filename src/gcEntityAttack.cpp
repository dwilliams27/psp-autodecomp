class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *mFile;
    int mPos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class gcEntityAttack {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
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

struct WriteEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

class gcEntityAnimationConfig;

void gcEntityAnimationConfig_ctor(gcEntityAnimationConfig *, cBase *);
void gcEvent_gcEvent(void *, cBase *, const char *);

extern char cBaseclassdesc[];
extern char gcEntityAttackvirtualtable[];
extern const char gcEntityAttack_event_name[];
extern cType *D_000385DC;
extern cType *D_0009A3F8;

cBase *gcEntityAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntityAttack *result = 0;
    gcEntityAttack *obj =
        (gcEntityAttack *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcEntityAttackvirtualtable;
        gcEntityAnimationConfig_ctor((gcEntityAnimationConfig *)((char *)obj + 8), (cBase *)obj);
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = 0;
        ((int *)obj)[13] = -1;
        gcEvent_gcEvent((char *)obj + 0x38, (cBase *)obj, gcEntityAttack_event_name);
        *(char *)((char *)obj + 0x54) = 0;
        __asm__ volatile("" : : "m"(*(char *)((char *)obj + 0x54)));
        ((void **)obj)[23] = cBaseclassdesc;
        *(char *)((char *)obj + 0x55) = 0;
        ((gcEntityAttack **)obj)[22] = obj;
        int desiredVTable = 0x388568;
        int one = 1;
        ((int *)obj)[23] = desiredVTable;
        *(char *)((char *)obj + 0x60) = one;
        *(char *)((char *)obj + 0x61) = 0;
        ((int *)obj)[25] = 0;
        int encoded = (int)obj | 1;
        ((int *)obj)[26] = 0;
        ((int *)obj)[27] = encoded;
        ((int *)obj)[28] = 0;
        ((gcEntityAttack **)obj)[29] = obj;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcEntityAttack::GetType(void) const {
    if (D_0009A3F8 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009A3F8 = cType::InitializeType(0, 0, 0x144, D_000385DC,
                                           &gcEntityAttack::New, 0, 0, 0);
    }
    return D_0009A3F8;
}

void gcEntityAttack::Write(cFile &file) const {
    cFile *out = &file;
    const gcEntityAttack *self = this;
    cWriteBlock wb(file, 6);

    {
        char *typePtr = *(char **)((const char *)self + 0x0C);
        WriteEntry *entry = (WriteEntry *)(typePtr + 0x28);
        char *base = (char *)self + 8;
        entry->fn(base + entry->offset, out);
    }

    wb.Write(*(const int *)((const char *)self + 0x2C));
    wb.Write(*(const int *)((const char *)self + 0x30));
    ((const cBaseArray *)((const char *)self + 0x70))->Write(wb);

    {
        char *typePtr = *(char **)((const char *)self + 0x3C);
        WriteEntry *entry = (WriteEntry *)(typePtr + 0x28);
        char *base = (char *)self + 0x38;
        entry->fn(base + entry->offset, wb.mFile);
    }

    wb.Write(*(const unsigned int *)((const char *)self + 0x34));
    wb.Write(*(const bool *)((const char *)self + 0x54));
    wb.Write(*(const bool *)((const char *)self + 0x55));

    {
        char *typePtr = *(char **)((const char *)self + 0x5C);
        WriteEntry *entry = (WriteEntry *)(typePtr + 0x28);
        char *base = (char *)self + 0x58;
        entry->fn(base + entry->offset, wb.mFile);
    }

    wb.End();
}
