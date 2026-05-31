// eMoviePlatform::decode_audioDecode(void)  @ 0x00011f80, eAll_psp.obj
// symbol: __0fOeMoviePlatformSdecode_audioDecodev

extern "C" {
    int sceMpegGetAtracAu(void *, int, void *, void *);
    int sceMpegAtracDecode(void *, void *, int, int);
}

class eMoviePlatform {
public:
    int  decode_audioDecode();
    int  soundbuf_getCapacity();
    int  soundbuf_getDrawbuf();
    void soundbuf_setBuf();
    void soundbuf_setPts(unsigned int);

    char           pad0[12];       // 0x000
    char           m_mpeg[0x1F8 - 12];  // 0x00C  mpeg ringbuffer (this+0xC)
    int            m_1F8;          // 0x1F8
    char           pad2[0x240 - 0x1F8 - 4];
    char           m_au[0x244 - 0x240];  // 0x240  au info buffer (this+0x240)
    unsigned int   m_244;          // 0x244
    char           pad3[0x290 - 0x244 - 4];
    int            m_290;          // 0x290
};

int eMoviePlatform::decode_audioDecode()
{
    int auInfo;

    if (soundbuf_getCapacity() == 0)
        return 1;

    int rc = sceMpegGetAtracAu(m_mpeg, m_1F8, m_au, &auInfo);
    if (rc != 0)
        return rc;

    int rc2 = sceMpegAtracDecode(m_mpeg, m_au, soundbuf_getDrawbuf(), m_290);
    if (rc2 != 0)
        return rc2;

    soundbuf_setPts(m_244);
    if (m_290 == 0)
        soundbuf_setBuf();
    return 0;
}
