// Multi-class wrapper file for small AssignCopy / GetText / ctor methods.
// Functions:
//   gcExternalCinematicGroup::AssignCopy(const cBase *) @ 0x00238834
//   gcValCameraIsValid::GetText(char *) const          @ 0x00323334
//   gcValConstant::AssignCopy(const cBase *)           @ 0x0032535c
//   gcValVariable::AssignCopy(const cBase *)           @ 0x0036a9a8
//   eFrameBufferTexture::eFrameBufferTexture(cBase *)  @ 0x00081718

class cBase;

void cStrAppend(char *, const char *, ...);

extern const char gcValCameraIsValid_text[];   // @ 0x36F18C

// ─────────────────────────────────────────────────────────────────────────
// gcExternalCinematicGroup::AssignCopy(const cBase *) @ 0x00238834
// ─────────────────────────────────────────────────────────────────────────

template <class T> T *dcast(const cBase *);

class gcExternalCinematicGroup {
public:
    char _pad0[8];
    unsigned char f8;
    char _pad9[3];
    int fC;
    void AssignCopy(const cBase *);
};

void gcExternalCinematicGroup::AssignCopy(const cBase *base) {
    gcExternalCinematicGroup *other = dcast<gcExternalCinematicGroup>(base);
    this->f8 = other->f8;
    this->fC = other->fC;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValCameraIsValid::GetText(char *) const @ 0x00323334
// ─────────────────────────────────────────────────────────────────────────

class gcDesiredCamera {
public:
    void GetText(char *) const;
};

class gcValCameraIsValid {
public:
    char _pad0[8];
    gcDesiredCamera f8;
    void GetText(char *) const;
};

void gcValCameraIsValid::GetText(char *buf) const {
    this->f8.GetText(buf);
    cStrAppend(buf, gcValCameraIsValid_text);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValConstant::AssignCopy(const cBase *) @ 0x0032535c
// ─────────────────────────────────────────────────────────────────────────

class gcValConstant {
public:
    char _pad0[8];
    int f8;
    void AssignCopy(const cBase *);
};

void gcValConstant::AssignCopy(const cBase *base) {
    gcValConstant *other = dcast<gcValConstant>(base);
    int *src = (int *)((char *)other + 8);
    int *dst = (int *)((char *)this + 8);
    *dst = *src;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValVariable::AssignCopy(const cBase *) @ 0x0036a9a8
// ─────────────────────────────────────────────────────────────────────────

class gcValVariable {
public:
    char _pad0[8];
    int f8;
    void AssignCopy(const cBase *);
};

void gcValVariable::AssignCopy(const cBase *base) {
    gcValVariable *other = dcast<gcValVariable>(base);
    int *src = (int *)((char *)other + 8);
    int *dst = (int *)((char *)this + 8);
    *dst = *src;
}

// ─────────────────────────────────────────────────────────────────────────
// eFrameBufferTexture::eFrameBufferTexture(cBase *) @ 0x00081718
// ─────────────────────────────────────────────────────────────────────────

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
};

class eFrameBufferTexture : public eVirtualTexture {
public:
    eFrameBufferTexture(cBase *);
};

extern char eFrameBufferTextureclassdesc[];

eFrameBufferTexture::eFrameBufferTexture(cBase *parent) : eVirtualTexture(parent) {
    ((void **)this)[1] = eFrameBufferTextureclassdesc;
    __asm__ volatile("" ::: "memory");
    ((unsigned char *)this)[0x50] = 1;
}
