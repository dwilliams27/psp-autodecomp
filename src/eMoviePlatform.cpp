class eMoviePlatform {
public:
    char _pad0[0x2D8];
    unsigned int m_dispbuf_pts[2];     // 0x2D8
    unsigned int m_dispbuf_writeIdx;   // 0x2E0
    unsigned int m_dispbuf_readIdx;    // 0x2E4
    int m_dispbuf_start;               // 0x2E8
    int m_dispbuf_end;                 // 0x2EC
    char _pad1[0x318 - 0x2F0];         // 0x2F0 - 0x318
    unsigned int m_soundbuf_pts[2];    // 0x318
    char _pad2[0x32C - 0x320];         // 0x320 - 0x32C
    unsigned int m_soundbuf_readIdx;   // 0x32C
    int m_soundbuf_start;              // 0x330
    int m_soundbuf_end;                // 0x334

    int dispbuf_getCapacity(void);
    int soundbuf_getCapacity(void);
    void dispbuf_setPts(unsigned int);
    unsigned int dispbuf_getPts(void);
    unsigned int soundbuf_getPts(void);
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
