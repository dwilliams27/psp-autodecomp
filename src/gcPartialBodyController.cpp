// gcPartialBodyController — derives from gcPartialEntityController.
// Decompiled functions:
//   0x0014024c gcPartialBodyController::Write(cFile &) const
//   0x00140298 gcPartialBodyController::Read(cFile &, cMemPool *)
//   0x002a5d90 gcPartialBodyController::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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

class cObject {
public:
    static int WillBeDeleted(const cBase *, const cMemPool *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);

class gcPartialEntityController {
public:
    gcPartialEntityController(cBase *);
    ~gcPartialEntityController();
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

static inline void gcPBC_delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
}

class gcPartialBodyController : public gcPartialEntityController {
public:
    gcPartialBodyController(cBase *);
    ~gcPartialBodyController();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static void operator delete(void *p) {
        gcPBC_delete(p);
    }
};

extern cType *D_000385DC;
extern cType *D_0009F5E0;
extern cType *D_0009F5F4;
extern char gcPartialBodyControllerclassdesc[];

extern "C" {
    void gcPartialBodyController__gcPartialBodyController_cBaseptr(void *self, cBase *parent);
    void *__vec_new(void *, int, int, void (*)(void *));
    void gcReplicationGroup__gcReplicationGroup_void(void *);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// =====================================================================
// 0x00140354 — gcPartialBodyController::gcPartialBodyController(cBase *)
// =====================================================================
gcPartialBodyController::gcPartialBodyController(cBase *parent)
    : gcPartialEntityController(parent) {
    *(char **)((char *)this + 4) = gcPartialBodyControllerclassdesc;
    *(int *)((char *)this + 0x38) = 0;
    *(int *)((char *)this + 0x3C) = 0;
    *(int *)((char *)this + 0x40) = 0;
    *(unsigned char *)((char *)this + 0x44) = 0;
    *(float *)((char *)this + 0x48) = 1.0f;
    __vec_new((char *)this + 0x4C, 1, 6, gcReplicationGroup__gcReplicationGroup_void);
}

// =====================================================================
// 0x001403bc — gcPartialBodyController::~gcPartialBodyController(void)
// =====================================================================
gcPartialBodyController::~gcPartialBodyController() {
    *(char **)((char *)this + 4) = gcPartialBodyControllerclassdesc;
    if (*(void **)((char *)this + 0x40) != 0) {
        void *ptr = *(void **)((char *)this + 0x40);
        cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, ptr);
        *(int *)((char *)this + 0x40) = 0;
    }
}

// =====================================================================
// 0x0014024c — gcPartialBodyController::Write(cFile &) const
// =====================================================================
void gcPartialBodyController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcPartialEntityController::Write(file);
    wb.End();
}

// =====================================================================
// 0x00140298 — gcPartialBodyController::Read(cFile &, cMemPool *)
// =====================================================================
int gcPartialBodyController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcPartialEntityController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// =====================================================================
// 0x002a5d90 — gcPartialBodyController::New(cMemPool *, cBase *) static
// =====================================================================
cBase *gcPartialBodyController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcPartialBodyController *result = 0;
    gcPartialBodyController *obj = (gcPartialBodyController *)rec->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcPartialBodyController__gcPartialBodyController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// =====================================================================
// 0x002a5e0c — gcPartialBodyController::GetType(void) const
// =====================================================================
const cType *gcPartialBodyController::GetType(void) const {
    if (D_0009F5F4 == 0) {
        if (D_0009F5E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F5E0 = cType::InitializeType(0, 0, 0x105, D_000385DC,
                                               &gcPartialEntityController::New,
                                               0, 0, 0);
        }
        D_0009F5F4 = cType::InitializeType(0, 0, 0x107, D_0009F5E0,
                                           &gcPartialBodyController::New,
                                           0, 0, 0);
    }
    return D_0009F5F4;
}

// =====================================================================
// 0x00140594 — gcPartialBodyController::OnMemPoolReset(const cMemPool *, unsigned int)
// =====================================================================
void gcPartialBodyController::OnMemPoolReset(const cMemPool *pool, unsigned int flags) {
    if (cObject::WillBeDeleted(*(cBase **)((char *)this + 0x38), pool, flags)) {
        *(int *)((char *)this + 0x38) = 0;
    }
    if (cObject::WillBeDeleted(*(cBase **)((char *)this + 0x3C), pool, flags)) {
        *(int *)((char *)this + 0x3C) = 0;
    }
}
