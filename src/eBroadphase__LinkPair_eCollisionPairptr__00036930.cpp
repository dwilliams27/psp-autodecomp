// eBroadphase::LinkPair(eCollisionPair *) @ 0x00036930
//
// Intrusive-list insert with two buckets selected by whether the pair's two
// bodies are sleeping.  Awake pairs -> list head at eBroadphase+0x00 (flag bit
// 8 cleared); sleeping pairs -> list head at eBroadphase+0x04 (flag bit 8 set).
// The pair's link node is at 0x24 (next) / 0x20 (prev); flag halfword at 0x40.
//
// HARD-ISOLATION split-TU: classes declared locally; nothing shared touched.

class eCollisionPair {
public:
    int BodiesSleeping() const;

    char _pad0[0x20];
    eCollisionPair *m_pPrev; // 0x20
    eCollisionPair *m_pNext; // 0x24
    char _pad28[0x18];
    unsigned short m_flags;  // 0x40
};

class eBroadphase {
public:
    void LinkPair(eCollisionPair *);

    eCollisionPair *m_pList0; // 0x00
    eCollisionPair *m_pList1; // 0x04
};

void eBroadphase::LinkPair(eCollisionPair *pair) {
    if (pair->BodiesSleeping() != 0) {
        eCollisionPair *head = m_pList1;
        if (head != 0) {
            pair->m_pNext = head->m_pNext;
            pair->m_pPrev = m_pList1;
            pair->m_pNext->m_pPrev = pair;
            pair->m_pPrev->m_pNext = pair;
        } else {
            m_pList1 = pair;
            pair->m_pPrev = pair;
            pair->m_pNext = pair;
        }
        pair->m_flags = pair->m_flags | 8;
    } else {
        eCollisionPair *head = m_pList0;
        if (head != 0) {
            pair->m_pNext = head->m_pNext;
            pair->m_pPrev = m_pList0;
            pair->m_pNext->m_pPrev = pair;
            pair->m_pPrev->m_pNext = pair;
        } else {
            m_pList0 = pair;
            pair->m_pPrev = pair;
            pair->m_pNext = pair;
        }
        pair->m_flags = pair->m_flags & ~8;
    }
}
