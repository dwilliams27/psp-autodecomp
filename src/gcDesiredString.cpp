// gcDesiredString.cpp — decompiled from gcAll_psp.obj.
//
// Functions matched here:
//   gcDesiredString::GetObject(bool) const                          @ 0x0012A700
//   gcDesiredString::Write(cFile &) const                           @ 0x0012A428
//   gcDesiredString::New(cMemPool *, cBase *) static                @ 0x002706B8
//   gcDesiredString::GetType(void) const                            @ 0x00270758

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;
class cObject;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredObject {
public:
    void Write(cFile &) const;
};

class gcDesiredStringHelper {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

class gcDesiredString {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    gcDesiredString &operator=(const gcDesiredString &);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

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
