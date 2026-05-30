// strcpy @ 0x001ad7e4 (gMain_psp.obj), 68 bytes, leaf
extern "C" char *strcpy(char *a0, char *a1) {
    char *ret = a0;
    char c = *a1;
    char *src = a1 + 1;
    *a0 = c;
    char *dst = a0 + 1;
    a1 = src;
    a0 = dst;
    if (c != 0) {
        do {
            c = *src;
            src = a1 + 1;
            *dst = c;
            dst = a0 + 1;
            a1 = src;
            a0 = dst;
        } while (c != 0);
    }
    return ret;
}
