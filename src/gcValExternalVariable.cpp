// gcValExternalVariable — gcLValue subclass wrapping a cHandle reference
// to an external (designer-named) variable.
//
// Functions matched here:
//   gcValExternalVariable::Write(cFile &) const          @ 0x003430ac  (gcAll_psp.obj)

class cFile;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcLValue {
public:
    void Write(cFile &) const;
};

class gcValExternalVariable : public gcLValue {
public:
    void Write(cFile &) const;
};

// 0x003430ac (88B) — Write
void gcValExternalVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    ((const cHandle *)((const char *)this + 8))->Write(wb);
    wb.End();
}
