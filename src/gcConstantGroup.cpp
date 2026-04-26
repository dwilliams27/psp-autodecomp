class cBase;

template <class T> T *dcast(const cBase *);

class gcConstantGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void gcConstantGroup::AssignCopy(const cBase *base) {
    gcConstantGroup *src = dcast<gcConstantGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}
