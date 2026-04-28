// Days of Thunder decompilation
//   gcFormatString::AssignCopy(const cBase *)     @ 0x0027c258
//   gcFormatString::Write(cFile &) const          @ 0x0027c43c
//   gcFormatString::GetName(char *) const         @ 0x0027c94c
//   gcValCameraVariable::GetText(char *) const    @ 0x00322cf8

class cBase;
class cFile;
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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    cBaseArray &operator=(const cBaseArray &);
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

template <class T> T *dcast(const cBase *);

extern "C" {
    void cStrCat(char *, const char *);
}

void cStrAppend(char *, const char *, ...);

class gcStringValue {
public:
    int _v0;
    int _v4;
    void Write(cFile &) const;
};

// ─────────────────────────────────────────────────────────────────────────
// gcFormatString @ 0x0027c258 / 0x0027c43c / 0x0027c94c
// ─────────────────────────────────────────────────────────────────────────

class gcFormatString : public gcStringValue {
public:
    cHandlePairT<gcStringTable, cSubHandleT<gcString> > mPair;  // 0x08
    cBaseArray mArray0;                                          // 0x10
    cBaseArray mArray1;                                          // 0x18

    void GetName(char *) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
};

void gcFormatString::AssignCopy(const cBase *src) {
    gcFormatString *o = dcast<gcFormatString>(src);
    this->mPair.mHandle = o->mPair.mHandle;
    __asm__ volatile("" ::: "memory");
    this->mPair.mSubHandle = o->mPair.mSubHandle;
    this->mArray0 = o->mArray0;
    this->mArray1 = o->mArray1;
}

void gcFormatString::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((cHandle *)&this->mPair.mHandle)->Write(wb);
    wb.Write((unsigned int)this->mPair.mSubHandle.mIndex);
    this->mArray0.Write(wb);
    this->mArray1.Write(wb);
    wb.End();
}

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
