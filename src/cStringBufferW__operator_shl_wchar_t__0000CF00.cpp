// cStringBufferW::operator<<(wchar_t) @ 0x0000cf00
// Split-TU local declaration to emit __0oOcStringBufferWlsw

class cStringBufferW {
public:
    wchar_t *pos;
    wchar_t *end;
    cStringBufferW &operator<<(wchar_t c);
};

cStringBufferW &cStringBufferW::operator<<(wchar_t c) {
    if ((unsigned int) pos < (unsigned int) (end - 1)) {
        *pos++ = c;
    }
    *pos = 0;
    return *this;
}
