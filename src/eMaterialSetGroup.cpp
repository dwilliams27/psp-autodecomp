// AssignCopy trampolines for engine *Group classes that copy a flag byte
// (offset 8) and an int field (offset 12) from a downcast source object.

class cBase;

template <class T> T *dcast(const cBase *);

class eMaterialSetGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSurfaceGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSpriteGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void eMaterialSetGroup::AssignCopy(const cBase *base) {
    eMaterialSetGroup *src = dcast<eMaterialSetGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eSurfaceGroup::AssignCopy(const cBase *base) {
    eSurfaceGroup *src = dcast<eSurfaceGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eSpriteGroup::AssignCopy(const cBase *base) {
    eSpriteGroup *src = dcast<eSpriteGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ----------------------------------------------------------------------
// lflush @ 0x001ab114 (28B) — wraps fflush(NULL)
// ----------------------------------------------------------------------
extern "C" {
    int fflush(void);

    void lflush(void) {
        fflush();
    }

    // ------------------------------------------------------------------
    // atof @ 0x001ac584 (28B) — wraps strtod(s, NULL)
    // ------------------------------------------------------------------
    double strtod(const char *, char **);

    double atof(const char *s) {
        return strtod(s, 0);
    }
}
