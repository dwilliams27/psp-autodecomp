// gcValEntityIsActive — gcValue subclass holding an entity reference whose
// "is active" flag is read at evaluation time.
//
// Functions matched here:
//   gcValEntityIsActive::Write(cFile &) const   @ 0x00332610  (gcAll_psp.obj)
//   gcValEntityIsActive::GetText(char *) const  @ 0x003327d0  (gcAll_psp.obj)
//
// The class layout (relevant subset):
//   0x00..0x07  gcValue base (mParent, mClassdesc)
//   0x08..0x0f  gcDesiredEntity { _parent, mType }  ← reflective member
//
// Both methods do reflective dispatch through gcDesiredEntity::mType: the
// cType holds entries at 0x28 (write) and 0x78 (text), each a {short offset,
// short pad, void *fn} record. The offset re-bases the call inside the
// gcDesiredEntity payload.

class cFile;
class cBase;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
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
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityIsActive : public gcValue {
public:
    void Write(cFile &) const;
    void GetText(char *) const;
};

extern "C" void cStrCat(char *, const char *);

extern const char gcValEntityIsActive_text[];   // @ 0x36F2DC

// 0x00332610 (108B) — Write
void gcValEntityIsActive::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

// 0x003327d0 (80B) — GetText
void gcValEntityIsActive::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrCat(buf, gcValEntityIsActive_text);
}
