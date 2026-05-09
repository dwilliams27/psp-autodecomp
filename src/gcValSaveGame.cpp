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
    void Read(class cReadBlock &);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcValue {
public:
    int Read(cFile &, cMemPool *);
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

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

class cType {
public:
    char pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char cBaseclassdesc[];
extern char D_00389508[];

extern "C" void cObject_cObject(void *, cBase *);
void gcDesiredObject_gcDesiredObject(void *, cBase *);
void gcValue_Write(const void *, cFile &);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class gcValSaveGame {
public:
    int pad0;
    int pad4;
    int pad8;
    int padC;
    int pad10;
    int pad14;

    gcValSaveGame &operator=(const gcValSaveGame &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredEntityTemplate {
public:
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValSaveGame asm("D_0009F8D4");

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

const cType *gcValSaveGame::GetType(void) const {
    if (!type_gcValSaveGame) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValSaveGame = cType::InitializeType(0, 0, 0x1B6, type_value,
                                                   gcValSaveGame::New,
                                                   0, 0, 0);
    }
    return type_gcValSaveGame;
}

void gcValSaveGame::AssignCopy(const cBase *base) {
    const gcValSaveGame *other = 0;

    if (base != 0) {
        if (!type_gcValSaveGame) {
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
            type_gcValSaveGame = cType::InitializeType(
                0, 0, 0x1B6, type_value, gcValSaveGame::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValSaveGame;
        cType *type = entry->fn((char *)base + entry->offset);
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
            other = (const gcValSaveGame *)base;
        }
    }

    operator=(*other);
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

int gcValSaveGame::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19") = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);

    if (rb[3] != 3 || ((gcValue *)this)->Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x08, 4);
    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);

    int sp14;
    int value = *(int *)((char *)this + 0x14);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x14);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x14) = newValue;
    __0oKcReadBlockdtv(rb, 2);
    return result;
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
