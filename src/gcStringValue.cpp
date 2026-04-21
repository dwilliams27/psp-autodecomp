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
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern "C" {
    void cStrCat(char *, const char *);
}

class gcStringValue {
public:
    void Get(cHandlePairT<gcStringTable, cSubHandleT<gcString> > *) const;
    void Write(cFile &) const;
};

class gcTableColumn {
public:
    void Write(cFile &) const;
};

class gcPartialEntityController {
public:
    void Write(cFile &) const;
};

class gcSubGeomController {
public:
    void Write(cFile &) const;
};

class gcDoSwitch {
public:
    void GetText(char *) const;
};

void gcStringValue::Get(cHandlePairT<gcStringTable, cSubHandleT<gcString> > *out) const {
    out->mHandle.mIndex = 0;
    cSubHandleT<gcString> *p = &out->mSubHandle;
    p->mIndex = 0;
}

void gcStringValue::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

void gcTableColumn::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

void gcPartialEntityController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

void gcSubGeomController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

void gcDoSwitch::GetText(char *buf) const {
    cStrCat(buf, "away from %s )");
}
