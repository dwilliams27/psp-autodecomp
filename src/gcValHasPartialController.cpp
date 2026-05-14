#include "cBase.h"

class cFile;
class cFileHandle;

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
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
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

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcValue {
public:
    void Write(cFile &) const;
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

class gcDoEntitySetPrimaryController {
public:
    void GetText(char *) const;
};

class gcValLookAtControllerVariable : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcDesiredEnumerationEntryHelper {
public:
    void GetText(char *, bool) const;
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValHasPartialController : public gcValue {
public:
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
    ~gcValHasPartialController(void);
    void GetText(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValHasPartialController_base_name[];
extern const char gcValHasPartialController_base_desc[];

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                              bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

void gcValHasPartialController::GetText(char *buf) const {
    const gcValHasPartialController *self = this;
    cTypeMethod *entityText =
        (cTypeMethod *)((char *)((gcDesiredObject *)((char *)self + 8))->mType +
                        0x78);
    char *entityBase = (char *)self + 8;
    ((void (*)(void *, char *))entityText->fn)(entityBase + entityText->offset,
                                               buf);

    const char *partial;
    if (*(unsigned char *)((const char *)self + 0x4C) != 0) {
        partial = (const char *)0x36F45C;
    } else {
        partial = (const char *)0x36F47C;
    }
    cStrCat(buf, partial);

    register int asValue __asm__("a2") = 0;
    __asm__ volatile("" : "+r"(asValue));
    ((const gcDesiredEnumerationEntryHelper *)((const char *)self + 0x3C))
        ->GetText(buf, asValue);

    int flag = 0;
    int val = *(int *)((const char *)self + 0x48);
    int tag = val & 1;
    if (tag != 0) {
        flag = 1;
    }

    int valid;
    if (flag == 0) goto has_partial_not_tagged;
    valid = 0;
    goto has_partial_valid_done;
has_partial_not_tagged:
    valid = val != 0;
    valid &= 0xFF;
    valid = valid != 0;
has_partial_valid_done:

    if (valid != 0) {
        cStrAppend(buf, (const char *)0x36DCBC);

        flag = 0;
        val = *(int *)((const char *)self + 0x48);
        __asm__ volatile("" : "+r"(val), "+r"(self));
        tag = val & 1;
        if (tag != 0) {
            flag = 1;
        }
        if (flag != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)check + slot->offset, buf);
        } else {
            cStrCat(buf, (const char *)0x36DB24);
        }
    }

    cStrCat(buf, (const char *)0x36E2E8);
}

void gcDoEntitySetPrimaryController::GetText(char *buf) const {
    register const gcDoEntitySetPrimaryController *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    cTypeMethod *entityText =
        (cTypeMethod *)((char *)*(void **)((const char *)self + 0x10) + 0x78);
    char *entityBase = (char *)self + 0x0C;
    ((void (*)(void *, char *))entityText->fn)(entityBase + entityText->offset,
                                               out);

    unsigned char usePrimary = *(unsigned char *)((const char *)self + 0x3D);
    unsigned char enabled = *(unsigned char *)((const char *)self + 0x3C);
    if (usePrimary != 0) {
        const char *fmt = (const char *)0x36EA8C;
        const char *state;
        if (enabled != 0) {
            state = (const char *)0x36E428;
        } else {
            state = (const char *)0x36EAA8;
        }
        cStrAppend(out, fmt, state);
    } else {
        const char *fmt = (const char *)0x36EAAC;
        const char *state;
        if (enabled != 0) {
            state = (const char *)0x36E428;
        } else {
            state = (const char *)0x36EAA8;
        }
        cStrAppend(out, fmt, state);

        int val = *(int *)((const char *)self + 0x38);
        int flag = 0;
        if (val & 1) {
            flag = 1;
        }
        if (flag != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)check + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        cStrAppend(out, (const char *)0x36DCEC);
    }
}

void gcValHasPartialController::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    const cTypeMethod *entityWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    const char *entityBase = (const char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)entityWrite->fn)((void *)(entityBase + entityWrite->offset), wb.file);

    const cTypeMethod *templateWrite =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x34))->mType + 0x28);
    const char *templateBase = (const char *)this + 0x34;
    ((WriteFn)templateWrite->fn)((void *)(templateBase + templateWrite->offset), wb.file);

    wb.Write(*(const bool *)((const char *)this + 0x4C));
    wb.End();
}

int gcValHasPartialController::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19") = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);

    if (rb[3] != 2 || gcValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    char *entity = (char *)this + 8;
    char *entityType = *(char **)((char *)this + 12);
    const cTypeMethod *entityRead = (const cTypeMethod *)(entityType + 0x30);
    cFile *f = *(cFile **)&rb[0];
    typedef void (*ReadFn)(void *, cFile *, void *);
    ((ReadFn)entityRead->fn)(entity + entityRead->offset, f,
                             cMemPool::GetPoolFromPtr(entity));

    char *controller = (char *)this + 0x34;
    char *controllerType = *(char **)((char *)this + 0x38);
    const cTypeMethod *controllerRead =
        (const cTypeMethod *)(controllerType + 0x30);
    f = *(cFile **)&rb[0];
    ((ReadFn)controllerRead->fn)(controller + controllerRead->offset, f,
                                 cMemPool::GetPoolFromPtr(controller));

    char sp14;
    cFileSystem::Read(*(cFileHandle **)rb[0], &sp14, 1);
    *(bool *)((char *)this + 0x4C) = sp14 != 0;

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValHasPartialController;

const cType *gcValHasPartialController::GetType(void) const {
    if (!type_gcValHasPartialController) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcValHasPartialController_base_name,
                        gcValHasPartialController_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValHasPartialController =
            cType::InitializeType(0, 0, 0x1C8, type_value,
                                  gcValHasPartialController::New, 0, 0, 0);
    }
    return type_gcValHasPartialController;
}

cBase *gcValHasPartialController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValHasPartialController *result = 0;
    gcValHasPartialController *obj =
        (gcValHasPartialController *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x1" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,-0x78d0" : "+r"(type0));
            ((void **)obj)[1] = type0;
        }
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        {
            register void *type1 asm("a0");
            __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338"
                             : "=r"(type1));
            ((void **)obj)[3] = type1;
        }
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        {
            register void *helper_desc asm("a0");
            __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                             : "=r"(helper_desc));
            ((void **)obj)[8] = (void *)0x37E6A8;
            ((void **)obj)[3] = helper_desc;
        }
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        int sub_or_1 = (int)sub | 1;
        ((void **)obj)[14] = (void *)0x37E6A8;
        ((int *)obj)[12] = sub_or_1;
        ((gcValHasPartialController **)obj)[13] = obj;
        ((void **)obj)[14] = (void *)0x388568;
        ((char *)obj)[0x3C] = 1;
        ((char *)obj)[0x3D] = 0;
        ((int *)obj)[16] = 0;
        int obj_or_1 = (int)obj | 1;
        ((int *)obj)[17] = 0;
        ((int *)obj)[18] = obj_or_1;
        ((char *)obj)[0x4C] = 0;
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcValLookAtControllerVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLookAtControllerVariable *result = 0;
    gcValLookAtControllerVariable *obj =
        (gcValLookAtControllerVariable *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = (void *)0x37E6A8;
        {
            register void *type0 asm("a0");
            __asm__ volatile("lui %0,0x0" : "=r"(type0));
            ((cBase **)obj)[0] = parent;
            __asm__ volatile("addiu %0,%0,0x78e8" : "+r"(type0));
            ((void **)obj)[1] = type0;
        }
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        {
            register void *type1 asm("a0");
            __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338"
                             : "=r"(type1));
            ((void **)obj)[3] = type1;
        }
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        {
            register void *helper_desc asm("a0");
            __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                             : "=r"(helper_desc));
            ((void **)obj)[8] = (void *)0x37E6A8;
            ((void **)obj)[3] = helper_desc;
        }
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        int sub_or_1 = (int)sub | 1;
        ((void **)obj)[14] = (void *)0x37E6A8;
        ((int *)obj)[12] = sub_or_1;
        ((gcValLookAtControllerVariable **)obj)[13] = obj;
        ((void **)obj)[14] = (void *)0x388568;
        ((char *)obj)[0x3C] = 1;
        ((char *)obj)[0x3D] = 0;
        ((int *)obj)[16] = 0;
        int obj_or_1 = (int)obj | 1;
        ((int *)obj)[17] = 0;
        ((int *)obj)[18] = obj_or_1;
        ((int *)obj)[19] = 0;
        result = obj;
    }
    return (cBase *)result;
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oZgcValHasPartialControllerdtv, 0x1dc\n");

gcValHasPartialController::~gcValHasPartialController(void) {
    register void *vt asm("a0");
    __asm__ volatile("lui %0,0x1\n\taddiu %0,%0,-0x78d0" : "=r"(vt));
    *(void **)((char *)this + 4) = vt;
    char *p34 = (char *)this + 0x34;
    void *baseDesc = (void *)0x37E6A8;
    char *outer = (char *)this + 0x8;

    if ((void *)p34 != 0) {
        *(void **)((char *)this + 0x38) = (void *)0x388568;
        if ((void *)((char *)this + 0x48) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x48);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x48) = 0;
            }
        }
        *(void **)((char *)this + 0x38) = baseDesc;
    }
    if ((void *)outer != 0) {
        *(void **)((char *)this + 0xC) = (void *)0x388A48;
        if ((void *)((char *)this + 0x1C) != 0) {
            *(void **)((char *)this + 0x20) = (void *)0x388568;
            if ((void *)((char *)this + 0x30) != 0) {
                int owned = 1;
                int val = *(int *)((char *)this + 0x30);
                if (val & 1) {
                    owned = 0;
                }
                if (owned != 0 && val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x30) = 0;
                }
            }
            *(void **)((char *)this + 0x20) = baseDesc;
        }
        *(void **)((char *)this + 0xC) = (void *)0x3889A8;
        if ((void *)((char *)this + 0x10) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x10);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
        *(void **)((char *)this + 0xC) = baseDesc;
    }
    *(void **)((char *)this + 4) = baseDesc;
}
