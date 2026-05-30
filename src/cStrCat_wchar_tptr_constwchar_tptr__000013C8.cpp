void cStrCat(wchar_t *dst, const wchar_t *src)
{
    if (src != 0) {
        while (*dst != 0) {
            dst++;
        }
        while ((*dst++ = *src++) != 0) {
        }
    }
}
