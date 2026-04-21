void memcpyalpha(char *dst, const char *src, int n) {
    char *p = dst;
    while (n-- != 0) {
        *p++ = *src++;
    }
}
