// cItoA(int, wchar_t *) @ 0x00000e84, 152B, cAll_psp.obj
// mangled: __0FFcItoAiPw

int cItoA(int n, wchar_t *start)
{
    wchar_t *p = start;

    if (n < 0) {
        *p = 0x2D;
        p++;
        n = -n;
    }

    wchar_t *lo = p;

    do {
        *p = (wchar_t)((n % 10) + 0x30);
        n = n / 10;
        p++;
    } while (n > 0);

    int len = (int)((char *)p - (char *)start);
    int ret = len / 2;
    *p = 0;

    wchar_t *a0 = p - 1;
    wchar_t *a3 = a0;
    do {
        wchar_t v = *lo;
        wchar_t w = *a3;
        *a0 = v;
        a0 = a3 - 1;
        *lo = w;
        a3 = a0;
        lo++;
    } while ((unsigned int)lo < (unsigned int)a3);

    return ret;
}
