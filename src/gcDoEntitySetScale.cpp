// gcDoEntitySetScale.cpp - gcAll_psp.obj

class cBase;
class cFile;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    char _pad[12];

    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    int mField0;
    void Write(cWriteBlock &) const;
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntitySetScale : public gcAction {
public:
    char _pad0C[0x2C];        // 0x0C
    gcDesiredValue mDesired;  // 0x38
    int mScaleType;           // 0x3C

    void Write(cFile &) const;
};

// 0x002cbc2c - gcDoEntitySetScale::Write(cFile &) const
void gcDoEntitySetScale::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo = *(char **)((const char *)this + 0x10);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (void *)((const char *)this + 0x0C);
    rec->fn((char *)base + off, wb._file);

    mDesired.Write(wb);
    wb.Write(mScaleType);
    wb.End();
}
