// gcValStringCompare -- decompiled from gcAll_psp.obj
//
// Functions:
//   0x0035dad0  gcValStringCompare::New(cMemPool *, cBase *) static
//   0x0035dc84  gcValStringCompare::Write(cFile &) const

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void WriteBase(const cBase *);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcValStringComparevirtualtable[];

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValStringCompare : public gcValue {
public:
    int mLeft;
    int mRight;
    bool mCaseSensitive;
    bool mExact;

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// -- gcValStringCompare::New(cMemPool *, cBase *) static @ 0x0035dad0 --
cBase *gcValStringCompare::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValStringCompare *result = 0;
    gcValStringCompare *obj =
        (gcValStringCompare *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValStringComparevirtualtable;
        unsigned int tagged = ((unsigned int)obj) | 1;
        obj->mLeft = tagged;
        obj->mRight = tagged;
        obj->mCaseSensitive = true;
        obj->mExact = true;
        result = obj;
    }
    return (cBase *)result;
}

// -- gcValStringCompare::Write(cFile &) const @ 0x0035dc84 --
void gcValStringCompare::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);

    int left = mLeft;
    int leftFlag = 0;
    if (left & 1) {
        leftFlag = 1;
    }
    cBase *leftPtr;
    if (leftFlag != 0) {
        leftPtr = 0;
    } else {
        leftPtr = (cBase *)left;
    }
    wb.WriteBase(leftPtr);

    int right = mRight;
    int rightFlag = 0;
    if (right & 1) {
        rightFlag = 1;
    }
    cBase *rightPtr;
    if (rightFlag != 0) {
        rightPtr = 0;
    } else {
        rightPtr = (cBase *)right;
    }
    wb.WriteBase(rightPtr);

    wb.Write(mCaseSensitive);
    wb.Write(mExact);
    wb.End();
}
