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

class ePhysicsMotorConfig {
public:
    char _pad[0x38];
    int startOffset;
    unsigned char enabled;
};

class eSimulatedController {
public:
    char _pad[0x38];
    char *base;
    void ApplyTorque(int, const struct mVec3 &);
};

typedef unsigned int SceULong128 __attribute__((mode(TI)));
typedef int v4sf_t __attribute__((mode(V4SF)));

struct mVec3 {
    SceULong128 qw;
};

struct eSimpleMotorVec {
    v4sf_t v;
};

struct eSimpleMotorConfigLocal {
    char _pad0[0x20];
    eSimpleMotorVec axis;
    float forceScale;
    float maxTorque;
    int bodyIndex;
    char _pad3C;
    unsigned char positioned;
};

struct eSimpleMotorStateLocal {
    char _pad0[0x20];
    void *bodyData;
};

struct eSimpleMotorVTableEntry {
    short adjust;
    short _pad;
    void (*fn)(void *, int, const mVec3 &, const mVec3 &);
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
    void Initialize(ePhysicsMotorConfig *, eSimulatedController *);
    int Apply(float);
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

// ── eSimpleMotor::Initialize(ePhysicsMotorConfig *, eSimulatedController *) @ 0x0006ba30 ──
void eSimpleMotor::Initialize(ePhysicsMotorConfig *cfg, eSimulatedController *ctrl) {
    *(eSimulatedController **)((char *)this + 0x18) = ctrl;
    *(ePhysicsMotorConfig **)((char *)this + 8) = cfg;
    char *base = ((volatile eSimulatedController *)ctrl)->base;
    int startOffset = cfg->startOffset;
    *(char **)((char *)this + 0x1C) = base + startOffset * 48;
    *(unsigned char *)((char *)this + 0x14) = cfg->enabled;
}

// ── eSimpleMotor::Apply(float) @ 0x0006b8f4 ──
int eSimpleMotor::Apply(float dt) {
    volatile mVec3 vecs[6];
    unsigned char enabled = *(unsigned char *)((char *)this + 0x14);
    int result = 1;

    if (enabled != 0) {
        eSimpleMotorConfigLocal *cfg =
            *(eSimpleMotorConfigLocal **)((char *)this + 8);
        unsigned char positioned = cfg->positioned;
        float forceScale = cfg->forceScale;
        __asm__ volatile("lv.q C120, 0x20(%0)" : : "r"(cfg) : "memory");
        eSimpleMotorStateLocal *state =
            *(eSimpleMotorStateLocal **)((char *)this + 0x1C);
        eSimulatedController *ctrl =
            *(eSimulatedController **)((char *)this + 0x18);

        if (positioned == 0) {
            float denom = (forceScale * 2.0f) * dt;
            float maxTorque = cfg->maxTorque;
            float scale = maxTorque / denom;

            __asm__ volatile(
                "mfc1 $7, %0\n"
                "mtv $7, S100\n"
                "vscl.t C130, C120, S100\n"
                "sv.q C130, 0x10($sp)\n"
                :
                : "f"(maxTorque)
                : "$7", "memory");
            void *basis = (char *)state->bodyData + 0x50;
            __asm__ volatile(
                "lv.q C200, 0x0(%0)\n"
                "vdot.t S100, C120, C200\n"
                "mfv $7, S100\n"
                "mtc1 $7, $f13\n"
                "mfc1 $7, $f13\n"
                "mtv $7, S100\n"
                "lv.q C200, 0x0(%0)\n"
                "vscl.t C120, C120, S100\n"
                "vadd.t C120, C200, C120\n"
                "sv.q C120, 0x30($sp)\n"
                :
                : "r"(basis)
                : "$7", "memory");
            __asm__ volatile(
                "mfc1 $5, %0\n"
                "mtv $5, S100\n"
                "vscl.t C120, C120, S100\n"
                "sv.q C120, 0x20($sp)\n"
                "vsub.t C120, C130, C120\n"
                "sv.q C120, 0x0($sp)\n"
                :
                : "f"(scale)
                : "$5", "memory");
            ctrl->ApplyTorque(cfg->bodyIndex, (const mVec3 &)vecs[0]);
        } else {
            __asm__ volatile("mul.s %0, %1, %0" : "+f"(dt) : "f"(forceScale));
            float scale = dt;

            __asm__ volatile(
                "mfc1 $7, %0\n"
                "mtv $7, S100\n"
                "vscl.t C120, C120, S100\n"
                "sv.q C120, 0x50($sp)\n"
                :
                : "f"(scale)
                : "$7", "memory");
            void *basis = (char *)state->bodyData + 0x50;
            __asm__ volatile(
                "lv.q C130, 0x0(%0)\n"
                "vsub.t C120, C120, C130\n"
                "sv.q C120, 0x50($sp)\n"
                :
                : "r"(basis)
                : "memory");

            eSimpleMotorVTableEntry *entry =
                (eSimpleMotorVTableEntry *)(*(char **)((char *)ctrl + 4) + 0xC0);
            void *self = (char *)ctrl + entry->adjust;
            int bodyIndex = cfg->bodyIndex;
            float zero = 0.0f;
            __asm__ volatile(
                "mfc1 $6, %0\n"
                "mfc1 $7, %0\n"
                "mfc1 $9, %0\n"
                "mtv $6, S120\n"
                "mtv $7, S121\n"
                "mtv $9, S122\n"
                "sv.q C120, 0x40($sp)\n"
                :
                : "f"(zero)
                : "$6", "$7", "$9", "memory");
            const mVec3 *zeroVec = (const mVec3 *)&vecs[4];
            const mVec3 *deltaVec = (const mVec3 *)&vecs[5];
            __asm__ volatile("" : "+r"(zeroVec), "+r"(deltaVec));
            entry->fn(self, bodyIndex, *zeroVec, *deltaVec);
        }
    }
    return result;
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
