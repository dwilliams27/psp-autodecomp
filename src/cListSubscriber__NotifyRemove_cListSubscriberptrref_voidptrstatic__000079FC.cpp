// cListSubscriber::NotifyRemove(cListSubscriber *&, void *) static
// Address: 0x000079fc, Size: 128B
// Obj: cAll_psp.obj

struct NotifyEntry {
    short adjust;   // +0xA0
    short pad;
    void (*fn)(void *, void *, void *);  // +0xA4
};

struct NotifyObj {
    char pad[0xA0];
    NotifyEntry entry;  // at +0xA0
};

class cListSubscriber {
public:
    char pad0[4];
    NotifyObj *obj;        // +0x4
    char pad8[4];
    cListSubscriber *next; // +0xC

    static void NotifyRemove(cListSubscriber *&head, void *param);
};

void cListSubscriber::NotifyRemove(cListSubscriber *&head, void *param)
{
    cListSubscriber *node = head;
    if (node != 0) {
        do {
            NotifyObj *obj = node->obj;
            cListSubscriber *next = node->next;
            NotifyEntry *e = &obj->entry;
            void *self = (char *)node + e->adjust;
            void (*fn)(void *, void *, void *) = e->fn;
            fn(self, param, (void *)fn);
            if (next == head) break;
            if (next == node) break;
            node = next;
        } while (node != 0);
    }
}
