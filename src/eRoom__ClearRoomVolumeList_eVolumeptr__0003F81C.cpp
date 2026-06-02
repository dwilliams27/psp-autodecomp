// eRoom::ClearRoomVolumeList(eVolume *) @ 0x0003f81c
// symbol: __0fFeRoomTClearRoomVolumeListP6HeVolume

class eVolume;

struct VNode;

struct Owner {
    char _pad[0x114];
    VNode *headB;  // 0x114
};

struct VNode {
    VNode *f0;     // 0x00
    Owner *owner;  // 0x04
    VNode *prevB;  // 0x08
    VNode *nextB;  // 0x0C
    VNode *prevA;  // 0x10
    VNode *nextA;  // 0x14
};

struct VHolder {
    char _pad[0x80];
    VNode *head;   // 0x80
};

struct FreeList {
    char _pad[0x4];
    VNode *head;   // 0x04
};

class eRoomVolumeLinkMgr {
public:
    static FreeList *s_pLinkMgr;  // @ 0x37D2F4
};

class eRoom {
public:
    void ClearRoomVolumeList(eVolume *);
};

void eRoom::ClearRoomVolumeList(eVolume *volume)
{
    VNode *node = ((VHolder *)volume)->head;
    if (node == 0) {
        return;
    }
    do {
        Owner *owner = node->owner;

        if (node->prevA != 0) {
            VNode *na = node->nextA;
            if (na != 0) {
                ((VHolder *)volume)->head = na;
                node->prevA->nextA = node->nextA;
                node->nextA->prevA = node->prevA;
                node->prevA = 0;
                node->nextA = 0;
                if (((VHolder *)volume)->head == node) {
                    ((VHolder *)volume)->head = 0;
                }
            }
        }

        if (node->prevB != 0) {
            VNode *nb = node->nextB;
            if (nb != 0) {
                if (owner->headB == node) {
                    owner->headB = node->nextB;
                }
                node->prevB->nextB = node->nextB;
                node->nextB->prevB = node->prevB;
                node->prevB = 0;
                node->nextB = 0;
                if (owner->headB == node) {
                    owner->headB = 0;
                }
            }
        }

        node->prevB = 0;
        node->nextB = 0;
        node->prevA = 0;
        node->nextA = 0;
        node->f0 = 0;
        node->owner = 0;

        VNode *flHead = eRoomVolumeLinkMgr::s_pLinkMgr->head;
        if (flHead != 0) {
            node->prevB = flHead->prevB;
            node->nextB = eRoomVolumeLinkMgr::s_pLinkMgr->head;
            node->prevB->nextB = node;
            node->nextB->prevB = node;
        } else {
            eRoomVolumeLinkMgr::s_pLinkMgr->head = node;
            node->nextB = node;
            node->prevB = node;
        }

        node = ((VHolder *)volume)->head;
    } while (node != 0);
}
