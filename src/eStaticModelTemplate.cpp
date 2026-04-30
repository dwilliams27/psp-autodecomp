inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(float);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class eGeomTemplate : public cObject {
public:
    void Write(cFile &) const;
};

class eStaticGeomTemplate : public eGeomTemplate {
public:
    void Write(cFile &) const;
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T>
class cArray : public cArrayBase<T> {
};

template <class T> T *dcast(const cBase *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class eStaticModelTemplate : public eStaticGeomTemplate {
public:
    cHandle mHandle44;
    cHandle mHandle48;
    cHandle mHandle4C;
    float mField50;
    cArray<float> mValues;

    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern char eStaticModelTemplatevirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469A8;
extern cType *D_000469AC;
extern cType *D_000469B0;

// -- eStaticModelTemplate::Write(cFile &) const @ 0x00041158 --
void eStaticModelTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eStaticGeomTemplate *)this)->Write(file);
    mHandle44.Write(wb);
    mHandle48.Write(wb);
    mHandle4C.Write(wb);
    wb.Write(mField50);

    int count = 0;
    if (mValues.mData != 0) {
        count = ((int *)mValues.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(count);

    count = 0;
    if (mValues.mData != 0) {
        count = ((int *)mValues.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(count, mValues.mData);
    wb.End();
}

// -- eStaticModelTemplate::AssignCopy(const cBase *) @ 0x001ebec0 --
void eStaticModelTemplate::AssignCopy(const cBase *base) {
    eStaticModelTemplate *other = dcast<eStaticModelTemplate>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    mHandle44 = other->mHandle44;
    mHandle48 = other->mHandle48;
    mHandle4C = other->mHandle4C;
    cArrayBase<float> *dstArray = &mValues;
    const cArrayBase<float> *srcArray = &other->mValues;
    mField50 = other->mField50;
    dstArray->operator=(*srcArray);
}

// -- eStaticModelTemplate::New(cMemPool *, cBase *) static @ 0x001ebf44 --
cBase *eStaticModelTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eStaticModelTemplate *result = 0;
    eStaticModelTemplate *obj = (eStaticModelTemplate *)rec->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        new ((cObject *)obj) cObject(parent);
        *(void **)((char *)obj + 4) = eStaticModelTemplatevirtualtable;
        obj->mHandle44.mIndex = 0;
        obj->mHandle48.mIndex = 0;
        obj->mHandle4C.mIndex = 0;
        obj->mField50 = 0.0f;
        obj->mValues.mData = 0;
        result = obj;
    }
    return (cBase *)result;
}

// -- eStaticModelTemplate::GetType(void) const @ 0x001EBFE4 --
const cType *eStaticModelTemplate::GetType(void) const {
    if (D_000469B0 == 0) {
        if (D_000469AC == 0) {
            if (D_000469A8 == 0) {
                if (D_000385E4 == 0) {
                    if (D_000385E0 == 0) {
                        if (D_000385DC == 0) {
                            const char *name = (const char *)0x36CD74;
                            const char *desc = (const char *)0x36CD7C;
                            __asm__ volatile("" : "+r"(name), "+r"(desc));
                            D_000385DC = cType::InitializeType(
                                name, desc, 1, 0, 0, 0, 0, 0);
                        }
                        const cType *parentType = D_000385DC;
                        cBase *(*factory)(cMemPool *, cBase *) = &cNamed::New;
                        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                        D_000385E0 = cType::InitializeType(
                            0, 0, 2, parentType, factory, 0, 0, 0);
                    }
                    D_000385E4 = cType::InitializeType(
                        0, 0, 3, D_000385E0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385E4;
                __asm__ volatile("" : "+r"(parentType));
                __asm__ volatile("" ::: "memory");
                const char *kindName = (const char *)0x36CE2C;
                const char *kindDesc = (const char *)0x36CE3C;
                __asm__ volatile("" : "+r"(kindName), "+r"(kindDesc));
                D_000469A8 = cType::InitializeType(
                    0, 0, 0x20, parentType, 0, kindName, kindDesc, 5);
            }
            D_000469AC = cType::InitializeType(0, 0, 0x21, D_000469A8,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000469AC;
        cBase *(*factory)(cMemPool *, cBase *) = &eStaticModelTemplate::New;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_000469B0 = cType::InitializeType(0, 0, 0x1E, parentType, factory,
                                           0, 0, 0);
    }
    return D_000469B0;
}
