// eCameraEffectMgrEntry::eCameraEffectMgrEntry(void)  @ 0x001e373c (40B)
// Symbol: __0oVeCameraEffectMgrEntryctv

class eCameraEffectMgrEntry {
public:
    char  m_flag;   // 0x00
    float m_f04;    // 0x04
    float m_f08;    // 0x08
    int   m_i0C;    // 0x0C
    int   m_i10;    // 0x10
    eCameraEffectMgrEntry();
};

eCameraEffectMgrEntry::eCameraEffectMgrEntry() {
    m_flag = 0;
    __asm__ volatile("" ::: "memory");
    float zero = 0.0f;
    float one = 1.0f;
    m_f08 = zero;
    m_f04 = one;
    m_i0C = 0;
    m_i10 = 0;
}
