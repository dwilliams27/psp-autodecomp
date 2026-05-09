class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

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
    void Read(class cReadBlock &);
};

class gcEntityAttack {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
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

struct ReadEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class gcEntityAnimationConfig {
public:
    int Read(cFile &, cMemPool *);
};

void gcEntityAnimationConfig_ctor(gcEntityAnimationConfig *, cBase *);
void gcEvent_gcEvent(void *, cBase *, const char *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

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

int gcEntityAttack::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 6, true);
    if ((unsigned int)rb[3] >= 7 || (unsigned int)rb[3] < 5) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    ((gcEntityAnimationConfig *)((char *)this + 0x08))->Read(file, pool);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x2C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x30, 4);
    ((cBaseArray *)((char *)this + 0x70))->Read(*(cReadBlock *)rb);

    {
        char *typeInfo = *(char **)((char *)this + 0x3C);
        char *base = (char *)this + 0x38;
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        short off = rec->offset;
        char *target = base + off;
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x34, 4);

    if ((unsigned int)rb[3] >= 6) {
        char sp14;
        cFileSystem::Read(*(cFileHandle **)rb[0], &sp14, 1);
        cFile *f1 = *(cFile **)&rb[0];
        *(unsigned char *)((char *)this + 0x54) = sp14 != 0;

        char sp15;
        cFileSystem::Read(*(cFileHandle **)f1, &sp15, 1);
        *(unsigned char *)((char *)this + 0x55) = sp15 != 0;

        char *typeInfo = *(char **)((char *)this + 0x5C);
        char *base = (char *)this + 0x58;
        ReadEntry *rec = (ReadEntry *)(typeInfo + 0x30);
        short off = rec->offset;
        char *target = base + off;
        __asm__ volatile("" ::: "memory");
        cFile *f = *(cFile **)&rb[0];
        cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
        rec->fn(target, f, childPool);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
