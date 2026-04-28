// gcValMemCardStatus.cpp — decompiled from gcAll_psp.obj.
//
// Functions:
//   0x003501ac gcValMemCardStatus::Write(cFile &) const             100B
//   0x00350490 gcValMemCardStatus::~gcValMemCardStatus(void)         100B  (deleting)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct gcValMemCardStatus_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct gcValMemCardStatus_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValMemCardStatus {
public:
    void *_field0;          // 0x0
    void *_field4;          // 0x4 — classdesc
    unsigned int mField8;   // 0x8
    unsigned int mFieldC;   // 0xC

    ~gcValMemCardStatus(void);
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        void *self = p;
        gcValMemCardStatus_DeleteRecord *rec =
            (gcValMemCardStatus_DeleteRecord *)
            (((gcValMemCardStatus_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, self);
    }
};

extern char cBaseclassdesc[];   // @ 0x37E6A8

// ── gcValMemCardStatus::Write @ 0x003501ac ──
void gcValMemCardStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcValue *)this)->Write(file);
    wb.Write(mField8);
    wb.Write(mFieldC);
    wb.End();
}

// ── gcValMemCardStatus::~gcValMemCardStatus @ 0x00350490, 100B ──
// Canonical destructor: SNC auto-emits the (this != 0) guard, the absence of a
// parent destructor chain, and the deleting-tail dispatch through operator
// delete on (flag & 1). Body just resets the classdesc pointer at offset 4.
gcValMemCardStatus::~gcValMemCardStatus(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
