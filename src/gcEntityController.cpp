// gcEntityController — select methods from gcAll_psp.obj.
// Other members live elsewhere (PostPhysicsMove etc. are in gcGame.cpp).

class cBase;
class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class gcEntity;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern "C" void __vec_delete(void *, int, int, void (*)(void *), int, int);
extern "C" void gcStateInfo__gcStateInfo_void(void *);
extern "C" void gcReplicationGroup__gcReplicationGroup_void(void *);

class cTimeValue {
public:
    int mTime;
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class cObject {
public:
    static int WillBeDeleted(const cBase *, const cMemPool *, unsigned int);
};

class eDynamicModel {
public:
    int SetPhysicsController(const cType *);
    void *GetAnimationState(void) const;
};

class gcEntityAttackState {
public:
    void *m_entity;
    bool IsActive(void) const { return m_entity != 0; }
    void OnMemPoolReset(gcEntity *, const cMemPool *, unsigned int);
    void QueryCollisions(gcEntity *, cTimeValue);
};

class gcEventParams {
public:
    char _data[0x58];
};

class gcEventStackData {
public:
    char _data[0x1C];
};

class gcEntity {
public:
    enum gcDesiredStateMachine {
        DESIRED_STATE_MACHINE_2 = 2
    };
};

extern "C" void gcEntity_Send_SystemMessage(
    gcEntity *, bool, int, int, const gcEventStackData &, const gcEventParams &)
    __asm__("__0fIgcEntityESendb65IgcEntityi__5IgcEntityVgcDesiredStateMachine65PgcStateMessagesj__5PgcStateMessagesPgcSystemMessageRC6NgcEventParamsRC6QgcEventStackData");

struct gcEntityControllerEntData {
    char pad0[0x65];
    signed char idx;
    char pad66[0x1E0 - 0x66];
    void **arr;
};

struct gcEntityControllerNavData {
    char pad0[0x4C];
    int flags;
    char pad50[0xA4 - 0x50];
    void *path;
    char padA8[0xC0 - 0xA8];
    void *follow;
    char padC4[0xE0 - 0xC4];
    unsigned char current;
};

struct gcEntityControllerNavDataPlain {
    char pad0[0x4C];
    int flags;
};

struct gcEntityControllerEntFull {
    char pad0[0x65];
    signed char idx;
    char pad66[0x1E0 - 0x66];
    void **arr;
    char pad1E4[0x1F8 - 0x1E4];
    gcEntityControllerNavData *nav;
};

struct EntityActivatedScratch {
    int words0[4];
    int zero10;
    int zero14;
    unsigned char flag18;
    char pad19[3];
    int word1C;
    float floats20[10];
    int word48;
    int word4C;
    int word50;
    int word54;
    int word58;
    int word5C;
    int word60;
    int word64;
    int word68;
    int word6C;
    int word70;
    int vec74[4];
};

class gcEntityController {
public:
    gcEntityControllerEntData *m_ent;   // offset 0
    char pad4[0x24 - 4];
    unsigned char m_flags;              // offset 0x24
    char pad25[0x70 - 0x25];
    void *m_u70;                        // offset 0x70
    void *m_u74;                        // offset 0x74
    char pad78[0x7C - 0x78];
    gcEntityAttackState *m_attack;      // offset 0x7C

    bool IsCurrentController(void) const;
    gcEntityController(cBase *);
    ~gcEntityController();
    int Read(cFile &, cMemPool *);
    void CancelFollowPath(void);
    void ClearCurrentNavMeshPosition(void);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void SetPhysicsController(const cType *);
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void ValidateNavMeshPos(void);
    void OnActivated(void);
    void PostUpdateFinal(void);

    static void operator delete(void *p);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void gcEntityController::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
    short off = rec->offset;
    void (*fn)(void *, void *) = rec->fn;
    fn(block + off, p);
}

// ── IsCurrentController (0x00110cd4) ──

bool gcEntityController::IsCurrentController(void) const {
    gcEntityControllerEntData *ent = m_ent;
    int entry = 0;
    int idx = ent->idx;
    if (idx >= 0) {
        int count = 0;
        void **arr = ent->arr;
        if (arr != 0) {
            count = ((int *)arr)[-1];
        }
        if (idx < count) {
            entry = (int)arr[idx];
        }
    }
    return entry == (int)this;
}

// ── Write (0x001107b8) ──

void gcEntityController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cNamed *)this)->cNamed::Write(file);
    wb.End();
}

// ── GetType (0x0025d698) ──

const cType *gcEntityController::GetType(void) const {
    if (D_0009A404 == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                               &cNamed::New, 0, 0, 0);
        }
        D_0009A404 = cType::InitializeType(0, 0, 0x99, D_000385E0,
                                           0, 0, 0, 0);
    }
    return D_0009A404;
}

// ── Read (0x00110804) ──

int gcEntityController::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || ((cNamed *)this)->cNamed::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    return result;
}

// ── gcEntityController (0x001108c0) ──

gcEntityController::gcEntityController(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(unsigned short *)((char *)this + 0x1C) = 0;
    *(unsigned short *)((char *)this + 0x1E) = 0;
    *(unsigned char *)((char *)this + 8) = 0;
    *(void **)((char *)this + 4) = (void *)0x3887B0;
    *(void **)((char *)this + 0x20) = 0;
    *(unsigned char *)((char *)this + 0x24) = 0;
    *(unsigned char *)((char *)this + 0x25) = 0;
    __vec_new((char *)this + 0x28, 2, 0x24, gcStateInfo__gcStateInfo_void);
    *(void **)((char *)this + 0x70) = 0;
    *(void **)((char *)this + 0x74) = 0;
    *(void **)((char *)this + 0x78) = 0;
    *(void **)((char *)this + 0x7C) = 0;
    __vec_new((char *)this + 0x80, 1, 6, gcReplicationGroup__gcReplicationGroup_void);
    *(void **)((char *)this + 0x8C) = 0;
    *(float *)((char *)this + 0x88) = 0.0f;
}

// ── ~gcEntityController (0x00110958) ──

gcEntityController::~gcEntityController() {
    *(void **)((char *)this + 4) = (void *)0x3887B0;
    if (*(int *)((char *)this + 0x7C) != 0) {
        int attack = *(int *)((char *)this + 0x7C);
        cMemPool *pool = cMemPool::GetPoolFromPtr((void *)attack);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, (void *)attack);
        *(void **)((char *)this + 0x7C) = 0;
    }
    __vec_delete((char *)this + 0x28, 2, 0x24, (void (*)(void *))0x2597C0, 0, 0);
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// ── SetPhysicsController (0x00113a58) ──

void gcEntityController::SetPhysicsController(const cType *type) {
    eDynamicModel *m = *(eDynamicModel **)this;
    if (m != 0) {
        *((unsigned char *)this + 0x24) &= ~1;
        if (*(void **)((char *)m + 0xE0) != 0 &&
            ((eDynamicModel *)((char *)m + 0x80))->SetPhysicsController(type) != 0 &&
            type != 0) {
            *((unsigned char *)this + 0x24) |= 1;
        }
    }
}

// ── CancelFollowPath (0x001113ec) ──

void gcEntityController::CancelFollowPath(void) {
    gcEntityControllerEntFull *ent = *(gcEntityControllerEntFull **)this;
    gcEntityControllerNavData *nav = ent->nav;
    int flags = nav->flags;
    flags &= ~1;
    nav->flags = flags;
    flags = nav->flags & ~0x10;
    nav->flags = flags;
    nav->path = 0;
    if (flags & 4) {
        int count = 0xFF;
        flags = nav->flags;
        nav->follow = 0;
        flags |= 4;
        __asm__ volatile("" ::: "memory");
        nav->current = count;
        register int mask __asm__("$7") = ~8;
        nav->flags = flags;
        flags = nav->flags & mask;
        nav->flags = flags;
        void *current = 0;
        int idx = ent->idx;
        if (idx >= 0) {
            count = 0;
            void **arr = ent->arr;
            if (arr != 0) {
                count = ((int *)arr)[-1];
            }
            if (idx < count) {
                current = arr[idx];
            }
        }
        ((gcEntityController *)current)->ValidateNavMeshPos();
    }
}

// ── ClearCurrentNavMeshPosition (0x001114b0) ──

void gcEntityController::ClearCurrentNavMeshPosition(void) {
    gcEntityControllerNavDataPlain *nav =
        (gcEntityControllerNavDataPlain *)(*(gcEntityControllerEntFull **)this)->nav;
    int flags = nav->flags;
    nav->flags = flags & ~8;
}

// ── OnMemPoolReset (0x00110c58) ──

void gcEntityController::OnMemPoolReset(const cMemPool *pool, unsigned int flags) {
    if (cObject::WillBeDeleted(*(cBase **)((char *)this + 0x70), pool, flags)) {
        *(void **)((char *)this + 0x70) = 0;
    }
    int will_delete = cObject::WillBeDeleted(
        *(cBase **)((char *)this + 0x74), pool, flags);
    gcEntityAttackState *attack = *(gcEntityAttackState **)((char *)this + 0x7C);
    if (will_delete) {
        *(void **)((char *)this + 0x74) = 0;
    }
    if (attack != 0) {
        attack->OnMemPoolReset(*(gcEntity **)this, pool, flags);
    }
}

// ── OnActivated (0x00111904) ──

void gcEntityController::OnActivated(void) {
    gcEntity *entity = *(gcEntity **)this;
    EntityActivatedScratch scratch;

    scratch.zero10 = 0;
    scratch.zero14 = 0;
    scratch.flag18 = 1;
    scratch.words0[0] = 0;
    scratch.words0[1] = 0;
    scratch.words0[2] = 0;
    scratch.words0[3] = 0;
    scratch.word1C = 0;
    scratch.floats20[0] = 0.0f;
    scratch.word58 = 0;
    __vec_new(scratch.vec74, 2, 8, (void (*)(void *))0x2275F0);
    scratch.floats20[1] = 0.0f;
    scratch.floats20[2] = 0.0f;
    scratch.floats20[3] = 0.0f;
    scratch.floats20[4] = 0.0f;
    scratch.floats20[5] = 0.0f;
    scratch.floats20[6] = 0.0f;
    scratch.floats20[7] = 0.0f;
    scratch.floats20[8] = 0.0f;
    scratch.floats20[9] = 0.0f;
    scratch.word5C = 0;
    scratch.word60 = 0;
    scratch.word64 = 0;
    scratch.word68 = 0;
    scratch.word6C = 0;
    scratch.word70 = 0;
    gcEntity_Send_SystemMessage(entity, true, gcEntity::DESIRED_STATE_MACHINE_2, 10,
                                *(gcEventStackData *)&scratch.words0[0],
                                *(gcEventParams *)&scratch.word1C);
}

// ── PostUpdateFinal (0x001110fc) ──

void gcEntityController::PostUpdateFinal(void) {
    gcEntityAttackState *attack = *(gcEntityAttackState **)((char *)this + 0x7C);
    if (attack == 0) return;
    if (!attack->IsActive()) return;
    void *anim = ((eDynamicModel *)((char *)*(void **)this + 0x80))->GetAnimationState();
    if (anim == 0) return;

    int raw = *(int *)((char *)anim + 12);
    gcEntityAttackState *attack2 = *(gcEntityAttackState **)((char *)this + 0x7C);
    gcEntity *entity2 = *(gcEntity **)this;
    cTimeValue t;
    t.mTime = ((raw & 0x7FFFFFFF) ^ 0x40000000) - 0x40000000;
    attack2->QueryCollisions(entity2, t);
}
