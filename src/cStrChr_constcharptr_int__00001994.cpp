// cStrChr(const char *, int) @ 0x00001994 — cAll_psp.obj
// symbol: __0FHcStrChrPCci

const char *cStrChr(const char *s, int c)
{
    char d;
    for (;;) {
        d = *s;
        if (d == 0) break;
        if (d == (char)c) break;
        s++;
    }
    if (d == (char)c) {
        return s;
    }
    return 0;
}
