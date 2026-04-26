class cBase;

template <class T> T *dcast(const cBase *);

class gcEntityGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void gcEntityGroup::AssignCopy(const cBase *base) {
    gcEntityGroup *src = dcast<gcEntityGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}
