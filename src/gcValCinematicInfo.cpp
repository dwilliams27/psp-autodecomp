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
class cReadBlock;

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

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
    void Read(cReadBlock &);
};

class gcExpression {
public:
    void *_base[2];   // cBase parent + vtable
    void Write(cFile &) const;
};

class gcValue : public gcExpression {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
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

struct GetTextSlot {
    short offset;
    short pad;
    void *fn;
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

extern "C" {
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
    void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
    void __0oKcReadBlockdtv(void *, int);
}

// ── Write(cFile &) const  @ 0x00323cc8 ──
void gcValCinematicInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);
    wb.Write(field_8);
    field_C.Write(wb);
    field_10.Write(wb);
    wb.End();
}

// 0x00323d38 - gcValCinematicInfo::Read(cFile &, cMemPool *)
int gcValCinematicInfo::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    int tag = rb[3];
    if ((unsigned int)tag >= 3 || (unsigned int)tag < 1 ||
        gcValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    if ((unsigned int)rb[3] >= 2) {
        cFileSystem_Read(*(void **)rb[0], (char *)this + 8, 4);
        ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
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

void gcValCinematicInfo::GetText(char *buf) const {
    register const gcValCinematicInfo *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;

    cStrAppend(out, (const char *)0x36DCE4, (const char *)0x36DAF0);

    switch (*(int *)((const char *)self + 8)) {
    case 1:
    case 2:
    case 3: {
        int val = *(int *)((const char *)self + 0x0C);
        int tagged = 0;
        int tag = val & 1;
        register const char *suffix __asm__("$18") = (const char *)0x36E440;
        if (tag != 0) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, (const char *)0x36DAD8);

        val = *(int *)((const char *)self + 0x10);
        tagged = 0;
        if (val & 1) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        check = val;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, suffix);
        break;
    }
    case 7:
    case 8:
    case 9: {
        int val = *(int *)((const char *)self + 0x0C);
        int tagged = 0;
        int tag = val & 1;
        register const char *suffix __asm__("$16") = (const char *)0x36E440;
        if (tag != 0) {
            tagged = 1;
        }
        if (tagged != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *type = *(char **)(check + 4);
            GetTextSlot *slot = (GetTextSlot *)(type + 0xD0);
            ((void (*)(void *, char *))slot->fn)(
                (char *)val + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }

        cStrAppend(out, suffix);
        break;
    }
    case 4:
    case 5:
    case 6:
    default:
        break;
    }

    cStrAppend(out, (const char *)0x36EBE4);
}
