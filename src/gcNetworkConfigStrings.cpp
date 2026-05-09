// gcNetworkConfigStrings — gcAll_psp.obj
//   0x00285de8 New(cMemPool *, cBase *) static            (144B)
//   0x00285f54 Write(cFile &) const                       (100B)
//
// Layout: derives from gcStringValue, embeds gcDesiredValue at offset 0x8,
// has int member at 0xC. Total size 0x10. Type ID for Write block = 10.

class cBase;
class cFile;
class cFileHandle;
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

class cBase {
public:
    int _b0;
    int _b4;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char gcNetworkConfigStringsvirtualtable[];
extern char cBaseclassdesc[];

class gcStringValue : public cBase {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
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

class gcNetworkConfigStrings : public gcStringValue {
public:
    int mField08;
    int mField0C;

    ~gcNetworkConfigStrings();
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static gcNetworkConfigStrings *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F554;

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

// 0x00285fb8 — Read(cFile &, cMemPool *)
int gcNetworkConfigStrings::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 10, true);
    if (rb[3] != 10 || gcStringValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    ((gcDesiredValue *)((char *)this + 8))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x00285f54 — Write(cFile &) const
void gcNetworkConfigStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 10);
    gcStringValue::Write(file);
    wb.Write(*(const int *)((const char *)this + 0xC));
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// 0x00285de8 — New(cMemPool *, cBase *) static
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcNetworkConfigStrings *gcNetworkConfigStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcNetworkConfigStrings *result = 0;
    int *p = (int *)entry->fn(base, 0x10, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4)  = (int)gcNetworkConfigStringsvirtualtable;
        *(int *)((char *)p + 8)  = (int)p | 1;
        *(int *)((char *)p + 0xC) = 0;
        result = (gcNetworkConfigStrings *)p;
    }
    return result;
}

// 0x002861b4 — GetName(char *) const
void gcNetworkConfigStrings::GetName(char *buf) const {
    register int field __asm__("$6") = *(int *)((const char *)this + 0x0C);
    if (field == 0) {
        cStrAppend(buf, (const char *)0x36E2BC);

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

        cStrCat(buf, (const char *)0x36E2CC);
    }
    cStrCat(buf, (const char *)0x36DAF0);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oWgcNetworkConfigStringsdtv, 0xd4\n");

// 0x0028652c - gcNetworkConfigStrings::~gcNetworkConfigStrings(void)
gcNetworkConfigStrings::~gcNetworkConfigStrings() {
    *(char **)((char *)this + 4) = gcNetworkConfigStringsvirtualtable;
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

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fWgcNetworkConfigStringsKAssignCopyPC6FcBase, 0x270\n");

// 0x00285b78 — AssignCopy(const cBase *)
void gcNetworkConfigStrings::AssignCopy(const cBase *base) {
    const gcNetworkConfigStrings *other = 0;

    if (base != 0) {
        if (D_0009F554 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(
                    0, 0, 0x170, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F554 = cType::InitializeType(
                0, 0, 0xF8, D_0009F454,
                (cBase *(*)(cMemPool *, cBase *))&gcNetworkConfigStrings::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F554;
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
            other = (const gcNetworkConfigStrings *)base;
        }
    }

    int finalField;

    if ((char *)other + 0x08 != (char *)this + 0x08) {
        goto copy_desired;
    }
    finalField = other->mField0C;
    goto done;

copy_desired:
    {
        int value = mField08;
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
            mField08 = value;
            if (old != 0) {
                ReleaseEntry *release =
                    (ReleaseEntry *)(*(char **)(old + 4) + 0x50);
                release->fn((char *)old + release->offset, 3);
            }
        }

        register int srcValue __asm__("$4") = other->mField08;
        register int srcFlag __asm__("$5") = 1;
        register int srcTag __asm__("$6") = srcValue & 1;
        if (srcTag != 0) {
            srcFlag = 0;
        }
        if (srcFlag != 0) {
            register int source __asm__("$17") = srcValue;
            register CloneEntry *clone __asm__("$19") =
                (CloneEntry *)(*(char **)(source + 4) + 0x10);
            short cloneOffset = clone->offset;
            register void *target __asm__("$17") = (char *)source + cloneOffset;
            register cMemPool *pool __asm__("$4") =
                cMemPool::GetPoolFromPtr((char *)this + 0x08);
            register int current __asm__("$22") =
                mField08;
            register int currentFlag __asm__("$5") = 0;
            if (current & 1) {
                currentFlag = 1;
            }
            if (currentFlag != 0) {
                current &= ~1;
            } else {
                current = *(int *)current;
            }
            mField08 = (int)clone->fn(target, pool, (cBase *)current);
        }
        finalField = other->mField0C;
    }
done:
    mField0C = finalField;
}
