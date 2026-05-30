// ePhysics::RemoveFromUpdateList(eSimulatedController *) @ 0x00037594
//
// Two-bucket variant of the intrusive circular-list unlink (sibling:
// RemoveFromUpdateList(eBipedController *) @ 0x000376f8).  The controller
// carries a "was sleeping" flag at 0x12 that selects which list head it was
// parked in: the sleeping bucket head lives at ePhysics+0x10, the active
// bucket head at ePhysics+0x0C.  The controller's link node is at 0x154
// (next) / 0x158 (prev).
//
// HARD-ISOLATION split-TU: classes declared locally; nothing shared touched.

class eSimulatedController {
public:
    char _pad0[0x12];
    bool m_wasSleeping;            // 0x12
    char _pad13[0x141];
    eSimulatedController *m_pNext; // 0x154
    eSimulatedController *m_pPrev; // 0x158
};

class ePhysics {
public:
    void RemoveFromUpdateList(eSimulatedController *);

    char _pad0[0x0C];
    eSimulatedController *m_pActiveList;   // 0x0C
    eSimulatedController *m_pSleepingList; // 0x10
};

void ePhysics::RemoveFromUpdateList(eSimulatedController *item) {
    if (item->m_wasSleeping != 0) {
        if ((item != 0) && (item->m_pNext != 0) && (item->m_pPrev != 0)) {
            if (m_pSleepingList == item) {
                m_pSleepingList = item->m_pPrev;
            }
            item->m_pNext->m_pPrev = item->m_pPrev;
            item->m_pPrev->m_pNext = item->m_pNext;
            item->m_pNext = 0;
            item->m_pPrev = 0;
            if (m_pSleepingList == item) {
                m_pSleepingList = 0;
            }
        }
    } else {
        if ((item != 0) && (item->m_pNext != 0) && (item->m_pPrev != 0)) {
            if (m_pActiveList == item) {
                m_pActiveList = item->m_pPrev;
            }
            item->m_pNext->m_pPrev = item->m_pPrev;
            item->m_pPrev->m_pNext = item->m_pNext;
            item->m_pNext = 0;
            item->m_pPrev = 0;
            if (m_pActiveList == item) {
                m_pActiveList = 0;
            }
        }
    }
}
