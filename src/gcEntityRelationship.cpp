// gcEntityRelationship — gcAll_psp.obj
//   0x00152fe4 Write(cFile &) const                       (88B)
//   0x0015303c Read(cFile &, cMemPool *)                  (200B)
//   0x0031aa00 New(cMemPool *, cBase *) static            (220B)
//
// Layout: derives from gcObjectRelationship, embeds gcDesiredEntityHelper
// at offset 0x20.  Total size 0x28.

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cType {
public:
    int _pad[7];
    const cType *mParent;
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
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

class gcObjectRelationship {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcDesiredEntityHelper {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

void gcDesiredEntityHelper_gcDesiredEntityHelper(void *, int, int, int);

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcEntityRelationship : public gcObjectRelationship {
public:
    ~gcEntityRelationship();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static gcEntityRelationship *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

struct CloneEntry {
    short offset;
    short pad;
    unsigned int (*fn)(void *, cMemPool *, unsigned int);
};

struct TwoWords {
    unsigned int first;
    unsigned int second;
};

extern cType *D_000385DC;
extern cType *D_0009F3F0;
extern cType *D_0009F774;

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oUgcEntityRelationshipdtv, 0xf0\n");

// 0x0031add4 — ~gcEntityRelationship(void)
gcEntityRelationship::~gcEntityRelationship() {
    *(char **)((char *)this + 4) = (char *)0x388928;
    char *baseVtable = (char *)0x37E6A8;
    char *sub = (char *)this + 8;
    if (sub != 0) {
        *(char **)((char *)this + 0xC) = (char *)0x388568;
        char *slot = (char *)this + 0x1C;
        if (slot != 0) {
            int keep = 1;
            int val = *(int *)((char *)this + 0x1C);
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
                *(int *)((char *)this + 0x1C) = 0;
            }
        }
        *(char **)((char *)this + 0xC) = baseVtable;
    }
    *(char **)((char *)this + 4) = baseVtable;
}

// 0x00152fe4 — Write(cFile &) const
void gcEntityRelationship::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcObjectRelationship::Write(file);
    ((const gcDesiredEntityHelper *)((const char *)this + 0x20))->Write(wb);
    wb.End();
}

// 0x0015303c — Read(cFile &, cMemPool *)
int gcEntityRelationship::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && gcObjectRelationship::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredEntityHelper *)((char *)this + 0x20))->Read(rb);
    return result;
}

// 0x0031aa00 — New(cMemPool *, cBase *) static
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcEntityRelationship *gcEntityRelationship::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcEntityRelationship *result = 0;
    int *p = (int *)entry->fn(base, 0x28, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4)  = 0x388928;
        *(int *)((char *)p + 12) = 0x37E6A8;
        *(int **)((char *)p + 8) = p;
        *(int *)((char *)p + 12) = 0x388568;
        *((unsigned char *)p + 16) = 1;
        *((unsigned char *)p + 17) = 0;
        *(int *)((char *)p + 20) = 0;
        *(int *)((char *)p + 24) = 0;
        *(int *)((char *)p + 28) = (int)p | 1;
        *(int *)((char *)p + 4)  = 0x38C7A0;
        gcDesiredEntityHelper_gcDesiredEntityHelper((char *)p + 0x20, 1, 0, 0);
        result = (gcEntityRelationship *)p;
    }
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fUgcEntityRelationshipKAssignCopyPC6FcBase, 0x2a8\n");

// 0x0031a758 -- gcEntityRelationship::AssignCopy(const cBase *)
void gcEntityRelationship::AssignCopy(const cBase *base) {
    const gcEntityRelationship *other = 0;
    void *slot = (char *)this + 0x1C;

    if (base != 0) {
        if (D_0009F774 == 0) {
            if (D_0009F3F0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F3F0 = cType::InitializeType(
                    0, 0, 0x131, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F774 = cType::InitializeType(
                0, 0, 0x132, D_0009F3F0,
                (cBase *(*)(cMemPool *, cBase *))&gcEntityRelationship::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = D_0009F774;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
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
                type = (cType *)type->mParent;
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
            other = (const gcEntityRelationship *)base;
        }
    }

    *(signed char *)((char *)this + 0x10) =
        *(const signed char *)((const char *)other + 0x10);
    const char *src14 = (const char *)other + 0x14;
    *(signed char *)((char *)this + 0x11) =
        *(const signed char *)((const char *)other + 0x11);
    char *dst14 = (char *)this + 0x14;
    *(unsigned int *)dst14 = *(const unsigned int *)src14;
    src14 = (const char *)other + 0x18;
    char *dst18 = (char *)this + 0x18;
    const char *srcSlot = (const char *)other + 0x1C;
    *(unsigned int *)dst18 = *(const unsigned int *)src14;

    if (srcSlot != slot) {
        register unsigned int oldValue __asm__("$6") =
            *(unsigned int *)((char *)this + 0x1C);
        int oldOwned = 1;
        register unsigned int oldTagged __asm__("$4") = oldValue & 1;
        if (oldTagged != 0) {
            oldOwned = 0;
        }
        if (oldOwned != 0) {
            register unsigned int oldPtr __asm__("$4") = oldValue;
            register unsigned int tagCopy __asm__("$7") = oldTagged;
            register unsigned int releaseValue __asm__("$6");
            int oldUntagged = 0;
            if (tagCopy != 0) {
                oldUntagged = 1;
            }
            if (oldUntagged != 0) {
                releaseValue = oldValue & ~1U;
                releaseValue = releaseValue | 1;
            } else {
                releaseValue = *(unsigned int *)oldValue;
                releaseValue = releaseValue | 1;
            }
            *(unsigned int *)((char *)this + 0x1C) = releaseValue;
            if (oldPtr != 0) {
                void *classDesc = *(void **)((char *)oldPtr + 4);
                DtorDeleteRecord *entry =
                    (DtorDeleteRecord *)((char *)classDesc + 0x50);
                short offset = entry->offset;
                void (*fn)(void *, void *) = entry->fn;
                fn((char *)oldPtr + offset, (void *)3);
            }
        }

        unsigned int newValue = *(const unsigned int *)((const char *)other + 0x1C);
        int newOwned = 1;
        unsigned int newTagged = newValue & 1;
        if (newTagged != 0) {
            newOwned = 0;
        }
        if (newOwned != 0) {
            void *classDesc = *(void **)((char *)newValue + 4);
            CloneEntry *entry = (CloneEntry *)((char *)classDesc + 0x10);
            short offset = entry->offset;
            char *source = (char *)newValue + offset;
            cMemPool *pool = cMemPool::GetPoolFromPtr(slot);
            unsigned int current = *(unsigned int *)((char *)this + 0x1C);
            int currentUntagged = 0;
            unsigned int (*fn)(void *, cMemPool *, unsigned int) = entry->fn;
            if ((current & 1) != 0) {
                currentUntagged = 1;
            }
            unsigned int currentValue;
            if (currentUntagged != 0) {
                currentValue = current & ~1U;
            } else {
                currentValue = *(unsigned int *)current;
            }
            *(unsigned int *)((char *)this + 0x1C) =
                fn(source, pool, currentValue);
        }
    }

    const TwoWords *src20 = (const TwoWords *)((const char *)other + 0x20);
    TwoWords *dst20 = (TwoWords *)((char *)this + 0x20);
    unsigned int word20 = src20->first;
    unsigned int word24 = src20->second;
    dst20->first = word20;
    dst20->second = word24;
}
