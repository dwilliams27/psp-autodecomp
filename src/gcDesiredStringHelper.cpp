// gcDesiredStringHelper.cpp — decompiled from gcAll_psp.obj
//
//   0x00129e0c  Write(cWriteBlock &) const   (104B)
//
// Wrapper around a small payload: 2 chars, a cHandle (offset 4), and a u32
// (offset 8). Wraps payload in a nested cWriteBlock with type-id 3.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(char);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcDesiredStringHelper {
public:
    char         mC0;
    char         mC1;
    char         _pad2[2];
    cHandle      mH4;
    unsigned int mU8;

    void Write(cWriteBlock &) const;
};

// ── 0x00129e0c — Write(cWriteBlock &) const, 104B ──
void gcDesiredStringHelper::Write(cWriteBlock &arg_wb) const {
    cWriteBlock wb(*reinterpret_cast<cFile *>(arg_wb._data[0]), 3);
    wb.Write(mC0);
    wb.Write(mC1);
    mH4.Write(wb);
    wb.Write(mU8);
    wb.End();
}
