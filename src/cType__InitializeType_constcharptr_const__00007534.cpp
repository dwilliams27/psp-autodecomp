class cMemPool;
class cBase;

class cType {
public:
    unsigned int mFlags;      // 0x00
    unsigned int mName;       // 0x04
    const char *mName8;       // 0x08
    const char *mNameC;       // 0x0C
    cBase *(*mCreate)(cMemPool *, cBase *); // 0x10
    const char *m14;          // 0x14
    const char *m18;          // 0x18
    const cType *mPrev;       // 0x1C
    cType *m20;               // 0x20
    cType *m24;               // 0x24
    cType *m28;               // 0x28
    unsigned int m2c;         // 0x2C

    void Initialize(const char *, const char *, unsigned int, const cType *,
                    cBase *(*)(cMemPool *, cBase *), const char *, const char *,
                    unsigned int);
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *), const char *,
                                 const char *, unsigned int);
};

extern void *memset(void *, int, unsigned int);

static char gTypeTable[0x8CA0];      // 0x372F60
static cType *gTypeTablePtr;         // 0x37C068
static unsigned char gTypeInit;      // 0x37C064

cType *cType::InitializeType(const char *name, const char *name8,
                             unsigned int idx, const cType *parent,
                             cBase *(*create)(cMemPool *, cBase *),
                             const char *s14, const char *s18,
                             unsigned int extraFlags) {
    if (gTypeInit == 0) {
        memset(gTypeTable, 0, 0x8CA0);
        gTypeTablePtr = (cType *)gTypeTable;
        gTypeInit = 1;
    }

    cType *base = gTypeTablePtr;
    if (base[idx].mName != idx) {
        base[idx].Initialize(name, name8, idx, parent, create, s14, s18,
                             extraFlags);
        base = gTypeTablePtr;
    }
    return &base[idx];
}
