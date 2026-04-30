// gcConfig — core game configuration block.
//
// Functions matched here:
//   gcConfig::~gcConfig(void)                       @ 0x000f0050
//   gcConfig::New(cMemPool *, cBase *) static       @ 0x00246534
//   gcConfig::GetType(void) const                   @ 0x002465b0

class cBase;
class cFile;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

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
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcConfig {
public:
    cBase *mParent;
    void *mClassdesc;
    unsigned int mField8;
    unsigned int mFieldC;
    unsigned int mField10;
    unsigned int mField14;
    unsigned int mField18;
    unsigned int mField1C;
    unsigned int mField20;
    unsigned int mField24;
    unsigned int mField28;
    unsigned int mField2C;
    unsigned int mField30;
    unsigned int mField34;
    unsigned int mField38;
    unsigned int mField3C;
    unsigned int mField40;
    unsigned int mField44;
    unsigned int mField48;
    unsigned int mField4C;
    unsigned int mField50;
    unsigned int mField54;
    unsigned int mField58;
    unsigned int mField5C;
    unsigned int mField60;
    unsigned int mField64;
    unsigned int mField68;
    int mField6C;
    int mField70;
    int mField74;

    gcConfig(cBase *);
    ~gcConfig();

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void CalcPresetSizes(void);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcConfigvirtualtable[];
extern char cBaseclassdesc[];

extern cType *D_000385DC;
extern cType *D_00099AF0;
extern void *gcConfig__s_pInstance__0037D84C;

extern "C" {
    void gcConfig__gcConfig_cBaseptr(void *self, cBase *parent);
}

// ── gcConfig::Write(cFile &) const @ 0x000efd00 ──
void gcConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.Write(mField8);
    wb.Write(mFieldC);
    wb.Write(mField10);
    wb.Write(mField18);
    wb.Write(mField1C);
    wb.Write(mField20);
    wb.Write(mField28);
    wb.Write(mField3C);
    wb.Write(mField50);
    wb.Write(mField64);
    wb.Write(mField68);
    wb.Write(mField6C);
    wb.Write(mField70);
    wb.Write(mField74);
    wb.End();
}

// ── gcConfig::gcConfig(cBase *) @ 0x000effa8 ──
gcConfig::gcConfig(cBase *parent) {
    mParent = parent;
    mClassdesc = gcConfigvirtualtable;
    mField8 = 0x300000;
    mFieldC = 0x200000;
    mField10 = 0x64;
    mField18 = 0x60000;
    mField1C = 0x60000;
    mField20 = 0x1E;
    mField28 = 0x400000;
    mField3C = 0xC00000;
    mField50 = 0x64;
    mField64 = 0x60000;
    mField68 = 0x60000;
    mField6C = 0x4000;
    mField70 = 0x18000;
    mField74 = 0x1000;
    *(gcConfig **)0x37D7DC = this;
    CalcPresetSizes();
}

// ── gcConfig::New(cMemPool *, cBase *) static @ 0x00246534 ──
cBase *gcConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcConfig *result = 0;
    gcConfig *obj = (gcConfig *)rec->fn(base, 0x78, 4, 0, 0);
    if (obj != 0) {
        gcConfig__gcConfig_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcConfig::GetType(void) const @ 0x002465b0 ──
const cType *gcConfig::GetType(void) const {
    if (D_00099AF0 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AF0 = cType::InitializeType(0, 0, 0x28D, D_000385DC,
                                           &gcConfig::New, 0, 0, 0);
    }
    return D_00099AF0;
}

// ── gcConfig::~gcConfig(void) @ 0x000f0050 ──
gcConfig::~gcConfig() {
    *(void **)((char *)this + 4) = gcConfigvirtualtable;
    gcConfig__s_pInstance__0037D84C = 0;
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
