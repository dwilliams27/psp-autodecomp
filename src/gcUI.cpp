// gcUI — UI manager. Functions matched here:
//   gcUI::~gcUI(void)                          @ 0x000e04bc  (gcAll_psp.obj)
//   gcUI::New(cMemPool *, cBase *) static      @ 0x0023d8d8  (gcAll_psp.obj)
//   gcUI::GetType(void) const                  @ 0x0023d958  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cType;
class cBase;

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

extern char cBaseclassdesc[];                  // 0x37E6A8

extern const char gcUI_base_name[];            // 0x36D894
extern const char gcUI_base_desc[];            // 0x36D89C

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcUI {
public:
    gcUI(cBase *, unsigned int);
    ~gcUI();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

// ── New ──  @ 0x0023d8d8, 128B
cBase *gcUI::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcUI *result = 0;
    gcUI *obj = (gcUI *)entry->fn(base, 0x188, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUI(parent, 0);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ──  @ 0x0023d958, 160B
static cType *type_base;
static cType *type_gcUI;

const cType *gcUI::GetType(void) const {
    if (!type_gcUI) {
        if (!type_base) {
            type_base = cType::InitializeType(gcUI_base_name, gcUI_base_desc, 1,
                                              0, 0, 0, 0, 0);
        }
        type_gcUI = cType::InitializeType(0, 0, 0xED, type_base, &gcUI::New,
                                          0, 0, 0);
    }
    return type_gcUI;
}

// ── Destructor ──  @ 0x000e04bc, 100B
extern "C" {

void gcUI___dtor_gcUI_void(gcUI *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = cBaseclassdesc;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
