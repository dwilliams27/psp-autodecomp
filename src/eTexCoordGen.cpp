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
class cWriteBlock;

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

    eTexCoordGen(cBase *);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(char);
    void Write(unsigned char);
    void Write(int);
    void Write(int, const float *);
    void End(void);
};

template <class T> T *dcast(const cBase *);

extern "C" void *__vec_new(void *, int, int, void (*)(void *));

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

eTexCoordGen::eTexCoordGen(cBase *parent) {
    *(cBase **)this = parent;
    *(char **)((char *)this + 4) = (char *)0x3804B0;
    *(signed char *)((char *)this + 8) = 1;
    *(unsigned char *)((char *)this + 9) = 1;
    *(unsigned char *)((char *)this + 10) = 1;
    *(unsigned char *)((char *)this + 11) = 0;
    *(unsigned char *)((char *)this + 12) = 0;
    *(unsigned char *)((char *)this + 13) = 1;
    __vec_new((char *)this + 0x10, 5, 0x14, (void (*)(void *))0x1E4250);
    *(int *)((char *)this + 0x20) = 1;
    *(float *)((char *)this + 0x10) = 0.0f;
    *(float *)((char *)this + 0x14) = 1.0f;
    *(float *)((char *)this + 0x18) = 0.0f;
    *(float *)((char *)this + 0x1C) = 1.0f;
    *(int *)((char *)this + 0x34) = 1;
    *(float *)((char *)this + 0x24) = 0.0f;
    *(float *)((char *)this + 0x28) = 1.0f;
    *(float *)((char *)this + 0x2C) = 0.0f;
    *(float *)((char *)this + 0x30) = 1.0f;
    *(int *)((char *)this + 0x48) = 0;
    *(float *)((char *)this + 0x38) = 0.0f;
    *(float *)((char *)this + 0x3C) = 1.0f;
    *(float *)((char *)this + 0x40) = 0.0f;
    *(float *)((char *)this + 0x44) = 1.0f;
    *(int *)((char *)this + 0x5C) = 0;
    *(float *)((char *)this + 0x4C) = 0.0f;
    *(float *)((char *)this + 0x50) = 1.0f;
    *(float *)((char *)this + 0x54) = 0.0f;
    *(float *)((char *)this + 0x58) = 1.0f;
    *(int *)((char *)this + 0x70) = 0;
    *(float *)((char *)this + 0x60) = 0.0f;
    *(float *)((char *)this + 0x64) = 1.0f;
    *(float *)((char *)this + 0x68) = 0.0f;
    *(float *)((char *)this + 0x6C) = 1.0f;
}

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

void eTexCoordGen::Write(cFile &file) const {
    int i;
    int offset;
    char *entry;

    cWriteBlock wb(file, 2);
    wb.Write(*(const char *)((const char *)this + 8));
    wb.Write(*(const unsigned char *)((const char *)this + 9));
    wb.Write(*(const unsigned char *)((const char *)this + 0xA));
    wb.Write(*(const unsigned char *)((const char *)this + 0xB));
    wb.Write(*(const unsigned char *)((const char *)this + 0xC));
    wb.Write(*(const unsigned char *)((const char *)this + 0xD));
    __asm__ volatile("" : "+r"(offset));
    i = 0;
    entry = (char *)this + 0x10;
    offset = 0;
    entry = entry + offset;
    do {
        wb.Write(((const eTexCoordItem *)entry)->w4);
        wb.Write(4, (const float *)entry);
        i += 1;
        entry += 0x14;
    } while (i < 5);
    wb.End();
}
