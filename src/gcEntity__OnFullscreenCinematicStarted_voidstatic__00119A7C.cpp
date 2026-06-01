// gcEntity::OnFullscreenCinematicStarted(void) static @ 0x00119a7c
//
// Sibling of OnFullscreenCinematicStarting (0x00119a24). Same global-flag guard
// (bit1 / 0x2 of the word at 0x37D858) and same circular entity-list walk
// (head at *0x37D86C, next at entity+0x228). For each entity with no parent
// (signed byte at +0x68 == 0) whose state word at +0x5C has 0x80 set but NOT
// 0x20000 set, calls gcEntity::Destroy(cTimeValue) with a default (zero)
// cTimeValue.
//
// Split-TU pattern: gcEntity is declared locally with only the members this TU
// needs, so SNC mangles the method natively as
// __0fIgcEntitycOnFullscreenCinematicStartedvT.

struct cTimeValue {
    int t;
};

class gcEntity {
public:
    void Destroy(cTimeValue);
    static void OnFullscreenCinematicStarted(void);
};

struct gcEntityNode {
    char pad0[0x5C];             // +0x00
    int  state;                  // +0x5C
    char pad1[0x68 - 0x60];
    char parent;                 // +0x68 (signed byte)
    char pad2[0x228 - 0x69];
    gcEntityNode *next;          // +0x228
};

void gcEntity::OnFullscreenCinematicStarted(void) {
    int *flags = (int *)0x380000;
    int active = ((flags[-2538] & 0x2) != 0) & 0xFF;
    if (active) {
        gcEntityNode **head = (gcEntityNode **)0x380000;
        gcEntityNode *node = head[-2533];   // *0x37D86C
        if (node != 0) {
            do {
                if (node->parent == 0) {
                    int s = node->state;
                    if ((s & 0x80) && !(s & 0x20000)) {
                        cTimeValue tv = { 0 };
                        ((gcEntity *)node)->Destroy(tv);
                    }
                }
                node = node->next;
            } while (node != head[-2533]);
        }
    }
}
