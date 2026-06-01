// gcNetGame::HasFullAuthority(void) static
// Address: 0x00108d04, Size: 104B
// Obj: gcAll_psp.obj
//
// Authority is held when the global state word at 0x37D858 has bit 0 clear,
// or bit 2 set; otherwise it depends on whether a fullscreen cinematic is in
// progress.

class gcStreamedCinematic {
public:
    static unsigned char IsFullscreenInProgress(void);
};

class gcNetGame {
public:
    static unsigned char HasFullAuthority(void);
};

unsigned char gcNetGame::HasFullAuthority(void) {
    int result = 0;
    int authority = 0;
    unsigned int flag = *(unsigned int *)0x37D858;
    if ((flag & 1) == 0 || (flag & 4) != 0) {
        authority = 1;
    }
    unsigned char check = (unsigned char)authority;
    if (check != 0 || gcStreamedCinematic::IsFullscreenInProgress()) {
        result = 1;
    }
    return result;
}
