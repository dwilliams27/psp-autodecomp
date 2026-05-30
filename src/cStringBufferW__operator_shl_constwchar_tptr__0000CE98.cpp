// cStringBufferW::operator<<(const wchar_t *) @ 0x0000ce98
// Split-TU local declaration to emit __0oOcStringBufferWlsPCw

class cStringBufferW {
public:
    wchar_t *pos;
    wchar_t *end;
    cStringBufferW &operator<<(const wchar_t *s);
};

cStringBufferW &cStringBufferW::operator<<(const wchar_t *s) {
    if (s == 0) {
        return *this;
    }
    wchar_t c = *s;
    if (c != 0) {
        do {
            if ((unsigned int) pos < (unsigned int) (end - 1)) {
                *pos++ = c;
            }
            ++s;
            c = *s;
        } while (c != 0);
    }
    *pos = 0;
    return *this;
}
