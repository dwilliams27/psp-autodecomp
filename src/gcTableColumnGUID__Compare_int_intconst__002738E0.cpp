// gcTableColumnGUID::Compare(int, int) const
// Address: 0x002738e0, Size: 160B
// Obj: gcAll_psp.obj

struct cGUID {
    unsigned int mA;
    unsigned int mB;
};

class gcTableColumn {
public:
    void *mOwner;
    void *mClassDesc;
};

class gcTableColumnGUID : public gcTableColumn {
public:
    cGUID *mValues;

    int Compare(int, int) const;
};

static inline int guid_less(const cGUID &x, const cGUID &y) {
    int r = 0;
    if (x.mA < y.mA || (x.mA == y.mA && x.mB < y.mB)) {
        r = 1;
    }
    return r;
}

int gcTableColumnGUID::Compare(int a, int b) const {
    int result = -1;
    unsigned char lt = (unsigned char)guid_less(mValues[a], mValues[b]);
    if (!lt) {
        unsigned char gt = (unsigned char)guid_less(mValues[b], mValues[a]);
        result = (gt != 0);
    }
    return result;
}
