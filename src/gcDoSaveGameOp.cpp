#include "cBase.h"

class cFile;
class cMemPool;
class cType;

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
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoSaveGameOp {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    gcDoSaveGameOp &operator=(const gcDoSaveGameOp &);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

struct GetTextSlot {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

void gcAction_gcAction(gcDoSaveGameOp *, cBase *);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern char gcDoSaveGameOpvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoSaveGameOp asm("D_0009F6E0");

cBase *gcDoSaveGameOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoSaveGameOp *result = 0;
    gcDoSaveGameOp *obj = (gcDoSaveGameOp *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoSaveGameOpvirtualtable;
        ((int *)obj)[3] = 0;
        int encoded = (int)obj | 1;
        ((int *)obj)[4] = encoded;
        ((int *)obj)[5] = encoded;
        ((int *)obj)[6] = encoded;
        ((int *)obj)[7] = encoded;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoSaveGameOp::GetType(void) const {
    if (!type_gcDoSaveGameOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoSaveGameOp = cType::InitializeType(
            0, 0, 0x1A7, type_action, gcDoSaveGameOp::New, 0, 0, 0);
    }
    return type_gcDoSaveGameOp;
}

void gcDoSaveGameOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoSaveGameOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoSaveGameOp = cType::InitializeType(
                0, 0, 0x1A7, type_action, gcDoSaveGameOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoSaveGameOp;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoSaveGameOp *)copy;
}

void gcDoSaveGameOp::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    ((const gcAction *)this)->Write(file);
    wb.Write(*(const int *)((const char *)this + 0x0C));
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);
    wb.End();
}

void gcDoSaveGameOp::GetText(char *buf) const {
    register const gcDoSaveGameOp *self __asm__("$s0") = this;
    register char *out __asm__("$s1") = buf;

    cStrAppend(out, (const char *)0x36EF20);
    cStrAppend(out, (const char *)0x36DAF0);

    int mode = *(const int *)((const char *)self + 0x0C);
    int flag;
    switch (mode - 2) {
    case 0:
    case 1:
    case 11:
    case 14:
    case 15:
    case 16:
    case 21:
        flag = 1;
        break;
    default:
        flag = 0;
        break;
    }
    if ((flag != 0) || (mode == 15)) {
        cStrAppend(out, (const char *)0x36DAD8);
        int value = *(int *)((const char *)self + 0x10);
        int owned = 0;
        if (value & 1) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" : "+r"(value));
        }
        int check = value;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
    }

    mode = *(const int *)((const char *)self + 0x0C);
    switch (mode - 4) {
    case 0:
    case 1:
    case 2:
    case 16:
    case 17:
        flag = 1;
        break;
    default:
        flag = 0;
        break;
    }
    if (flag != 0) {
        cStrAppend(out, (const char *)0x36DAD8);
        int value = *(int *)((const char *)self + 0x14);
        int owned = 0;
        if (value & 1) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" : "+r"(value));
        }
        int check = value;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
    }

    mode = *(const int *)((const char *)self + 0x0C);
    if ((mode == 12) || (mode == 7) || (mode == 3)) {
        flag = 1;
    } else {
        flag = 0;
        __asm__ volatile("" ::: "memory");
    }
    if (flag != 0) {
        cStrAppend(out, (const char *)0x36DAD8);
        int value = *(int *)((const char *)self + 0x18);
        int owned = 0;
        if (value & 1) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" : "+r"(value));
        }
        int check = value;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
    }

    mode = *(const int *)((const char *)self + 0x0C);
    switch (mode) {
    case 0:
    case 1:
    case 4:
    case 5:
    case 6:
    case 20:
        flag = 1;
        break;
    default:
        flag = 0;
        break;
    }
    if (flag != 0) {
        cStrAppend(out, (const char *)0x36DAD8);
        int value = *(int *)((const char *)self + 0x1C);
        int owned = 0;
        if (value & 1) {
            owned = 1;
        }
        if (owned != 0) {
            value = 0;
        } else {
            __asm__ volatile("" : "+r"(value));
        }
        int check = value;
        if (check != 0) {
            GetTextSlot *slot = (GetTextSlot *)(*(char **)(check + 4) + 0xD0);
            slot->fn((char *)value + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
    }

    cStrAppend(out, (const char *)0x36DCEC);
}
