class cInStream;

class cArrayByte {
public:
    unsigned char *mData;
    void Read(cInStream &);
};

int cAtoI(const wchar_t *);
int cStrFormat(wchar_t *, const wchar_t *, ...);

struct gcTableColumnByte {
    char _pad[0x08];
    cArrayByte mValues;

    float Get(int row) const;
    int Compare(int row1, int row2) const;
    void Read(cInStream &stream);
    void Get(int row, wchar_t *buf, int bufsize) const;
    void Set(int row, const wchar_t *text, bool flag);
};

float gcTableColumnByte::Get(int row) const {
    return (float)mValues.mData[row];
}

int gcTableColumnByte::Compare(int row1, int row2) const {
    int result = -1;
    int a = mValues.mData[row1];
    int b = mValues.mData[row2];
    if (a >= b) {
        result = b < a;
    }
    return result;
}

void gcTableColumnByte::Read(cInStream &stream) {
    mValues.Read(stream);
}

void gcTableColumnByte::Get(int row, wchar_t *buf, int bufsize) const {
    cStrFormat(buf, L"%d", mValues.mData[row]);
}

void gcTableColumnByte::Set(int row, const wchar_t *text, bool flag) {
    mValues.mData[row] = (unsigned char)cAtoI(text);
}
