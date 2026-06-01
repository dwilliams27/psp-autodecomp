// ePhysics::AddToUpdateList(eSimulatedController *) @ 0x000374d8
//
// Insertion counterpart of RemoveFromUpdateList(eSimulatedController *) @
// 0x00037594.  The controller's sleeping state is queried through a real
// call (eSimulatedController::IsSleeping()) which selects which intrusive
// circular doubly-linked list the controller is parked in: the sleeping
// bucket head at ePhysics+0x10, the active bucket head at ePhysics+0x0C.
// The controller's link node is next=0x154 / prev=0x158, and a "was
// sleeping" flag is cached at 0x12.
//
// HARD-ISOLATION split-TU: classes declared locally; nothing shared touched.

class eSimulatedController {
public:
    bool IsSleeping(void) const;

    char _pad0[0x12];
    char m_wasSleeping;           // 0x12
    char _pad13[0x141];
    eSimulatedController *m_pNext; // 0x154
    eSimulatedController *m_pPrev; // 0x158
};

class ePhysics {
public:
    void AddToUpdateList(eSimulatedController *);

    char _pad0[0x0C];
    eSimulatedController *m_pActiveList;   // 0x0C
    eSimulatedController *m_pSleepingList; // 0x10
};

void ePhysics::AddToUpdateList(eSimulatedController *item) {
    if (item->IsSleeping()) {
        eSimulatedController *head = m_pSleepingList;
        if (head != 0) {
            item->m_pNext = head->m_pNext;
            item->m_pPrev = m_pSleepingList;
            item->m_pNext->m_pPrev = item;
            item->m_pPrev->m_pNext = item;
        } else {
            m_pSleepingList = item;
            item->m_pPrev = item;
            item->m_pNext = item;
        }
        item->m_wasSleeping = 1;
    } else {
        eSimulatedController *head = m_pActiveList;
        if (head != 0) {
            item->m_pNext = head->m_pNext;
            item->m_pPrev = m_pActiveList;
            item->m_pNext->m_pPrev = item;
            item->m_pPrev->m_pNext = item;
        } else {
            m_pActiveList = item;
            item->m_pPrev = item;
            item->m_pNext = item;
        }
        char f = 0;
        item->m_wasSleeping = f;
    }
}
