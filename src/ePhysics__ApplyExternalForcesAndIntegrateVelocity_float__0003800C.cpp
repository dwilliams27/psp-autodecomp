// ePhysics::ApplyExternalForcesAndIntegrateVelocity(float) @ 0x0003800c (eAll_psp.obj)
// Split-TU local declarations.

struct eForceComponent {
    short m_offset;                 // 0x70 region: short at +0, fn ptr at +4
    short m_pad;
    void (*m_apply)(void *target, float dt);
};

class eSimulatedController {
public:
    void AddExternalForces(float dt, float x);
    void IntegrateVelocity(float dt);
};

struct ePhysicsObject {
    void *m_unk0;
    eSimulatedController *m_owner;  // offset 4
    char m_pad8[0x70 - 8];
    eForceComponent m_force;        // offset 0x70
};

struct ePhysicsNode {
    void *m_unk0;
    ePhysicsObject *m_obj;          // offset 4
    void *m_unk8;
    ePhysicsNode *m_next;           // offset 0xC
};

class ePhysics {
public:
    void *m_unk0;
    void *m_unk4;
    void *m_unk8;
    eSimulatedController *m_simList; // offset 0xC
    char m_pad10[0x14 - 0x10];
    ePhysicsNode *m_forceList;        // offset 0x14
    char m_pad18[0x1c - 0x18];
    float m_gravity;                  // offset 0x1c
    void ApplyExternalForcesAndIntegrateVelocity(float dt);
};

// eSimulatedController has m_next at offset 0x154
struct eSimNode {
    char m_pad[0x154];
    eSimulatedController *m_next;
};

void ePhysics::ApplyExternalForcesAndIntegrateVelocity(float dt)
{
    ePhysicsNode *node = m_forceList;
    if (node != 0) {
        do {
            eForceComponent *fc = &node->m_obj->m_force;
            short off = fc->m_offset;
            fc->m_apply((char *)node + off, dt);
            node = node->m_next;
        } while (node != m_forceList);
    }

    eSimulatedController *sim = m_simList;
    if (sim != 0) {
        do {
            sim->AddExternalForces(dt, m_gravity);
            sim->IntegrateVelocity(dt);
            sim = ((eSimNode *)sim)->m_next;
        } while (sim != m_simList);
    }
}
