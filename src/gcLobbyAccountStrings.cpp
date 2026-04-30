// gcLobbyAccountStrings — string-table value with extra fixed flag.
//
// Functions matched here:
//   gcLobbyAccountStrings::Write(cFile &) const            @ 0x0027fcc4  (gcAll_psp.obj)
//   gcLobbyAccountStrings::New(cMemPool *, cBase *) static @ 0x0027fb50  (gcAll_psp.obj)
//   gcLobbyAccountStrings::~gcLobbyAccountStrings(void)    @ 0x002802e0  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char cBaseclassdesc[];
extern char gcLobbyAccountStringsclassdesc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
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

class gcStringValue : public cBase {
public:
    gcStringValue(cBase *parent) : cBase(parent) {}
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcLobbyAccountStrings : public gcStringValue {
public:
    int mField08;       // 0x08 — constant 1
    int mField0C;       // 0x0C — gcDesiredValue first slot, holds (this | 1)
    int mField10;       // 0x10

    gcLobbyAccountStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcLobbyAccountStringsclassdesc;
        mField08 = 1;
        mField0C = (int)this | 1;
        mField10 = 0;
    }
    ~gcLobbyAccountStrings();
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static gcLobbyAccountStrings *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4E8;

// ── GetType ──  @ 0x0027fbe8, 220B
const cType *gcLobbyAccountStrings::GetType(void) const {
    if (D_0009F4E8 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4E8 = cType::InitializeType(
            0, 0, 0xFF, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyAccountStrings::New, 0, 0, 0);
    }
    return D_0009F4E8;
}

// ── Write ──  @ 0x0027fcc4, 112B
void gcLobbyAccountStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringValue::Write(file);
    wb.Write(mField08);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.Write(mField10);
    wb.End();
}

// ── New ──  @ 0x0027fb50, 152B
gcLobbyAccountStrings *gcLobbyAccountStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyAccountStrings *result = 0;
    gcLobbyAccountStrings *obj = (gcLobbyAccountStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyAccountStrings(parent);
        result = obj;
    }
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oVgcLobbyAccountStringsdtv, 0xd4\n");

// 0x002802e0 - gcLobbyAccountStrings::~gcLobbyAccountStrings(void)
gcLobbyAccountStrings::~gcLobbyAccountStrings() {
    *(char **)((char *)this + 4) = gcLobbyAccountStringsclassdesc;
    char *slot = (char *)this + 0x0C;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}
