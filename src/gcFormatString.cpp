// Days of Thunder decompilation
//   gcFormatString::GetName(char *) const         @ 0x0027c94c
//   gcValCameraVariable::GetText(char *) const    @ 0x00322cf8

class gcStringTable;
class gcString;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
    const char *GetName(char *, bool, char *) const;
};

extern "C" {
    void cStrCat(char *, const char *);
}

void cStrAppend(char *, const char *, ...);

// ─────────────────────────────────────────────────────────────────────────
// gcFormatString::GetName(char *) const @ 0x0027c94c
// ─────────────────────────────────────────────────────────────────────────

class gcFormatString {
public:
    int _b0;
    int _b4;
    cHandlePairT<gcStringTable, cSubHandleT<gcString> > mPair;  // 0x08
    void GetName(char *) const;
};

void gcFormatString::GetName(char *buf) const {
    char local[4096];
    this->mPair.GetName(local, false, (char *)1);
    cStrCat(buf, local);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValCameraVariable::GetText(char *) const @ 0x00322cf8
// ─────────────────────────────────────────────────────────────────────────

class gcDesiredCamera {
public:
    void GetText(char *) const;
};

extern const char gcValCameraVariable_fmt[];   // @ 0x36DCB8
extern const char gcValGetText_text[];         // @ 0x36DAF0

class gcValCameraVariable {
public:
    int _b0;
    int _b4;
    gcDesiredCamera mDesired;  // 0x08
    void GetText(char *) const;
};

void gcValCameraVariable::GetText(char *buf) const {
    this->mDesired.GetText(buf);
    cStrAppend(buf, gcValCameraVariable_fmt, gcValGetText_text);
}
