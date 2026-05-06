#include "thread.h"
#include <displaysvc.h>
#include <utility/utility_module.h>

extern "C" void __0oNcFilePlatformctv(void *);
extern "C" void *memset(void *, int, unsigned int);

class eMoviePlatform {
public:
    int m_workAreaBase;                // 0x000
    int m_workAreaSize;                // 0x004
    int m_workAreaCurrent;             // 0x008
    char _pad_00C[0x2B0 - 0x00C];
    int control_eventFlag;             // 0x2B0
    char _pad_2B4[0x2C4 - 0x2B4];      // 0x2B4 - 0x2C4
    int m_dispbuf_sema;                // 0x2C4
    int m_dispbuf_eventFlag;           // 0x2C8
    char _pad_2CC[0x2D0 - 0x2CC];      // 0x2CC - 0x2D0
    int m_dispbuf_buf[2];              // 0x2D0
    unsigned int m_dispbuf_pts[2];     // 0x2D8
    int m_dispbuf_writeIdx;            // 0x2E0
    int m_dispbuf_readIdx;             // 0x2E4
    int m_dispbuf_start;               // 0x2E8
    int m_dispbuf_end;                 // 0x2EC
    char _pad_2F0[0x304 - 0x2F0];      // 0x2F0 - 0x304
    int m_soundbuf_sema;               // 0x304
    int m_soundbuf_buf[4];             // 0x308
    unsigned int m_soundbuf_pts[4];    // 0x318
    int m_soundbuf_writeIdx;           // 0x328
    int m_soundbuf_readIdx;            // 0x32C
    int m_soundbuf_start;              // 0x330
    int m_soundbuf_end;                // 0x334
    char _pad_338[0x358 - 0x338];      // 0x338 - 0x358
    int m_avsync_video_buf[5];         // 0x358
    int m_avsync_video_readIdx;        // 0x36C
    int m_avsync_video_count;          // 0x370
    int m_avsync_video_size;           // 0x374
    char _pad_378[0x38C - 0x378];      // 0x378 - 0x38C
    int m_read_state;                  // 0x38C

    eMoviePlatform(void);
    int dispbuf_getCapacity(void);
    int startCheck(void);
    int dispbuf_delete(void);
    int checkDecodeEnd(void);
    int soundbuf_getCapacity(void);
    void dispbuf_setPts(unsigned int);
    unsigned int dispbuf_getPts(void);
    unsigned int soundbuf_getPts(void);
    int dispbuf_getDrawbuf(void);
    void dispbuf_dataSet(void);
    int dispbuf_checkDecodeEnd(void);
    int soundbuf_getDrawbuf(void);
    int GetWorkAreaFreeSize(int);
    int AllocWorkArea(int, int);
    int dispbuf_show(void);
    void dispbuf_DrawFrame(char *);
    void control_waitDisp(void);
    int soundbuf_setBuf(void);
    void soundbuf_delete(void);
    int read_create(void);

    int avsync_create(void);
    void avsync_delete(void);
    int avsync_video_getPts(void);
    void avsync_video_setPts(unsigned int);
    static void OnSuspend(void *);
    void control_delete(void);
    void Close(bool);
    void control_setCondition(unsigned int);
    int control_getCondition(void);
    void read_checkConditionEnd(void);
    int read_isEnd(void);
    int read_getCapacity(void);
    int read_isFull(void);
    static int read_func(unsigned int, void *);
    static bool Initialize(void);
};

class cVolatile {
public:
    static int s_pBase;

    static int GetFreeSize(int);
    static int Alloc(unsigned int, int);
};

class eVideo {
public:
    static void PreFlip(void);
};

class eVideoPlatform {
public:
    static void Flip(bool, bool);
};

#pragma control sched=1

eMoviePlatform::eMoviePlatform(void) {
    *(int *)((char *)this + 0x000) = 0;
    *(int *)((char *)this + 0x004) = 0;
    *(int *)((char *)this + 0x008) = 0;
    __0oNcFilePlatformctv((char *)this + 0x090);

    *(unsigned char *)((char *)this + 0x1B0) = 0;
    *(int *)((char *)this + 0x1BC) = 0;
    *(unsigned char *)((char *)this + 0x1C0) = 0;
    *(int *)((char *)this + 0x1B8) = 0;
    *(int *)((char *)this + 0x1B4) = 0;
    __asm__ volatile("" ::: "memory");
    int neg = -1;
    *(int *)((char *)this + 0x1C4) = neg;
    *(int *)((char *)this + 0x1C8) = neg;
    *(int *)((char *)this + 0x1CC) = neg;
    *(int *)((char *)this + 0x1D0) = 0;
    *(int *)((char *)this + 0x1D4) = 0;
    *(int *)((char *)this + 0x1D8) = 0;
    *(int *)((char *)this + 0x1DC) = 0;
    *(int *)((char *)this + 0x1E0) = 0;
    *(int *)((char *)this + 0x1E4) = 0;
    *(int *)((char *)this + 0x1E8) = 0;
    *(int *)((char *)this + 0x1EC) = 0;
    *(int *)((char *)this + 0x1F0) = 0;
    *(int *)((char *)this + 0x1FC) = 0;
    *(int *)((char *)this + 0x290) = 0;
    *(int *)((char *)this + 0x294) = 0;
    *(int *)((char *)this + 0x298) = 0;
    *(unsigned char *)((char *)this + 0x29C) = 0;
    *(int *)((char *)this + 0x2A0) = neg;
    *(int *)((char *)this + 0x2A4) = 0;
    *(unsigned char *)((char *)this + 0x2AC) = 0;
    *(int *)((char *)this + 0x2B0) = neg;
    *(int *)((char *)this + 0x2B4) = neg;
    *(int *)((char *)this + 0x2C0) = neg;
    *(int *)((char *)this + 0x2C4) = neg;
    *(int *)((char *)this + 0x2C8) = neg;
    *(int *)((char *)this + 0x2CC) = 0;
    *(int *)((char *)this + 0x2E0) = 0;
    *(int *)((char *)this + 0x2E4) = 0;
    *(int *)((char *)this + 0x2E8) = 0;
    *(int *)((char *)this + 0x2EC) = 0;
    *(int *)((char *)this + 0x2F0) = 0;
    *(int *)((char *)this + 0x2F4) = 0;
    *(int *)((char *)this + 0x2F8) = 0;
    *(int *)((char *)this + 0x2FC) = 0;
    *(int *)((char *)this + 0x300) = neg;
    *(int *)((char *)this + 0x304) = neg;
    *(int *)((char *)this + 0x328) = 0;
    *(int *)((char *)this + 0x32C) = 0;
    *(int *)((char *)this + 0x330) = 0;
    *(int *)((char *)this + 0x334) = 0;
    *(int *)((char *)this + 0x338) = 0;
    *(int *)((char *)this + 0x33C) = 0;
    *(int *)((char *)this + 0x340) = 0;
    *(int *)((char *)this + 0x344) = 0;
    *(int *)((char *)this + 0x348) = 0;
    *(int *)((char *)this + 0x34C) = 0;
    *(int *)((char *)this + 0x368) = 0;
    *(int *)((char *)this + 0x36C) = 0;
    *(int *)((char *)this + 0x370) = 0;
    *(int *)((char *)this + 0x374) = 0;
    *(int *)((char *)this + 0x378) = 0;
    *(int *)((char *)this + 0x37C) = 0;
    *(int *)((char *)this + 0x380) = neg;
    *(int *)((char *)this + 0x384) = 0;
    *(int *)((char *)this + 0x388) = 0;
    *(int *)((char *)this + 0x38C) = 0;

    memset((char *)this + 0x00C, 0, 0x004);
    memset((char *)this + 0x010, 0, 0x080);
    memset((char *)this + 0x1F4, 0, 0x008);
    memset((char *)this + 0x200, 0, 0x040);
    memset((char *)this + 0x240, 0, 0x040);
    memset((char *)this + 0x280, 0, 0x010);
    memset((char *)this + 0x2B8, 0, 0x008);
    memset((char *)this + 0x2D0, 0, 0x008);
    memset((char *)this + 0x2D8, 0, 0x008);
    memset((char *)this + 0x308, 0, 0x010);
    memset((char *)this + 0x318, 0, 0x010);
    memset((char *)this + 0x350, 0, 0x008);
    memset((char *)this + 0x358, 0, 0x010);
}

#pragma control sched=2

int eMoviePlatform::dispbuf_getCapacity(void) {
    return m_dispbuf_end - m_dispbuf_start;
}

int eMoviePlatform::soundbuf_getCapacity(void) {
    return m_soundbuf_end - m_soundbuf_start;
}

int eMoviePlatform::startCheck(void) {
    if (dispbuf_getCapacity() != 0) {
        goto check_decode;
    }

    if (*(int *)((char *)this + 0x294) != 0) {
        if (soundbuf_getCapacity() != 0) {
            goto check_decode;
        }
    }

    if (read_isFull() != 0) {
        goto set_condition;
    }

check_decode:
    if (checkDecodeEnd() != 0) {
set_condition:
        control_setCondition(1);
        return 0;
    }

    return -1;
}

int eMoviePlatform::dispbuf_delete(void) {
    int sema = m_dispbuf_sema;
    if (sema > 0) {
        sceKernelDeleteSema(sema);
        m_dispbuf_sema = -1;
    }

    int flag = m_dispbuf_eventFlag;
    if (flag > 0) {
        sceKernelDeleteEventFlag(flag);
        m_dispbuf_eventFlag = -1;
    }

    int i = 0;
    int size = m_dispbuf_end;
    for (; i < size; i++) {
        m_dispbuf_buf[i] = 0;
        m_dispbuf_pts[i] = 0;
    }
    return 0;
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

void eMoviePlatform::dispbuf_dataSet(void) {
    sceKernelWaitSema(m_dispbuf_sema, 1, 0);
    int next = (m_dispbuf_writeIdx + 1) % m_dispbuf_end;
    m_dispbuf_start = m_dispbuf_start + 1;
    m_dispbuf_writeIdx = next;
    sceKernelSignalSema(m_dispbuf_sema, 1);
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

int eMoviePlatform::GetWorkAreaFreeSize(int align) {
    int useVolatile = (cVolatile::s_pBase != 0) & 0xff;
    if (useVolatile != 0) {
        return cVolatile::GetFreeSize(align);
    }
    int base = m_workAreaBase;
    if (base == 0) {
        return 0;
    }
    int n = align;
    if (n < 4) {
        n = 4;
    }
    unsigned int un = (unsigned int)(n + 3) >> 2 << 2;
    unsigned int m1 = un - 1;
    unsigned int aligned = ((unsigned int)m_workAreaSize + m1) / un * un;
    return m_workAreaCurrent - (int)(aligned - (unsigned int)base);
}

int eMoviePlatform::AllocWorkArea(int align, int size) {
    int result = 0;
    int useVolatile = (cVolatile::s_pBase != 0) & 0xff;
    if (useVolatile == 0) {
        int n = align;
        if (n < 4) {
            n = 4;
        }
        unsigned int un = (unsigned int)(n + 3) >> 2 << 2;
        __asm__ volatile("" ::: "memory");
        unsigned int m1 = un - 1;
        unsigned int aligned = ((unsigned int)m1 + m_workAreaSize) / un * un;
        unsigned int newSize = aligned + (unsigned int)size;
        if ((unsigned int)((int)newSize - m_workAreaBase) <= (unsigned int)m_workAreaCurrent) {
            m_workAreaSize = (int)newSize;
            result = (int)aligned;
        }
        return result;
    }
    return cVolatile::Alloc((unsigned int)size, align);
}

int eMoviePlatform::dispbuf_show(void) {
    int state = *(int *)((char *)this + 0x2F4);
    if (state == 1 || state == 0xFF) {
        eVideo::PreFlip();
        dispbuf_DrawFrame((char *)m_dispbuf_buf[m_dispbuf_readIdx]);
        m_dispbuf_readIdx = (m_dispbuf_readIdx + 1) % m_dispbuf_end;
        control_waitDisp();
        eVideoPlatform::Flip(true, false);
    } else {
        m_dispbuf_readIdx = (m_dispbuf_readIdx + 1) % m_dispbuf_end;
    }
    return 0;
}

int eMoviePlatform::soundbuf_setBuf(void) {
    sceKernelWaitSema(m_soundbuf_sema, 1, 0);
    m_soundbuf_start = m_soundbuf_start + 1;
    sceKernelSignalSema(m_soundbuf_sema, 1);
    m_soundbuf_writeIdx = (m_soundbuf_writeIdx + 1) % m_soundbuf_end;
    return 0;
}

void eMoviePlatform::soundbuf_delete(void) {
    int sema = m_soundbuf_sema;
    if (sema > 0) {
        sceKernelDeleteSema(sema);
        m_soundbuf_sema = -1;
    }

    int i = 0;
    int size = m_soundbuf_end;
    for (; i < size; i++) {
        m_soundbuf_buf[i] = 0;
        m_soundbuf_pts[i] = 0;
    }

    for (int i = 0; i < 2; i++) {
        *(int *)((char *)this + i * 4 + 0x350) = 0;
    }
}

void eMoviePlatform::control_delete(void) {
    if (*(unsigned char *)((char *)this + 0x2AC) != 0) {
        sceDisplaySetVblankCallback(0, 0, 0);
        *(unsigned char *)((char *)this + 0x2AC) = 0;
    }

    int flag = *(int *)((char *)this + 0x2B0);
    if (flag > 0) {
        sceKernelDeleteEventFlag(flag);
        *(int *)((char *)this + 0x2B0) = -1;
    }

    flag = *(int *)((char *)this + 0x2B4);
    if (flag > 0) {
        sceKernelDeleteEventFlag(flag);
        *(int *)((char *)this + 0x2B4) = -1;
    }
}

int eMoviePlatform::avsync_create(void) {
    m_avsync_video_size = 4;
    int rate = (int)((90000.0f / sceDisplayGetFramePerSec()) * 2.0f);
    *(int *)((char *)this + 0x378) = 0;
    *(int *)((char *)this + 0x368) = 0;
    m_avsync_video_readIdx = 0;
    m_avsync_video_count = 0;
    *(int *)((char *)this + 0x37C) = rate;
    return 1;
}

#pragma control sched=1
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
#pragma control sched=2

int eMoviePlatform::read_isFull(void) {
    control_getCondition();
    if (read_isEnd()) {
        return 1;
    }
    return read_getCapacity() == 0;
}

int eMoviePlatform::read_create(void) {
    int thread = sceKernelCreateThread((const char *)0x36CFE0, read_func, 0x12, 0x2000, 0, 0);
    *(int *)((char *)this + 0x380) = thread;
    if (thread >= 0) {
        int state = 1;
        __asm__ volatile("" ::: "memory");
        int start = *(int *)((char *)this + 0x1E4);
        *(int *)((char *)this + 0x38C) = state;
        int end = *(int *)((char *)this + 0x1F0);
        *(int *)((char *)this + 0x388) = start;
        *(int *)((char *)this + 0x384) = end;
        return 1;
    }
    return 0;
}

bool eMoviePlatform::Initialize(void) {
    int module = sceUtilityLoadModule(0x303);
    *(int *)0x37D060 = module;
    if (module < 0) {
        return false;
    }

    int sema = sceKernelCreateSema((const char *)0x36CFEC, 0, 1, 1, 0);
    *(int *)0x37D05C = sema;
    if (sema < 0) {
        return false;
    }

    return true;
}
