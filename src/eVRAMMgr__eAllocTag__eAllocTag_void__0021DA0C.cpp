// eVRAMMgr::eAllocTag::eAllocTag(void)
// Address: 0x0021da0c, Size: 32B
// Obj: eAll_psp.obj
// Symbol: __0o5IeVRAMMgrJeAllocTagctv

class eVRAMMgr {
public:
    class eAllocTag {
    public:
        int   m0;   // 0x0  sw
        int   m4;   // 0x4  sw
        char  m8;   // 0x8  sb
        char  m9;   // 0x9  sb
        int   mC;   // 0xC  sw
        int   m10;  // 0x10 sw
        eAllocTag();
    };
};

eVRAMMgr::eAllocTag::eAllocTag() {
    m0  = 0;
    m4  = 0;
    m8  = 0;
    m9  = 0;
    mC  = 0;
    m10 = 0;
}
