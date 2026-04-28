// gcEnumeration.cpp — decompiled from gcAll_psp.obj.
// Parent: cObject. Same shape as gcStringTable / gcConfiguration but with
// extra payload — cBaseArray at 0x44, u8 at 0x4C, int at 0x50, cHandle at 0x54.
//
// Functions:
//   0x000d3eb4 gcEnumeration::Write(cFile &) const            124B
//   0x00238d04 gcEnumeration::AssignCopy(const cBase *)       108B

#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned char);
    void Write(int);
    void End(void);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

template <class T> T *dcast(const cBase *);

class gcEnumeration : public cObject {
public:
    char _pad[0x44];        // cObject internals
    cBaseArray mArray;      // 0x44 (8 bytes)
    unsigned char mField4C; // 0x4C
    char _pad2[3];
    int mField50;           // 0x50
    cHandle mHandle;        // 0x54

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

// ── gcEnumeration::AssignCopy(const cBase *) @ 0x00238d04 ──
void gcEnumeration::AssignCopy(const cBase *src) {
    gcEnumeration *other = dcast<gcEnumeration>(src);
    cObject::operator=(*other);
    mArray = other->mArray;
    mField4C = other->mField4C;
    mField50 = other->mField50;
    mHandle = other->mHandle;
}

// ── gcEnumeration::Write(cFile &) const @ 0x000d3eb4 ──
void gcEnumeration::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    cObject::Write(file);
    mArray.Write(wb);
    wb.Write(mField4C);
    wb.Write(mField50);
    mHandle.Write(wb);
    wb.End();
}
