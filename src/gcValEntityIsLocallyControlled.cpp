// Batch of unrelated functions grouped by the orchestrator into one TU.
// Sections are independent.

class cBase;

template <class T> T *dcast(const cBase *);

// ── gcValEntityIsLocallyControlled::AssignCopy(const cBase *) @ 0x00334A64 ──
class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityIsLocallyControlled {
public:
    void AssignCopy(const cBase *);
};

void gcValEntityIsLocallyControlled::AssignCopy(const cBase *base) {
    gcValEntityIsLocallyControlled *other = dcast<gcValEntityIsLocallyControlled>(base);
    gcDesiredEntity &srcDesired = *(gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(srcDesired);
}

// ── gcValStringIndex::AssignCopy(const cBase *) @ 0x0035E880 ──
class gcDesiredString {
public:
    gcDesiredString &operator=(const gcDesiredString &);
};

class gcValStringIndex {
public:
    void AssignCopy(const cBase *);
};

void gcValStringIndex::AssignCopy(const cBase *base) {
    gcValStringIndex *other = dcast<gcValStringIndex>(base);
    gcDesiredString &srcDesired = *(gcDesiredString *)((char *)other + 8);
    ((gcDesiredString *)((char *)this + 8))->operator=(srcDesired);
}

// ── eStaticMesh::PlatformFree(void) @ 0x0004497C ──
class eStaticMeshVisData;

extern "C" void eStaticMeshVisData___dtor_eStaticMeshVisData_void(void *, int);

class eStaticMesh {
public:
    void PlatformFree(void);
};

void eStaticMesh::PlatformFree(void) {
    void *data = *(void **)((char *)this + 0x60);
    if (data != 0) {
        eStaticMeshVisData___dtor_eStaticMeshVisData_void(data, 3);
        *(void **)((char *)this + 0x60) = 0;
    }
}

// ── eWeatherEffect::eWeatherEffect(cBase *) @ 0x000607D8 ──
class eGeom {
public:
    eGeom(cBase *);
};

class eWeatherEffect : public eGeom {
public:
    eWeatherEffect(cBase *);
};

extern char eWeatherEffectvirtualtable[];

eWeatherEffect::eWeatherEffect(cBase *parent) : eGeom(parent) {
    *(void **)((char *)this + 4) = eWeatherEffectvirtualtable;
    *(int *)((char *)this + 0x90) = 0;
}

// ── eFontGroup::AssignCopy(const cBase *) @ 0x001DD23C ──
class eFontGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void eFontGroup::AssignCopy(const cBase *base) {
    eFontGroup *src = dcast<eFontGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}
