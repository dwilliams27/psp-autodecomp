#include "eVideo.h"

struct eViewport {
    int x, y, w, h;
};

extern eViewport D_00041010;

void eVideo::SetFullscreenViewport(const eViewport &vp) {
    D_00041010 = vp;
}
