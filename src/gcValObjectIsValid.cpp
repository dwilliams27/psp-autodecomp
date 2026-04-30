// gcValObjectIsValid.cpp - decompiled from gcAll_psp.obj.
//
// Functions:
//   0x00356628 gcValObjectIsValid::New(cMemPool *, cBase *) static  140B

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T dcast(const cBase *);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
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

struct CloneEntry {
    short offset;
    short pad;
    cBase *(*fn)(void *, cMemPool *, cBase *);
};

struct ReleaseEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcValue {
public:
    cBase *mParent;
    void *mClassDesc;

    void Write(cFile &) const;
};

class gcValObjectIsValid : public gcValue {
public:
    int mObject;

    ~gcValObjectIsValid();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char cBaseclassdesc[];
extern char gcValObjectIsValidvirtualtable[];

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValObjectIsValid;

// -- gcValObjectIsValid::New @ 0x00356628 --
cBase *gcValObjectIsValid::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectIsValid *result = 0;
    gcValObjectIsValid *obj =
        (gcValObjectIsValid *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValObjectIsValidvirtualtable;
        obj->mObject = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcValObjectIsValid::AssignCopy(const cBase *base) {
    int temp_s1 = (int)this;
    int temp_s2 = (int)dcast<gcValObjectIsValid *>(base);

    if (temp_s2 + 8 != temp_s1 + 8) {
        temp_s1 += 8;
        int temp_a2 = *(int *)((char *)this + 8);
        int var_a1 = 1;
        int temp_a0 = temp_a2 & 1;
        if (temp_a0 != 0) {
            var_a1 = 0;
        }
        int temp_a3 = temp_a0;
        if (var_a1 != 0) {
            int var_a1_2 = 0;
            int var_a2;
            temp_a0 = temp_a2;
            if (temp_a3 != 0) {
                var_a1_2 = 1;
            }
            if (var_a1_2 != 0) {
                var_a2 = temp_a2 & ~1;
            } else {
                var_a2 = *(int *)temp_a2;
            }
            *(int *)((char *)this + 8) = var_a2 | 1;
            if (temp_a0 != 0) {
                ReleaseEntry *entry =
                    (ReleaseEntry *)(*(char **)(temp_a0 + 4) + 0x50);
                short temp_a3 = entry->offset;
                void (*temp_a2_3)(void *, int) = entry->fn;
                temp_a2_3((char *)temp_a0 + temp_a3, 3);
            }
        }

        temp_s2 = *(int *)(temp_s2 + 8);
        int temp_a0_3 = 1;
        int temp_a1_2 = temp_s2 & 1;
        if (temp_a1_2 != 0) {
            temp_a0_3 = 0;
        }
        if (temp_a0_3 != 0) {
            int temp_a1_3 = *(int *)(temp_s2 + 4);
            int old_s1 = temp_s1;
            temp_s1 = temp_a1_3 + 0x10;
            short temp_a1 = *(short *)temp_s1;
            temp_s2 += temp_a1;
            cMemPool *temp_a1_4 = cMemPool::GetPoolFromPtr((void *)old_s1);
            int temp_a0_4 = *(int *)((char *)this + 8);
            int var_a2_2 = 0;
            if (temp_a0_4 & 1) {
                var_a2_2 = 1;
            }
            if (var_a2_2 != 0) {
                temp_a0_4 &= ~1;
            } else {
                temp_a0_4 = *(int *)temp_a0_4;
            }
            *(int *)((char *)this + 8) =
                (int)((CloneEntry *)temp_s1)
                    ->fn((void *)temp_s2, temp_a1_4, (cBase *)temp_a0_4);
        }
    }
}

const cType *gcValObjectIsValid::GetType(void) const {
    if (!type_gcValObjectIsValid) {
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
        type_gcValObjectIsValid = cType::InitializeType(
            0, 0, 0x1BC, type_value, gcValObjectIsValid::New, 0, 0, 0);
    }
    return type_gcValObjectIsValid;
}

void gcValObjectIsValid::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);

    int value = mObject;
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);
    wb.End();
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSgcValObjectIsValiddtv, 0xd4\n");

gcValObjectIsValid::~gcValObjectIsValid() {
    *(void **)((char *)this + 4) = gcValObjectIsValidvirtualtable;
    char *slot = (char *)this + 8;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 8);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            ReleaseEntry *rec = (ReleaseEntry *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, int) = rec->fn;
            fn(obj + off, 3);
            *(int *)((char *)this + 8) = 0;
        }
    }
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
