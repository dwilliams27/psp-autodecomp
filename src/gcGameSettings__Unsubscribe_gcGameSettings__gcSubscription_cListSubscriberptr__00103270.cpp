// gcGameSettings::Unsubscribe(gcGameSettings::gcSubscription, cListSubscriber*)
// Address: 0x00103270, Size: 104B, Obj: gcAll_psp.obj
// Self-contained TU emitting:
//   __0fOgcGameSettingsLUnsubscribe65OgcGameSettingsh__5OgcGameSettingsOgcSubscriptionP6PcListSubscriber

struct cListSubscriber {
    void *pad0;            // 0x00
    void *pad4;            // 0x04
    cListSubscriber *next; // 0x08
    cListSubscriber *prev; // 0x0C
};

class gcGameSettings {
public:
    enum gcSubscription { kSubA };
    char pad[0x80];                  // 0x00..0x7F
    cListSubscriber *mAnchors[2];    // 0x80 indexed by subscription

    void Unsubscribe(gcSubscription, cListSubscriber *);
};

void gcGameSettings::Unsubscribe(gcSubscription sub, cListSubscriber *node) {
    if (node != 0 && node->next != 0) {
        if (node->prev != 0) {
            if (mAnchors[sub] == node) {
                mAnchors[sub] = node->prev;
            }
            node->next->prev = node->prev;
            node->prev->next = node->next;
            node->next = 0;
            node->prev = 0;
            if (mAnchors[sub] == node) {
                mAnchors[sub] = 0;
            }
        }
    }
}
