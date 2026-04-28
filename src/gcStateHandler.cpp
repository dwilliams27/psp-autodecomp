// gcStateHandler — derived from gcStateHandlerBase. Methods here are the
// derived-class entry points that delegate to the base implementation.

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

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

class gcStateHandlerBase {
public:
    gcStateHandlerBase(cBase *);
    ~gcStateHandlerBase();
    static cBase *New(cMemPool *, cBase *);
    gcStateHandlerBase &operator=(const gcStateHandlerBase &);
    void Write(cFile &) const;
};

class gcStateHandler;
gcStateHandler *dcast(const cBase *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool_local {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

class gcStateHandler : public gcStateHandlerBase {
public:
    gcStateHandler(cBase *);
    ~gcStateHandler();
    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
    static cBase *New(cMemPool *pool, cBase *parent);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *other);
    void Write(cFile &file) const;
};

extern char gcStateHandlervirtualtable[];

static cType *type_cBase;
static cType *type_gcStateHandlerBase;
static cType *type_gcStateHandler;

// 0x00258244, 136B
cBase *gcStateHandler::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStateHandler *result = 0;
    gcStateHandler *obj = (gcStateHandler *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        new ((gcStateHandlerBase *)obj) gcStateHandlerBase(parent);
        *(void **)((char *)obj + 4) = gcStateHandlervirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00258210, 52B
void gcStateHandler::AssignCopy(const cBase *other) {
    gcStateHandler *src = dcast(other);
    ((gcStateHandlerBase *)this)->operator=(*(gcStateHandlerBase *)src);
}

// 0x0010a930, 76B
void gcStateHandler::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcStateHandlerBase *)this)->Write(file);
    wb.End();
}

// 0x002582cc, 228B
const cType *gcStateHandler::GetType(void) const {
    if (!type_gcStateHandler) {
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
        type_gcStateHandler = cType::InitializeType(0, 0, 0xD1,
                                                    type_gcStateHandlerBase,
                                                    gcStateHandler::New,
                                                    0, 0, 0);
    }
    return type_gcStateHandler;
}

// 0x002583b0, 124B
gcStateHandler::~gcStateHandler() {
    *(void **)((char *)this + 4) = gcStateHandlervirtualtable;
}
