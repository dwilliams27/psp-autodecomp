// Days of Thunder decompilation: gcValEntityPrimaryController
//   0x0033b0a0 AssignCopy(const cBase *)
//   0x0033b304 Write(cFile &) const
//   0x0033b530 GetText(char *) const

#include "gcValEntityPrimaryController.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
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
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];         // to 0x78
    cTypeMethod text_m;     // 0x78

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
    void *Get(bool) const;
};

gcValEntityPrimaryController *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void gcLValue_Write(const gcValEntityPrimaryController *, cFile &);
int gcValue_Read(gcValEntityPrimaryController *, cFile &, cMemPool *);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
}

extern const char gcValEntityPrimaryController_text_fmt[];
extern const char gcValEntityPrimaryController_text_arg[];
extern const char gcValEntityPrimaryController_text_suffix[];
extern char cBaseclassdesc[];
extern char D_00007C78[];
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
static cType *type_gcValEntityPrimaryController asm("D_0009F83C");

// 0x0033b0a0 (72B) — AssignCopy
void gcValEntityPrimaryController::AssignCopy(const cBase *base) {
    gcValEntityPrimaryController *other = dcast(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)(other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 52) = *(const int *)((char *)other + 52);
}

// 0x0033b0e8 (260B) — New (static factory)
cBase *gcValEntityPrimaryController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityPrimaryController *result = 0;
    gcValEntityPrimaryController *obj =
        (gcValEntityPrimaryController *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = D_00007C78;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[3] = D_00000338;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((void **)obj)[3] = (void *)0x388A48;
        ((char **)obj)[8] = cBaseclassdesc;
        ((char **)obj)[7] = sub;
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

// 0x0033b1ec (280B) — GetType
const cType *gcValEntityPrimaryController::GetType(void) const {
    if (!type_gcValEntityPrimaryController) {
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
        type_gcValEntityPrimaryController =
            cType::InitializeType(0, 0, 0x1C9, type_value,
                                  gcValEntityPrimaryController::New,
                                  0, 0, 0);
    }
    return type_gcValEntityPrimaryController;
}

// 0x0033b304 (120B) — Write
void gcValEntityPrimaryController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue_Write(this, file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.Write(*(const int *)((const char *)this + 52));
    wb.End();
}

// 0x0033b37c (268B) — Read
int gcValEntityPrimaryController::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int tag = rb._pad[1];
    int version;
    __asm__ volatile("ori %0, $0, 1" : "=r"(version));
    if (tag != version || gcValue_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(rb.file, rb._pos);
        return 0;
    }
    char *sub = (char *)this + 8;
    char *mType = *(char **)((char *)this + 12);
    const cTypeMethod *e = (const cTypeMethod *)(mType + 48);
    cFile *f = rb.file;
    typedef void (*ReadFn)(void *, cFile *, void *);
    ((ReadFn)e->fn)(sub + e->offset, f, cMemPool_GetPoolFromPtr(sub));
    cFileSystem_Read(*(void **)rb.file, (char *)this + 52, 4);
    return 1;
}

// 0x0033b530 (104B) — GetText
void gcValEntityPrimaryController::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf,
               gcValEntityPrimaryController_text_fmt,
               gcValEntityPrimaryController_text_arg);
    cStrAppend(buf, gcValEntityPrimaryController_text_suffix);
}

// 0x0033b488 (168B) — Evaluate
float gcValEntityPrimaryController::Evaluate(void) const {
    float neg_one = -1.0f;
    char *entity = (char *)((const gcDesiredEntity *)((const char *)this + 8))->Get(true);
    int selector;
    if (entity == 0) goto retNegOne;
    selector = *(const int *)((const char *)this + 52);
    if (selector > 0) goto ret64;
    if (selector < 0) goto retNegOne;
    return (float)entity[0x65];
retNegOne:
    return neg_one;
ret64:
    if (selector < 2) return (float)entity[0x64];
    goto retNegOne;
}
