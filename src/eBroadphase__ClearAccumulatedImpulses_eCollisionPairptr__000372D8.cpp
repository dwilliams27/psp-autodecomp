// eBroadphase::ClearAccumulatedImpulses(eCollisionPair *) @ 0x000372d8
//
// Walks the intrusive ring of collision pairs starting at `head`.  For each
// pair: zero a 16-byte block at +0x4C (memset), zero the impulse float at +4
// of every contact constraint in the pair's contact list (head at +0x48),
// call eCollisionPair::RemoveContacts(), then clear flag bit 1 (mask ~2) in
// the halfword at +0x40.  Advances via the ring link at +0x20 until back at
// the starting pair.
//
// HARD-ISOLATION split-TU: classes declared locally; nothing shared touched.

extern "C" void *memset(void *, int, unsigned long);

class eCollisionConstraint {
public:
    eCollisionConstraint *m_pNext;  // 0x00
    float m_impulse;                // 0x04
};

class eCollisionPair {
public:
    char _pad0[0x20];
    eCollisionPair *m_pRingNext;        // 0x20
    char _pad24[0x1C];                  // 0x24
    unsigned short m_flags;             // 0x40
    char _pad42[0x06];                  // 0x42
    eCollisionConstraint *m_pContacts;  // 0x48
    char _pad4C[0x10];                  // 0x4C
    void RemoveContacts();
};

class eBroadphase {
public:
    void ClearAccumulatedImpulses(eCollisionPair *head);
};

void eBroadphase::ClearAccumulatedImpulses(eCollisionPair *head) {
    if (head != 0) {
        eCollisionPair *pair = head;
        do {
            eCollisionConstraint *c = pair->m_pContacts;
            memset(&pair->_pad4C[0], 0, 0x10);
            if (c != 0) {
                do {
                    c->m_impulse = 0.0f;
                    c = c->m_pNext;
                } while (c != 0);
            }
            pair->RemoveContacts();
            pair->m_flags = (unsigned short)(pair->m_flags & ~2);
            pair = pair->m_pRingNext;
        } while (pair != head);
    }
}
