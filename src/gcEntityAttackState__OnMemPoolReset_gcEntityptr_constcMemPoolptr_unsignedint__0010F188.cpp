// gcEntityAttackState::OnMemPoolReset(gcEntity *, const cMemPool *, unsigned int)  @ 0x0010f188
// Symbol: __0fTgcEntityAttackStateOOnMemPoolResetP6IgcEntityPC6IcMemPoolUi

class cMemPool;
class cBase;
class gcEntity;

extern "C" void *memset(void *, int, unsigned int);

class cObject {
public:
    static bool WillBeDeleted(const cBase *, const cMemPool *, unsigned int);
};

class gcEntityAttackState {
public:
    void OnMemPoolReset(gcEntity *ent, const cMemPool *pool, unsigned int id);
};

struct gcEntityAttackStateLayout {
    cBase *m_obj0;   // +0
    cBase *m_obj4;   // +4
    cBase *m_obj8;   // +8
    cBase *m_objC;   // +0xC
    int    m_x10;    // +0x10
    char   m_pad14[0x10]; // +0x14
};

void gcEntityAttackState::OnMemPoolReset(gcEntity *ent, const cMemPool *pool, unsigned int id) {
    gcEntityAttackStateLayout *self = reinterpret_cast<gcEntityAttackStateLayout *>(this);
    if (cObject::WillBeDeleted(self->m_obj0, pool, id) ||
        cObject::WillBeDeleted(self->m_obj4, pool, id) ||
        cObject::WillBeDeleted(self->m_obj8, pool, id) ||
        cObject::WillBeDeleted(self->m_objC, pool, id)) {
        self->m_obj0 = 0;
        self->m_obj4 = 0;
        memset(&self->m_obj8, 0, 8);
        self->m_x10 = 0;
        memset(self->m_pad14, 0, 0x10);
    }
}
