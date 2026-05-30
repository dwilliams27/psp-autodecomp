// cLanguage::Unsubscribe(cLanguage::gcSubscription, cListSubscriber*) static
// Address: 0x00008310, Size: 112B, Obj: cAll_psp.obj
// Self-contained TU emitting:
//   __0fJcLanguageLUnsubscribe65JcLanguagec__5JcLanguageOgcSubscriptionP6PcListSubscriberT

struct cListSubscriber {
    void *pad0;            // 0x00
    void *pad4;            // 0x04
    cListSubscriber *next; // 0x08
    cListSubscriber *prev; // 0x0C
};

class cLanguage {
public:
    enum gcSubscription { kSubA };
    static void Unsubscribe(gcSubscription, cListSubscriber *);
};

// Global anchor table at 0x38884, indexed by subscription.
static cListSubscriber *gAnchors[1];

void cLanguage::Unsubscribe(gcSubscription sub, cListSubscriber *node) {
    if (node != 0 && node->next != 0) {
        if (node->prev != 0) {
            if (gAnchors[sub] == node) {
                gAnchors[sub] = node->prev;
            }
            node->next->prev = node->prev;
            node->prev->next = node->next;
            node->next = 0;
            node->prev = 0;
            if (gAnchors[sub] == node) {
                gAnchors[sub] = 0;
            }
        }
    }
}
