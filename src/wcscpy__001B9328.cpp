// wcscpy @ 0x001b9328 (96B), gMain_psp.obj
typedef unsigned short wchar_t_psp;

extern "C" void *memcpy(void *dst, const void *src, unsigned int n);
extern "C" unsigned int wcslen(const wchar_t_psp *s);

extern "C" wchar_t_psp *wcscpy(wchar_t_psp *dst, const wchar_t_psp *src)
{
    unsigned int len = wcslen(src);
    memcpy(dst, src, len * 2);
    dst[len] = 0;
    return dst;
}
