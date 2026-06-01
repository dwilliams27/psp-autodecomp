// gcLookAtController::OnActivated(void) — 0x00147b5c
//
// On activation the controller validates its target reference (IsValid). If
// the target is currently valid it flags two slots in the owner's bone/array
// table (owner+0x1AC indexed by m_ref->mIdx*0x20) with bits 1 and 2, and sets
// a status bit (0x10) at owner+0x152. If the target is NOT valid it forwards a
// notification through the ref's interface dispatch table: m_ref->unk4 is a
// thunk record {short adjust @0x40; fn @0x44}; the call adjusts the ref ptr by
// the thunk offset and passes a zero-initialised stack buffer by reference.
//
// Split-TU local redeclaration mirrors the layout style used by the matched
// sibling src/gcLookAtController__IsValid_voidconst__00147AD0.cpp. No new
// declarations are added to include/*.h.

struct LookAtThunk {
    short mAdjust;            // 0x40 (read as halfword at +0x40 of dispatch)
    short _pad42;
    void (*mFn)(void *, void *); // 0x44
};

struct LookAtRefBody {
    char _pad0[4];
    char *mDispatch;          // 0x04 -> points at struct whose +0x40 is LookAtThunk
    char _pad08[0x14 - 0x08];
    signed char mIdx;         // 0x14
};

struct LookAtOwner {
    char _pad0[0x152];
    unsigned char mStatus;    // 0x152
    char _pad153[0x1AC - 0x153];
    char *mTable;             // 0x1AC
};

struct LookAtSlot {
    char _pad0[0x1C];
    unsigned short mFlags;    // 0x1C
};

class gcLookAtController {
public:
    LookAtOwner *m_owner;     // 0x00
    char _pad4[4];
    LookAtRefBody *m_ref;     // 0x08
    bool IsValid(void) const;
    void OnActivated(void);
};

void gcLookAtController::OnActivated(void) {
    LookAtRefBody *ref = m_ref;
    signed char idx = ref->mIdx;

    if (!IsValid()) {
        char buf[256];
        buf[0] = 0;
        LookAtRefBody *r = m_ref;
        LookAtThunk *thunk = (LookAtThunk *)(r->mDispatch + 0x40);
        short adjust = thunk->mAdjust;
        thunk->mFn((char *)r + adjust, buf);
        return;
    }

    LookAtOwner *owner = m_owner;
    int off = idx << 5;
    LookAtSlot *slot = (LookAtSlot *)(owner->mTable + off);
    slot->mFlags = (unsigned short)((slot->mFlags | 1) & 0xFFFF);
    LookAtSlot *slot2 = (LookAtSlot *)(owner->mTable + off);
    slot2->mFlags = (unsigned short)((slot2->mFlags | 2) & 0xFFFF);
    owner->mStatus |= 0x10;
}
