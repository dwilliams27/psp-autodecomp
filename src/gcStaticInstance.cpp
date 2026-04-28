class cBase;
class cFile;
class cMemPool;

class cObject {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcStaticInstance : public cObject {
public:
    char _pad8[0x44];
    cHandle mField44;
    cBase *mField48;
    float mField4C;
    bool mField50;

    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void gcStaticInstance__gcStaticInstance_cBaseptr(void *self, cBase *parent);

void gcStaticInstance::Write(cFile &file) const {
    cWriteBlock wb(file, 0xA);
    cObject::Write(file);
    mField44.Write(wb);
    wb.WriteBase(mField48);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.End();
}

cBase *gcStaticInstance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStaticInstance *result = 0;
    gcStaticInstance *obj = (gcStaticInstance *)rec->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcStaticInstance__gcStaticInstance_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
