#include "eVideo.h"

struct eViewport {
    int x, y, w, h;
};

extern eViewport D_00098358;

void eVideo::GetViewport(eViewport *vp) {
    *vp = D_00098358;
}
