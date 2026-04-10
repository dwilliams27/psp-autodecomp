class cStringBufferW {
public:
    cStringBufferW(wchar_t *buf, int count);
    wchar_t *start;
    wchar_t *end;
};

cStringBufferW::cStringBufferW(wchar_t *buf, int count) {
    start = buf;
    end = buf + count;
}
