// cNamed — a 32-byte named base object: parent, classdesc, cName (name buffer + 2 shorts).
//
// Functions matched here:
//   cNamed::Write(cFile &) const                  @ 0x00008e64  (cAll_psp.obj)
//   cNamed::Read(cFile &, cMemPool *)             @ 0x00008ea4  (cAll_psp.obj)
//   cNamed::GenerateName(const char *)            @ 0x00008f50  (cAll_psp.obj)
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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cName {
public:
    void Read(cReadBlock &);
    void Set(const char *, ...);
    void Write(cWriteBlock &) const;
};

void cFile_SetCurrentPos(void *, unsigned int);

unsigned int cIRand(void);
int cStrLength(const char *);
void cStrCopy(char *, const char *, int);
void cStrAppend(char *, const char *, ...);

extern char cBaseclassdesc[];
extern char cNamedclassdesc[];

extern const char cNamed_base_name[];
extern const char cNamed_base_desc[];
extern const char cNamed_genname_fmt[];   // @ 0x36CA64

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

struct DispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *);
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

class cNamedPoolNS {
public:
    static cNamedPoolNS *GetPoolFromPtr(const void *);
};

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
    ~cNamed();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GenerateName(const char *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        cNamedPoolNS *pool = cNamedPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

cNamed *dcast(const cBase *);

// ── Write ──  @ 0x00008e64, 64B
void cNamed::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cName *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// ── Read ──  @ 0x00008ea4, 172B
int cNamed::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    ((cName *)((char *)this + 8))->Read(rb);
    DispatchEntry *e = (DispatchEntry *)(*(char **)((char *)this + 4) + 0x70);
    e->fn((char *)this + e->offset);
    return result;
}

// ── GenerateName ──  @ 0x00008f50, 224B
void cNamed::GenerateName(const char *src) {
    char buf[256];
    int n = cStrLength(src) - 1;
    while (n >= 0) {
        signed char c = src[n];
        if (!(((c >= '0') & (c < ':')) & 0xff)) break;
        n--;
    }
    n += 2;
    int len = (n < 18) ? n : 18;
    buf[0] = 0;
    cStrCopy(buf, src, len);
    cStrAppend(buf, cNamed_genname_fmt, cIRand() % 100);
    ((cName *)((char *)this + 8))->Set(buf);
    DispatchEntry *e = (DispatchEntry *)(*(char **)((char *)this + 4) + 0x70);
    e->fn((char *)this + e->offset);
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
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain (cBase has none), and the deleting-
// tail dispatch through operator delete on (flag & 1). Body just resets the
// classdesc pointer at offset 4 to the parent (cBase) classdesc.
cNamed::~cNamed() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
