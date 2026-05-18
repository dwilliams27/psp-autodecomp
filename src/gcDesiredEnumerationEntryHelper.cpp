// gcDesiredEnumerationEntryHelper — gcAll_psp.obj
//   0x0010e450  Write(cWriteBlock &) const  (104B)

class cFile;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(char);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct gcDesiredEnumerationEntryHelper {
    char mField0;
    char mField1;
    cHandle mField4;
    unsigned int mField8;
    void Write(cWriteBlock &) const;
};

// 0x0010e450 — Write(cWriteBlock &) const, 104B
void gcDesiredEnumerationEntryHelper::Write(cWriteBlock &outer) const {
    cWriteBlock wb(*outer._file, 1);
    wb.Write(mField0);
    wb.Write(mField1);
    mField4.Write(wb);
    wb.Write(mField8);
    wb.End();
}
