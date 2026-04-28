class cBase;
class cMemPool;
class eMaterial;

class cObject {
public:
    cObject &operator=(const cObject &);
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
    static cBase *New(cMemPool *, cBase *);
};

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
