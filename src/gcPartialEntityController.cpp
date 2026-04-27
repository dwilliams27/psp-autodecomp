// gcPartialEntityController — decompiled from Days of Thunder (PSP)
//   0x0013e6f0 gcPartialEntityController::Read(cFile &, cMemPool *)
//   0x0013e768 gcPartialEntityController::gcPartialEntityController(cBase *)
//   0x0013e7c8 gcPartialEntityController::~gcPartialEntityController(void)
//   0x0013e82c gcPartialEntityController::Reset(cMemPool *, bool)
//   0x002a0878 gcPartialEntityController::AssignCopy(const cBase *)
//   0x002a0964 gcPartialEntityController::New(cMemPool *, cBase *) static
//   0x002a09e0 gcPartialEntityController::GetType(void) const

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;
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
}

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

extern "C" {
    void cFile_SetCurrentPos(void *, unsigned int);
    void *__vec_new(void *, int, int, void (*)(void *));
    void gcReplicationGroup__gcReplicationGroup_void(void *);
}

// Pool-block layout used by the deleting-destructor tail.
struct gcPEC_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcPEC_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcPEC_cMemPoolNS {
public:
    static gcPEC_cMemPoolNS *GetPoolFromPtr(const void *);
};

extern char gcPartialEntityControllerclassdesc[];
extern char gcPartialEntityControllerbasevtable[];
extern char cBaseclassdesc[];

extern const char gcPartialEntityController_base_name[];
extern const char gcPartialEntityController_base_desc[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcPartialEntityController {
public:
    gcPartialEntityController(cBase *);
    ~gcPartialEntityController();
    void Reset(cMemPool *, bool);
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcPEC_cMemPoolNS *pool = gcPEC_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcPEC_DeleteRecord *rec =
            (gcPEC_DeleteRecord *)
            (((gcPEC_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

gcPartialEntityController *dcast(const cBase *);

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
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard,
// the absence of a parent-destructor chain (cBase has no virtual dtor here),
// and the deleting-tail dispatch through operator delete on (flag & 1).
gcPartialEntityController::~gcPartialEntityController() {
    *(void **)((char *)this + 4) = gcPartialEntityControllerbasevtable;
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
        new (obj) gcPartialEntityController(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x002a09e0 — GetType
static cType *type_base;
static cType *type_gcPartialEntityController;

const cType *gcPartialEntityController::GetType(void) const {
    if (!type_gcPartialEntityController) {
        if (!type_base) {
            type_base = cType::InitializeType(gcPartialEntityController_base_name,
                                              gcPartialEntityController_base_desc, 1,
                                              0, 0, 0, 0, 0);
        }
        type_gcPartialEntityController = cType::InitializeType(0, 0, 0x105, type_base,
                                                               &gcPartialEntityController::New,
                                                               0, 0, 0);
    }
    return type_gcPartialEntityController;
}

// 0x002a0878 — AssignCopy
struct gcReplicationGroupShorts { short a, b, c; };

void gcPartialEntityController::AssignCopy(const cBase *base) {
    gcPartialEntityController *other = dcast(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(int *)((char *)other + 12);
    *(unsigned char *)((char *)this + 16) = *(unsigned char *)((char *)other + 16);
    *(unsigned char *)((char *)this + 17) = *(unsigned char *)((char *)other + 17);
    {
        int *d = (int *)((char *)this + 20);
        int *s = (int *)((char *)other + 20);
        *d = *s;
        d = (int *)((char *)this + 24);
        s = (int *)((char *)other + 24);
        *d = *s;
        d = (int *)((char *)this + 28);
        s = (int *)((char *)other + 28);
        *d = *s;
        d = (int *)((char *)this + 32);
        s = (int *)((char *)other + 32);
        *d = *s;
        d = (int *)((char *)this + 36);
        s = (int *)((char *)other + 36);
        *d = *s;
        d = (int *)((char *)this + 40);
        s = (int *)((char *)other + 40);
        *d = *s;
    }
    *(int *)((char *)this + 44) = *(int *)((char *)other + 44);
    *(unsigned short *)((char *)this + 48) = *(unsigned short *)((char *)other + 48);

    int i = 0;
    gcReplicationGroupShorts *dst = (gcReplicationGroupShorts *)((char *)this + 50);
    gcReplicationGroupShorts *src = (gcReplicationGroupShorts *)((char *)other + 50);
    do {
        *dst = *src;
        ++i;
        ++dst;
        ++src;
    } while (i <= 0);
}
