// gcNPCBipedController::Move(cTimeValue) @ 0x00154eac (132B), gcAll_psp.obj
// Symbol: __0fUgcNPCBipedControllerEMove6KcTimeValue
//
// Split-TU: classes declared locally (only the members/methods this TU
// references). Does NOT touch include/*.h or any shared TU.
//
// ABI shape (from disasm):
//   s2 = *(void**)this            // entity pointer at this[0]
//   s1 = entity->[0x1F8]          // nav pointer at entity+504
//   this->HandleNextAnimation(t)  // re-passes t (spilled to 0(sp), reloaded)
//   if (!entity->[0x218]) {
//       this->UpdateLocalToWorld(t, &ocs, (mVec3*)(this+0xA0));
//       if (nav->[0x4C] & 2)
//           this->HandleHeightOnPath(&v);
//   }
// The time arg is intentionally NOT cached in a local so SNC keeps only
// 3 callee-save regs (s0/s1/s2) and reloads a1 from 0(sp) before each call.

class cTimeValue {
public:
    int value;
};

struct __attribute__((aligned(16))) mVec3 {
    float x, y, z;
};

struct __attribute__((aligned(16))) mOCS {
    float m[12];
};

struct gcNav {
    char pad[0x4C];
    int flags4C;   // 0x4C
};

struct gcEntityCore {
    char pad1F8[0x1F8];
    gcNav *nav;        // 0x1F8
    char pad1FC[0x218 - 0x1FC];
    int flag218;       // 0x218

    bool IsBlocked() const { return flag218 != 0; }
};

class gcEntityController {
public:
    void HandleNextAnimation(cTimeValue t);
    void HandleHeightOnPath(mVec3 *v);
};

class gcBipedController : public gcEntityController {
public:
    void UpdateLocalToWorld(cTimeValue t, mOCS *ocs, mVec3 *v);
};

class gcNPCBipedController : public gcBipedController {
public:
    void Move(cTimeValue t);
};

void gcNPCBipedController::Move(cTimeValue t)
{
    gcEntityCore *e = *(gcEntityCore **)this;
    gcNav *nav = e->nav;

    mOCS ocs;
    mVec3 v;

    HandleNextAnimation(t);

    if (!e->IsBlocked()) {
        UpdateLocalToWorld(t, &ocs, (mVec3 *)((char *)this + 0xA0));
        if (nav->flags4C & 2) {
            HandleHeightOnPath(&v);
        }
    }
}
