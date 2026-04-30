class cBase;
class cMemPool;
class cType;
class eMaterial;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class eFontCharKern;

class eFont : public cObject {
public:
    class ePageInfo;

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00041114;

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void eFont__eFont_cBaseptr(void *, cBase *);

// -- eFont::AssignCopy(const cBase *) @ 0x001e7948 --
void eFont::AssignCopy(const cBase *src) {
    eFont &other = *dcast<eFont>(src);
    ((cObject *)this)->operator=(*(const cObject *)&other);
    *(int *)((char *)this + 0x44) = *(int *)((char *)&other + 0x44);
    ((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x48))->operator=(*(cArrayBase<cHandleT<eMaterial> > *)((char *)&other + 0x48));
    *(float *)((char *)this + 0x4C) = *(float *)((char *)&other + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)&other + 0x50);
    *(float *)((char *)this + 0x54) = *(float *)((char *)&other + 0x54);
    *(float *)((char *)this + 0x58) = *(float *)((char *)&other + 0x58);
    ((cArrayBase<eFontCharKern> *)((char *)this + 0x5C))->operator=(*(cArrayBase<eFontCharKern> *)((char *)&other + 0x5C));
    ((cArrayBase<eFont::ePageInfo> *)((char *)this + 0x60))->operator=(*(cArrayBase<eFont::ePageInfo> *)((char *)&other + 0x60));
}

// -- eFont::New(cMemPool *, cBase *) static @ 0x001e79d4 --
cBase *eFont::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eFont *result = 0;
    eFont *obj = (eFont *)rec->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        eFont__eFont_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=1
const cType *eFont::GetType(void) const {
    if (D_00041114 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *name = (const char *)0x36CD74;
                    const char *desc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(name), "+r"(desc));
                    D_000385DC = cType::InitializeType(name, desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*factory)(cMemPool *, cBase *) =
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType, factory,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x1E79D4;
        const char *kindName = (const char *)0x36CDE8;
        const char *kindDesc = (const char *)0x36CDF0;
        D_00041114 = cType::InitializeType(0, 0, 0x40, parentType, factory,
                                           kindName, kindDesc, 5);
    }
    return D_00041114;
}
#pragma control sched=2
