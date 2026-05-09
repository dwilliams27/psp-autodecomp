class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cMemPool;
class cType;
class cFileHandle;

class gcStringLValue {};

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];

    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

struct ReadEntry {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

class gcLValue : public gcStringLValue {
public:
    int Read(cFile &, cMemPool *);
};

class gcValTableEntry : public gcStringLValue {
public:
    gcValTableEntry &operator=(const gcValTableEntry &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
};

class eGeomTemplate;

template <class T> class cHandleT {
public:
    int mIndex;
};

template <class T> class cArrayBase {
public:
    void *mData;
    void Read(cReadBlock &);
};

struct PoolReadEntry {
    short offset;
    short _pad;
    void (*fn)(void *, cMemPool *, int);
};

class eWeatherSystem {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern char D_00000838[];
extern char D_0000A198[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_variable asm("D_0009F3EC");
static cType *type_gcValTableEntry asm("D_0009F8E8");

cBase *gcValTableEntry::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValTableEntry *result = 0;
    gcValTableEntry *obj =
        (gcValTableEntry *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = D_0000A198;
        gcDesiredObject_gcDesiredObject((char *)obj + 8, (cBase *)obj);
        ((void **)obj)[3] = D_00000838;
        ((int *)obj)[5] = 1;
        ((int *)obj)[6] = 0;
        ((int *)obj)[3] = 0x38A000;
        int self = (int)((unsigned int)obj | 1);
        ((int *)obj)[7] = self;
        ((int *)obj)[8] = self;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValTableEntry::GetType(void) const {
    if (!type_gcValTableEntry) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValTableEntry = cType::InitializeType(
            0, 0, 0x218, type_variable, gcValTableEntry::New, 0, 0, 0);
    }
    return type_gcValTableEntry;
}

void gcValTableEntry::AssignCopy(const cBase *base) {
    const gcValTableEntry *other = 0;

    if (base != 0) {
        if (!type_gcValTableEntry) {
            if (!type_variable) {
                if (!type_value) {
                    if (!type_expression) {
                        if (!type_base) {
                            type_base = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        type_expression = cType::InitializeType(
                            0, 0, 0x6A, type_base, 0, 0, 0, 0);
                    }
                    type_value = cType::InitializeType(
                        0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
                }
                type_variable = cType::InitializeType(
                    0, 0, 0x6D, type_value, 0, 0, 0, 0);
            }
            type_gcValTableEntry = cType::InitializeType(
                0, 0, 0x218, type_variable, gcValTableEntry::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *wanted = type_gcValTableEntry;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValTableEntry *)base;
        }
    }

    operator=(*other);
}

int gcValTableEntry::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 2, true);
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._pad[1] >= 3 || (unsigned int)rb._pad[1] < 1 ||
        ((gcLValue *)this)->Read(file, pool) == 0) {
        rb.file->SetCurrentPos(rb._pos);
        return 0;
    }

    if ((unsigned int)rb._pad[1] >= 2) {
        char *base = (char *)this + 8;
        __asm__ volatile("" ::: "memory");
        char *typeInfo = *(char **)((char *)this + 12);
        cFile *f = rb.file;
        ReadEntry *entry = (ReadEntry *)(typeInfo + 0x30);
        short offset = entry->offset;
        entry->fn(base + offset, f, cMemPool::GetPoolFromPtr(base));
    } else {
        *(int *)((char *)this + 0x14) = 7;
        char *handle = (char *)this + 0x18;
        *(int *)handle = 0;
        ((cHandle *)handle)->Read(rb, cMemPool::GetPoolFromPtr(handle));
    }

    ((gcDesiredValue *)((char *)this + 0x1C))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x20))->Read(rb);
    return result;
}

#pragma control sched=1
int eWeatherSystem::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);

    if ((unsigned int)rb._pad[1] != 2) {
        rb.file->SetCurrentPos(rb._pos);
        return 0;
    }

    ((cArrayBase<cHandleT<eGeomTemplate> > *)((char *)this + 8))->Read(rb);
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x14, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x18, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x1C, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x20, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x24, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x28, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x2C, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb.file;
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, (char *)this + 0x30, 4);
    }

    char *classDesc = *(char **)((char *)this + 4);
    PoolReadEntry *entry = (PoolReadEntry *)(classDesc + 0x38);
    entry->fn((char *)this + entry->offset, pool, 0);
    return result;
}
#pragma control sched=2
