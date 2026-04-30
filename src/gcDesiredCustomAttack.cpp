// gcDesiredCustomAttack - gcAll_psp.obj
//
// Functions matched here:
//   gcDesiredCustomAttack::Write(cFile &) const                 @ 0x00125938
//   gcDesiredCustomAttack::New(cMemPool *, cBase *) static      @ 0x0026709c
//   gcDesiredCustomAttack::GetType(void) const                  @ 0x0026713c

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern char D_00000478[];
extern char cBaseclassdesc[];
extern char gcDesiredObject_dtor_classdesc[];

extern cType *D_000385DC;
extern cType *D_0009F3F4;
extern cType *D_0009F438;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct ReleaseEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class gcDesiredEnumeration {
public:
    ~gcDesiredEnumeration();
};

class gcDesiredCustomAttack {
public:
    ~gcDesiredCustomAttack();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

inline void operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DeleteEntry *entry = (DeleteEntry *)(((char **)block)[7] + 0x30);
    short off = entry->offset;
    void (*fn)(void *, void *) = entry->fn;
    fn(block + off, p);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oUgcDesiredEnumerationdtv, 0xd4\n");

// 0x0026a01c - gcDesiredEnumeration::~gcDesiredEnumeration(void)
gcDesiredEnumeration::~gcDesiredEnumeration() {
    *(void **)((char *)this + 4) = gcDesiredObject_dtor_classdesc;
    if ((char *)this + 8 != 0) {
        int keep = 1;
        int value = *(int *)((char *)this + 8);
        if ((value & 1) != 0) {
            keep = 0;
        }
        if (keep && value) {
            void *desc = *(void **)((char *)value + 4);
            ReleaseEntry *entry = (ReleaseEntry *)((char *)desc + 0x50);
            short off = entry->offset;
            void (*fn)(void *, int) = entry->fn;
            fn((char *)value + off, 3);
            *(int *)((char *)this + 8) = 0;
        }
    }
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oVgcDesiredCustomAttackdtv, 0xd4\n");

// 0x00267218 - gcDesiredCustomAttack::~gcDesiredCustomAttack(void)
gcDesiredCustomAttack::~gcDesiredCustomAttack() {
    *(void **)((char *)this + 4) = gcDesiredObject_dtor_classdesc;
    if ((char *)this + 8 != 0) {
        int keep = 1;
        int value = *(int *)((char *)this + 8);
        if ((value & 1) != 0) {
            keep = 0;
        }
        if (keep && value) {
            void *desc = *(void **)((char *)value + 4);
            ReleaseEntry *entry = (ReleaseEntry *)((char *)desc + 0x50);
            short off = entry->offset;
            void (*fn)(void *, int) = entry->fn;
            fn((char *)value + off, 3);
            *(int *)((char *)this + 8) = 0;
        }
    }
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// 0x00125938 - gcDesiredCustomAttack::Write(cFile &) const
void gcDesiredCustomAttack::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcDesiredObject *)this)->Write(file);

    cWriteBlock wb2(**(cFile **)&wb._data[0], 3);
    wb2.Write(*(int *)((const char *)this + 12));
    ((const cHandle *)((const char *)this + 16))->Write(wb2);
    wb2.End();
    wb.End();
}

// 0x0026709c - gcDesiredCustomAttack::New(cMemPool *, cBase *) static
cBase *gcDesiredCustomAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDesiredCustomAttack *result = 0;
    gcDesiredCustomAttack *obj =
        (gcDesiredCustomAttack *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        gcDesiredObject_gcDesiredObject(obj, parent);
        ((void **)obj)[1] = D_00000478;
        *(int *)((char *)obj + 12) = 7;
        *(int *)((char *)obj + 16) = 0;
        ((void **)obj)[1] = (void *)0x389270;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0026713c - gcDesiredCustomAttack::GetType(void) const
const cType *gcDesiredCustomAttack::GetType(void) const {
    if (D_0009F438 == 0) {
        if (D_0009F3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F438 = cType::InitializeType(0, 0, 0x1CF, D_0009F3F4,
                                           &gcDesiredCustomAttack::New,
                                           0, 0, 0);
    }
    return D_0009F438;
}
