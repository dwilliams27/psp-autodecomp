// gcPartialEntityController — decompiled from Days of Thunder (PSP)
//   0x0013e6f0 gcPartialEntityController::Read(cFile &, cMemPool *)
//   0x0013e768 gcPartialEntityController::gcPartialEntityController(cBase *)
//   0x0013e7c8 gcPartialEntityController::~gcPartialEntityController(void)
//   0x0013e82c gcPartialEntityController::Reset(cMemPool *, bool)
//   0x002a0964 gcPartialEntityController::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class gcStateMachine;
class gcState;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
    cHandlePairT() { mHandle.mIndex = 0; mSubHandle.mIndex = 0; }
};

class gcStateInfo {
public:
    void Reset(cHandleT<gcStateMachine> handle,
               const cHandlePairT<gcStateMachine, cSubHandleT<gcState> > &pair);
};

extern "C" {
    void gcStateInfo__gcStateInfo_void(void *);
    void gcPartialEntityController__gcPartialEntityController_cBaseptr(void *, void *);
}

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

extern "C" {
    void cFile_SetCurrentPos(void *, unsigned int);
    void *cMemPool_GetPoolFromPtr(void *);
    void *__vec_new(void *, int, int, void (*)(void *));
    void gcReplicationGroup__gcReplicationGroup_void(void *);
}

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern char gcPartialEntityControllerclassdesc[];
extern char gcPartialEntityControllerbasevtable[];

class gcPartialEntityController {
public:
    gcPartialEntityController(cBase *);
    void Reset(cMemPool *, bool);
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
};

// 0x0013e768 — ctor
gcPartialEntityController::gcPartialEntityController(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(char **)((char *)this + 4) = gcPartialEntityControllerclassdesc;
    *(int *)((char *)this + 8) = 0;
    gcStateInfo__gcStateInfo_void((char *)this + 12);
    *(unsigned short *)((char *)this + 48) = 3;
    __vec_new((char *)this + 50, 1, 6, gcReplicationGroup__gcReplicationGroup_void);
}

// 0x0013e7c8 — dtor
extern "C" void gcPartialEntityController___dtor_gcPartialEntityController_void(
        gcPartialEntityController *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = gcPartialEntityControllerbasevtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

// 0x0013e82c — Reset
void gcPartialEntityController::Reset(cMemPool *, bool) {
    *(unsigned short *)((char *)this + 48) |= 3;
    gcStateInfo *info = (gcStateInfo *)((char *)this + 12);
    info->Reset(*(cHandleT<gcStateMachine> *)((char *)*(void **)((char *)this + 8) + 16),
                cHandlePairT<gcStateMachine, cSubHandleT<gcState> >());
}

// 0x0013e6f0 — Read
int gcPartialEntityController::Read(cFile &file, cMemPool *) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return result;
}

// 0x002a0964 — New (static factory)
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcPartialEntityController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcPartialEntityController *result = 0;
    gcPartialEntityController *obj =
        (gcPartialEntityController *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcPartialEntityController__gcPartialEntityController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
