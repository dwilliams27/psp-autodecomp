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
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000998F0;

class gcEnumeration : public cObject {
public:
    char _pad[0x44];        // cObject internals
    cBaseArray mArray;      // 0x44 (8 bytes)
    unsigned char mField4C; // 0x4C
    char _pad2[3];
    int mField50;           // 0x50
    cHandle mHandle;        // 0x54

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
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

// ── gcEnumeration::GetType(void) const @ 0x00238e0c ──
const cType *gcEnumeration::GetType(void) const {
    if (D_000998F0 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000998F0 = cType::InitializeType(0, 0, 0xAB, D_000385E4,
                                           &gcEnumeration::New,
                                           (const char *)0x36D8A4,
                                           (const char *)0x36D8B4,
                                           5);
    }
    return D_000998F0;
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
