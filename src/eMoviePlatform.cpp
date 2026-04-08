#include "cFileSystemPlatform.h"

int eMoviePlatform::dispbuf_getCapacity(void) {
    return dispbuf_bufEnd - dispbuf_buf;
}

void eMoviePlatform::dispbuf_setPts(unsigned int pts) {
    dispbuf_pts[dispbuf_writeIdx] = pts;
}

int eMoviePlatform::dispbuf_getPts(void) {
    return dispbuf_pts[dispbuf_readIdx];
}

int eMoviePlatform::soundbuf_getCapacity(void) {
    return soundbuf_bufEnd - soundbuf_buf;
}

int eMoviePlatform::soundbuf_getPts(void) {
    return soundbuf_pts[soundbuf_readIdx];
}
