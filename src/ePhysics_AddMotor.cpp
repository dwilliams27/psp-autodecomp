struct eMotorLink {
    char _pad[0x0C];
    eMotorLink *next;
    eMotorLink *prev;
};

struct ePhysMotorList {
    char _pad[0x14];
    eMotorLink *motorHead;
};

extern "C" {

void ePhysics__AddMotor_eSimulatedMotorptr(ePhysMotorList *self, eMotorLink *item) {
    eMotorLink *head = self->motorHead;
    if (head != 0) {
        item->next = head->next;
        item->prev = self->motorHead;
        item->next->prev = item;
        item->prev->next = item;
    } else {
        self->motorHead = item;
        item->prev = item;
        item->next = item;
    }
}

}
