class cBase;

template <class T> T *dcast(const cBase *);

class gcEntityCustomAnimationGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void gcEntityCustomAnimationGroup::AssignCopy(const cBase *base) {
    gcEntityCustomAnimationGroup *src = dcast<gcEntityCustomAnimationGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}
