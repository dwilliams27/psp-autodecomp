class cBase;
class cFile;
class cMemPool;

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
    void Write(cFile &) const;
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

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void eRoomSet__eRoomSet_cBaseptr(void *self, cBase *parent);
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
        eRoomSet__eRoomSet_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
