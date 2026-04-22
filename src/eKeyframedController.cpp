#include "eKeyframedController.h"

class cBase;
class mVec3;
class mOCS;

class ePhysicsController {
public:
    void *mOwner;
    void *mClassDesc;
    int mField8;
    int mFieldC;
    ePhysicsController(cBase *);
};

extern char eKeyframedControllerclassdesc[];

class eKeyframedControllerImpl : public ePhysicsController {
public:
    int mField10;
    int mField14;
    eKeyframedControllerImpl(cBase *);
    void GetVelocity(int, mVec3 *, mVec3 *) const;
};

class eKeyframedController;
eKeyframedController *dcast(const cBase *);

typedef int v4sf_t __attribute__((mode(V4SF)));

// eKeyframedController::eKeyframedController(cBase *) @ 0x00074ebc
eKeyframedControllerImpl::eKeyframedControllerImpl(cBase *b) : ePhysicsController(b) {
    mClassDesc = eKeyframedControllerclassdesc;
    mField14 = 0;
}

// eKeyframedController::GetVelocity(int, mVec3 *, mVec3 *) const @ 0x000750d4
// Matches byte-exact at -Xsched=1 (confirmed via file-rename test). At the
// default -Xsched=2 used for eKeyframedController.cpp, SNC fills the load-use
// delay between the two consecutive lw's with the sll, producing 8 bytes of
// diff. Needs a Makefile override: $(BUILD_DIR)/src/eKeyframedController%.o: CFLAGS := $(ECFLAGS)
void eKeyframedControllerImpl::GetVelocity(int idx, mVec3 *out_a, mVec3 *out_b) const {
    v4sf_t *v = (v4sf_t *)(**(char ***)((char *)this + 0x10) + idx * 192);
    *(v4sf_t *)out_b = v[5];
    *(v4sf_t *)out_a = v[4];
}

void eKeyframedController::ApplyPositionedImpulse(int, const mVec3 &, const mVec3 &) {
}

#pragma control sched=1

// eKeyframedController::AssignCopy(const cBase *) @ 0x0020de14
void eKeyframedController::AssignCopy(const cBase *other) {
    eKeyframedController *p = dcast(other);
    eKeyframedControllerImpl *src = (eKeyframedControllerImpl *)p;
    eKeyframedControllerImpl *self = (eKeyframedControllerImpl *)this;
    self->mField8 = src->mField8;
    __asm__ volatile("" ::: "memory");
    self->mFieldC = src->mFieldC;
    self->mField10 = src->mField10;
    self->mField14 = src->mField14;
}
