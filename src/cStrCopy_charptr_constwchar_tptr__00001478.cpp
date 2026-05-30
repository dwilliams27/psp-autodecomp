void cStrCopy(char *dst, const wchar_t *src)
{
    wchar_t w;
    if (src != 0) {
        w = *src++;
        *dst++ = (char)w;
        while ((char)w != 0) {
            w = *src++;
            *dst++ = (char)w;
        }
        return;
    }
    *dst = 0;
}
