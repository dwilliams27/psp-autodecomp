// Multiple small functions bundled here per orchestrator routing.
// eSweptTest::AddIgnoreGeom, eMovie::OnDraw, ePathPoint::ePathPoint,
// eSimpleMotor::Initialize, eCylinderShape::CastRay.

// ---------- Function 1: eSweptTest::AddIgnoreGeom ----------

class eDynamicGeom;

class eSweptTest {
public:
    int _pad0;                          // 0x00
    int count;                          // 0x04
    const eDynamicGeom *ignores[4];     // 0x08

    void AddIgnoreGeom(const eDynamicGeom *);
};

void eSweptTest::AddIgnoreGeom(const eDynamicGeom *geom) {
    if (count != 4) {
        ignores[count++] = geom;
    }
}

// ---------- Function 2: eMovie::OnDraw (static) ----------

extern "C" {
extern void (*eMovie_sDrawCallback)(void *);
extern void *eMovie_sDrawCallbackData;
}

extern "C" void eMovie_OnDraw(void) {
    if (eMovie_sDrawCallback != 0) {
        eMovie_sDrawCallback(eMovie_sDrawCallbackData);
    }
}

// ---------- Function 3: ePathPoint::ePathPoint() ----------

extern "C" void *ePathPoint_ctor(void *self) {
    __asm__ volatile(
        "mtc1 $zero, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mfc1 $a3, $f12\n"
        "mtv $a1, S120\n"
        "mtv $a2, S121\n"
        "mtv $a3, S122\n"
        "sv.q C120, 0x0($a0)\n"
        "swc1 $f12, 16($a0)\n"
        "swc1 $f12, 20($a0)\n"
    );
    return self;
}

// ---------- Function 4: eSimpleMotor::Initialize ----------

struct eSimpleMotor_layout {
    char _pad00[0x08];                  // 0x00
    void *config;                       // 0x08
    char _pad0C[0x08];                  // 0x0C
    unsigned char unk14;                // 0x14
    char _pad15[0x03];                  // 0x15
    void *ctrl;                         // 0x18
    char *unk1C;                        // 0x1C
};

struct ePhysicsMotorConfig_layout {
    char _pad00[0x38];                  // 0x00
    int unk38;                          // 0x38
    unsigned char unk3C;                // 0x3C
};

struct eSimulatedController_layout {
    char _pad00[0x38];                  // 0x00
    char *unk38;                        // 0x38
};

extern "C" void eSimpleMotor_Initialize(
    struct eSimpleMotor_layout *self,
    struct ePhysicsMotorConfig_layout *cfg,
    struct eSimulatedController_layout *ctrl)
{
    self->ctrl = ctrl;
    self->config = cfg;
    self->unk1C = ctrl->unk38 + cfg->unk38 * 48;
    self->unk14 = (unsigned char)cfg->unk3C;
}

// ---------- Function 5: eCylinderShape::CastRay ----------
// Forwards virtual dispatch to inner shape at +0x88.
// Inner vtable entry for CastRay is at offset 0x98 (method index 19).

class eCylinderShape_InnerShape {
public:
    int _data0;  // data member forces vtable to offset 0x04
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void CastRay(const void *, const void *, void *, void *, float *) const;
};

struct eCylinderShape_layout {
    char _pad00[0x88];
    eCylinderShape_InnerShape *mInner;  // 0x88
};

extern "C" void eCylinderShape_CastRay(
    const struct eCylinderShape_layout *self,
    const void *ray, const void *info,
    void *a, void *b, float *c)
{
    self->mInner->CastRay(ray, info, a, b, c);
}
