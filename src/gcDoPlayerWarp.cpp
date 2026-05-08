#include "cBase.h"

class cFile;
class cMemPool;
class cType;

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class gcDoPlayerWarp {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    float Evaluate(void) const;
    void GetText(char *) const;
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

namespace gcPlayer {
    void HandleClientWarp(void);
}

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cStrAppend(char *, const char *, ...);
extern "C" void gcAction_gcAction(void *, cBase *);
void gcAction_Write(const gcDoPlayerWarp *, cFile &);
int gcAction_Read(void *, cFile &, cMemPool *);
gcDoPlayerWarp *dcast(const cBase *);
extern char gcDoPlayerWarpvirtualtable[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoPlayerWarp asm("D_0009F6D0");

cBase *gcDoPlayerWarp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPlayerWarp *result = 0;
    gcDoPlayerWarp *obj = (gcDoPlayerWarp *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoPlayerWarpvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoPlayerWarp::GetType(void) const {
    if (!type_gcDoPlayerWarp) {
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
        type_gcDoPlayerWarp = cType::InitializeType(
            0, 0, 0x1DC, type_action, gcDoPlayerWarp::New, 0, 0, 0);
    }
    return type_gcDoPlayerWarp;
}

void gcDoPlayerWarp::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)this, arg3);
    }
}

float gcDoPlayerWarp::Evaluate(void) const {
    gcPlayer::HandleClientWarp();
    return 1.0f;
}

void gcDoPlayerWarp::GetText(char *buf) const {
    cStrAppend(buf, "PlayerWarp( )");
}

void gcDoPlayerWarp::AssignCopy(const cBase *base) {
    gcDoPlayerWarp *other = dcast(base);
    int *self = (int *)this;
    int *src = (int *)other;
    self[2] = self[2] & ~3;
    self[2] = self[2] | (src[2] & 3);
}

void gcDoPlayerWarp::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.End();
}

int gcDoPlayerWarp::Read(cFile &file, cMemPool *pool) {
    register int ok __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(ok));
    if ((unsigned int)rb._data[3] == 1 && gcAction_Read(this, file, pool)) goto success;
    ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
    return 0;
success:
    return ok;
}
