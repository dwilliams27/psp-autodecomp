// _multadd @ 0x001b84dc (gMain_psp.obj), 256 bytes
// dtoa.c Bigint multiply-add helper (16-bit / non-Pack_32 variant).

typedef unsigned long ULong;

struct Bigint {
    Bigint *next;
    int k, maxwds, sign, wds;
    ULong x[1];
};

extern "C" Bigint *_Balloc(int p, int k);
extern "C" void _Bfree(int p, Bigint *v);
extern "C" void *memcpy(void *dst, const void *src, unsigned int n);

extern "C" Bigint *_multadd(int p, Bigint *b, int m, int a)
{
    int i, wds;
    ULong *x, y;
    ULong carry;
    Bigint *b1;

    wds = b->wds;
    x = b->x;
    i = 0;
    do {
        y = (*x & 0xffff) * m + a;
        carry = (*x >> 16) * m + (y >> 16);
        a = carry >> 16;
        *x++ = (carry << 16) + (y & 0xffff);
    } while (++i < wds);
    if (a) {
        if (wds >= b->maxwds) {
            b1 = _Balloc(p, b->k + 1);
            memcpy((char *)&b1->next + 0xC, (char *)&b->next + 0xC, b->wds * 4 + 8);
            _Bfree(p, b);
            b = b1;
        }
        b->x[wds++] = a;
        b->wds = wds;
    }
    return b;
}
