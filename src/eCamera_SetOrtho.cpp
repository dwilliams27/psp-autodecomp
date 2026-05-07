// ODR-WARNING: split translation unit local redeclaration for eCamera::SetOrtho only.

struct mPlane {
    float x, y, z, w;
};

struct __attribute__((aligned(16))) mFrustum {
    mPlane planes[6];
    void SetOrtho(float, float, float, float, float, float);
};

class eCamera {
public:
    void SetOrtho(float zoom);
    void SetFrustum(const mFrustum &f);
};

void eCamera::SetOrtho(float zoom) {
    int pad[4];
    mFrustum localFrustum;
    mFrustum *frustum = &localFrustum;
    char *base = (char *)this + 0x20;
    float nearPlane = ((mPlane *)base)->w;
    base += 0x10;
    float farPlane = -((mPlane *)base)->w;
    int flags = *(int *)this;
    int width = *(int *)((char *)this + 0x0C);
    int height = *(int *)((char *)this + 0x10);
    float fwidth = (float)width;
    float negHalf = -0.5f;
    float posHalf = 0.5f;
    float fheight = (float)height;
    if (flags & 1) {
        localFrustum.SetOrtho(nearPlane, farPlane,
                          fwidth * negHalf, fwidth * posHalf,
                          fheight * posHalf, fheight * negHalf);
    } else {
        float aspect = fwidth / fheight;
        float negZoom = zoom * -0.5f;
        float posZoom = zoom * 0.5f;
        if (!(aspect <= 1.0f)) {
            float posScale = posHalf / aspect;
            float negScale = negHalf / aspect;
            localFrustum.SetOrtho(nearPlane, farPlane,
                              negZoom, posZoom,
                              posScale * zoom, negScale * zoom);
        } else {
            float z = zoom;
            float negScale = aspect * negHalf;
            float posScale = aspect * posHalf;
            localFrustum.SetOrtho(nearPlane, farPlane,
                                  negScale * z, posScale * z,
                              posZoom, negZoom);
        }
    }
    SetFrustum(*frustum);
    __asm__ volatile("" : : "m"(pad));
}
