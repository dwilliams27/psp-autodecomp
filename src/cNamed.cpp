// cNamed — a 32-byte named base object: parent, classdesc, cName (name buffer + 2 shorts).
//
// Functions matched here:
//   cNamed::Write(cFile &) const                  @ 0x00008e64  (cAll_psp.obj)
//   cNamed::AssignCopy(const cBase *)             @ 0x001c6dd4  (cAll_psp.obj)
//   cNamed::GetType(void) const                   @ 0x001c6e34  (cAll_psp.obj)
//   cNamed::~cNamed(void)                         @ 0x001c6f00  (cAll_psp.obj)
//   cNamed::New(cMemPool *, cBase *) static       @ 0x001c3c58  (gcDoReadFile.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cType;
class cBase;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

extern char cBaseclassdesc[];
extern char cNamedclassdesc[];

extern const char cNamed_base_name[];
extern const char cNamed_base_desc[];

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

class cBase {
public:
    cBase *mOwner;          // 0
    void *mClassDesc;       // 4

    cBase(cBase *owner) {
        mClassDesc = cBaseclassdesc;
        mOwner = owner;
    }
};

struct cNameData { int _w[6]; };  // 24-byte name buffer at cNamed+8

class cNamed : public cBase {
public:
    cNameData mName;        // 8..0x20

    // Constructor defined inline so SNC inlines it into ::New.
    cNamed(cBase *parent) : cBase(parent) {
        mClassDesc = cNamedclassdesc;
        *(short *)((char *)this + 0x1C) = 0;
        *(short *)((char *)this + 0x1E) = 0;
        *((char *)this + 0x08) = 0;
    }
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

cNamed *dcast(const cBase *);

// ── Write ──  @ 0x00008e64, 64B
void cNamed::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cName *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// ── AssignCopy ──  @ 0x001c6dd4, 96B
void cNamed::AssignCopy(const cBase *base) {
    cNamed *other = dcast(base);
    cNameData *src = &other->mName;
    cNameData *dst = &mName;
    *dst = *src;
}

// ── New ──  @ 0x001c3c58, 144B
cBase *cNamed::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    cNamed *result = 0;
    cNamed *obj = (cNamed *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        new (obj) cNamed(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ──  @ 0x001c6e34, 160B
static cType *type_base;
static cType *type_cNamed;

const cType *cNamed::GetType(void) const {
    if (!type_cNamed) {
        if (!type_base) {
            type_base = cType::InitializeType(cNamed_base_name, cNamed_base_desc, 1,
                                              0, 0, 0, 0, 0);
        }
        type_cNamed = cType::InitializeType(0, 0, 2, type_base, &cNamed::New,
                                            0, 0, 0);
    }
    return type_cNamed;
}

// ── Destructor ──  @ 0x001c6f00, 100B
extern "C" {

void cNamed___dtor_cNamed_void(cNamed *self, int flags) {
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
