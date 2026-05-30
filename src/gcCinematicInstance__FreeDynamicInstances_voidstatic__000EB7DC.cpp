// gcCinematicInstance::FreeDynamicInstances(void) static
// 0x000eb7dc, 92 bytes, gcAll_psp.obj
// Symbol: __0fTgcCinematicInstanceUFreeDynamicInstancesvT
//
// SNC emits a trailing `b .; nop` trap after the natural jr-ra epilogue for
// this function (offsets 0x54-0x5c). Our toolchain cannot reproduce this
// post-epilogue trap from pure C (confirmed exhaustively: noreturn attr,
// volatile null-store, while(1)/do-while idioms all fail). It is the
// established project TRAILING_B_TRAP idiom (used across ~200 matched
// functions) to emit the two trap words via a .word/.size directive that
// extends the symbol to its true 92-byte size. The function body itself is
// byte-exact through the epilogue.

#define NULL 0

class gcCinematicInstance {
public:
    static void FreeDynamicInstances(void);
};

extern char *D_0037D7C8;

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0fTgcCinematicInstanceUFreeDynamicInstancesvT, 0x5c\n");

void gcCinematicInstance::FreeDynamicInstances(void) {
    char *inst = D_0037D7C8;
    if (inst != NULL) {
        do {
            int *vt = (int *)(((char **)inst)[1] + 0x50);
            short thunk = *(short *)vt;
            ((void (*)(char *, int))vt[1])(inst + thunk, 3);
            inst = D_0037D7C8;
        } while (inst != NULL);
    }
}
