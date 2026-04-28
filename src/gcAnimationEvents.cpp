class cBase;
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
    short pad;
    void (*fn)(void *, void *);
};

extern char gcAnimationEventvirtualtable[];
extern char cBaseclassdesc[];

extern const char gcAnimationEvents_base_name[];
extern const char gcAnimationEvents_base_desc[];

extern cType *D_000385DC;
extern cType *D_00099AB8;

void *cMemPool_GetPoolFromPtr(const void *);

extern "C" {
    void gcAnimationEvents__gcAnimationEvents_cBaseptr(void *, cBase *);
    void gcEvent___dtor_gcEvent_void(void *, int);
}

class gcAnimationEvents {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    ~gcAnimationEvents();
};

inline void operator delete(void *p) {
    void *pool = cMemPool_GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
    short off = rec->offset;
    rec->fn((char *)block + off, p);
}

cBase *gcAnimationEvents::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcAnimationEvents *result = 0;
    gcAnimationEvents *obj = (gcAnimationEvents *)entry->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        gcAnimationEvents__gcAnimationEvents_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcAnimationEvents::GetType(void) const {
    if (D_00099AB8 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType(gcAnimationEvents_base_name,
                                               gcAnimationEvents_base_desc,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AB8 = cType::InitializeType(0, 0, 0x1A3, D_000385DC,
                                           &gcAnimationEvents::New, 0, 0, 0);
    }
    return D_00099AB8;
}

gcAnimationEvents::~gcAnimationEvents() {
    *(void **)((char *)this + 4) = gcAnimationEventvirtualtable;
    gcEvent___dtor_gcEvent_void((char *)this + 0x5C, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0x40, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0x24, 2);
    gcEvent___dtor_gcEvent_void((char *)this + 0x08, 2);
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
