// gcBackgroundLoader::PreLoad(void)  @ 0x000f804c
// Split-TU: local declarations only; symbol __0fSgcBackgroundLoaderHPreLoadv

class gcRegion;

template <class T>
struct cGUIDT {
    int mA;
    int mB;
};

extern "C" void *memset(void *, int, unsigned int);

class gcBackgroundLoader {
public:
    char pad_00[0x0C];                  // 0x000
    int m0C;                            // 0x00C
    char m10;                           // 0x010
    char m11;                           // 0x011
    char pad_12[0x14 - 0x12];           // 0x012
    cGUIDT<gcRegion> m14;               // 0x014
    cGUIDT<gcRegion> m1C;               // 0x01C
    char m24[0x10];                     // 0x024
    cGUIDT<gcRegion> m34;               // 0x034
    char m3C[0x7D0];                    // 0x03C
    char pad_80C[0x80C - (0x3C + 0x7D0)]; // 0x80C
    int m80C;                           // 0x80C
    char m810[8];                       // 0x810
    int m818;                           // 0x818
    int m81C;                           // 0x81C
    int m820;                           // 0x820
    char m824[8];                       // 0x824
    char m82C;                          // 0x82C
    char m82D;                          // 0x82D

    void PreLoad(void);
};

void gcBackgroundLoader::PreLoad(void) {
    m11 = 0;
    {
        int *p = &m14.mA;
        p[0] = 0;
        p[1] = 0;
    }
    {
        int *p = &m1C.mA;
        p[0] = 0;
        p[1] = 0;
    }
    {
        int *p = &m34.mA;
        p[0] = 0;
        p[1] = 0;
    }
    m80C = 0;
    m818 = 0;
    m81C = 0;
    m0C = 0;
    m82C = 0;
    m10 = 0;
    m82D = 0;
    m820 = 0;
    memset(m810, 0, 8);
    memset(m24, 0, 0x10);
    memset(m824, 0, 8);
    memset(m3C, 0, 0x7D0);
}
