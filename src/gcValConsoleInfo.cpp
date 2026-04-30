#include "gcValConsoleInfo.h"
#include "cBase.h"

class cFile;
class cMemPool;

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

class cType {
public:
    char pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

void gcValue_Write(const gcValConsoleInfo *, cFile &);
void gcValConsoleInfo_gcValConsoleInfo(void *, cBase *);

extern const char gcValConsoleInfo_base_name[];
extern const char gcValConsoleInfo_base_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

// -----------------------------------------------------------------------------
// Function 1: gcValConsoleInfo::gcValConsoleInfo(cBase *)
// -----------------------------------------------------------------------------
gcValConsoleInfo::gcValConsoleInfo(cBase *parent) {
    this->pad0 = (int)parent;
    this->pad4 = 0x38D558;
    this->pad8 = 0;
    this->padC = 0;
    this->pad10 = (int)this | 1;
    this->pad14 = (int)this | 1;
}

// -----------------------------------------------------------------------------
// Function 2: gcValConsoleInfo::Evaluate(void) const
// -----------------------------------------------------------------------------
int gcValConsoleInfo::Evaluate(void) const {
    return this->EvaluateCommon();
}

// -----------------------------------------------------------------------------
// Function 3: gcValConsoleInfo::New(cMemPool *, cBase *) static
// -----------------------------------------------------------------------------
cBase *gcValConsoleInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValConsoleInfo *result = 0;
    gcValConsoleInfo *obj = (gcValConsoleInfo *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcValConsoleInfo_gcValConsoleInfo(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// Function 4: gcValConsoleInfo::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcValConsoleInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcValue_Write(this, file);
    wb.Write(this->pad8);
    wb.Write(this->padC);
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);
    int val = this->pad14;
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

// -----------------------------------------------------------------------------
// Function 5: gcValConsoleInfo::GetType(void) const
// -----------------------------------------------------------------------------
static cType *type_base;
static cType *type_6A;
static cType *type_6C;
static cType *type_final;

const cType *gcValConsoleInfo::GetType(void) const {
    if (!type_final) {
        if (!type_6C) {
            if (!type_6A) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcValConsoleInfo_base_name, gcValConsoleInfo_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_6A = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_6C = cType::InitializeType(0, 0, 0x6C, type_6A, 0, 0, 0, 0x80);
        }
        type_final = cType::InitializeType(0, 0, 0x1E0, type_6C, gcValConsoleInfo::New, 0, 0, 0);
    }
    return type_final;
}

// -----------------------------------------------------------------------------
// Function 6: gcValConsoleInfo::AssignCopy(const cBase *)
// -----------------------------------------------------------------------------
void gcValConsoleInfo::AssignCopy(const cBase *base) {
    const gcValConsoleInfo *other = 0;

    if (base != 0) {
        if (!type_final) {
            if (!type_6C) {
                if (!type_6A) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_6A = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_6C = cType::InitializeType(
                    0, 0, 0x6C, type_6A, 0, 0, 0, 0x80);
            }
            type_final = cType::InitializeType(
                0, 0, 0x1E0, type_6C, gcValConsoleInfo::New, 0, 0, 0);
        }

        DispatchEntry *entry =
            (DispatchEntry *)((char *)*(void **)((char *)base + 4) + 8);
        cType *wanted = type_final;
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
            other = (const gcValConsoleInfo *)base;
        }
    }

    operator=(*other);
}
