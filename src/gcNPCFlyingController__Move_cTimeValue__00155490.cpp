// gcNPCFlyingController::Move(cTimeValue) @ 0x00155490 (132B) — gcAll_psp.obj
//
// Twin of gcNPCBipedController::Move (0x154eac). Register allocation target:
//   s0 = this, s2 = *this (entity), s1 = entity->unk1F8 (nav).
//   The cTimeValue arg (a1) is spilled to 0(sp) and reloaded (lw a1,0(sp))
//   before each call — NOT cached in a callee-save. Only 3 callee-saves used.
//
// Split-TU local declarations.

class cTimeValue;
struct __attribute__((aligned(16))) mVec3 { float x, y, z; };
struct __attribute__((aligned(16))) mOCS { float m[12]; };

struct Nav {
    char _pad4C[0x4C];
    int flags;          // +0x4C
};

struct Entity {
    char _pad1F8[0x1F8];
    Nav *nav;           // +0x1F8
    char _pad218[0x218 - 0x1FC];
    int handle;         // +0x218
};

class gcEntityController {
public:
    void HandleNextAnimation(cTimeValue);
    void UpdateLocalToWorld(cTimeValue, mOCS *, mVec3 *);
    void HandleHeightOnPath(mVec3 *);
};

class gcNPCFlyingController : public gcEntityController {
public:
    void Move(cTimeValue);
};

// cTimeValue is a single-word value type passed in a1.
class cTimeValue {
public:
    int _t;
};

// ── gcNPCFlyingController::Move(cTimeValue) @ 0x00155490 ──
void gcNPCFlyingController::Move(cTimeValue t) {
    Entity *entity = *(Entity **)this;
    Nav *nav = entity->nav;

    this->HandleNextAnimation(t);

    if (!(unsigned char)(entity->handle != 0)) {
        mOCS sp10;
        this->UpdateLocalToWorld(t, &sp10, (mVec3 *)((char *)this + 0xA0));
        if (nav->flags & 2) {
            mVec3 sp40;
            this->HandleHeightOnPath(&sp40);
        }
    }
}
