class cWriteBlock {
public:
    void Write(int, const short *);
    void Write(int, const wchar_t *);
};

void cWriteBlock::Write(int count, const wchar_t *data) {
    Write(count, (const short *)data);
}
