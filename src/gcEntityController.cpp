// gcEntityController — select methods from gcAll_psp.obj.
// Other members live elsewhere (PostPhysicsMove etc. are in gcGame.cpp).

class cBase;
class cFile;
class cMemPool;
class cType;
class gcEntity;

class cTimeValue {
public:
    int mTime;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cNamed {
public:
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

struct gcEntityControllerEntData {
    char pad0[0x65];
    signed char idx;
    char pad66[0x1E0 - 0x66];
    void **arr;
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
    void Write(cFile &) const;
    void SetPhysicsController(const cType *);
    void OnMemPoolReset(const cMemPool *, unsigned int);
    void PostUpdateFinal(void);
};

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
