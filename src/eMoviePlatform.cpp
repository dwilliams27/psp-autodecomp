#include "cFileSystemPlatform.h"
#include "thread.h"

void eMoviePlatform::OnSuspend(void *ctx) {
    ((eMoviePlatform *)ctx)->Close(false);
}

void eMoviePlatform::control_setCondition(unsigned int cond) {
    sceKernelSetEventFlag(control_eventFlag, cond);
}

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

void eMoviePlatform::soundbuf_setPts(unsigned int pts) {
    if (pts == (unsigned int)-1) {
        pts = soundbuf_lastPts + soundbuf_delta;
    }
    soundbuf_lastPts = pts;
    soundbuf_pts[soundbuf_writeIdx] = pts;
}

int eMoviePlatform::soundbuf_getPts(void) {
    return soundbuf_pts[soundbuf_readIdx];
}

void eMoviePlatform::avsync_delete(void) {
    int i = 0;
    int n = avsync_count;
    if (i < n) {
        do {
            avsync_pts[i] = 0;
            i++;
        } while (i < n);
    }
}

int eMoviePlatform::avsync_video_getPts(void) {
    int idx = avsync_readIdx;
    int cnt = avsync_count;
    int next = (idx + 1) % cnt;
    int pts = avsync_pts[idx];
    avsync_pending = avsync_pending - 1;
    avsync_readIdx = next;
    return pts;
}
