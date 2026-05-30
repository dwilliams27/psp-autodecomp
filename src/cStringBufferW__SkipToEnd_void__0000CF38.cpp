// cStringBufferW::SkipToEnd(void) @ 0x0000cf38
// Split-TU local declaration to emit __0fOcStringBufferWJSkipToEndv

class cStringBufferW {
public:
    wchar_t *pos;
    wchar_t *end;
    void SkipToEnd();
};

void cStringBufferW::SkipToEnd() {
    while ((unsigned int) pos < (unsigned int) (end - 1) && *pos != 0) {
        ++pos;
    }
    *pos = 0;
}
