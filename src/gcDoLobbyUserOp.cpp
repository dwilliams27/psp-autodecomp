// gcDoLobbyUserOp.cpp — decompiled from gcAll_psp.obj.
// Inherits from gcAction.
//
// Functions:
//   0x002e61d8 gcDoLobbyUserOp::New(cMemPool *, cBase *) static  156B
//   0x002e638c gcDoLobbyUserOp::Write(cFile &) const             124B

#include "cBase.h"

class cMemPool;
class cFile;
class cType;

class gcDoLobbyUserOp {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

class gcDoLog {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
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
    void Write(bool);
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
    int (*fn)(void *, int, int, int, int);
};

void gcAction_gcAction(gcDoLobbyUserOp *, cBase *);
void gcAction_Write(const gcDoLobbyUserOp *, cFile &);
extern char gcDoLobbyUserOpvirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoLobbyUserOp asm("D_0009F6A0");
static cType *type_gcDoLog asm("D_0009F6A4");

// 0x002e61d8 — New(cMemPool *, cBase *) static
cBase *gcDoLobbyUserOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLobbyUserOp *result = 0;
    gcDoLobbyUserOp *obj = (gcDoLobbyUserOp *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLobbyUserOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int *)obj)[5] = (int)obj | 1;
        *((char *)obj + 24) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x002e6274 — GetType(void) const
const cType *gcDoLobbyUserOp::GetType(void) const {
    if (!type_gcDoLobbyUserOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLobbyUserOp = cType::InitializeType(
            0, 0, 0x1CB, type_action, gcDoLobbyUserOp::New, 0, 0, 0);
    }
    return type_gcDoLobbyUserOp;
}

// 0x002e638c — Write(cFile &) const
void gcDoLobbyUserOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(int *)((char *)this + 0x10));
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x18));
    wb.End();
}

// 0x002e6c64 — GetType(void) const
const cType *gcDoLog::GetType(void) const {
    if (!type_gcDoLog) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLog = cType::InitializeType(
            0, 0, 0x71, type_action, gcDoLog::New, 0, 0, 0);
    }
    return type_gcDoLog;
}
