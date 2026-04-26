class cBase;

template <class T> T *dcast(const cBase *);

class ePointGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSkyGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class ePortalGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void ePointGroup::AssignCopy(const cBase *base) {
    ePointGroup *src = dcast<ePointGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eSkyGroup::AssignCopy(const cBase *base) {
    eSkyGroup *src = dcast<eSkyGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void ePortalGroup::AssignCopy(const cBase *base) {
    ePortalGroup *src = dcast<ePortalGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

class gcDoConsoleOp {
public:
    void EvaluateCommon() const;
    float Evaluate() const;
};

float gcDoConsoleOp::Evaluate() const {
    EvaluateCommon();
    return 0.0f;
}

struct _reent;
extern "C" int _close_r(struct _reent *, int);

struct __sFILE_lite {
    char _pad0[14];
    short _file;
    char _pad1[68];
    struct _reent *_data;
};

extern "C" int __sclose(struct __sFILE_lite *fp) {
    return _close_r(fp->_data, fp->_file);
}
