typedef unsigned char u8;

extern "C" void *memchr(const void *s, int c, unsigned int n);

void *memchr(const void *s, int c, unsigned int n)
{
    const u8 *p = (const u8 *)s;
    unsigned char ch = (unsigned char)c;

    while (n-- != 0) {
        if (*p == ch) {
            return (void *)p;
        }
        p++;
    }
    return 0;
}
