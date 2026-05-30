void cStrCopy(char *dst, const char *src)
{
    char c = 0;
    if (src != 0) {
        c = *src++;
        *dst++ = c;
        while (c != 0) {
            c = *src++;
            *dst++ = c;
        }
        return;
    }
    *dst = 0;
}
