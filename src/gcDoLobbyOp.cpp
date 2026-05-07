#include "cBase.h"

class cMemPool;
class cFile;
class cType;

class gcDoLobbyOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    gcDoLobbyOp &operator=(const gcDoLobbyOp &);
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
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
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
    short pad;
    void (*fn)(void *, char *);
};

void gcAction_gcAction(gcDoLobbyOp *, cBase *);
void gcAction_Write(const gcDoLobbyOp *, cFile &);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern char gcDoLobbyOpvirtualtable[];
extern const char gcDoLobbyOp_base_name[] asm("D_0036D894");
extern const char gcDoLobbyOp_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyOp asm("D_0009F698");

cBase *gcDoLobbyOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoLobbyOp *result = 0;
    gcDoLobbyOp *obj = (gcDoLobbyOp *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyOpvirtualtable;
        ((int *)obj)[3] = 1;
        int encoded = (int)obj | 1;
        ((int *)obj)[4] = encoded;
        ((int *)obj)[5] = encoded;
        ((int *)obj)[6] = encoded;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoLobbyOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoLobbyOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoLobbyOp_base_name, gcDoLobbyOp_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoLobbyOp = cType::InitializeType(
                0, 0, 0x181, type_action, gcDoLobbyOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoLobbyOp;
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
    *this = *(const gcDoLobbyOp *)copy;
}

const cType *gcDoLobbyOp::GetType(void) const {
    if (!type_gcDoLobbyOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoLobbyOp_base_name, gcDoLobbyOp_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLobbyOp = cType::InitializeType(
            0, 0, 0x181, type_action, gcDoLobbyOp::New, 0, 0, 0);
    }
    return type_gcDoLobbyOp;
}

void gcDoLobbyOp::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);

    int val = ((int *)this)[6];
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

void gcDoLobbyOp::GetText(char *buf) const {
    register const gcDoLobbyOp *self __asm__("$16") = this;
    register char *out __asm__("$17") = buf;
    cStrAppend(out, (const char *)0x36EC94, (const char *)0x36DAF0);

    int op = *(int *)((const char *)self + 0x0C);
    int valid = 0;
    switch (op) {
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
        valid = 1;
        break;
    default:
        valid = 0;
        break;
    }

    if (valid != 0) {
        int val0 = *(int *)((const char *)self + 0x10);
        int flag0 = 0;
        if (val0 & 1) {
            flag0 = 1;
        }
        if (flag0 != 0) {
            val0 = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }
        int check0 = val0;
        if (check0 != 0) {
            char *typeInfo = *(char **)(check0 + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)val0 + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
        op = *(int *)((const char *)self + 0x0C);
    }

    int isJoin = op == 11;
    isJoin &= 0xFF;
    if (isJoin != 0) {
        cStrAppend(out, (const char *)0x36DAD8);
        int val1 = *(int *)((const char *)self + 0x14);
        int flag1 = 0;
        if (val1 & 1) {
            flag1 = 1;
        }
        if (flag1 != 0) {
            val1 = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }
        int check1 = val1;
        if (check1 != 0) {
            char *typeInfo = *(char **)(check1 + 4);
            GetTextSlot *slot = (GetTextSlot *)(typeInfo + 0xD0);
            slot->fn((char *)val1 + slot->offset, out);
        } else {
            cStrCat(out, (const char *)0x36DB24);
        }
    }

    cStrAppend(out, (const char *)0x36DCEC);
}
