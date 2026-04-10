class cBase;
class cInStream;

class cArrayFloat {
public:
    float *mData;
    cArrayFloat &operator=(const cArrayFloat &);
    void Read(cInStream &);
};

struct gcTableColumnFloat;
float cAtoF(const wchar_t *);
gcTableColumnFloat *dcast(const cBase *);

struct gcTableColumnFloat {
    char _pad[0x08];
    cArrayFloat mValues;

    void Set(int row, float value);
    float Get(int row) const;
    void Read(cInStream &stream);
    void AssignCopy(const cBase *other);
    void Set(int row, const wchar_t *text, bool flag);
};

void gcTableColumnFloat::Set(int row, float value) {
    mValues.mData[row] = value;
}

float gcTableColumnFloat::Get(int row) const {
    return mValues.mData[row];
}

void gcTableColumnFloat::Read(cInStream &stream) {
    mValues.Read(stream);
}

void gcTableColumnFloat::AssignCopy(const cBase *other) {
    gcTableColumnFloat *src = dcast(other);
    cArrayFloat &srcArr = *(cArrayFloat *)((char *)src + 8);
    ((cArrayFloat *)((char *)this + 8))->operator=(srcArr);
}

void gcTableColumnFloat::Set(int row, const wchar_t *text, bool flag) {
    float val = cAtoF(text);
    mValues.mData[row] = val;
}
