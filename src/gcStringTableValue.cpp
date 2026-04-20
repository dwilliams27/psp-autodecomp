// Days of Thunder decompilation: gcStringTableValue methods

class cBase {
public:
    int _b0;
    int _b4;
};

class cFile;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _d1;
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
    cTypeMethod write_m;   // 0x28
    char _p1[0x10];
    cTypeMethod name_m;    // 0x40
};

class gcDesiredObject {
public:
    int _p0;
    cType *mType;
};

class gcString;
class gcStringTableValue;

class gcDesiredString : public gcDesiredObject {
public:
    char _body[0x18];
    gcDesiredString &operator=(const gcDesiredString &);
    gcString *Get(bool) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcStringTableValue : public gcStringValue {
public:
    gcDesiredString mDesired;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void Get(wchar_t *, int) const;
    void GetName(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcString {
public:
    char _pad[0x18];
    const wchar_t *mText;
};

gcStringTableValue *dcast(const cBase *);
void cStrCopy(wchar_t *, const wchar_t *, int);
extern const wchar_t gcStringTableValueEmptyStr[];

typedef void (*WriteFn)(void *, cFile *);
typedef void (*NameFn)(void *, char *);

void gcStringTableValue::GetName(char *buf) const {
    const cTypeMethod *e = &this->mDesired.mType->name_m;
    char *base = (char *)this + 8;
    ((NameFn)e->fn)(base + e->offset, buf);
}

void gcStringTableValue::AssignCopy(const cBase *other) {
    gcStringTableValue *src = dcast(other);
    gcDesiredString &srcDesired = *(gcDesiredString *)((char *)src + 8);
    ((gcDesiredString *)((char *)this + 8))->operator=(srcDesired);
}

void gcStringTableValue::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    this->gcStringValue::Write(file);
    const cTypeMethod *e = &this->mDesired.mType->write_m;
    char *base = (char *)this + 8;
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

void gcStringTableValue::Get(wchar_t *buf, int size) const {
    gcString *p = this->mDesired.Get(true);
    const wchar_t *src;
    if (p != 0) {
        src = (p->mText != 0) ? p->mText : gcStringTableValueEmptyStr;
    } else {
        src = gcStringTableValueEmptyStr;
    }
    cStrCopy(buf, src, size);
}

void gcStringTableValue::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    this->mDesired.VisitReferences(flags, (cBase *)this, cb, user, mask);
}
