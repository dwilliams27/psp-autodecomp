struct mPlane {
    float x, y, z, w;
};

struct __attribute__((aligned(16))) mFrustum {
    mPlane planes[6];
    void SetFOV(float, float, float, float);
};

struct eViewport {
    int x, y, w, h;
};

class eCamera;

class cTimeValue {
public:
    float mTime;
};

extern void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
extern void cHandleT_eCameraEffect_ctor(void *);
extern void eCameraEffectMgrEntry_ctor(void *);

class eCamera {
public:
    void Update(cTimeValue);
    float GetOrthoZoom(void) const;
    void SetFOV(float fov, float near, float far);
    void SetViewport(const eViewport *vp, bool adjustFrustum);
    eCamera(void);

    void SetFrustum(const mFrustum &f);
    void GetAdjustedFOV(float fov, float *out1, float *out2, float aspect) const;
    void AdjustFrustum(void);
    void Reset(void);

    static void GetMaxWidthHeight(int *w, int *h);
};

void eCamera::Update(cTimeValue) {
    *(int *)this &= ~8;
}

float eCamera::GetOrthoZoom(void) const {
    char *base = (char *)this + 0x20;
    mPlane *p3 = (mPlane *)(base + 0x30);
    mPlane *p2 = (mPlane *)(base + 0x20);
    float h = -(p3->w + p2->w);
    mPlane *p4 = (mPlane *)(base + 0x40);
    mPlane *p5 = (mPlane *)(base + 0x50);
    float w = -(p4->w + p5->w);
    if (!(h <= w)) {
        w = h;
    }
    return w;
}

void eCamera::SetFOV(float fov, float near, float far) {
    float adj_h, adj_v;
    GetAdjustedFOV(fov, &adj_h, &adj_v, near);
    mFrustum localFrustum;
    localFrustum.SetFOV(adj_h, adj_v, near, far);
    SetFrustum(localFrustum);
}

void eCamera::SetViewport(const eViewport *vp, bool adjustFrustum) {
    if (vp == 0) {
        *(int *)((char *)this + 4) = 0;
        *(int *)((char *)this + 8) = 0;
        GetMaxWidthHeight((int *)((char *)this + 0xC), (int *)((char *)this + 0x10));
    } else {
        *(eViewport *)((char *)this + 4) = *vp;
    }
    if (adjustFrustum) {
        AdjustFrustum();
    }
}

eCamera::eCamera(void) {
    *(int *)((char *)this + 0x1E8) = 0;
    *(int *)((char *)this + 0x250) = 0;
    __vec_new((char *)this + 0x25C, 4, 4, cHandleT_eCameraEffect_ctor);
    __vec_new((char *)this + 0x26C, 8, 0x14, eCameraEffectMgrEntry_ctor);
    __asm__ volatile(
        ".word 0xf3838080\n"  /* vmidt.q M000 */
        ".word 0xd0008086\n"  /* vmov.q C120, C000 */
        ".word 0xd0008187\n"  /* vmov.q C130, C010 */
        ".word 0xd0008288\n"  /* vmov.q C200, C020 */
        ".word 0xd0008389\n"  /* vmov.q C210, C030 */
        ".word 0xfa0600e0\n"  /* sv.q C120, 0xE0(s0) */
        ".word 0xfa0700f0\n"  /* sv.q C130, 0xF0(s0) */
        ".word 0xfa080100\n"  /* sv.q C200, 0x100(s0) */
        ".word 0xfa090110\n"  /* sv.q C210, 0x110(s0) */
    );
    Reset();
}
