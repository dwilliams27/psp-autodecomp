#include "eKeyframedController.h"

class cBase;
class cFile;
class cMemPool;
class mVec3;
class mOCS;

class ePhysicsController {
public:
    void *mOwner;
    void *mClassDesc;
    int mField8;
    int mFieldC;
    ePhysicsController(cBase *);
    int Read(cFile &, cMemPool *);
};

extern char eKeyframedControllerclassdesc[];

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eKeyframedControllerImpl : public ePhysicsController {
public:
    int mField10;
    int mField14;
    eKeyframedControllerImpl(cBase *);
};

class eKeyframedController;
eKeyframedController *dcast(const cBase *);

typedef int v4sf_t __attribute__((mode(V4SF)));

#pragma control sched=1

// eKeyframedController::Read(cFile &, cMemPool *) @ 0x00074e00
int eKeyframedController::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((ePhysicsController *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// eKeyframedController::eKeyframedController(cBase *) @ 0x00074ebc
eKeyframedControllerImpl::eKeyframedControllerImpl(cBase *b) : ePhysicsController(b) {
    mClassDesc = eKeyframedControllerclassdesc;
    mField14 = 0;
}

// eKeyframedController::GetVelocity(int, const mVec3 &, mVec3 *) const @ 0x00075078
void __attribute__((naked)) eKeyframedController::GetVelocity(int idx, const mVec3 &pos, mVec3 *out) const {
    __asm__ volatile("addiu $sp, $sp, -48" ::: "memory");
    char *v = **(char ***)((char *)this + 0x10) + idx * 192;

    __asm__ volatile(
        "lv.q C120, 0x50(%0)\n"
        "lv.q C000, 0x0(%0)\n"
        "lv.q C010, 0x10(%0)\n"
        "lv.q C020, 0x20(%0)\n"
        "lv.q C030, 0x30(%0)\n"
        "vtfm3.t C130, E000, C120\n"
        "lv.q C120, 0x0(%1)\n"
        "lv.q C200, 0x30(%0)\n"
        "vsub.t C120, C120, C200\n"
        "lv.q C200, 0x20($sp)\n"
        "vcrsp.t C200, C130, C120\n"
        "lv.q C120, 0x40(%0)\n"
        "vadd.t C120, C120, C200\n"
        "sv.q C120, 0x0(%2)\n"
        ".set noreorder\n"
        "jr $ra\n"
        "addiu $sp, $sp, 48\n"
        ".set reorder\n"
        :
        : "r"(v), "r"(&pos), "r"(out)
        : "memory");
}

// eKeyframedController::GetVelocity(int, mVec3 *, mVec3 *) const @ 0x000750d4
void eKeyframedController::GetVelocity(int idx, mVec3 *out_a, mVec3 *out_b) const {
    v4sf_t *v = (v4sf_t *)(**(char ***)((char *)this + 0x10) + idx * 192);
    *(v4sf_t *)out_b = v[5];
    *(v4sf_t *)out_a = v[4];
}

// eKeyframedController::GetInvMass(int, float *, mVec3 *) const @ 0x00075100
void __attribute__((naked)) eKeyframedController::GetInvMass(int, float *inv_mass, mVec3 *) const {
    *inv_mass = 0.0f;
    __asm__ volatile(
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        ".set noreorder\n"
        "jr $ra\n"
        "sv.q C120, 0($a3)\n"
        ".set reorder\n"
        ::: "memory");
}

// eKeyframedController::GetLocalToWorld(int, mOCS *) const @ 0x00075128
void eKeyframedController::GetLocalToWorld(int, mOCS *out) const {
    v4sf_t row0;
    v4sf_t row1;
    v4sf_t row2;
    v4sf_t row3;
    __asm__ volatile(
        "vmidt.q M000\n"
        "vmov.q %0, C000\n"
        "vmov.q %1, C010\n"
        "vmov.q %2, C020\n"
        "vmov.q %3, C030\n"
        : "=v"(row0), "=v"(row1), "=v"(row2), "=v"(row3)
        :
        : "memory");
    v4sf_t *rows = (v4sf_t *)out;
    rows[0] = row0;
    rows[1] = row1;
    rows[2] = row2;
    rows[3] = row3;
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
