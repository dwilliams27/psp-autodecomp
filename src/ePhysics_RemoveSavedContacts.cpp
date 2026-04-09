struct eCtrlLink {
    char _pad[0x154];
    eCtrlLink *next;
};

struct ePhysCtrlList {
    char _pad[0x0C];
    eCtrlLink *ctrlHead;
};

extern "C" {

extern void eSimulatedController__RemoveContacts_void__0006CEFC(eCtrlLink *);

void ePhysics__RemoveSavedContacts_void(ePhysCtrlList *self) {
    eCtrlLink *node = self->ctrlHead;
    if (node != 0) {
        do {
            eSimulatedController__RemoveContacts_void__0006CEFC(node);
            node = node->next;
        } while (node != self->ctrlHead);
    }
}

}
