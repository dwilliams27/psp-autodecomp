// gcDesiredObject.cpp — decompiled from gcAll_psp.obj.
//
// Functions in this file:
//   0x0011b578 gcDesiredObject::Write(cFile &) const             100B
//   0x0025f28c gcDesiredObject::GetType(void) const              152B
//   0x0025f324 gcDesiredObject::GetName(char *) const            144B

class cBase;
class cFile;
class cMemPool;
class cType;

extern "C" int cStrAppend(char *, const char *, ...);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;   // shared cBase root type cache
extern cType *D_0009F3F4;   // gcDesiredObject type cache

struct gcDesiredObjectCastSlot {
    short mOffset;                              // +0
    short _pad;                                 // +2
    cBase *(*mCast)(cBase *);                   // +4
};

struct gcDesiredObjectGetNameSlot {
    short mOffset;                              // +0
    short _pad;                                 // +2
    void (*mGetName)(cBase *, char *);          // +4
};

struct gcDesiredObjectTypeInfo {
    char _pad[0x78];                            // +0x00
    gcDesiredObjectGetNameSlot mInner;          // +0x78 — parent GetName slot
    gcDesiredObjectCastSlot    mOwner;          // +0x80 — named-object cast slot
};

struct cBaseWithName {
    char _pad[12];                              // +0x00
    const char *mName;                          // +0x0C
};

class gcDesiredObject {
public:
    cBase *mParent;                             // +0x00
    gcDesiredObjectTypeInfo *mTypeInfo;         // +0x04
    int mField8;                                // +0x08

    void Write(cFile &) const;
    const cType *GetType(void) const;
    void GetName(char *) const;
};

// ── gcDesiredObject::Write @ 0x0011b578 ──
void gcDesiredObject::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    int val = mField8;
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

// ── gcDesiredObject::GetType @ 0x0025f28c ──
const cType *gcDesiredObject::GetType(void) const {
    if (D_0009F3F4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F3F4 = cType::InitializeType(0, 0, 0x12C, D_000385DC, 0, 0, 0, 0);
    }
    return D_0009F3F4;
}

// ── gcDesiredObject::GetName @ 0x0025f324 ──
void gcDesiredObject::GetName(char *dst) const {
    gcDesiredObjectCastSlot *owner = &mTypeInfo->mOwner;
    const char *fmt = (const char *)0x36DCE4;
    cBase *obj = owner->mCast((cBase *)((char *)this + owner->mOffset));
    cStrAppend(dst, fmt, ((cBaseWithName *)obj)->mName);

    gcDesiredObjectGetNameSlot *inner = &mTypeInfo->mInner;
    inner->mGetName((cBase *)((char *)this + inner->mOffset), dst);

    cStrAppend(dst, (const char *)0x36DCEC);
}
