// gcDesiredString.cpp — decompiled from gcAll_psp.obj.
//
// Functions matched here:
//   gcDesiredString::GetObject(bool) const                          @ 0x0012A700
//   gcDesiredString::Write(cFile &) const                           @ 0x0012A428
//   gcDesiredString::New(cMemPool *, cBase *) static                @ 0x002706B8
//   gcDesiredString::GetType(void) const                            @ 0x00270758
//   gcDesiredString::Read(cFile &, cMemPool *)                      @ 0x0012A48C
//   gcDesiredString::GetText(char *) const                          @ 0x0012A71C
//   gcDesiredString::GetDesiredType(void) const                     @ 0x0012AAFC

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cType;
class cObject;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class gcDesiredObject {
public:
    void Write(cFile &) const;
    int  Read(cFile &, cMemPool *);
};

class gcDesiredStringHelper {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
    void GetText(char *) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

extern "C" void cFile_SetCurrentPos(void *file, unsigned int pos);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *rb, cFile &file,
                                             unsigned int id, bool validate);
extern "C" void __0oKcReadBlockdtv(void *rb, int flags);

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

struct GetTextSlot {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern char gcDesiredString_cBase_vtable[];     // 0x3899E0

extern const char gcDesiredString_base_name[];  // 0x36D894
extern const char gcDesiredString_base_desc[];  // 0x36D89C

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F474;
extern cType *D_00099900;     // cached gcString cType (desired type)

class gcString {
public:
    static cBase *New(cMemPool *, cBase *);
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

class gcDesiredString {
public:
    ~gcDesiredString();
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    gcDesiredString &operator=(const gcDesiredString &);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int  Read(cFile &, cMemPool *);
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    const cType *GetDesiredType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oPgcDesiredStringdtv, 0x130\n");

gcDesiredString::~gcDesiredString(void) {
    *(void **)((char *)this + 4) = (void *)0x3899E0;
    char *second = (char *)this + 8;

    if ((void *)((char *)this + 0x18) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x18) = 0;
            }
        }
    }

    *(void **)((char *)this + 4) = (void *)0x3889A8;
    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 8);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 8) = 0;
        }
    }

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// ============================================================
// 0x0012A700 — GetObject(bool) const, 28B (already matched)
// ============================================================
cObject *gcDesiredString::GetObject(bool b) const {
    return Get(b);
}

// ============================================================
// 0x0012A428 — Write(cFile &) const, 100B
// ============================================================
void gcDesiredString::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcDesiredObject *)this)->Write(file);
    ((const gcDesiredStringHelper *)((const char *)this + 12))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 24))->Write(wb);
    wb.End();
}

// ============================================================
// 0x00270550 — AssignCopy(const cBase *), 360B
// ============================================================
void gcDesiredString::AssignCopy(const cBase *base) {
    const gcDesiredString *other = 0;

    if (base != 0) {
        if (D_0009F474 == 0) {
            if (D_0009F3F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F474 = cType::InitializeType(0, 0, 0x215, D_0009F3F4,
                                               &gcDesiredString::New,
                                               0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F474;
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
            other = (const gcDesiredString *)base;
        }
    }
    operator=(*other);
}

// ============================================================
// 0x002706B8 — New(cMemPool *, cBase *) static, 160B
// ============================================================
cBase *gcDesiredString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredString *result = 0;
    gcDesiredString *obj = (gcDesiredString *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] = gcDesiredString_cBase_vtable;
        *((char *)obj + 0x0C) = 0;
        *((char *)obj + 0x0D) = 0;
        *(int *)((char *)obj + 0x10) = 0;
        *(int *)((char *)obj + 0x14) = 0;
        *(int *)((char *)obj + 0x18) = (int)((unsigned int)obj | 1);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00270758 — GetType(void) const, 220B
// ============================================================
const cType *gcDesiredString::GetType(void) const {
    if (D_0009F474 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType(gcDesiredString_base_name,
                                                   gcDesiredString_base_desc,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F474 = cType::InitializeType(0, 0, 0x215, D_0009F3F4,
                                           &gcDesiredString::New, 0, 0, 0);
    }
    return D_0009F474;
}

// ============================================================
// 0x0012A48C — Read(cFile &, cMemPool *), 212B
// ============================================================
int gcDesiredString::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 ||
        ((gcDesiredObject *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    ((gcDesiredStringHelper *)((char *)this + 12))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 24))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// ============================================================
// 0x0012AAFC — GetDesiredType(void) const, 160B
// ============================================================
const cType *gcDesiredString::GetDesiredType(void) const {
    if (D_00099900 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099900 = cType::InitializeType(0, 0, 0x87, D_000385DC,
                                           &gcString::New, 0, 0, 0);
    }
    return D_00099900;
}

// ============================================================
// 0x0012A71C — GetText(char *) const, 272B
// ============================================================
void gcDesiredString::GetText(char *buf) const {
    int v8 = *(int *)((const char *)this + 8);
    int flag1 = 0;
    if (v8 & 1) {
        flag1 = 1;
    }
    int hasReal;
    if (flag1 != 0) {
        hasReal = 0;
    } else {
        int raw = (v8 != 0);
        hasReal = ((unsigned char)raw) != 0;
    }
    if (hasReal != 0) {
        __asm__ volatile("" ::: "memory");
    } else {
        ((const gcDesiredStringHelper *)((const char *)this + 12))->GetText(buf);
    }

    int v24 = *(int *)((const char *)this + 0x18);
    int isLit2 = v24 & 1;
    int flag2 = 0;
    if (isLit2 != 0) {
        flag2 = 1;
    }
    if (flag2 != 0) {
        v24 = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    if (v24 == 0) {
        return;
    }

    cStrAppend(buf, (const char *)0x36DCBC);

    int v24b = *(int *)((const char *)this + 0x18);
    int flag3 = 0;
    if (v24b & 1) {
        flag3 = 1;
    }
    if (flag3 != 0) {
        v24b = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    char *obj = (char *)v24b;
    if (obj != 0) {
        char *vt = *(char **)(obj + 4);
        GetTextSlot *slot = (GetTextSlot *)(vt + 0xD0);
        slot->fn(obj + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }
}
