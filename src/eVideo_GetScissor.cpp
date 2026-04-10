#include "eVideo.h"

struct eViewport {
    int x, y, w, h;
};

extern eViewport D_00098368;

void eVideo::GetScissor(eViewport *vp) {
    *vp = D_00098368;
}
