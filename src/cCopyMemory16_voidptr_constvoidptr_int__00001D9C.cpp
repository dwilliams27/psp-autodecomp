// cCopyMemory16(void *, const void *, int) @ 0x00001d9c, cAll_psp.obj
// symbol: __0FNcCopyMemory16PvPCvi
// VFPU quad copy loop: 16-byte aligned blocks dst<-src, count bytes.

typedef int v4sf_t __attribute__((mode(V4SF)));

void cCopyMemory16(void *dst, const void *src, int count)
{
    v4sf_t *d = (v4sf_t *)dst;
    v4sf_t *end = (v4sf_t *)((char *)dst + count);
    const v4sf_t *s = (const v4sf_t *)src;
    for (; d < end; ++d, ++s) {
        *d = *s;
    }
}
