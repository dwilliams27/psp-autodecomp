// gcLobbyScoreboardStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcLobbyScoreboardStrings

class cBase {
public:
    int _b0;
    int _b4;
};

class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcStringValue : public cBase {
};

extern char gcLobbyScoreboardStringsvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcLobbyScoreboardStrings : public gcStringValue {
public:
    int mValue0;  // 0x08 - tagged self-pointer
    int mValue1;  // 0x0C - tagged self-pointer
    int mValue2;  // 0x10
    int mValue3;  // 0x14
    int mValue4;  // 0x18 - tagged self-pointer

    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4FC;

const cType *gcLobbyScoreboardStrings::GetType(void) const {
    if (D_0009F4FC == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4FC = cType::InitializeType(0, 0, 0x125, D_0009F454,
                                           &gcLobbyScoreboardStrings::New,
                                           0, 0, 0);
    }
    return D_0009F4FC;
}

cBase *gcLobbyScoreboardStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLobbyScoreboardStrings *result = 0;
    gcLobbyScoreboardStrings *obj =
        (gcLobbyScoreboardStrings *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcLobbyScoreboardStringsvirtualtable;
        int tagged = (int)obj | 1;
        obj->mValue0 = tagged;
        obj->mValue1 = tagged;
        obj->mValue2 = 0;
        obj->mValue3 = 0;
        obj->mValue4 = tagged;
        result = obj;
    }
    return (cBase *)result;
}
