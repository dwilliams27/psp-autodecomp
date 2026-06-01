// gcCinematicInstance::FindPlayingCinematic(gcCinematic *, cHandle) static
// Address: 0x000ebd0c, Size: 168B
// Obj: gcAll_psp.obj
// Split-TU local class redeclaration — this overload is not in include/gcCinematic.h
// and must NOT be added there (banned).

#define NULL 0

class cHandle {
public:
    int mIndex;
};

class gcCinematic;

class gcCinematicInstance {
public:
    char pad_00[0x08];
    cHandle mHandle;                 // 0x08
    char pad_0C[0x54 - 0x0C];
    gcCinematicInstance *mNext2;     // 0x54

    static gcCinematicInstance *FindPlayingCinematic(gcCinematic *pCinematic, cHandle handle);
};

// Object identity field at +0x00.
struct gcCinematicNode {
    gcCinematic *mObject;            // 0x00
    char pad_04[0x08 - 0x04];
    int mHandle;                     // 0x08
    char pad_0C[0x54 - 0x0C];
    gcCinematicNode *mNext2;         // 0x54
};

// Handle-table entry (the 0x30 field is compared against the handle key).
struct gcHandleEntry {
    char pad_00[0x30];
    int mKey;                        // 0x30
};

extern gcCinematicNode *D_0037D7C4;                 // global list head, lui 0x38 / -10300
extern gcHandleEntry *D_00048890[];                 // handle table, lui 0x4 / -30576

gcCinematicInstance *gcCinematicInstance::FindPlayingCinematic(gcCinematic *pCinematic, cHandle handle) {
    gcCinematicNode *head = D_0037D7C4;
    if (head == NULL) {
        return NULL;
    }

    gcCinematicNode *node = head;
    int key = handle.mIndex;
    gcHandleEntry **table = &D_00048890[key & 0xFFFF];

    do {
        if (node->mObject == (gcCinematic *)pCinematic) {
            gcHandleEntry *entry;
            if (key == 0) {
                entry = NULL;
            } else {
                gcHandleEntry *cand = *table;
                entry = (cand != NULL && cand->mKey == key) ? cand : NULL;
            }

            if (entry == NULL || (unsigned char)(key == node->mHandle)) {
                return (gcCinematicInstance *)node;
            }
        }
        node = node->mNext2;
    } while (node != head);

    return NULL;
}
