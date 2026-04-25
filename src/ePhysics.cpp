// ePhysics — physics scheduler / singleton.
//
// Owns three intrusive circular doubly-linked lists used during the per-frame
// update step:
//   m_pSimCtrlList  (eSimulatedController, link at 0x150/0x154)
//   m_pMotorList    (eSimulatedMotor,      link at 0x0C/0x10)
//   m_pBipedList    (eBipedController,     link at 0x2B0/0x2B4)
//
// Functions in this TU:
//   ePhysics::Get() static
//   ePhysics::GetPhysicsMemPoolSize() static
//   ePhysics::AddToUpdateList(eBipedController *)
//   ePhysics::AddMotor(eSimulatedMotor *)
//   ePhysics::UpdateController(eSimulatedController *)

class eBipedController;
class eSimulatedController;
class eSimulatedMotor;

// Minimal facets of related classes used by this TU.  Real headers exist for
// eBipedController/eSimulatedController but carry layouts unrelated to the
// physics list links; declaring only the link nodes here keeps the source
// honest about which fields participate in the byte-exact codegen.

class eSimulatedMotor {
public:
    char _pad0[0x0C];
    eSimulatedMotor *m_pNext;  // 0x0C
    eSimulatedMotor *m_pPrev;  // 0x10
};

class eBipedController {
public:
    char _pad0[0x2B0];
    eBipedController *m_pNext;  // 0x2B0
    eBipedController *m_pPrev;  // 0x2B4
};

class eSimulatedController {
public:
    bool IsSleeping(void) const;

    char _pad0[0x12];
    bool m_wasSleeping;  // 0x12
};

class eConfigBase {
public:
    static eConfigBase *s_pInstance;

    char _pad0[0x60];
    int m_numPositionalConstraints;  // 0x60
    int m_numAngularConstraints;     // 0x64
    int m_numMotors;                 // 0x68
};

class ePhysics {
public:
    static ePhysics *Get(void);
    static int GetPhysicsMemPoolSize(void);

    void AddToUpdateList(eBipedController *);
    void AddToUpdateList(eSimulatedController *);
    void AddMotor(eSimulatedMotor *);
    void RemoveFromUpdateList(eSimulatedController *);
    void UpdateController(eSimulatedController *);

    static ePhysics *s_pPhysics;

    char _pad0[0x0C];
    eSimulatedController *m_pSimCtrlList;  // 0x0C
    char _pad1[0x04];
    eSimulatedMotor *m_pMotorList;         // 0x14
    eBipedController *m_pBipedList;        // 0x18
};

// ── Get ────────────────────────────────────────────────────────────────────
ePhysics *ePhysics::Get(void) {
    return s_pPhysics;
}

// ── GetPhysicsMemPoolSize ──────────────────────────────────────────────────
// Inlined sum of pool budgets keyed off the active eConfigBase singleton.
// Constants 20/80/112 correspond to per-instance sizes of the three pools and
// 0x145C is the fixed overhead for the physics scratch space.
int ePhysics::GetPhysicsMemPoolSize(void) {
    eConfigBase *cfg = eConfigBase::s_pInstance;
    return cfg->m_numMotors * 20
         + cfg->m_numPositionalConstraints * 80
         + cfg->m_numAngularConstraints * 112
         + 0x145C;
}

// ── AddToUpdateList(eBipedController *) ────────────────────────────────────
// Insert `item` after the list head; if the list was empty `item` becomes the
// head and points to itself in both directions.  The redundant re-loads of
// `m_pBipedList` and `item->m_pNext`/`m_pPrev` mirror the SNC scheduling that
// fills the lw/sw delay-slots.
void ePhysics::AddToUpdateList(eBipedController *item) {
    eBipedController *head = m_pBipedList;
    if (head != 0) {
        item->m_pNext = head->m_pNext;
        item->m_pPrev = m_pBipedList;
        item->m_pNext->m_pPrev = item;
        item->m_pPrev->m_pNext = item;
    } else {
        m_pBipedList = item;
        item->m_pPrev = item;
        item->m_pNext = item;
    }
}

// ── AddMotor ───────────────────────────────────────────────────────────────
void ePhysics::AddMotor(eSimulatedMotor *motor) {
    eSimulatedMotor *head = m_pMotorList;
    if (head != 0) {
        motor->m_pNext = head->m_pNext;
        motor->m_pPrev = m_pMotorList;
        motor->m_pNext->m_pPrev = motor;
        motor->m_pPrev->m_pNext = motor;
    } else {
        m_pMotorList = motor;
        motor->m_pPrev = motor;
        motor->m_pNext = motor;
    }
}

// ── UpdateController ───────────────────────────────────────────────────────
// If the sleeping flag has flipped since the last update, re-bucket the
// controller by removing it from and re-adding it to the update list.
void ePhysics::UpdateController(eSimulatedController *ctrl) {
    if (ctrl->IsSleeping() != ctrl->m_wasSleeping) {
        RemoveFromUpdateList(ctrl);
        AddToUpdateList(ctrl);
    }
}
