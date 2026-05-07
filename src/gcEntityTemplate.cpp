inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
};

class cHandle;
template <typename T>
class cArray;
template <typename T>
class cHandleT;
template <typename T>
class cSubHandleT;
template <typename A, typename B>
class cHandlePairT;
class gcEnumeration;
class gcEnumerationEntry;

template <typename T>
class cArrayBase {
public:
    cArrayBase<T> &operator=(const cArrayBase<T> &);
};

class gcEntity;

class cMemBlockAllocation {
public:
    cMemBlockAllocation(void *, bool);
    ~cMemBlockAllocation();
    char _pad[0x18];
};

extern cType *D_000385DC;
extern cType *D_000385D8;
extern cType *D_000385D4;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F448;
extern cType *D_0009F6A8;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcEntityTemplate {
public:
    gcEntityTemplate(cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    static void FreeDynamicInstance(gcEntity *);
};

class gcDoMouseOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
};

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

struct DtorEntry {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

struct CopyEntry {
    short offset;
    short _pad;
    int (*fn)(void *, void *, int);
};

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

class gcExternalVariable {
public:
    gcExternalVariable(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcGeomCurveConfig {
public:
    gcGeomCurveConfig(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// gcEntityTemplate::GetType(void) const @ 0x002688a8
const cType *gcEntityTemplate::GetType(void) const {
    if (D_0009F448 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F448 = cType::InitializeType(0, 0, 0x8E, D_000385E4,
                                           &gcEntityTemplate::New,
                                           (const char *)0x36D9B8,
                                           (const char *)0x36D9C8,
                                           5);
    }
    return D_0009F448;
}

// gcEntityTemplate::AssignCopy(const cBase *) @ 0x00268544
void gcEntityTemplate::AssignCopy(const cBase *base) {
    const gcEntityTemplate *other = 0;

    if (base != 0) {
        if (D_0009F448 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(
                    0, 0, 3, D_000385E0, 0, 0, 0, 0);
            }
            D_0009F448 = cType::InitializeType(
                0, 0, 0x8E, D_000385E4, &gcEntityTemplate::New,
                (const char *)0x36D9B8, (const char *)0x36D9C8, 5);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = D_0009F448;
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
            other = (const gcEntityTemplate *)base;
        }
    }

    ((cObject *)this)->operator=(*(const cObject *)other);
    *(unsigned int *)((char *)this + 0x44) =
        *(const unsigned int *)((const char *)other + 0x44);
    unsigned int *dst48 = (unsigned int *)((char *)this + 0x48);
    const char *src48 = (const char *)other + 0x48;
    unsigned int word48 = *(const unsigned int *)src48;
    *dst48 = word48;
    ((cBaseArray *)((char *)this + 0x4C))->operator=(
        *(const cBaseArray *)((const char *)other + 0x4C));
    ((cBaseArray *)((char *)this + 0x54))->operator=(
        *(const cBaseArray *)((const char *)other + 0x54));
    ((cBaseArray *)((char *)this + 0x5C))->operator=(
        *(const cBaseArray *)((const char *)(int)other + 0x5C));
    ((cBaseArray *)((char *)this + 0x64))->operator=(
        *(const cBaseArray *)((const char *)other + 0x64));
    ((cBaseArray *)((char *)this + 0x6C))->operator=(
        *(const cBaseArray *)((const char *)other + 0x6C));
    *(signed char *)((char *)this + 0x74) =
        *(const signed char *)((const char *)other + 0x74);
    *(unsigned char *)((char *)this + 0x75) =
        *(const unsigned char *)((const char *)other + 0x75);
    ((cBaseArray *)((char *)this + 0x78))->operator=(
        *(const cBaseArray *)((const char *)other + 0x78));
    ((cBaseArray *)((char *)this + 0x80))->operator=(
        *(const cBaseArray *)((const char *)other + 0x80));
    ((cArrayBase<cHandleT<gcEnumeration> > *)((char *)this + 0x88))->operator=(
        *(const cArrayBase<cHandleT<gcEnumeration> > *)((const char *)other + 0x88));
    ((cArrayBase<cHandleT<gcEnumeration> > *)((char *)this + 0x8C))->operator=(
        *(const cArrayBase<cHandleT<gcEnumeration> > *)((const char *)other + 0x8C));
    ((cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x90))->operator=(
        *(const cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((const char *)other + 0x90));
    ((cArrayBase<cArray<cHandle> > *)((char *)this + 0x94))->operator=(
        *(const cArrayBase<cArray<cHandle> > *)((const char *)other + 0x94));

    const unsigned int *src98 =
        (const unsigned int *)((const char *)other + 0x98);
    unsigned int *dst98 = (unsigned int *)((char *)this + 0x98);
    unsigned int w98_4 = src98[1];
    unsigned int w98_0 = src98[0];
    unsigned int w98_8 = src98[2];
    dst98[0] = w98_0;
    unsigned int w98_C = src98[3];
    dst98[1] = w98_4;
    unsigned int w98_10 = src98[4];
    dst98[2] = w98_8;
    unsigned int w98_14 = src98[5];
    dst98[3] = w98_C;
    dst98[4] = w98_10;
    dst98[5] = w98_14;

    const unsigned int *srcB0 =
        (const unsigned int *)((const char *)other + 0xB0);
    unsigned int *dstB0 = (unsigned int *)((char *)this + 0xB0);
    unsigned int wB0_4 = srcB0[1];
    unsigned int wB0_0 = srcB0[0];
    unsigned int wB0_8 = srcB0[2];
    dstB0[0] = wB0_0;
    unsigned int wB0_C = srcB0[3];
    dstB0[1] = wB0_4;
    unsigned int wB0_10 = srcB0[4];
    dstB0[2] = wB0_8;
    unsigned int wB0_14 = srcB0[5];
    dstB0[3] = wB0_C;
    dstB0[4] = wB0_10;
    dstB0[5] = wB0_14;

    unsigned int wC8 = *(const unsigned int *)((const char *)other + 0xC8);
    const unsigned int *srcCC =
        (const unsigned int *)((const char *)other + 0xCC);
    unsigned int *dstCC = (unsigned int *)((char *)this + 0xCC);
    *(unsigned int *)((char *)this + 0xC8) = wC8;
    unsigned int wCC = *srcCC;
    *dstCC = wCC;
}

__asm__(".size __0fQgcEntityTemplateKAssignCopyPC6FcBase, 0x2e8\n");

// gcEntityTemplate::New(cMemPool *, cBase *) static @ 0x0026882c
cBase *gcEntityTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEntityTemplate *result = 0;
    gcEntityTemplate *obj = (gcEntityTemplate *)rec->fn(base, 0xD0, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEntityTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

// gcEntityTemplate::FreeDynamicInstance(gcEntity *) static @ 0x001275cc
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fQgcEntityTemplateTFreeDynamicInstanceP6IgcEntityT, 0x64\n");

void gcEntityTemplate::FreeDynamicInstance(gcEntity *entity) {
    cMemBlockAllocation alloc(entity, true);
    if (entity != 0) {
        int *vt = (int *)(((char **)entity)[1] + 0x50);
        short thunk = *(short *)vt;
        ((void (*)(char *, int))vt[1])((char *)entity + thunk, 3);
    }
}

// gcExternalVariable::New(cMemPool *, cBase *) static @ 0x0027c0b4
cBase *gcExternalVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcExternalVariable *result = 0;
    gcExternalVariable *obj = (gcExternalVariable *)rec->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcExternalVariable(parent);
        result = obj;
    }
    return (cBase *)result;
}

// gcGeomCurveConfig::New(cMemPool *, cBase *) static @ 0x0031bd08
cBase *gcGeomCurveConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcGeomCurveConfig *result = 0;
    gcGeomCurveConfig *obj = (gcGeomCurveConfig *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcGeomCurveConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fLgcDoMouseOpKAssignCopyPC6FcBase, 0x300\n");

// gcDoMouseOp::AssignCopy(const cBase *) @ 0x002e7518
void gcDoMouseOp::AssignCopy(const cBase *base) {
    const gcDoMouseOp *other = 0;
    register char *desiredThis __asm__("$23") = (char *)this + 0x20;

    if (base != 0) {
        if (D_0009F6A8 == 0) {
            if (D_000385D4 == 0) {
                if (D_000385D8 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385D8 = cType::InitializeType(
                        0, 0, 0x6A, D_000385DC, 0, 0, 0, 0);
                }
                D_000385D4 = cType::InitializeType(
                    0, 0, 0x6B, D_000385D8, 0, 0, 0, 0);
            }
            D_0009F6A8 = cType::InitializeType(
                0, 0, 0x20C, D_000385D4, &gcDoMouseOp::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = D_0009F6A8;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_mouse_target;
        }
        isValid = 0;
        goto mouse_cast_done;

have_mouse_target:
        if (type != 0) {
mouse_loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)*((void **)((char *)type + 0x1C));
                if (type != 0) {
                    goto mouse_loop_cast;
                }
                goto mouse_invalid_cast;
            }
        } else {
mouse_invalid_cast:
            isValid = 0;
        }

mouse_cast_done:
        if (isValid != 0) {
            other = (const gcDoMouseOp *)base;
        }
    }

    unsigned int flags = *(unsigned int *)((char *)this + 8) & ~3U;
    *(unsigned int *)((char *)this + 8) = flags;
    flags = flags | (*(const unsigned int *)((const char *)other + 8) & 3U);
    *(unsigned int *)((char *)this + 8) = flags;
    *(unsigned int *)((char *)this + 0x0C) =
        *(const unsigned int *)((const char *)other + 0x0C);
    const char *src10 = (const char *)other + 0x10;
    char *dst10 = (char *)this + 0x10;
    const char *src14 = (const char *)other + 0x14;
    *(unsigned int *)dst10 = *(const unsigned int *)src10;
    char *dst14 = (char *)this + 0x14;
    const char *src20 = (const char *)other + 0x20;
    *(unsigned int *)dst14 = *(const unsigned int *)src14;

    if (src20 != desiredThis) {
        register int oldValue __asm__("$6") = *(int *)((char *)this + 0x20);
        register int ownsOld __asm__("$5") = 1;
        register int oldTagged __asm__("$4") = oldValue & 1;
        if (oldTagged != 0) {
            ownsOld = 0;
        }
        if (ownsOld != 0) {
            int oldWasTagged = 0;
            if (oldTagged != 0) {
                oldWasTagged = 1;
            }

            int replacement;
            if (oldWasTagged != 0) {
                replacement = oldValue & ~1;
            } else {
                replacement = *(int *)oldValue;
            }
            replacement = replacement | 1;
            *(int *)((char *)this + 0x20) = replacement;

            if (oldValue != 0) {
                char *classDesc = *(char **)(oldValue + 4);
                DtorEntry *entry = (DtorEntry *)(classDesc + 0x50);
                __asm__ volatile("" ::: "memory");
                short offset = entry->offset;
                void (*fn)(void *, int) = entry->fn;
                fn((unsigned char *)oldValue + offset, 3);
            }
        }

        int srcValue = *(const int *)((const char *)other + 0x20);
        int ownsSrc = 1;
        int srcTagged = srcValue & 1;
        if (srcTagged != 0) {
            ownsSrc = 0;
        }
        if (ownsSrc != 0) {
            char *classDesc = *(char **)(srcValue + 4);
            CopyEntry *entry = (CopyEntry *)(classDesc + 0x10);
            int useCurrent = 0;
            int current = *(int *)((char *)this + 0x20);
            short offset = entry->offset;
            int currentTagged = current & 1;
            srcValue = srcValue + offset;
            void *pool = cMemPool_GetPoolFromPtr(desiredThis);
            int currentBase;
            if (currentTagged != 0) {
                useCurrent = 1;
            }
            if (useCurrent != 0) {
                currentBase = current & ~1;
            } else {
                currentBase = *(int *)current;
            }
            *(int *)((char *)this + 0x20) =
                entry->fn((void *)srcValue, pool, currentBase);
        }
    }

    char *dst24 = (char *)this + 0x24;
    __asm__ volatile("" ::: "memory");
    const char *src24 = (const char *)other + 0x24;
    *(unsigned int *)(dst24 + 0x00) = *(const unsigned int *)(src24 + 0x00);
    *(unsigned int *)(dst24 + 0x04) = *(const unsigned int *)(src24 + 0x04);
}
