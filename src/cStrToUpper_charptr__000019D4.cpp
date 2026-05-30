// cStrToUpper(char *) @ 0x000019d4  (cAll_psp.obj)
// Uppercases an ASCII string in place. Leaf function.

void cStrToUpper(char *s)
{
    if (s != 0) {
        char *p = s;
        int c = *p;
        if (c != 0) {
            do {
                c = (c << 0x18) >> 0x18;
                if (c >= 0x61 && c < 0x7b) {
                    c = (char)(c - 0x20);
                }
                *p = (char)c;
                ++p;
                c = *p;
            } while (c != 0);
        }
    }
}
