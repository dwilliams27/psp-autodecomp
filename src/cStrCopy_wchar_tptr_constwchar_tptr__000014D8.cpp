void cStrCopy(wchar_t *dst, const wchar_t *src)
{
    wchar_t c = 0;
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
