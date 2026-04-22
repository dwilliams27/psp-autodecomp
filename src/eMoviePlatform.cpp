#include "thread.h"

class eMoviePlatform {
public:
    char _pad_00[0x2B0];
    int control_eventFlag;             // 0x2B0
    char _pad_2B4[0x2D8 - 0x2B4];      // 0x2B4 - 0x2D8
    unsigned int m_dispbuf_pts[2];     // 0x2D8
    unsigned int m_dispbuf_writeIdx;   // 0x2E0
    unsigned int m_dispbuf_readIdx;    // 0x2E4
    int m_dispbuf_start;               // 0x2E8
    int m_dispbuf_end;                 // 0x2EC
    char _pad_2F0[0x318 - 0x2F0];      // 0x2F0 - 0x318
    unsigned int m_soundbuf_pts[2];    // 0x318
    char _pad_320[0x32C - 0x320];      // 0x320 - 0x32C
    unsigned int m_soundbuf_readIdx;   // 0x32C
    int m_soundbuf_start;              // 0x330
    int m_soundbuf_end;                // 0x334
    char _pad_338[0x358 - 0x338];      // 0x338 - 0x358
    int m_avsync_video_buf[5];         // 0x358
    int m_avsync_video_readIdx;        // 0x36C
    int m_avsync_video_count;          // 0x370
    int m_avsync_video_size;           // 0x374
    char _pad_378[0x38C - 0x378];      // 0x378 - 0x38C
    int m_read_state;                  // 0x38C

    int dispbuf_getCapacity(void);
    int soundbuf_getCapacity(void);
    void dispbuf_setPts(unsigned int);
    unsigned int dispbuf_getPts(void);
    unsigned int soundbuf_getPts(void);
    int dispbuf_getDrawbuf(void);
    int dispbuf_checkDecodeEnd(void);
    int soundbuf_getDrawbuf(void);

    void avsync_delete(void);
    int avsync_video_getPts(void);
    void avsync_video_setPts(unsigned int);
    static void OnSuspend(void *);
    void Close(bool);
    void control_setCondition(unsigned int);
    int control_getCondition(void);
    void read_checkConditionEnd(void);
    int read_isEnd(void);
    int read_getCapacity(void);
    int read_isFull(void);
};

int eMoviePlatform::dispbuf_getCapacity(void) {
    return m_dispbuf_end - m_dispbuf_start;
}

int eMoviePlatform::soundbuf_getCapacity(void) {
    return m_soundbuf_end - m_soundbuf_start;
}

void eMoviePlatform::dispbuf_setPts(unsigned int pts) {
    m_dispbuf_pts[m_dispbuf_writeIdx] = pts;
}

unsigned int eMoviePlatform::dispbuf_getPts(void) {
    return m_dispbuf_pts[m_dispbuf_readIdx];
}

unsigned int eMoviePlatform::soundbuf_getPts(void) {
    return m_soundbuf_pts[m_soundbuf_readIdx];
}

void eMoviePlatform::avsync_delete(void) {
    for (int i = 0; i < m_avsync_video_size; i++) {
        m_avsync_video_buf[i] = 0;
    }
}

int eMoviePlatform::avsync_video_getPts(void) {
    int idx = m_avsync_video_readIdx;
    int size = m_avsync_video_size;
    int newIdx = (idx + 1) % size;
    int result = m_avsync_video_buf[idx];
    m_avsync_video_count = m_avsync_video_count - 1;
    m_avsync_video_readIdx = newIdx;
    return result;
}

void eMoviePlatform::OnSuspend(void *arg) {
    ((eMoviePlatform *)arg)->Close(false);
}

void eMoviePlatform::control_setCondition(unsigned int bits) {
    sceKernelSetEventFlag(control_eventFlag, bits);
}

void eMoviePlatform::read_checkConditionEnd(void) {
    if ((control_getCondition() & 0xff) == 0xff) {
        m_read_state = 3;
    }
}

int eMoviePlatform::dispbuf_getDrawbuf(void) {
    int result = 0;
    if (dispbuf_getCapacity() > 0) {
        int idx = *(int *)((char *)this + 0x2E0);
        result = *(int *)((char *)this + idx * 4 + 0x2D0);
    }
    return result;
}

int eMoviePlatform::dispbuf_checkDecodeEnd(void) {
    if (control_getCondition() & 8) {
        *(int *)((char *)this + 0x2F4) = 0xFF;
        return 1;
    }
    return 0;
}

int eMoviePlatform::soundbuf_getDrawbuf(void) {
    int result = 0;
    if (soundbuf_getCapacity() > 0) {
        int idx = *(int *)((char *)this + 0x328);
        result = *(int *)((char *)this + idx * 4 + 0x308);
    }
    return result;
}

void eMoviePlatform::avsync_video_setPts(unsigned int pts) {
    if (pts == (unsigned int)-1) {
        pts = *(unsigned int *)((char *)this + 0x378) + *(unsigned int *)((char *)this + 0x37C);
    }
    int idx = *(int *)((char *)this + 0x368);
    int size = *(int *)((char *)this + 0x374);
    *(unsigned int *)((char *)this + 0x378) = pts;
    int count = *(int *)((char *)this + 0x370);
    *(unsigned int *)((char *)this + idx * 4 + 0x358) = pts;
    *(int *)((char *)this + 0x370) = count + 1;
    *(int *)((char *)this + 0x368) = (idx + 1) % size;
}

int eMoviePlatform::read_isFull(void) {
    control_getCondition();
    if (read_isEnd()) {
        return 1;
    }
    return read_getCapacity() == 0;
}
