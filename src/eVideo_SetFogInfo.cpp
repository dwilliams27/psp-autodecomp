// ODR-WARNING: split TU local redeclarations for one eVideo method.
// Do not include eVideo.h here; adding this static method to the shared header
// can perturb already-matched eVideo siblings.

class eCamera;

class eVideo {
public:
    static void SetFogInfo(const eCamera *);
};

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

struct eVideoStateFog {
    char pad[0x5C];
    int fogEnabled;
};

extern eVideoStateFog D_00098428;
extern GpuCmdList D_000984D0;
extern int D_00098398;
extern float D_0037D364;
extern float D_0037D368;

void eVideo::SetFogInfo(const eCamera *camera) {
    volatile int sp0;
    volatile int spC;
    volatile unsigned char sp10;
    volatile unsigned char sp11;
    volatile unsigned char sp12;
    volatile unsigned char sp13;
    volatile float sp18;
    volatile float sp1C;

    register int enabled asm("a2");
    __asm__ volatile("li %0, 0" : "=r"(enabled));
    if (camera != 0) {
        register int flags asm("a3") = *(int *)camera;
        flags &= 2;
        if (flags != 0) {
            enabled = 1;
        }
    }
    enabled = (unsigned char)enabled;

    if (enabled != 0) {
        sp0 = 0;
        sp0 = *(int *)((const char *)camera + 0x1E8);
        float start = *(float *)((const char *)camera + 0x1EC);
        float end = *(float *)((const char *)camera + 0x1F0);

        if (D_0037D364 != start || D_0037D368 != end || sp0 != D_00098398) {
            float range = end - start;
            float scale = 0.0f;
            if (range != 0.0f) {
                scale = 1.0f / range;
            }

            sp13 = ((volatile unsigned char *)&sp0)[3] & 0xff;
            sp12 = ((volatile unsigned char *)&sp0)[0] & 0xff;
            sp11 = ((volatile unsigned char *)&sp0)[1] & 0xff;
            sp10 = ((volatile unsigned char *)&sp0)[2] & 0xff;
            sp18 = -end;
            spC = *(volatile int *)&sp10;
            int packed = spC;
            packed = (packed & 0x00FFFFFF) | 0xCF000000;
            float negScale = -scale;
            int *p = D_000984D0.ptr;
            D_000984D0.ptr = p + 3;
            p[0] = packed;
            sp1C = negScale;
            p[1] = ((*(volatile unsigned int *)&sp18) >> 8) | 0xCD000000;
            p[2] = ((*(volatile unsigned int *)&sp1C) >> 8) | 0xCE000000;

            D_0037D364 = start;
            D_0037D368 = end;
            D_00098398 = sp0;
        }
    }

    if (enabled != D_00098428.fogEnabled) {
        int *p = D_000984D0.ptr;
        D_000984D0.ptr = p + 1;
        *p = enabled | 0x1F000000;
        D_00098428.fogEnabled = enabled;
    }
}
