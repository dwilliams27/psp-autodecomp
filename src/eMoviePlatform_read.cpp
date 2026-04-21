#include <libmpeg.h>

class eMoviePlatform {
public:
    char _pad0[0x10];
    SceMpegRingbuffer read_ringbuf;  // 0x10

    int read_getCapacity(void);
    void read_sendErrCondition(void);
    void read_sendEndCondition(void);
    void control_setCondition(unsigned int);
};

int eMoviePlatform::read_getCapacity(void) {
    return sceMpegRingbufferAvailableSize(&read_ringbuf);
}

void eMoviePlatform::read_sendErrCondition(void) {
    control_setCondition(0xC00);
}

void eMoviePlatform::read_sendEndCondition(void) {
    control_setCondition(4);
}
