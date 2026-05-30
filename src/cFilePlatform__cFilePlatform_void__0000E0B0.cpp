// cFilePlatform::cFilePlatform(void) @ 0x0000E0B0
// Trivial ctor: zero 11 fields, one -1 store at offset 264, returns this.
// Symbol: __0oNcFilePlatformctv

class cFilePlatform {
public:
    char m_b0;          // 0x000
    char pad0[0xFF];    // 0x001..0x0FF
    int  m_100;         // 0x100
    int  m_104;         // 0x104
    int  m_108;         // 0x108
    char m_10C;         // 0x10C
    char m_10D;         // 0x10D
    char m_10E;         // 0x10E
    char pad1;          // 0x10F
    int  m_110;         // 0x110
    int  m_114;         // 0x114
    int  m_118;         // 0x118
    int  m_11C;         // 0x11C

    cFilePlatform();
};

cFilePlatform::cFilePlatform()
{
    m_b0  = 0;
    m_100 = 0;
    m_104 = 0;
    m_108 = -1;
    m_10C = 0;
    m_10D = 0;
    m_10E = 0;
    m_110 = 0;
    m_114 = 0;
    m_118 = 0;
    m_11C = 0;
}
