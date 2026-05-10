// gcValEntitySoundFrequency_AssignCopy.cpp - split TU for AssignCopy.
// ODR-WARNING: this TU re-declares gcValEntitySoundFrequency locally with
// only AssignCopy; the canonical class definition lives in
// src/gcValEntitySoundFrequency.cpp. The type cache statics use asm() linkage
// to point to the same memory used by gcValEntitySoundFrequency::GetType.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

struct DispatchEntry {
    short offset;
    short pad;
    class cType *(*fn)(void *, short, void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct CloneSlot {
    short offset;
    short pad;
    void *(*fn)(void *, cMemPool *, int);
};

class cType {
public:
    char _p0[0x1C];
    cType *mParent;

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

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntitySoundFrequency {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_lvalue asm("D_0009F3EC");
static cType *type_gcValEntitySoundFrequency asm("D_0009F844");

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0fZgcValEntitySoundFrequencyKAssignCopyPC6FcBase, 0x304\n");

void gcValEntitySoundFrequency::AssignCopy(const cBase *base) {
    const gcValEntitySoundFrequency *other = 0;

    if (base != 0) {
        if (!type_gcValEntitySoundFrequency) {
            if (!type_lvalue) {
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
                type_lvalue = cType::InitializeType(
                    0, 0, 0x6D, type_value, 0, 0, 0, 0);
            }
            type_gcValEntitySoundFrequency = cType::InitializeType(
                0, 0, 0x1E2, type_lvalue,
                gcValEntitySoundFrequency::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *wanted = type_gcValEntitySoundFrequency;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
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
            other = (const gcValEntitySoundFrequency *)base;
        }
    }

    const gcDesiredEntity *otherEntity =
        (const gcDesiredEntity *)((const char *)other + 8);
    __asm__ volatile("" :: "r"(otherEntity) : "memory");
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*otherEntity);

    if (((const char *)other + 0x34) != ((char *)this + 0x34)) {
        int val = *(int *)((char *)this + 0x34);
        int tagged = val & 1;
        int owned = 1;
        if (tagged != 0) {
            owned = 0;
        }
        if (owned != 0) {
            int wasTagged = 0;
            if (tagged != 0) {
                wasTagged = 1;
            }
            int parent;
            if (wasTagged != 0) {
                parent = val & ~1;
            } else {
                parent = *(int *)val;
            }
            *(int *)((char *)this + 0x34) = parent | 1;
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
            }
        }

        int oVal = *(int *)((const char *)other + 0x34);
        int oTagged = oVal & 1;
        int oOwned = 1;
        if (oTagged != 0) {
            oOwned = 0;
        }
        if (oOwned != 0) {
            char *typeInfo = *(char **)(oVal + 4);
            CloneSlot *slot = (CloneSlot *)(typeInfo + 0x10);
            short offset = slot->offset;
            cMemPool *pool = cMemPool::GetPoolFromPtr((char *)this + 0x34);
            int curVal = *(int *)((char *)this + 0x34);
            void *(*cloneFn)(void *, cMemPool *, int) = slot->fn;
            int curTagged = 0;
            if (curVal & 1) {
                curTagged = 1;
            }
            int curParent;
            if (curTagged != 0) {
                curParent = curVal & ~1;
            } else {
                curParent = *(int *)curVal;
            }
            *(int *)((char *)this + 0x34) = (int)cloneFn(
                (char *)oVal + offset, pool, curParent);
        }
    }

    *((unsigned char *)this + 0x38) = *((const unsigned char *)other + 0x38);
    *((unsigned char *)this + 0x39) = *((const unsigned char *)other + 0x39);
    *((unsigned char *)this + 0x3A) = *((const unsigned char *)other + 0x3A);
}
