extern int cItoA(int, wchar_t *);

int cFtoA(float value, wchar_t *buf, int precision)
{
    float f = value;
    int ipart = (int) f;
    short *p = (short *)buf + cItoA(ipart, buf);

    if (precision > 0) {
        *p = 0x2E;
        p += 1;
    }

    for (int i = 0; i < precision; i++) {
        f = (f - (float) ipart) * 10.0f;
        ipart = (int) f;
        *p = (short)(ipart + 0x30);
        p += 1;
    }

    *p = 0;
    int count = (int)((char *)p - (char *)buf);
    return count / 2;
}
