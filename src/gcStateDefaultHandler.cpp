class cBase;
class cMemPool;
class cType;
class gcState;
class gcStateMachine;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcStateHandlerBase {
public:
    gcStateHandlerBase(cBase *);
    static cBase *New(cMemPool *, cBase *);
    gcStateHandlerBase &operator=(const gcStateHandlerBase &);
};

template <class T>
class cSubHandleT;

template <class A, class B>
class cHandlePairT;

template <class T>
class cArrayBase {
public:
    cArrayBase &operator=(const cArrayBase &);
};

template <class T>
T dcast(const cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcStateDefaultHandler : public gcStateHandlerBase {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
};

extern char gcStateHandlervirtualtable[];

static cType *type_cBase;
static cType *type_gcStateHandlerBase;
static cType *type_gcStateDefaultHandler;

typedef cHandlePairT<gcStateMachine, cSubHandleT<gcState> > StatePair;
typedef cArrayBase<StatePair> StatePairArray;

cBase *gcStateDefaultHandler::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStateDefaultHandler *result = 0;
    gcStateDefaultHandler *obj =
        (gcStateDefaultHandler *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        new ((gcStateHandlerBase *)obj) gcStateHandlerBase(parent);
        *(void **)((char *)obj + 4) = gcStateHandlervirtualtable;
        *(int *)((char *)obj + 0x30) = 0;
        *(int *)((char *)obj + 0x34) = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcStateDefaultHandler::GetType(void) const {
    if (!type_gcStateDefaultHandler) {
        if (!type_gcStateHandlerBase) {
            if (!type_cBase) {
                type_cBase = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            type_gcStateHandlerBase = cType::InitializeType(0, 0, 0xD9,
                                                            type_cBase,
                                                            gcStateHandlerBase::New,
                                                            0, 0, 0);
        }
        type_gcStateDefaultHandler = cType::InitializeType(0, 0, 0xDA,
                                                           type_gcStateHandlerBase,
                                                           gcStateDefaultHandler::New,
                                                           0, 0, 0);
    }
    return type_gcStateDefaultHandler;
}

void gcStateDefaultHandler::AssignCopy(const cBase *base) {
    gcStateDefaultHandler *src = dcast<gcStateDefaultHandler *>(base);
    ((gcStateHandlerBase *)this)->operator=(*(gcStateHandlerBase *)src);
    ((StatePairArray *)((char *)this + 0x30))->operator=(
        *(const StatePairArray *)((char *)src + 0x30));
    ((StatePairArray *)((char *)this + 0x34))->operator=(
        *(const StatePairArray *)((char *)src + 0x34));
}
