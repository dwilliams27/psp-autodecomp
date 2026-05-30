void cStrCat(char *dst, const char *src)
{
    if (src != 0) {
        while (*dst != 0) {
            dst++;
        }
        while ((*dst++ = *src++) != 0) {
        }
    }
}
