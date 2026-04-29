class cBase;
class cFile;
class cType;
class cMemPool;
class gcEnumeration;
class gcEnumerationGroup;

inline void *operator new(unsigned int, void *p) { return p; }

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class cName {
public:
    void Set(const char *, ...);
};

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

void cFile_SetCurrentPos(void *, unsigned int);

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cFactory {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cMemPool {
public:
    char _pad0[0x24];
    void *mBlock;
    void *mOwner;

    static cMemPool *GetPoolFromPtr(const void *);
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

struct cGUID {
    int a;
    int b;
};

struct DispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class gcGameGlobals : public cFactory {
public:
    char _pad0[0x54];
    int mGroups[1];

    gcGameGlobals(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

template <class T> T *dcast(const cBase *);

struct WordCopy {
    int value;
};

// ── gcGameGlobals::AssignCopy(const cBase *) @ 0x0024dbd4 ──
void gcGameGlobals::AssignCopy(const cBase *base) {
    gcGameGlobals *other = dcast<gcGameGlobals>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(int *)((char *)this + 0x44) = *(const int *)((char *)other + 0x44);
    *(int *)((char *)this + 0x48) = *(const int *)((char *)other + 0x48);
    *(WordCopy *)((char *)this + 0x4C) =
        *(const WordCopy *)((char *)other + 0x4C);
    *(WordCopy *)((char *)this + 0x50) =
        *(const WordCopy *)((char *)other + 0x50);
    int i = 0;
    gcGameGlobals *src = other;
    gcGameGlobals *dst = this;
    do {
        ++i;
        dst->mGroups[0] = src->mGroups[0];
        src = (gcGameGlobals *)((char *)src + 4);
        dst = (gcGameGlobals *)((char *)dst + 4);
    } while (i <= 0);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C90;
extern cType *D_0009A2FC;
extern "C" int gcEnumerationGroup__IsManagedTypeExternalStatic_voidstatic(void);

extern "C" cHandleT<gcEnumeration>
gcGameGlobals_CreateSystemMessageEnumeration(
    gcGameGlobals *, int, const char *, const cGUID &)
    __asm__("cHandleT_gcEnumeration_gcGameGlobals__CreateEnumeration_gcStateMessages__gcSystemMessage__gcGameGlobals_constcharptr_constcGUIDref__0024E424");

extern "C" cHandleT<gcEnumeration>
gcGameGlobals_CreateConnectionErrorEnumeration(
    gcGameGlobals *, int, const char *, const cGUID &)
    __asm__("cHandleT_gcEnumeration_gcGameGlobals__CreateEnumeration_nwConnection__nwConnectionError__gcGameGlobals_constcharptr_constcGUIDref__0024E7C0");

// ── gcGameGlobals::Write(cFile &) const @ 0x00105a30 ──
void gcGameGlobals::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cFactory *)this)->Write(file);
    wb.End();
}

// ── gcGameGlobals::Read(cFile &, cMemPool *) @ 0x00105a7c ──
int gcGameGlobals::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cFactory::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcGameGlobals::gcGameGlobals(cBase *) @ 0x00105b38 ──
gcGameGlobals::gcGameGlobals(cBase *parent) {
    int one = 1;
    cHandleT<gcEnumeration> *systemMessages =
        (cHandleT<gcEnumeration> *)((char *)this + 0x4C);
    cHandleT<gcEnumeration> *connectionErrors =
        (cHandleT<gcEnumeration> *)((char *)this + 0x50);
    volatile cGUID guid;
    gcEnumerationGroup *group;
    gcEnumerationGroup *selected;

    new ((cObject *)this) cObject(parent);
    *(int *)((char *)this + 0x44) = one;
    *(int *)((char *)this + 0x48) = 0x800;
    *(void **)((char *)this + 4) = (void *)0x388080;
    systemMessages->mIndex = 0;
    connectionErrors->mIndex = 0;
    *(gcGameGlobals **)0x37D850 = this;
    guid.a = one;
    guid.b = one;
    ((cName *)((char *)this + 8))->Set((const char *)0x36DC2C);

    DispatchEntry *dispatch =
        (DispatchEntry *)(*(char **)((char *)this + 4) + 0x70);
    dispatch->fn((char *)this + dispatch->offset);

    *(int *)((char *)this + 0x20) = one;
    *(int *)((char *)this + 0x24) = one;
    *(int *)((char *)this + 0x44) &= ~1;

    cMemPool *pool = cMemPool::GetPoolFromPtr(this);
    pool->mOwner = this;
    void *block = pool->mBlock;
    AllocRec *rec =
        (AllocRec *)(*(char **)((char *)block + 0x1C) + 0x28);
    group = (gcEnumerationGroup *)rec->fn(
        (char *)block + rec->offset, 0x10, 4, 0, 0);
    selected = 0;
    if (group != 0) {
        int external = 0;
        if (gcEnumerationGroup__IsManagedTypeExternalStatic_voidstatic() == 0) {
            external = 1;
        }
        *(void **)((char *)group + 4) = (void *)0x37E6A8;
        *(gcGameGlobals **)group = this;
        *(void **)((char *)group + 4) = (void *)0x37EA80;
        external = (unsigned char)(external & 0xff);
        *(unsigned char *)((char *)group + 8) =
            (unsigned char)(external & 0xff);
        *(int *)((char *)group + 0xC) = 0;
        *(void **)((char *)group + 4) = (void *)0x3865A0;
        selected = group;
    }
    mGroups[0] = (int)selected;
    *(unsigned char *)((char *)selected + 8) = 1;

    guid.b = 2;
    *systemMessages =
        gcGameGlobals_CreateSystemMessageEnumeration(
            this, 0x31, (const char *)0x36DC3C, (const cGUID &)guid);

    guid.b = 3;
    *connectionErrors =
        gcGameGlobals_CreateConnectionErrorEnumeration(
            this, 0xB, (const char *)0x36DC44, (const cGUID &)guid);

    *(unsigned short *)((char *)this + 0x28) =
        (unsigned short)(*(unsigned short *)((char *)this + 0x28) | 0x10);
}

// ── gcGameGlobals::GetType(void) const @ 0x0024dce4 ──
const cType *gcGameGlobals::GetType(void) const {
    if (D_0009A2FC == 0) {
        if (D_00040C90 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        &cNamed::New, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(
                    0, 0, 3, D_000385E0, 0, 0, 0, 0);
            }
            D_00040C90 = cType::InitializeType(
                0, 0, 5, D_000385E4, 0, 0, 0, 0);
        }
        D_0009A2FC = cType::InitializeType(
            0, 0, 0xC6, D_00040C90, &gcGameGlobals::New,
            (const char *)0x36D944, (const char *)0x36D944, 0);
    }
    return D_0009A2FC;
}

// ── gcGameGlobals::New(cMemPool *, cBase *) static @ 0x0024dc68 ──
cBase *gcGameGlobals::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcGameGlobals *result = 0;
    gcGameGlobals *obj = (gcGameGlobals *)rec->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcGameGlobals(parent);
        result = obj;
    }
    return (cBase *)result;
}
