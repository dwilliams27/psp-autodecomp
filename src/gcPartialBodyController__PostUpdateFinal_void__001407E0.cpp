// gcPartialBodyController::PostUpdateFinal(void)
// 0x001407e0, 160B, gcAll_psp.obj
// Symbol: __0fXgcPartialBodyControllerPPostUpdateFinalv
//
// Structural twin of gcEntityController::PostUpdateFinal (0x001110fc, src/gcEntityController.cpp).
// Differences are member offsets and the animation-state accessor:
//   attack pointer at this+0x40 (vs +0x7C),
//   the partial-animation index is the s16 at (this+0x8)+0x1C,
//   the animation state is eDynamicModel::GetPartialAnimationState(index) on (*this)+0x80,
//   the 31-bit signed time bitfield lives at +0xC of the returned state.

class gcEntity;

class cTimeValue {
public:
    int mTime;
};

class eDynamicModel {
public:
    void *GetPartialAnimationState(int);
};

class gcEntityAttackState {
public:
    void *m_active;
    bool IsActive(void) const { return m_active != 0; }
    void QueryCollisions(gcEntity *, cTimeValue);
};

// Object held at this+0x8; the partial-animation index is the s16 at +0x1C.
struct gcPartialAnimInfo {
    char _pad[0x1C];
    short mIndex;  // 0x1C
};

class gcPartialBodyController {
public:
    void PostUpdateFinal(void);
};

void gcPartialBodyController::PostUpdateFinal(void) {
    gcEntityAttackState *attack = *(gcEntityAttackState **)((char *)this + 0x40);
    if (attack == 0) return;
    if (!attack->IsActive()) return;

    int index = (*(gcPartialAnimInfo **)((char *)this + 0x8))->mIndex;
    eDynamicModel *model = (eDynamicModel *)(*(char **)this + 0x80);
    void *anim = model->GetPartialAnimationState(index);
    if (anim == 0) return;

    struct AnimTimeView {
        int padA;
        int padB;
        int padC;
        signed int time : 31;
        unsigned int flag : 1;
    };
    AnimTimeView *view = (AnimTimeView *)anim;
    gcEntityAttackState *attack2 = *(gcEntityAttackState **)((char *)this + 0x40);
    gcEntity *entity2 = *(gcEntity **)this;
    cTimeValue t;
    t.mTime = view->time;
    attack2->QueryCollisions(entity2, t);
}
