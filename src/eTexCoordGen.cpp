// eTexCoordGen — texture coordinate generator object derived from cBase.
// Layout: classdesc at offset 4, signed flag at 8, 5 unsigned bytes at 9..D,
// followed by 5 entries of a 20-byte item starting at offset 0x10.
//
// Functions matched here:
//   eTexCoordGen::~eTexCoordGen(void)             @ 0x00031f10
//   eTexCoordGen::AssignCopy(const cBase *)       @ 0x001e4278
//   eTexCoordGen::GetType(void) const             @ 0x001e4394

class cBase;
class cFile;
class cMemPool;
class cType;

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

extern char eTexCoordGenclassdesc[];

extern const char eTexCoordGen_base_name[];
extern const char eTexCoordGen_base_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cTypeNS {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eTexCoordGen {
public:
    char _pad[0x7C];

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

template <class T> T *dcast(const cBase *);

// ── ~eTexCoordGen ──  @ 0x00031f10, 100B
extern "C" void eTexCoordGen___dtor_eTexCoordGen_void(eTexCoordGen *self, int flags) {
    if (self != 0) {
        *(char **)((char *)self + 4) = eTexCoordGenclassdesc;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

// ── AssignCopy ──  @ 0x001e4278, 160B
struct eTexCoordItem { int w0, w1, w2, w3, w4; };

void eTexCoordGen::AssignCopy(const cBase *src) {
    eTexCoordGen *other = dcast<eTexCoordGen>(src);
    *(signed char *)((char *)this + 8) = *(const signed char *)((const char *)other + 8);
    *(unsigned char *)((char *)this + 9) = *(const unsigned char *)((const char *)other + 9);
    *(unsigned char *)((char *)this + 10) = *(const unsigned char *)((const char *)other + 10);
    *(unsigned char *)((char *)this + 11) = *(const unsigned char *)((const char *)other + 11);
    *(unsigned char *)((char *)this + 12) = *(const unsigned char *)((const char *)other + 12);
    *(unsigned char *)((char *)this + 13) = *(const unsigned char *)((const char *)other + 13);

    int i = 0;
    eTexCoordItem *d = (eTexCoordItem *)((char *)this + 0x10);
    const eTexCoordItem *s = (const eTexCoordItem *)((const char *)other + 0x10);
    do {
        *d = *s;
        i++;
        d++;
        s++;
    } while (i < 5);
}

// ── GetType ──  @ 0x001e4394, 160B
static cType *type_base;
static cType *type_eTexCoordGen;

const cType *eTexCoordGen::GetType(void) const {
    if (!type_eTexCoordGen) {
        if (!type_base) {
            type_base = cTypeNS::InitializeType(eTexCoordGen_base_name, eTexCoordGen_base_desc, 1,
                                                0, 0, 0, 0, 0);
        }
        type_eTexCoordGen = cTypeNS::InitializeType(0, 0, 0x47, type_base, &eTexCoordGen::New,
                                                    0, 0, 0);
    }
    return type_eTexCoordGen;
}
