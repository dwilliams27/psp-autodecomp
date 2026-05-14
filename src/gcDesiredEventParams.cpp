// gcDesiredEventParams.cpp - decompiled from gcAll_psp.obj.
//
// Functions:
//   0x00128e14 gcDesiredEventParams::Write(cFile &) const
//   0x0026a3a0 gcDesiredEventParams::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class cType;

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

class cBaseArray {
public:
    int _data[2];
    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

inline void *operator new(unsigned int, void *p) { return p; }

class gcDesiredEventParams {
public:
    char _pad[8];
    cBaseArray mParams0;
    cBaseArray mParams1;
    int mEventObject;

    gcDesiredEventParams(cBase *);
    void Write(cFile &) const;
    bool GetText(char *, bool) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
};

extern cType *D_000385DC;
extern cType *D_0009F45C;

class gcDesiredEnumerationEntry {
public:
    int GetTextEx(char *, bool, bool) const;
};

struct GetTextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct TypeDispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

struct DeleteDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, int, void *, short);
};

struct CloneDispatchEntry {
    short offset;
    short _pad;
    int (*fn)(void *, cMemPool *, int, void *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

// gcDesiredEventParams::Write(cFile &) const @ 0x00128e14
void gcDesiredEventParams::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    mParams0.Write(wb);
    mParams1.Write(wb);

    int val = mEventObject;
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

// gcDesiredEventParams::New(cMemPool *, cBase *) static @ 0x0026a3a0
cBase *gcDesiredEventParams::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDesiredEventParams *result = 0;
    gcDesiredEventParams *obj = (gcDesiredEventParams *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDesiredEventParams(parent);
        result = obj;
    }
    return (cBase *)result;
}

// gcDesiredEventParams::GetType(void) const @ 0x0026a41c
const cType *gcDesiredEventParams::GetType(void) const {
    if (D_0009F45C == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F45C = cType::InitializeType(0, 0, 0x153, D_000385DC,
                                           &gcDesiredEventParams::New, 0, 0, 0);
    }
    return D_0009F45C;
}

void gcDesiredEventParams::AssignCopy(const cBase *base) {
    const gcDesiredEventParams *other = 0;
    char *destSlot = (char *)this + 0x18;

    if (base != 0) {
        if (D_0009F45C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType(
                    (const char *)0x36D894, (const char *)0x36D89C,
                    1, 0, 0, 0, 0, 0);
            }
            D_0009F45C = cType::InitializeType(
                0, 0, 0x153, D_000385DC, &gcDesiredEventParams::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((const char *)base + 4);
        cType *target = D_0009F45C;
        TypeDispatchEntry *entry = (TypeDispatchEntry *)((char *)classDesc + 8);
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
                type = *(cType **)((char *)type + 0x1C);
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
            other = (const gcDesiredEventParams *)base;
        }
    }

    const cBaseArray &srcArr0 =
        *(const cBaseArray *)((const char *)other + 0x08);
    ((cBaseArray *)((char *)this + 0x08))->operator=(srcArr0);
    const cBaseArray &srcArr1 =
        *(const cBaseArray *)((const char *)other + 0x10);
    ((cBaseArray *)((char *)this + 0x10))->operator=(srcArr1);

    if ((const char *)other + 0x18 != destSlot) {
        int oldValue = *(int *)((char *)this + 0x18);
        int keepOld = 1;
        int oldTag = oldValue & 1;
        if (oldTag != 0) {
            keepOld = 0;
        }
        if (keepOld != 0) {
            int oldRaw = oldValue;
            int tagged = 0;
            if (oldTag != 0) {
                tagged = 1;
            }
            int stored;
            if (tagged != 0) {
                stored = oldValue & ~1;
                stored = stored | 1;
            } else {
                stored = *(int *)oldValue;
                stored = stored | 1;
            }
            *(int *)((char *)this + 0x18) = stored;
            if (oldRaw != 0) {
                DeleteDispatchEntry *rec =
                    (DeleteDispatchEntry *)(*(char **)(oldRaw + 4) + 0x50);
                short off = rec->offset;
                void (*fn)(void *, int, void *, short) = rec->fn;
                fn((char *)oldRaw + off, 3, (void *)fn, off);
            }
        }

        int srcValue = *(int *)((const char *)other + 0x18);
        int cloneIt = 1;
        int srcTag = srcValue & 1;
        if (srcTag != 0) {
            cloneIt = 0;
        }
        if (cloneIt != 0) {
            char *classDesc = *(char **)(srcValue + 4);
            CloneDispatchEntry *rec = (CloneDispatchEntry *)(classDesc + 0x10);
            short off = rec->offset;
            char *srcBase = (char *)srcValue + off;
            cMemPool *pool = cMemPool::GetPoolFromPtr(destSlot);
            int current = *(int *)((char *)this + 0x18);
            int tagged = 0;
            if (current & 1) {
                tagged = 1;
            }
            int cloneArg;
            if (tagged != 0) {
                cloneArg = current & ~1;
            } else {
                cloneArg = *(int *)current;
            }
            void *fn = (void *)rec->fn;
            *(int *)((char *)this + 0x18) = rec->fn(srcBase, pool, cloneArg, fn);
        }
    }
}

bool gcDesiredEventParams::GetText(char *buf, bool addSeparator) const {
    register const gcDesiredEventParams *self __asm__("$s0") = this;
    register char *out __asm__("$s1") = buf;
    register int count __asm__("$s2") = (((int)addSeparator) & 0xFF) != 0;
    register int index __asm__("$s3") = 0;
    register int offset __asm__("$s4") = 0;
    register const char *separator __asm__("$s5") = (const char *)0x36DAD8;
    register const char *format __asm__("$s6") = (const char *)0x36DF4C;
    register const char *noneText __asm__("$s7") = (const char *)0x36DAF0;
    __asm__("" : "+r"(self), "+r"(out), "+r"(count), "+r"(index),
            "+r"(offset), "+r"(separator), "+r"(format), "+r"(noneText));

    do {
        void *value = *(void **)(*(char **)((const char *)self + 8) + offset);
        if (value != 0) {
            if (count > 0) {
                cStrCat(out, separator);
            }
            cStrAppend(out, format, noneText);
            value = *(void **)(*(char **)((const char *)self + 8) + offset);
            GetTextSlot *slot = (GetTextSlot *)(*(char **)((char *)value + 4) + 0x78);
            slot->fn((char *)value + slot->offset, out);
            count += 1;
        }
        index += 1;
        offset += 4;
    } while (index < 4);

    index = 0;
    offset = 0;
    do {
        gcDesiredEnumerationEntry *entry =
            *(gcDesiredEnumerationEntry **)(*(char **)((const char *)self + 0x10) + offset);
        if (entry != 0) {
            count += entry->GetTextEx(out, (count > 0) & 0xFF, true) != 0;
        }
        index += 1;
        offset += 4;
    } while (index < 2);

    int eventObject = *(const int *)((const char *)self + 0x18);
    char text[256];
    text[0] = '\0';

    int owned = 0;
    int tagged = eventObject & 1;
    if (tagged != 0) {
        owned = 1;
    }
    int valid;
    if (owned != 0) {
        valid = 0;
    } else {
        valid = eventObject != 0;
        valid &= 0xFF;
        valid = valid != 0;
    }

    if (valid != 0) {
        int owned2 = 0;
        if (tagged != 0) {
            owned2 = 1;
        }
        if (owned2 != 0) {
            eventObject = 0;
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(eventObject + 4) + 0x40);
            slot->fn((char *)eventObject + slot->offset, text);
        } else {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(eventObject + 4) + 0x40);
            slot->fn((char *)eventObject + slot->offset, text);
        }
        if (count > 0) {
            cStrCat(out, separator);
        }
        cStrAppend(out, (const char *)0x36DF50, text);
        count += 1;
    }

    return count > 0;
}
