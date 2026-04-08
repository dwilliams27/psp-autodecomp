struct eBipedLink {
    char _pad[0x2B0];
    eBipedLink *next;
    eBipedLink *prev;
};

struct ePhysBipedList {
    char _pad[0x18];
    eBipedLink *bipedHead;
};

extern "C" {

void ePhysics__AddToUpdateList_eBipedControllerptr(ePhysBipedList *self, eBipedLink *item) {
    eBipedLink *head = self->bipedHead;
    if (head != 0) {
        item->next = head->next;
        item->prev = self->bipedHead;
        item->next->prev = item;
        item->prev->next = item;
    } else {
        self->bipedHead = item;
        item->prev = item;
        item->next = item;
    }
}

}
