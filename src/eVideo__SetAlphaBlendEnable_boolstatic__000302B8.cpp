#include "eVideo.h"

extern int D_00098428;

struct GpuCmdList {
    int pad[2];
    int *ptr;
};

extern GpuCmdList D_000984D0;

void eVideo::SetAlphaBlendEnable(bool enable) {
    unsigned int e = enable & 0xff;
    if (e != D_00098428) {
        int v = e | 0x21000000;
        int *p = D_000984D0.ptr;
        D_000984D0.ptr = p + 1;
        *p = v;
        D_00098428 = e;
    }
}
