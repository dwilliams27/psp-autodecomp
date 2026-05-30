typedef unsigned short u16;

extern "C" u16 *wstrrev(u16 *s)
{
    int n = 0;
    u16 *p = s;
    if (*p != 0) {
        do {
            p++;
            n++;
        } while (*p != 0);
    }
    int i = 0;
    n = n - 1;
    if (i < n) {
        p = s + n;
        u16 *r = s;
        do {
            u16 t = *p;
            u16 u = *r;
            *r = t;
            *p = u;
            i++;
            n--;
            r++;
            p--;
        } while (i < n);
    }
    return s;
}
