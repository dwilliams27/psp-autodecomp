inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcEntityCustomAttack : public cObject {
public:
    char _pad[0x44];
    cBase *mField44;

    gcEntityCustomAttack(cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

void gcEntityCustomAttack::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    int val = (int)mField44;
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

cBase *gcEntityCustomAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEntityCustomAttack *result = 0;
    gcEntityCustomAttack *obj =
        (gcEntityCustomAttack *)rec->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEntityCustomAttack(parent);
        result = obj;
    }
    return (cBase *)result;
}
