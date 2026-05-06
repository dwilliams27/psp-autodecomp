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

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

struct NamedObject {
    char pad[0x0C];
    const char *name;
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
    float Evaluate(void) const;
    const cType *GetType(void) const;
    void GetText(char *) const;
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

extern "C" void cStrAppend(char *, const char *, ...);

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

float gcValObjectIsValid::Evaluate(void) const {
    int obj = mObject;
    int flag = 0;
    int tag = obj & 1;
    if (tag != 0) {
        flag = 1;
    }
    int valid;
    if (flag == 0) goto eval_not_tagged;
    valid = 0;
    goto eval_valid_done;
eval_not_tagged:
    valid = obj != 0;
    valid &= 0xFF;
    valid = valid != 0;
eval_valid_done:
    if (valid != 0) {
        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }
        int base;
        char *type;
        if (flag2 == 0) goto eval_untagged_base;
        base = 0;
        type = *(char **)(base + 4);
        goto eval_type_done;
eval_untagged_base:
        base = obj;
        type = *(char **)(base + 4);
eval_type_done:
        cTypeMethod *entry = (cTypeMethod *)(type + 0x70);
        short off = entry->offset;
        int (*fn)(void *, int) = (int (*)(void *, int))entry->fn;
        if (fn((char *)base + off, 0) != 0) {
            return 1.0f;
        }
    }
    return 0.0f;
}

void gcValObjectIsValid::GetText(char *buf) const {
    int obj = mObject;
    int flag = 0;
    int tag = obj & 1;
    if (tag != 0) {
        flag = 1;
    }
    int valid;
    if (flag == 0) goto text_not_tagged;
    valid = 0;
    goto text_valid_done;
text_not_tagged:
    valid = obj != 0;
    valid &= 0xFF;
    valid = valid != 0;
text_valid_done:
    if (valid != 0) {
        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }
        int base;
        char *type;
        if (flag2 == 0) goto text_untagged_base;
        base = 0;
        type = *(char **)(base + 4);
        goto text_type_done;
text_untagged_base:
        base = obj;
        type = *(char **)(base + 4);
text_type_done:
        cTypeMethod *text = (cTypeMethod *)(type + 0x78);
        short textOff = text->offset;
        register void *textBase __asm__("$5") = (char *)base + textOff;
        __asm__ volatile("" : "+r"(textBase));
        void (*textFn)(void *, char *) = (void (*)(void *, char *))text->fn;
        textFn(textBase, buf);

        obj = mObject;
        int flag3 = 0;
        const char *fmt = (const char *)0x36F6CC;
        int tag2 = obj & 1;
        if (tag2 != 0) {
            flag3 = 1;
        }
        int base2 = obj;
        char *type2;
        if (flag3 != 0) {
            base2 = 0;
            type2 = *(char **)(base2 + 4);
        } else {
            type2 = *(char **)(base2 + 4);
        }
        cTypeMethod *name = (cTypeMethod *)(type2 + 0x80);
        short nameOff = name->offset;
        NamedObject *(*nameFn)(void *) = (NamedObject *(*)(void *))name->fn;
        cStrAppend(buf, fmt, nameFn((char *)base2 + nameOff)->name);
    } else {
        cStrAppend(buf, (const char *)0x36F6E0);
    }
}
