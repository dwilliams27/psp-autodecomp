// eGeomMtl.cpp — decompiled from eAll_psp.obj.
// Parent: eMaterial.
//
// Functions:
//   0x0002CBD8 eGeomMtl::Write(cFile &) const     372B
//   0x0002CE90 eGeomMtl::eGeomMtl(cBase *)         84B

class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(char);
    void Write(unsigned char);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    void *mData;
};

class eMaterial {
public:
    char _pad[0x5C];

    eMaterial(cBase *);
    void Write(cFile &) const;
};

class eGeomMtl : public eMaterial {
public:
    unsigned char mField5C;                        // 0x5C
    unsigned char mField5D;                        // 0x5D
    unsigned char mField5E;                        // 0x5E
    signed char mField5F;                          // 0x5F
    cArrayBase<cHandleT<eMaterial> > mField60;    // 0x60
    cArrayBase<cHandleT<eMaterial> > mField64;    // 0x64

    eGeomMtl(cBase *);
    void Write(cFile &) const;
};

extern char eGeomMtlvirtualtable[];

// ── eGeomMtl::Write @ 0x0002CBD8 ──
void eGeomMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    eMaterial::Write(file);
    wb.Write(*(const signed char *)((const char *)this + 0x5F));
    wb.Write(*(const unsigned char *)((const char *)this + 0x5C));
    wb.Write(*(const unsigned char *)((const char *)this + 0x5D));
    wb.Write(*(const unsigned char *)((const char *)this + 0x5E));

    int *handles = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))->mData;
    int count0 = 0;
    if (handles != 0) {
        count0 = handles[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int count = 0;
    handles = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x60))->mData;
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }

    int *baseHandles = handles;
    int i = 0;
    if (i < count) {
        int offset = 0;
        int *handle = baseHandles + offset;
        do {
            ((cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    int *handles2 = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))->mData;
    int count1 = 0;
    if (handles2 != 0) {
        count1 = handles2[-1] & 0x3FFFFFFF;
    }
    wb.Write(count1);

    int count2 = 0;
    handles2 = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x64))->mData;
    if (handles2 != 0) {
        count2 = handles2[-1] & 0x3FFFFFFF;
    }

    int *baseHandles2 = handles2;
    int j = 0;
    if (j < count2) {
        int offset2 = 0;
        int *handle2 = baseHandles2 + offset2;
        do {
            ((cHandle *)handle2)->Write(wb);
            j++;
            handle2++;
        } while (j < count2);
    }

    wb.End();
}

// ── eGeomMtl::eGeomMtl @ 0x0002CE90 ──
eGeomMtl::eGeomMtl(cBase *parent) : eMaterial(parent) {
    *(void **)((char *)this + 4) = eGeomMtlvirtualtable;
    mField5C = 0;
    mField5D = 1;
    mField5E = 0;
    mField5F = 2;
    mField60.mData = 0;
    mField64.mData = 0;
}
