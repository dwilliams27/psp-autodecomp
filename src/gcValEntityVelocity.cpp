// gcValEntityVelocity — gcValue subclass holding an entity reference whose
// velocity component is read at evaluation time.
//
// Functions matched here:
//   gcValEntityVelocity::Write(cFile &) const            @ 0x00341274  (gcAll_psp.obj)
//   gcValEntityVelocity::GetText(char *) const           @ 0x00341720  (gcAll_psp.obj)

class cFile;
class cBase;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];
    cTypeMethod text_m;     // 0x78
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityVelocity : public gcValue {
public:
    void Write(cFile &) const;
    void GetText(char *) const;
};

void cStrAppend(char *, const char *, ...);

extern const char gcValEntityVelocity_text_fmt[];   // @ 0x36DCB8
extern const char gcValEntityVelocity_text_arg[];   // @ 0x36DAF0

// 0x00341274 (120B) — Write
void gcValEntityVelocity::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 12))->mType + 40);
    char *base = (char *)this + 12;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.Write(*(const int *)((const char *)this + 8));
    wb.End();
}

// 0x00341720 (88B) — GetText
void gcValEntityVelocity::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 12))->mType + 120);
    char *base = (char *)this + 12;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf,
               gcValEntityVelocity_text_fmt,
               gcValEntityVelocity_text_arg);
}
