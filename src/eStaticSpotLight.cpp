// Mixed small functions routed into this TU by the orchestrator.
// Functions:
//   0x0005f788 eStaticSpotLight::eStaticSpotLight(cBase *)        eAll_psp.obj
//   0x0005f9ac eStaticSpotLight::GetSampleRay(mRay *, mVec3 *, const mVec3 &, const mVec3 &) const eAll_psp.obj
//   0x000d4d4c gcNamedSet::Write(cFile &) const                   gcAll_psp.obj
//   0x00155eb0 gcRigidBodyControllerTemplate::gcRigidBodyControllerTemplate(cBase *) gcAll_psp.obj

class cBase;
class cFile;
class mRay;
class mVec3;

typedef int v4sf_t __attribute__((mode(V4SF)));

// ============================================================
// 0x0005f788 — eStaticSpotLight::eStaticSpotLight(cBase *)
// ============================================================

class eStaticLight {
public:
    eStaticLight(cBase *);
};

class eStaticSpotLight : public eStaticLight {
public:
    eStaticSpotLight(cBase *);
    void GetSampleRay(mRay *, mVec3 *, const mVec3 &, const mVec3 &) const;
};

extern char eStaticSpotLightvirtualtable[];

#pragma control sched=1
eStaticSpotLight::eStaticSpotLight(cBase *parent) : eStaticLight(parent) {
    *(void **)((char *)this + 4) = eStaticSpotLightvirtualtable;
    *(float *)((char *)this + 0x90) = 90.0f;
    __asm__ volatile("" ::: "memory");
}

void eStaticSpotLight::GetSampleRay(mRay *ray, mVec3 *, const mVec3 &p1, const mVec3 &) const {
    __asm__ volatile(
        ".word 0xd8860080\n"  /* lv.q   C120, 0x80($a0)        */
        ".word 0xd8e70000\n"  /* lv.q   C130, 0x0($a3)         */
        ".word 0x60878606\n"  /* vsub.t C120, C120, C130       */
        ".word 0xf8a60010\n"  /* sv.q   C120, 0x10($a1)        */
        ".word 0x64868604\n"  /* vdot.t S100, C120, C120       */
        ".word 0x6c000408\n"  /* vcmp.s ez, S100               */
        ".word 0xd0110424\n"  /* vrsq.s S101, S100             */
        ".word 0xd0160444\n"  /* vsqrt.s S102, S100            */
        ".word 0xdc0010e5\n"  /* vpfxs  1, Y, Z, W             */
        ".word 0xd2a00424\n"  /* vcmovt.s S101, S100, 0        */
        ".word 0x65248606\n"  /* vscl.t C120, C120, S101       */
        ".word 0xf8a60010\n"  /* sv.q   C120, 0x10($a1)        */
        ".word 0x48640044\n"  /* mfv    $a0, S102              */
        ".word 0x44846000\n"  /* mtc1   $a0, $f12              */
        ".word 0xe4ac0020\n"  /* swc1   $f12, 32($a1)          */
        ::: "memory"
    );
    *(v4sf_t *)ray = *(v4sf_t *)&p1;
}
#pragma control sched=2

// ============================================================
// 0x000d4d4c — gcNamedSet::Write(cFile &) const
// ============================================================

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcNamedSet {
public:
    cBase *m_parent;     // 0x00
    void *m_vtable;      // 0x04
    cHandle mName;       // 0x08
    void Write(cFile &) const;
};

void gcNamedSet::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    mName.Write(wb);
    wb.End();
}

// ============================================================
// 0x00155eb0 — gcRigidBodyControllerTemplate::gcRigidBodyControllerTemplate(cBase *)
// ============================================================

class gcEntityControllerTemplate {
public:
    gcEntityControllerTemplate(cBase *);
};

class gcRigidBodyControllerTemplate : public gcEntityControllerTemplate {
public:
    gcRigidBodyControllerTemplate(cBase *);
};

extern char gcRigidBodyControllerTemplatevirtualtable[];

gcRigidBodyControllerTemplate::gcRigidBodyControllerTemplate(cBase *parent)
    : gcEntityControllerTemplate(parent) {
    *(void **)((char *)this + 4) = gcRigidBodyControllerTemplatevirtualtable;
    *(int *)((char *)this + 0x18) |= 1;
}
