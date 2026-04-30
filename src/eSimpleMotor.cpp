// eSimpleMotor — derives from eSimulatedMotor (size 0x20).
//
// Functions matched here:
//   eSimpleMotor::eSimpleMotor(cBase *)         @ 0x0006b840  56B
//   eSimpleMotor::Write(cFile &) const          @ 0x0006b738  76B
//   eSimpleMotor::Read(cFile &, cMemPool *)     @ 0x0006b784  188B
//   eSimpleMotor::~eSimpleMotor(void)           @ 0x0006b878  124B
//   eSimpleMotor::New(cMemPool *, cBase *)      @ 0x00209bd0  124B

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

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
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

struct DeleteRecord {
    short offset;
    short _pad;
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

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class eSimulatedMotor {
public:
    char _pad[0x20];
    eSimulatedMotor(cBase *);
    ~eSimulatedMotor();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern char eSimpleMotorvirtualtable[];
extern cType *D_000385DC;
extern cType *D_00046BCC;
extern cType *D_00046BD0;

class eSimpleMotor : public eSimulatedMotor {
public:
    eSimpleMotor(cBase *);
    ~eSimpleMotor();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

template <class T>
T dcast(const cBase *);

// ── eSimpleMotor::eSimpleMotor(cBase *) @ 0x0006b840 ──
eSimpleMotor::eSimpleMotor(cBase *parent) : eSimulatedMotor(parent) {
    *(void **)((char *)this + 4) = eSimpleMotorvirtualtable;
    *(int *)((char *)this + 0x1C) = 0;
}

#pragma control sched=1

// ── eSimpleMotor::Write(cFile &) const @ 0x0006b738 ──
void eSimpleMotor::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eSimulatedMotor::Write(file);
    wb.End();
}

// ── eSimpleMotor::Read(cFile &, cMemPool *) @ 0x0006B784 ──
int eSimpleMotor::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((eSimulatedMotor *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── eSimpleMotor::AssignCopy(const cBase *) @ 0x00209b78 ──
void eSimpleMotor::AssignCopy(const cBase *base) {
    eSimpleMotor *src = dcast<eSimpleMotor *>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)src + 8);
    *(int *)((char *)this + 0xC) = *(int *)((char *)src + 0xC);
    *(int *)((char *)this + 0x10) = *(int *)((char *)src + 0x10);
    *(unsigned char *)((char *)this + 0x14) =
        *(unsigned char *)((char *)src + 0x14);
    *(int *)((char *)this + 0x18) = *(int *)((char *)src + 0x18);
    *(int *)((char *)this + 0x1C) = *(int *)((char *)src + 0x1C);
}

// ── eSimpleMotor::~eSimpleMotor(void) @ 0x0006b878 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~eSimulatedMotor(this, 0), and the deleting-tail dispatch
// through operator delete (pool/block lookup + slot fn).
eSimpleMotor::~eSimpleMotor() {
    *(void **)((char *)this + 4) = eSimpleMotorvirtualtable;
}

// ── eSimpleMotor::New(cMemPool *, cBase *) static @ 0x00209bd0 ──
cBase *eSimpleMotor::New(cMemPool *pool, cBase *parent) {
    eSimpleMotor *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSimpleMotor *obj = (eSimpleMotor *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        new (obj) eSimpleMotor(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eSimpleMotor::GetType(void) const {
    if (D_00046BD0 == 0) {
        if (D_00046BCC == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_00046BCC = cType::InitializeType(
                0, 0, 0x261, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_00046BCC;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x209BD0;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BD0 = cType::InitializeType(
            0, 0, 0x262, parentType, factory, 0, 0, 0);
    }
    return D_00046BD0;
}

#pragma control sched=2

// ── unrelated free-function bag (kept from prior session) ──

struct gcExternalVariableGroup {
    char pad0[8];
    unsigned char unk8;
    char pad9[3];
    int unkC;
};

struct gcRoomInstanceGroup {
    char pad0[8];
    unsigned char unk8;
    char pad9[3];
    int unkC;
};

struct gcValExternalVariableValue {
    int unk8;
};

struct gcValExternalVariable {
    char pad0[8];
    gcValExternalVariableValue value;
};

struct gcValMemCardStatus {
    char pad0[8];
    int unk8;
    int unkC;
};

extern "C" {

gcExternalVariableGroup *dcastdcast_gcExternalVariableGroupptr__constcBaseptr__0021F6D0(const cBase *);
gcRoomInstanceGroup *dcastdcast_gcRoomInstanceGroupptr__constcBaseptr__0021FC20(const cBase *);
gcValExternalVariable *dcastdcast_gcValExternalVariableptr__constcBaseptr__0022EE5C(const cBase *);
gcValMemCardStatus *dcastdcast_gcValMemCardStatusptr__constcBaseptr__002306BC(const cBase *);

void gcExternalVariableGroup__AssignCopy_constcBaseptr__002379C4(gcExternalVariableGroup *self, const cBase *other) {
    gcExternalVariableGroup *src = dcastdcast_gcExternalVariableGroupptr__constcBaseptr__0021F6D0(other);
    self->unk8 = src->unk8;
    self->unkC = src->unkC;
}

void gcRoomInstanceGroup__AssignCopy_constcBaseptr__00238364(gcRoomInstanceGroup *self, const cBase *other) {
    gcRoomInstanceGroup *src = dcastdcast_gcRoomInstanceGroupptr__constcBaseptr__0021FC20(other);
    self->unk8 = src->unk8;
    self->unkC = src->unkC;
}

void gcValExternalVariable__AssignCopy_constcBaseptr__00342E98(gcValExternalVariable *self, const cBase *other) {
    gcValExternalVariable *src = dcastdcast_gcValExternalVariableptr__constcBaseptr__0022EE5C(other);
    self->value = src->value;
}

void gcValMemCardStatus__AssignCopy_constcBaseptr__0034FFD0(gcValMemCardStatus *self, const cBase *other) {
    gcValMemCardStatus *src = dcastdcast_gcValMemCardStatusptr__constcBaseptr__002306BC(other);
    self->unk8 = src->unk8;
    self->unkC = src->unkC;
}

}
