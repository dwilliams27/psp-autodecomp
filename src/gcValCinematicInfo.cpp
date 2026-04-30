// gcValCinematicInfo.cpp — decompiled from gcAll_psp.obj
//
// Class layout (size 0x14):
//   [0x00] cBase fields (parent, vtable)
//   [0x08] int field_8 (cinematic id / index)
//   [0x0C] gcDesiredValue mDesiredA
//   [0x10] gcDesiredValue mDesiredB

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    int _value;
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void *_base[2];   // cBase parent + vtable
    void Write(cFile &) const;
};

class gcValue : public gcExpression {
public:
    void Write(cFile &) const;
};

class gcValCinematicInfo : public gcValue {
public:
    int field_8;
    gcDesiredValue field_C;
    gcDesiredValue field_10;

    gcValCinematicInfo &operator=(const gcValCinematicInfo &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

class cType {
public:
    char pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char cBaseclassdesc[];                          // @ 0x37E6A8
extern char gcValCinematicInfovirtualtable[];          // @ 0x006AE0 (linker reloc)

struct ePoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct eAllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── Write(cFile &) const  @ 0x00323cc8 ──
void gcValCinematicInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);
    wb.Write(field_8);
    field_C.Write(wb);
    field_10.Write(wb);
    wb.End();
}

// ── New(cMemPool *, cBase *)  @ 0x00323b1c ──
cBase *gcValCinematicInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((ePoolBlock *)block)->allocTable;
    eAllocEntry *entry = (eAllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValCinematicInfo *result = 0;
    gcValCinematicInfo *obj = (gcValCinematicInfo *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValCinematicInfovirtualtable;
        *(int *)((char *)obj + 8) = 0;
        *(int *)((char *)obj + 0xC) = (int)obj | 1;
        *(int *)((char *)obj + 0x10) = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValCinematicInfo;

const cType *gcValCinematicInfo::GetType(void) const {
    if (!type_gcValCinematicInfo) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValCinematicInfo =
            cType::InitializeType(0, 0, 0x1DD, type_value,
                                  gcValCinematicInfo::New, 0, 0, 0);
    }
    return type_gcValCinematicInfo;
}

// ── AssignCopy(const cBase *)  @ 0x00323978 ──
void gcValCinematicInfo::AssignCopy(const cBase *base) {
    const gcValCinematicInfo *other = 0;

    if (base != 0) {
        if (!type_gcValCinematicInfo) {
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
            type_gcValCinematicInfo = cType::InitializeType(
                0, 0, 0x1DD, type_value, gcValCinematicInfo::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValCinematicInfo;
        cType *type = entry->fn((char *)base + entry->offset);
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
            other = (const gcValCinematicInfo *)base;
        }
    }

    operator=(*other);
}
