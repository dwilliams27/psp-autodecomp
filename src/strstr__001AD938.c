/* strstr @ 0x001ad938  (gMain_psp.obj)
 * Byte-exact reconstruction of the SNC-compiled strstr. */

char *strstr(const char *haystack, const char *needle)
{
    int v35040 = 0;

    if (*haystack == 0) {
        char *v0 = 0;
        if (*needle == 0) {
            v0 = (char *)haystack;
        }
        return v0;
    }

    {
        char a2;
loop_top:
        a2 = needle[v35040];
inner:
        if (a2 == 0) {
            return (char *)haystack;
        }
        if (a2 == haystack[v35040]) {
            v35040 += 1;
            a2 = needle[v35040];
            goto inner;
        }
        v35040 = 0;
        haystack += 1;
        if (*haystack == 0) {
            return 0;
        }
        goto loop_top;
    }
}
