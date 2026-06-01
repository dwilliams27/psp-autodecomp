// cAtoI(const wchar_t *) @ 0x00000FEC, cAll_psp.obj
// symbol: __0FFcAtoIPCw

extern int cStrLength(const wchar_t *);

int cAtoI(const wchar_t *str)
{
    const wchar_t *s = str;
    int len = ::cStrLength(str);
    if (len == 0)
        return 0;

    unsigned int sign = 1;
    if (*str == 0x2D) {
        sign = -1;
        s += 1;
        len -= 1;
    }

    int accum = 0;
    int place = 1;
    int count = 0;

    if (count < len) {
        const wchar_t *p = s + len;
        do {
            unsigned short c = p[-1];
            if ((int)c < 0x30)
                return 0;
            if ((int)c >= 0x3A)
                return 0;
            int d = ((int)c - 0x30) * place;
            place *= 10;
            count += 1;
            accum += d;
            p -= 1;
        } while (count < len);
    }
    return sign * accum;
}
