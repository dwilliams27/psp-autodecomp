// gcDoPlayerSetEntity.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x002F1D00 gcDoPlayerSetEntity::Write(cFile &) const     120B
//   0x002F1FBC gcDoPlayerSetEntity::GetText(char *) const    224B

class cBase;
class cFile;

class cWriteBlock {
public:
    cFile *_file;       // +0
    int    _pos;        // +4
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

// gcAction layout (matches gcAction.cpp).
class gcAction : public gcExpression {
public:
    cBase        *mOwner;       // 0x00
    void         *mVTable;      // 0x04
    unsigned int  mNext;        // 0x08

    void Write(cFile &) const;
};

extern "C" {
    void cStrAppend(char *, const char *, ...);
    void cStrCat(char *, const char *);
}

extern const char gcDoPlayerSetEntity_fmt[];        // 0x36EDC0
extern const char gcDoPlayerSetEntity_emptyStr[];   // 0x36DB24

// Type-info slot layout used for the "GetText" / "Write" sub-object dispatch.
typedef void (*EntityWriteFn)(cBase *, cFile *);
typedef void (*EntityGetTextFn)(cBase *, char *);

struct EntityWriteSlot {
    short          mOffset;     // +0
    short          _pad;        // +2
    EntityWriteFn  mFn;         // +4
};

struct EntityGetTextSlot {
    short            mOffset;
    short            _pad;
    EntityGetTextFn  mFn;
};

struct EntityTypeInfoWrite {
    char            _pad[0x28];
    EntityWriteSlot mSlot;       // +0x28
};

struct EntityTypeInfoGetText {
    char              _pad[0x78];
    EntityGetTextSlot mSlot;     // +0x78
};

struct ParamTypeInfoGetText {
    char              _pad[0xD0];
    EntityGetTextSlot mSlot;     // +0xD0
};

class gcDoPlayerSetEntity : public gcAction {
public:
    // 0x0C: gcDesiredValue
    // 0x10: embedded entity base
    // 0x14: pointer to type info
    void Write(cFile &) const;
    void GetText(char *) const;
};

// ── gcDoPlayerSetEntity::Write @ 0x002F1D00 ──
void gcDoPlayerSetEntity::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    ((const gcDesiredValue *)((char *)this + 12))->Write(wb);

    EntityTypeInfoWrite *ti = *(EntityTypeInfoWrite **)((char *)this + 20);
    EntityWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 16);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.End();
}

// ── gcDoPlayerSetEntity::GetText @ 0x002F1FBC ──
void gcDoPlayerSetEntity::GetText(char *buf) const {
    char paramBuf[256];
    char nameBuf[256];
    paramBuf[0] = '\0';
    nameBuf[0] = '\0';
    char *namePtr = nameBuf;

    int val = ((int *)this)[3];

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

    char *eslotP;
    cBase *check = ptr;
    if (check != 0) {
        char *pti = *(char **)((char *)check + 4);
        EntityGetTextSlot *pslot = (EntityGetTextSlot *)(pti + 0xD0);
        pslot->mFn((cBase *)((char *)check + pslot->mOffset), paramBuf);
        eslotP = *(char **)((char *)this + 0x14) + 0x78;
    } else {
        cStrCat(paramBuf, gcDoPlayerSetEntity_emptyStr);
        eslotP = *(char **)((char *)this + 0x14) + 0x78;
    }

    EntityGetTextSlot *eslot = (EntityGetTextSlot *)eslotP;
    cBase *embedded = (cBase *)((char *)this + 0x10);
    eslot->mFn((cBase *)((char *)embedded + eslot->mOffset), namePtr);

    cStrAppend(buf, gcDoPlayerSetEntity_fmt, paramBuf, namePtr);
}
