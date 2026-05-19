// gcValStringCompare -- decompiled from gcAll_psp.obj
//
// Functions:
//   0x0035dad0  gcValStringCompare::New(cMemPool *, cBase *) static
//   0x0035dc84  gcValStringCompare::Write(cFile &) const
//   0x0035e0cc  gcValStringCompare::GetText(char *) const

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cFileHandle;
class cMemPool;
class cReadBlock;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];

    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void WriteBase(const cBase *);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcValStringComparevirtualtable[];

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcValStringCompare : public gcValue {
public:
    int mLeft;
    int mRight;
    bool mCaseSensitive;
    bool mExact;

    static cBase *New(cMemPool *, cBase *);
    gcValStringCompare &operator=(const gcValStringCompare &);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    float Evaluate(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
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

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

void cStrAppend(char *, const char *, ...);
void cStrCopy(char *, const char *);
void cStrCopy(char *, const wchar_t *, int);
int cStrCompare(const char *, const char *, bool);

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                             bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class cType {
public:
    char pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValStringCompare;

// -- gcValStringCompare::New(cMemPool *, cBase *) static @ 0x0035dad0 --
cBase *gcValStringCompare::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValStringCompare *result = 0;
    gcValStringCompare *obj =
        (gcValStringCompare *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValStringComparevirtualtable;
        unsigned int tagged = ((unsigned int)obj) | 1;
        obj->mLeft = tagged;
        obj->mRight = tagged;
        obj->mCaseSensitive = true;
        obj->mExact = true;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcValStringCompare::GetType(void) const {
    if (!type_gcValStringCompare) {
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
        type_gcValStringCompare =
            cType::InitializeType(0, 0, 0x1EC, type_value,
                                  gcValStringCompare::New, 0, 0, 0);
    }
    return type_gcValStringCompare;
}

void gcValStringCompare::AssignCopy(const cBase *base) {
    const gcValStringCompare *other = 0;

    if (base != 0) {
        if (!type_gcValStringCompare) {
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
            type_gcValStringCompare = cType::InitializeType(
                0, 0, 0x1EC, type_value, gcValStringCompare::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_gcValStringCompare;
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
            other = (const gcValStringCompare *)base;
        }
    }

    operator=(*other);
}

// -- gcValStringCompare::Write(cFile &) const @ 0x0035dc84 --
void gcValStringCompare::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    int left = mLeft;
    int leftFlag = 0;
    if (left & 1) {
        leftFlag = 1;
    }
    cBase *leftPtr;
    if (leftFlag != 0) {
        leftPtr = 0;
    } else {
        leftPtr = (cBase *)left;
    }
    wb.WriteBase(leftPtr);

    int right = mRight;
    int rightFlag = 0;
    if (right & 1) {
        rightFlag = 1;
    }
    cBase *rightPtr;
    if (rightFlag != 0) {
        rightPtr = 0;
    } else {
        rightPtr = (cBase *)right;
    }
    wb.WriteBase(rightPtr);

    wb.Write(mCaseSensitive);
    wb.Write(mExact);
    wb.End();
}

int gcValStringCompare::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] >= 3 || (unsigned int)rb[3] < 1 ||
        gcValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value = *(int *)((char *)this + 0x08);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x08);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int value2 = *(int *)((char *)this + 0x0C);
    int tag2 = value2 & 1;
    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x08) = newValue;

    int flag3 = 0;
    if (tag2 != 0) {
        flag3 = 1;
    }

    int outValue2;
    if (flag3 != 0) {
        outValue2 = 0;
        goto out2_done;
    }
    outValue2 = value2;
out2_done:
    int sp18 = outValue2;

    int flag4 = 0;
    if (tag2 != 0) {
        flag4 = 1;
    }

    int base2;
    if (flag4 != 0) {
        base2 = value2 & ~1;
    } else {
        base2 = *(int *)value2;
    }

    cMemPool *childPool2 = cMemPool::GetPoolFromPtr((char *)this + 0x0C);
    ((cReadBlock *)rb)->ReadBase(childPool2, (cBase *)base2,
                                 *(cBase **)&sp18);

    int newValue2;
    if (sp18 == 0) {
        newValue2 = base2 | 1;
    } else {
        newValue2 = sp18;
    }
    int readFile = rb[0];
    *(int *)((char *)this + 0x0C) = newValue2;

    char sp1c;
    cFileSystem::Read(*(cFileHandle **)readFile, &sp1c, 1);
    int version = rb[3];
    *(unsigned char *)((char *)this + 0x10) = sp1c != 0;

    if ((unsigned int)version >= 2) {
        char sp1d;
        cFileSystem::Read(*(cFileHandle **)rb[0], &sp1d, 1);
        *(unsigned char *)((char *)this + 0x11) = sp1d != 0;
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

float gcValStringCompare::Evaluate(void) const {
    char left[256];
    char right[256];
    wchar_t leftWide[4096];
    wchar_t rightWide[4096];

    left[0] = 0;
    right[0] = 0;

    int flag = 0;
    int value = *(int *)((const char *)this + 0x08);
    int tag = value & 1;
    if (tag != 0) {
        flag = 1;
    }

    int ok;
    if (flag != 0) {
        ok = 0;
    } else {
        ok = value != 0;
        ok &= 0xFF;
        ok = ok != 0;
    }

    if (ok != 0) {
        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }

        char *type;
        if (flag2 == 0) {
            type = *(char **)(value + 4);
        } else {
            value = 0;
            type = *(char **)(value + 4);
        }

        cTypeMethod *slot = (cTypeMethod *)(type + 0x70);
        ((void (*)(void *, wchar_t *, int))slot->fn)(
            (char *)value + slot->offset, leftWide, 0x1000);
        cStrCopy(left, leftWide, 0x100);
    }

    value = *(int *)((const char *)this + 0x0C);
    tag = value & 1;
    flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    if (flag != 0) {
        ok = 0;
    } else {
        ok = value != 0;
        ok &= 0xFF;
        ok = ok != 0;
    }

    if (ok != 0) {
        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }

        char *type;
        if (flag2 == 0) {
            type = *(char **)(value + 4);
        } else {
            value = 0;
            type = *(char **)(value + 4);
        }

        cTypeMethod *slot = (cTypeMethod *)(type + 0x70);
        ((void (*)(void *, wchar_t *, int))slot->fn)(
            (char *)value + slot->offset, rightWide, 0x1000);
        cStrCopy(right, rightWide, 0x100);
    }

    int cmp = cStrCompare(left, right, *(bool *)((const char *)this + 0x10));
    int result;
    if (*(unsigned char *)((const char *)this + 0x11) == 0) goto notExact;
    result = cmp == 0;
    goto done;
notExact:
    result = cmp;
done:
    return (float)result;
}

void gcValStringCompare::GetText(char *buf) const {
    register const gcValStringCompare *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    char left[256];
    char right[256];

    left[0] = '\0';
    right[0] = '\0';
    __asm__ volatile("" ::: "memory");

    int flag = 0;
    int val = *(int *)((const char *)self + 0x08);
    int tag = val & 1;
    if (tag != 0) {
        flag = 1;
    }

    int valid;
    if (flag != 0) {
        valid = 0;
    } else {
        valid = val != 0;
        valid &= 0xFF;
        valid = valid != 0;
    }

    int tag2;
    int val2;

    if (valid != 0) {
        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }

        int base = val;
        char *type;
        if (flag2 != 0) {
            base = 0;
            type = *(char **)(base + 4);
        } else {
            type = *(char **)(base + 4);
        }

        cTypeMethod *slot = (cTypeMethod *)(type + 0x40);
        ((void (*)(void *, char *))slot->fn)((char *)base + slot->offset,
                                             left);
        val2 = *(int *)((const char *)self + 0x0C);
        tag2 = val2 & 1;
    } else {
        cStrCopy(left, (const char *)0x36DACC);
        val2 = *(int *)((const char *)self + 0x0C);
        tag2 = val2 & 1;
    }

    int flag3 = 0;
    if (tag2 != 0) {
        flag3 = 1;
    }

    int valid2;
    if (flag3 != 0) {
        valid2 = 0;
    } else {
        valid2 = val2 != 0;
        valid2 &= 0xFF;
        valid2 = valid2 != 0;
    }

    if (valid2 != 0) {
        int flag4 = 0;
        if (tag2 != 0) {
            flag4 = 1;
        }

        int base2 = val2;
        char *type2;
        if (flag4 != 0) {
            base2 = 0;
            type2 = *(char **)(base2 + 4);
        } else {
            type2 = *(char **)(base2 + 4);
        }

        cTypeMethod *slot2 = (cTypeMethod *)(type2 + 0x40);
        ((void (*)(void *, char *))slot2->fn)((char *)base2 + slot2->offset,
                                              right);
    } else {
        cStrCopy(right, (const char *)0x36DACC);
    }

    const char *fmt;
    if (*(unsigned char *)((const char *)self + 0x11) != 0) {
        fmt = (const char *)0x36F778;
    } else {
        fmt = (const char *)0x36F784;
    }
    cStrAppend(out, fmt, left, right);
}
