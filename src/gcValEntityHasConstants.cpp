// Days of Thunder decompilation: gcValEntityHasConstants
//   0x0032f92c AssignCopy(const cBase *)
//   0x0032fb98 Write(cFile &) const

#include "cBase.h"

class cFile;
class cMemPool;
class cWriteBlock;

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityHasConstants {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);
void gcLValue_Write(const gcValEntityHasConstants *, cFile &);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern char cBaseclassdesc[];
extern char gcValEntityHasConstantsvirtualtable[];
extern char D_00000338[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValEntityHasConstants asm("D_0009F80C");

// 0x0032f92c (80B) — AssignCopy
void gcValEntityHasConstants::AssignCopy(const cBase *base) {
    gcValEntityHasConstants *other = dcast<gcValEntityHasConstants>(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(cHandle *)((char *)this + 52) = *(const cHandle *)((char *)other + 52);
}

// 0x0032f97c (260B) — New
cBase *gcValEntityHasConstants::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityHasConstants *result = 0;
    gcValEntityHasConstants *obj =
        (gcValEntityHasConstants *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = gcValEntityHasConstantsvirtualtable;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[3] = D_00000338;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((void **)obj)[3] = (void *)0x388A48;
        ((void **)obj)[8] = cBaseclassdesc;
        ((void **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)sub | 1;
        ((int *)obj)[13] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0032fa80 (280B) — GetType
const cType *gcValEntityHasConstants::GetType(void) const {
    if (!type_gcValEntityHasConstants) {
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
        type_gcValEntityHasConstants =
            cType::InitializeType(0, 0, 0xD5, type_value,
                                  gcValEntityHasConstants::New,
                                  0, 0, 0);
    }
    return type_gcValEntityHasConstants;
}

// 0x0032fb98 (120B) — Write
void gcValEntityHasConstants::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue_Write(this, file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    ((const cHandle *)((const char *)this + 52))->Write(wb);
    wb.End();
}
