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

typedef int v4sf_t __attribute__((mode(V4SF)));

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
    eCamera &operator=(const eCamera &);

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

eCamera &eCamera::operator=(const eCamera &other) {
    int word0 = *(const int *)((const char *)&other + 0x00);
    const int *srcWords = (const int *)((const char *)&other + 0x04);
    int *dstWords = (int *)((char *)this + 0x04);
    *(int *)((char *)this + 0x00) = word0;
    int word1 = srcWords[0];
    int word2 = srcWords[1];
    int word3 = srcWords[2];
    dstWords[0] = word1;
    int word4 = srcWords[3];
    dstWords[1] = word2;
    dstWords[2] = word3;
    dstWords[3] = word4;

    unsigned int i = 0;
    char *dst = (char *)this + 0x20;
    const char *src = (const char *)&other + 0x20;
    do {
        float w = ((const mPlane *)src)->w;
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        ((mPlane *)dst)->w = w;
        dst += 0x10;
        src += 0x10;
    } while (i < 6);

    i = 0;
    dst = (char *)this + 0x80;
    src = (const char *)&other + 0x80;
    do {
        float w = ((const mPlane *)src)->w;
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        ((mPlane *)dst)->w = w;
        dst += 0x10;
        src += 0x10;
    } while (i < 6);

    *(v4sf_t *)((char *)this + 0x110) =
        *(const v4sf_t *)((const char *)&other + 0x110);
    i = 0;
    *(v4sf_t *)((char *)this + 0x0E0) =
        *(const v4sf_t *)((const char *)&other + 0x0E0);
    dst = (char *)this + 0x120;
    *(v4sf_t *)((char *)this + 0x0F0) =
        *(const v4sf_t *)((const char *)&other + 0x0F0);
    src = (const char *)&other + 0x120;
    *(v4sf_t *)((char *)this + 0x100) =
        *(const v4sf_t *)((const char *)&other + 0x100);
    do {
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        dst += 0x10;
        src += 0x10;
    } while (i < 4);

    i = 0;
    dst = (char *)this + 0x160;
    src = (const char *)&other + 0x160;
    do {
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        dst += 0x10;
        src += 0x10;
    } while (i < 4);

    i = 0;
    dst = (char *)this + 0x1A0;
    src = (const char *)&other + 0x1A0;
    do {
        *(v4sf_t *)dst = *(const v4sf_t *)src;
        i++;
        __asm__ volatile("" ::: "memory");
        dst += 0x10;
        src += 0x10;
    } while (i < 4);

    const int *src1E8 = (const int *)((const char *)&other + 0x1E8);
    float value1E0 = *(const float *)((const char *)&other + 0x1E0);
    *(float *)((char *)this + 0x1E0) = value1E0;
    float value1E4 = *(const float *)((const char *)&other + 0x1E4);
    int *dst1E8 = (int *)((char *)this + 0x1E8);
    *(float *)((char *)this + 0x1E4) = value1E4;
    *dst1E8 = *src1E8;
    *(float *)((char *)this + 0x1EC) =
        *(const float *)((const char *)&other + 0x1EC);
    *(float *)((char *)this + 0x1F0) =
        *(const float *)((const char *)&other + 0x1F0);
    *(float *)((char *)this + 0x1F4) =
        *(const float *)((const char *)&other + 0x1F4);
    __asm__ volatile("" ::: "memory");
    *(v4sf_t *)((char *)this + 0x200) =
        *(const v4sf_t *)((const char *)&other + 0x200);
    *(v4sf_t *)((char *)this + 0x210) =
        *(const v4sf_t *)((const char *)&other + 0x210);
    *(v4sf_t *)((char *)this + 0x220) =
        *(const v4sf_t *)((const char *)&other + 0x220);
    float value230 = *(const float *)((const char *)&other + 0x230);
    *(float *)((char *)this + 0x230) = value230;
    const char *src250 = (const char *)&other;
    int value234 = *(const int *)((const char *)&other + 0x234);
    src250 += 0x250;
    *(int *)((char *)this + 0x234) = value234;
    *(v4sf_t *)((char *)this + 0x240) =
        *(const v4sf_t *)((const char *)&other + 0x240);
    int *dst250 = (int *)((char *)this + 0x250);
    int value250 = *(const int *)src250;
    *dst250 = value250;
    int value254 = *(const int *)((const char *)&other + 0x254);
    *(int *)((char *)this + 0x254) = value254;
    int value258 = *(const int *)((const char *)&other + 0x258);
    char *dstBase = (char *)this + 0x25C;
    const char *srcBase = (const char *)&other + 0x25C;
    *(int *)((char *)this + 0x258) = value258;

    int offset = 0;
    int count = 0x16;
    do {
        const int *srcPair = (const int *)((int)srcBase + offset);
        int pair0 = srcPair[0];
        int pair1 = srcPair[1];
        int *dstPair = (int *)(dstBase + offset);
        dstPair[0] = pair0;
        dstPair[1] = pair1;
        count--;
        offset += 8;
    } while (count != 0);

    return *this;
}
