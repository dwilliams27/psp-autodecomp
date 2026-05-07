// ODR-WARNING: split-TU local redeclaration for one gcValEntityPathDistance method.

class cBase;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityPathDistance {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
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

extern cType *type_base asm("D_000385DC");
extern cType *type_expression asm("D_000385D8");
extern cType *type_value asm("D_0009F3E8");
extern cType *type_gcValEntityPathDistance asm("D_0009F838");

void gcValEntityPathDistance::AssignCopy(const cBase *base) {
    const gcValEntityPathDistance *other = 0;
    char *destValue = (char *)this + 0x40;

    if (base != 0) {
        if (!type_gcValEntityPathDistance) {
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
            type_gcValEntityPathDistance = cType::InitializeType(
                0, 0, 0xE3, type_value, gcValEntityPathDistance::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *wanted = type_gcValEntityPathDistance;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *type = entry->fn((void *)((const char *)base + offset));
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
            other = (const gcValEntityPathDistance *)base;
        }
    }

    const gcDesiredEntity *srcEntity =
        (const gcDesiredEntity *)((const char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcEntity);
    *(int *)((char *)this + 0x34) =
        *(const int *)((const char *)other + 0x34);
    *(int *)((char *)this + 0x38) =
        *(const int *)((const char *)other + 0x38);
    *(int *)((char *)this + 0x3C) =
        *(const int *)((const char *)other + 0x3C);

    if ((const char *)other + 0x40 != destValue) {
        int oldValue = *(int *)((char *)this + 0x40);
        int keep = 1;
        int tag = oldValue & 1;
        if (tag != 0) {
            keep = 0;
        }
        int tagCopy = tag;
        if (keep != 0) {
            int tagged = 0;
            int stored;
            int oldRaw = oldValue;
            if (tagCopy != 0) {
                tagged = 1;
            }
            if (tagged != 0) {
                stored = oldValue & ~1;
                __asm__ volatile("" : "+r"(stored));
                stored = stored | 1;
            } else {
                stored = *(int *)oldValue;
                stored = stored | 1;
            }
            *(int *)((char *)this + 0x40) = stored;
            if (oldRaw != 0) {
                ReleaseEntry *release =
                    (ReleaseEntry *)(*(char **)(oldRaw + 4) + 0x50);
                short releaseOffset = release->offset;
                void (*releaseFn)(void *, int) = release->fn;
                releaseFn((char *)oldRaw + releaseOffset, 3);
            }
        }

        int srcValue = *(int *)((const char *)other + 0x40);
        int cloneIt = 1;
        int srcTag = srcValue & 1;
        if (srcTag != 0) {
            cloneIt = 0;
        }
        if (cloneIt != 0) {
            char *srcClassDesc = *(char **)(srcValue + 4);
            CloneEntry *clone = (CloneEntry *)(srcClassDesc + 0x10);
            short cloneOffset = clone->offset;
            void *cloneBase = (char *)srcValue + cloneOffset;
            cMemPool *pool = cMemPool::GetPoolFromPtr(destValue);
            int current = *(int *)((char *)this + 0x40);
            int currentTagged = 0;
            cBase *cloneArg;
            if (current & 1) {
                currentTagged = 1;
            }
            if (currentTagged != 0) {
                cloneArg = (cBase *)(current & ~1);
            } else {
                cloneArg = *(cBase **)current;
            }
            *(cBase **)((char *)this + 0x40) =
                clone->fn(cloneBase, pool, cloneArg);
        }
    }
}
