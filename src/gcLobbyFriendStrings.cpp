// gcLobbyFriendStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcLobbyFriendStrings
//   Total size 0x10. Layout:
//     0x00: cBase (mOwner, mClassDesc)
//     0x08: gcDesiredValue (tagged self-pointer)
//     0x0C: int mField

class cBase {
public:
    int _b0;
    int _b4;
};

class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char gcLobbyFriendStringsvirtualtable[];

struct DtorDeleteRecord {
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

class gcLobbyFriendStrings : public gcStringValue {
public:
    int _b8;   // 0x08 - gcDesiredValue tagged self-ptr
    int mField; // 0x0C

    ~gcLobbyFriendStrings();
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static gcLobbyFriendStrings *New(cMemPool *, cBase *);

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
extern cType *D_0009F4F0;

// ── gcLobbyFriendStrings::GetType(void) const  @ 0x00280b3c, 220B ──
const cType *gcLobbyFriendStrings::GetType(void) const {
    if (D_0009F4F0 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4F0 = cType::InitializeType(
            0, 0, 0x1C4, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyFriendStrings::New, 0, 0, 0);
    }
    return D_0009F4F0;
}

// ── gcLobbyFriendStrings::Write(cFile &) const  @ 0x00280c18, 100B ──
void gcLobbyFriendStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.Write(this->mField);
    wb.End();
}

// ── gcLobbyFriendStrings::New(cMemPool *, cBase *) static  @ 0x00280aac, 144B ──
gcLobbyFriendStrings *gcLobbyFriendStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLobbyFriendStrings *result = 0;
    int *p = (int *)entry->fn(base, 0x10, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4) = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4) = (int)gcLobbyFriendStringsvirtualtable;
        *(int *)((char *)p + 8) = (int)((unsigned int)p | 1);
        *(int *)((char *)p + 12) = 0;
        result = (gcLobbyFriendStrings *)p;
    }
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oUgcLobbyFriendStringsdtv, 0xd4\n");

// ── gcLobbyFriendStrings::~gcLobbyFriendStrings(void)  @ 0x002811d8, 212B ──
gcLobbyFriendStrings::~gcLobbyFriendStrings() {
    *(char **)((char *)this + 4) = gcLobbyFriendStringsvirtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
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
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(int *)((char *)this + 4) = 0x37E6A8;
}
