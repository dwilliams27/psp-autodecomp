// gcDesiredEntityHelper — gcAll_psp.obj
//   0x0011d15c  Write(cWriteBlock &) const  (104B)

class cFile;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(char);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct gcDesiredEntityHelper {
    char mField0;
    char mField1;
    char mField2;
    cHandle mField4;
    void Write(cWriteBlock &) const;
};

// 0x0011d15c — Write(cWriteBlock &) const, 104B
void gcDesiredEntityHelper::Write(cWriteBlock &outer) const {
    cWriteBlock wb(*outer._file, 2);
    wb.Write(mField0);
    wb.Write(mField1);
    wb.Write(mField2);
    mField4.Write(wb);
    wb.End();
}
