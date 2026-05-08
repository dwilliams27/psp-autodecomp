class cFile;
class cMemPool;

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);

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
    int Read(cFile &, cMemPool *);
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

int gcStringValue::Read(cFile &file, cMemPool *) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
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
