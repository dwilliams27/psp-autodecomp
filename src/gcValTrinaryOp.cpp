#include "gcValTrinaryOp.h"

class cFile;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

void gcValue_Write(const gcValTrinaryOp *, cFile &);
void cBaseArray_SetSize(void *, int);
void cBaseArray_RemoveAll(void *);
extern char gcValTrinaryOpvirtualtable[];
extern char cBaseclassdesc[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

gcValTrinaryOp::gcValTrinaryOp(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = gcValTrinaryOpvirtualtable;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 12) = 0;
    *(int *)((char *)this + 16) = 0;
    *(gcValTrinaryOp **)((char *)this + 20) = this;
    cBaseArray_SetSize((char *)this + 16, 3);
}

gcValTrinaryOp::~gcValTrinaryOp(void) {
    *(void **)((char *)this + 4) = gcValTrinaryOpvirtualtable;
    void *children = (char *)this + 16;
    if (children != 0) {
        cBaseArray_RemoveAll(children);
    }
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

gcExpression *gcValTrinaryOp::GetChild(int index) const {
    gcValTrinaryOpData *self = (gcValTrinaryOpData *)this;
    return self->mChildren[index];
}

cBase *gcValTrinaryOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValTrinaryOp *result = 0;
    gcValTrinaryOp *obj = (gcValTrinaryOp *)rec->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValTrinaryOp(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcValTrinaryOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue_Write(this, file);
    wb.Write(*(unsigned int *)((char *)this + 8));
    wb.Write(*(int *)((char *)this + 12));
    int i = 0;
    int off = 0;
    do {
        wb.WriteBase(*(const cBase **)((char *)*(void **)((char *)this + 16) + off));
        i++;
        off += 4;
    } while (i < 3);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValTrinaryOp;

const cType *gcValTrinaryOp::GetType(void) const {
    if (!type_gcValTrinaryOp) {
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
        type_gcValTrinaryOp = cType::InitializeType(0, 0, 0x70, type_value,
                                                    gcValTrinaryOp::New,
                                                    0, 0, 0);
    }
    return type_gcValTrinaryOp;
}
