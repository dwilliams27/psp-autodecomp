#include "gcDoUIFade.h"
#include "cBase.h"

extern char gcValBinaryOpvirtualtable[];
extern char gcValuevirtualtable_for_gcValBinaryOp[];

void cBaseArray_SetSize(void *, int);
void cBaseArray_RemoveAll(void *);
void gcValBinaryOp_gcValBinaryOp(gcValBinaryOp *, cBase *);
void gcValue_Write(const gcValBinaryOp *, cFile &);
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

// Function 1: 0x00320bd0, 8 bytes
int gcValBinaryOp::GetMaxChildren(void) const {
    return 2;
}

// Function 2: 0x001586f0, 76 bytes
gcValBinaryOp::gcValBinaryOp(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(char **)((char *)this + 4) = gcValBinaryOpvirtualtable;
    *(int *)((char *)this + 8) = 0;
    *(int *)((char *)this + 0xC) = 0;
    *(int *)((char *)this + 0x10) = (int)this;
    cBaseArray_SetSize((char *)this + 0xC, 2);
}

// Function 3: 0x00320a3c, 124 bytes
cBase *gcValBinaryOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValBinaryOp *result = 0;
    gcValBinaryOp *obj = (gcValBinaryOp *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcValBinaryOp_gcValBinaryOp(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// Function 4: 0x00158520, 140 bytes
void gcValBinaryOp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue_Write(this, file);
    wb.Write(*(int *)((char *)this + 8));
    int i = 0;
    int off = 0;
    do {
        wb.WriteBase(*(const cBase **)((char *)*(void **)((char *)this + 0xC) + off));
        i++;
        off += 4;
    } while (i < 2);
    wb.End();
}

// Function 5: 0x00320edc, 152 bytes
extern "C" {

void gcValBinaryOp___dtor_gcValBinaryOp_void(gcValBinaryOp *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = gcValBinaryOpvirtualtable;
        void *arr = (char *)self + 0xC;
        if (arr != 0) {
            cBaseArray_RemoveAll(arr);
        }
        *(char **)((char *)self + 4) = gcValuevirtualtable_for_gcValBinaryOp;
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
