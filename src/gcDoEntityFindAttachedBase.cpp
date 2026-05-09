// gcDoEntityFindAttachedBase.cpp - gcAll_psp.obj

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;
class cReadBlock;

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcExpression {
};

class gcAction : public gcExpression {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

struct ReadRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcEnumeration;
class gcEnumerationEntry;

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

template <class T>
class cArray {
public:
    void Read(cReadBlock &);
};

class gcDoEntityFindAttachedBase : public gcAction {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityFindAttachedBase;

// 0x002afb98 - gcDoEntityFindAttachedBase::GetType(void) const
const cType *gcDoEntityFindAttachedBase::GetType(void) const {
    if (!type_gcDoEntityFindAttachedBase) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894,
                        (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityFindAttachedBase = cType::InitializeType(
            0, 0, 0x157, type_action, 0, 0, 0, 0);
    }
    return type_gcDoEntityFindAttachedBase;
}

// 0x00143d2c - gcDoEntityFindAttachedBase::Write(cFile &) const
void gcDoEntityFindAttachedBase::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((char *)this + 0x14);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    char *base0 = (char *)this + 0x10;
    rec0->fn(base0 + rec0->offset, wb._data[0]);

    wb.Write(*(int *)((char *)this + 0x0C));
    wb.Write(*(int *)((char *)this + 0x3C));
    wb.Write(*(int *)((char *)this + 0x40));
    wb.Write(*(int *)((char *)this + 0x44));

    char *typeInfo1 = *(char **)((char *)this + 0x4C);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    char *base1 = (char *)this + 0x48;
    rec1->fn(base1 + rec1->offset, wb._data[0]);

    void *data = *(void **)((char *)this + 0x60);
    int size = 0;
    if (data != 0) {
        size = ((int *)data)[-1] & 0x3FFFFFFF;
    }
    wb.Write(size);

    data = *(void **)((char *)this + 0x60);
    size = 0;
    if (data != 0) {
        size = ((int *)data)[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < size) {
        char *entry = (char *)data;
        do {
            ((cHandle *)entry)->Write(wb);
            wb.Write(((unsigned int *)entry)[1]);
            i++;
            entry += 8;
        } while (i < size);
    }

    wb.End();
}

// 0x00143e6c - gcDoEntityFindAttachedBase::Read(cFile &, cMemPool *)
int gcDoEntityFindAttachedBase::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2 || gcAction::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos((unsigned int)rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    {
        char *typeInfo = *(char **)((char *)this + 0x14);
        char *base = (char *)this + 0x10;
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = (cFile *)rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x3C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x40, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x44, 4);

    {
        char *typeInfo = *(char **)((char *)this + 0x4C);
        char *base = (char *)this + 0x48;
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = (cFile *)rb[0];
        rec->fn(base + off, f, cMemPool::GetPoolFromPtr(base));
    }

    ((cArray<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x60))
        ->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
