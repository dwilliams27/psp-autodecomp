// eBroadphase::UnlinkPair(eCollisionPair *) @ 0x00036a08
//
// Two-bucket variant of the intrusive list unlink (sibling:
// ePhysics::RemoveFromUpdateList @ 0x00037594).  The pair carries a flag
// halfword at 0x40; bit 8 selects which list head the pair was linked into:
// set -> head at eBroadphase+0x04, clear -> head at eBroadphase+0x00.
// The pair's link node is at 0x24 (next) / 0x20 (prev).  The head is
// re-pointed to the prev pointer when the unlinked pair was the head.
//
// HARD-ISOLATION split-TU: classes declared locally; nothing shared touched.

class eCollisionPair {
public:
    char _pad0[0x20];
    eCollisionPair *m_pPrev; // 0x20
    eCollisionPair *m_pNext; // 0x24
    char _pad28[0x18];
    unsigned short m_flags;  // 0x40
};

class eBroadphase {
public:
    void UnlinkPair(eCollisionPair *);

    eCollisionPair *m_pList0; // 0x00
    eCollisionPair *m_pList1; // 0x04
};

void eBroadphase::UnlinkPair(eCollisionPair *pair) {
    if ((pair->m_flags & 8) != 0) {
        if ((pair != 0) && (pair->m_pNext != 0) && (pair->m_pPrev != 0)) {
            if (m_pList1 == pair) {
                m_pList1 = pair->m_pPrev;
            }
            pair->m_pNext->m_pPrev = pair->m_pPrev;
            pair->m_pPrev->m_pNext = pair->m_pNext;
            pair->m_pNext = 0;
            pair->m_pPrev = 0;
            if (m_pList1 == pair) {
                m_pList1 = 0;
            }
        }
    } else {
        if ((pair != 0) && (pair->m_pNext != 0) && (pair->m_pPrev != 0)) {
            if (m_pList0 == pair) {
                m_pList0 = pair->m_pPrev;
            }
            pair->m_pNext->m_pPrev = pair->m_pPrev;
            pair->m_pPrev->m_pNext = pair->m_pNext;
            pair->m_pNext = 0;
            pair->m_pPrev = 0;
            if (m_pList0 == pair) {
                m_pList0 = 0;
            }
        }
    }
}
