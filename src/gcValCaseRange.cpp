#include "gcDoReturn.h"
#include "cBase.h"

void cStrCat(char *, const char *);
gcValCaseRange *dcast(const cBase *);

struct DispatchEntry {
    short offset;
    short _pad;
    void (*func)();
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void gcAction_Write(const gcDoReturn *, cFile &);

class cBaseArray {
public:
    cBase **mData;
    cBaseArray &operator=(const cBaseArray &);
};

extern char gcValCaseRangevirtualtable[];
extern char cBaseclassdesc_for_gcValCaseRange[];

void cBaseArray_SetSize(void *, int);
void cBaseArray_RemoveAll(void *);
void gcValue_Write(const gcValCaseRange *, cFile &);
void gcValCaseRange_gcValCaseRange(gcValCaseRange *, cBase *);
void *cMemPool_GetPoolFromPtr(void *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

unsigned int gcValCaseRange::GetTextColor(void) const {
    return 0xFF808000;
}

gcExpression *gcValCaseRange::GetChild(int index) const {
    gcValCaseRangeData *self = (gcValCaseRangeData *)this;
    return self->mChildren[index];
}

void gcValCaseRange::SetChild(int index, gcExpression *child) {
    gcValCaseRangeData *self = (gcValCaseRangeData *)this;
    self->mChildren[index] = child;
}

void gcValCaseRange::GetText(char *buf) const {
    cStrCat(buf, "case_range");
}

void gcValCaseRange::AssignCopy(const cBase *base) {
    gcValCaseRange *other = dcast(base);
    cBaseArray &src = *(cBaseArray *)((char *)other + 8);
    ((cBaseArray *)((char *)this + 8))->operator=(src);
}

extern float D_37D8C4;

float gcValCaseRange::Evaluate(void) const {
    char *child = (char *)(*(gcExpression ***)((char *)this + 8))[0];
    DispatchEntry *e = (DispatchEntry *)(((char **)child)[1] + 0x70);
    float (*fn)(char *) = (float (*)(char *))e->func;
    float result = fn(child + e->offset);

    if (D_37D8C4 < result) goto ret_result;

    child = (char *)(*(gcExpression ***)((char *)this + 8))[1];
    e = (DispatchEntry *)(((char **)child)[1] + 0x70);
    fn = (float (*)(char *))e->func;
    result = fn(child + e->offset);

    if (D_37D8C4 <= result) goto ret_threshold;

ret_result:
    return result;

ret_threshold:
    return D_37D8C4;
}

void gcDoReturn::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    int val = ((int *)this)[3];
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

// 0x00150218, 72B
gcValCaseRange::gcValCaseRange(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(char **)((char *)this + 4) = gcValCaseRangevirtualtable;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 0xC) = (int)this;
    cBaseArray_SetSize((char *)this + 8, 2);
}

// 0x0030546c, 124B
cBase *gcValCaseRange::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValCaseRange *result = 0;
    gcValCaseRange *obj = (gcValCaseRange *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        gcValCaseRange_gcValCaseRange(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValCaseRange;

const cType *gcValCaseRange::GetType(void) const {
    if (!type_gcValCaseRange) {
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
        type_gcValCaseRange = cType::InitializeType(0, 0, 0xD0, type_value,
                                                    gcValCaseRange::New,
                                                    0, 0, 0);
    }
    return type_gcValCaseRange;
}

// 0x0015006c, 128B
void gcValCaseRange::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue_Write(this, file);
    int i = 0;
    int off = 0;
    do {
        wb.WriteBase(*(const cBase **)((char *)*(void **)((char *)this + 8) + off));
        i++;
        off += 4;
    } while (i < 2);
    wb.End();
}

// 0x003058d8, 152B
extern "C" {

void gcValCaseRange___dtor_gcValCaseRange_void(gcValCaseRange *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = gcValCaseRangevirtualtable;
        void *arr = (char *)self + 8;
        if (arr != 0) {
            cBaseArray_RemoveAll(arr);
        }
        *(char **)((char *)self + 4) = cBaseclassdesc_for_gcValCaseRange;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
