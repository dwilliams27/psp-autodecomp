// gcDoEntitySetLookAtTarget.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    void Write(cFile &) const;
};

class gcDoEntitySetLookAtTarget : public gcAction {
public:
    gcDoEntitySetLookAtTarget(cBase *);

    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

cBase *gcDoEntitySetLookAtTarget::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySetLookAtTarget *result = 0;
    gcDoEntitySetLookAtTarget *obj =
        (gcDoEntitySetLookAtTarget *)rec->fn(base, 0x9C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySetLookAtTarget(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcDoEntitySetLookAtTarget::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    char *typeInfo1 = *(char **)((const char *)this + 0x3C);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x38;
    rec1->fn((char *)base1 + off1, wb._file);

    wb.Write(*(int *)((const char *)this + 0x50));
    ((const cHandle *)((const char *)this + 0x54))->Write(wb);

    char *typeInfo2 = *(char **)((const char *)this + 0x5C);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x58;
    rec2->fn((char *)base2 + off2, wb._file);

    ((const cName *)((const char *)this + 0x84))->Write(wb);
    wb.End();
}
