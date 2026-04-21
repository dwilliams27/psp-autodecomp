void *memcpy(void *dst, const void *src, unsigned int n) {
    char *d = (char *)dst;
    const char *s = (const char *)src;
    void *ret = dst;
    while (n-- != 0) {
        *d++ = *s++;
    }
    return ret;
}
