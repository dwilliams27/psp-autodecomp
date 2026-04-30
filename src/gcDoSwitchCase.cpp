#include "gcDoReturn.h"
#include "cBase.h"

class cBaseArray {
public:
    cBaseArray &operator=(const cBaseArray &);
    void SetSize(int);
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
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcExpressionList {
public:
    gcExpressionList(cBase *);
    gcExpressionList &operator=(const gcExpressionList &);
    void Write(cWriteBlock &) const;
};

gcDoSwitchCase *dcast(const cBase *);

void gcDoSwitchCase__gcDoSwitchCase_cBaseptr(gcDoSwitchCase *, cBase *);
void gcAction_gcAction(gcDoSwitchCase *, cBase *);
void gcAction_Write(const gcDoSwitchCase *, cFile &);
void gcValue_Write(const void *, cFile &);
void gcExpressionList_gcExpressionList(void *, cBase *);

extern char gcDoSwitchCasevirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoSwitchCase asm("D_0009F708");

typedef void (*EmbeddedWriteFn)(cBase *, cFile *);

struct EmbeddedWriteSlot {
    short offset;
    short _pad;
    EmbeddedWriteFn fn;
};

struct EmbeddedTypeInfo {
    char _pad[0x28];
    EmbeddedWriteSlot write;
};

class gcValEntityConstant {
public:
    void Write(cFile &) const;
};

class gcValEntityHasAnimation {
public:
    void Write(cFile &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

// 0x0014fec0, 96B
gcDoSwitchCase::gcDoSwitchCase(cBase *parent) {
    gcAction_gcAction(this, parent);
    ((void **)this)[1] = gcDoSwitchCasevirtualtable;
    *(int *)((char *)this + 0xC) = 0;
    *(gcDoSwitchCase **)((char *)this + 0x10) = this;
    cBaseArray *children = (cBaseArray *)((char *)this + 0xC);
    gcExpressionList_gcExpressionList((char *)this + 0x14, (cBase *)this);
    children->SetSize(3);
}

// 0x00304f5c, 280B
const cType *gcDoSwitchCase::GetType(void) const {
    if (!type_gcDoSwitchCase) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoSwitchCase = cType::InitializeType(0, 0, 0xD7, type_action,
                                                     gcDoSwitchCase::New,
                                                     0, 0, 0);
    }
    return type_gcDoSwitchCase;
}

void gcDoSwitchCase::AssignCopy(const cBase *base) {
    gcDoSwitchCase *other = dcast(base);
    int *self = (int *)this;
    int *src = (int *)other;
    self[2] = self[2] & ~3;
    self[2] = self[2] | (src[2] & 3);
    ((cBaseArray *)((char *)this + 0xC))->operator=(*(const cBaseArray *)((char *)other + 0xC));
    ((gcExpressionList *)((char *)this + 0x14))->operator=(*(const gcExpressionList *)((char *)other + 0x14));
}

cBase *gcDoSwitchCase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSwitchCase *result = 0;
    gcDoSwitchCase *obj = (gcDoSwitchCase *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcDoSwitchCase__gcDoSwitchCase_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

int gcDoSwitchCase::GetMaxChildren(void) const {
    return 3;
}

int gcDoSwitchCase::GetExprFlags(void) const {
    return 0x18;
}

int gcValCaseRange::GetMaxChildren(void) const {
    return 2;
}

int gcValCaseRange::GetExprFlags(void) const {
    return 3;
}

int gcDoSwitch::GetMaxChildren(void) const {
    return 1;
}

int gcDoSwitch::GetMaxBranches(void) const {
    return 2;
}

void gcDoSwitchCase::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    ((const gcExpressionList *)((const char *)this + 0x14))->Write(wb);
    int i = 0;
    int off = 0;
    do {
        wb.WriteBase(*(const cBase **)((char *)*(void **)((char *)this + 0xC) + off));
        i++;
        off += 4;
    } while (i < 3);
    wb.End();
}

void gcValEntityConstant::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue_Write(this, file);

    EmbeddedTypeInfo *type0 = *(EmbeddedTypeInfo **)((char *)this + 0xC);
    EmbeddedWriteSlot *slot0 = &type0->write;
    cBase *embedded0 = (cBase *)((char *)this + 8);
    slot0->fn((cBase *)((char *)embedded0 + slot0->offset), wb._file);

    EmbeddedTypeInfo *type1 = *(EmbeddedTypeInfo **)((char *)this + 0x38);
    EmbeddedWriteSlot *slot1 = &type1->write;
    cBase *embedded1 = (cBase *)((char *)this + 0x34);
    slot1->fn((cBase *)((char *)embedded1 + slot1->offset), wb._file);

    wb.End();
}

void gcValEntityHasAnimation::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue_Write(this, file);

    EmbeddedTypeInfo *type0 = *(EmbeddedTypeInfo **)((char *)this + 0xC);
    EmbeddedWriteSlot *slot0 = &type0->write;
    cBase *embedded0 = (cBase *)((char *)this + 8);
    slot0->fn((cBase *)((char *)embedded0 + slot0->offset), wb._file);

    EmbeddedTypeInfo *type1 = *(EmbeddedTypeInfo **)((char *)this + 0x38);
    EmbeddedWriteSlot *slot1 = &type1->write;
    cBase *embedded1 = (cBase *)((char *)this + 0x34);
    slot1->fn((cBase *)((char *)embedded1 + slot1->offset), wb._file);

    wb.End();
}
