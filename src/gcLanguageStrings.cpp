// gcLanguageStrings — gcAll_psp.obj
//   0x0027f17c New(cMemPool *, cBase *) static            (140B)
//   0x0027f2e4 Write(cFile &) const                       (88B)
//   0x0027f33c Read(cFile &, cMemPool *)                  (200B)
//   0x0027f804 ~gcLanguageStrings(void)                   (212B)
//
// Layout: derives from gcStringValue, embeds gcDesiredValue at offset 0x8.
// Total size 0xC.

class cBase;
class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
void cStrCopy(char *, const char *);

extern char gcLanguageStringsvirtualtable[];
extern char cBaseclassdesc[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4E4;

class gcStringValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
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

struct GetNameSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

class gcLanguageStrings : public gcStringValue {
public:
    ~gcLanguageStrings();
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static gcLanguageStrings *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// 0x0027ef18 — AssignCopy(const cBase *)
void gcLanguageStrings::AssignCopy(const cBase *base) {
    const gcLanguageStrings *other = 0;
    char *slot = (char *)this + 0x08;
    gcLanguageStrings *self = this;

    if (base != 0) {
        if (D_0009F4E4 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(
                    0, 0, 0x170, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F4E4 = cType::InitializeType(
                0, 0, 0x1A9, D_0009F454,
                (cBase *(*)(cMemPool *, cBase *))&gcLanguageStrings::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F4E4;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)*((void **)((char *)type + 0x1C));
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcLanguageStrings *)base;
        }
    }

    if ((char *)other + 0x08 != slot) {
        int value = *(int *)((char *)self + 0x08);
        int flag = 1;
        int tag = value & 1;
        if (tag != 0) {
            flag = 0;
        }
        if (flag != 0) {
            int old = value;
            int flag2 = 0;
            if (tag != 0) {
                flag2 = 1;
            }
            if (flag2 != 0) {
                value &= ~1;
                value |= 1;
            } else {
                value = *(int *)value;
                value |= 1;
            }
            *(int *)((char *)self + 0x08) = value;
            if (old != 0) {
                ReleaseEntry *release =
                    (ReleaseEntry *)(*(char **)(old + 4) + 0x50);
                release->fn((char *)old + release->offset, 3);
            }
        }

        int srcValue = *(int *)((char *)other + 0x08);
        int srcFlag = 1;
        int srcTag = srcValue & 1;
        if (srcTag != 0) {
            srcFlag = 0;
        }
        if (srcFlag != 0) {
            int source = srcValue;
            CloneEntry *clone =
                (CloneEntry *)(*(char **)(source + 4) + 0x10);
            short cloneOffset = clone->offset;
            void *target = (char *)source + cloneOffset;
            cMemPool *pool = cMemPool::GetPoolFromPtr(slot);
            int current = *(int *)((char *)self + 0x08);
            int currentFlag = 0;
            if (current & 1) {
                currentFlag = 1;
            }
            if (currentFlag != 0) {
                current &= ~1;
            } else {
                current = *(int *)current;
            }
            *(int *)((char *)self + 0x08) =
                (int)clone->fn(target, pool, (cBase *)current);
        }
    }
}

// 0x0027f208 — GetType(void) const
const cType *gcLanguageStrings::GetType(void) const {
    if (D_0009F4E4 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4E4 = cType::InitializeType(
            0, 0, 0x1A9, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLanguageStrings::New, 0, 0, 0);
    }
    return D_0009F4E4;
}

// 0x0027f2e4 — Write(cFile &) const
void gcLanguageStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// 0x0027f33c — Read(cFile &, cMemPool *)
int gcLanguageStrings::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcStringValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredValue *)((char *)this + 8))->Read(rb);
    return result;
}

// 0x0027f4a4 — GetName(char *) const
void gcLanguageStrings::GetName(char *buf) const {
    cStrCopy(buf, (const char *)0x36E038);

    int val = *(int *)((const char *)this + 0x08);
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
        GetNameSlot *slot = (GetNameSlot *)(typeInfo + 0xD0);
        slot->fn((char *)val + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }

    cStrAppend(buf, (const char *)0x36DCEC);
}

// 0x0027f17c — New(cMemPool *, cBase *) static
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcLanguageStrings *gcLanguageStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLanguageStrings *result = 0;
    int *p = (int *)entry->fn(base, 0xC, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4)  = (int)gcLanguageStringsvirtualtable;
        *(int *)((char *)p + 8)  = (int)p | 1;
        result = (gcLanguageStrings *)p;
    }
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oRgcLanguageStringsdtv, 0xd4\n");

// 0x0027f804 - gcLanguageStrings::~gcLanguageStrings(void)
gcLanguageStrings::~gcLanguageStrings() {
    *(char **)((char *)this + 4) = gcLanguageStringsvirtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}
