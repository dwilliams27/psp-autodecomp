// eGeomTrailTemplate.cpp - decompiled from eAll_psp.obj.

#pragma control sched=1

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(float);
    void End(void);
};

class cHandle {
public:
    unsigned int mKey;
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cObject {
public:
    char _pad[0x44];
    ~cObject(void);
    cObject &operator=(const cObject &);
};

class eDynamicGeomTemplate : public cObject {
public:
    float mDynamicField44;
    void Write(cFile &) const;
};

class eGeomTrailTemplate : public eDynamicGeomTemplate {
public:
    cHandle mHandle;
    int mField4C;
    int mField50;
    int mField54;
    float mField58;
    float mField5C;
    bool mField60;
    char _pad61[3];
    int mField64;

    eGeomTrailTemplate(cBase *);
    ~eGeomTrailTemplate(void);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *target = block + off;
        __asm__ volatile("" ::: "memory");
        void (*fn)(void *, void *) = rec->fn;
        fn(target, p);
    }
};

extern "C" {
    void eGeomTrailTemplate__eGeomTrailTemplate_cBaseptr(void *self, cBase *parent);
}

extern char eGeomTemplatevirtualtable[];

template <class T> T *dcast(const cBase *);

// -- eGeomTrailTemplate::Write(cFile &) const @ 0x00078f4c --
void eGeomTrailTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    mHandle.Write(wb);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.Write(mField54);
    wb.Write(mField58);
    wb.Write(mField5C);
    wb.Write(mField60);
    wb.Write(mField64);
    wb.End();
}

// -- eGeomTrailTemplate::New(cMemPool *, cBase *) static @ 0x00211224 --
cBase *eGeomTrailTemplate::New(cMemPool *pool, cBase *parent) {
    eGeomTrailTemplate *result = 0;
    __asm__ volatile("" : "+r"(parent), "+r"(result) :: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eGeomTrailTemplate *obj = (eGeomTrailTemplate *)rec->fn(base, 0x68, 4, 0, 0);
    if (obj != 0) {
        eGeomTrailTemplate__eGeomTrailTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// -- eGeomTrailTemplate::~eGeomTrailTemplate(void) @ 0x00211438 --
eGeomTrailTemplate::~eGeomTrailTemplate(void) {
    *(void **)((char *)this + 4) = eGeomTemplatevirtualtable;
}

// -- eGeomTrailTemplate::AssignCopy(const cBase *) @ 0x0021118c --
void eGeomTrailTemplate::AssignCopy(const cBase *src) {
    eGeomTrailTemplate *other = dcast<eGeomTrailTemplate>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    mDynamicField44 = other->mDynamicField44;
    __asm__ volatile("" ::: "memory");
    cHandle *dstHandle = &mHandle;
    cHandle *srcHandle = &other->mHandle;
    *(int *)dstHandle = *(int *)srcHandle;
    mField4C = other->mField4C;
    mField50 = other->mField50;
    mField54 = other->mField54;
    mField58 = other->mField58;
    mField5C = other->mField5C;
    mField60 = other->mField60;
    int *dstField64 = &mField64;
    int *srcField64 = &other->mField64;
    *dstField64 = *srcField64;
}
