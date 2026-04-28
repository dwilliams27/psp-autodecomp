#include "cBase.h"

class cMemPool;

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class gcEvent {
public:
    gcEvent &operator=(const gcEvent &);
};

template <class T> T *dcast(const cBase *);

class gcUIGeomTemplate : public cObject {
public:
    char _pad[0x44];          // cObject internals
    float mField44;           // 0x44
    int mField48;             // 0x48
    int mField4C;             // 0x4C
    float mField50;           // 0x50
    gcEvent mEvent;           // 0x54

    void Reset(cMemPool *, bool);
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void gcUIGeomTemplate__gcUIGeomTemplate_cBaseptr(void *self, cBase *parent);
}

class gcPartialBodyController {
public:
    void OnActivated(void);
};

class gcSubGeomController {
public:
    void Reset(cMemPool *, bool);
};

enum nwTransportType {};

class nwTransport {
public:
    static nwTransport *Create(nwTransportType, unsigned short);
};

class nwNetwork {
public:
    static void *GetLobby(void);
};

void gcUIGeomTemplate::Reset(cMemPool *, bool) {
}

// ── gcUIGeomTemplate::AssignCopy(const cBase *) @ 0x00290e54 ──
void gcUIGeomTemplate::AssignCopy(const cBase *src) {
    gcUIGeomTemplate *other = dcast<gcUIGeomTemplate>(src);
    cObject::operator=(*other);
    mField44 = other->mField44;
    mField48 = other->mField48;
    mField4C = other->mField4C;
    mField50 = other->mField50;
    mEvent = other->mEvent;
}

// ── gcUIGeomTemplate::New(cMemPool *, cBase *) static @ 0x00290ec0 ──
cBase *gcUIGeomTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIGeomTemplate *result = 0;
    gcUIGeomTemplate *obj = (gcUIGeomTemplate *)rec->fn(base, 0x70, 4, 0, 0);
    if (obj != 0) {
        gcUIGeomTemplate__gcUIGeomTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcPartialBodyController::OnActivated(void) {
}

void gcSubGeomController::Reset(cMemPool *, bool) {
}

nwTransport *nwTransport::Create(nwTransportType, unsigned short) {
    return 0;
}

void *nwNetwork::GetLobby(void) {
    return 0;
}
