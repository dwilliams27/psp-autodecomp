// cStrCopy(wchar_t *, const char *)
// Address: 0x00001528, Size: 96B
// Obj: cAll_psp.obj
// Symbol: __0FIcStrCopyPwPCc

void cStrCopy(wchar_t *dst, const char *src)
{
    if (src != 0) {
        wchar_t c = (unsigned char)*src++;
        *dst++ = c;
        while (c & 0xFFFF) {
            c = (unsigned char)*src++;
            *dst++ = c;
        }
    } else {
        *dst = 0;
    }
}
