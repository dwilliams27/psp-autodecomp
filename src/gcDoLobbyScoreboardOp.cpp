#include "cBase.h"

class cMemPool;
class cFile;
class cType;

class gcDoLobbyScoreboardOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoLobbyScoreboardOp &operator=(const gcDoLobbyScoreboardOp &);
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
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

void gcAction_gcAction(gcDoLobbyScoreboardOp *, cBase *);
void gcAction_Write(const gcDoLobbyScoreboardOp *, cFile &);
extern char gcDoLobbyScoreboardOpvirtualtable[];
extern const char gcDoLobbyScoreboardOp_base_name[] asm("D_0036D894");
extern const char gcDoLobbyScoreboardOp_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyScoreboardOp asm("D_0009F69C");

cBase *gcDoLobbyScoreboardOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyScoreboardOp *result = 0;
    gcDoLobbyScoreboardOp *obj = (gcDoLobbyScoreboardOp *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyScoreboardOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = (int)obj | 1;
        ((int *)obj)[6] = (int)obj | 1;
        ((int *)obj)[7] = (int)obj | 1;
        ((int *)obj)[8] = (int)obj | 1;
        ((int *)obj)[9] = (int)obj | 1;
        ((int *)obj)[10] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoLobbyScoreboardOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoLobbyScoreboardOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoLobbyScoreboardOp_base_name,
                            gcDoLobbyScoreboardOp_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoLobbyScoreboardOp = cType::InitializeType(
                0, 0, 0x123, type_action, gcDoLobbyScoreboardOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoLobbyScoreboardOp;
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
    *this = *(const gcDoLobbyScoreboardOp *)copy;
}

const cType *gcDoLobbyScoreboardOp::GetType(void) const {
    if (!type_gcDoLobbyScoreboardOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoLobbyScoreboardOp_base_name,
                        gcDoLobbyScoreboardOp_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLobbyScoreboardOp = cType::InitializeType(
            0, 0, 0x123, type_action, gcDoLobbyScoreboardOp::New, 0, 0, 0);
    }
    return type_gcDoLobbyScoreboardOp;
}

void gcDoLobbyScoreboardOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(int *)((char *)this + 0x10));
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x18))->Write(wb);

    {
        int value = *(int *)((char *)this + 0x1C);
        int flag = 0;
        if (value & 1) {
            flag = 1;
        }
        cBase *ptr;
        if (flag != 0) {
            ptr = 0;
        } else {
            ptr = (cBase *)value;
        }
        wb.WriteBase(ptr);
    }

    {
        int value = *(int *)((char *)this + 0x20);
        int flag = 0;
        if (value & 1) {
            flag = 1;
        }
        cBase *ptr;
        if (flag != 0) {
            ptr = 0;
        } else {
            ptr = (cBase *)value;
        }
        wb.WriteBase(ptr);
    }

    ((gcDesiredValue *)((char *)this + 0x24))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x28))->Write(wb);
    wb.End();
}
