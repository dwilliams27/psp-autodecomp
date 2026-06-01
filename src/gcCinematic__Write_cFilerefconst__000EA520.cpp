// gcCinematic::Write(cFile &) const
// Address: 0x000ea520, Obj: gcAll_psp.obj
// Symbol: __0fLgcCinematicFWriteR6FcFileK

class cFile;

class cWriteBlock {
public:
    cFile *mFile;
    unsigned int mOffset;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    void Write(cFile &) const;
};

typedef void (*cBaseWriteFn)(void *, cFile *);

struct cBaseDispatchRecord {
    short offset;
    short pad;
    void *fn;
};

struct gcCinematicData {
    char pad_00[0x4C];
    bool mFlag0;           // 0x4C
    bool mFlag1;           // 0x4D
    bool mFlag2;           // 0x4E
    char pad_4F[0x01];
    char pad_50[0x04];     // 0x50  mHandleA
    unsigned int mValA;    // 0x54
    char pad_58[0x04];     // 0x58  mHandleB
    unsigned int mValB;    // 0x5C
    char mSub[0x04];       // 0x60  embedded subobject
    char *mSubDesc;        // 0x64  dispatch-table pointer for mSub
};

class gcCinematic {
public:
    void Write(cFile &) const;
};

void gcCinematic::Write(cFile &file) const {
    cWriteBlock wb(file, 5);

    ((cObject *)this)->Write(file);

    gcCinematicData *d = (gcCinematicData *)this;

    ((cBaseArray *)((char *)this + 0x44))->Write(wb);

    wb.Write(d->mFlag0);
    wb.Write(d->mFlag1);
    wb.Write(d->mFlag2);

    ((cHandle *)((char *)this + 0x50))->Write(wb);
    wb.Write(d->mValA);

    ((cHandle *)((char *)this + 0x58))->Write(wb);
    wb.Write(d->mValB);

    char *subBase = (char *)this + 0x60;
    cBaseDispatchRecord *we = (cBaseDispatchRecord *)(d->mSubDesc + 0x28);
    ((cBaseWriteFn)we->fn)(subBase + we->offset, wb.mFile);

    wb.End();
}
