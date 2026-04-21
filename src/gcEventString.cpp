// Days of Thunder decompilation: gcEventString methods

class cBase {
public:
    int _b0;
    int _b4;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class cFile;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _d1;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcEventString : public gcStringValue {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void Get(wchar_t *, int) const;
    void GetName(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

gcEventString *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
void cStrCopy(wchar_t *, const wchar_t *, int);

extern const char gcEventString_Name[];

struct gcLangCtxEntry {
    char _pad[20];
    const wchar_t *mText;  // 0x14
};

struct gcLangCtx {
    char _pad[12];
    gcLangCtxEntry *mEntry;  // 0xC
};

extern gcLangCtx *gLangCtx;

void gcEventString::AssignCopy(const cBase *other) {
    dcast(other);
}

void gcEventString::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcStringValue::Write(file);
    wb.End();
}

void gcEventString::Get(wchar_t *wbuf, int size) const {
    gcLangCtxEntry *q = 0;
    if (gLangCtx != 0) {
        q = gLangCtx->mEntry;
    }
    cStrCopy(wbuf, q->mText, size);
}

void gcEventString::GetName(char *buf) const {
    cStrAppend(buf, gcEventString_Name);
}

void gcEventString::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)(void *)this, arg3);
    }
}
