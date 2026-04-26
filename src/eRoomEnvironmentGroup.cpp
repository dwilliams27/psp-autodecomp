class cBase;

template <class T> T *dcast(const cBase *);

class eRoomEnvironmentGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void eRoomEnvironmentGroup::AssignCopy(const cBase *base) {
    eRoomEnvironmentGroup *src = dcast<eRoomEnvironmentGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}
