#include "cBase.h"

class cMemPool;
class cFile;
class cType;

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void End(void);
};

class eDynamicGeomTemplate {
public:
    void Write(cFile &) const;
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

    gcUIGeomTemplate(cBase *);
    const cType *GetType(void) const;
    void Reset(cMemPool *, bool);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct ParamWriteVtableEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" {
    void gcUIGeomTemplate__gcUIGeomTemplate_cBaseptr(void *self, cBase *parent);
    void gcEvent__gcEvent_cBaseptr_constcharptr(void *, cBase *, const char *);
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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;
extern cType *D_000469E0;
extern cType *D_0009F590;

void gcUIGeomTemplate::Reset(cMemPool *, bool) {
}

const cType *gcUIGeomTemplate::GetType(void) const {
    if (D_0009F590 == 0) {
        if (D_000469E0 == 0) {
            if (D_000469A8 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            D_000385DC = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, D_000385DC,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                D_000469A8 = cType::InitializeType(
                    0, 0, 0x20, D_000385E4, 0, (const char *)0x36DA5C,
                    (const char *)0x36DA6C, 5);
            }
            D_000469E0 = cType::InitializeType(
                0, 0, 0x22, D_000469A8, 0, 0, 0, 0);
        }
        D_0009F590 = cType::InitializeType(
            0, 0, 0x83, D_000469E0, gcUIGeomTemplate::New, 0, 0, 0);
    }
    return D_0009F590;
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

gcUIGeomTemplate::gcUIGeomTemplate(cBase *parent) : cObject(parent) {
    mField44 = 1000.0f;
    *(void **)((char *)this + 4) = (void *)0x38B3F0;
    mField48 = 0x280;
    mField4C = 0x1E0;
    mField50 = 1.0f;
    gcEvent__gcEvent_cBaseptr_constcharptr((char *)this + 0x54, (cBase *)this, 0);
}

void gcUIGeomTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    wb.Write(mField48);
    wb.Write(mField4C);
    wb.Write(mField50);
    const ParamWriteVtableEntry *entry =
        (const ParamWriteVtableEntry *)(*(char **)((const char *)this + 0x58) + 0x28);
    char *eventBase = (char *)this + 0x54;
    entry->fn(eventBase + entry->offset, *(cFile **)&wb._data[0]);
    wb.End();
}
