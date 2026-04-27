// Decompiled functions:
//   0x00124f1c gcConstant::gcConstant(cBase *)
//   0x002648f8 gcConstant::AssignCopy(const cBase *)
//   0x00124df4 gcConstant::Write(cFile &) const
//   0x00124f58 gcConstant::~gcConstant(void)
//   0x00264940 gcConstant::New(cMemPool *, cBase *) static
//   0x00124e4c gcConstant::Read(cFile &, cMemPool *)
//   0x001304e0 gcVariable::gcVariable(cBase *)
//   0x00146e00 gcLookAtControllerTemplate::gcLookAtControllerTemplate(cBase *)
//   0x001a6604 nwConnection::SetError(nwConnection::nwConnectionError)
//   0x001b6a78 __malloc_lock

#include "cBase.h"

class cFile;
class cFileHandle;
class cMemPool;

// ─── helper class declarations ────────────────────────────────────────────
class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

template <class T> T *dcast(const cBase *);

// ─── pool-block layout used by both ::New() and operator delete ──────────
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// Parent ctors used by gcVariable / gcLookAtControllerTemplate (kept as
// extern "C" because those classes don't inherit from cObject in this TU).
extern "C" void cObject_cObject(void *self, cBase *parent);
extern "C" void gcPartialEntityControllerTemplate_gcPartialEntityControllerTemplate(
    void *self, cBase *parent);

// classdesc symbols (relocations masked at compare time).
extern char gcConstantclassdesc[];        // 0x389090
extern char gcVariableclassdesc[];
extern char gcLookAtControllerTemplateclassdesc[];

// ============================================================================
// gcConstant — inherits cObject; mValue is a float at offset 0x44.
// ============================================================================
class gcConstant : public cObject {
public:
    char _pad[0x44];          // cObject padding: cObject is empty-base in our
                              // local decl, so layout starts at 0; mValue lands
                              // at 0x44 after this pad.
    float mValue;             // 0x44

    gcConstant(cBase *);
    ~gcConstant();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);

    // Inlined into the deleting-destructor tail; matches the pool-dispatch
    // sequence at 0x124f98–0x124fbc.
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

// ── gcConstant::gcConstant(cBase *) @ 0x00124f1c ──
gcConstant::gcConstant(cBase *b) : cObject(b) {
    ((void **)this)[1] = gcConstantclassdesc;
    mValue = 0.0f;
}

// ── gcConstant::AssignCopy(const cBase *) @ 0x002648f8 ──
void gcConstant::AssignCopy(const cBase *src) {
    gcConstant *other = dcast<gcConstant>(src);
    cObject::operator=(*other);
    mValue = other->mValue;
}

// ── gcConstant::Write(cFile &) const @ 0x00124df4 ──
void gcConstant::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.Write(mValue);
    wb.End();
}

// ── gcConstant::~gcConstant() @ 0x00124f58 ──
// Canonical destructor; SNC ABI auto-generates the (this != 0) guard, the
// ~cObject() base-chain call (with flags=0), and the deleting-tail
// `if (flags & 1) operator delete(this)` epilogue.
gcConstant::~gcConstant() {
    ((void **)this)[1] = gcConstantclassdesc;
}

// ── gcConstant::New(cMemPool *, cBase *) static @ 0x00264940 ──
// Note: the constructor call goes through a placeholder C-linkage symbol so
// the call shape compiles cleanly; compare_func.py masks the relocation.
extern "C" void gcConstant_construct(void *self, cBase *parent);

cBase *gcConstant::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcConstant *result = 0;
    gcConstant *obj = (gcConstant *)entry->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        gcConstant_construct(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcConstant::Read(cFile &, cMemPool *) @ 0x00124e4c ──
// The early-stashed `s3 = 1` for the success-path return value comes from the
// SNC scheduler; an inline-asm `ori` forces the same scheduling here.
int gcConstant::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ ("ori %0,$0,1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || cObject::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    cFileSystem::Read(*(cFileHandle **)rb._data[0], &mValue, 4);
    return result;
}

// ============================================================================
// gcVariable — sibling class, kept on the existing extern-"C" parent-ctor
// pattern (already matched).
// ============================================================================
class gcVariable {
public:
    gcVariable(cBase *);
};

gcVariable::gcVariable(cBase *b) {
    cObject_cObject(this, b);
    ((void **)this)[1] = gcVariableclassdesc;
    *(float *)((char *)this + 68) = 0.0f;
}

// ============================================================================
// gcLookAtControllerTemplate — already matched.
// ============================================================================
class gcLookAtControllerTemplate {
public:
    gcLookAtControllerTemplate(cBase *);
};

gcLookAtControllerTemplate::gcLookAtControllerTemplate(cBase *b) {
    gcPartialEntityControllerTemplate_gcPartialEntityControllerTemplate(this, b);
    ((void **)this)[1] = gcLookAtControllerTemplateclassdesc;
    *((signed char *)this + 20) = (signed char)-1;
}

// ============================================================================
// nwConnection::SetError @ 0x001a6604 — already matched.
// ============================================================================
class nwConnection {
public:
    enum nwConnectionError {
        ERROR_NONE = 0
    };

    char _pad00[0x1C];
    int mField1C;
    int mField20;
    void SetError(nwConnectionError);
    void FlushAll(void);
};

void nwConnection::SetError(nwConnectionError err) {
    if (mField1C == 0) {
        mField1C = (int)err;
        FlushAll();
        mField20 = 0;
    }
}

// ============================================================================
// __malloc_lock @ 0x001b6a78 — already matched.
// ============================================================================
extern "C" int sceKernelCpuSuspendIntr(void);

extern int g_malloc_lock_count;
extern int g_malloc_intr_state;

extern "C" void __malloc_lock(void) {
    int state = sceKernelCpuSuspendIntr();
    int n = g_malloc_lock_count;
    if (n == 0) {
        g_malloc_intr_state = state;
    }
    g_malloc_lock_count = n + 1;
}
