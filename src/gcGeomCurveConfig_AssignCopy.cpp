// ODR-WARNING: split-TU local redeclaration for one gcGeomCurveConfig method.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

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

class gcEntityGeomConfig {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcGeomCurveConfig {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

class gcDesiredEntityHelper {};

template<class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

struct ReleaseEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct CloneEntry {
    short offset;
    short pad;
    cBase *(*fn)(void *, cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_0009F43C;
extern cType *D_0009F784;

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fRgcGeomCurveConfigKAssignCopyPC6FcBase, 0x33c\n");

void gcGeomCurveConfig::AssignCopy(const cBase *base) {
    const gcGeomCurveConfig *other = 0;
    char *destTagged = (char *)this + 0x84;

    if (base != 0) {
        if (D_0009F784 == 0) {
            if (D_0009F43C == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F43C = cType::InitializeType(
                    0, 0, 0xA0, D_000385DC, &gcEntityGeomConfig::New,
                    0, 0, 0);
            }
            D_0009F784 = cType::InitializeType(
                0, 0, 0x1D6, D_0009F43C, &gcGeomCurveConfig::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = D_0009F784;
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
            other = (const gcGeomCurveConfig *)base;
        }
    }

    // Field at +0x08 (4 bytes)
    {
        unsigned int *dst = (unsigned int *)((char *)this + 8);
        const unsigned int *src = (const unsigned int *)((const char *)other + 8);
        unsigned int word = *src;
        *dst = word;
    }

    // Fields at +0x0C..+0x20 (6 words)
    {
        const unsigned int *src =
            (const unsigned int *)((const char *)other + 0xC);
        unsigned int *dst = (unsigned int *)((char *)this + 0xC);
        unsigned int w0 = src[0];
        unsigned int w1 = src[1];
        unsigned int w2 = src[2];
        dst[0] = w0;
        unsigned int w3 = src[3];
        dst[1] = w1;
        unsigned int w4 = src[4];
        dst[2] = w2;
        unsigned int w5 = src[5];
        dst[3] = w3;
        dst[4] = w4;
        dst[5] = w5;
    }

    // Field at +0x24 (4 bytes)
    {
        unsigned int *dst = (unsigned int *)((char *)this + 0x24);
        const unsigned int *src = (const unsigned int *)((const char *)other + 0x24);
        unsigned int word = *src;
        *dst = word;
    }

    // VFPU quad copies at +0x30 and +0x40
    *(SceULong128 *)((char *)this + 0x30) =
        *(const SceULong128 *)((const char *)other + 0x30);
    *(SceULong128 *)((char *)this + 0x40) =
        *(const SceULong128 *)((const char *)other + 0x40);

    // Byte at +0x50
    *(unsigned char *)((char *)this + 0x50) =
        *(const unsigned char *)((const char *)other + 0x50);

    // Byte at +0x60, word at +0x64, gcEvent op= at +0x68
    *(unsigned char *)((char *)this + 0x60) =
        *(const unsigned char *)((const char *)other + 0x60);
    *(unsigned int *)((char *)this + 0x64) =
        *(const unsigned int *)((const char *)other + 0x64);
    ((gcEvent *)((char *)this + 0x68))->operator=(
        *(const gcEvent *)((const char *)other + 0x68));

    // Tagged-base handling at +0x84
    if ((const char *)other + 0x84 != destTagged) {
        int oldValue = *(int *)((char *)this + 0x84);
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
                stored = stored | 1;
            } else {
                stored = *(int *)oldValue;
                stored = stored | 1;
            }
            *(int *)((char *)this + 0x84) = stored;
            if (oldRaw != 0) {
                ReleaseEntry *release =
                    (ReleaseEntry *)(*(char **)(oldRaw + 4) + 0x50);
                short releaseOffset = release->offset;
                void (*releaseFn)(void *, int) = release->fn;
                releaseFn((char *)oldRaw + releaseOffset, 3);
            }
        }

        int srcValue = *(int *)((const char *)other + 0x84);
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
            cMemPool *pool = cMemPool::GetPoolFromPtr(destTagged);
            int current = *(int *)((char *)this + 0x84);
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
            *(cBase **)((char *)this + 0x84) =
                clone->fn(cloneBase, pool, cloneArg);
        }
    }

    // Field at +0x88 (4 bytes)
    {
        unsigned int *dst = (unsigned int *)((char *)this + 0x88);
        const unsigned int *src = (const unsigned int *)((const char *)other + 0x88);
        unsigned int word = *src;
        *dst = word;
    }

    // Byte at +0x8C
    *(unsigned char *)((char *)this + 0x8C) =
        *(const unsigned char *)((const char *)other + 0x8C);

    // Field at +0x90 (4 bytes - cHandle)
    {
        unsigned int *dst = (unsigned int *)((char *)this + 0x90);
        const unsigned int *src = (const unsigned int *)((const char *)other + 0x90);
        unsigned int word = *src;
        *dst = word;
    }

    // Field at +0x94 (4 bytes)
    {
        unsigned int *dst = (unsigned int *)((char *)this + 0x94);
        const unsigned int *src = (const unsigned int *)((const char *)other + 0x94);
        unsigned int word = *src;
        *dst = word;
    }

    // cArrayBase<gcDesiredEntityHelper>::operator= at +0x98
    ((cArrayBase<gcDesiredEntityHelper> *)((char *)this + 0x98))->operator=(
        *(const cArrayBase<gcDesiredEntityHelper> *)((const char *)other + 0x98));
}
