#include "cBase.h"

class cFile;
class cMemPool;
class cType;

extern "C" {
    void cStrCat(char *, const char *);
    void gcAction_gcAction(void *, cBase *);
    void gcExpressionList_gcExpressionList(void *, cBase *);
}

extern char gcDoWhilevirtualtable[];

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcExpressionList {
public:
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

inline void *operator new(unsigned int, void *p) { return p; }

extern const char gcDoWhile_base_name[] asm("D_0036D894");
extern const char gcDoWhile_base_desc[] asm("D_0036D89C");

static cType *type_gcDoWhile asm("D_0009F760");
static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");

class gcDoWhile {
public:
    char _pad[0xC];
    int m_state;
    gcExpression *branch;
    char _pad2[4];

    gcDoWhile(cBase *);
    static cBase *New(cMemPool *, cBase *);
    gcExpression *GetChild(int) const;
    void SetBranch(int, gcExpression *);
    void GetText(char *) const;
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

// 0x00318ea8, 8B
void gcDoWhile::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

// 0x00318d7c, 52B
gcExpression *gcDoWhile::GetChild(int index) const {
    gcExpression *result = 0;
    if (index == 0) {
        int raw = m_state;
        int flag = 0;
        if (raw & 1) {
            flag = 1;
        }
        if (flag != 0) {
            result = 0;
        } else {
            result = (gcExpression *)raw;
        }
    }
    return result;
}

// 0x0015231c, 40B
void gcDoWhile::GetText(char *buf) const {
    cStrCat(buf, "while");
}

// 0x00318c54, 280B
const cType *gcDoWhile::GetType(void) const {
    if (!type_gcDoWhile) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoWhile_base_name, gcDoWhile_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoWhile = cType::InitializeType(0, 0, 0x1D1, type_action, gcDoWhile::New, 0, 0, 0);
    }
    return type_gcDoWhile;
}

void gcDoWhile::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcAction *)this)->Write(file);
    int val = ((const int *)this)[3];
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
    ((const gcExpressionList *)((const char *)this + 0x10))->Write(wb);
    wb.End();
}

// 0x00152130, 72B
gcDoWhile::gcDoWhile(cBase *parent) {
    gcAction_gcAction(this, parent);
    ((void **)this)[1] = gcDoWhilevirtualtable;
    m_state = ((int)this) | 1;
    gcExpressionList_gcExpressionList((char *)this + 0x10, (cBase *)this);
}

// 0x00318bd8, 124B — static
cBase *gcDoWhile::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoWhile *result = 0;
    gcDoWhile *obj = (gcDoWhile *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoWhile(parent);
        result = obj;
    }
    return (cBase *)result;
}
