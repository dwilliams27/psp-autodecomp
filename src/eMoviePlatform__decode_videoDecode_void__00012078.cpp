// eMoviePlatform::decode_videoDecode(void)  @ 0x00012078, eAll_psp.obj
// symbol: __0fOeMoviePlatformSdecode_videoDecodev

extern "C" {
    int sceMpegGetAvcAu(void *, int, void *, void *);
    int sceMpegAvcDecodeYCbCr(void *, void *, void *, void *);
}

class eMoviePlatform {
public:
    int          decode_videoDecode();
    int          dispbuf_getCapacity();
    int          dispbuf_getDrawbuf();
    void         dispbuf_setPts(unsigned int);
    void         dispbuf_dataSet();
    void         avsync_video_setPts(unsigned int);
    unsigned int avsync_video_getPts();

    char           pad0[12];                  // 0x000
    char           m_mpeg[0x1F4 - 12];        // 0x00C  mpeg ringbuffer (this+0xC)
    int            m_1F4;                      // 0x1F4
    char           pad1[0x200 - 0x1F4 - 4];
    char           m_au[0x204 - 0x200];        // 0x200  au info buffer (this+0x200)
    unsigned int   m_204;                      // 0x204
    char           pad2[0x2B8 - 0x204 - 4];
    int            m_2B8;                      // 0x2B8  draw buffer
};

int eMoviePlatform::decode_videoDecode()
{
    int auInfo;
    int decodeStatus;

    if (dispbuf_getCapacity() == 0)
        return 1;

    int rc = sceMpegGetAvcAu(m_mpeg, m_1F4, m_au, &auInfo);
    if (rc != 0)
        return rc;

    m_2B8 = dispbuf_getDrawbuf();
    int rc2 = sceMpegAvcDecodeYCbCr(m_mpeg, m_au, &m_2B8, &decodeStatus);
    if (rc2 != 0)
        return rc2;

    avsync_video_setPts(m_204);
    if (decodeStatus == 1) {
        dispbuf_setPts(avsync_video_getPts());
        dispbuf_dataSet();
    }
    return 0;
}
