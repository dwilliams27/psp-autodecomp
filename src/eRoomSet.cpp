class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    void Write(cFile &) const;
    cObject &operator=(const cObject &);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T> T *dcast(const cBase *);

class eRoomTreeNode;
class eRoom;
class ePortal;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

struct ShortTriple {
    short x;
    short y;
    short z;
};

struct IntField {
    int value;
};

class eRoomAABBTree {
public:
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class eRoomSet : public cObject {
public:
    char mAABBTree[8];
    int *mHandles;

    eRoomSet(cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntitySetCollisionMask {
public:
    void AssignCopy(const cBase *);
};

class gcGeomTrailController {
public:
    void AssignCopy(const cBase *);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000468D8;

eRoomSet::eRoomSet(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x380A08;
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    *(int *)((char *)this + 0x4C) = 0;
    *(int *)((char *)this + 0x50) = 0;
    *(int *)((char *)this + 0x54) = 0;
    *(int *)((char *)this + 0x58) = 0;
}

const cType *eRoomSet::GetType(void) const {
    if (D_000468D8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000468D8 = cType::InitializeType(0, 0, 0x222, D_000385E4,
                                           &eRoomSet::New,
                                           (const char *)0x36CE10,
                                           (const char *)0x36CE1C, 3);
    }
    return D_000468D8;
}

// -- eRoomSet::Write(cFile &) const @ 0x0003f934 --
void eRoomSet::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cObject::Write(file);
    ((const eRoomAABBTree *)mAABBTree)->Write(wb);

    int count0 = 0;
    if (mHandles != 0) {
        count0 = mHandles[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int count = 0;
    if (mHandles != 0) {
        count = mHandles[-1] & 0x3FFFFFFF;
    }
    int *handles = mHandles;

    int i = 0;
    int offset = 0;
    if (i < count) {
        int *handle = handles + offset;
        do {
            ((cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }
    wb.End();
}

// -- eRoomSet::AssignCopy(const cBase *) @ 0x001eb0b8 --
void eRoomSet::AssignCopy(const cBase *base) {
    eRoomSet *other = dcast<eRoomSet>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cArrayBase<eRoomTreeNode> *)((char *)this + 0x44))->operator=(
        *(const cArrayBase<eRoomTreeNode> *)((char *)other + 0x44));
    ((cArrayBase<cHandleT<eRoom> > *)((char *)this + 0x48))->operator=(
        *(const cArrayBase<cHandleT<eRoom> > *)((char *)other + 0x48));
    ((cArrayBase<cHandleT<ePortal> > *)((char *)this + 0x4C))->operator=(
        *(const cArrayBase<cHandleT<ePortal> > *)((char *)other + 0x4C));
    *(int *)((char *)this + 0x50) = *(const int *)((char *)other + 0x50);
    *(int *)((char *)this + 0x54) = *(const int *)((char *)other + 0x54);
    int *srcField58 = (int *)((char *)other + 0x58);
    int *dstField58 = (int *)((char *)this + 0x58);
    *dstField58 = *srcField58;
}

// -- eRoomSet::New(cMemPool *, cBase *) static @ 0x001eb13c --
cBase *eRoomSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eRoomSet *result = 0;
    eRoomSet *obj = (eRoomSet *)rec->fn(base, 0x5C, 4, 0, 0);
    if (obj != 0) {
        new (obj) eRoomSet(parent);
        result = obj;
    }
    return (cBase *)result;
}

// -- gcDoEntitySetCollisionMask::AssignCopy(const cBase *) @ 0x002c571c --
void gcDoEntitySetCollisionMask::AssignCopy(const cBase *base) {
    gcDoEntitySetCollisionMask *other = dcast<gcDoEntitySetCollisionMask>(base);
    int *self = (int *)this;
    int *src = (int *)other;
    int flags = self[2] & ~3;
    self[2] = flags;
    self[2] = flags | (src[2] & 3);
    IntField *dstFieldC = (IntField *)((char *)this + 0xC);
    IntField *srcFieldC = (IntField *)((char *)other + 0xC);
    int valueC = srcFieldC->value;
    dstFieldC->value = valueC;
    int value10 = src[4];
    gcDesiredEntity *dstEntity14 = (gcDesiredEntity *)((char *)this + 0x14);
    const gcDesiredEntity *srcEntity14 = (const gcDesiredEntity *)((char *)other + 0x14);
    self[4] = value10;
    dstEntity14->operator=(*srcEntity14);
    ((gcDesiredEntity *)((char *)this + 0x40))->operator=(
        *(const gcDesiredEntity *)((char *)other + 0x40));
}

// -- gcGeomTrailController::AssignCopy(const cBase *) @ 0x0031c85c --
void gcGeomTrailController::AssignCopy(const cBase *base) {
    gcGeomTrailController *other = dcast<gcGeomTrailController>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(const int *)((char *)other + 12);
    *(float *)((char *)this + 16) = *(const float *)((char *)other + 16);
    *(float *)((char *)this + 20) = *(const float *)((char *)other + 20);
    *(float *)((char *)this + 24) = *(const float *)((char *)other + 24);

    int i = 0;
    ShortTriple *dst = (ShortTriple *)((char *)this + 0x1C);
    ShortTriple *src = (ShortTriple *)((char *)other + 0x1C);
    do {
        *dst = *src;
        i++;
        dst++;
        src++;
    } while (i <= 0);
}
