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
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

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
