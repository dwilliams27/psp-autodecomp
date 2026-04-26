// gcPartialBodyControllerTemplate::Reset(cMemPool *, bool) @ 0x0013fe38
// Two embedded cBaseArray members at +0x14 and +0x20; both get Reset(pool).

class cBase;
class cMemPool;

class cBaseArray {
public:
    void Reset(cMemPool *);
};

class gcPartialBodyControllerTemplate {
public:
    char _pad0[0x14];
    cBaseArray mArr1;     // +0x14
    char _pad18[0x20 - 0x14 - sizeof(cBaseArray)];
    cBaseArray mArr2;     // +0x20
    void Reset(cMemPool *, bool);
};

void gcPartialBodyControllerTemplate::Reset(cMemPool *pool, bool) {
    mArr1.Reset(pool);
    mArr2.Reset(pool);
}
