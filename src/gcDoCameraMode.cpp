// gcDoCameraMode.cpp - gcAll_psp.obj

class cBase;
class cFile;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
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

class gcDoCameraMode : public gcAction {
public:
    char _pad0C[0x38];        // 0x0C
    gcDesiredValue mDesired;  // 0x44
    bool mMode;               // 0x48

    void Write(cFile &) const;
};

// 0x002947e8 - gcDoCameraMode::Write(cFile &) const
void gcDoCameraMode::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    char *typeInfo = *(char **)((const char *)this + 0x10);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (void *)((const char *)this + 0x0C);
    rec->fn((char *)base + off, wb._file);

    mDesired.Write(wb);
    wb.Write(mMode);
    wb.End();
}
