class cBase;

template <class T> T *dcast(const cBase *);

class gcTriggerGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class gcCinematicGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

void gcTriggerGroup::AssignCopy(const cBase *base) {
    gcTriggerGroup *src = dcast<gcTriggerGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void gcCinematicGroup::AssignCopy(const cBase *base) {
    gcCinematicGroup *src = dcast<gcCinematicGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

extern "C" {

long strtol(const char *s, char **endptr, int base);
struct lconv *_localeconv_r(void *r);
void *memset(void *dst, int val, unsigned int n);

extern void *_impure_ptr;

int atoi(const char *s) {
    return (int)strtol(s, 0, 10);
}

struct lconv *localeconv(void) {
    return _localeconv_r(_impure_ptr);
}

void *__memzero(void *dst, unsigned int n) {
    return memset(dst, 0, n);
}

}
