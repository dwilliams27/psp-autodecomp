// src/eStaticPointLight.cpp
// Functions:
//   0x0005eec4  eStaticPointLight::Write(cFile &) const                       eAll_psp.obj
//   0x0005f15c  eStaticPointLight::GetSampleRay(mRay*, mVec3*, const mVec3&, const mVec3&) const   eAll_psp.obj
//   0x00205df0  eStaticPointLight::AssignCopy(const cBase *)                  eAll_psp.obj

class cBase;
class cFile;
class mRay;
class mVec3;

typedef int v4sf_t __attribute__((mode(V4SF)));

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

class eStaticLight {
public:
    void Write(cFile &) const;
};

class eStaticPointLight : public eStaticLight {
public:
    void Write(cFile &) const;
    void GetSampleRay(mRay *, mVec3 *, const mVec3 &, const mVec3 &) const;
    void AssignCopy(const cBase *);
};

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mIndex;
};

#pragma control sched=1
// ── 0x0005eec4 — Write(cFile &) const ──
void eStaticPointLight::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eStaticLight *)this)->Write(file);
    wb.End();
}

// ── 0x00205df0 — AssignCopy(const cBase *) ──
void eStaticPointLight::AssignCopy(const cBase *base) {
    eStaticPointLight *other = dcast<eStaticPointLight>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(float *)((char *)this + 68) = *(const float *)((char *)other + 68);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 72) = *(const cHandle *)((char *)other + 72);
    __asm__ volatile("" ::: "memory");
    *(v4sf_t *)((char *)this + 0x80) = *(const v4sf_t *)((char *)other + 0x80);
    *(v4sf_t *)((char *)this + 0x50) = *(const v4sf_t *)((char *)other + 0x50);
    *(v4sf_t *)((char *)this + 0x60) = *(const v4sf_t *)((char *)other + 0x60);
    *(v4sf_t *)((char *)this + 0x70) = *(const v4sf_t *)((char *)other + 0x70);
}

// ── 0x0005f15c — GetSampleRay(...) const ──
void eStaticPointLight::GetSampleRay(mRay *ray, mVec3 *, const mVec3 &p1, const mVec3 &) const {
    __asm__ volatile(
        ".word 0xd8860080\n"  /* lv.q     C120, 0x80($a0)        */
        ".word 0xd8e70000\n"  /* lv.q     C130, 0x0($a3)         */
        ".word 0x60878606\n"  /* vsub.t   C120, C120, C130       */
        ".word 0xf8a60010\n"  /* sv.q     C120, 0x10($a1)        */
        ".word 0x64868604\n"  /* vdot.t   S100, C120, C120       */
        ".word 0x6c000408\n"  /* vcmp.s   ez, S100               */
        ".word 0xd0110424\n"  /* vrsq.s   S101, S100             */
        ".word 0xd0160444\n"  /* vsqrt.s  S102, S100             */
        ".word 0xdc0010e5\n"  /* vpfxs    1, Y, Z, W             */
        ".word 0xd2a00424\n"  /* vcmovt.s S101, S100, 0          */
        ".word 0x65248606\n"  /* vscl.t   C120, C120, S101       */
        ".word 0xf8a60010\n"  /* sv.q     C120, 0x10($a1)        */
        ".word 0x48640044\n"  /* mfv      $a0, S102              */
        ".word 0x44846000\n"  /* mtc1     $a0, $f12              */
        ".word 0xe4ac0020\n"  /* swc1     $f12, 32($a1)          */
        ::: "memory"
    );
    *(v4sf_t *)ray = *(v4sf_t *)&p1;
}
#pragma control sched=2
