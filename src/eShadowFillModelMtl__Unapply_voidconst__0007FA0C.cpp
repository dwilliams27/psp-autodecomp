// eShadowFillModelMtl::Unapply(void) const  @ 0x0007fa0c
// symbol: __0fTeShadowFillModelMtlHUnapplyvK

class eShadowFillModelMtl {
public:
    void Unapply(void) const;
};

struct RenderState {
    char pad[0x20];
    int field_20;
};

extern RenderState D_00098428;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

void eShadowFillModelMtl::Unapply(void) const {
    int f = D_00098428.field_20;
    if (f != 0) {
        int cmd = 0x24000000;
        __asm__ volatile("" : "+r"(cmd));
        int *p = D_000984D0.ptr;
        D_000984D0.ptr = p + 1;
        *p = cmd;
        f = 0;
        D_00098428.field_20 = f;
    }
}
