// gcTimerGroup trampolines and small methods.

class cBase;
class cFile;

class cGroup {
public:
    void Write(cFile &) const;
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcTimerGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);

void gcTimerGroup::AssignCopy(const cBase *base) {
    gcTimerGroup *src = dcast<gcTimerGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcTimerGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void gcTimerGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cGroup *)this)->Write(file);
    wb.End();
}
