class cBase;
class cFile;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcLValue {
public:
    void Write(cFile &) const;
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

typedef void (*DesiredWriteFn)(void *, cFile *);

struct DesiredWriteSlot {
    short mOffset;
    short _pad;
    DesiredWriteFn mFn;
};

struct DesiredTypeInfoWrite {
    char _pad[0x28];
    DesiredWriteSlot mSlot;
};

class gcValRigidBodyControllerVariable {
public:
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

class gcValTableEntry {
public:
    void Write(cFile &) const;
};

struct SixInts {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
};

template <class T>
T dcast(const cBase *);

void gcValRigidBodyControllerVariable::AssignCopy(const cBase *base) {
    gcValRigidBodyControllerVariable *other = dcast<gcValRigidBodyControllerVariable *>(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 0x34) = *(const int *)((const char *)other + 0x34);
    *(SixInts *)((char *)this + 0x38) = *(const SixInts *)((const char *)other + 0x38);
}

void gcValRigidBodyControllerVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcLValue *)this)->Write(file);

    DesiredTypeInfoWrite *type = *(DesiredTypeInfoWrite **)((const char *)this + 0x0C);
    DesiredWriteSlot *slot = &type->mSlot;
    const char *desired = (const char *)this + 0x08;
    slot->mFn((void *)(desired + slot->mOffset), wb._file);

    wb.Write(*(const int *)((const char *)this + 0x34));
    ((const cName *)((const char *)this + 0x38))->Write(wb);
    wb.End();
}

void gcValTableEntry::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcLValue *)this)->Write(file);

    DesiredTypeInfoWrite *type = *(DesiredTypeInfoWrite **)((const char *)this + 0x0C);
    DesiredWriteSlot *slot = &type->mSlot;
    const char *desired = (const char *)this + 0x08;
    slot->mFn((void *)(desired + slot->mOffset), wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x20))->Write(wb);
    wb.End();
}
