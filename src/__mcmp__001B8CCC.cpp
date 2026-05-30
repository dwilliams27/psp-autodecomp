// __mcmp @ 0x001b8ccc (gMain_psp.obj), 136 bytes
// dtoa.c Bigint comparison helper.

typedef unsigned long ULong;

struct Bigint {
    Bigint *next;
    int k, maxwds, sign, wds;
    ULong x[1];
};

extern "C" int __mcmp(Bigint *a, Bigint *b)
{
    ULong *xa, *xa0, *xb, *xb0;
    int i, j;

    i = a->wds;
    j = b->wds;
    if (i -= j)
        return i;
    xa0 = a->x;
    xa = xa0 + j;
    xb0 = b->x;
    xb = xb0 + j;
    for (;;) {
        if (*--xa != *--xb)
            return *xa < *xb ? -1 : 1;
        if (xa <= xa0)
            break;
    }
    return 0;
}
